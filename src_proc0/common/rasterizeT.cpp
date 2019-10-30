#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "stdio.h"
#include "nmprofiler.h"

SECTION(".data_imu7")   Triangles localTr;
SECTION(".data_imu7")   int numbers[NMGL_SIZE];
SECTION(".data_imu7")   int maskBitsTemp[BIG_NMGL_SIZE / 32];


SECTION(".text_demo3d") Polygons* getPolygonsHead() {
	volatile int a = 0;
	//printf("Polygons: head-tail=%d\n", nmglPolygonsRB->head - nmglPolygonsRB->tail);
	while (halRingBufferIsFull(nmglPolygonsRB)) {
		a++;
	}
	return (Polygons*)halRingBufferHead(nmglPolygonsRB);
}

SECTION(".text_demo3d")
void rasterizeT(const Triangles* triangles, const SegmentMask* masks, int count){

	localTr.x2 = cntxt.buffer0;
	localTr.y2 = cntxt.buffer0 + NMGL_SIZE;
	localTr.x1 = cntxt.buffer1;
	localTr.y1 = cntxt.buffer1 + NMGL_SIZE;
	localTr.x0 = cntxt.buffer2;
	localTr.y0 = cntxt.buffer2 + NMGL_SIZE;

	for (int segY = 0, iSeg = 0; segY < cntxt.windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt.windowInfo.nColumns; segX++, iSeg++) {
			if (masks[iSeg].hasNotZeroBits != 0) {
				addInstrNMC1(&cntxt.synchro->commandsRB,
					NMC1_COPY_SEG_FROM_IMAGE,
					cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y0[segY],
					cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY]);

				int sizeMask32 = MIN(BIG_NMGL_SIZE / 32, count / 32 + 2);
				nmblas_scopy(sizeMask32, (float*)masks[iSeg].bits, 1, (float*)maskBitsTemp, 1);

				int* treatedBitInMask = (int*)&cntxt.tmp.vec[0];
				treatedBitInMask[0] = 0;
				while (treatedBitInMask[0] < count) {
					Polygons* poly = getPolygonsHead();

					localTr.z = poly->z;
					localTr.colors = (v4nm32s*)poly->color;

					
					int resultSize = readMask(maskBitsTemp, numbers, treatedBitInMask, count, NMGL_SIZE);
					copyArraysByIndices((void**)triangles, numbers, (void**)&localTr, 7, resultSize);
					copyColorByIndices_BGRA_RGBA(triangles->colors, numbers, (v4nm32s*)localTr.colors, resultSize);
					fillPolygonsT(poly, &localTr, resultSize, segX, segY);

					nmglPolygonsRB->head++;
					addInstrNMC1(&cntxt.synchro->commandsRB, NMC1_DRAW_TRIANGLES);
				}

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