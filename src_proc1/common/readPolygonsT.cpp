#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"
#include "service.h"
#include "imagebuffer.h"
#include "nmprofiler.h"

int totalSum(nm32s* pVec, int size);

//SECTION(".text_demo3d") void readPolygonsT(DataForNmpu1* data){
SECTION(".text_demo3d") int getAddrPtrnsT(DataForNmpu1* data) {
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	PatternPack * patternPack = &cntxt->patternPack;
	PatternsArray *patternsArray = cntxt->patterns;
	Size segmentSize = cntxt->segmentSize;
	SimpleBuffer<int> *buffers = cntxt->buffers;

	getPatternsPackT(data, patternsArray, &segmentSize, patternPack, buffers);

	return 0;
}
