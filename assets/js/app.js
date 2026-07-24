/*
 * HuffZip — interface
 * Wires the Huffman core to the page: live analysis on every keystroke, the
 * tree drawing, the code table, the bitstream, and the FR/EN switch.
 */

import { analyze } from './huffman.js';

/* ------------------------------------------------------------------
   Translations
   ------------------------------------------------------------------ */

const I18N = {
  fr: {
    'nav.how': 'Comment ça marche',
    'nav.about': 'À propos',
    'hero.eyebrow': 'Codage de Huffman · démonstration interactive',
    'hero.title': 'Chaque caractère mérite <span class="accent">sa juste longueur</span>.',
    'hero.lede': "Tapez un texte : HuffZip compte les caractères, construit l'arbre de Huffman et attribue les codes les plus courts aux symboles les plus fréquents. Compression sans perte, calculée en direct dans votre navigateur.",
    'studio.input': 'Texte source',
    'studio.readout': 'Résultat',
    'studio.verified': 'aller-retour vérifié',
    'studio.verifyFail': 'échec de vérification',
    'studio.chars': 'caractères',
    'studio.unique': 'symboles',
    'studio.import': 'Importer un fichier .txt',
    'studio.dropHint': 'ou glissez-déposez un .txt ici',
    'studio.errText': 'Format non pris en charge : choisissez un fichier texte (.txt).',
    'studio.saved': 'de taille en moins',
    'studio.original': 'Origine',
    'studio.compressed': 'Huffman',
    'metric.avg': 'Longueur moyenne',
    'metric.entropy': 'Entropie (limite)',
    'metric.bitsSym': 'bits/symb.',
    'metric.origBytes': "Poids d'origine",
    'metric.compBytes': 'Poids compressé',
    'tree.eyebrow': 'La structure',
    'tree.title': "L'arbre de Huffman",
    'tree.desc': "On fusionne à répétition les deux symboles les plus rares. Le chemin de la racine à une feuille est son code : à gauche on écrit 0, à droite 1. Les feuilles proches de la racine — les caractères fréquents — obtiennent les codes les plus courts.",
    'tree.left': 'à gauche = 0',
    'tree.right': 'à droite = 1',
    'tree.empty': 'Tapez un texte pour construire l’arbre.',
    'codes.eyebrow': 'La correspondance',
    'codes.title': 'Table des codes',
    'codes.desc': "Aucun code n'est le préfixe d'un autre : la suite de bits se relit sans ambiguïté ni séparateur. Les symboles les plus fréquents apparaissent en premier.",
    'stream.eyebrow': 'Le résultat brut',
    'stream.title': 'Le flux binaire',
    'stream.desc': "Chaque caractère remplacé par son code, bout à bout. C'est cette suite que l'on regroupe en octets pour l'écrire sur le disque.",
    'stream.more': 'bits supplémentaires',
    'how.eyebrow': 'Le principe',
    'how.title': 'Quatre étapes',
    'how.s1.t': 'Compter', 'how.s1.d': "On relève le nombre d'occurrences de chaque caractère du texte.",
    'how.s2.t': 'Construire', 'how.s2.d': "On fusionne les deux nœuds les plus rares, encore et encore, jusqu'à un seul arbre.",
    'how.s3.t': 'Coder', 'how.s3.d': 'Le chemin vers chaque feuille donne un code binaire, court pour les fréquents.',
    'how.s4.t': 'Empaqueter', 'how.s4.d': 'On concatène les codes et on regroupe les bits en octets écrits sur le disque.',
    'about.eyebrow': 'À propos',
    'about.title': "D'un projet C à une démo web",
    'about.p1': "HuffZip est né d'un projet universitaire de 2022 : un compresseur et un décompresseur écrits en C, avec leurs propres listes chaînées et arbres binaires, sans aucune bibliothèque. Le code d'origine vit toujours dans le dépôt, sous <span class=\"foot mono\">engine/</span>.",
    'about.p2': "Cette interface reprend le même algorithme, réécrit en JavaScript pour s'exécuter dans le navigateur. Rien n'est envoyé à un serveur : tout le calcul se fait sur votre machine, et chaque compression est revérifiée par décodage pour garantir qu'elle est sans perte.",
    'about.f1k': 'Algorithme',
    'about.f2k': 'Type', 'about.f2v': 'Sans perte',
    'about.f3k': "Moteur d'origine",
    'about.f4k': 'Interface', 'about.f4v': 'JavaScript, sans dépendance',
    'about.f5k': 'Calcul', 'about.f5v': '100 % côté client',
    'foot.tag': 'codage de Huffman, en direct',
    'foot.legal': 'Mentions légales',
    'foot.credit': 'Abdellah Hassani · Thibault Garcia-Megevand',
  },
  en: {
    'nav.how': 'How it works',
    'nav.about': 'About',
    'hero.eyebrow': 'Huffman coding · interactive demo',
    'hero.title': 'Every character earns <span class="accent">its own length</span>.',
    'hero.lede': 'Type some text: HuffZip counts the characters, builds the Huffman tree, and hands the shortest codes to the most frequent symbols. Lossless compression, computed live in your browser.',
    'studio.input': 'Source text',
    'studio.readout': 'Result',
    'studio.verified': 'round-trip verified',
    'studio.verifyFail': 'verification failed',
    'studio.chars': 'characters',
    'studio.unique': 'symbols',
    'studio.import': 'Import a .txt file',
    'studio.dropHint': 'or drop a .txt here',
    'studio.errText': 'Unsupported format: please choose a text file (.txt).',
    'studio.saved': 'smaller',
    'studio.original': 'Original',
    'studio.compressed': 'Huffman',
    'metric.avg': 'Average length',
    'metric.entropy': 'Entropy (limit)',
    'metric.bitsSym': 'bits/sym.',
    'metric.origBytes': 'Original size',
    'metric.compBytes': 'Compressed size',
    'tree.eyebrow': 'The structure',
    'tree.title': 'The Huffman tree',
    'tree.desc': 'The two rarest symbols are merged over and over. The path from the root to a leaf is its code: left writes 0, right writes 1. Leaves near the root — the frequent characters — get the shortest codes.',
    'tree.left': 'left = 0',
    'tree.right': 'right = 1',
    'tree.empty': 'Type some text to build the tree.',
    'codes.eyebrow': 'The mapping',
    'codes.title': 'Code table',
    'codes.desc': 'No code is a prefix of another, so the bitstream reads back with no ambiguity and no separators. The most frequent symbols come first.',
    'stream.eyebrow': 'The raw output',
    'stream.title': 'The bitstream',
    'stream.desc': 'Each character replaced by its code, end to end. This is the sequence that gets packed into bytes and written to disk.',
    'stream.more': 'more bits',
    'how.eyebrow': 'The idea',
    'how.title': 'Four steps',
    'how.s1.t': 'Count', 'how.s1.d': 'Tally how many times each character appears in the text.',
    'how.s2.t': 'Build', 'how.s2.d': 'Merge the two rarest nodes, again and again, down to a single tree.',
    'how.s3.t': 'Encode', 'how.s3.d': 'The path to each leaf gives a binary code, short for the frequent ones.',
    'how.s4.t': 'Pack', 'how.s4.d': 'Concatenate the codes and group the bits into bytes written to disk.',
    'about.eyebrow': 'About',
    'about.title': 'From a C project to a web demo',
    'about.p1': 'HuffZip grew out of a 2022 university project: a compressor and decompressor written in C, with their own linked lists and binary trees, no libraries at all. The original code still lives in the repository under <span class="foot mono">engine/</span>.',
    'about.p2': 'This interface reuses the same algorithm, rewritten in JavaScript to run in the browser. Nothing is sent to a server: every computation happens on your machine, and each compression is re-decoded to prove it is lossless.',
    'about.f1k': 'Algorithm',
    'about.f2k': 'Type', 'about.f2v': 'Lossless',
    'about.f3k': 'Original engine',
    'about.f4k': 'Interface', 'about.f4v': 'JavaScript, no dependencies',
    'about.f5k': 'Computation', 'about.f5v': '100% client-side',
    'foot.tag': 'Huffman coding, live',
    'foot.legal': 'Legal notice',
    'foot.credit': 'Abdellah Hassani · Thibault Garcia-Megevand',
  },
};

