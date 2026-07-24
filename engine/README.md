# HuffZip — C engine (2022)

The original implementation of the project: a Huffman compressor and
decompressor written in C, using hand-written linked lists and binary trees with
no external libraries. It works on raw bytes and reads and writes files whose
names are fixed in the source.

## Build

```bash
make            # builds ./compresser and ./decompresser
make clean      # removes the binaries
```

Or compile by hand:

```bash
gcc -Wall -O2 -o compresser   compression/*.c
gcc -Wall -O2 -o decompresser decompression/Decompression.c
```

## Run

Both programs use hard-coded file names and operate in the current directory.

**Compression** — reads `Fichier a compresser.txt` and produces
`Fichier Compresse.txt`:

```bash
cd compression
../compresser
```

**Decompression** — reads `Fichier Compresse.txt` and produces
`Fichier Decompresse.txt`:

```bash
cd decompression
../decompresser
```

## How the compressed file is organised

The compressed file is written in two parts, separated by the marker `/./`:

1. A header holding the original length, then every character with its
   occurrence count. This lets the decompressor rebuild the exact same Huffman
   tree.
2. The Huffman bitstream, packed seven bits at a time into printable characters.
   The very first character of the file records how many padding bits were added
   to complete the last group.

## Known limitation

On some inputs the last decoded character can be wrong (the final padded group
of bits is not always reconstructed exactly). This is a limitation of the 2022
engine and is documented here for honesty. The JavaScript implementation used by
the web application does not have this problem: it re-decodes every encoding and
confirms the round trip is exact.
