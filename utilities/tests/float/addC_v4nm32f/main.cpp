#include "utility_float.h"
#include "debugprint.h"

#define MAX_SIZE 1024
#define STEP 2
#define INIT_DST_VALUE -1

void initSrc(v4nm32f *src, int size){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < 4; j++){
			src[i].vec[j] = 4 * i + j;
		}
	}
}
void initDst(v4nm32f *dst, int size){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < 4; j++){
			dst[i].vec[j] = INIT_DST_VALUE;
		}
	}
}

v4nm32f src[MAX_SIZE];
v4nm32f srcC;
v4nm32f dst[MAX_SIZE + STEP];

void testSize(){
	initSrc(src, MAX_SIZE);
	initDst(dst, MAX_SIZE + STEP);
	
	srcC = {1,2,3,4};

	for(int size = 0; size < MAX_SIZE; size+=STEP){

		addC_v4nm32f(src, &srcC, dst, size);

		for (int i = 0; i < 4; i++)
		{
			if(dst[size].vec[i] != INIT_DST_VALUE){
				DEBUG_PLOG_ERROR("overflow(size=%d)\n", size);
				return;
			}
		}

		if(size == 0) continue;
		for (int i = 0; i < 4; i++){
			if (dst[size - 1].vec[i] == INIT_DST_VALUE){
				DEBUG_PLOG_ERROR("underflow (size=%d)\n", size);
				return;
			}
		}
	}
	DEBUG_PLOG_LEVEL_0("testSize OK\n");
}

void testValues(){
	src[0] = {0, -3, 4, -1};
	srcC = {1, 2, 3, 4};
	v4nm32f ref_values = {1, -1, 7, 3};

	addC_v4nm32f(src, &srcC, dst, 1);

	for(int i = 0; i < 4; i++){
		if(dst[0].vec[i] != ref_values.vec[i]){
			DEBUG_PLOG_ERROR("%d!=%d\n", dst[0].vec[i], ref_values.vec[i]);
			return;
		}
	}
	DEBUG_PLOG_LEVEL_0("Test values OK\n");
}

int main(){
	DEBUG_PLOG_FILE();
	testSize();
	testValues();
	return 0;
}