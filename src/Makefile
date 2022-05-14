CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=gnu++0x -std=c++17
LFLAGS = -pthread

INCLUDES = -I math -I audio -I model -I command
SRC = math/*.cpp audio/*.cpp model/*.cpp command/*.cpp main.cpp

EXE = speech_recognizer

all: build

build: $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(INCLUDES) $(LFLAGS) -o $(EXE)
