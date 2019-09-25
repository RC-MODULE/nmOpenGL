#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"

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
extern "C" long tmp;

#pragma data_section ".data_imu0"

#pragma data_section ".data_imu1"
	int src[16384];
		
#pragma data_section ".data_imu2"
	int* ppSrc[32];
	int* ppDst[32];
	int sizes[32];
	
#pragma data_section ".data_imu3"
	int dst[16384];

#define SIZE_ONE 32*32

extern "C" void copyPacket_32s(nm32s** pSrc, nm32s** pDst, int* size, int count);

int main()
{		
	unsigned int crc = 0;
	clock_t t0, t1;
	nmppsRandUniform_32s(src,16384);
	nmppsSet_32s(dst,0xCDCDCDCD,16384);
	for(int i=0;i<32;i++){
		ppSrc[i] = src;
		ppDst[i] = dst;
		sizes[i] = 31;
	}
	copyPacket_32s(ppSrc, ppDst, sizes, 3);
	nmppsCrcAcc_32s(dst,16384,&crc);
	return crc>>2;
}
