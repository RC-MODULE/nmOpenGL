#include "utility_fixed.h"
#include "section-hal.h"

#define MAX_SIZE 1024
#define STEP 2


INSECTION(".data_imu0") int src[MAX_SIZE];
INSECTION(".data_imu1") int dst[MAX_SIZE + STEP];

void testSize(){
	for(int i = 0; i < MAX_SIZE; i++){
		src[i] = 0;
	}
	for(int i = 0; i < MAX_SIZE + STEP; i++){
		dst[i] = 0xCDCDCDCD;
	}
	for(int size = 0; size < MAX_SIZE; size+=STEP){
		absIfNegElse0(src, dst, size);
		if(dst[size] != 0xCDCDCDCD){
			DEBUG_PLOG_ERROR("size=%d\n", size);
			return;
		}
		if(size !=0 && dst[size - 1] == 0xCDCDCDCD){
			DEBUG_PLOG_ERROR("size = %d\n", size);
			return;
		}
	}
	DEBUG_PLOG_LEVEL_0("Test size OK\n");
}

void testValues(){
	src[0] = 0;
	src[1] = -3;
	src[2] = 4;
	src[3] = -1;
	int ref_values[4] = {0, 3, 0, 1};
	absIfNegElse0(src, dst, 4);
	for(int i = 0; i < 4; i++){
		if(dst[i] != ref_values[i]){
			DEBUG_PLOG_ERROR("%d!=%d\n", dst[i], ref_values[i]);
			return;
		}
	}
	DEBUG_PLOG_LEVEL_0("Test values OK\n");
}

int main(){
	DEBUG_PLOG_LEVEL_1("src=%p, dst=%p\n", src, dst);
	testValues();
	testSize();
	return 0;
}