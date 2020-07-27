#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"

#include "stdio.h"
#include "nmprofiler.h"

//extern  int indices[NMGL_SIZE];
Lines localLine;

SECTION(".text_demo3d")
void rasterizeL(const Lines* lines, const BitMask* masks){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	PolygonsConnector *connector = cntxt->polygonsConnectors;

	int count = lines->size;
	localLine.x0 = cntxt->buffer0;
	localLine.y0 = cntxt->buffer0 + NMGL_SIZE;
	localLine.x1 = cntxt->buffer1;
	localLine.y1 = cntxt->buffer1 + NMGL_SIZE;
	localLine.colors = (v4nm32s*)cntxt->buffer3;
	localLine.z = (int*)cntxt->buffer3 + 4 * NMGL_SIZE;
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

					copyArraysByIndices((void**)lines, indices, (void**)&localLine, 5, resultSize);

#ifdef OUTPUT_IMAGE_RGB8888
					copyColorByIndices_BGRA_RGBA(lines->colors, indices, (v4nm32s*)localLine.colors, resultSize);
#endif // OUTPUT_IMAGE_RGB8888
#ifdef OUTPUT_IMAGE_RGB565
					copyColorByIndices(lines->colors, indices, (v4nm32s*)localLine.colors, resultSize);
#endif // OUTPUT_IMAGE_RGB565


					//waitPolygons(connector);
					while (connector->isFull());
					Polygons* poly = connector->ptrHead();
					poly->count = 0;
					//updatePolygonsL(poly, &localLine, resultSize, segX, segY);

					connector->incHead();
					cntxt->synchro.writeInstr(1, NMC1_DRAW_LINES);

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