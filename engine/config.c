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

#include "mytypes.h"
#include "utils.h"
#include "config.h"

#define BUFFER_LEN 100


static char *cfgFullPath()
{
    static char result[512] = "";

    if (result[0] == '\0') {
#ifdef _WIN32
        const char *appData = getenv("APPDATA");
        if (appData && *appData) {
            snprintf(result, sizeof(result), "%s\\JohnnyCastaway\\%s", appData, CFG_FILENAME);
            return result;
        }
#endif
        const char *home = getenv("HOME");
        if (home && *home) {
            snprintf(result, sizeof(result), "%s/%s", home, CFG_FILENAME);
            return result;
        }
        snprintf(result, sizeof(result), "%s", CFG_FILENAME);
    }

    return result;
}


void cfgFileWrite(struct TConfig *cfg)
{
    FILE *f = fopen(cfgFullPath(), "w");

    if (f == NULL) {
        debugMsg("Warning: couldn't open %s for writing", CFG_FILENAME);
    }
    else {
        fprintf(f, "currentDay=%d\n", cfg->currentDay);
        fprintf(f, "date=%d\n", cfg->date);
        fclose(f);
    }
}


void cfgFileRead(struct TConfig *cfg)
{
    char buf[BUFFER_LEN];

    cfg->currentDay = 0;
    cfg->date       = 0;

    FILE *f = fopen(cfgFullPath(), "r");

    if (f != NULL) {

        while (!feof(f)) {

            fgets(buf, BUFFER_LEN, f);

            if (!feof(f)) {

                if(strstr(buf, "currentDay=") == buf)
                    cfg->currentDay = atoi(buf + 11);

                if(strstr(buf, "date=") == buf)
                    cfg->date = atoi(buf + 5);
            }
        }

        fclose(f);
    }
}

