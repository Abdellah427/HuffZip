# HuffZip

**Lossless text compression with Huffman coding — computed live in the browser.**

**Live:** [abdellah-hassani.fr/huffzip](https://abdellah-hassani.fr/huffzip)

HuffZip turns a piece of text into its Huffman encoding and shows every step of
the way: the frequency of each character, the binary tree that assigns short
codes to frequent symbols, the code table, and the resulting bitstream. Nothing
is uploaded — the whole computation runs on your machine, and each compression
is re-decoded to prove it is lossless.

![HuffZip interface](docs/preview.png)

The project started as a 2022 university assignment: a compressor and a
decompressor written in C, with hand-rolled linked lists and binary trees and no
external libraries. That original engine still lives in [`engine/`](engine/).
The web application reuses the same algorithm, rewritten in dependency-free
JavaScript.

---

## Features

- **Live compression** — type or paste text and watch the ratio, entropy and
  average code length update instantly.
- **Import a `.txt` file** — drag and drop a text file (or pick one) to compress
  a whole book at once; it is read and processed in the browser and never leaves
  your machine.
- **Huffman tree view** — an SVG rebuilds as you type; left edges are `0`
  (teal), right edges are `1` (amber).
- **Code table** — every symbol with its frequency and its prefix-free code.
- **Bitstream** — the encoded output, grouped into bytes the way it is written
  to disk.
- **Round-trip check** — the encoder's output is decoded again on every change
  to guarantee the result is lossless.
- **Bilingual** — French and English, with the choice remembered between visits.
- **Zero dependencies, no external requests** — plain HTML, CSS and ES modules,
  with self-hosted fonts. No build step, no framework, no CDN, no tracking.

## Repository layout

```
.
├── index.html            Web application
├── mentions.html         Legal notice (French)
├── assets/
│   ├── css/style.css     Visual system
│   ├── js/huffman.js     Huffman core (frequencies, tree, codes, encode/decode)
│   ├── js/app.js         Interface, tree drawing, i18n
│   └── favicon.svg
├── engine/               Original C compressor / decompressor (2022)
│   ├── compression/
│   ├── decompression/
│   ├── huffzip.bat       One-click build + compress + decompress + verify (Windows)
│   ├── demo.sh           Same, for macOS / Linux
│   ├── Makefile
│   └── README.md
└── docs/preview.png
```

## Deployment

The web app is live at
[abdellah-hassani.fr/huffzip](https://abdellah-hassani.fr/huffzip). It is a set
of static files (no PHP, database or Node runtime), so it can be served from any
static host by copying `index.html`, `mentions.html` and the `assets/` folder
into the web root.

## Run the web app locally

The site is fully static. Because it uses ES modules, open it through a local
web server rather than the `file://` protocol:

```bash
# any static server works, for example:
python3 -m http.server 8000
# then open http://localhost:8000
```

## Run the C engine locally

Build the original compressor and decompressor and check a full round-trip in
one step:

```bash
cd engine
./demo.sh path/to/file.txt      # macOS / Linux
```

On Windows, double-click `engine/huffzip.bat` or drag a `.txt` file onto it. See
[`engine/README.md`](engine/README.md) for details and the file format.

## The Huffman idea, in four steps

1. **Count** how often each character appears.
2. **Build** the tree by repeatedly merging the two lowest-frequency nodes.
3. **Encode** each symbol with the path from the root to its leaf.
4. **Pack** the concatenated codes into bytes.

Frequent characters end up near the root and get the shortest codes, so the
total number of bits drops below the fixed 8 bits per character of plain text —
without ever losing information.

## Authors

Abdellah Hassani · Thibault Garcia-Megevand

## License

Released under the [MIT License](LICENSE).

---

## À propos (français)

**Compression de texte sans perte par codage de Huffman, calculée en direct dans
le navigateur.**

HuffZip transforme un texte en son encodage de Huffman et montre chaque étape :
la fréquence de chaque caractère, l'arbre binaire qui attribue des codes courts
aux symboles fréquents, la table des codes et le flux binaire obtenu. Rien n'est
envoyé sur un serveur — tout le calcul se fait sur votre machine, et chaque
compression est redécodée pour prouver qu'elle est sans perte.

Le projet est né d'un devoir universitaire de 2022 : un compresseur et un
décompresseur écrits en C, avec leurs propres listes chaînées et arbres
binaires, sans aucune bibliothèque. Ce moteur d'origine se trouve toujours dans
[`engine/`](engine/). L'application web reprend le même algorithme, réécrit en
JavaScript sans dépendance.

**En ligne** — l'application est déployée sur
[abdellah-hassani.fr/huffzip](https://abdellah-hassani.fr/huffzip).

**Lancer le site en local** — il est statique mais utilise des modules ES :
servez-le via un petit serveur local (`python3 -m http.server 8000`) plutôt
qu'en `file://`.

**Tester le moteur C** — dans `engine/`, lancez `./demo.sh mon_fichier.txt`
(macOS / Linux) ou double-cliquez sur `engine/huffzip.bat` (Windows, ou
glissez-y un `.txt`). Il compile, compresse, décompresse et vérifie que
l'original est reconstruit à l'identique.
