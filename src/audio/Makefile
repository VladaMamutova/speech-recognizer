CC=g++
CFLAGS = -std=c++11 -Wall -Werror -g
OBJS = WavHeader.o WavData.o MfccFeatures.o Frame.o AudioProcessor.o

all: $(OBJS)

WavHeader.o: WavHeader.cpp WavHeader.h
	$(CC) $(CFLAGS) -c WavHeader.cpp

WavData.o: WavData.cpp WavData.h WavHeader.h
	$(CC) $(CFLAGS) -c WavData.cpp

MfccFeatures.o: MfccFeatures.cpp MfccFeatures.h
	$(CC) $(CFLAGS) -c MfccFeatures.cpp

Frame.o: Frame.cpp Frame.h ../config.h MfccFeatures.h ../math/MFCC.h
	$(CC) $(CFLAGS) -c Frame.cpp

AudioProcessor.o: AudioProcessor.cpp AudioProcessor.h WavData.h Frame.h
	$(CC) $(CFLAGS) -c AudioProcessor.cpp


clean:
	rm -rf audio *.o
