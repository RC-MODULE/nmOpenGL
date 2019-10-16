#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "stdio.h"
#include "nmprofiler.h"

SECTION(".data_imu7")   Triangles localTr;

SECTION(".text_demo3d") void waitPolygons() {
	volatile int a = 0;
	while (halRingBufferIsFull(nmglPolygonsRB)) {
		a++;
	}
}

SECTION(".text_demo3d")
void rasterizeT(Triangles* triangles, SegmentMask* masks, int count){
	for (int segY = 0, iSeg = 0; segY < cntxt.windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt.windowInfo.nColumns; segX++, iSeg++) {
			if (masks[iSeg].hasNotZeroBits) {
				addInstrNMC1(&cntxt.synchro->commandsRB,
					NMC1_COPY_SEG_FROM_IMAGE,
					cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y0[segY],
					cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY]);

				waitPolygons();
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
				
				maskSelectionLight_RGBA_BGRA((v4nm32s*)triangles->colors, (nm1*)masks[iSeg].bits, (v4nm32s*)localTr.colors, count);
				int localSize = copyArraysByMask((void**)triangles, (nm1*)masks[iSeg].bits, (void**)&localTr, 7, count);
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