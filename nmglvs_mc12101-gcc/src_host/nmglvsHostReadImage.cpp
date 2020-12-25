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


extern ImageConnector hostImageRB;
extern StackTraceConnector stackTraceConnector;
bool gccmap_address2symbol_(char* mapfile, unsigned addr, char* fullname);

int nmglvsHostReadImage(int* dstImage)
{
	S_VS_MouseStatus mouseStatus;
	VS_GetMouseStatus(&mouseStatus);
#if defined(PROFILER0) || defined(PROFILER1)
	if (mouseStatus.nKey == VS_MOUSE_LBUTTON) {
#else
	if (mouseStatus.nKey != VS_MOUSE_LBUTTON) {
#endif
		clock_t t0, t1;
		t0 = clock();
		char fullname[1024];
		while (hostImageRB.isEmpty()) {
			t1 = clock();
			if (t1 - t0 > 5000) {
				int count = STACK_TRACE_POINT_COUNT;
				//int count = 500;
				int head = stackTraceConnector.getHead() - 1;
				TraceData point;
				stackTraceConnector.memcopyPop(stackTraceConnector.ptrItem(head), &point, sizeof32(TraceData));
				int params[5];
				stackTraceConnector.memcopyPop((void*)(point.sp + 0x40000), params, 5);
				int depth = point.depth;
				gccmap_address2symbol_(MAP0, point.func, fullname);
				printf("time=%u, addr=%p, func=%s, depth=%d\n", point.time, point.func, fullname, point.depth);
				for (int i = 0; i < 5; i++) {
					printf("param[%d]=0x%x\n", i, params[i]);
				}
				head--;

				while (count > 0) {
					count--;
					TraceData pointUp;
					stackTraceConnector.memcopyPop(stackTraceConnector.ptrItem(head), &pointUp, sizeof32(TraceData));
					int depthUp = pointUp.depth;
					if (depthUp == depth - 1) {
						gccmap_address2symbol_(MAP0, pointUp.func, fullname);
						printf("time=%u, addr=%p, func = %s, depth=%d\n", pointUp.time, pointUp.func, fullname, pointUp.depth);
						depth = depthUp;
					}
					head--;
				}
				while (true);
			}
		}
		printf("\n");
		hostImageRB.pop((NMGL_IMAGE*)dstImage, 1);
	}
	else {
		while (hostImageRB.isEmpty()) {
			halSleep(2);
		}
		hostImageRB.incTail();
	}
	return 0;
};
