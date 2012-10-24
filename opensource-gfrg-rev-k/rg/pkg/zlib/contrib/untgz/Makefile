CC=cc
JMK_CFLAGS=-g

untgz: untgz.o  ../../libz.a
	$(CC) $(JMK_CFLAGS) -o untgz  untgz.o -L../.. -lz

untgz.o: untgz.c ../../zlib.h
	$(CC) $(JMK_CFLAGS) -c -I../.. untgz.c

../../libz.a:
	cd ../..; make

clean:
	rm -f untgz untgz.o *~
