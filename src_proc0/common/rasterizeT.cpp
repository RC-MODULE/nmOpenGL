#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "stdio.h"
#include "nmprofiler.h"

SECTION(".data_imu7")   int indices[NMGL_SIZE];
SECTION(".data_imu7")   int maskBitsTemp[BIG_NMGL_SIZE / 32];

SECTION(".text_demo3d")
void rasterizeT(const Triangles* triangles, const SegmentMask* masks){
	int count = triangles->size;
	Triangles localTrian;
	localTrian.x0 = cntxt.buffer0;
	localTrian.y0 = cntxt.buffer0 + NMGL_SIZE;
	localTrian.x1 = cntxt.buffer1;
	localTrian.y1 = cntxt.buffer1 + NMGL_SIZE;
	localTrian.x2 = cntxt.buffer2;
	localTrian.y2 = cntxt.buffer2 + NMGL_SIZE;

	PolygonsConnector connector(cntxt.polygonsData);

	for (int segY = 0, iSeg = 0; segY < cntxt.windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt.windowInfo.nColumns; segX++, iSeg++) {
			if (masks[iSeg].hasNotZeroBits != 0) {
				cntxt.synchro.writeInstr(1, NMC1_COPY_SEG_FROM_IMAGE,
					cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y0[segY],
					cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY]);

				int* treatedBitInMask = (int*)&cntxt.tmp.vec[0];
				treatedBitInMask[0] = 0;

				int resultSize = readMask(masks[iSeg].bits, indices, count);
				if (resultSize) {
					while (connector.isFull()) {
						halSleep(2);
					}
					Polygons* poly = connector.ptrHead();
					localTrian.z = (int*)poly->z;
					localTrian.colors = (v4nm32s*)poly->color;
					copyArraysByIndices((void**)triangles, indices, (void**)&localTrian, 7, resultSize);
					copyColorByIndices_BGRA_RGBA(triangles->colors, indices, (v4nm32s*)localTrian.colors, resultSize);
					fillPolygonsT(poly, &localTrian, resultSize, segX, segY);

					(*connector.pHead)++;
					cntxt.synchro.writeInstr(1, NMC1_DRAW_TRIANGLES);
				}

				cntxt.synchro.writeInstr( 1,
					NMC1_COPY_SEG_TO_IMAGE,
					cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y0[segY],
					cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
					cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY]);
				
			}
		}
	}

}