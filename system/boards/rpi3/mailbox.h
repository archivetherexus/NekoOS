#pragma once
#ifndef RPI3_MAILBOX_H
#define RPI3_MAILBOX_H

#define RPI3_MAILBOX_REQUEST 0

// Tags. //
#define RPI3_MAILBOX_TAG_LAST 0


enum RPI3_MailboxChannel {
    RPI3_MailBoxChannelPower       = 0,
    RPI3_MailBoxChannelFrameBuffer = 1,
    RPI3_MailBoxChannelVUART       = 2,
    RPI3_MailBoxChannelVCHIQ       = 3,
    RPI3_MailBoxChannelLeds        = 4,
    RPI3_MailBoxChannelButtons     = 5,
    RPI3_MailBoxChannelTouch       = 6,
    RPI3_MailBoxChannelCount       = 7,
    RPI3_MailBoxChannelProp        = 8,
};

extern volatile unsigned int rpi3_mailbox[36];

int rpi3_mailbox_call(unsigned char ch);

#endif