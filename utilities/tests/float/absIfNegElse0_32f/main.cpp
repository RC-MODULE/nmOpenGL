#include "utility_float.h"
#include "debugprint.h"

#define MAX_SIZE 1024
#define STEP 2
#define INIT_DST_VALUE -1


float src[MAX_SIZE];
float dst[MAX_SIZE + STEP];

void initSrc(float *src, int size){
	for(int i = 0; i < MAX_SIZE; i++){
		src[i] = i;
	}
}
void initDst(float *dst, int size){
	for(int i = 0; i < MAX_SIZE + STEP; i++){
		dst[i] = INIT_DST_VALUE;
	}
}


void testSize(){
	initSrc(src, MAX_SIZE);
	initDst(dst, MAX_SIZE + STEP);

	for(int size = 0; size < MAX_SIZE; size+=STEP){
		absIfNegElse0_32f(src, dst, size);
		if(dst[size] != INIT_DST_VALUE){
			DEBUG_PLOG_ERROR("overflow (size=%d)\n", size);
			return;
		}
		if(size == 0) continue;
		if(dst[size - 1] == INIT_DST_VALUE){
			DEBUG_PLOG_ERROR("underflow (size=%d)\n", size);
			return;
		}
	}
	DEBUG_PLOG_LEVEL_0("Test size OK\n");
}

void testValues(){
	
	initDst(dst, MAX_SIZE + STEP);
	src[0] = 0;
	src[1] = -3;
	src[2] = 4;
	src[3] = -1;
	float ref_values[4] = {0, 3, 0, 1};


	absIfNegElse0_32f(src, dst, 4);


	for(int i = 0; i < 4; i++){
		if(dst[i] != ref_values[i]){
			DEBUG_PLOG_ERROR("%f!=%f\n", dst[i], ref_values[i]);
			return;
		}
	}
	DEBUG_PLOG_LEVEL_0("Test values OK\n");
}

int main(){
	DEBUG_PLOG_FILE();
	testValues();
	testSize();
	return 0;
}