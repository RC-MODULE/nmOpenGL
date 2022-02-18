/*!
 *  \file setSegmentMask.cpp
 *  \author Zhilenkov Ivan
 */
#include "nmpp.h"
#include "utility_float.h"
#include "segment.h"
#include "uassert.h"



int firstNonZeroIndx_32s(int* pSrcVec, int nSize);

//SECTION(".text_demo3d")
void setSegmentMask(ImageSegments *segments, v2nm32f* minXY, v2nm32f* maxXY, BitMask* masks, int size){

	int nSegments = segments->count;
	v2nm32f* lowerLimit = segments->lowerLeft;
	v2nm32f* upperLimit = segments->upperRight;

	for (int iSeg = 0; iSeg < nSegments; iSeg++) {
		checkRectanglesOverlaps(minXY, maxXY, upperLimit + iSeg, lowerLimit + iSeg, masks[iSeg].bits, size);
		int size32 = (size + 31) / 32;
		int ind = firstNonZeroIndx_32s(masks[iSeg].bits, size32);
		masks[iSeg].hasNotZeroBits = (ind != -1);
	}

}

int segmentsInit(ImageSegments *segments, Vector2 origin, Size size, Size segmensSize){
	segments->nColumns = (size.height + segmensSize.height - 1) / segmensSize.height;
	segments->nRows = (size.width + segmensSize.width - 1) / segmensSize.width;

	if(segments->nColumns * segments->nRows > MAX_SEGMENTS_COUNT){
		return -1;
	}
	if(size.width < 0 || size.height < 0){
		return -2;
	}
	if(segmensSize.width < 0 || segmensSize.height < 0){
		return -3;
	}

	int endX = origin.x + size.width;
	int endY = origin.y + size.height;
	int nSegments = 0;
	for (int y0 = origin.y; y0 < endY; y0 += segmensSize.height) {
		for (int x0 = origin.x; x0 < endX; x0 += segmensSize.width, nSegments++) {
			int localWidth = MIN(segmensSize.width, endX - x0);
			int localHeight = MIN(segmensSize.height, endY - y0);
			segments->rectangles[nSegments].x = x0;
			segments->rectangles[nSegments].y = y0;
			segments->rectangles[nSegments].width = localWidth;
			segments->rectangles[nSegments].height = localHeight;
			segments->lowerLeft[nSegments].v0 = x0;
			segments->lowerLeft[nSegments].v1 = y0;
			segments->upperRight[nSegments].v0 = x0 + localWidth;
			segments->upperRight[nSegments].v1 = y0 + localHeight;
		}
	}
	segments->count = nSegments;
	return 0;
}
