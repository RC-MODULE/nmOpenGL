#ifndef __SEGMENT_H__
#define __SEGMENT_H__
#include "utility.h"
#include "nmtype.h"

#define MAX_SEGMENTS_COUNT 40

struct ImageSegments{
	Rectangle rectangles[MAX_SEGMENTS_COUNT];
	v2nm32f lowerLeft[MAX_SEGMENTS_COUNT];
	v2nm32f upperRight[MAX_SEGMENTS_COUNT];
	int nColumns;
	int nRows;
	int count;
	int dummy;
};

int segmentsInit(ImageSegments *segments, Vector2 origin, Size size, Size segmensSize);


void setSegmentMask(ImageSegments *segments, v2nm32f* minXY, v2nm32f* maxXY, BitMask* masks, int size);

#endif