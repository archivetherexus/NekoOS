#pragma once
#ifndef KFONT_DRAW_H
#define KFONT_DRAW_H

#include "kdraw/framebuffer.h"
#include "font.h"

void kfont_draw_character(const char character, const int x, const int y, const struct KFONT_BitmapFont *font, const struct KDRAW_Framebuffer *fb);

void kfont_draw_ncharacters(const char *characters, const int n, const int x, const int y, const struct KFONT_BitmapFont *font, const struct KDRAW_Framebuffer *fb);

void kfont_draw_characters(const char *characters, const int x, const int y, const struct KFONT_BitmapFont *font, const struct KDRAW_Framebuffer *fb);

#endif