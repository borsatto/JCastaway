# jc-dev: reverse-engineering + cross-build environment for the Johnny Castaway screensaver.
#   - REA (rea-agents) + Ghidra 12.1.4 + JDK 21  -> analysis of SCRANTIC.SCR (Win16 NE)
#   - gcc/cmake/ninja + SDL2                      -> Linux build
#   - mingw-w64 + SDL2 mingw devel                -> Windows .scr cross-build
#   - Xvfb                                        -> headless smoke tests / frame captures
# macOS (.saver, arm64) is built on a macOS runner (see .github/workflows/build.yml).
FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive
ARG NODE_MAJOR=24
ARG GHIDRA_URL=https://github.com/NationalSecurityAgency/ghidra/releases/download/Ghidra_12.1.4_build/ghidra_12.1.4_PUBLIC_20260921.zip
ARG SDL2_VERSION=2.32.10
ARG USERNAME=dev
ARG UID=1000

RUN apt-get update && apt-get install -y --no-install-recommends \
        ca-certificates curl wget unzip xz-utils git gnupg procps file less vim-tiny \
        build-essential cmake ninja-build pkg-config gdb \
        libsdl2-dev \
        mingw-w64 \
        openjdk-21-jdk-headless \
        xvfb xauth imagemagick \
        python3 python3-pip python3-venv \
    && rm -rf /var/lib/apt/lists/*

# Node.js (REA requires >= 24.22.1 on the 24.x line)
RUN curl -fsSL https://deb.nodesource.com/setup_${NODE_MAJOR}.x | bash - \
    && apt-get install -y --no-install-recommends nodejs \
    && rm -rf /var/lib/apt/lists/* \
    && node --version

# Ghidra
RUN curl -fsSL -o /tmp/ghidra.zip "${GHIDRA_URL}" \
    && unzip -q /tmp/ghidra.zip -d /opt \
    && rm /tmp/ghidra.zip \
    && ln -s /opt/ghidra_* /opt/ghidra
ENV GHIDRA_INSTALL_DIR=/opt/ghidra \
    JAVA_HOME=/usr/lib/jvm/java-21-openjdk-amd64

# SDL2 for MinGW (x86_64-w64-mingw32 and i686-w64-mingw32)
RUN curl -fsSL -o /tmp/sdl2-mingw.tar.gz \
        "https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-devel-${SDL2_VERSION}-mingw.tar.gz" \
    && mkdir -p /opt/sdl2-mingw && tar -xzf /tmp/sdl2-mingw.tar.gz -C /opt/sdl2-mingw --strip-components=1 \
    && rm /tmp/sdl2-mingw.tar.gz
ENV SDL2_MINGW_DIR=/opt/sdl2-mingw

# REA CLI + MCP server
RUN npm install --global rea-agents@latest node-pkware@6 && rea --help >/dev/null

# Non-root user (REA's process ownership checks behave better as non-root)
RUN (id -u ${UID} >/dev/null 2>&1 && userdel -r "$(id -un ${UID})" || true) \
    && useradd -m -u ${UID} -s /bin/bash ${USERNAME}
USER ${USERNAME}
WORKDIR /work

# /work   : this repository (bind mount)
# /assets : original Johnny Castaway files (RESOURCE.MAP, RESOURCE.001, SCRANTIC.SCR) - never baked into the image
VOLUME ["/work", "/assets"]
ENV JC_ASSETS_DIR=/assets

CMD ["bash"]
