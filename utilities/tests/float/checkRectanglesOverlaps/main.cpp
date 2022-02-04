#include "utility_float.h"
#include "stdio.h"

#define MAX_SIZE 1024
#define STEP 2

#define PRINT_DEBUG(message, level) if(DEBUG_LEVEL > level) printf(message);


v2nm32f minXY[MAX_SIZE];
v2nm32f maxXY[MAX_SIZE];
v2nm32f upper = { 1, 1};
v2nm32f lower = {-1, -1};
int dst[MAX_SIZE + STEP];

void testSize(){
	for(int i = 0; i < MAX_SIZE; i++){
		minXY[i].v0 = 0;
		minXY[i].v1 = 0;
		maxXY[i].v0 = 0;
		maxXY[i].v1 = 0;
	}

	for(int i = 0; i < MAX_SIZE + STEP; i++){
		dst[i] = 0xCDCDCDCD;
	}

	for(int size = 0; size < MAX_SIZE; size+=STEP){

		checkRectanglesOverlaps(minXY, maxXY, &upper, &lower, dst, size);

		int maskSize = (size + 31) / 32;

		if(dst[maskSize] != 0xCDCDCDCD){
			DEBUG_PLOG_ERROR("overflow (size=%d)\n", size);
			return;
		}
		if(maskSize == 0) continue;
		if (dst[maskSize - 1] == 0xCDCDCDCD){
			DEBUG_PLOG_ERROR("overflow (size=%d)\n", size);
			return;
		}
	}
	DEBUG_PLOG_LEVEL_0("Test size OK\n");
}

void testValues(){
	// true
	int size = 0;
	minXY[size] = {-0.5, -0.5}; 	maxXY[size] = {0.5, 0.5};		
	size++;
	// 9 position overlap
	for(float y = -1.5; y < 1; y += 1){
		for(float x = -1.5; x < 1; x+= 1){
			minXY[size] = {x, y};
			maxXY[size] = {x + 1, y + 1};			
			size++;
		}
	}
	for(int i=0; i < size; i++){
		DEBUG_PLOG_LEVEL_2("%2d: minXY={%+2.2f, %+2.2f}, maxXY={%+2.2f, %+2.2f}\n", i, minXY[i].v0, minXY[i].v1, maxXY[i].v0, maxXY[i].v1);
	}
	
	int dst_ref = 0x3FF;
	checkRectanglesOverlaps(minXY, maxXY, &upper, &lower, dst, size);

	int size32 = (size + 31) / 32;
	for(int i = 0; i < size32; i++){
		DEBUG_PLOG_LEVEL_2("%d: dst=0x%x\n", i, dst[i]);
		if(dst[i] != dst_ref){
			DEBUG_PLOG_ERROR("0x%x!=0x%x\n", dst[i], dst_ref);
			return;
		}
	}
	DEBUG_PLOG_LEVEL_0("Test values OK\n");
}

int main(){
	testValues();
	testSize();
	return 0;
}