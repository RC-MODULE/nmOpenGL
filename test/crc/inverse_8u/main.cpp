#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"
#include "demo3d_common.h"
#include "demo3d_nm1.h"

#define SIZE 1024
#define SMALL_SIZE 64

SECTION(".data_imu1") int src[SIZE];
SECTION(".data_imu3") int dst[SIZE];

extern "C" void inverse_8u(const nm8u *srcArray,  nm8u *dstArray, int count);

int main()
{
	clock_t t0, t1;
	unsigned crc = 0;
	nmppsRandUniform_32s(src, SIZE);
	//nmppsRandUniform_32s(offsets, SIZE);
	//nmppsAndC_32u((nm32u*)offsets, 0x3FF, (nm32u*)offsets, SMALL_SIZE);
	for(int i = 0; i < SIZE + 2; i++){
		src[i] = 0x010F5100;
	}
	for(int i = 0; i < SIZE + 2; i++){
		dst[i] = 0xCDCDCDCD;
	}
	t0 = clock();
	inverse_8u((nm8u*)src, (nm8u*)dst, SIZE);
	t1 = clock();
	
	for(int i = 0; i < 16; i++){
		printf("0x%x\n", dst[i]);
	}
	//nmppsCrcAcc_32s(dst, SMALL_SIZE + 2, &crc);
	return t1-t0;
}
