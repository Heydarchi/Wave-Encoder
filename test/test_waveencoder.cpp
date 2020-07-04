/*
 * TestMain.cpp
 *
 *  Created on: May 11, 2020
 *      Author: ubuntu
 */
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "WaveHelper.h"
#include "EncoderClass.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <stdint.h>

using namespace std;
vector<string> wave_list_ref={
		"bootSound.wav",
		"Connect.wav",
		"Disconnect.wav",
		"FileRecv.wav",
		"PolaroidShutter.wav"
};
#if __linux__
	vector<string> test_wave_files{"./test/wav/Connect.wav",
									"./test/wav/bootSound.wav"};
	string wav_dir="./test/wav/";
#else
	vector<string> test_wave_files{".\\test\\wav\\Connect.wav",
									".\\test\\wav\\bootSound.wav"};
	string wav_dir=".\\test\\wav\\";
#endif
//

string _Replace_(string orig,string target_str,string new_str)
{
	orig.replace(  orig.find(target_str) ,orig.find(target_str)+target_str.length(),new_str);
	return orig;
}


TEST_CASE( "1: Check WaveHelperClass : Mono Wave file", "[WaveHelperClass-1]" ) {

	WaveHelperClass wav_helper(test_wave_files.at(0));
    CHECK( wav_helper.checkExist(test_wave_files.at(0)) == true );
    CHECK( wav_helper.waveHeader.ChunkID == RIFF );
    CHECK( wav_helper.waveHeader.Format == WAVE );
    CHECK( wav_helper.waveHeader.AudioFormat == PCM );
    CHECK( wav_helper.waveHeader.NumChannels == MONO_W );
    CHECK( wav_helper.waveHeader.SampleRate == 22050 );
    CHECK( wav_helper.waveDataSubHeader.Subchunk2ID == DATA_ID2 );
    CHECK( wav_helper.waveHeader.ByteRate == 44100 );
    CHECK( wav_helper.waveHeader.BitsPerSample == 16 );
    CHECK( wav_helper.waveHeader.Subchunk1Size == DEF_SUBCHUNK_SIZE );
    CHECK( wav_helper.waveDataSubHeader.Subchunk2Size == 36592 );
}

TEST_CASE( "2: Check WaveHelperClass : Stereo Wave file", "[WaveHelperClass-2]" ) {
	WaveHelperClass wav_helper(test_wave_files.at(1));
    CHECK( wav_helper.checkExist(test_wave_files.at(1)) == true );
    CHECK( wav_helper.waveHeader.ChunkID == RIFF );
    CHECK( wav_helper.waveHeader.Format == WAVE );
    CHECK( wav_helper.waveHeader.AudioFormat == PCM );
    CHECK( wav_helper.waveHeader.NumChannels == STEREO_W );
    CHECK( wav_helper.waveHeader.SampleRate == 44100 );
    CHECK( wav_helper.waveDataSubHeader.Subchunk2ID == DATA_ID2 );
    CHECK( wav_helper.waveHeader.ByteRate == 176400 );
    CHECK( wav_helper.waveHeader.BitsPerSample == 16 );
    CHECK( wav_helper.waveHeader.Subchunk1Size == 18 );
    CHECK( wav_helper.waveDataSubHeader.Subchunk2Size == 360448 );
}

TEST_CASE( "3: Check utility : Just Wave file must be included in return vector", "[utility]" ) {
	vector<string> wave_list=enumerateWaveFileFromDir(wav_dir.c_str(),".wav");
    SECTION( "Check whether 2 vectors size are same : " ) {
    	CHECK ( (wave_list.size())==wave_list_ref.size() );
    }
    SECTION( "Check whether all reference file name are exist in result vector : " ) {
		for(uint32_t i=0;i<(wave_list_ref.size());i++)
		{
			bool exist=false;
			for(uint32_t j=0;j<wave_list.size();j++)
			{
				if(wave_list.at(j).find(wave_list_ref.at(i),0)!=string::npos)
				{
					exist=true;
					break;
				}
			}
			INFO(wave_list_ref.at(i));
			CHECK ( exist );
		}
	}



}

TEST_CASE( "4: Check EncoderClass : converting Wave file", "[EncoderClass]" ) {
	vector<string> wave_list=enumerateWaveFileFromDir(wav_dir.c_str(),".wav");
	for(uint32_t i=0;i<wave_list.size();i++)
	{
		string output_fn(_Replace_(wave_list.at(i), ".wav",".mp3" ));
	    //output_fn.replace(  output_fn.find(".wav") ,output_fn.find(".wav")+4,".mp3");
		if(checkExist(output_fn)==true)
		{
			  remove( output_fn.c_str() );
		}
	}
	EncoderClass enc_obj (wave_list);
	enc_obj.startEncodingProgress();

	vector<string> mp3_list=enumerateWaveFileFromDir(wav_dir.c_str(),".mp3");
    SECTION( "Check whether size of the mp3 and wave file names vectors  are same : " ) {
    CHECK( mp3_list.size()==wave_list.size());
    }
    SECTION( "Check whether all reference file name are exist in mp3 result vector : " ) {

		for(uint32_t i=0;i<(wave_list_ref.size());i++)
		{
			bool exist=false;
			string mp3_fn(_Replace_(wave_list_ref.at(i), ".wav",".mp3" ));
			for(uint32_t j=0;j<mp3_list.size();j++)
			{
				if(mp3_list.at(j).find(mp3_fn,0)!=string::npos)
				{
					exist=true;
					break;
				}
			}
			CHECK ( exist );
		}
	}


}



