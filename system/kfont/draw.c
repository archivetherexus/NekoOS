#include "draw.h"
#include <stdbool.h>

static bool get_pixel_8x8(char buff[128][8], const char character, const int x, const int y) {
    return buff[(unsigned char)character][x] & 1 << y;
}

static bool font_get_pixel(const char character, const int x, const int y, const struct KFONT_BitmapFont *font) {
    switch (font->type) {
        case KFONT_BITMAP_FONT_8x8:
            return get_pixel_8x8(font->bitmap, character, y, x);
        default:
            return true; // TODO: Should we report an error herer?
    }
}

// TODO: Perhaps the mapping between data and pixel could be moved into kdraw in some intelligent way...
void kfont_draw_character(const char character, const int x, const int y, const struct KFONT_BitmapFont *font, const struct KDRAW_Framebuffer *fb) {
    int depth_in_bytes = fb->depth / 8;
    int height = font->character_height;
    int width = font->character_width;
    unsigned char *ptr = fb->framebuffer;
    char pixelOn[4] = {255, 255, 255, 0};
    char pixelOff[4] = {0, 0, 0, 0};
    ptr += y * fb->pitch + x * depth_in_bytes;
    for (int cy = 0; cy < height; cy++) {
        for (int cx = 0; cx < width; cx++) {
            *((unsigned int *)ptr) = font_get_pixel(character, cx, cy, font) ? *((unsigned int *)&pixelOn) : *((unsigned int *)&pixelOff);
            ptr += depth_in_bytes;
        }
        ptr += fb->pitch - width * depth_in_bytes;
    }
}

void kfont_draw_ncharacters(const char *characters, const int n, const int x, const int y, const struct KFONT_BitmapFont *font, const struct KDRAW_Framebuffer *fb) {
    for(int i = 0; i < n; i++) {
        kfont_draw_character(characters[i], x + font->character_width * i, y, font, fb);
    }
}

void kfont_draw_characters(const char *characters, const int x, const int y, const struct KFONT_BitmapFont *font, const struct KDRAW_Framebuffer *fb) {
    int nx = x;
    while(characters[0] != '\0') {
        kfont_draw_character(characters[0], nx, y, font, fb);
        characters++;
        nx += font->character_width;
    }
}