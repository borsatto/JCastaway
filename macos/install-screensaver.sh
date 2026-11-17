#!/bin/bash
set -e

echo "=========================================================="
echo "       Johnny Castaway macOS Screensaver Installer"
echo "=========================================================="

SAVER_DIR="$HOME/Library/Screen Savers"
ASSETS_DIR="$HOME/Library/Application Support/Johnny Castaway"

mkdir -p "$SAVER_DIR"
mkdir -p "$ASSETS_DIR"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_DIR="$(dirname "$SCRIPT_DIR")"

SAVER_SRC=""
if [ -d "$REPO_DIR/build/JohnnyCastaway.saver" ]; then
    SAVER_SRC="$REPO_DIR/build/JohnnyCastaway.saver"
elif [ -d "$REPO_DIR/build-mac/JohnnyCastaway.saver" ]; then
    SAVER_SRC="$REPO_DIR/build-mac/JohnnyCastaway.saver"
fi

if [ -n "$SAVER_SRC" ]; then
    echo "[*] Instalando JohnnyCastaway.saver em $SAVER_DIR..."
    rm -rf "$SAVER_DIR/JohnnyCastaway.saver"
    cp -R "$SAVER_SRC" "$SAVER_DIR/"
    # Ad-hoc sign
    codesign --force --deep --sign - "$SAVER_DIR/JohnnyCastaway.saver" 2>/dev/null || true
    echo "[OK] Screensaver instalado com sucesso!"
else
    echo "[ERRO] JohnnyCastaway.saver nao encontrado em build/ nem em build-mac/."
    echo "       Compile primeiro executando: cmake -B build -S . && cmake --build build"
    exit 1
fi

if [ -f "$REPO_DIR/original/RESOURCE.MAP" ]; then
    echo "[*] Copiando recursos e audios para $ASSETS_DIR..."
    cp "$REPO_DIR/original/RESOURCE.MAP" "$ASSETS_DIR/"
    cp "$REPO_DIR/original/RESOURCE.001" "$ASSETS_DIR/"
    if [ -f "$REPO_DIR/original/sound0.wav" ]; then
        cp "$REPO_DIR/original/sound"*.wav "$ASSETS_DIR/"
    fi
    echo "[OK] Recursos e efeitos sonoros copiados!"
else
    echo "[AVISO] Coloque RESOURCE.MAP e RESOURCE.001 em:"
    echo "       $ASSETS_DIR"
fi

echo ""
echo "Instalacao concluida. Ative o Johnny Castaway em Ajustes do Sistema > Bloqueio de Tela."
