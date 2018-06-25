
#include "gpu.h"
#include "uart.h"

static void show_rectangle() {

  int rectangle_width = 100;
  int rectangle_height = 100;

  int x, y;
  unsigned char *ptr = rpi3_fb;
  char pixel[4];
  pixel[0] = 255;
  pixel[1] = 0;
  pixel[2] = 255;

  ptr += (rpi3_fb_height - rectangle_height) / 2 * rpi3_fb_pitch +
         (rpi3_fb_width - rectangle_width) * 2;
  for (y = 0; y < rectangle_height; y++) {
    for (x = 0; x < rectangle_width; x++) {
      *((unsigned int *)ptr) = *((unsigned int *)&pixel);
      ptr += 4;
    }
    ptr += rpi3_fb_pitch - rectangle_width * 4;
  }
}

void init_drivers() {
  rpi3_uart_init();
  rpi3_gpu_init();

  show_rectangle();
}