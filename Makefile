CC=gcc
CFLAGS=-c -Wall
DEPS = httpd.h
OBJ = httpd.o

all: httpd.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

httpd: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o httpd