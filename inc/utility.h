/*
 * utility.h
 *
 *  Created on: May 12, 2020
 *      Author: ubuntu
 */

#include <string>
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <dirent.h>

#ifndef HEADER_UTILITY_H_
#define HEADER_UTILITY_H_
using namespace std;

//Enumerates all files exist in directory with specific extension
vector<string> enumerateWaveFileFromDir(const char *dir,const char* ext);

//Check how a file existor not
bool checkExist (string file_name);

#endif /* HEADER_UTILITY_H_ */
