#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"

#define SIZE 1024
#define MAX_STEP 4

SECTION(".data_imu2") float srcAxy[2 *SIZE+2];
SECTION(".data_imu3") float srcBxy[2 * SIZE+2];
SECTION(".data_imu4") float srcCxy[2 * SIZE+2];
SECTION(".data_imu5") int temp[2 * SIZE + 2];
	
	
extern "C" void sortByY3(float* srcAxy, 
						 float* srcBxy, 
						 float* srcCxy, 
						 int size);

int main()
{
	clock_t t0,t1;
	nmppsRand_32f(srcAxy, 2 * SIZE, 1, 5000);
	nmppsRand_32f(srcBxy, 2 * SIZE, 5000, 10000);
	nmppsRand_32f(srcCxy, 2 * SIZE, 3000, 7000);
/*	printf("srcAxy=%f,%f\n", srcAxy[0], srcAxy[1]);
	printf("srcBxy=%f,%f\n", srcBxy[0], srcBxy[1]);
	printf("srcCxy=%f,%f\n", srcCxy[0], srcCxy[1]);
	nmppsConvert_32f32s_rounding(srcAxy, temp, 0, 2 * SIZE);
	nmppsConvert_32s32f(temp, srcAxy, 2 * SIZE);
	nmppsConvert_32f32s_rounding(srcAxy, temp, 0, 2 * SIZE);
	nmppsConvert_32s32f(temp, srcBxy, 2 * SIZE);
	nmppsConvert_32f32s_rounding(srcAxy, temp, 0, 2 * SIZE);
	nmppsConvert_32s32f(temp, srcCxy, 2 * SIZE);*/
	/*for(int i=0;i<36;i++){
		printf("y_a,b,c[%d]=%8.2f, ", i, srcAxy[2*i+1]);
		printf("%8.2f, ", srcBxy[2*i+1]);
		printf("%8.2f\n", srcCxy[2*i+1]);
	}*/
	printf("\n");
	unsigned crc = 0;
	//for (int size = 0; size < SIZE; size++) {
		int size = 36;
		t0 = clock();
		sortByY3(srcCxy, srcBxy, srcAxy, size);
		nmppsCrcAcc_32f(srcAxy, 4, size + 2, &crc);
		nmppsCrcAcc_32f(srcBxy, 4, size + 2, &crc);
		nmppsCrcAcc_32f(srcCxy, 4, size + 2, &crc);
		t1 = clock();
	//}
	
	for(int i=0;i<36;i++){
		printf("y_a,b,c[%d]=%8.2f, ", i, srcAxy[2*i+1]);
		printf("%8.2f, ", srcBxy[2*i+1]);
		printf("%8.2f\n", srcCxy[2*i+1]);
	}
	printf("crc=0x%x\n", crc);
	return crc>>2;
}
