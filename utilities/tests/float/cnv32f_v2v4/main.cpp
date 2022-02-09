#include "utility_float.h"
#include "debugprint.h"

#define MAX_SIZE 1024
#define INIT_DST_VALUE -1
#define STEP 1

v2nm32f src[MAX_SIZE];
v4nm32f dst[MAX_SIZE + STEP];

void initDst(v4nm32f *dst, int size){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < 4; j++){
			dst[i].vec[j] = INIT_DST_VALUE;
		}
	}
}

void testValue(){
	src[0].v0 = 1;
	src[0].v1 = 1;
	src[1].v0 = 3;
	src[1].v1 = -1;
	src[2].v0 = 0;
	src[2].v1 = 0;
	initDst(dst, MAX_SIZE + STEP);

	v4nm32f dst_ref[] = { {1, 1, 1, 2}, {3, -1, 1, 2}, {0, 0, 1, 2} };

	cnv32f_v2v4(src, dst, 1, 2, 3);
	for(int i = 0; i < 3; i++){
		for(int j = 0; j< 4; j++){
			if (dst[i].vec[j] != dst_ref[i].vec[j]){
				DEBUG_PLOG_ERROR("%f!=%f\n", dst[i].vec[j], dst_ref[i].vec[j]);
				return;
			}
		}
	}
	DEBUG_PLOG_LEVEL_0("Test value OK\n");
}

void testSize(){
	initDst(dst, MAX_SIZE + STEP);
	for(int i = 0; i < MAX_SIZE; i++){
		src[i].v0 = 2 * i;
		src[i].v1 = 2 * i + 1;
	}
	for(int size = 0; size < MAX_SIZE; size += STEP){
		cnv32f_v2v4(src, dst, 3, 4, size);
		if (dst[size].vec[0] != INIT_DST_VALUE){
			DEBUG_PLOG_ERROR("overflow (size=%d)\n", size);
			return;
		}
		if (size == 0) continue;
		if (dst[size - 1].vec[3] == INIT_DST_VALUE){
			DEBUG_PLOG_ERROR("underflow (size=%d)\n", size);
			return;
		}
	}
	DEBUG_PLOG_LEVEL_0("Test size OK\n");
}


int main(){
	DEBUG_PLOG_FILE();
	testValue();
	testSize();
	return 0;
}