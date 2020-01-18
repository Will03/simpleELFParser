all: ELFHeaderParser

CC = g++
CFLAGS = -ljson-c
ELFHeaderParser: main.cpp ELFHeaderParser.cpp
	$(CC) -o ELFHeaderParser main.cpp ELFHeaderParser.cpp $(CFLAGS)


.PHONY: clean,run
clean:
	rm ELFHeaderParser

run:
	./ELFHeaderParser ./test/sample

runs:
	 ./ELFHeaderParser ./test/sample2
