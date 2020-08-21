#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"

#include "stdio.h"
#include "nmprofiler.h"


SECTION(".text_demo3d") void pointOffset(Points &src, Points &dst, int offset) {
	dst.x0 = src.x0 + offset;
	dst.y0 = src.y0 + offset;
	dst.z = src.z + offset;
	dst.colors = src.colors + offset;
}

SECTION(".text_demo3d")
void rasterizeP(const Points* points, const BitMask* masks){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	PolygonsConnector *connector = cntxt->polygonsConnectors;
	Points localPoint;
	Points localPoint2;

	int count = points->size;
	localPoint.x0 = cntxt->buffer0;
	localPoint.y0 = cntxt->buffer0 + NMGL_SIZE;
	localPoint.colors = (v4nm32s*)cntxt->buffer3;
	localPoint.z = (int*)cntxt->buffer3 + 4 * NMGL_SIZE;
	int* indices = (int*)cntxt->buffer4;

	for (int segY = 0, iSeg = 0; segY < cntxt->windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt->windowInfo.nColumns; segX++, iSeg++) {
			if (masks[iSeg].hasNotZeroBits != 0) {

				int resultSize = readMask(masks[iSeg].bits, indices, count);
				if (resultSize) {

					cntxt->synchro.writeInstr(1, NMC1_COPY_SEG_FROM_IMAGE,
						cntxt->windowInfo.x0[segX],
						cntxt->windowInfo.y0[segY],
						cntxt->windowInfo.x1[segX] - cntxt->windowInfo.x0[segX],
						cntxt->windowInfo.y1[segY] - cntxt->windowInfo.y0[segY],
						iSeg);

					copyArraysByIndices((void**)points, indices, (void**)&localPoint, 3, resultSize);

#ifdef OUTPUT_IMAGE_RGB8888
					copyColorByIndices_BGRA_RGBA(points->colors, indices, (v4nm32s*)localPoint.colors, resultSize);
#endif // OUTPUT_IMAGE_RGB8888
#ifdef OUTPUT_IMAGE_RGB565
					copyColorByIndices(points->colors, indices, (v4nm32s*)localPoint.colors, resultSize);
#endif // OUTPUT_IMAGE_RGB565

					localPoint.size = resultSize;
					
					int offset = 0;
					Triangles localTrian2;
					while (offset < resultSize) {
						int localSize = MIN(resultSize - offset, POLYGONS_SIZE);
						pointOffset(localPoint, localPoint2, offset);
						offset += POLYGONS_SIZE;
						while (connector[0].isFull());
						Polygons* poly = connector[0].ptrHead();
						poly->count = 0;
						updatePolygonsP(poly, &localPoint2, localSize, segX, segY);
						connector[0].incHead();
						cntxt->synchro.writeInstr(1, NMC1_DRAW_POINTS);
					}	

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