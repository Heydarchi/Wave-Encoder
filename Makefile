PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
CATCH_SINGLE_INCLUDE=test
OBJS= WaveHelper.o  EncoderClass.o cinemo.o utility.o test_cinemo.o
ifeq ($(OS), Windows_NT)
	TARGET =	cinemo.exe
	TEST=		test_main.exe
	VPATH=$(PROJECT_ROOT) ./src ./inc ./libs ./test
	LIBS =   -lm
	LAMELIB= libmp3lame_win.a
	DEPENDENCIES = cinemo.cpp EncoderClass.cpp EncoderClass.h lame.h WaveHelper.cpp WaveHelper.h utility.h utility.cpp
	CXXFLAGS =	 -Ilibs -Iinc -Isrc -Itest -pthread -O2 -g -Wall #-fmessage-length=0  
else
	TARGET =	cinemo
	TEST=		test_main
	VPATH=$(PROJECT_ROOT) ./src ./inc ./libs ./test
	LIBS =   -lm 
	LAMELIB= libmp3lame.a
	DEPENDENCIES = lame.h utility.cpp WaveHelper.o EncoderClass.o waveencoder.o
	CXXFLAGS =	 -Ilibs -Iinc -Isrc -Itest -pthread -O2 -g -Wall #-fmessage-length=0  
endif

all: $(TARGET)

$(TARGET):	 $(DEPENDENCIES)  $(LAMELIB) 
	$(CXX) $(CXXFLAGS)   $(LIBS)   -o $(TARGET)   $^

waveencoder.o : waveencoder.cpp EncoderClass.h  utility.h
		$(CXX) $(CXXFLAGS)   $(LIBS) -c $< 
	

EncoderClass.o : EncoderClass.cpp EncoderClass.h lame.h WaveHelper.h
		$(CXX) $(CXXFLAGS)   $(LIBS) -c $< 
	

WaveHelper.o : WaveHelper.cpp WaveHelper.h
		$(CXX)  $(CXXFLAGS)   $(LIBS) -c $<  

utility.o : utility.cpp utility.h
		$(CXX)  $(CXXFLAGS)   $(LIBS) -c $<  



clean:
	rm -f $(OBJS) $(TARGET) $(TEST)


test:	$(TEST)

$(TEST): 	test_waveencoder.o catch.hpp lame.h utility.cpp WaveHelper.o EncoderClass.o    $(LAMELIB) 
	$(CXX) $(CXXFLAGS) -I$(CATCH_SINGLE_INCLUDE)  $(LIBS)   -o $(TEST)   $^

test_waveencoder.o : test_waveencoder.cpp WaveHelper.h EncoderClass.h catch.hpp  utility.h
		$(CXX) $(CXXFLAGS) -I$(CATCH_SINGLE_INCLUDE) $(LIBS) -c $< 
	

		
		
