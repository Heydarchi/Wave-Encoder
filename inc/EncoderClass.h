/*
 * EncoderClass.h
 *
 *  Created on: May 7, 2020
 *      Author: M.H.H
 */

#include <dirent.h>
#include <pthread.h>
#include <string.h>
#include <vector>
#include <string>
#include "lame.h"

#ifndef ENCODERCLASS_H_
#define ENCODERCLASS_H_

using namespace std;
using std::string;


#define WAVE_BUF_SIZE 8196

//Some definition for lame
#define LAME_FAST_QUALITY	7
#define LAME_UPPERGOOD_QUALITY	4
#define LAME_GOOD_QUALITY	5
#define LAME_NBEST_QUALITY 2

class EncoderClass {
public:

	EncoderClass(vector<string> file_name_list);
	virtual ~EncoderClass();
	void startEncodingProgress();
	void* encodeWaveFileToMP3(void*);
	int getCpuInfo();
	string getNextFileName();
	int initLame(lame_global_flags **gfp, uint16_t ch_num,uint32_t in_srate);
	void disposeLame(lame_global_flags **gfp);

private:
	int maxConcurrentThread=1;

};

#endif /* ENCODERCLASS_H_ */
