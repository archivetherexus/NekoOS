#pragma once
#ifndef RPI3_GPU_H
#define RPI3_GPU_H

extern unsigned int rpi3_fb_width, rpi3_fb_height, rpi3_fb_pitch;
extern unsigned char *rpi3_fb;

void rpi3_gpu_init();

#endif