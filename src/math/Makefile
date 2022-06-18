CC=g++
CFLAGS = -std=c++11 -Wall -Werror -g
OBJS = Basic.o MFCC.o Tree.o Matrix.o SVD.o

all: $(OBJS)

Basic.o: Basic.cpp Basic.h ../config.h
	$(CC) $(CFLAGS) -c Basic.cpp

MFCC.o: MFCC.cpp MFCC.h
	$(CC) $(CFLAGS) -c MFCC.cpp

Tree.o: Tree.cpp Tree.h
	$(CC) $(CFLAGS) -c Tree.cpp

Matrix.o: Matrix.cpp Matrix.h
	$(CC) $(CFLAGS) -c Matrix.cpp

SVD.o: SVD.cpp SVD.h Matrix.h
	$(CC) $(CFLAGS) -c SVD.cpp


clean:
	rm *.o
