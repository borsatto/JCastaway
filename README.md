# Johnny Castaway: screensaver multiplataforma

Recriação do clássico *Screen Antics: Johnny Castaway* (Dynamix/Sierra, 1992) como screensaver nativo para **Windows 10/11 (.scr)**, **Linux (xscreensaver / standalone)** e **macOS Apple Silicon (.saver)**.

- Engine: fork do [jc_reborn](https://github.com/jno6809/jc_reborn) de jno6809 (C99 + SDL2, GPL-3), em `engine/`, commit de origem `524a580`.
- Engenharia reversa do executável original com [REA](https://github.com/morluto/rea) + Ghidra: ver [docs/re-notes.md](docs/re-notes.md).

## Assets originais

Este repositório **não contém nem distribui** os arquivos da Sierra. Você precisa fornecer a sua cópia:

```
original/RESOURCE.MAP
original/RESOURCE.001
original/SCRANTIC.SCR   (só para engenharia reversa)
```

Se você tem o instalador de 1992 (arquivos `*.SC$`, `RESOURCE.00$`):

```bash
cd scripts && npm install
node unpack-installer.mjs ../original ../jc/RESOURCE.00$ ../jc/SCRANTIC.SC$
cp ../jc/RESOURCE.MAP ../original/
```

## Ambiente de desenvolvimento (container)

```bash
docker buildx build --platform linux/amd64 -t jc-dev:latest docker   # ou: docker compose build
docker compose up -d
docker compose exec jc-dev bash
```

Dentro do container:

```bash
# Linux
cmake -S . -B build-linux -G Ninja && cmake --build build-linux
# Windows (.scr)
cmake -S . -B build-win64 -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-w64-x86_64.cmake && cmake --build build-win64
# Teste headless
cd /assets && xvfb-run -s "-screen 0 640x480x24" /work/build-linux/johnny window
# REA
rea doctor --json
```

O build do macOS (`.saver` arm64) roda num runner macOS (GitHub Actions `macos-14`).

## Licença

GPL-3.0 (herdada do jc_reborn). *Johnny Castaway* e *Screen Antics* são marcas e obras da Sierra/Dynamix (hoje Activision); este projeto não é afiliado a elas.
