#include "draw.h"
#include <stdbool.h>

static bool font_get_pixel(const char character, const int x, const int y, const struct KFONT_BitmapFont *font) {
    char *bitmap;
    switch (font->type) {
        case KFONT_BITMAP_FONT_8x8:
            bitmap = ((char**)font->bitmap)[character];
            return bitmap[x] & 1 << y;
            //return ((char **)font->bitmap)[character][x] & 1 << y ? true : false;
            
            //return y % 2 == 0;
            //return 
        default:
            return true; // TODO: Should we report an error herer?
    }
}

void uart_hex(unsigned int d) {
    unsigned int n;
    int c;
    for(c=28;c>=0;c-=4) {
        // get highest tetrad
        n=(d>>c)&0xF;
        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        n+=n>9?0x37:0x30;
        rpi3_uart_putc(n);
    }
}

// TODO: Perhaps this could be moved into kdraw in some intelligent way...
static void rgb_draw_character(const char character, const int x, const int y, const struct KFONT_BitmapFont *font, const struct KDRAW_Framebuffer *fb) {
    //font_get_pixel(character, )
    rpi3_uart_puts("HEH!");
    if (font->bitmap == 0) {
        rpi3_uart_puts("NEIN!");
    }
    rpi3_uart_putc(character);
    rpi3_uart_puts("\n\r");
    uart_hex(*(((char**)font->bitmap)[character]));
    {
        int x,y;
        int set;
        for (x=0; x < 8; x++) {
            for (y=0; y < 8; y++) {
                //set = ((char**)font->bitmap)[character][x] & 1 << y;
                set = 1;
                rpi3_uart_putc(set ? 'X' : '.');
            }
            rpi3_uart_puts("\n\r");
        }
    }
    rpi3_uart_puts("DONE!");


    int height = font->character_height;
    int width = font->character_width;
    unsigned char *ptr = fb->framebuffer;
    char pixelOn[4] = {255, 255, 255, 0};
    char pixelOff[4] = {0, 0, 0, 0};
    ptr += y * fb->pitch + x;
    for (int cy = 0; cy < height; cy++) {
        for (int cx = 0; cx < width; cx++) {
            *((unsigned int *)ptr) = font_get_pixel(character, cx, cy, font) ? *((unsigned int *)&pixelOn) : *((unsigned int *)&pixelOff);
            ptr += 4; // TODO: Maybe use pitch here?
        }
        ptr += fb->pitch - width * 4;
    }

}


void kfont_draw_character(const char character, const int x, const int y, const struct KFONT_BitmapFont *font, const struct KDRAW_Framebuffer *fb) {
    switch(fb->mode) {
    case KDRAW_FRAMEBUFFER_RGB:
        rgb_draw_character(character, x, y, font, fb);
        break;
    default:
        // TODO: Report error;
        break;
    }
}