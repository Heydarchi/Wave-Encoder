# Wave-Encoder

This application use lame library to encode wave file to mp3 format.

I. File & Folder list
------------

/src					//Includes .h header files
/header					//Includes .cpp source files
/test					//Includes source, header and some wave file to test the project 
/test/wave				//Includes wav file required for running the tests method
/wave					//Includes some wave file as an example
/libs					//Includes libmp3lame.a, libmp3lame_win.a library for Ubuntu & Win10
/docs					//Document of source code
/lame-3.10				//source code of lame library

waveencoder.cpp				//This file includes the main function of peojects
EncoderClass.cpp		//Implementation of ENcoder class including function for encoding Wave file
EncoderClass.h			//EncoderClass header
WaveHelper.cpp			//Implementation of WaveHelperClass including some function to work with Wave file
WaveHelper.h			//WaveHelperClass header
utility.cpp				//Some useful functions like Enumerating files ebist in directory, Check file exist
utility.h				//utility header
test_waveencoder.cpp			//This file includes test routines
catch.hpp				//Catch2 single file for implementing the tests routines




II. How to build
------------

1. Prequesties 

	The waveencoder wave encoder project required Lame static library which you can build from the source. The Lame source version used to test the projects is 3.100
	Also the projects inculdes 2 static libraries built from Lame 3.100 source for x64 Windwows-10 and Ubuntu-18. So you do not require to rebuild the Lame libraries if your system
	configuration is compatible with them.
	library for Windows :libmp3lame_win.a
	library for Ubuntu  : libmp3lame.a
	
2. Tools to build
	To build the project I used tools below :
	Windows  10 : g++ version  7.4.0 (GCC)	from CygWin
	Ubuntu 18   : g++ version  from 

3. How build Lame 3.100
	To build the Lame libraries you need to run some command
	-	./configure --disable-frontend
	-	make
	- then built(libmp3lame.a) librar must be copied to /libs fodler in the project directory. The generated library can be found in /usr/local/lib of your system
	- for run in windows the (libmp3lame.a) must renamed to  (libmp3lame_win.a)
	
4. How build the waveencoder project
	To build the waveencoder project you need to run some command
	-	make 	clean
	-	make	
	
5. How build the test
	To build the test app you need to run some command
	-	make 	clean
	-	make	test
	
6. How to run application

	- For Windows 	:		waveencoder.exe  	[path]   
							waveencoder.exe   wav\
	- For Ubuntu 	:    	./waveencoder  		[path]
							./waveencoder	./wav
	
7. How to run test 

	- For Windows 	:		test_main.exe  	
	- For Ubuntu 	:    	./test_waveencoder  		


III. Code Flow
------------


1. First application enumerates the files in the path   user sent as an input parameter.
2. Next  an object of type EncoderClass will be created.
3. After that startEncodingProgress of EncoderClass object will be called to encoding all wave enumerated file.
4. To encode the wave file by EncoderClass object, some thraeds based on Cpu core numbers will be started
5. Before encoding any file the EncoderClass object checks the header of files to assure that the file format is correct. This be done by an object of WaveHelperClass type 




