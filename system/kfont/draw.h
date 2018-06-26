#pragma once
#ifndef KFONT_DRAW_H
#define KFONT_DRAW_H

#include "kdraw/framebuffer.h"
#include "font.h"

void kfont_draw_character(const char character, const int x, const int y, const struct KFONT_BitmapFont *font, const struct KDRAW_Framebuffer *fb);

#endif