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

if [ -d "$REPO_DIR/build-mac/JohnnyCastaway.saver" ]; then
    echo "[*] Instalando JohnnyCastaway.saver em $SAVER_DIR..."
    rm -rf "$SAVER_DIR/JohnnyCastaway.saver"
    cp -R "$REPO_DIR/build-mac/JohnnyCastaway.saver" "$SAVER_DIR/"
    # Ad-hoc sign
    codesign --force --deep --sign - "$SAVER_DIR/JohnnyCastaway.saver" 2>/dev/null || true
    echo "[OK] Screensaver instalado!"
fi

if [ -f "$REPO_DIR/original/RESOURCE.MAP" ]; then
    echo "[*] Copiando RESOURCE.MAP e RESOURCE.001 para $ASSETS_DIR..."
    cp "$REPO_DIR/original/RESOURCE.MAP" "$ASSETS_DIR/"
    cp "$REPO_DIR/original/RESOURCE.001" "$ASSETS_DIR/"
    echo "[OK] Recursos copiados!"
else
    echo "[AVISO] Coloque RESOURCE.MAP e RESOURCE.001 em:"
    echo "       $ASSETS_DIR"
fi

echo ""
echo "Instalacao concluida. Ative o Johnny Castaway em Ajustes do Sistema > Bloqueio de Tela."
