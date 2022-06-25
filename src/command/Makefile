CC=g++
CFLAGS = -std=c++11 -Wall -Werror -g
OBJS = CommandProcessor.o

all: $(OBJS)

CommandProcessor.o: CommandProcessor.cpp CommandProcessor.h Help.h ../audio/AudioProcessor.h ../model/Storage.h ../model/SpeechProcessor.h
	$(CC) $(CFLAGS) -c CommandProcessor.cpp

clean:
	rm *.o
