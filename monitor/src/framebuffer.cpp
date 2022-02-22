#include "framebuffer.h"
#include "refresh.h"
#include <iostream>
#include <qDebug>

using namespace std;


bool Refresh::frameBufferIsEmpty(NMGL_Framebuffer *remoteAddr) {
    qDebug() << __FUNCTION__;
    readFramebufferNM(local, remoteAddr);

	return (local.head == local.tail);
}

void Refresh::frameBufferIncTail(NMGL_Framebuffer *remoteAddr) {
    qDebug() << __FUNCTION__;
	readFramebufferNM(local, remoteAddr);

	local.tail++;
	int tailOffset = offsetof(NMGL_Framebuffer, tail) / 4;
    board->writeMemBlock((PL_Word *)&local.tail, (PL_Addr)remoteAddr + tailOffset * local.sizeOfInt, 2, 0);
}

void Refresh::readColorBackNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height){
    qDebug() << __FUNCTION__;
	readFramebufferNM(local, fb);

	if (local.width == width) {
        board->readMemBlock((PL_Addr)local.buffers[1], (PL_Word *)data, width * height, 0);
	}
}

void Refresh::readColorFrontNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height){
    qDebug() << __FUNCTION__;
	readFramebufferNM(local, fb);

    if (local.width == width) {
        board->readMemBlock((PL_Addr)local.buffers[0], (PL_Word *)data, width * height, 0);
	}
}

void Refresh::readDepthNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height){
    qDebug() << __FUNCTION__;
	readFramebufferNM(local, fb);

    if (local.width == width) {
        board->readMemBlock((PL_Addr)local.buffers[2], (PL_Word *)data, width * height, 0);
	}
}

void Refresh::readFramebufferNM(NMGL_Framebuffer &framebuffer, NMGL_Framebuffer *remoteAddr) {
    qDebug() << __FUNCTION__;
    //cout << "access: " << _access << endl;
    //cout << "local: " << &framebuffer << endl;
    //cout << "remote: " << remoteAddr << endl;
    int fbMainSize32 = offsetof(NMGL_Framebuffer, buffers) / 4;
    //cout << "fbMainSize32: " << fbMainSize32 << endl;

    board->readMemBlock((PL_Addr)remoteAddr, (PL_Word *)&framebuffer, fbMainSize32, 0);
	int sizeofRemoteAddr = framebuffer.sizeOfAddr;
	int sizeofInt = framebuffer.sizeOfInt;
	for (int i = 0; i < MAX_BUFFERS_COUNT; i++)
	{
        size_t remoteBuffAddr = (size_t)(remoteAddr) + fbMainSize32 * sizeofInt + i * sizeofRemoteAddr;
        board->readMemBlock((PL_Addr)remoteBuffAddr, (PL_Word *)&framebuffer.buffers[i], sizeofRemoteAddr / sizeofInt, 0);
		//framebuffer.buffers[i] = halMapAddrFrom(framebuffer.buffers[i], 0);
    }
}
