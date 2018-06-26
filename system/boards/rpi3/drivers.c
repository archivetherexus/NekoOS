
#include "gpu.h"
#include "uart.h"
#include "kdraw/framebuffer.h"
#include "kfont/default.h"
#include "kfont/draw.h"

extern struct KDRAW_Framebuffer rpi3_framebuffer;

static void show_rectangle(struct KDRAW_Framebuffer *fb) {

  int rectangle_width = 100;
  int rectangle_height = 100;

  int x, y;
  unsigned char *ptr = fb->framebuffer;
  char pixel[4];
  pixel[0] = 255;
  pixel[1] = 0;
  pixel[2] = 255;

  ptr += (fb->height - rectangle_height) / 2 * fb->pitch +
         (fb->width - rectangle_width) * 2;
  for (y = 0; y < rectangle_height; y++) {
    for (x = 0; x < rectangle_width; x++) {
      *((unsigned int *)ptr) = *((unsigned int *)&pixel);
      ptr += 4;
    }
    ptr += fb->pitch - rectangle_width * 4;
  }
}



void init_drivers() {
  rpi3_uart_init();
  rpi3_gpu_init();

  show_rectangle(&rpi3_framebuffer);

  char font8x8copy[128][8];
  struct KFONT_BitmapFont default_font = kfont_get_default_font(font8x8copy);


  kfont_draw_character('H', 20, 20, /*&kfont_default_font*/ &default_font, &rpi3_framebuffer);
}