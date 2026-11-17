#ifndef JC_XBRZ_BRIDGE_H
#define JC_XBRZ_BRIDGE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void jcApplyXbrz(int factor, const uint32_t *src, uint32_t *dst, int width, int height);

#ifdef __cplusplus
}
#endif

#endif // JC_XBRZ_BRIDGE_H
