CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=gnu++0x -std=c++17
LFLAGS = -pthread

OBJS = math/*.o audio/*.o model/*.o command/*.o main.o
EXE = speech_recognizer

all: build

build: build_directories main.o
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(EXE)

build_directories:
	cd math && $(MAKE)
	cd audio && $(MAKE)
	cd model && $(MAKE)
	cd command && $(MAKE)

main.o: main.cpp command/CommandProcessor.h
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm $(OBJS)
