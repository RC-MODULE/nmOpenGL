#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "stdio.h"
#include "nmprofiler.h"

SECTION(".data_imu7")   int indices[NMGL_SIZE];
SECTION(".data_imu7")   Triangles localTrian2;
SECTION(".data_imu7")   Triangles localTrian;

SECTION(".text_demo3d") int waitPolygons(PolygonsConnector &connector) {
	int a = 0;
	while (connector.isFull()) {
		a++;
	}
	return a;
}

SECTION(".text_demo3d")
void rasterizeT(const Triangles* triangles, const BitMask* masks){
	int count = triangles->size;
	localTrian.x0 = cntxt.buffer0;
	localTrian.y0 = cntxt.buffer0 + NMGL_SIZE;
	localTrian.x1 = cntxt.buffer1;
	localTrian.y1 = cntxt.buffer1 + NMGL_SIZE;
	localTrian.x2 = cntxt.buffer2;
	localTrian.y2 = cntxt.buffer2 + NMGL_SIZE;
	localTrian.colors = (v4nm32s*)cntxt.buffer3;
	localTrian.z = (int*)cntxt.buffer3 + 4 * NMGL_SIZE;

#ifdef TEXTURE_ENABLED
	if (cntxt.texState.textureEnabled){
		localTrian.s0 = cntxt.buffer0 + 10 * NMGL_SIZE;
		localTrian.t0 = cntxt.buffer0 + 11 * NMGL_SIZE;
		localTrian.s1 = cntxt.buffer1 + 10 * NMGL_SIZE;
		localTrian.t1 = cntxt.buffer1 + 11 * NMGL_SIZE;
		localTrian.s2 = cntxt.buffer2 + 10 * NMGL_SIZE;
		localTrian.t2 = cntxt.buffer2 + 11 * NMGL_SIZE;
		localTrian.zEye0 = cntxt.buffer3 + 11 * NMGL_SIZE;
		localTrian.zEye1 = cntxt.buffer3 + 12 * NMGL_SIZE;
		localTrian.zEye2 = cntxt.buffer3 + 13 * NMGL_SIZE;
	}
#endif //TEXTURE_ENABLED

	PolygonsConnector connector(cntxt.polygonsData);
	for (int segY = 0, iSeg = 0; segY < cntxt.windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt.windowInfo.nColumns; segX++, iSeg++) {
			if (masks[iSeg].hasNotZeroBits != 0) {

				int resultSize = readMask(masks[iSeg].bits, indices, count);
				if (resultSize) {
					cntxt.synchro.writeInstr(1, NMC1_COPY_SEG_FROM_IMAGE,
						cntxt.windowInfo.x0[segX],
						cntxt.windowInfo.y0[segY],
						cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
						cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY],
						iSeg);
#ifdef TEXTURE_ENABLED
					if (cntxt.texState.textureEnabled){
						copyArraysByIndices((void**)triangles, indices, (void**)&localTrian, 16, resultSize);
					} else {
						copyArraysByIndices((void**)triangles, indices, (void**)&localTrian, 7, resultSize);
					}
#else //TEXTURE_ENABLED
					copyArraysByIndices((void**)triangles, indices, (void**)&localTrian, 7, resultSize);
#endif //TEXTURE_ENABLED
					copyColorByIndices_BGRA_RGBA(triangles->colors, indices, (v4nm32s*)localTrian.colors, resultSize);

					//waitPolygons(connector);
					while (connector.isFull());
					Polygons* poly = connector.ptrHead();
					poly->count = 0;
					updatePolygonsT(poly, &localTrian, resultSize, segX, segY);
					connector.incHead();
					cntxt.synchro.writeInstr(1, NMC1_DRAW_TRIANGLES);

					cntxt.synchro.writeInstr(1,
						NMC1_COPY_SEG_TO_IMAGE,
						cntxt.windowInfo.x0[segX],
						cntxt.windowInfo.y0[segY],
						cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX],
						cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY]);
				}				
			}
		}
	}

}
