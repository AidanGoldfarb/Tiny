CC = gcc
CFLAGS = -g -Wall -I -fno-stack-protector.

# This flag includes the Pthreads library on a Linux box.
# Others systems will probably require something different.
LIB = -lpthread

all: tiny cgi buf

tiny: tiny.c csapp.o
	$(CC) $(CFLAGS) -o tiny tiny.c csapp.o $(LIB)

csapp.o: csapp.c
	$(CC) $(CFLAGS) -c csapp.c

cgi:
	(cd cgi-bin; make)
	
buf: buf.c
	$(CC) $(CFLAGS) -Wall -o buf buf.c $(LIB)

clean:
	rm -f *.o tiny *~
	(cd cgi-bin; make clean)

