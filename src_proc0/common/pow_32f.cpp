#include "nmpp.h"
#include "demo3d_common.h"

SECTION(".text_demo3d")
void pow_32f(nm32f* srcVec, nm32f* dstVec, float powC,  int size, nm32f* pTmp1 ) {
	nmppsLn_32f(srcVec, dstVec, size);
	nmppsMulC_32f(dstVec, pTmp1, powC, size);
	nmppsExp_32f(pTmp1, dstVec, size);
}