const SAMPLES = [
  {
    label: { fr: 'Proverbe', en: 'Proverb' },
    text: {
      fr: 'petit à petit, l’oiseau fait son nid. petit à petit, la lettre trouve son code.',
      en: 'little by little, the bird builds its nest. little by little, the letter finds its code.',
    },
  },
  {
    label: { fr: 'ADN', en: 'DNA' },
    text: {
      fr: 'GATTACAGATTACAGGGTTAAACCCGATTACATTTTGGGGCCCCAAAAGATTACAGGGA',
      en: 'GATTACAGATTACAGGGTTAAACCCGATTACATTTTGGGGCCCCAAAAGATTACAGGGA',
    },
  },
  {
    label: { fr: 'Binaire', en: 'Binary' },
    text: {
      fr: '1101001011101010001110101010010111010000111010101110100101',
      en: '1101001011101010001110101010010111010000111010101110100101',
    },
  },
  {
    label: { fr: 'Paragraphe', en: 'Paragraph' },
    text: {
      fr: "L'entropie mesure la quantité minimale d'information nécessaire pour décrire une source. Huffman s'en approche symbole par symbole, sans jamais la dépasser vers le bas.",
      en: 'Entropy measures the least amount of information needed to describe a source. Huffman approaches it symbol by symbol, and can never do better on average.',
    },
  },
];

