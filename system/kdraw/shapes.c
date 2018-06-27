#include "framebuffer.h"

void kdraw_draw_rectangle(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, char const color[4], struct KDRAW_Framebuffer *fb) {
    int depth_in_bytes = fb->depth / 8;
    unsigned char *ptr = fb->framebuffer;
    ptr += y * fb->pitch + x * depth_in_bytes;
    for (int cy = 0; cy < height; cy++) {
        for (int cx = 0; cx < width; cx++) {
            *((unsigned int *)ptr) = *((unsigned int *)&color);
            ptr += depth_in_bytes;
        }
        ptr += fb->pitch - width * depth_in_bytes;
    }
}