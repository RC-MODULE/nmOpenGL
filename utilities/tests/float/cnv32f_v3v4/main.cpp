#include "utility_float.h"
#include "debugprint.h"
#include "uassert.h"

#define MAX_SIZE 1024
#define INIT_DST_VALUE -1
#define STEP 2

float src[3 * MAX_SIZE];
float dst[4 * MAX_SIZE + STEP];

void initDst(float *dst, int size){
	for(int i = 0; i < size; i++){
		dst[i] = INIT_DST_VALUE;
		//DEBUG_PLOG_LEVEL_2("dst[%d]=%f\n", i, dst[i]);
	}
}


void testValue(){
	src[0] = 1;
	src[1] = 1;
	src[2] = 2;

	src[3] = 3;
	src[4] = -1;
	src[5] = 0;

	src[6] = 0;
	src[7] = 0;
	src[8] = 8;

	src[9]  = -1;
	src[10] = -2;
	src[11] = -1555;
	initDst(dst, 4 * MAX_SIZE + STEP);

	float dst_ref[] = { 1, 1, 2, 2,    3, -1, 0, 2,  0, 0, 8, 2,     -1, -2, -1555, 2 };

	cnv32f_v3v4(src, dst, 2, 4);
	for(int i = 0; i < 12; i++){
		uassert(dst[i] == dst_ref[i]);
		// if (dst[i] != dst_ref[i]){
		// 	DEBUG_PLOG_ERROR("%f!=%f\n", dst[i], dst_ref[i]);
		// 	return;
		// }
	}
	DEBUG_TEST_OK();
}

void testSize(){
	initDst(dst, 4 * MAX_SIZE + STEP);
	for(int i = 0; i < 3 * MAX_SIZE; i++){
		src[i] = i;
	}
	for(int size = 0; size < MAX_SIZE; size += STEP){
		cnv32f_v3v4(src, dst, 4, size);
		uassert(dst[4 * size] == INIT_DST_VALUE);
		// if (dst[4 * size] != INIT_DST_VALUE){
		// 	DEBUG_PLOG_ERROR("overflow (size=%d)\n", size);
		// 	DEBUG_PLOG_LEVEL_1("dst[%4d]: {%f, %f, %f, %f}\n", size, dst[4 * size + 0], dst[4 * size + 1], dst[4 * size + 2], dst[4 * size + 3]);
		// 	return;
		// }
		if (size == 0) continue;
		uassert(dst[4 * size - 1] != INIT_DST_VALUE);
		// if (dst[4 * size - 1] == INIT_DST_VALUE){
		// 	DEBUG_PLOG_ERROR("underflow (size=%d)\n", size);
		// 	return;
		// }
	}
	DEBUG_TEST_OK();
}


int main(){
	DEBUG_PLOG_FILE();
	testValue();
	testSize();
	return 0;
}