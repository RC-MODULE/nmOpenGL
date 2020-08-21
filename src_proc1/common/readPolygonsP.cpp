#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"
#include "service.h"
#include "imagebuffer.h"

int getAddrPtrnsP(PolygonsOld* data) {

}

//SECTION(".text_demo3d") void readPolygonsL(DataForNmpu1* data){
SECTION(".text_demo3d") int getAddrPtrnsP(DataForNmpu1* data) {
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	int size = data->count;
	int offset0 = 0;
	int offset1 = 0;
	DataForNmpu1* dataTmp = (DataForNmpu1*)cntxt->buffer0;
	offset0 += sizeof32(DataForNmpu1);
	msdAdd(data, dataTmp, sizeof32(DataForNmpu1), 0);
	msdWaitDma(0);
	
#ifdef DEBUG
	if (offset0 > SIZE_BANK || offset1 > SIZE_BANK) {
		printf("error!! \n");
		printf("readPolygonsT - over size of bank\n");
		return 0;
	}
#endif // DEBUG	

	//этот кусок кода является си-реализацией этой функции и является более наглядным	
	for (int i = 0; i < size; i++) {
		int pointSize = cntxt->pointSize;
		int minX = data->x0[i] - pointSize / 2;
		int maxX = data->x0[i] + pointSize / 2;
		int minY = data->y0[i] - pointSize / 2;
		int maxY = data->y0[i] + pointSize / 2;
		cntxt->ppSrcPackPtrns[i] = cntxt->patterns->pointPtrns[pointSize - 1];
		cntxt->ppDstPackPtrns[i] = (nm32s*)cntxt->ppPtrnsCombined_2s[i % SMALL_SIZE];
		cntxt->nSizePtrn32[i] = pointSize * WIDTH_PTRN / 16;
		cntxt->ptrnInnPoints[i].x = (minX < 0) ? -minX : 0;
		cntxt->ptrnInnPoints[i].y = (minY < 0) ? -minY : 0;
		minX = MAX(minX, 0);
		maxX = MIN(maxX, cntxt->smallColorBuff.getWidth());
		minY = MAX(minY, 0);
		maxY = MIN(maxY, cntxt->smallColorBuff.getHeight());
		cntxt->ptrnSizes[i].width = maxX - minX;
		cntxt->ptrnSizes[i].height = maxY - minY;
		int imageOffset = minY * cntxt->smallColorBuff.getWidth() + minX;
		if (imageOffset % 2) {
			imageOffset--;
			cntxt->ptrnSizes[i].width++;
			cntxt->ptrnInnPoints[i].x--;
		}
		if (cntxt->ptrnSizes[i].width % 2) {
			cntxt->ptrnSizes[i].width++;
		}
		cntxt->imagePoints[i] = nmppsAddr_32s((nm32s*)cntxt->smallColorBuff.mData, imageOffset);
		cntxt->zBuffPoints[i] = nmppsAddr_32s((nm32s*)cntxt->smallDepthBuff.mData, imageOffset);
	}
	nmppsConvert_32s8s(dataTmp->color, (nm8s*)cntxt->valuesC, 4 * size);
	nmppsCopy_32s(dataTmp->z, cntxt->valuesZ, size);
	
	
	return 0;
}