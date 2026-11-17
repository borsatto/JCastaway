/*
 *  This file is part of 'Johnny Reborn'
 *
 *  An open-source engine for the classic
 *  'Johnny Castaway' screensaver by Sierra.
 *
 *  Copyright (C) 2019 Jeremie GUILLAUME
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "mytypes.h"
#include "utils.h"
#include "resource.h"
#include "respath.h"
#include "dump.h"
#include "graphics.h"
#include "events.h"
#include "sound.h"
#include "ttm.h"
#include "ads.h"
#include "story.h"


static int  argDump     = 0;
static int  argBench    = 0;
static int  argTtm      = 0;
static int  argAds      = 0;
static int  argPlayAll  = 0;
static int  argIsland   = 0;

static char *args[3];
static int  numArgs  = 0;


static void usage()
{
        printf("\n");
        printf(" Usage :\n");
        printf("         jc_reborn\n");
        printf("         jc_reborn help\n");
        printf("         jc_reborn version\n");
        printf("         jc_reborn dump\n");
        printf("         jc_reborn [<options>] bench\n");
        printf("         jc_reborn [<options>] ttm <TTM name>\n");
        printf("         jc_reborn [<options>] ads <ADS name> <ADS tag no>\n");
        printf("\n");
        printf(" Available options are:\n");
        printf("         window     - play in windowed mode\n");
        printf("         nosound    - quiet mode\n");
        printf("         island     - display the island as background for ADS play\n");
        printf("         debug      - print some debug info on stdout\n");
        printf("         hotkeys    - enable hot keys\n");
        printf("\n");
        printf(" While-playing hot-keys (if enabled):\n");
        printf("         Esc        - Terminate immediately\n");
        printf("         Alt+Return - Toggle full screen / windowed mode\n");
        printf("         Space      - Toggle pause / unpause\n");
        printf("         Return     - When paused, advance one frame\n");
        printf("         <M>        - toggle max / normal speed\n");
        printf("\n");
        exit(1);
}


static void version()
{
        printf("\n");
        printf("    Johnny Reborn, an open-source engine for\n");
        printf("    the classic Johnny Castaway screensaver by Sierra.\n");
        printf("    Development version Copyright (C) 2019 Jeremie GUILLAUME\n");
        printf("\n");
        exit(1);
}


static void showConfigDialog(void)
{
#ifdef _WIN32
    MessageBoxA(NULL,
        "Johnny Castaway Screensaver\n\n"
        "Assets necessarios: RESOURCE.MAP e RESOURCE.001\n"
        "Podem ser colocados em:\n"
        "  - %APPDATA%\\JohnnyCastaway\\\n"
        "  - Mesma pasta do JohnnyCastaway.scr\n\n"
        "Em modo interativo (janela):\n"
        "  Esc        : Sair\n"
        "  Alt+Return : Alternar tela cheia\n"
        "  Espaco     : Pausar/Continuar\n"
        "  M          : Velocidade maxima",
        "Johnny Castaway Settings",
        MB_OK | MB_ICONINFORMATION);
#else
    printf("Johnny Castaway Screensaver\n"
           "Configure assets in ~/.local/share/johnny-castaway/ or via JC_ASSETS_DIR.\n");
#endif
    exit(0);
}

static void parseArgs(int argc, char **argv)
{
    int numExpectedArgs = 0;

    for (int i=1; i < argc; i++) {

        if (numExpectedArgs) {
            args[numArgs++] = argv[i];
            numExpectedArgs--;
        }
        else {
            // Screensaver flags (Windows /s, /c, /p)
            if (!strcasecmp(argv[i], "/s") || !strcasecmp(argv[i], "-s")) {
                evScreensaverMode = 1;
                grWindowed = 0;
            }
            else if (!strncasecmp(argv[i], "/c", 2) || !strncasecmp(argv[i], "-c", 2)) {
                showConfigDialog();
            }
            else if (!strncasecmp(argv[i], "/p", 2) || !strncasecmp(argv[i], "-p", 2)) {
                const char *hwndStr = NULL;
                if (argv[i][2] == ':') {
                    hwndStr = &argv[i][3];
                } else if (i + 1 < argc && argv[i+1][0] != '/' && argv[i+1][0] != '-') {
                    hwndStr = argv[++i];
                }
                if (hwndStr) {
                    uintptr_t hwndVal = (uintptr_t)strtoull(hwndStr, NULL, 10);
                    grParentHwnd = (void*)hwndVal;
                    soundDisabled = 1;
                }
                evScreensaverMode = 1;
            }
            else if (!strcmp(argv[i], "-window-id")) {
                if (i + 1 < argc) {
                    SDL_setenv("SDL_WINDOWID", argv[++i], 1);
                    evScreensaverMode = 1;
                }
            }
            else if (!strcmp(argv[i], "--assets-dir") || !strcmp(argv[i], "-a")) {
                if (i + 1 < argc) setAssetDir(argv[++i]);
            }
            else if (!strcmp(argv[i], "--tick-ms") || !strcmp(argv[i], "--speed")) {
                if (i + 1 < argc) evTickDurationMs = atoi(argv[++i]);
            }
            else if (!strcmp(argv[i], "--xbrz")) {
                if (i + 1 < argc && argv[i+1][0] >= '2' && argv[i+1][0] <= '6') {
                    grXbrzFactor = atoi(argv[++i]);
                } else {
                    grXbrzFactor = 3; // Default 3x (1920x1440 Full HD)
                }
            }
            else if (!strcmp(argv[i], "--no-xbrz")) {
                grXbrzFactor = 1;
            }
            else if (!strcmp(argv[i], "--smooth")) {
                grSmoothFilter = 1;
            }
            else if (!strcmp(argv[i], "--retro") || !strcmp(argv[i], "--pixel")) {
                grSmoothFilter = 0;
                grXbrzFactor = 1;
            }
            else if (!strcmp(argv[i], "help") || !strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
                usage();
            }
            else if (!strcmp(argv[i], "version") || !strcmp(argv[i], "--version")) {
                version();
            }
            else if (!strcmp(argv[i], "dump")) {
                argDump = 1;
            }
            else if (!strcmp(argv[i], "bench")) {
                argBench = 1;
            }
            else if (!strcmp(argv[i], "ttm")) {
                argTtm = 1;
                numExpectedArgs = 1;
            }
            else if (!strcmp(argv[i], "ads")) {
                argAds = 1;
                numExpectedArgs = 2;
            }
            else if (!strcmp(argv[i], "window")) {
                grWindowed = 1;
            }
            else if (!strcmp(argv[i], "nosound")) {
                soundDisabled = 1;
            }
            else if (!strcmp(argv[i], "island")) {
                argIsland = 1;
            }
            else if (!strcmp(argv[i], "debug")) {
                debugMode = 1;
            }
            else if (!strcmp(argv[i], "hotkeys")) {
                evHotKeysEnabled = 1;
            }
        }
    }

    if (numExpectedArgs)
        usage();

    if (argDump + argBench + argTtm + argAds > 1)
        usage();

    if (argDump + argBench + argTtm + argAds == 0)
        argPlayAll = 1;

    const char *xscreenWin = getenv("XSCREENSAVER_WINDOW");
    if (xscreenWin && *xscreenWin && !grWindowed) {
        SDL_setenv("SDL_WINDOWID", xscreenWin, 1);
        evScreensaverMode = 1;
    }
}


int main(int argc, char **argv)
{
    parseArgs(argc, argv);

    if (argDump)
        debugMode = 1;

    if (!checkAssetsAvailable()) {
#ifdef _WIN32
        MessageBoxA(NULL,
            "Johnny Castaway: Arquivos de recursos nao encontrados!\n\n"
            "Coloque RESOURCE.MAP e RESOURCE.001 em:\n"
            "%APPDATA%\\JohnnyCastaway\\ ou ao lado do arquivo .scr.",
            "Johnny Castaway - Erro",
            MB_OK | MB_ICONERROR);
#else
        fprintf(stderr,
            "Error: RESOURCE.MAP not found.\n"
            "Please provide RESOURCE.MAP and RESOURCE.001 in:\n"
            "  - ~/.local/share/johnny-castaway/\n"
            "  - or set JC_ASSETS_DIR=/path/to/assets\n");
#endif
        return 1;
    }

    parseResourceFiles("RESOURCE.MAP");

    if (argPlayAll) {
        graphicsInit();
        soundInit();

        storyPlay();

        soundEnd();
        graphicsEnd();
    }

    else if (argDump) {
        dumpAllResources();
    }

    else if (argBench) {
        graphicsInit();
        adsPlayBench();
        graphicsEnd();
    }

    else if (argTtm) {
        graphicsInit();
        soundInit();

        adsPlaySingleTtm(args[0]);

        soundEnd();
        graphicsEnd();
    }

    else if (argAds) {

        graphicsInit();
        soundInit();

        if (argIsland)
            adsInitIsland();
        else
            adsNoIsland();

        adsPlay(args[0], atoi(args[1]));

        soundEnd();
        graphicsEnd();
    }

    return 0;
}

