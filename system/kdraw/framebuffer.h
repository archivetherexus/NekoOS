#pragma once
#ifndef KDRAW_FRAMEBUFFER_H
#define KDRAW_FRAMEBUFFER_H


struct KDRAW_Framebuffer {
    unsigned int width;
    unsigned int height;
    unsigned int pitch;
    unsigned int depth;
    void *framebuffer;
};

#endif