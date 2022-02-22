#include "framebuffer.h"
#include "hal_host.h"
#include "hal.h"
//#include <iostream>

NMGL_Framebuffer local;

void readFramebufferNM(NMGL_Framebuffer &framebuffer, NMGL_Framebuffer *remoteAddr);

bool frameBufferIsEmpty(NMGL_Framebuffer *remoteAddr) {
	readFramebufferNM(local, remoteAddr);

	return (local.head == local.tail);
}

void frameBufferIncTail(NMGL_Framebuffer *remoteAddr) {
	readFramebufferNM(local, remoteAddr);

	local.tail++;
	int tailOffset = offsetof(NMGL_Framebuffer, tail) / 4;
	halWriteMemBlock((void*)&local.tail, (size_t)remoteAddr + tailOffset * local.sizeOfInt, 2);
}

void readColorBackNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height){
	readFramebufferNM(local, fb);

	if (local.width == width) {
		halReadMemBlock(data, (size_t)local.buffers[1], width * height);
	}
}

void readColorFrontNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height){
	readFramebufferNM(local, fb);

	if (local.width == width) {
		halReadMemBlock(data, (size_t)local.buffers[0], width * height);
	}
}

void readDepthNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height){
	readFramebufferNM(local, fb);

	if (local.width == width) {
		halReadMemBlock(data, (size_t)local.buffers[2], width * height);
	}
}

void readFramebufferNM(NMGL_Framebuffer &framebuffer, NMGL_Framebuffer *remoteAddr) {

	int fbMainSize32 = offsetof(NMGL_Framebuffer, buffers) / 4;

	halReadMemBlock(&framebuffer, (size_t)remoteAddr, fbMainSize32);
	int sizeofRemoteAddr = framebuffer.sizeOfAddr;
	int sizeofInt = framebuffer.sizeOfInt;
	for (int i = 0; i < MAX_BUFFERS_COUNT; i++)
	{
		size_t remoteBuffAddr = (size_t)(remoteAddr) + fbMainSize32 * sizeofInt + i * sizeofRemoteAddr;
		halReadMemBlock(&framebuffer.buffers[i], (size_t)remoteBuffAddr, sizeofRemoteAddr / sizeofInt);
		//framebuffer.buffers[i] = halMapAddrFrom(framebuffer.buffers[i], 0);
    }
}