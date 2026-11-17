#!/usr/bin/env node
/**
 * Extracts 23 original WAVE audio sound effects from 16-bit SCRANTIC.SCR
 */
import fs from 'node:fs';
import path from 'node:path';

const scrPath = process.argv[2] || 'original/SCRANTIC.SCR';
if (!fs.existsSync(scrPath)) {
  console.error(`SCRANTIC.SCR not found at ${scrPath}`);
  process.exit(1);
}

const buf = fs.readFileSync(scrPath);
const neOffset = buf.readUInt32LE(0x3c);
const resTableOffset = neOffset + buf.readUInt16LE(neOffset + 0x24);
const shiftCount = buf.readUInt16LE(resTableOffset);

let p = resTableOffset + 2;
const waves = [];

while (true) {
  const typeId = buf.readUInt16LE(p);
  if (typeId === 0) break;
  const count = buf.readUInt16LE(p + 2);
  p += 8;
  if (typeId === 0xf005) { // WAVE resource type
    for (let i = 0; i < count; i++) {
      const offset = buf.readUInt16LE(p) << shiftCount;
      const length = buf.readUInt16LE(p + 2) << shiftCount;
      const id = buf.readUInt16LE(p + 6) & 0x7fff;
      const riffMagic = buf.toString('ascii', offset, offset + 4);
      if (riffMagic === 'RIFF') {
        const actualSize = buf.readUInt32LE(offset + 4) + 8;
        waves.push({ id, offset, actualSize });
      }
      p += 12;
    }
  } else {
    p += count * 12;
  }
}

waves.sort((a, b) => a.id - b.id);
console.log(`Found ${waves.length} WAVE files:`);

const destDirs = ['original'];
if (process.env.APPDATA) {
  destDirs.push(path.join(process.env.APPDATA, 'JohnnyCastaway'));
}

for (const d of destDirs) {
  if (!fs.existsSync(d)) fs.mkdirSync(d, { recursive: true });
}

waves.forEach((w, idx) => {
  const wavData = buf.slice(w.offset, w.offset + w.actualSize);
  const outName = `sound${idx}.wav`;
  for (const d of destDirs) {
    fs.writeFileSync(path.join(d, outName), wavData);
  }
  console.log(`  Extracted ${outName} (${wavData.length} bytes, ID ${w.id})`);
});

console.log('Audio extraction complete.');
