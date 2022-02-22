#include "utility_float.h"
#include "debugprint.h"
#include "uassert.h"

#define MAX_SIZE 1024
#define STEP 2


float src[MAX_SIZE];
float dst[MAX_SIZE + STEP];

void testSize(){
	for(int i = 0; i < MAX_SIZE; i++){
		src[i] = 0;
	}
	for(int i = 0; i < MAX_SIZE + STEP; i++){
		dst[i] = 0xCDCDCDCD;
	}
	for(int size = 0; size < MAX_SIZE; size+=STEP){
		clamp_32f(src, -1, 1, dst, size);
		uassert(dst[size] == 0xCDCDCDCD)
		// if(dst[size] != 0xCDCDCDCD){
		// 	DEBUG_PLOG_ERROR("overflow (size=%d)\n", size);
		// 	return;
		// }
		if(size == 0) continue;
		uassert(dst[size - 1] != 0xCDCDCDCD)
		// if(dst[size - 1] == 0xCDCDCDCD){
		// 	DEBUG_PLOG_ERROR("underflow (size=%d)\n", size);
		// 	return;
		// }
	}
	DEBUG_TEST_OK();
}

void testValues(){
	src[0] = 0;
	src[1] = 0.5;
	src[2] = -0.75;
	src[3] = -1;
	src[4] = 1;
	src[5] = -2;
	src[6] = 3;
	src[7] = -1000;
	float ref_values[8] = {0, 0.5, -0.75, -1, 1, -1, 1, -1};
	clamp_32f(src, -1, 1, dst, 8);
	for(int i = 0; i < 8; i++){
		uassert(dst[i] == ref_values[i]);
		// if(dst[i] != ref_values[i]){
		// 	DEBUG_PLOG_ERROR("%f!=%f\n", dst[i], ref_values[i]);
		// 	return;
		// }
	}
	DEBUG_TEST_OK();
}

int main(){
	DEBUG_PLOG_FILE();
	testSize();
	testValues();
	return 0;
}