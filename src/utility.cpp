/*
 * utility.cpp
 *
 *  Created on: May 12, 2020
 *      Author: ubuntu
 */
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>

using namespace std;


/**
 * Return a list of files exist in the @dir path
 *
 * @dir const char * type of elements contained in vector
 * @ext const char* type contains extension of target files
 * @return copy of vector contains files which are found
 */
vector<string> enumerateWaveFileFromDir(const char *dir,const char* ext)
{
	dirent **name_list;
	vector<string> wave_list;
	//Scan directory to get all things the directory contains
	int ret=scandir(dir,&name_list,NULL,alphasort);
	cout<<"dir : " <<dir<<endl;
	if(ret>0)
	{
		for(int i=0;i<ret;i++)
		{
			//Check if this is a file with extention passed by user
			if(name_list[i]->d_type==DT_REG && strstr(name_list[i]->d_name,ext)!=NULL)
			{
				string fn( dir );
				//Append file name tp directory path 
				fn.append( name_list[i]->d_name );
				cout<<fn<<endl;
				//Push the file name in vector
				wave_list.push_back(fn);

			}
		}
		cout<<"Application found [ "<<wave_list.size()<<" ]  "<<ext<<" files in path : "<<dir<<endl<<endl;

	}
	else
	{
		cout<<" No file is found"<<endl;
	}
	return wave_list;
}


/**
 * check how @(file_name) exist
 *
 * @file_name string type of elements contained target file name
 * @return bool as the result of unction, true= exist , false= not exist
 */
bool checkExist (string file_name) {
    ifstream f(file_name);
    cout<<"file name :"<<file_name<<endl;
    return f.good();
}
