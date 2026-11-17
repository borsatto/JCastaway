#include "xbrz_bridge.h"
#include "xbrz.h"

extern "C" void jcApplyXbrz(int factor, const uint32_t *src, uint32_t *dst, int width, int height)
{
    if (src == nullptr || dst == nullptr || width <= 0 || height <= 0) {
        return;
    }

    if (factor < 2) factor = 2;
    if (factor > 6) factor = 6;

    xbrz::scale(static_cast<size_t>(factor),
                src,
                dst,
                width,
                height,
                xbrz::ColorFormat::ARGB);
}
