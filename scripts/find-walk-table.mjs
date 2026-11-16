// Locate jc_reborn's walk_data.h table inside a given SCRANTIC.SCR and compare it.
import { readFileSync } from 'node:fs';
const [, , hdr, exePath] = process.argv;
const rows = [...readFileSync(hdr, 'latin1').matchAll(/\{\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+)\s*\}/g)].map(m => m.slice(1).map(Number));
const buf = Buffer.alloc(rows.length * 6);
rows.forEach(([f, x, y, fr], i) => { buf.writeUInt16LE((f << 15) | fr, i * 6); buf.writeUInt16LE(x, i * 6 + 2); buf.writeUInt16LE(y, i * 6 + 4); });
const exe = readFileSync(exePath);
const probe = buf.subarray(0, 36);
const at = exe.indexOf(probe);
console.log(`rows=${rows.length} tableBytes=${buf.length} firstMatchAt=0x${at.toString(16)}`);
if (at >= 0) {
  let same = 0; for (let i = 0; i < buf.length; i++) if (exe[at + i] === buf[i]) same++;
  console.log(`identical bytes: ${same}/${buf.length}`, same === buf.length ? 'IDENTICAL' : 'DIFFERS');
}
