#include "utility_float.h"
#include "debugprint.h"
#include "extnmtype.h"
#include "malloc32.h"
#include "section-hal.h"

#define MAX_SIZE 1024
#define STEP 2
#define INIT_DST_VALUE -1


INSECTION(".data_imu0") v2nm32f src[MAX_SIZE];
INSECTION(".data_imu1") v4nm32f mulC = {2,3,4,5};
INSECTION(".data_imu1") v4nm32f add[MAX_SIZE];

INSECTION(".data_imu2") v4nm32f dst[MAX_SIZE + STEP];
INSECTION(".data_imu3") v4nm32f dst_ref[MAX_SIZE + STEP];

void initSrc(v2nm32f *src, int size){
	for(int i = 0; i < MAX_SIZE; i++){
		src[i].v0 = i;
		src[i].v1 = i;
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
		dotMulC_Add_v4nm32f(src, &mulC, add, dst, size);
		if(dst[size] != init_dst_ref ){
			DEBUG_PLOG_ERROR("overflow (size=%d)\n", size);
			DEBUG_PLOG_LEVEL_1("size=%d\n", size);
			return;
		}
		if(size == 0) continue;
		if(dst[size - 1] == init_dst_ref ){
			DEBUG_PLOG_ERROR("underflow (size=%d)\n", size);
			DEBUG_PLOG_LEVEL_1("size=%d\n", size);
			return;
		}
	}
	DEBUG_PLOG_LEVEL_0("Test size OK\n");
}

void testValues(){
	// int sizes[] = {5, 32, 40, 41, 64, 96, 1024};
	// for(int i = 0 ; i < sizeof(sizes) / sizeof(int); i++){
	// 	int size = sizes[i];
	// 	DEBUG_PLOG_LEVEL_1("size=%d\n", size);		
	// 	for(int i = 0; i < size; i++){
			
	// 	}
	// 	initDst(dst, MAX_SIZE + STEP);

	// 	DEBUG_PLOG_LEVEL_1("dst ref values: {");		
	// 	for(int i = 0; i < size; i++){
	// 		for(int j = 0; j < 4; j++){
	// 			dst_ref[i].vec[i] = src[i].v0 * mulC.vec[j] + add[i].vec[i];
	// 		}
	// 		DEBUG_PLOG_LEVEL_1(" {%.2f, %.2f, %.2f, %.2f}, ", dst_ref[i].vec[0], dst_ref[i].vec[1], dst_ref[i].vec[2], dst_ref[i].vec[3]) ;		
	// 	}
	// 	DEBUG_PLOG_LEVEL_1("}\n");

	// 	dotMulC_Add_v4nm32f(src, &mulC, add, dst, size);

	// 	for(int i = 0; i < size; i++){
	// 		for(int j = 0; j< 4; )
	// 		if(dst[i] != dst_ref[i]){
	// 			DEBUG_PLOG_ERROR("{%.2f, %.2f, %.2f, %.2f}!={%.2f, %.2f, %.2f, %.2f }\n", 
	// 					dst[i].vec[0], dst[i].vec[1], dst[i].vec[2], dst[i].vec[3], 
	// 					dst_ref[i].vec[0], dst_ref[i].vec[1], dst_ref[i].vec[2], dst_ref[i].vec[3] );
	// 			DEBUG_PLOG_LEVEL_1("size=%d, i=%d\n", size, i);
	// 			return;
	// 		}
	// 	}
	// }
	DEBUG_PLOG_LEVEL_0("Test values OK\n");

}

int main(){
	DEBUG_PLOG_FILE();
	DEBUG_PLOG_LEVEL_1("src=0x%p, srcC=0x%p, add=%p, dst=0x%p\n", src, &mulC, add, dst);
	testValues();
	testSize();
	return 0;
}