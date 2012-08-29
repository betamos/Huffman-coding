
all:	compress extract

compress:	main_compress.o compress.o common.o bitarray.o bitfile.o pqueue.o
	$(CC) $(CFLAGS) -o compress main_compress.o compress.o common.o bitarray.o bitfile.o pqueue.o
extract:	main_extract.o extract.o common.o bitarray.o bitfile.o pqueue.o 
	$(CC) $(CFLAGS) -o extract main_extract.o extract.o common.o bitarray.o bitfile.o pqueue.o

# Main files
main_compress.o:
	$(CC) $(CFLAGS) -c main_compress.c
main_extract.o:
	$(CC) $(CFLAGS) -c main_extract.c

# User libs
common.o:
	$(CC) $(CFLAGS) -c common.c
compress.o:
	$(CC) $(CFLAGS) -c compress.c
extract.o:
	$(CC) $(CFLAGS) -c extract.c

# External libs
bitarray.o:
	$(CC) $(CFLAGS) -c lib/bitarray/bitarray.c
bitfile.o:
	$(CC) $(CFLAGS) -c lib/bitfile/bitfile.c
pqueue.o:
	$(CC) $(CFLAGS) -c lib/pqueue/pqueue.c

clean:
	rm *.o compress extract

CC=gcc
CFLAGS= -Wall
