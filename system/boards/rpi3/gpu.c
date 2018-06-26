#include "mailbox.h"
#include "kdraw/framebuffer.h"

struct KDRAW_Framebuffer rpi3_framebuffer;

void rpi3_gpu_init() {
  rpi3_mailbox[0] = 35 * 4;
  rpi3_mailbox[1] = RPI3_MAILBOX_REQUEST;

  rpi3_mailbox[2] = 0x48003; // set phy wh
  rpi3_mailbox[3] = 8;
  rpi3_mailbox[4] = 8;
  rpi3_mailbox[5] = 1024; // FrameBufferInfo.width
  rpi3_mailbox[6] = 768; // FrameBufferInfo.height

  rpi3_mailbox[7] = 0x48004; // set virt wh
  rpi3_mailbox[8] = 8;
  rpi3_mailbox[9] = 8;
  rpi3_mailbox[10] = 1024; // FrameBufferInfo.virtual_width
  rpi3_mailbox[11] = 768; // FrameBufferInfo.virtual_height

  rpi3_mailbox[12] = 0x48009; // set virt offset
  rpi3_mailbox[13] = 8;
  rpi3_mailbox[14] = 8;
  rpi3_mailbox[15] = 0; // FrameBufferInfo.x_offset
  rpi3_mailbox[16] = 0; // FrameBufferInfo.y.offset

  rpi3_mailbox[17] = 0x48005; // set depth
  rpi3_mailbox[18] = 4;
  rpi3_mailbox[19] = 4;
  rpi3_mailbox[20] = 32; // FrameBufferInfo.depth

  rpi3_mailbox[21] = 0x48006; // set pixel order
  rpi3_mailbox[22] = 4;
  rpi3_mailbox[23] = 4;
  rpi3_mailbox[24] = 1; // RGB, not BGR preferably

  rpi3_mailbox[25] = 0x40001; // get framebuffer, gets alignment on request
  rpi3_mailbox[26] = 8;
  rpi3_mailbox[27] = 8;
  rpi3_mailbox[28] = 4096; // FrameBufferInfo.pointer
  rpi3_mailbox[29] = 0; // FrameBufferInfo.size

  rpi3_mailbox[30] = 0x40008; // get pitch
  rpi3_mailbox[31] = 4;
  rpi3_mailbox[32] = 4;
  rpi3_mailbox[33] = 0; // FrameBufferInfo.pitch

  rpi3_mailbox[34] = RPI3_MAILBOX_TAG_LAST;

  if (rpi3_mailbox_call(RPI3_MailBoxChannelProp) && rpi3_mailbox[20] == 32 &&
      rpi3_mailbox[28] != 0) {
    rpi3_mailbox[28] &= 0x3FFFFFFF;
    rpi3_framebuffer.width = rpi3_mailbox[5];
    rpi3_framebuffer.height = rpi3_mailbox[6];
    rpi3_framebuffer.pitch = rpi3_mailbox[33];
    rpi3_framebuffer.framebuffer = (void *)((unsigned long)rpi3_mailbox[28]);
    rpi3_framebuffer.mode = KDRAW_FRAMEBUFFER_RGB;
  } else {
    // uart_puts("Unable to set screen resolution to 1024x768x32\n");
  }
}