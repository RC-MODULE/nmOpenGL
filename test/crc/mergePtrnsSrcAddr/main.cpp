#include "nmpp.h"
#include "minrep.h"
#include "time.h"
#include "stdio.h"

#define NMGL_SIZE 1024
///////////////////////////////////////////////////////////////////////////////////////
#pragma data_section ".data_imu0"
nm32s *ppSrcCA[NMGL_SIZE];
#pragma data_section ".data_imu1"
nm32s *ppSrcCB[NMGL_SIZE];
#pragma data_section ".data_imu2"
nm32s *ppSrcBA[NMGL_SIZE];
#pragma data_section ".data_imu3"
nm32s *ppDst[3*NMGL_SIZE+2];

extern "C" void mergePtrnsAddr(nm32s** ppSrcCA, nm32s** ppSrcCB, nm32s** ppSrcBA, int step, nm32s** ppDst, int count);

int main()
{
	clock_t t0,t1;
	unsigned int crc = 0;
	int	MaxLongSize=NMGL_SIZE;

	nmppsRandUniform_32u((nm32u*)ppSrcCA,NMGL_SIZE);
	nmppsRandUniform_32u((nm32u*)ppSrcCB,NMGL_SIZE);
	nmppsRandUniform_32u((nm32u*)ppSrcBA,NMGL_SIZE);
	nmppsSet_32s((nm32s*)ppDst,(int)0xCCCCCCCC,3*NMGL_SIZE+2);
	//nmppsSet_32s((nm32s*)ppSrcCA,(int)1, NMGL_SIZE);
	//nmppsSet_32s((nm32s*)ppSrcCB,(int)2, NMGL_SIZE);
	//nmppsSet_32s((nm32s*)ppSrcBA,(int)3, NMGL_SIZE);
	//int LongSize=64*128;
	for(int LongSize=2;LongSize<=NMGL_SIZE;LongSize+=2)
	{
		t0=clock();
		mergePtrnsAddr(ppSrcCA, ppSrcCB, ppSrcBA, 32, ppDst, LongSize);
		t1=clock();
		nmppsCrcAcc_32u((nm32u*)ppDst,3*LongSize + 2,&crc);
	}
	//! \fn void nmppsAnd_64u(nm64u*,nm64u*,nm64u*,int)
	for(int i=0;i< 16;i++){
		printf("ppDst[%d]=%d\n",i,ppDst[i]);
	}
	printf("crc=0x%x\n",crc);
	return t1-t0;	
	//return crc>>2;
}
