#include "utility_float.h"
#include "stdio.h"

#define MAX_SIZE 1024
#define STEP 2



v4nm32f src[MAX_SIZE];
v4nm32f srcC;
v4nm32f dst[MAX_SIZE + STEP];

void testSize(){
	srcC = {0,0,0,0};
	for(int i = 0; i < MAX_SIZE; i++){
		for(int j = 0; j < 4; j++){
			src[i].vec[j] = 0;
		}
	}
	v4nm32f dst_ref;
	dst_ref.vec[0] = 0xCDCDCDCD;
	dst_ref.vec[1] = 0xCDCDCDCD;
	dst_ref.vec[2] = 0xCDCDCDCD;
	dst_ref.vec[3] = 0xCDCDCDCD;

	for(int i = 0; i < MAX_SIZE + STEP; i++){
		for(int j = 0; j < 4; j++){
			dst[i].vec[j] = dst_ref.vec[j];
		}
	}

	for(int size = 0; size < MAX_SIZE; size+=STEP){

		addC_v4nm32f(src, &srcC, dst, size);

		for (int i = 0; i < 4; i++)
		{
			if(dst[size].vec[i] != dst_ref.vec[i]){
				DEBUG_PLOG_ERROR("overflow(size=%d)\n", size);
				return;
			}
		}

		if(size == 0) continue;
		for (int i = 0; i < 4; i++){
			if (dst[size - 1].vec[i] == dst_ref.vec[i]){
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
	testSize();
	testValues();
	return 0;
}