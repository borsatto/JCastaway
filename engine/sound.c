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

#include <SDL2/SDL.h>
#include <string.h>
#include <stdlib.h>

#include "mytypes.h"
#include "utils.h"
#include "sound.h"
#include "respath.h"


#define NUM_OF_SOUNDS  25


struct TSound {
    uint32  length;
    uint8   *data;
};


int soundDisabled = 0;


static struct TSound sounds[NUM_OF_SOUNDS];
static struct TSound *currentSound;

static uint8  *currentPtr = NULL;
static uint32 currentRemaining = 0;
static SDL_AudioSpec deviceSpec;


static void soundCallback(void *userdata, uint8 *stream, int rqdLen)
{
    SDL_memset(stream, 0, rqdLen);

    if (currentRemaining > 0 && currentPtr != NULL) {
        int toMix = (currentRemaining > (uint32)rqdLen) ? rqdLen : (int)currentRemaining;
        SDL_MixAudioFormat(stream, currentPtr, deviceSpec.format, toMix, SDL_MIX_MAXVOLUME);
        currentPtr += toMix;
        currentRemaining -= toMix;
    }
}


void soundInit()
{
    if (soundDisabled)
        return;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        debugMsg("SDL init audio error: %s", SDL_GetError());
        soundDisabled = 1;
        return;
    }

    SDL_AudioSpec wanted;
    SDL_zero(wanted);
    wanted.freq     = 44100;
    wanted.format   = AUDIO_S16SYS;
    wanted.channels = 2;
    wanted.samples  = 1024;
    wanted.callback = soundCallback;

    if (SDL_OpenAudio(&wanted, &deviceSpec) < 0) {
        debugMsg("SDL_OpenAudio() error: %s", SDL_GetError());
        soundDisabled = 1;
        return;
    }

    for (int i=0; i < NUM_OF_SOUNDS; i++) {
        sounds[i].data   = NULL;
        sounds[i].length = 0;

        char filename[32];
        sprintf(filename, "sound%d.wav", i);
        const char *resolved = resolveAssetPath(filename);

        SDL_AudioSpec wavSpec;
        uint8_t *wavBuf = NULL;
        uint32_t wavLen = 0;

        if (SDL_LoadWAV(resolved, &wavSpec, &wavBuf, &wavLen) != NULL) {
            SDL_AudioCVT cvt;
            if (SDL_BuildAudioCVT(&cvt, wavSpec.format, wavSpec.channels, wavSpec.freq,
                                  deviceSpec.format, deviceSpec.channels, deviceSpec.freq) >= 0) {
                cvt.len = wavLen;
                cvt.buf = (uint8_t*)malloc(wavLen * cvt.len_mult);
                if (cvt.buf != NULL) {
                    memcpy(cvt.buf, wavBuf, wavLen);
                    if (SDL_ConvertAudio(&cvt) == 0) {
                        sounds[i].data   = cvt.buf;
                        sounds[i].length = cvt.len_cvt;
                    } else {
                        free(cvt.buf);
                    }
                }
            }
            SDL_FreeWAV(wavBuf);
        }
    }

    currentRemaining = 0;
    currentPtr = NULL;
    SDL_PauseAudio(0);
}


void soundEnd()
{
    if (soundDisabled)
        return;

    SDL_CloseAudio();

    for (int i=0; i < NUM_OF_SOUNDS; i++) {
        if (sounds[i].data != NULL) {
            free(sounds[i].data);
            sounds[i].data = NULL;
        }
    }
}


void soundPlay(int nb)
{
    if (soundDisabled)
        return;

    if (nb < 0 || NUM_OF_SOUNDS <= nb) {
        debugMsg("soundPlay(): wrong sound sample index #%d", nb);
        return;
    }

    if (sounds[nb].length > 0 && sounds[nb].data != NULL) {
        SDL_LockAudio();
        currentSound     = &sounds[nb];
        currentPtr       = currentSound->data;
        currentRemaining = currentSound->length;
        SDL_UnlockAudio();
    }
    else {
        debugMsg("Non-existent sound sample #%d", nb);
    }
}
