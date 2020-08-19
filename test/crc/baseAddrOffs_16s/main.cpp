#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"
#include "demo3d_common.h"
#include "demo3d_nm1.h"

///////////////////////////////////////////////////////////////////////////////////////
nm64s *L0;
nm64s *L1;
nm64s *G0;
nm64s *G1;
const int KB=1024/8;
const int SizeL0=30*KB;
const int SizeL1=30*KB;

const int SizeG0=30*KB;
const int SizeG1=30*KB;
long tmp;

#define SIZE 1024
#define SMALL_SIZE 64

SECTION(".data_imu1") int src[SIZE];
SECTION(".data_imu2") int offsets[SMALL_SIZE];
SECTION(".data_imu3") int* dst[SMALL_SIZE + 2];

#define SIZE_ONE 11*32

int main()
{
	unsigned crc = 0;
	nmppsRandUniform_32s(src, SIZE);
	for(int i=0;i<SIZE;i++){
		src[i] = i;
	}
	//nmppsRandUniform_32s(offsets, SIZE);
	//nmppsAndC_32u((nm32u*)offsets, 0x3FF, (nm32u*)offsets, SMALL_SIZE);
	for(int i=0;i<SMALL_SIZE;i++){
		offsets[i] = 2 * i;
	}
	for(int i=0;i<SMALL_SIZE + 2;i++){
		dst[i] = (int*)0xCDCDCDCD;
	}
	baseAddrOffs_16s((nm16s*)src, offsets, (nm16s**)dst, SMALL_SIZE);
	for(int i = 0; i < 16; i++){
		printf("0x%x\n", dst[i][0]);
	}
	//nmppsCrcAcc_32s(dst, SMALL_SIZE + 2, &crc);
	return crc;
}
