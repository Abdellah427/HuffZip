# HuffZip — C engine

The original implementation of the project: a Huffman compressor and
decompressor written in C, using hand-written linked lists and binary trees
with no external libraries. It works on raw bytes and reads and writes files
whose names are fixed in the source.

## Quick start (compile + compress + decompress + verify)

The easiest way to try it locally — it builds both programs, compresses a file,
decompresses it again, and checks the result is identical to the original.

- **Windows** — double-click `huffzip.bat`, or drag a `.txt` file onto it.
- **macOS / Linux** — `./demo.sh path/to/file.txt`

Both need a C compiler (`gcc`) on the PATH. On Windows, install
[MinGW-w64](https://www.mingw-w64.org/) or w64devkit.

## Build manually

```bash
make            # builds ./compresser and ./decompresser
make clean      # removes the binaries
```

Or by hand:

```bash
gcc -O2 -o compresser   compression/*.c
gcc -O2 -o decompresser decompression/Decompression.c
```

## Run manually

Both programs use hard-coded file names and operate in the current directory.

**Compression** — reads `Fichier a compresser.txt`, writes `Fichier Compresse.txt`:

```bash
cd compression && ../compresser
```

**Decompression** — reads `Fichier Compresse.txt`, writes `Fichier Decompresse.txt`:

```bash
cd decompression && ../decompresser
```

## How the compressed file is organised

The compressed file is written in two parts, separated by the marker `/./`:

1. A header holding the original length, then every character with its
   occurrence count. This lets the decompressor rebuild the exact same Huffman
   tree.
2. The Huffman bitstream, packed seven bits at a time into bytes. The first
   character of the file records how many padding bits were added to complete
   the last group, so decompression drops them and restores the exact input.

## Notes

- **Lossless round-trip.** The last-group padding is reconstructed exactly; the
  input is restored byte for byte (checked over a wide range of lengths,
  endings, multi-line and accented UTF-8 text).
- **Binary-safe I/O.** All files are opened in binary mode, so packed bytes are
  never altered by line-ending conversion — the tool behaves identically on
  Windows, macOS and Linux.
- **Degenerate input.** A file containing a single distinct character has no
  meaningful binary code; the compressor reports this and stops rather than
  producing an invalid file.
