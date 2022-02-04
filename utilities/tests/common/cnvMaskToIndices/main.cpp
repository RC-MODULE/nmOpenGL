#include "utility.h"
#include "stdio.h"
#include "section-hal.h"

#define MAX_SIZE 1024
#define STEP 2


INSECTION(".data_imu0") int src[MAX_SIZE];
INSECTION(".data_imu1") int dst[MAX_SIZE + STEP];


void testValues(){
	src[0] = 0x8000f301;
	int ref_values[8] = {0, 8, 9, 12, 13, 14, 15, 31};
	int count = cnvMaskToIndices(src, dst, 32);
	if(count != 8){
		DEBUG_PLOG_ERROR("count=%d (must be 8)\n", count);
	}

	for(int i = 0; i < 8; i++){
		if(dst[i] != ref_values[i]){
			DEBUG_PLOG_ERROR("unexpected value %d (must be %d)\n", dst[i], ref_values[i]);
			return;
		}
	}
	DEBUG_PLOG_LEVEL_0("Test values OK\n");
}

int main(){
	testValues();
	return 0;
}