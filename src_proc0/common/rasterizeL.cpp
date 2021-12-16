/*!
 *  \file rasterizeL.cpp
 *  \author Zhilenkov Ivan
 */
#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"

#include "stdio.h"
#include "nmprofiler.h"

//extern  int indices[NMGL_SIZE];
SECTION(".text_demo3d") void lineOffset(Lines &src, Lines &dst, int offset) {
	dst.x0 = src.x0 + offset;
	dst.y0 = src.y0 + offset;
	dst.x1 = src.x1 + offset;
	dst.y1 = src.y1 + offset;
	dst.s0 = src.s0 + offset;
	dst.t0 = src.t0 + offset;
	dst.s1 = src.s1 + offset;
	dst.t1 = src.t1 + offset;
	dst.w0 = src.w0 + offset;
	dst.w1 = src.w1 + offset;
	dst.z = src.z + offset;
	dst.colors = src.colors + offset;
}

SECTION(".text_demo3d")
void rasterizeL(const Lines* lines, const BitMask* masks){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	int nSegments = cntxt->currentSegments->count;
	Rectangle *rectangles = cntxt->currentSegments->rectangles;
	v2nm32f *lowerLeft = cntxt->currentSegments->lowerLeft;

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
				DataForNmpu1* data = NMGL_PolygonsCurrent(NMGL_LINES, iSeg);
				bool drawingCheck = data->count + resultSize >= POLYGONS_SIZE;
				if (drawingCheck) {
					NMGL_PopSegment(rectangles[iSeg], iSeg);
				}

				copyArraysByIndices((void**)lines, indices, (void**)&localLine, 5, resultSize);
				copyColorByIndices_BGRA_RGBA(lines->colors, indices, (v4nm32s*)localLine.colors, resultSize);
				localLine.size = resultSize;
			
				int offset = 0;
				while (offset < resultSize) {
					int localSize = MIN(resultSize - offset, POLYGONS_SIZE - data->count);
					lineOffset(localLine, localLine2, offset);
					offset += localSize;
					updatePolygonsL(data, &localLine2, localSize, lowerLeft[iSeg]);
					if (data->count == POLYGONS_SIZE) {
						NMGL_PolygonsMoveNext(NMGL_LINES, iSeg);
						data = NMGL_PolygonsCurrent(NMGL_LINES, iSeg);
						data->count = 0;
					}
				}	
				if (drawingCheck) {
					NMGL_PushSegment(rectangles[iSeg], iSeg);
				}
			}
		}
	}
}