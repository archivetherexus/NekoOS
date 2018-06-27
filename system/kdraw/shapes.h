#pragma once
#ifndef KDRAW_SHAPES_H
#define KDRAW_SHAPES_H

#include "framebuffer.h"

void kdraw_draw_rectangle(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, char const color[4], struct KDRAW_Framebuffer *fb);

#endif