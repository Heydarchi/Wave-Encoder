/*
 * EncoderClass.cpp
 *
 *  Created on: May 7, 2020
 *      Author: M.H.H
 */

#include "EncoderClass.h"
#include <iostream>
#include <cstdlib>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <thread>
#include "WaveHelper.h"
#include <vector>
#include "lame.h"

using namespace std;
using std::string;

struct ParamsStruct{
			unsigned int currentFileNameIndex=0;
			int fileListCount;
			int thrdIndex;
	}params;
int thrdAliveCnt=0;
vector<string> nameList;				//Includes wave file name required to encode
pthread_mutex_t mutexLock;				//Mutex lock Global variable

typedef void * (*THREADFUNCPTR)(void *);


/**
 * Constructor of class initiating some variable  
 *
 * @file_name string as a paremeter sent to function that is stored in waveFileName
 */
EncoderClass::EncoderClass(vector<string> file_name_list) {
	// TODO Auto-generated constructor stub
	nameList=file_name_list;
	params.fileListCount=file_name_list.size();
	params.currentFileNameIndex=0;
	//Get Cpu count
	getCpuInfo();
	//mutex lock variable initializing
	pthread_mutex_init(&mutexLock,NULL);
	if(DEBUG_EN)
		cout<<"Class initialized !"<<params.fileListCount<<endl;
}

/**
 * Deconstructor of class  
 *
 */
EncoderClass::~EncoderClass() {
	// TODO Auto-generated destructor stub
}


/**
 * Initlize the lame pointer and also some paramters  
 *
 * @gfp lame_global_flags type used for working with lame library
 * @ch_num uint16_t type including Channel Number
 * @in_srate uint32_t type including input PCM data Sample Rate
 * @return int tpe return result of paramaters initialization
*/
int EncoderClass::initLame(lame_global_flags **gfp, uint16_t ch_num,uint32_t in_srate) {
	// TODO the lame parameters initialization

	//lame pointer initialization
    *gfp = lame_init();
	// input channel = Stereo or Mono
	lame_set_num_channels(*gfp,ch_num); 
	//set input PCM data sample rate
    lame_set_in_samplerate(*gfp,in_srate);
	//set lame output bitrate
    lame_set_brate(*gfp,128);
	//set number of channels to lame
	/*if(ch_num==STEREO_W)
		lame_set_mode(*gfp, STEREO ); // Streo or Mono
	else
		lame_set_mode(*gfp, MONO ); // Streo or Mono*/
	//set lame quality
    lame_set_quality(*gfp,LAME_GOOD_QUALITY);
    int ret_code = lame_init_params(*gfp);
	return ret_code;
}


/**
 * Dispose the lame pointer
 *
*/
void EncoderClass::disposeLame(lame_global_flags **gfp){
	lame_close(*gfp);
}

/**
 * Start Encoding process by creating number of thread based on cpu core numbers
 *
*/
void EncoderClass::startEncodingProgress() {
	// TODO StartEncodingProgress
	if(DEBUG_EN_DETAIL)
		cout<<"StartEncodingProgress "<<params.fileListCount<<endl;
	//declare an array of pthread to handle thread that will be created
	pthread_t enc_thrd[maxConcurrentThread];
	for(int i=0;i<maxConcurrentThread;i++)
		{
			//Create thread
		     pthread_create( &enc_thrd[i], NULL,     (THREADFUNCPTR)&EncoderClass::encodeWaveFileToMP3  ,(void*)&i);
		}

	if(DEBUG_EN_DETAIL)
		cout<<"All threads joining !!!"<<endl;
	
	//Join thread to assure all threads was finished before finishing application
    for(int i=0;i<maxConcurrentThread;i++)
		 pthread_join(enc_thrd[i],NULL);

    if(DEBUG_EN)
		cout<<"All threads were Terminated !"<<endl<< std::flush;

}


