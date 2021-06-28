/*!
 *  \file rasterizeP.cpp
 *  \author Zhilenkov Ivan
 */
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

	int nSegments = cntxt->currentSegments->count;
	Rectangle* rectangles = cntxt->currentSegments->rectangles;
	v2nm32f* lowerLeft = cntxt->currentSegments->lowerLeft;

	Points localPoint;
	Points localPoint2;

	int count = points->size;
	localPoint.x0 = cntxt->buffer0;
	localPoint.y0 = cntxt->buffer1;
	localPoint.colors = (v4nm32s*)cntxt->buffer2;
	localPoint.z = (int*)cntxt->buffer3;
	int* indices = (int*)cntxt->buffer4;

	for (int iSeg = 0; iSeg < nSegments; iSeg++) {
		if (masks[iSeg].hasNotZeroBits != 0) {
			int resultSize = readMask(masks[iSeg].bits, indices, count);
			if (resultSize) {

				PolygonsConnector *connector = cntxt->pointConnectors + iSeg;
				bool drawingCheck = connector->ptrHead()->count + resultSize >= POLYGONS_SIZE;

				CommandNm1 command;
				if (drawingCheck) {
					command.instr = NMC1_COPY_SEG_FROM_IMAGE;
					command.params[0] = CommandArgument(rectangles[iSeg].x);
					command.params[1] = CommandArgument(rectangles[iSeg].y);
					command.params[2] = CommandArgument(rectangles[iSeg].width);
					command.params[3] = CommandArgument(rectangles[iSeg].height);
					command.params[4] = CommandArgument(iSeg);
					cntxt->synchro.pushInstr(&command);
				}


				copyArraysByIndices((void**)points, indices, (void**)&localPoint, 3, resultSize);

				copyColorByIndices_BGRA_RGBA(points->colors, indices, (v4nm32s*)localPoint.colors, resultSize);

				localPoint.size = resultSize;
					
				int offset = 0;
				Triangles localTrian2;
				while (offset < resultSize) {
					DataForNmpu1* data = connector->ptrHead();
					int localSize = MIN(resultSize - offset, POLYGONS_SIZE - data->count);
					pointOffset(localPoint, localPoint2, offset);
					offset += localSize;
					updatePolygonsP(data, &localPoint2, localSize, lowerLeft[iSeg]);
					if (data->count == POLYGONS_SIZE) {
						transferPolygons(connector, NMC1_DRAW_POINTS);
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