CC=cc
JMK_CFLAGS=-O -I../..

UNZ_OBJS = miniunz.o unzip.o ../../libz.a
ZIP_OBJS = minizip.o zip.o   ../../libz.a

.c.o:
	$(CC) -c $(JMK_CFLAGS) $*.c

all: miniunz minizip

miniunz:  $(UNZ_OBJS)
	$(CC) $(JMK_CFLAGS) -o $@ $(UNZ_OBJS)

minizip:  $(ZIP_OBJS)
	$(CC) $(JMK_CFLAGS) -o $@ $(ZIP_OBJS)

test:	miniunz minizip
	./minizip test readme.txt
	./miniunz -l test.zip
	mv readme.txt readme.old
	./miniunz test.zip

clean:
	/bin/rm -f *.o *~ minizip miniunz
