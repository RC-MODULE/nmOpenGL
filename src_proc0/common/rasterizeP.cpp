/*!
 *  \file rasterizeP.cpp
 *  \author Zhilenkov Ivan
 */
#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"

#include "stdio.h"
#include "nmprofiler.h"


SECTION(".text_demo3d") void pointOffset(Points &src, Points &dst, int offset) {
	dst.x = src.x + offset;
	dst.y = src.y + offset;
	dst.z = src.z + offset;
	dst.colors = src.colors + offset;
}

SECTION(".text_demo3d")
void rasterizeP(const Points* points, const BitMask* masks){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	int nSegments = cntxt->currentSegments->count;
	Rectangle *rectangles = cntxt->currentSegments->rectangles;
	v2nm32f *lowerLeft = cntxt->currentSegments->lowerLeft;

	Points localPoint;
	Points localPoint2;

	int count = points->size;
	localPoint.x = cntxt->buffer0;
	localPoint.y = cntxt->buffer1;
	localPoint.colors = (v4nm32s*)cntxt->buffer2;
	localPoint.z = (int*)cntxt->buffer3;
	int* indices = (int*)cntxt->buffer4;

	for (int iSeg = 0; iSeg < nSegments; iSeg++) {
		if (masks[iSeg].hasNotZeroBits != 0) {
			int resultSize = readMask(masks[iSeg].bits, indices, count);
			if (resultSize) {
				DataForNmpu1* data = NMGL_PolygonsCurrent(NMGL_POINTS, iSeg);
				bool drawingCheck = data->count + resultSize >= POLYGONS_SIZE;

				if (drawingCheck) {
					NMGL_PopSegment(rectangles[iSeg], iSeg);
				}

				copyArraysByIndices((void**)points, indices, (void**)&localPoint, 3, resultSize);
				copyColorByIndices_BGRA_RGBA(points->colors, indices, (v4nm32s*)localPoint.colors, resultSize);

				localPoint.size = resultSize;					
				int offset = 0;
				Triangles localTrian2;
				while (offset < resultSize) {
					int localSize = MIN(resultSize - offset, POLYGONS_SIZE - data->count);
					pointOffset(localPoint, localPoint2, offset);
					offset += localSize;
					updatePolygonsP(data, &localPoint2, localSize, lowerLeft[iSeg]);
					if (data->count == POLYGONS_SIZE) {
						NMGL_PolygonsMoveNext(NMGL_POINTS, iSeg);
						data = NMGL_PolygonsCurrent(NMGL_POINTS, iSeg);
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