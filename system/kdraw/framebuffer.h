#pragma once
#ifndef KDRAW_FRAMEBUFFER_H
#define KDRAW_FRAMEBUFFER_H

enum KDRAW_FramebufferMode {
    KDRAW_FRAMEBUFFER_RGB,
};

struct KDRAW_Framebuffer {
    unsigned int width;
    unsigned int height;
    int pitch;
    enum KDRAW_FramebufferMode mode;
    void *framebuffer;
};

#endif