/**
 * This function encodes the wave file to mp3 format called as a seprated thread from startEncodingProgress
 *
*/
void* EncoderClass::encodeWaveFileToMP3(void* index) {
	// TODO EncodeWaveToMP3
	int thrd_id=thrdAliveCnt;
	//Increase value if variable containing created thread numbers
    thrdAliveCnt++;
    if(DEBUG_EN)
		cout<<" Thread No "<<thrd_id<<" Started Successfully"<<endl;
    string ret_file_name;
	//Get a wave file name from the list
	ret_file_name=getNextFileName();
	//Itterate while the file name is valid ,  NULL is invalid name
   while( ret_file_name.compare("NULL")!=0 ){
		if(DEBUG_EN_DETAIL)
			cout<<"Thread :"<<thrd_id<<" ret file name :"<<ret_file_name<<endl;
		//Check Whether the file name is valid or not,  NULL is invalid name
		if(ret_file_name.compare("NULL")!=0)
		{
			uint32_t read_size=0;
			//Buffer to store the read PCM data
			short int interleaved_data[WAVE_BUF_SIZE*2];
			//Buffer to store encoded PCM data as a mp3 data 
			short int mp3_data[WAVE_BUF_SIZE*2];
			//Create a new Wave object
			WaveHelperClass wave_obj(ret_file_name);
			//Check if file format is Wave
		    string output_fn(ret_file_name);
			//Check whether the file is in Wave format or not
			if(wave_obj.waveHeader.Format==WAVE && wave_obj.waveHeader.BitsPerSample==16)
			{
				lame_global_flags *gfp;
				//Initialization of lame pointer
				int ret_init=initLame(&gfp,wave_obj.waveHeader.NumChannels,wave_obj.waveHeader.SampleRate);
				if(DEBUG_EN_DETAIL)
					cout<<"Init lame parameters : "<<ret_init<<endl;
				int ret=0;
				//Edit the file name to make mp3 target file name
			    output_fn.replace(  output_fn.find(".wav") ,output_fn.find(".wav")+4,".mp3");
				//new vaiable for writing mp3 data
			    std::ofstream if_mp3;
				//Open a new handle to new file to write encoded data as a mp3 file
			    if_mp3.open(output_fn, std::ios::binary|std::ios::out);
				//store the buf size in based on char tpe a variable
			    int buf_size_type=(sizeof(short int)/sizeof(char));
				do{
					int ret_mp3 =0;
					ret=wave_obj.readPcmData_Interleaved(interleaved_data,
								(int)( buf_size_type * 2 * WAVE_BUF_SIZE ) );
					if(ret>0)
					{
						//Check if the wave file is Stereo or not
						if(wave_obj.waveHeader.NumChannels==STEREO_W)
						{
							///encode the pcm data, this function get interleaved data
							ret_mp3 = lame_encode_buffer_interleaved(gfp,
								interleaved_data, ret/(buf_size_type*wave_obj.waveHeader.NumChannels),
								 (unsigned char*)mp3_data, WAVE_BUF_SIZE*2*buf_size_type);

						}
						else if(wave_obj.waveHeader.NumChannels==MONO_W)
						{
							///encode the pcm data, sice we have one channel replicate one channel to second channel
							//and also before MONO was set to lame parameters
							ret_mp3 = lame_encode_buffer(gfp,
								interleaved_data,interleaved_data, ret/(buf_size_type*wave_obj.waveHeader.NumChannels),
								 (unsigned char*)mp3_data, WAVE_BUF_SIZE*2*buf_size_type);
						}
					}
					else
					{
						do
						{
							//flush last frame from encoder
							ret_mp3 = lame_encode_flush(gfp,
									 (unsigned char*)mp3_data, WAVE_BUF_SIZE*2*buf_size_type);
						}while(ret_mp3>0);
					}
					//Write encoded data to mp3 file
					if_mp3.write((char *)mp3_data,ret_mp3);
					if(DEBUG_EN_DETAIL)
						cout<<" Write "<<ret_mp3 <<endl;
					read_size+=ret;
				}while( ret>0 );
				
				//Get mp3 tags to write at first of the mp3 file
				ret=lame_get_lametag_frame(gfp,(unsigned char*)mp3_data, WAVE_BUF_SIZE*2*buf_size_type);
				//Check if the function returned mp3 tag data
				if(ret>0)
				{
					//Seek start of file 
					if_mp3.seekp(0);
					//Write mp3 tag data to file
					if_mp3.write((char *)mp3_data,ret);
				}
				//Close handle of generated mp3 file 
				if_mp3.close();
				if(DEBUG_EN_DETAIL)
					cout<<" Last Read "<<ret<<" bytes from "<< ret_file_name <<endl;

				if(DEBUG_EN_DETAIL)
					cout<<" Total Read : "<<read_size<<" bytes from "<< ret_file_name<<" Subchunk2Size :"<< wave_obj.waveDataSubHeader.Subchunk2Size<<endl;
			    // FILE* f_mp3 = if_mp3.fd();
				//lame_mp3_tags_fid(gfp,f_mp3);
				//f_mp3=null;
				disposeLame(&gfp);
				//if(DEBUG_EN)
					cout<<"Thread :"<<thrd_id<<" Output file is generated : "<<output_fn<<endl;
			}
			//Clsoe handle of wave file
			wave_obj.closeWaveFile();
			if(wave_obj.waveHeader.Format==WAVE && wave_obj.waveHeader.BitsPerSample!=16)
					cout<<"!!!   Thread :"<<thrd_id<<" File [ "<<output_fn << " ] was rejected because PCM data is not in 16 bit format " <<endl;
			if(wave_obj.waveHeader.Format!=WAVE )
					cout<<"!!!   Thread :"<<thrd_id<<" File [ "<<output_fn << " ] was rejected because it is not WAVE " <<endl;

		}
		//Get the next wave file name from list
		ret_file_name=getNextFileName();
	}
	if(DEBUG_EN)
		cout<<endl<<"Exit Thread :"<<thrd_id<<endl<< std::flush;
    thrdAliveCnt--;

	return NULL;
}


/**
 * Get Cpu Core numbers
 *
*/
int EncoderClass::getCpuInfo() {
	// TODO GetCpuInfo
	maxConcurrentThread=std::thread::hardware_concurrency();
	if(DEBUG_EN)
		cout<<endl<<"Number of CPU Cores : "<<maxConcurrentThread<<endl<< std::flush;
	return maxConcurrentThread;
}


/**
 * Start Encoding process by creating number of thread based on cpu core numbers
 *@return string type including the nest wave file name, if "NULL" it means that there is no more file to encode
*/
string EncoderClass::getNextFileName() {
	// TODO GetNextFileName
	string fn;
	//Lock the mutex to assure that it can not be called before exiting from locked scope
    pthread_mutex_lock(&mutexLock);
    if(DEBUG_EN_DETAIL)
    {
    	cout<<"Get next file name :"<<params.currentFileNameIndex << nameList.size()<<endl;
    }
	//return NULL if no more file name remained
	if( params.currentFileNameIndex >= nameList.size() )
	{
		fn= "NULL";
	}
	else
	{
		//Get next file name
		fn=nameList.at(params.currentFileNameIndex);
		//Increase indx variabel of wave files vector
		params.currentFileNameIndex++;
	}
	//unclock the  mutex 
	pthread_mutex_unlock(&mutexLock);
	return fn;
}


