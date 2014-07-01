CC:=gcc
CFLAGS:= -O3 -Wall
.PHONY:all clean

all:verticalize

verticalize:verticalize.c
	$(CC) $(CFLAGS) -o $@ $<
clean:
	rm -f verticalize
