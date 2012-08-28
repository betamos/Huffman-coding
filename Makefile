
all:
	gcc -Wall main.c lib/bitfile/bitfile.c lib/bitarray/bitarray.c tree.c analyzer.c lib/pqueue/pqueue.c
