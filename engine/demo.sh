#!/usr/bin/env bash
# ============================================================
#  HuffZip - moteur C : compile, compresse, decompresse, verifie.
#  Usage : ./demo.sh [chemin/vers/fichier.txt]
# ============================================================
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
BIN="$ROOT/bin"
WORK="$ROOT/work"
mkdir -p "$BIN" "$WORK"

if ! command -v gcc >/dev/null 2>&1; then
  echo "[ERREUR] gcc introuvable. Installe-le (build-essential, xcode-select --install, ...)."
  exit 1
fi

echo "Compilation du moteur..."
gcc -O2 -o "$BIN/compresser"   "$ROOT"/compression/*.c
gcc -O2 -o "$BIN/decompresser" "$ROOT/decompression/Decompression.c"
echo "  OK"

INPUT="${1:-}"
if [ -z "$INPUT" ]; then
  read -rp "Chemin du fichier .txt : " INPUT
fi
if [ ! -f "$INPUT" ]; then
  echo "[ERREUR] Fichier introuvable : $INPUT"
  exit 1
fi

cp "$INPUT" "$WORK/Fichier a compresser.txt"
( cd "$WORK" && echo "Compression..."   && "$BIN/compresser" )
( cd "$WORK" && echo "Decompression..." && "$BIN/decompresser" )

echo "------------------------------------------------------------"
if cmp -s "$WORK/Fichier a compresser.txt" "$WORK/Fichier Decompresse.txt"; then
  echo "[OK] Aller-retour sans perte : l'original est reconstruit a l'identique."
else
  echo "[ECHEC] Le fichier decompresse differe de l'original."
  exit 1
fi
echo "  Taille originale  : $(wc -c < "$WORK/Fichier a compresser.txt") octets"
echo "  Taille compressee : $(wc -c < "$WORK/Fichier Compresse.txt") octets"
echo "  Fichiers produits dans : $WORK"
echo "------------------------------------------------------------"
