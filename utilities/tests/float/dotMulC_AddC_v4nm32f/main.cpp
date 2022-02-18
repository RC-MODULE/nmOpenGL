#include "utility_float.h"
#include "debugprint.h"
#include "extnmtype.h"
#include "malloc32.h"
#include "section-hal.h"
#include "uassert.h"
#include <math.h>

#define MAX_SIZE 1024
#define STEP 2
#define INIT_DST_VALUE -1


INSECTION(".data_imu0") v2nm32f src[MAX_SIZE];
INSECTION(".data_imu1") v4nm32f mulC = {2,3,4,5};
INSECTION(".data_imu1") v4nm32f addC = {10,12,13,14};

INSECTION(".data_imu2") v4nm32f dst[MAX_SIZE + STEP];
INSECTION(".data_imu3") v4nm32f excepted[MAX_SIZE + STEP];

void initSrc(v2nm32f *src, int size){
	for(int i = 0; i < size; i++){
		src[i] = { (float)i - size / 2, (float)i - size / 2};
	}
}
void initDst(v4nm32f *dst, int size){
	for(int i = 0; i < MAX_SIZE + STEP; i++){
		dst[i] = { INIT_DST_VALUE, INIT_DST_VALUE, INIT_DST_VALUE, INIT_DST_VALUE };
	}
}


void testSize(){
	initSrc(src, MAX_SIZE);
	initDst(dst, MAX_SIZE + STEP);
	
	v4nm32f init_dst_ref = {INIT_DST_VALUE, INIT_DST_VALUE, INIT_DST_VALUE, INIT_DST_VALUE} ;
	for(int size = 0; size < MAX_SIZE; size+=STEP){
		dotMulC_AddC_v4nm32f(src, &mulC, &addC, dst, size);

		uassert(dst[size] == init_dst_ref);

		if(size == 0) continue;

		uassert(dst[size - 1] != init_dst_ref);
	}
	DEBUG_PLOG_LEVEL_0("Test size OK\n");
}

void testValues(){
	initSrc(src, MAX_SIZE);
	int sizes[] = {5, 32, 40, 41, 64, 96, 1024};
	for(int i = 0 ; i < sizeof(sizes) / sizeof(int); i++){
		int size = sizes[i];

		for(int i = 0; i < size; i++){
			DEBUG_PLOG_LEVEL_1("src[%3d]: { %f, %f }\n", i, src[i].v0, src[i].v1);
			uassert(src[i].v0 == src[i].v1);
		}

		DEBUG_PLOG_LEVEL_1("size=%d\n", size);		
		initDst(dst, MAX_SIZE + STEP);

		DEBUG_PLOG_LEVEL_1("excepted values: {");		
		for(int i = 0; i < size; i++){
			for(int j = 0; j < 4; j++){
				excepted[i].vec[j] = src[i].v0 * mulC.vec[j] + addC.vec[j];
			}
			DEBUG_PLOG_LEVEL_1(" {%.2f, %.2f, %.2f, %.2f}, ", 	excepted[i].vec[0], 
																excepted[i].vec[1], 
																excepted[i].vec[2], 
																excepted[i].vec[3]) ;		
		}
		DEBUG_PLOG_LEVEL_1("}\n");

		dotMulC_AddC_v4nm32f(src, &mulC, &addC, dst, size);

		for(int i = 0; i < size; i++){
			DEBUG_PLOG_LEVEL_1("src: {%.2f, %.2f}, srcC{%.2f, %.2f, %.2f, %.2f }, addC={%.2f, %.2f, %.2f, %.2f }\n", 
					src[i].v0, src[i].v1,
					mulC.vec[0], mulC.vec[1], mulC.vec[2], mulC.vec[3],
					addC.vec[0], addC.vec[1], addC.vec[2], addC.vec[3]);
			DEBUG_PLOG_LEVEL_1("dst: {%.2f, %.2f, %.2f, %.2f}, excepted{%.2f, %.2f, %.2f, %.2f }\n", 
					dst[i].vec[0], dst[i].vec[1], dst[i].vec[2], dst[i].vec[3], 
					excepted[i].vec[0], excepted[i].vec[1], excepted[i].vec[2], excepted[i].vec[3] );
			DEBUG_PLOG_LEVEL_1("size=%d, i=%d\n", size, i);
			for(int j = 0; j < 4; j++){
				uassert(fabs(dst[i].vec[j] - excepted[i].vec[j]) < 0.01);
			}
			
			// if(dst[i] != dst_ref[i]){
			// 	DEBUG_PLOG_ERROR("{%.2f, %.2f, %.2f, %.2f}!={%.2f, %.2f, %.2f, %.2f }\n", 
			// 			dst[i].vec[0], dst[i].vec[1], dst[i].vec[2], dst[i].vec[3], 
			// 			dst_ref[i].vec[0], dst_ref[i].vec[1], dst_ref[i].vec[2], dst_ref[i].vec[3] );
			// 	DEBUG_PLOG_LEVEL_1("size=%d, i=%d\n", size, i);
			// 	return;
			// }
		}
	}
	DEBUG_PLOG_LEVEL_0("Test values OK\n");

}

int main(){
	DEBUG_PLOG_FILE();
	DEBUG_PLOG_LEVEL_1("src=0x%p, srcC=0x%p, add=%p, dst=0x%p\n", src, &mulC, &addC, dst);
	testValues();
	testSize();
	return 0;
}