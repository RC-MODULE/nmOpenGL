
#include "nmpp.h"


void remap_32u(nm32u*	pSrcVec,		
	nm32u*	pDstVec,
	nm32s*			pRemapTable,
	int				nDstVecSize
	)
{
	int t;
	for(int i=0;i<nDstVecSize;i++)
	{
		t=pSrcVec[pRemapTable[i]];
		pDstVec[i]=t;
	}
	
}
 
