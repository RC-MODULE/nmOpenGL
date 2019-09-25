#include "nmpp.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"

SECTION(".text_demo3d") void selectPatterns(nm32s* dydxTable, nm32s* dX, nm32s* dY, nm32s* x0, nm32s* pStartPtrns, nm32s** pAddrPack, int nSize, int* pTmp){
	
	nmppsMulC_AddV_AddC_32s(dY, 2 * WIDTH_PTRN, dX, WIDTH_PTRN, pTmp, nSize);

	nmppsRemap_32u((nm32u*)dydxTable, (nm32u*)pAddrPack, pTmp, nSize);
	nmppsAdd_32s((nm32s*)pAddrPack, x0, pTmp, nSize);

#ifdef __NM__
	nmppsMulC_AddV_AddC_32s(pTmp, WIDTH_PTRN*HEIGHT_PTRN/16, pStartPtrns, 0, (nm32s*)pAddrPack, nSize);
#else
	for(int k=0;k<nSize;k++){
		pAddrPack[k] = nmppsAddr_32s((nm32s*)pStartPtrns[k], pTmp[k] * WIDTH_PTRN*HEIGHT_PTRN/16);
	}	
#endif

	return;
}
