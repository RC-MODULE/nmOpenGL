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
long tmp;

#pragma data_section ".data_imu0"	

#pragma data_section ".data_imu1"
	nm64s dump;
	int image[16384];
		
#pragma data_section ".data_imu2"
	nm64s dump2;
	int srcTr[16384];
	int pHeights[32];
	nm16s* pROI[32];
	int pWidths[32];
	
#pragma data_section ".data_imu3"
	nm64s dump3;
	int dstTr[16384 +2];

#define SIZE_ONE 11*32

extern "C" void mSub_Mask_VxN_16s(nm16s** pROI, int imageStride, nm16s* pTriangles, nm16s* dst,  int* pTriangsHeight, int* pTriangsWidth, int count);

int main()
{
	
	int count=32;

	clock_t t0,t1;

	unsigned int crc = 0;
	unsigned int crc1 = 0;
	for(int i=0;i<count;i++){
		pHeights[i] = 32;
		pWidths[i] = 32;		
	}
	pROI[0] = (nm16s*)image;
	pROI[1] = nmppsAddr_16s((nm16s*)image, 80);
	//nmppsRandUniform_32s(image,16384);
	nmppsSet_32s((nm32s*)image, 0x33338888, 16384);
	//nmppsRandUniform_32s(srcTr,16384);
	nmppsSet_32s((nm32s*)srcTr, 0x40001234, 16384);
	nmppsSet_32s((nm32s*)dstTr, 0xCCCCCCCC, 16384+2);
	printf("image\n");
	//for(int i=0;i<= count;i++)
	int i = 32;
	{
		for(int k=0;k<count;k++){
			pHeights[k] = 4;
			pWidths[k] = 16;		
		}
		//printf("i=%d\n",i);
		t0=clock();
		mSub_Mask_VxN_16s((nm16s**)pROI,16,(nm16s*)srcTr,(nm16s*)dstTr,pHeights,pWidths,2);
		t1=clock();
		nmppsCrcAcc_32u((nm32u*)dstTr,(i*32*i)/2+2,&crc);
		nmppsCrcAcc_32u((nm32u*)image,16384,&crc);
		printf("count=%2d, crc=0x%x\n", i, crc);
	}
	
	for(int i=0;i < 16; i++){
		for(int j=0;j<8;j++){
			printf("0x%x ",image[i*8+j]);
		}
		printf("\n");
	}
	return crc;
}
