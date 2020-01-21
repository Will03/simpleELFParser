all: ELFHeaderParser

CC = g++
CFLAGS = -ljson-c -lcapstone
ELFHeaderParser: main.cpp ELFHeaderParser.cpp dissamFunc.cpp
	$(CC) -o ELFHeaderParser main.cpp ELFHeaderParser.cpp dissamFunc.cpp $(CFLAGS)


.PHONY: clean,run
clean:
	rm ELFHeaderParser

run:
	./ELFHeaderParser ./test/sample

runs:
	 ./ELFHeaderParser ./test/sample2
