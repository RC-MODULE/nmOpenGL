#include "utility_float.h"
#include "debugprint.h"
#include <math.h>
#include "uassert.h"

#define MAX_SIZE 1024
#define STEP 2
#define INIT_DST_VALUE -1


float src[MAX_SIZE];
float dst[MAX_SIZE + STEP];
float expected[MAX_SIZE + STEP];

void resetDst(){
	for(int i = 0; i < MAX_SIZE + STEP; i++){
		dst[i] = INIT_DST_VALUE;
		expected[i] = INIT_DST_VALUE;
		DEBUG_PLOG_LEVEL_3("dst[%2d]=%f\n", i, dst[i]);
	}
}


void logicTest(){
	resetDst();

	for(int i = 0; i < MAX_SIZE; i++){
		src[i] = i - MAX_SIZE / 2;
		expected[i] = fabs(src[i]);
	}

	abs_32f(src, dst, MAX_SIZE);

	for(int i = 0; i < MAX_SIZE; i++){
		DEBUG_PLOG_LEVEL_1("i=%d\n", i);
		uassert(dst[i] == expected[i]);
	}
	DEBUG_PLOG_LEVEL_0("Value test OK\n");
}


void borderTest(){
	resetDst();
	
	for(int size = 0; size < MAX_SIZE; size+=STEP){
		abs_32f(src, dst, size);

		DEBUG_PLOG_LEVEL_1("size=%d\n", size);
		for(int i = 0; i < size; i++){
			DEBUG_PLOG_LEVEL_2("%d: src=%f, dst=%f\n", i, src[i], dst[i]);
		}
		DEBUG_PLOG_LEVEL_2("dst=%f\n", src[size], dst[size]);


		m_uassert(dst[size] == INIT_DST_VALUE, "overflow: size=%d\n", size);

		if(size == 0) continue;

		m_uassert(dst[size - 1] != INIT_DST_VALUE, "underflow: size=%d\n", size);
	}
	DEBUG_PLOG_LEVEL_0("Size test OK\n");
}



int main(){
	DEBUG_PLOG_FILE();
	logicTest();
	borderTest();
	return 0;
}