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

	int nSegments = cntxt->currentSegments->count;
	Rectangle* rectangles = cntxt->currentSegments->rectangles;
	v2nm32f* lowerLeft = cntxt->currentSegments->lowerLeft;

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

	for (int iSeg = 0; iSeg < nSegments; iSeg++) {
		if (masks[iSeg].hasNotZeroBits != 0) {

			int resultSize = readMask(masks[iSeg].bits, indices, count);
			if (resultSize) {
				CommandNm1 command;
				command.instr = NMC1_COPY_SEG_FROM_IMAGE;
				command.params[0] = CommandArgument(rectangles[iSeg].x);
				command.params[1] = CommandArgument(rectangles[iSeg].y);
				command.params[2] = CommandArgument(rectangles[iSeg].width);
				command.params[3] = CommandArgument(rectangles[iSeg].height);
				command.params[4] = CommandArgument(iSeg );

				PolygonsConnector *connector = cntxt->lineConnectors + iSeg;
				bool drawingCheck = connector->ptrHead()->count + resultSize >= POLYGONS_SIZE;
				if (drawingCheck) {
					cntxt->synchro.pushInstr(&command);
				}

				copyArraysByIndices((void**)lines, indices, (void**)&localLine, 5, resultSize);
				copyColorByIndices_BGRA_RGBA(lines->colors, indices, (v4nm32s*)localLine.colors, resultSize);
				localLine.size = resultSize;
			
				int offset = 0;
				while (offset < resultSize) {
					DataForNmpu1* data = connector->ptrHead();
					int localSize = MIN(resultSize - offset, POLYGONS_SIZE - data->count);
					lineOffset(localLine, localLine2, offset);
					offset += localSize;
					updatePolygonsL(data, &localLine2, localSize, lowerLeft[iSeg]);
					if (data->count == POLYGONS_SIZE) {
						transferPolygons(connector, NMC1_DRAW_LINES);
					}
				}	
				if (drawingCheck) {
					command.instr = NMC1_COPY_SEG_TO_IMAGE;
					cntxt->synchro.pushInstr(&command);
				}
			}
		}
	}
}