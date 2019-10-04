#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "stdio.h"
#include "nmprofiler.h"

SECTION(".data_imu4")	float minXY[2 * NMGL_SIZE];
SECTION(".data_imu5")	float maxXY[2 * NMGL_SIZE];
SECTION(".data_shmem0")	int maskRes[NMGL_SIZE / 32];
SECTION(".data_shmem0")	MasksSeg masks;
SECTION(".data_imu7")   Triangles localTr;



SECTION(".text_nmgl")
inline int andMask(MasksSeg* srcMask, int* dstMask, int size) {
	int result = 0;
	int i = 0;
	/*i = size << 27;
	int size32 = size >> 5;
	if (i) {
		size32++;
	}
	size32 += size32 & 1;
	addInstrNMC1(&nmglSynchro->instantCommandsRB, NMC1_AND4,
		(int)srcMask->xLt,
		(int)srcMask->yLt,
		(int)srcMask->xGt,
		(int)srcMask->yGt,
		(int)dstMask,
		size32);
	while (!halRingBufferIsEmpty(&nmglSynchro->instantCommandsRB));
	i = 0;
	while (size > 0) {
		if (dstMask[i] != 0) {
			return 1;
		}
		i++;
		size -= 32;
	}
	return 0;*/

	while (size>0) {
		dstMask[i] = (srcMask->xLt[i] & srcMask->yLt[i]) & (srcMask->xGt[i] & srcMask->yGt[i]);
		result |= dstMask[i];
		i++;
		size -= 32;
	}
	return result;
}

SECTION(".text_demo3d")
void rasterizeT(Triangles* triangles, int count){

	float* minX = cntxt.buffer0;
	float* maxX = cntxt.buffer1;
	float* minY = cntxt.buffer2;
	float* maxY = cntxt.buffer3;
	findMinMax3(triangles->x0, triangles->x1, triangles->x2, minX, maxX, count);
	findMinMax3(triangles->y0, triangles->y1, triangles->y2, minY, maxY, count);
	nmppsMerge_32f(minX, minY, minXY, count);
	nmppsMerge_32f(maxX, maxY, maxXY, count);

	for (int segY = 0, iSeg = 0; segY < cntxt.windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt.windowInfo.nColumns; segX++, iSeg++) {
			int polygonsCheck = 0;
			v2nm32f* lowerLimit = (v2nm32f*)&cntxt.tmp;
			v2nm32f* upperLimit = (v2nm32f*)&cntxt.tmp.vec[2];
			lowerLimit->v0 = cntxt.windowInfo.x0_f[segX];
			lowerLimit->v1 = cntxt.windowInfo.y0_f[segY];
			upperLimit->v0 = cntxt.windowInfo.x1_f[segX];
			upperLimit->v1 = cntxt.windowInfo.y1_f[segY];

			nmppsCmpLtC_v2nm32f((v2nm32f*)minXY, (v2nm32f*)upperLimit, masks.xLt, masks.yLt, 1, count);
			nmppsCmpGtC_v2nm32f((v2nm32f*)maxXY, (v2nm32f*)lowerLimit, masks.xGt, masks.yGt, 1, count);
			polygonsCheck = andMask(&masks, maskRes, count);
			if (polygonsCheck) {
				addInstrNMC1(&cntxt.synchro->commandsRB,
					NMC1_COPY_SEG_FROM_IMAGE,
					cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y0[segY],
					cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY]);

				while (halRingBufferIsFull(nmglPolygonsRB));
				Polygons* poly = (Polygons*)halRingBufferHead(nmglPolygonsRB);
				clock_t t0, t1;
				localTr.x2 = cntxt.buffer0;
				localTr.y2 = cntxt.buffer0 + NMGL_SIZE;
				localTr.x1 = cntxt.buffer1;
				localTr.y1 = cntxt.buffer1 + NMGL_SIZE;
				localTr.x0 = cntxt.buffer2;
				localTr.y0 = cntxt.buffer2 + NMGL_SIZE;
				localTr.z = poly->z;
				localTr.colors = (v4nm32s*)poly->color;
				
				maskSelectionLight_RGBA_BGRA((v4nm32s*)triangles->colors, (nm1*)maskRes, (v4nm32s*)localTr.colors, count);
				int localSize = copyArraysByMask((void**)triangles, (nm1*)maskRes, (void**)&localTr, 7, count);
				fillPolygonsT(poly, &localTr, localSize, segX, segY);

				nmglPolygonsRB->head++;
				addInstrNMC1(&cntxt.synchro->commandsRB, NMC1_DRAW_TRIANGLES);

				addInstrNMC1(&cntxt.synchro->commandsRB,
					NMC1_COPY_SEG_TO_IMAGE,
					cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y0[segY],
					cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY]);
				
			}
		}
	}
}