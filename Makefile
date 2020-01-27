all: ELFHeaderParser

CC = g++
CFLAGS = -ljson-c -lcapstone
ELFHeaderParser: main.cpp ELFHeaderParser.cpp disassem.cpp
	$(CC) -o ELFHeaderParser main.cpp ELFHeaderParser.cpp disassem.cpp $(CFLAGS)


.PHONY: clean,run
clean:
	rm ELFHeaderParser

run:
	./ELFHeaderParser ./test/sample ./111.json ./111.asm

runs:
	 ./ELFHeaderParser ./test/sample2
