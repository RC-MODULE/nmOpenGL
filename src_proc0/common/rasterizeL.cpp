/*!
 *  \file rasterizeL.cpp
 *  \author Zhilenkov Ivan
 */
#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"

#include "stdio.h"
#include "nmprofiler.h"

//extern  int indices[NMGL_SIZE];
SECTION(".text_demo3d") void lineOffset(Lines &src, Lines &dst, int offset) {
	dst.x0 = src.x0 + offset;
	dst.y0 = src.y0 + offset;
	dst.x1 = src.x1 + offset;
	dst.y1 = src.y1 + offset;
	dst.z = src.z + offset;
	dst.colors = src.colors + offset;
}

SECTION(".text_demo3d")
void rasterizeL(const Lines* lines, const BitMask* masks){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	Lines localLine;
	Lines localLine2;

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
					PolygonsConnector *connector = cntxt->lineConnectors + iSeg;
					bool drawingCheck = connector->ptrHead()->count + resultSize >= POLYGONS_SIZE;
					if (drawingCheck) {
						cntxt->synchro.writeInstr(1, NMC1_COPY_SEG_FROM_IMAGE,
							cntxt->windowInfo.x0[segX],
							cntxt->windowInfo.y0[segY],
							cntxt->windowInfo.x1[segX] - cntxt->windowInfo.x0[segX],
							cntxt->windowInfo.y1[segY] - cntxt->windowInfo.y0[segY],
							iSeg);
					}

					copyArraysByIndices((void**)lines, indices, (void**)&localLine, 5, resultSize);

#ifdef OUTPUT_IMAGE_RGB8888
					copyColorByIndices_BGRA_RGBA(lines->colors, indices, (v4nm32s*)localLine.colors, resultSize);
#endif // OUTPUT_IMAGE_RGB8888
#ifdef OUTPUT_IMAGE_RGB565
					copyColorByIndices(lines->colors, indices, (v4nm32s*)localLine.colors, resultSize);
#endif // OUTPUT_IMAGE_RGB565

					localLine.size = resultSize;
					
					int offset = 0;
					while (offset < resultSize) {
						DataForNmpu1* data = connector->ptrHead();
						int localSize = MIN(resultSize - offset, POLYGONS_SIZE - data->count);
						lineOffset(localLine, localLine2, offset);
						offset += localSize;
						updatePolygonsL(data, &localLine2, localSize, segX, segY);
						if (data->count == POLYGONS_SIZE) {
							transferPolygons(connector, NMC1_DRAW_LINES);
						}
					}	
					if (drawingCheck) {
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
}