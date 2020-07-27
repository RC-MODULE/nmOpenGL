#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"
#include "service.h"
#include "imagebuffer.h"

SECTION(".data_demo3d") unsigned int points[14];
SECTION(".data_demo3d") MyDmaTask task;

#define CHECK_STATUS(a) while (!msdGetStatusCopy(points[a], 0))

inline void ADD_COPY(const void* src, void* dst, int size, int i) {
	task.src = src;
	task.dst = dst;
	task.size = size;
	points[i] = msdAdd(task, 0);
}

SECTION(".text_demo3d") int readPolygonsT(DataForNmpu1* data){
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	DataForNmpu1* dataTmp = (DataForNmpu1*)cntxt->buffer0;
	nm32s* temp0 = nmppsAddr_32s(cntxt->buffer0, sizeof32(DataForNmpu1));
	nm32s* dydx = nmppsAddr_32s(temp0, POLYGONS_SIZE);
	nm32s* temp1 = cntxt->buffer1;
	nm32s* temp2 = nmppsAddr_32s(cntxt->buffer1, POLYGONS_SIZE);
	msdAdd(data, dataTmp, sizeof32(DataForNmpu1), 0);
	msdWaitDma(0);

	return 0;
}