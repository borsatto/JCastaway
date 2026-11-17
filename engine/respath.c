#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
  #include <windows.h>
#elif defined(__APPLE__)
  #include <mach-o/dyld.h>
  #include <unistd.h>
#else
  #include <unistd.h>
#endif

#include "respath.h"

static char customAssetDir[512] = "";
static char resolvedPathBuffer[1024] = "";

static int fileExists(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0 && !S_ISDIR(st.st_mode));
}

static int dirExists(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

void setAssetDir(const char *dir)
{
    if (dir && *dir) {
        strncpy(customAssetDir, dir, sizeof(customAssetDir) - 1);
        customAssetDir[sizeof(customAssetDir) - 1] = '\0';
    }
}

const char *getAssetDir(void)
{
    return customAssetDir;
}

static void getExecutableDir(char *out, size_t maxLen)
{
    out[0] = '\0';
#ifdef _WIN32
    GetModuleFileNameA(NULL, out, (DWORD)maxLen);
    char *lastSlash = strrchr(out, '\\');
    if (!lastSlash) lastSlash = strrchr(out, '/');
    if (lastSlash) *lastSlash = '\0';
#elif defined(__APPLE__)
    uint32_t size = (uint32_t)maxLen;
    if (_NSGetExecutablePath(out, &size) == 0) {
        char *lastSlash = strrchr(out, '/');
        if (lastSlash) *lastSlash = '\0';
    }
#else
    ssize_t len = readlink("/proc/self/exe", out, maxLen - 1);
    if (len > 0) {
        out[len] = '\0';
        char *lastSlash = strrchr(out, '/');
        if (lastSlash) *lastSlash = '\0';
    }
#endif
}

const char *resolveAssetPath(const char *relName)
{
    char candidate[1024];

    // 1. Explicit asset directory
    if (customAssetDir[0]) {
        snprintf(candidate, sizeof(candidate), "%s/%s", customAssetDir, relName);
        if (fileExists(candidate)) {
            strncpy(resolvedPathBuffer, candidate, sizeof(resolvedPathBuffer));
            return resolvedPathBuffer;
        }
    }

    // 2. JC_ASSETS_DIR environment variable
    const char *envDir = getenv("JC_ASSETS_DIR");
    if (envDir && *envDir) {
        snprintf(candidate, sizeof(candidate), "%s/%s", envDir, relName);
        if (fileExists(candidate)) {
            strncpy(resolvedPathBuffer, candidate, sizeof(resolvedPathBuffer));
            return resolvedPathBuffer;
        }
    }

    // 3. Current working directory
    snprintf(candidate, sizeof(candidate), "./%s", relName);
    if (fileExists(candidate)) {
        strncpy(resolvedPathBuffer, candidate, sizeof(resolvedPathBuffer));
        return resolvedPathBuffer;
    }

    // 4. Alongside executable
    char exeDir[512];
    getExecutableDir(exeDir, sizeof(exeDir));
    if (exeDir[0]) {
        snprintf(candidate, sizeof(candidate), "%s/%s", exeDir, relName);
        if (fileExists(candidate)) {
            strncpy(resolvedPathBuffer, candidate, sizeof(resolvedPathBuffer));
            return resolvedPathBuffer;
        }
        // Subfolder 'original' or 'assets' next to executable
        snprintf(candidate, sizeof(candidate), "%s/original/%s", exeDir, relName);
        if (fileExists(candidate)) {
            strncpy(resolvedPathBuffer, candidate, sizeof(resolvedPathBuffer));
            return resolvedPathBuffer;
        }
        snprintf(candidate, sizeof(candidate), "%s/assets/%s", exeDir, relName);
        if (fileExists(candidate)) {
            strncpy(resolvedPathBuffer, candidate, sizeof(resolvedPathBuffer));
            return resolvedPathBuffer;
        }
    }

    // 5. System/User standard directories
#ifdef _WIN32
    const char *appData = getenv("APPDATA");
    if (appData && *appData) {
        snprintf(candidate, sizeof(candidate), "%s\\JohnnyCastaway\\%s", appData, relName);
        if (fileExists(candidate)) {
            strncpy(resolvedPathBuffer, candidate, sizeof(resolvedPathBuffer));
            return resolvedPathBuffer;
        }
    }
#else
    const char *home = getenv("HOME");
    if (home && *home) {
        snprintf(candidate, sizeof(candidate), "%s/.local/share/johnny-castaway/%s", home, relName);
        if (fileExists(candidate)) {
            strncpy(resolvedPathBuffer, candidate, sizeof(resolvedPathBuffer));
            return resolvedPathBuffer;
        }
        snprintf(candidate, sizeof(candidate), "%s/Library/Application Support/Johnny Castaway/%s", home, relName);
        if (fileExists(candidate)) {
            strncpy(resolvedPathBuffer, candidate, sizeof(resolvedPathBuffer));
            return resolvedPathBuffer;
        }
    }
    snprintf(candidate, sizeof(candidate), "/usr/share/johnny-castaway/%s", relName);
    if (fileExists(candidate)) {
        strncpy(resolvedPathBuffer, candidate, sizeof(resolvedPathBuffer));
        return resolvedPathBuffer;
    }
#endif

    // Fallback: return relName or candidate in current dir
    snprintf(resolvedPathBuffer, sizeof(resolvedPathBuffer), "%s", relName);
    return resolvedPathBuffer;
}

int checkAssetsAvailable(void)
{
    const char *mapPath = resolveAssetPath("RESOURCE.MAP");
    return fileExists(mapPath);
}
