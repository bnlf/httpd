CC=gcc
CFLAGS=-c -Wall
#DEPS = src/httpd.h

all: httpd

httpd: httpd.o
	$(CC) httpd.o -o bin/httpd

httpd.o: 
	$(CC) $(CFLAGS) src/httpd.c 

clean:
	rm -f *.o httpd