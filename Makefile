CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -O2
LIBS=-lm

all: bms2A bms2B


bms2A: bms2A.cpp
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@ libsndfile.a

bms2B: bms2B.cpp
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@ libsndfile.a

clean:
	rm -f *.o bms2A bms2B

test: all
	./bms2B "test-input/example.wav"
	#./bms2A "test-input/example.txt"

generate-test-output:
	./bms2A "tests01/01.txt"
	./bms2A "tests01/02.txt"
	./bms2A "tests01/03.txt"
	./bms2A "tests01/04.txt"
	./bms2A "tests01/05.txt"
	./bms2A "tests01/06.txt"
	./bms2A "tests01/07.txt"
	./bms2A "tests01/08.txt"
	./bms2A "tests01/09.txt"
	./bms2A "tests01/10.txt"
	./bms2A "tests01/11.txt"
	./bms2A "tests01/12.txt"