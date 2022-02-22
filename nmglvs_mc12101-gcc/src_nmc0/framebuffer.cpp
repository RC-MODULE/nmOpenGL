#include "framebuffer.h"

void NMGL_FramebufferInit(NMGL_Framebuffer *fb, int width, int height){
    fb->head = 0;
    fb->tail = 0;
    fb->sizeOfAddr = sizeof(void *);
    fb->sizeOfInt = sizeof(int);
    fb->width = width;
    fb->height = height;
}
