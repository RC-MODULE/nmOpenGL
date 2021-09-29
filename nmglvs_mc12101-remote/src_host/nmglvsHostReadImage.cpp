//------------------------------------------------------------------------
//  Demostration for MAX 2017 
//
// 
//
//  Copyright (c) 2017 RC Module Inc.
//------------------------------------------------------------------------

#include "VShell.h"
#include "demo3d_host.h"
#include "service.h"
#include "imagebuffer.h"
#include "stacktrace.h"
#include "hal_host.h"

extern int framebufferAddr;

int nmglvsGetWidth() {
	NMGL_Framebuffer framebuffer;
	halReadMemBlock(&framebuffer, framebufferAddr, sizeof32(NMGL_Framebuffer));
	return framebuffer.getWidth();
}

int nmglvsGetHeight() {
	NMGL_Framebuffer framebuffer;
	halReadMemBlock(&framebuffer, framebufferAddr, sizeof32(NMGL_Framebuffer));
	return framebuffer.getHeight();
}

#ifdef STACK_TRACE_ENABLED
extern StackTraceConnector stackTraceConnector;
bool gccmap_address2symbol_(char* mapfile, unsigned addr, char* fullname);

void PrintTrace(StackTraceConnector *connector, int count, int spDepth0) {
	if (connector->getHead() == 0) {
		printf("StackData error\n");
		return;
	}
	int head = connector->getHead() - 1;
	int *params = new int[spDepth0];
	char fullname[1024];
	for (int i = 0; i < count; i++, head--) {
		TraceData point;
		stackTraceConnector.memcopyPop(stackTraceConnector.ptrItem(head), &point, sizeof32(TraceData));
		gccmap_address2symbol_(MAP0, point.func, fullname);
		printf("time=%u, addr=%p, func=%s, depth=%d\n", point.time, point.func, fullname, point.depth);
		//if (i == 0) {
			stackTraceConnector.memcopyPop((void*)(point.sp - spDepth0 + 2 + 0x40000), params, spDepth0);
			for (int i = spDepth0 - 1; i >=0; i--) {
				printf("stack[%d]=0x%x\n", i, params[i]);
			}
		//}
	}
	delete params;
}
#endif //STACK_TRACE_ENABLED

int nmglvsHostReadImage(int* dstImage)
{
	NMGL_Framebuffer framebuffer;
	do  {
		halSleep(50);
		halReadMemBlock(&framebuffer, framebufferAddr, sizeof32(NMGL_Framebuffer));		
	} while (framebuffer.head == framebuffer.tail);
	S_VS_MouseStatus mouseStatus;
	VS_GetMouseStatus(&mouseStatus);
	if (mouseStatus.nKey != VS_MOUSE_LBUTTON) {
		halReadMemBlock(dstImage, (size_t)framebuffer.imageBufferFront.data, framebuffer.getSize());
	}
	framebuffer.tail++;
	halWriteMemBlock((void*)&framebuffer.tail, (size_t)framebufferAddr + ((int*)&framebuffer.tail - (int*)&framebuffer), 2);
	return 0;
};
