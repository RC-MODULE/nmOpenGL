//------------------------------------------------------------------------
//  Demostration for MAX 2017 
//
// 
//
//  Copyright (c) 2017 RC Module Inc.
//------------------------------------------------------------------------

#include "VShell.h"
#include "demo3d_common.h"
#include "demo3d_host.h"
#include "service.h"
#include "imagebuffer.h"
#include "stacktrace.h"
#include "framebuffer.h"


extern NMGL_Framebuffer *remote;
static int imageTemp[WIDTH_IMAGE * HEIGHT_IMAGE];


bool frameBufferIsEmpty(NMGL_Framebuffer *remoteAddr);
void frameBufferIncTail(NMGL_Framebuffer *remoteAddr);
void readColorBackNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
void readColorFrontNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);
void readDepthNM(void *data, NMGL_Framebuffer *fb, int x, int y, int width, int height);


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
	S_VS_MouseStatus mouseStatus;
	VS_GetMouseStatus(&mouseStatus);
#if defined(PROFILER0) || defined(PROFILER1)
	if (mouseStatus.nKey == VS_MOUSE_LBUTTON) {
#else
	if (mouseStatus.nKey != VS_MOUSE_LBUTTON) {
#endif
		while(frameBufferIsEmpty(remote)){
			halSleep(2);
		}
		readColorFrontNM(imageTemp, remote, 0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);
		frameBufferIncTail(remote);
		for(int y = 0; y < HEIGHT_IMAGE; y++){
			for(int x = 0; x < WIDTH_IMAGE; x++){
				dstImage[y * WIDTH_IMAGE + x] = imageTemp[(HEIGHT_IMAGE - y - 1) * WIDTH_IMAGE + x];
			}
		}
	}
	else {
		while(frameBufferIsEmpty(remote)){
			halSleep(2);
		}
		frameBufferIncTail(remote);
	}
	return 0;
};