/* ------------------------------------------------------------------
   Small helpers
   ------------------------------------------------------------------ */

const $ = (sel) => document.querySelector(sel);

const escapeXml = (s) =>
  s.replace(/[&<>"']/g, (c) => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' }[c]));

// Make invisible characters visible in the tree and code table.
function showSymbol(sym) {
  const map = { ' ': '␣', '\n': '⏎', '\t': '⇥', '\r': '⏎' };
  return map[sym] || sym;
}

// Wrap a code string so 0s are teal and 1s are amber.
function colorBits(code) {
  let out = '';
  for (const b of code) out += b === '0' ? '<span class="z">0</span>' : '<span class="o">1</span>';
  return out;
}

/* ------------------------------------------------------------------
   Tree drawing
   ------------------------------------------------------------------ */

function drawTree(tree, lang) {
  const svg = $('#tree-svg');

  if (!tree) {
    svg.setAttribute('viewBox', '0 0 400 120');
    svg.setAttribute('height', '120');
    svg.innerHTML = `<text x="200" y="64" text-anchor="middle" class="node-freq">${escapeXml(I18N[lang]['tree.empty'])}</text>`;
    return;
  }

  const GAP_X = 52, GAP_Y = 64, MARGIN = 26, R = 7, LEAF_W = 40, LEAF_H = 36;
  let leafCount = 0;
  let maxDepth = 0;

  // Post-order pass: leaves get sequential x, internal nodes centre on children.
  (function place(node, depth) {
    node._depth = depth;
    maxDepth = Math.max(maxDepth, depth);
    if (!node.left && !node.right) {
      node._x = leafCount++;
      return node._x;
    }
    const lx = place(node.left, depth + 1);
    const rx = place(node.right, depth + 1);
    node._x = (lx + rx) / 2;
    return node._x;
  })(tree, 0);

  const numLeaves = Math.max(leafCount, 1);
  const px = (x) => MARGIN + x * GAP_X + LEAF_W / 2;
  const py = (d) => MARGIN + d * GAP_Y + LEAF_H / 2;
  const width = MARGIN * 2 + (numLeaves - 1) * GAP_X + LEAF_W;
  const height = MARGIN * 2 + maxDepth * GAP_Y + LEAF_H;

  const edges = [];
  const nodes = [];

  (function render(node) {
    const x = px(node._x), y = py(node._depth);
    for (const [child, bit] of [[node.left, '0'], [node.right, '1']]) {
      if (!child) continue;
      const cx = px(child._x), cy = py(child._depth);
      const cls = bit === '0' ? 'b0' : 'b1';
      edges.push(`<line class="edge ${cls}" x1="${x.toFixed(1)}" y1="${y.toFixed(1)}" x2="${cx.toFixed(1)}" y2="${cy.toFixed(1)}"/>`);
      const lx = x + (cx - x) * 0.4, ly = y + (cy - y) * 0.4;
      edges.push(`<text class="edge-label ${cls}" x="${lx.toFixed(1)}" y="${(ly - 3).toFixed(1)}" text-anchor="middle">${bit}</text>`);
      render(child);
    }
    if (!node.left && !node.right) {
      nodes.push(
        `<g><rect class="node-leaf" x="${(x - LEAF_W / 2).toFixed(1)}" y="${(y - LEAF_H / 2).toFixed(1)}" width="${LEAF_W}" height="${LEAF_H}"/>` +
        `<text class="node-sym" x="${x.toFixed(1)}" y="${(y - 1).toFixed(1)}" text-anchor="middle">${escapeXml(showSymbol(node.symbol))}</text>` +
        `<text class="node-freq" x="${x.toFixed(1)}" y="${(y + 12).toFixed(1)}" text-anchor="middle">${node.freq}</text></g>`
      );
    } else {
      nodes.push(`<rect class="node-internal" x="${(x - R).toFixed(1)}" y="${(y - R).toFixed(1)}" width="${(R * 2)}" height="${(R * 2)}"/>`);
    }
  })(tree);

  // Scale the whole drawing to fit the available box in both dimensions, never
  // upscaling past its natural size. The viewBox keeps internal coordinates
  // intact; only the rendered size changes, so the tree always fits the screen.
  const container = svg.parentElement;
  const avail = Math.max(240, (container ? container.clientWidth : 900) - 20);
  const maxH = Math.min(560, (typeof window !== 'undefined' ? window.innerHeight : 800) * 0.72);
  const scale = Math.min(1, avail / width, maxH / height);

  svg.setAttribute('viewBox', `0 0 ${width.toFixed(0)} ${height.toFixed(0)}`);
  svg.setAttribute('preserveAspectRatio', 'xMidYMid meet');
  svg.style.width = `${(width * scale).toFixed(0)}px`;
  svg.style.height = `${(height * scale).toFixed(0)}px`;
  svg.innerHTML = edges.join('') + nodes.join('');
}

/* ------------------------------------------------------------------
   Render everything from an analysis
   ------------------------------------------------------------------ */

let currentLang = localStorage.getItem('huffzip-lang') || 'fr';

function render() {
  const text = $('#editor').value;
  const r = analyze(text);
  const t = I18N[currentLang];

  // counts
  $('#char-count').textContent = r.total.toLocaleString(currentLang);
  $('#unique-count').textContent = r.uniqueSymbols;

  // headline + gauge
  $('#saved-pct').textContent = `${Math.round(r.saved * 100)}%`;
  $('#orig-val').textContent = `${r.originalBits.toLocaleString(currentLang)} b`;
  $('#comp-val').textContent = `${r.compressedBits.toLocaleString(currentLang)} b`;
  $('#comp-bar').style.width = `${(r.ratio * 100).toFixed(1)}%`;

  // metrics
  $('#avg-len').textContent = r.avgCodeLength.toFixed(2);
  $('#entropy').textContent = r.entropy.toFixed(2);
  $('#orig-bytes').textContent = r.originalBytes.toLocaleString(currentLang);
  $('#comp-bytes').textContent = r.compressedBytes.toLocaleString(currentLang);

  // verify badge
  const verify = $('#verify');
  const ok = r.total === 0 || r.verified;
  verify.classList.toggle('bad', !ok);
  verify.querySelector('span:last-child').textContent = ok ? t['studio.verified'] : t['studio.verifyFail'];

  // tree
  drawTree(r.tree, currentLang);

  // code table
  const grid = $('#code-grid');
  grid.innerHTML = r.frequencies
    .map((f) => {
      const code = r.codes.get(f.symbol) || '';
      return (
        `<div class="code-card"><span class="code-sym">${escapeXml(showSymbol(f.symbol))}` +
        `<span class="freq">×${f.freq}</span></span>` +
        `<span class="code-bits">${colorBits(code)}</span></div>`
      );
    })
    .join('');

  // bitstream, grouped into bytes (capped so very long inputs stay responsive).
  // The spaces between byte groups are what let the line wrap.
  const CAP = 1400;
  const shown = r.previewBits.slice(0, CAP);
  const bytes = [];
  for (let i = 0; i < shown.length; i += 8) {
    bytes.push(`<span class="byte">${colorBits(shown.slice(i, i + 8))}</span>`);
  }
  let streamHtml = bytes.join(' ');
  const remaining = r.compressedBits - shown.length;
  if (remaining > 0) {
    streamHtml += ` <span class="stream-more">+${remaining.toLocaleString(currentLang)} ${t['stream.more']}</span>`;
  }
  $('#stream').innerHTML = streamHtml;
}

/* ------------------------------------------------------------------
   Language
   ------------------------------------------------------------------ */

function applyLang(lang) {
  currentLang = lang;
  localStorage.setItem('huffzip-lang', lang);
  document.documentElement.lang = lang;

  document.querySelectorAll('[data-i18n]').forEach((el) => {
    if (el.dataset.locked) return; // e.g. a loaded file name, not a translatable string
    const key = el.getAttribute('data-i18n');
    if (I18N[lang][key] !== undefined) el.innerHTML = I18N[lang][key];
  });

  document.querySelectorAll('.lang-toggle button').forEach((b) => {
    b.setAttribute('aria-pressed', String(b.dataset.lang === lang));
  });

  renderSamples();
  render();
}

function renderSamples() {
  const box = $('#samples');
  box.innerHTML = '';
  SAMPLES.forEach((s) => {
    const btn = document.createElement('button');
    btn.className = 'chip';
    btn.type = 'button';
    btn.textContent = s.label[currentLang];
    btn.addEventListener('click', () => {
      $('#editor').value = s.text[currentLang];
      render();
    });
    box.appendChild(btn);
  });
}

/* ------------------------------------------------------------------
   Wire up
   ------------------------------------------------------------------ */

let raf = 0;
$('#editor').addEventListener('input', () => {
  cancelAnimationFrame(raf);
  raf = requestAnimationFrame(render);
});

document.querySelectorAll('.lang-toggle button').forEach((b) => {
  b.addEventListener('click', () => applyLang(b.dataset.lang));
});

// Re-fit the tree when the window is resized.
let rz = 0;
window.addEventListener('resize', () => {
  clearTimeout(rz);
  rz = setTimeout(render, 150);
});

/* ---- .txt import: file picker + drag & drop ---- */
const editorEl = $('#editor');
const fileInput = $('#file-input');
const ioHint = $('#io-hint');

function humanSize(bytes) {
  if (bytes < 1024) return `${bytes} o`;
  if (bytes < 1024 * 1024) return `${Math.round(bytes / 1024)} Ko`;
  return `${(bytes / (1024 * 1024)).toFixed(1)} Mo`;
}

function loadTextFile(file) {
  const isText = (file.type && file.type.startsWith('text/')) || /\.txt$/i.test(file.name);
  if (!isText) {
    ioHint.classList.add('io-err');
    ioHint.dataset.locked = '';
    delete ioHint.dataset.locked;
    ioHint.textContent = I18N[currentLang]['studio.errText'];
    return;
  }
  const reader = new FileReader();
  reader.onload = () => {
    editorEl.value = typeof reader.result === 'string' ? reader.result : '';
    ioHint.classList.remove('io-err');
    ioHint.textContent = `${file.name} · ${humanSize(file.size)}`;
    ioHint.dataset.locked = '1'; // keep the file name through a language switch
    render();
  };
  reader.onerror = () => {
    ioHint.classList.add('io-err');
    ioHint.textContent = I18N[currentLang]['studio.errText'];
  };
  reader.readAsText(file);
}

fileInput.addEventListener('change', () => {
  if (fileInput.files && fileInput.files[0]) loadTextFile(fileInput.files[0]);
  fileInput.value = ''; // allow re-selecting the same file
});

['dragenter', 'dragover'].forEach((ev) =>
  editorEl.addEventListener(ev, (e) => { e.preventDefault(); editorEl.classList.add('dragging'); })
);
['dragleave', 'dragend'].forEach((ev) =>
  editorEl.addEventListener(ev, () => editorEl.classList.remove('dragging'))
);
editorEl.addEventListener('drop', (e) => {
  e.preventDefault();
  editorEl.classList.remove('dragging');
  const f = e.dataTransfer && e.dataTransfer.files && e.dataTransfer.files[0];
  if (f) loadTextFile(f);
});

// First paint: seed the editor with a sample in the saved language.
$('#editor').value = SAMPLES[0].text[currentLang];
applyLang(currentLang);
