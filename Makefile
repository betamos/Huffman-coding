
all:	compress extract

compress:	main_compress.o compress.o common.o bitarray.o bitfile.o pqueue.o
	$(CC) $(CFLAGS) -o compress main_compress.o compress.o common.o bitarray.o bitfile.o pqueue.o
extract:	main_extract.o extract.o common.o bitfile.o pqueue.o 
	$(CC) $(CFLAGS) -o extract main_extract.o extract.o common.o bitfile.o pqueue.o

# Main files
main_compress.o:	main_compress.c
	$(CC) $(CFLAGS) -c main_compress.c
main_extract.o:	main_extract.c
	$(CC) $(CFLAGS) -c main_extract.c

# User libs
common.o:	common.c
	$(CC) $(CFLAGS) -c common.c
compress.o:	compress.c
	$(CC) $(CFLAGS) -c compress.c
extract.o:	extract.c
	$(CC) $(CFLAGS) -c extract.c

# External libs
bitarray.o:	lib/bitarray/bitarray.c
	$(CC) $(CFLAGS) -c lib/bitarray/bitarray.c
bitfile.o:	lib/bitfile/bitfile.c
	$(CC) $(CFLAGS) -c lib/bitfile/bitfile.c
pqueue.o:	lib/pqueue/pqueue.c
	$(CC) $(CFLAGS) -c lib/pqueue/pqueue.c

clean:
	rm *.o compress extract

CC=gcc
CFLAGS= -Wall
