CC=gcc
#CFLAGS=-c -Wall
DIR=src

all: 
	$(CC) $(CFLAGS) $(DIR)/httpd.c $(DIR)/main.c -o bin/httpd

clean:
	rm -f *.o httpd