#ifndef RESPATH_H
#define RESPATH_H

#ifdef __cplusplus
extern "C" {
#endif

// Sets an explicit assets search directory
void setAssetDir(const char *dir);

// Returns the active assets directory
const char *getAssetDir(void);

// Resolves a relative asset filename (e.g. "RESOURCE.MAP", "sound0.wav")
// to a static buffer containing the full path if found, or returns fallback.
const char *resolveAssetPath(const char *relName);

// Returns 1 if RESOURCE.MAP is found in the current asset paths, 0 otherwise
int checkAssetsAvailable(void);

#ifdef __cplusplus
}
#endif

#endif // RESPATH_H
