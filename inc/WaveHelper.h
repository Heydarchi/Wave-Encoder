#include <stdint.h>
#include <string.h>
#include <string>
#include <fstream>

#ifndef WAVEHELPERCLASS_H_
#define WAVEHELPERCLASS_H_

using namespace std;

//Some definition for Wave file header
#define	WAVE					0x57415645
#define RIFF					0x52494646
#define FMT						0x666d7420
#define DATA_ID2				0x64617461
#define MONO_W					1	//Num channels
#define STEREO_W					2	//Num channels
#define PCM						1	//Audio Format
#define DEF_SUBCHUNK_SIZE 		16
#define DATA_SUBCHUNK_SIZE			8

//Debug definition for debugging
#define DEBUG_EN				0
#define DEBUG_EN_DETAIL			0

//Ref Link -> http://soundfile.sapp.org/doc/WaveFormat/
struct WaveHeaderStruct{
	uint32_t ChunkID;			//Contains the letters "RIFF" in ASCII form  (0x52494646 big-endian form
	uint32_t ChunkSize;			//
	uint32_t Format;			//Contains the letters "WAVE" (0x57415645 big-endian form).
	uint32_t Subchunk1ID;		//Contains the letters "fmt " (0x666d7420 big-endian form)
	uint32_t Subchunk1Size;		//16 for PCM.  This is the size of the rest of the Subchunk which follows this number.
	uint16_t AudioFormat;		//PCM = 1 (i.e. Linear quantization) Values other than 1 indicate some form of compression.
	uint16_t NumChannels;		//Mono = 1, Stereo = 2, etc.
	uint32_t SampleRate;		//8000, 44100, etc.
	uint32_t ByteRate;			//== SampleRate * NumChannels * BitsPerSample/8
	uint16_t BlockAlign;		//== NumChannels * BitsPerSample/8
	uint16_t BitsPerSample;		//8 bits = 8, 16 bits = 16, etc.
};

struct WaveHedearDataSubchunk{
	uint32_t Subchunk2ID;		//Contains the letters "data" (0x64617461 big-endian form)
	uint32_t Subchunk2Size;		//== NumSamples * NumChannels * BitsPerSample/8

};



class WaveHelperClass{
	
public :
	WaveHelperClass(string file_name);		//WaveHelperClass constructor
	virtual ~WaveHelperClass();				//WaveHelperClass deconstructor
	int readWaveHeader();					//Read header of file included in waveFileName
	int openWaveFile();						//Open a handle to a file including in waveFileName
	int closeWaveFile();					//Close the handle to a file including in waveFileName
	int readPcmData_Interleaved(short int* interleaved_data,int sample_num);	//Read some byte of wave file 
	WaveHeaderStruct waveHeader;				//A variable of tpe WaveHeaderStruct
	WaveHedearDataSubchunk waveDataSubHeader;	//A Variable of type WaveHedearDataSubchunk
	bool checkExist (string file_name);			//Check how a file passed to function is exist
private :
	string waveFileName;						//Includes the file name of target wave which the class function work on
    ifstream ifWav;								//A handle to wave file used in function

};

#endif
