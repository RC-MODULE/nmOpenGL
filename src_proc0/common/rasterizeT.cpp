#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "stdio.h"
#include "nmprofiler.h"

SECTION(".data_imu7")   int numbers[NMGL_SIZE];
SECTION(".data_imu7")   int maskBitsTemp[BIG_NMGL_SIZE / 32];


SECTION(".text_demo3d") Polygons* getPolygonsHead() {
	volatile int a = 0;
	while (halRingBufferIsFull(nmglPolygonsRB)) {
		a++;
	}
	return (Polygons*)halRingBufferHead(nmglPolygonsRB);
}

SECTION(".text_demo3d")
void rasterizeT(const Triangles* triangles, const SegmentMask* masks, int count){

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
					
					int resultSize = readMask(maskBitsTemp, numbers, treatedBitInMask, count, NMGL_SIZE);
					copyArraysByIndices((void**)triangles, numbers, (void**)&cntxt.trianInner, 7, resultSize);
					copyColorByIndices_BGRA_RGBA(triangles->colors, numbers, (v4nm32s*)cntxt.trianInner.colors, resultSize);
					fillPolygonsT(poly, &cntxt.trianInner, resultSize, segX, segY);
					nmblas_scopy(resultSize, (float*)cntxt.trianInner.z, 1, (float*)poly->z, 1);
					nmblas_scopy(4 * resultSize, (float*)cntxt.trianInner.colors, 1, (float*)poly->color, 1);

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