#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"
#include "service.h"
#include "imagebuffer.h"


//SECTION(".text_demo3d") void readPolygonsL(DataForNmpu1* data){
SECTION(".text_demo3d") int getAddrPtrnsL(DataForNmpu1* data) {
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	int size = data->count;
	int offset0 = 0;
	int offset1 = 0;
	DataForNmpu1* dataTmp = (DataForNmpu1*)cntxt->buffer0;
	offset0 += sizeof32(DataForNmpu1);
	msdAdd(data, dataTmp, sizeof32(DataForNmpu1), 0);
	msdWaitDma(0);
	int* dx01 = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* dy01 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;

	int* minX = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* maxX = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;

	int* localX0 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;

	int* temp0 = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* temp1 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* temp2 = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* temp3 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* imageOffset = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;

	int** srcPackTmp02 = (int**)cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int** srcPackTmp01 = (int**)cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int** dstPackTmp02 = (int**)cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int** dstPackTmp01 = (int**)cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* sizePackTmp01 = (int*)cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
#ifdef DEBUG
	if (offset0 > SIZE_BANK || offset1 > SIZE_BANK) {
		printf("error!! \n");
		printf("readPolygonsT - over size of bank\n");
		return 0;
	}
#endif // DEBUG	

	//этот кусок кода является си-реализацией этой функции и является более наглядным	
	for (int i = 0; i < size; i++) {
		int dx = data->x1[i] - data->x0[i];
		int dy = data->y1[i] - data->y0[i];
		int ptrnNo, minX, maxX;
		ptrnNo = cntxt->patterns->lineTable[dy * 2 * WIDTH_PTRN + dx + WIDTH_PTRN];

		if (dx < 0) {
			minX = data->x1[i];
			maxX = data->x0[i];
		}
		else {
			minX = data->x0[i];
			maxX = data->x1[i];
		}
		cntxt->ppSrcPackPtrns[i] = (nm32s*)&cntxt->patterns->linePtrns[ptrnNo];
		cntxt->ppDstPackPtrns[i] = (nm32s*)cntxt->ppPtrnsCombined_2s[i % SMALL_SIZE];
		if (dy == 0)
			dy++;
		cntxt->nSizePtrn32[i] = dy * WIDTH_PTRN / 16;

		cntxt->ptrnInnPoints[i].x = (dx < 0) ? WIDTH_PTRN + dx : 0;

		if (minX < 0) {
			cntxt->ptrnInnPoints[i].x += -minX;
			minX = 0;
		}
		else {
			cntxt->ptrnInnPoints[i].x = cntxt->ptrnInnPoints[i].x;
			//cntxt->ptrnInnPoints[i].x = 0;
		}
		int minY = data->y0[i];
		if (minY < 0) {
			cntxt->ptrnInnPoints[i].y = -minY;
			minY = 0;
		}
		else {
			cntxt->ptrnInnPoints[i].y = 0;
		}		
		maxX = MIN(cntxt->smallColorBuff.getWidth(), maxX);
		int maxY = MIN(cntxt->smallColorBuff.getWidth(), data->y1[i]);		
		cntxt->ptrnSizes[i].width = maxX - minX;
		cntxt->ptrnSizes[i].height = maxY - minY;
		if (cntxt->ptrnSizes[i].width == 0)
			cntxt->ptrnSizes[i].width++;
		if (cntxt->ptrnSizes[i].height == 0)
			cntxt->ptrnSizes[i].height++;
		
		imageOffset[i] = minY * cntxt->smallColorBuff.getWidth() + minX;
		if (imageOffset[i] % 2) {
			imageOffset[i]--;
			cntxt->ptrnInnPoints[i].x--;
			cntxt->ptrnSizes[i].width++;
		}
		if (cntxt->ptrnSizes[i].width % 2) {
			cntxt->ptrnSizes[i].width++;
		}

		cntxt->imagePoints[i] = nmppsAddr_32s((nm32s*)cntxt->smallColorBuff.mData, imageOffset[i]);
		cntxt->zBuffPoints[i] = nmppsAddr_32s((nm32s*)cntxt->smallDepthBuff.mData, imageOffset[i]);

		cntxt->valuesC[i]  = (data->color[4 * i + 0] & 0xFF);
		cntxt->valuesC[i] |= (data->color[4 * i + 1] & 0xFF) << 8;
		cntxt->valuesC[i] |= (data->color[4 * i + 2] & 0xFF) << 16;
		cntxt->valuesC[i] |= (data->color[4 * i + 3] & 0xFF) << 24;

		cntxt->valuesZ[i] = data->z[i];
	}
	
	return 0;
}