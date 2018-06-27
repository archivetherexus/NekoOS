
#include "gpu.h"
#include "uart.h"
#include "kdraw/framebuffer.h"
#include "kdraw/shapes.h"
#include "kfont/default.h"
#include "kfont/draw.h"

extern struct KDRAW_Framebuffer rpi3_framebuffer;

void init_drivers() {
  rpi3_uart_init();
  rpi3_gpu_init();

  char pixel[4];
  pixel[0] = 255;
  pixel[1] = 255;
  pixel[2] = 0;
  kdraw_draw_rectangle(100, 100, 100, 100, pixel, &rpi3_framebuffer);

  kfont_draw_character('H', 8, 20, &kfont_default_font, &rpi3_framebuffer);

  kfont_draw_ncharacters("Ok", 2, 8, 28, &kfont_default_font, &rpi3_framebuffer);

  kfont_draw_characters("Hello World!", 20, 60, &kfont_default_font, &rpi3_framebuffer);
}