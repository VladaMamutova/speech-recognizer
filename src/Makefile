CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=gnu++0x
LFLAGS = -pthread

INCLUDES = -I audio -I command
SRC = audio/Wav.cpp command/CommandProcessor.cpp main.cpp
OBJ_DIR = obj

OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o, $(SRC:.c=.o))

EXE = speech_recognizer

all: build

build: $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(INCLUDES) $(LFLAGS) -o $(EXE)

clean:
	rm -rf *.o
