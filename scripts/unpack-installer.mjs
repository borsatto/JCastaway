// Unpacks InstallShield-3 style single-file archives (magic 65 5D 13 8C) whose payload is PKWARE DCL "implode".
import { readFileSync, writeFileSync, mkdirSync } from 'node:fs';
import { createHash } from 'node:crypto';
import { join, basename } from 'node:path';
import { explode } from 'node-pkware/simple';

const [, , outDir, ...files] = process.argv;
mkdirSync(outDir, { recursive: true });
for (const f of files) {
  const b = readFileSync(f);
  if (b.readUInt32LE(0) !== 0x8c135d65) { console.log(`${f}: not IS3 compressed`); continue; }
  const nameLen = b[0x1c];
  const name = b.subarray(0x1d, 0x1d + nameLen).toString('latin1');
  const off = 0x1d + nameLen + 1;
  const payload = b.subarray(off);
  const ab = payload.buffer.slice(payload.byteOffset, payload.byteOffset + payload.byteLength);
  const out = Buffer.from(explode(ab));
  const outName = name.toUpperCase() === 'SCRANTIC.EXE' ? 'SCRANTIC.SCR' : name.toUpperCase();
  writeFileSync(join(outDir, outName), out);
  console.log(`${basename(f)} -> ${outName} (stored as ${name}) ${out.length} bytes md5=${createHash('md5').update(out).digest('hex')}`);
}
