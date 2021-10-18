#pragma once
#include "imagebuffer.h"
#include "hal.h"
#include "hal_host.h"




class NMGL_FramebufferHost {
	NMGL_Framebuffer frameBuffer;
	int _procNo = 0;
	void update(){
		halReadMemBlock(&frameBuffer, framebufferRemote, 10, _procNo);

		size_t temp;

		halReadMemBlock(&temp, framebufferRemote + 10, frameBuffer.sizeOfAddr32, _procNo);
		//frameBuffer.imageBuffer.data = halMapAddrFrom((void*)temp, _procNo);
		frameBuffer.imageBuffer.data = 0;
		frameBuffer.imageBuffer.data = (void*)temp;

		halReadMemBlock(&temp, framebufferRemote + 10 + frameBuffer.sizeOfBufferStruct32, frameBuffer.sizeOfAddr32, _procNo);
		//frameBuffer.imageBufferFront.data = halMapAddrFrom((void*)temp, _procNo);
		frameBuffer.imageBufferFront.data = 0;
		frameBuffer.imageBufferFront.data = (void*)temp;

		halReadMemBlock(&temp, framebufferRemote + 10 + 2 * frameBuffer.sizeOfBufferStruct32, frameBuffer.sizeOfAddr32, _procNo);
		//frameBuffer.depthBuffer.data = halMapAddrFrom((void*)temp, _procNo);
		frameBuffer.depthBuffer.data = 0;
		frameBuffer.depthBuffer.data = (void*)temp;
	}
public:
	size_t framebufferRemote;
	

	NMGL_FramebufferHost(size_t framebufferAddr, int procNo){
		init(framebufferAddr, procNo);
		update();
	}

	void init(size_t framebufferAddr, int procNo){
		framebufferRemote = framebufferAddr;
		_procNo = procNo;
	}

	void readImage(void* dst){
		do{
			halSleep(2);
			update();
		} while(frameBuffer.tail == frameBuffer.head);
		halReadMemBlock(dst, (size_t)frameBuffer.imageBufferFront.data, frameBuffer.mWidth * frameBuffer.mHeight, _procNo);
		frameBuffer.tail++;
		halWriteMemBlock((void*)&frameBuffer.tail, framebufferRemote + 8, 2, _procNo);
	}	

	int getWidth(){
		update();
		return frameBuffer.mWidth;
	}

	int getHeight(){
		update();
		return frameBuffer.mHeight;
	}

};