#include "segment.h"
#include "uassert.h"
#include "debugprint.h"
#include "nmdata.h"
#include <math.h>

#define SIZE 1024

ImageSegments segments;	

DATA_IMU0 v2nm32f minXY[SIZE];
DATA_IMU1 v2nm32f maxXY[SIZE];
BitMask masks[MAX_SEGMENTS_COUNT];
int bit_masks[MAX_SEGMENTS_COUNT][ SIZE / 32 ];

void initSegmentsTest(){
	{
		Vector2 origin = {128, 128};
		Size size = {256, 256};
		Size segmentSize = {128, 128};
		int ok = segmentsInit(&segments, origin, size, segmentSize);
		uassert(ok == 0);
		DEBUG_PLOG_LEVEL_1("segments.nColumns=%d\n", segments.nColumns);
		DEBUG_PLOG_LEVEL_2("segments.nRows=%d\n", segments.nRows);
		uassert(segments.nColumns == 2);
		uassert(segments.nRows == 2);
	}
	{
		Vector2 origin = {-128, -128};
		Size size = {256, 256};
		Size segmentSize = {128, 128};
		int ok = segmentsInit(&segments, origin, size, segmentSize);
		uassert(ok == 0);
	}
	{
		Vector2 origin = {-128, -128};
		Size size = {1024, 2048};
		Size segmentSize = {128, 128};
		int ok = segmentsInit(&segments, origin, size, segmentSize);
		uassert(ok == -1);
	}
	{
		Vector2 origin = {128, 128};
		Size size = {-256, 256};
		Size segmentSize = {128, 128};
		int ok = segmentsInit(&segments, origin, size, segmentSize);
		uassert(ok == -2);
	}
	{
		Vector2 origin = {128, 128};
		Size size = {256, 256};
		Size segmentSize = {128, -128};
		int ok = segmentsInit(&segments, origin, size, segmentSize);
		uassert(ok == -3);
	}

	DEBUG_PLOG_LEVEL_0("Segments Init OK\n");
}
void valueTest(){
	Vector2 origin = {128, 128};
	Size size = {768, 768};
	Size segmentSize = {128, 128};

	segmentsInit(&segments, origin, size, segmentSize);
	for(int i = 0; i < MAX_SEGMENTS_COUNT; i++){
		masks[i].hasNotZeroBits = 0;
	}

	{
		DEBUG_PLOG_LEVEL_1("Test 1\n");
		for(int i = 0; i < SIZE; i++){
			minXY[i] = {-256, -256};
			maxXY[i] = { -128, -128};
		}
		setSegmentMask(&segments, minXY, maxXY, masks, SIZE);
		for(int i = 0; i < MAX_SEGMENTS_COUNT; i++){
			uassert(masks[i].hasNotZeroBits == 0);
		}
	}
	
	{
		DEBUG_PLOG_LEVEL_1("Test 2\n");
		for(int i = 0; i < SIZE; i++){
			minXY[i] = { 128, -256};
			maxXY[i] = { 256, -128};
		}
		setSegmentMask(&segments, minXY, maxXY, masks, SIZE);
		for(int i = 0; i < MAX_SEGMENTS_COUNT; i++){
			uassert(masks[i].hasNotZeroBits == 0);
		}
	}

	{
		DEBUG_PLOG_LEVEL_1("Test 3\n");
		for(int i = 0; i < SIZE; i++){
			minXY[i] = { 256, 256};
			maxXY[i] = { 270, 270};
		}
		setSegmentMask(&segments, minXY, maxXY, masks, SIZE);
		for(int i = 0; i < segments.count; i++){
			//DEBUG_PLOG_LEVEL_1("%d\n", masks[i].hasNotZeroBits);
			if (i == segments.nColumns * 1 + 1){
				uassert(masks[i].hasNotZeroBits != 0);
			}
			else{
				uassert(masks[i].hasNotZeroBits == 0);
			}
		}
	}

	
	DEBUG_PLOG_LEVEL_0("Test value OK\n");
}

int main(){
	DEBUG_PLOG_LEVEL_0("Segment tests\n");

	for(int i = 0; i < MAX_SEGMENTS_COUNT; i++){
		masks[i].bits = bit_masks[i];
	}
	initSegmentsTest();
	valueTest();
	
	return 0;
}