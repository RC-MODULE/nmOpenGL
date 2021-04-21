#include "nmpp.h"
#include "hal.h"
#include "stdio.h"
#include "time.h"
#include "section-hal.h"
//#include "demo3d_nm1.h"

///////////////////////////////////////////////////////////////////////////////////////
#define SIZE 8192
#define COUNT 32

INSECTION(".data_imu1")	int src[SIZE];
		
INSECTION(".data_imu2") int* ppSrc[COUNT];
INSECTION(".data_imu2") int* ppDst[COUNT];
INSECTION(".data_imu2") int sizes[COUNT];
	
INSECTION(".data_imu3")	int dst[SIZE];

extern "C" void copy_vec_test(void* src, void* dst, int size, int count);
extern "C" void copy_risc_test(void* src, void* dst, int size, int count);

int main()
{		
	unsigned int crc = 0;
	clock_t t0, t1;
	nmppsRandUniform_32s(src,SIZE);
	nmppsSet_32s(dst,0xCDCDCDCD,SIZE);
	for(int i=0;i<COUNT;i++){
		ppSrc[i] = src;
		ppDst[i] = dst;
		sizes[i] = 0;
	}

	// t0 = clock();
	// nmppsCopy_32s(src, dst, SIZE);
	// t1 = clock();
	// printf("nmppsCopy=%d\n", (int)(t1-t0));
	// t0 = clock();
	// halCopyRISC(src, dst, SIZE);
	// t1 = clock();
	// printf("halCopyRISC=%d\n", (int)(t1-t0));

	for(int i=0; i <= 32; i++){
		t0 = clock();
		copy_vec_test(src, dst, i, 1);
		t1 = clock();
		printf("vec[%d]=%d\n", i, (int)(t1-t0));

		t0 = clock();
		copy_risc_test(src, dst, i, 1);
		t1 = clock();
		printf("risc[%d]=%d\n\n", i, (int)(t1-t0));
	}
	
	nmppsCrcAcc_32s(dst,SIZE,&crc);
	//return crc>>2;
	return t1-t0;
}
