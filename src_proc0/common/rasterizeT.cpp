#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "stdio.h"
#include "nmprofiler.h"

SECTION(".data_imu7")   Triangles localTr;
SECTION(".data_imu7")   Triangles trianglesClone;


SECTION(".text_demo3d") void waitPolygons() {
	volatile int a = 0;
	while (halRingBufferIsFull(nmglPolygonsRB)) {
		a++;
	}
}

SECTION(".text_demo3d")
void rasterizeT(const Triangles* triangles, const SegmentMask* masks, int count){
	for (int segY = 0, iSeg = 0; segY < cntxt.windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt.windowInfo.nColumns; segX++, iSeg++) {
			if (masks[iSeg].hasNotZeroBits) {
				addInstrNMC1(&cntxt.synchro->commandsRB,
					NMC1_COPY_SEG_FROM_IMAGE,
					cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y0[segY],
					cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY]);

				trianglesClone.colors = triangles->colors;
				trianglesClone.z = triangles->z;
				trianglesClone.x0 = triangles->x0;
				trianglesClone.y0 = triangles->y0;
				trianglesClone.x1 = triangles->x1;
				trianglesClone.y1 = triangles->y1;
				trianglesClone.x2 = triangles->x2;
				trianglesClone.y2 = triangles->y2;
				int* maskBits = masks[iSeg].bits;
				int countClone = count;

				while (countClone > 0) {
					int localCount = MIN(countClone, NMGL_SIZE);
					waitPolygons();
					Polygons* poly = (Polygons*)halRingBufferHead(nmglPolygonsRB);
					localTr.x2 = cntxt.buffer0;
					localTr.y2 = cntxt.buffer0 + NMGL_SIZE;
					localTr.x1 = cntxt.buffer1;
					localTr.y1 = cntxt.buffer1 + NMGL_SIZE;
					localTr.x0 = cntxt.buffer2;
					localTr.y0 = cntxt.buffer2 + NMGL_SIZE;
					localTr.z = poly->z;
					localTr.colors = (v4nm32s*)poly->color;
					int* numbers = (int*)cntxt.buffer3;

					int resultSize = readMask(maskBits, numbers, localCount);
					if(resultSize != 0){
						copyArraysByIndices((void**)&trianglesClone, numbers, (void**)&localTr, 7, resultSize);
						maskSelectionLight_RGBA_BGRA(trianglesClone.colors, (nm1*)maskBits, (v4nm32s*)localTr.colors, localCount);
						
						fillPolygonsT(poly, &localTr, resultSize, segX, segY);
						nmglPolygonsRB->head++;
						addInstrNMC1(&cntxt.synchro->commandsRB, NMC1_DRAW_TRIANGLES);
					}

					countClone -= NMGL_SIZE;
					trianglesClone.colors += NMGL_SIZE;
					trianglesClone.z += NMGL_SIZE;
					trianglesClone.x0 += NMGL_SIZE;
					trianglesClone.y0 += NMGL_SIZE;
					trianglesClone.x1 += NMGL_SIZE;
					trianglesClone.y1 += NMGL_SIZE;
					trianglesClone.x2 += NMGL_SIZE;
					trianglesClone.y2 += NMGL_SIZE;
					maskBits += NMGL_SIZE / 32;
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