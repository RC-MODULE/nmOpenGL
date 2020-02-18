#include "nmpp.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"
#include "demo3d_common.h"

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

#define SIZE 16

SECTION(".data_imu0") v4nm32f srcV[SIZE];
SECTION(".data_imu1") v4nm32f srcC = {0,100,500,1000};		
SECTION(".data_imu2") v4nm32f dst[SIZE + 2];

extern "C" void subCRev_v4nm32f(v4nm32f* pSrcV, v4nm32f* pSrcC, v4nm32f* pDst, int size);
int main()
{
	
	int count=32;

	clock_t t0,t1;
	nmppsRand_32f((float*)srcV, 4*SIZE,1,1000);
	for(int i=0;i<SIZE+2;i++){
		dst[i].vec[0] = 0;
		dst[i].vec[1] = 0;
		dst[i].vec[2] = 0;
		dst[i].vec[3] = 0;
	}
	unsigned int crc = 0;
	for(int size =0; size< SIZE;size++){
		subCRev_v4nm32f(srcV, &srcC, dst, size);
		nmppsCrcAcc_32f((float*)dst,4, 4*size+2,&crc);
	}
	
	printf("crc=0x%x\n", crc);
	
	return crc;
}
