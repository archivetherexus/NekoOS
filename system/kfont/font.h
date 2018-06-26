#pragma once
#ifndef KFONT_FONT_H
#define KFONT_FONT_H

#include <stdint.h>

enum KFONT_BitmapFontType {
    KFONT_BITMAP_FONT_8x8,
};

struct KFONT_BitmapFont {
    void *bitmap;
    enum KFONT_BitmapFontType type;
    uint8_t character_width;
    uint8_t character_height;
};

#endif