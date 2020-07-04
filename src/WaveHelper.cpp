#include <iostream>
#include <cstdlib>
#include <sys/sysinfo.h>
#include <string.h>
#include <stdio.h>
#include "WaveHelper.h"
#include <sys/stat.h>
#include <cerrno>
#include <inttypes.h>

using namespace std;
using std::exception;
using std::string;



/**
 * Constructor of class opening a handle to wave file and then call a function to read it heade  
 *
 * @file_name string as a paremeter sent to function that is stored in waveFileName
 */
WaveHelperClass::WaveHelperClass(string file_name) {
	// TODO Auto-generated constructor stub
	waveFileName= file_name;
	if(DEBUG_EN_DETAIL)
		cout<<"WaveHelper Class initialized for		=>  "<<waveFileName<<endl;
	//Open handle to wave file 
	openWaveFile();
	//Read wave file header
	readWaveHeader();
}


/**
 * Deconstructor of class closing the handle to wave file 
 *
 * @return int as the result of function, 0 == return => Successful , 0 !=return => failed
 */
WaveHelperClass::~WaveHelperClass() {
	// TODO Auto-generated destructor stub
	if(DEBUG_EN_DETAIL)
		cout<<"WaveHelperClass Disposed ! "<<waveFileName<<endl;
	//Close handle to wave file opened by openWaveFile()
	closeWaveFile();
}

/**
 * Open a handle to wave file 
 *
 * @return int as the result of function, 0 == return => Successful , 0 !=return => failed
 */
int WaveHelperClass::openWaveFile()
{
	try {
		ifWav.open(waveFileName, std::ios_base::binary|std::ifstream::in);
		if(DEBUG_EN_DETAIL)
			cout << waveFileName<<" Opened ! " <<ifWav.good()<<'\n';
		//return 0 if the state of the handke is good
		if (ifWav.good()) {
				return 0;
			}
			//return dailed
		return 1;
	}
	catch (std::ifstream::failure e) {
		cout << "Error opening input file: " << waveFileName<< "  :  " <<strerror(errno) << '\n';
		//return exception
		return 2;
	}
}

/**
 * Close the handle to wave file 
 *
 * @return int as the result of function, 0 == return => Successful , 0 !=return => failed
 */
 int WaveHelperClass::closeWaveFile()
{
	try {
		//Check if the handle is open
		if(ifWav.is_open())
			ifWav.close();
		if(DEBUG_EN_DETAIL)
			cout << waveFileName<<" Closed ! "<<endl;
		return 0;
	}
	catch (std::ifstream::failure e) {
		cout << "Error Closing input file: " << waveFileName<< "  :  " <<strerror(errno) << '\n';
		return 1;
	}
}


/**
 * Read header of wave file included in waveFileName and stores data to appropriare varables
 *
 */
int WaveHelperClass::readWaveHeader()
{
		try {

			if(DEBUG_EN_DETAIL)
				cout << waveFileName<<" ReadWaveHeader " <<ifWav.good()<<'\n';
			if (ifWav.good()) {
					//Read some bytes from wave file header and casts them to waveHeader
					ifWav.read(reinterpret_cast<char*>(&waveHeader), sizeof(waveHeader));
					//convert to big endian format
					waveHeader.ChunkID = __builtin_bswap32(waveHeader.ChunkID);
					//convert to big endian format
					waveHeader.Format = __builtin_bswap32(waveHeader.Format);
					//convert to big endian format
					waveHeader.Subchunk1ID = __builtin_bswap32(waveHeader.Subchunk1ID);
					//check size of subchunk1 to find start of data header 
					if((waveHeader.Subchunk1Size-DEF_SUBCHUNK_SIZE) > 0)
					{
						char _bf[waveHeader.Subchunk1Size-DEF_SUBCHUNK_SIZE];
						ifWav.read(_bf, waveHeader.Subchunk1Size-DEF_SUBCHUNK_SIZE);
					}
					//read some bytes (Wave File Sub chunk Data Header) from wave file and cast them to waveDataSubHeader
					ifWav.read(reinterpret_cast<char*>(&waveDataSubHeader), DATA_SUBCHUNK_SIZE);
					//convert to big endian format
					waveDataSubHeader.Subchunk2ID = __builtin_bswap32(waveDataSubHeader.Subchunk2ID);
					if(DEBUG_EN)
					{
						cout<<endl<<endl<<" ***************************************************** "<<endl;
						cout<<"			"<<waveFileName<<endl<<endl;
						cout<<" ChunkID : "<<(waveHeader.ChunkID==RIFF?"RIFF":"OTHERS")<<endl;
						cout<<" Format : "<<(waveHeader.Format==WAVE?"WAVE":"OTHERS")<<endl;
						cout<<" Audio format : "<<(waveHeader.AudioFormat==PCM?"PCM":"Others")<<endl;
						cout<<" Channel :"<<(waveHeader.NumChannels==MONO_W?"MONO":"STEREO")<<endl;
						cout<<" size : "<<waveDataSubHeader.Subchunk2Size<<endl;
						cout<<" Subchunk2ID : "<<(waveDataSubHeader.Subchunk2ID==DATA_ID2?"data":"OTHERS")<<endl;
						cout<<" Sample Rate :"<<waveHeader.SampleRate<<endl;
						cout<<" Bit Rate : "<<waveHeader.ByteRate<<endl;
						cout<<" BitsPerSample : "<<waveHeader.BitsPerSample<<endl;
						cout<<" Sub Chunk1 size :"<<waveHeader.Subchunk1Size<<endl;
						cout<<" ***************************************************** "<<endl;
					}
				}
		}
		catch (std::ifstream::failure e) {
			cout << "Error Reading Header of input file: " << waveFileName<< "  :  " <<strerror(errno) << '\n';
			return 1;
		}
		return 0;
	
}


/**
 * check how @(file_name) exist
 *
 * @interleaved_data	short int* type of
 * @sample_num	int type of number of samples passed by @interleaved_data
 * @return int as the result of function, 0 > return => Successful , 0 >= return => failed
 */
int WaveHelperClass::readPcmData_Interleaved(short int* interleaved_data,int sample_num) {
	// TODO ReadPcmData_Interleaved
	//cout <<" ReadPcmData_Interleaved : "<<sample_num<<" bytes "<<endl;
		try {
			if (ifWav.good()) {
				//Read some bytes from wave file and cast them to interleaved_data. the interleaved_data is a vriable passed by pointer
				ifWav.read(reinterpret_cast<char*>(interleaved_data), sample_num);
				return ifWav.gcount();
			}
			if(DEBUG_EN_DETAIL)
				cout <<" ifWav.good() : "<<ifWav.good()<< " eof: "<< ifWav.eof()<<" fail: "<< ifWav.fail()<< " bad: "<< ifWav.bad()<<endl;
			return 0;
		}
		catch (std::ifstream::failure e) {
			cout << "Error Reading PCM of input file: " << waveFileName<< "  :  " <<strerror(errno) << '\n';
			return -1;
		}
}

/**
 * check how @(file_name) exist
 *
 * @file_name string type of elements contained target file name
 * @return bool as the result of unction, true= exist , false= not exist
 */
bool WaveHelperClass::checkExist (string file_name) {
    ifstream f(file_name);
    cout<<"file name :"<<file_name<<endl;
    return f.good();
}
