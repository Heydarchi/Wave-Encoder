//============================================================================
// Name        : CinemoEncoderTask.cpp
// Author      : M.H.H
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include "EncoderClass.h"
#include "utility.h"

using namespace std;



int main(int argc, char **argv) {

	cout<<"!!! Cinemo MP3 Encoder Task !!!"<<endl;
	if(argc!=2)
	{
		cout<<endl<<endl<<" Application couldn't run because it requires path as a parameter !"<<endl;
		return EXIT_FAILURE;
	}

	//Enumerate the file names in directory passed by user
	vector<string> wave_list=enumerateWaveFileFromDir(argv[1],".wav");
	cout<<"Count "<<wave_list.size()<<endl;
	//Create an object of type EncoderClass to encode all files
	EncoderClass enc_obj (wave_list);
	//Start Encoding process
	enc_obj.startEncodingProgress();
	cout<<"Cinemo MP3 Encoder Task Finished !!!"<<endl<< std::flush;

	return EXIT_SUCCESS;
}
