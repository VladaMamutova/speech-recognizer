CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=gnu++0x
LFLAGS = -pthread

INCLUDES = -I math -I audio -I command
SRC = math/*.cpp audio/*.cpp command/*.cpp main.cpp
OBJ_DIR = obj

OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o, $(SRC:.c=.o))

EXE = speech_recognizer

all: build

build: $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(INCLUDES) $(LFLAGS) -o $(EXE)

clean:
	rm -rf *.o
