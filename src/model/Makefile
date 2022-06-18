CC=g++
CFLAGS = -std=c++11 -Wall -Werror -g
OBJS = Phoneme.o PhonemeMap.o PhonemePrediction.o TermFrequency.o LsaTrainer.o Storage.o SpeechProcessor.o

all: $(OBJS)

Phoneme.o: Phoneme.cpp Phoneme.h ../audio/MfccFeatures.h
	$(CC) $(CFLAGS) -c Phoneme.cpp

PhonemeMap.o: PhonemeMap.cpp PhonemeMap.h Phoneme.h PhonemePrediction.h
	$(CC) $(CFLAGS) -c PhonemeMap.cpp

PhonemePrediction.o: PhonemePrediction.cpp PhonemePrediction.h ../math/Tree.h ../audio/MfccFeatures.h
	$(CC) $(CFLAGS) -c PhonemePrediction.cpp

TermFrequency.o: TermFrequency.cpp TermFrequency.h
	$(CC) $(CFLAGS) -c TermFrequency.cpp

LsaTrainer.o: LsaTrainer.cpp LsaTrainer.h TermFrequency.h ../math/Matrix.h ../math/Tree.h
	$(CC) $(CFLAGS) -c LsaTrainer.cpp

Storage.o: Storage.cpp Storage.h PhonemeMap.h
	$(CC) $(CFLAGS) -c Storage.cpp

SpeechProcessor.o: SpeechProcessor.cpp SpeechProcessor.h Storage.h
	$(CC) $(CFLAGS) -c SpeechProcessor.cpp

clean:
	rm *.o
