# HuffZip — original C engine
CC      = gcc
CFLAGS  = -Wall -O2

.PHONY: all clean

all: compresser decompresser

compresser: compression/FonctionsArbre.c compression/FonctionsListe.c compression/FonctionsCompression.c compression/main.c
	$(CC) $(CFLAGS) -o $@ $^

decompresser: decompression/Decompression.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f compresser decompresser
