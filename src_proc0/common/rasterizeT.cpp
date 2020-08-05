#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"

#include "stdio.h"
#include "nmprofiler.h"

SECTION(".data_imu7")   int indices[NMGL_SIZE];
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
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	int count = triangles->size;
	localTrian.x0 = cntxt->buffer0;
	localTrian.y0 = cntxt->buffer0 + NMGL_SIZE;
	localTrian.x1 = cntxt->buffer1;
	localTrian.y1 = cntxt->buffer1 + NMGL_SIZE;
	localTrian.x2 = cntxt->buffer2;
	localTrian.y2 = cntxt->buffer2 + NMGL_SIZE;
	localTrian.colors = (v4nm32s*)cntxt->buffer3;
	localTrian.z = (int*)cntxt->buffer3 + 4 * NMGL_SIZE;

	PolygonsConnector connector(cntxt->polygonsData);

	for (int segY = 0, iSeg = 0; segY < cntxt->windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt->windowInfo.nColumns; segX++, iSeg++) {
			if (masks[iSeg].hasNotZeroBits != 0) {

				int resultSize = readMask(masks[iSeg].bits, indices, count);
				if (resultSize) {

					static int counter = 0;
					if (counter++ == 3) {
						counter = counter;
					}

					cntxt->synchro.writeInstr(1, NMC1_COPY_SEG_FROM_IMAGE,
						cntxt->windowInfo.x0[segX],
						cntxt->windowInfo.y0[segY],
						cntxt->windowInfo.x1[segX] - cntxt->windowInfo.x0[segX],
						cntxt->windowInfo.y1[segY] - cntxt->windowInfo.y0[segY],
						iSeg);

					copyArraysByIndices((void**)triangles, indices, (void**)&localTrian, 7, resultSize);

#ifdef OUTPUT_IMAGE_RGB8888
					copyColorByIndices_BGRA_RGBA(triangles->colors, indices, (v4nm32s*)localTrian.colors, resultSize);
#endif // OUTPUT_IMAGE_RGB8888
#ifdef OUTPUT_IMAGE_RGB565
					copyColorByIndices(triangles->colors, indices, (v4nm32s*)localTrian.colors, resultSize);
#endif // OUTPUT_IMAGE_RGB565
					

					//waitPolygons(connector);
					while (connector.isFull());
					Polygons* poly = connector.ptrHead();
					poly->count = 0;
					updatePolygonsT(poly, &localTrian, resultSize, segX, segY);

					connector.incHead();
					cntxt->synchro.writeInstr(1, NMC1_DRAW_TRIANGLES);

					cntxt->synchro.writeInstr(1,
						NMC1_COPY_SEG_TO_IMAGE,
						cntxt->windowInfo.x0[segX],
						cntxt->windowInfo.y0[segY],
						cntxt->windowInfo.x1[segX] - cntxt->windowInfo.x0[segX],
						cntxt->windowInfo.y1[segY] - cntxt->windowInfo.y0[segY]);
				}				
			}
		}
	}

}