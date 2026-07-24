/*
 * HuffZip — Huffman coding core
 *
 * A dependency-free implementation of Huffman coding that runs entirely in the
 * browser. It exposes frequency analysis, tree construction, code generation,
 * encoding and decoding, plus the information-theory measures (entropy, average
 * code length) that let the interface show how close the codes get to the
 * theoretical limit.
 *
 * Symbols are Unicode code points, so accented text behaves predictably. Sizes
 * are reported assuming 8 bits per original symbol, which mirrors the
 * byte-oriented C engine this project grew out of.
 *
 * The analyze() entry point is written to stay fast on large inputs (a whole
 * book, a megabyte of text): it never materialises the full bitstream just to
 * measure it, and it proves losslessness in time proportional to the number of
 * distinct symbols rather than the length of the text.
 */

const BITS_PER_SYMBOL = 8;

/**
 * Count how often each symbol appears in the text.
 * Returns an array of { symbol, freq } kept in first-seen order.
 */
function countFrequencies(text) {
  const counts = new Map();
  for (const symbol of text) {
    counts.set(symbol, (counts.get(symbol) || 0) + 1);
  }
  return Array.from(counts, ([symbol, freq]) => ({ symbol, freq }));
}

/**
 * Build the Huffman tree from a frequency table.
 *
 * Two lowest-weight nodes are merged repeatedly until one remains. Ties are
 * broken by insertion order so the tree is deterministic for a given input —
 * the same text always draws the same tree.
 *
 * Node shape: { symbol, freq, left, right, id }. Leaves carry a symbol and no
 * children; internal nodes carry null symbol and two children.
 */
function buildTree(frequencies) {
  if (frequencies.length === 0) return null;

  let counter = 0;
  const nodes = frequencies.map((f) => ({
    symbol: f.symbol,
    freq: f.freq,
    left: null,
    right: null,
    id: counter++,
    order: counter,
  }));

  // A single distinct symbol still needs a one-bit code, handled in buildCodes.
  if (nodes.length === 1) return nodes[0];

  // Alphabet size is bounded (a few hundred symbols at most), so a linear scan
  // for the two smallest is simpler than a heap and plenty fast.
  const pool = nodes.slice();
  const pickSmallest = () => {
    let best = 0;
    for (let i = 1; i < pool.length; i++) {
      const a = pool[i];
      const b = pool[best];
      if (a.freq < b.freq || (a.freq === b.freq && a.order < b.order)) best = i;
    }
    return pool.splice(best, 1)[0];
  };

  while (pool.length > 1) {
    const left = pickSmallest();
    const right = pickSmallest();
    pool.push({
      symbol: null,
      freq: left.freq + right.freq,
      left,
      right,
      id: counter++,
      order: counter,
    });
  }
  return pool[0];
}

/**
 * Walk the tree and assign a binary code to every leaf.
 * Left edges are 0, right edges are 1. Returns a Map of symbol -> code string.
 */
function buildCodes(tree) {
  const codes = new Map();
  if (!tree) return codes;

  // A tree with a single leaf has no edges; give it the natural one-bit code.
  if (!tree.left && !tree.right) {
    codes.set(tree.symbol, '0');
    return codes;
  }

  const walk = (node, prefix) => {
    if (!node.left && !node.right) {
      codes.set(node.symbol, prefix);
      return;
    }
    if (node.left) walk(node.left, prefix + '0');
    if (node.right) walk(node.right, prefix + '1');
  };
  walk(tree, '');
  return codes;
}

/** Encode text into a string of '0'/'1' using the code table. */
function encode(text, codes) {
  const parts = [];
  for (const symbol of text) parts.push(codes.get(symbol));
  return parts.join('');
}

/** Decode a bitstring back into text by walking the tree edge by edge. */
function decode(bits, tree) {
  if (!tree) return '';

  // Single-leaf trees: every bit maps back to the one symbol.
  if (!tree.left && !tree.right) {
    return tree.symbol.repeat(bits.length);
  }

  const out = [];
  let node = tree;
  for (const bit of bits) {
    node = bit === '0' ? node.left : node.right;
    if (!node.left && !node.right) {
      out.push(node.symbol);
      node = tree;
    }
  }
  return out.join('');
}

/**
 * Shannon entropy of the source in bits per symbol — the theoretical floor no
 * lossless code can beat on average.
 */
function entropy(frequencies, total) {
  if (total === 0) return 0;
  let h = 0;
  for (const { freq } of frequencies) {
    const p = freq / total;
    h -= p * Math.log2(p);
  }
  return h;
}

/**
 * Prove the code table is lossless, in time proportional to the alphabet size.
 *
 * Huffman codes are prefix-free by construction (every symbol sits at its own
 * leaf), so decoding is unambiguous. It is therefore enough to confirm that
 * walking the tree along each symbol's own code lands back on that symbol —
 * if that holds for every symbol, encoding then decoding any text made of
 * those symbols reproduces it exactly.
 */
function verifyCodes(tree, codes) {
  if (!tree) return true;
  if (!tree.left && !tree.right) return true; // single symbol
  for (const [symbol, code] of codes) {
    let node = tree;
    for (const bit of code) {
      node = bit === '0' ? node.left : node.right;
      if (!node) return false;
    }
    if (node.left || node.right || node.symbol !== symbol) return false;
  }
  return true;
}

/**
 * Encode just enough of the text to fill `minBits` bits — used for the preview
 * strip, so a megabyte of input never builds a megabyte of bitstring.
 */
function encodePreview(text, codes, minBits) {
  const parts = [];
  let length = 0;
  for (const symbol of text) {
    const code = codes.get(symbol);
    parts.push(code);
    length += code.length;
    if (length >= minBits) break;
  }
  return parts.join('');
}

/**
 * Run the full pipeline on a piece of text and return everything the interface
 * needs: the tree, the code table, a preview of the encoded bits, and the
 * measured stats. Stays fast on large inputs — the compressed size comes from
 * summing code lengths, and `verified` is an O(alphabet) proof.
 */
function analyze(text, { previewBits = 1400 } = {}) {
  const frequencies = countFrequencies(text);
  const total = Array.from(text).length;
  const tree = buildTree(frequencies);
  const codes = buildCodes(tree);

  // Compressed size = sum of code lengths weighted by frequency — no big string.
  let compressedBits = 0;
  for (const { symbol, freq } of frequencies) {
    compressedBits += freq * codes.get(symbol).length;
  }

  const originalBits = total * BITS_PER_SYMBOL;
  const ratio = originalBits === 0 ? 0 : compressedBits / originalBits;
  const saved = originalBits === 0 ? 0 : 1 - ratio;

  return {
    frequencies: frequencies.slice().sort((a, b) => b.freq - a.freq),
    total,
    uniqueSymbols: frequencies.length,
    tree,
    codes,
    previewBits: encodePreview(text, codes, previewBits),
    compressedBits,
    originalBits,
    originalBytes: Math.ceil(originalBits / 8),
    compressedBytes: Math.ceil(compressedBits / 8),
    ratio,
    saved,
    entropy: entropy(frequencies, total),
    avgCodeLength: total === 0 ? 0 : compressedBits / total,
    verified: verifyCodes(tree, codes),
  };
}

export {
  BITS_PER_SYMBOL,
  countFrequencies,
  buildTree,
  buildCodes,
  encode,
  decode,
  entropy,
  verifyCodes,
  analyze,
};
