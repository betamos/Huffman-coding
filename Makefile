
all:
	gcc -Wall main.c lib/bitfile/bitfile.c lib/bitarray/bitarray.c common.c compress.c lib/pqueue/pqueue.c extract.c
