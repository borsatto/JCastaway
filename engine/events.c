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

#include <SDL2/SDL.h>
#include "mytypes.h"
#include "graphics.h"
#include "events.h"


static uint32 lastTicks = 0x00ffffff;
static uint32 startTicks = 0;
static int paused   = 0;
static int maxSpeed = 0;
static int oneFrame = 0;
static int initialMouseX = -1;
static int initialMouseY = -1;

int evHotKeysEnabled = 0;
int evScreensaverMode = 0;
int evTickDurationMs = 16; // 16ms per tick (~60Hz), perfectly smooth animations


static void eventsProcessEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        switch(event.type) {

            case SDL_MOUSEMOTION:
                if (evScreensaverMode) {
                    uint32 now = SDL_GetTicks();
                    if (now - startTicks < 1500) {
                        // Startup grace period: continuously track mouse pos until window settles
                        initialMouseX = event.motion.x;
                        initialMouseY = event.motion.y;
                    }
                    else {
                        if (initialMouseX < 0) {
                            initialMouseX = event.motion.x;
                            initialMouseY = event.motion.y;
                        }
                        else {
                            int dx = abs(event.motion.x - initialMouseX);
                            int dy = abs(event.motion.y - initialMouseY);
                            if (dx > 25 || dy > 25) {
                                graphicsEnd();
                                exit(0);
                            }
                        }
                    }
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (evScreensaverMode) {
                    if (SDL_GetTicks() - startTicks > 1000) {
                        graphicsEnd();
                        exit(0);
                    }
                }
                break;

            case SDL_KEYDOWN:
                if (evScreensaverMode) {
                    if (SDL_GetTicks() - startTicks > 1000) {
                        graphicsEnd();
                        exit(0);
                    }
                    break;
                }

                if (evHotKeysEnabled) {

                    switch (event.key.keysym.sym) {

                        case SDLK_SPACE:
                            paused = !paused;
                            break;

                        case SDLK_m:
                            maxSpeed = !maxSpeed;
                            break;

                        case SDLK_w:
                            grToggleWidescreen();
                            break;

                        case SDLK_RETURN:
                            if (event.key.keysym.mod & KMOD_LALT) {
                                grToggleFullScreen();
                                oneFrame = 1;   // to redraw the window // TODO
                            }
                            else {
                                oneFrame = 1;
                            }
                            break;

                        case SDLK_ESCAPE:
                            graphicsEnd();
                            exit(0);
                            break;
                    }
                }
                else {
                    // Normal behaviour : no hot keys, the screen saver
                    // terminates if any key is pressed
                    graphicsEnd();
                    exit(0);
                }
                break;

            case SDL_WINDOWEVENT:
                grRefreshDisplay();
                break;

            case SDL_QUIT:
                graphicsEnd();
                exit(0);
                break;
        }
    }
}


void eventsInit()
{
    startTicks = SDL_GetTicks();
    lastTicks = startTicks;
    initialMouseX = -1;
    initialMouseY = -1;
}


void eventsWaitTick(uint16 delay)
{
    if (jcEngineStopRequested) return;

    delay *= evTickDurationMs;
    oneFrame = 0;

    eventsProcessEvents();

    while (!jcEngineStopRequested && ((paused && !oneFrame)
            || (!maxSpeed && (SDL_GetTicks() - lastTicks < delay)))) {
        SDL_Delay(1);
        eventsProcessEvents();
    }

    lastTicks = SDL_GetTicks();
}

