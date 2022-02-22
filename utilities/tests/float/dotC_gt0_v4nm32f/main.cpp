#include "utility_float.h"
#include "debugprint.h"
#include "extnmtype.h"
#include "malloc32.h"
#include "section-hal.h"
#include <math.h>
#include "uassert.h"

#define MAX_SIZE 1024
#define STEP 2
#define INIT_DST_VALUE -1


INSECTION(".data_imu0") v4nm32f src0[MAX_SIZE];
INSECTION(".data_imu1") v4nm32f src1 = {2,3,4,5};
INSECTION(".data_imu2") v2nm32f dst[MAX_SIZE + STEP];
INSECTION(".data_imu3") v2nm32f dst_ref[MAX_SIZE + STEP];

void initSrc(v4nm32f *src, int size){
	for(int i = 0; i < MAX_SIZE; i++){
		src[i] = { (float)(i - size / 2), 2.0f * (i - size / 2), 3.0f * (i - size / 2), 4.0f * (i - size / 2) };
	}
}
void initDst(v2nm32f *dst, int size){
	for(int i = 0; i < MAX_SIZE + STEP; i++){
		dst[i] = { INIT_DST_VALUE, INIT_DST_VALUE };
	}
}

void testSize(){
	initSrc(src0, MAX_SIZE);
	initDst(dst, MAX_SIZE + STEP);
	
	for(int size = 0; size < MAX_SIZE; size+=STEP){
		dotC_gt0_v4nm32f(src0, &src1, dst, size);
		uassert(dst[size].v0 == INIT_DST_VALUE);
		// if(dst[size].v0 != INIT_DST_VALUE){
		// 	DEBUG_PLOG_ERROR("overflow (size=%d)\n", size);
		// 	return;
		// }
		if(size == 0) continue;
		uassert(dst[size - 1].v0 != INIT_DST_VALUE);
		// if(dst[size - 1].v0 == INIT_DST_VALUE){
		// 	DEBUG_PLOG_ERROR("underflow (size=%d)\n", size);
		// 	return;
		// }
	}
	DEBUG_TEST_OK();
}

void testValues(){
	int sizes[] = {5, 32, 40, 41, 64, 96, 1024};
	for(int i = 0 ; i < sizeof(sizes) / sizeof(int); i++){
		int size = sizes[i];
		initSrc(src0, MAX_SIZE);
		initDst(dst, MAX_SIZE + STEP);

		DEBUG_PLOG_LEVEL_1("dst ref values: {");		
		for(int i = 0; i < size; i++){
			dst_ref[i].v0 = 0;
			for(int j = 0; j < 4; j++){
				dst_ref[i].v0 += src0[i].vec[j] * src1.vec[j];
			}
			if(dst_ref[i].v0 < 0) dst_ref[i].v0 = 0;
			dst_ref[i].v1 = dst_ref[i].v0;
			DEBUG_PLOG_LEVEL_1(" {%.2f, %.2f}, ", dst_ref[i].v0, dst_ref[i].v1) ;		
		}
		DEBUG_PLOG_LEVEL_1("}\n");

		dotC_gt0_v4nm32f(src0, &src1, dst, size);

		//DEBUG_PLOG_LEVEL_2("dst values: {");		
		//for(int i = 0; i < size; i++){
			//DEBUG_PLOG_LEVEL_2(" {%.2f, %.2f}, ", dst[i].v0, dst[i].v1) ;		
		//}
		//DEBUG_PLOG_LEVEL_2("}\n");


		for(int i = 0; i < size; i++){
			uassert(dst[i] == dst_ref[i]);
			// if(dst[i] != dst_ref[i]){
			// 	DEBUG_PLOG_ERROR("{%.2f, %.2f}!={%.2f, %.2f}\n", dst[i].v0, dst[i].v1, dst_ref[i].v0, dst_ref[i].v1);
			// 	DEBUG_PLOG_LEVEL_1("size=%d, i=%d\n", size, i);
			// 	return;
			// }
		}
	}
	DEBUG_TEST_OK();

}

int main(){
	DEBUG_PLOG_FILE();
	DEBUG_PLOG_LEVEL_1("src=0x%p, srcC=0x%p, dst=0x%p\n", src0, &src1, dst);
	testValues();
	testSize();
	return 0;
}