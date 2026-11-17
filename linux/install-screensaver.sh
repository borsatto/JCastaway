#!/bin/bash
set -e

echo "=========================================================="
echo "       Johnny Castaway Linux Screensaver Installer"
echo "=========================================================="

BIN_DIR="$HOME/.local/bin"
ASSETS_DIR="$HOME/.local/share/johnny-castaway"
APP_DIR="$HOME/.local/share/applications"

mkdir -p "$BIN_DIR"
mkdir -p "$ASSETS_DIR"
mkdir -p "$APP_DIR"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_DIR="$(dirname "$SCRIPT_DIR")"

if [ -f "$REPO_DIR/build-linux/johnny" ]; then
    echo "[*] Instalando executavel johnny em $BIN_DIR..."
    cp "$REPO_DIR/build-linux/johnny" "$BIN_DIR/"
    chmod +x "$BIN_DIR/johnny"
    echo "[OK] Executavel instalado!"
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

cp "$SCRIPT_DIR/johnny-castaway.desktop" "$APP_DIR/"
echo "[OK] Atalho .desktop registrado!"

echo ""
echo "Instalacao concluida! Execute com: johnny"
