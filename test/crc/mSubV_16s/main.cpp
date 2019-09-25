#include "nmpp.h"
#include "minrep.h"
#include "time.h"
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

int main()
{
	L0=nmppsMalloc_64s(SizeL0);
	L1=nmppsMalloc_64s(SizeL1);
	G0=nmppsMalloc_64s(SizeG0);
	G1=nmppsMalloc_64s(SizeG1);
	if ((L0==0)||(L1==0)||(G0==0)||(G1==0)) return -1;

	
	clock_t t0,t1;
	
	unsigned int crc = 0;
	int	MaxLongSize=1024;
	MaxLongSize=MIN(MaxLongSize,SizeL0);
	MaxLongSize=MIN(MaxLongSize,SizeL1);
	MaxLongSize=MIN(MaxLongSize,SizeG0);

	nmppsRandUniform_32u((nm32u*)G0,SizeL0*2);
	
	nmppsRandUniform_32u((nm32u*)L1,SizeL1*2);
	nmppsSet_32s((nm32s*)G1,(int)0xCCCCCCCC,(SizeG0*2));
	int LongSize=64*128;
	//for(int LongSize=2;LongSize<=64;LongSize+=2)
	{
		t0=clock();
		mSubV_16s((nm16s*)G0,80, (nm16s*)L1, 64, 
						(nm16s*)G1, 64,
						64, 34);
		t1=clock();
		nmppsCrcAcc_32u((nm32u*)G1,MIN(64+128,SizeG0),&crc);
	}
	//! \fn void nmppsAnd_64u(nm64u*,nm64u*,nm64u*,int)

	nmppsFree(L0);
	nmppsFree(L1);
	nmppsFree(G0);
	nmppsFree(G1);

	

	//return t1-t0;
	return crc>>2;
}
