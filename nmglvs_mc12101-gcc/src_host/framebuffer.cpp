#include "framebuffer.h"
#include "hal_host.h"
#include "hal.h"
//#include <iostream>

NMGL_FrameBuffer local;

void readFramebufferNM(NMGL_FrameBuffer &framebuffer, NMGL_FrameBuffer *remoteAddr);

bool frameBufferIsEmpty(NMGL_FrameBuffer *remoteAddr) {
	readFramebufferNM(local, remoteAddr);

	return (local.head == local.tail);
}

void frameBufferIncTail(NMGL_FrameBuffer *remoteAddr) {
	readFramebufferNM(local, remoteAddr);

	local.tail++;
	int tailOffset = offsetof(NMGL_FrameBuffer, tail) / 4;
	halWriteMemBlock((void*)&local.tail, (size_t)remoteAddr + tailOffset * local.sizeOfInt, 2);
}

void readColorBackNM(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height){
	readFramebufferNM(local, fb);

	if (local.width == width) {
		halReadMemBlock(data, (size_t)local.buffers[1], width * height);
	}
}

void readColorFrontNM(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height){
	readFramebufferNM(local, fb);

	if (local.width == width) {
		halReadMemBlock(data, (size_t)local.buffers[0], width * height);
	}
}

void readDepthNM(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height){
	readFramebufferNM(local, fb);

	if (local.width == width) {
		halReadMemBlock(data, (size_t)local.buffers[2], width * height);
	}
}

void readFramebufferNM(NMGL_FrameBuffer &framebuffer, NMGL_FrameBuffer *remoteAddr) {

	int fbMainSize32 = offsetof(NMGL_FrameBuffer, buffers) / 4;

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