SHELL=bash
CC:=gcc
CFLAGS:= -O3 -Wall
.PHONY:all clean test

all:verticalize

verticalize:verticalize.c
	$(CC) $(CFLAGS) -o $@ $<

test:verticalize
	echo  -en "#H1\tH2\nA1\tA2\nB1\tB2\nC1\tC2\nD1\tD2\tD3\nE1\n" | ./$<
	echo  -en "#H1\tH2\nA1\tA2\nB1\tB2\nC1\tC2\nD1\tD2\tD3\nE1\n" | ./$< | cat
	echo  -en "#H1;H2\nA1;A2\nB1;B2\nC1;C2\nD1;D2;D3\nE1\n" | ./$< -d ';'
	echo  -en "#H1;H2\nA1;A2\nB1;B2\nC1;C2\nD1;D2;D3\nE1\n" | ./$< -d ';' | cat
	echo  -en "A1\tA2\nB1\tB2\nC1\tC2\nD1\tD2\tD3\nE1\n" | ./$< -n
	echo  -en "A1\tA2\nB1\tB2\nC1\tC2\nD1\tD2\tD3\nE1\n" | ./$< -n | cat
	echo  -en "A1;A2\nB1;B2\nC1;C2\nD1;D2;D3\nE1\n" | ./$< -n -d ';'
	echo  -en "A1;A2\nB1;B2\nC1;C2\nD1;D2;D3\nE1\n" | ./$< -n -d ';' | cat

clean:
	rm -f verticalize
