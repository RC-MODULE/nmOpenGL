#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"
#include "service.h"
#include "imagebuffer.h"

int getAddrPtrnsP(PolygonsOld* data) {
	data->count = 0;
	return 0;
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


	int* minX = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* maxX = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* minY = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* maxY = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* temp0 = cntxt->buffer0 + offset0;
	offset0 += 2 * POLYGONS_SIZE;
	int* temp1 = cntxt->buffer1 + offset1;
	offset1 += 2 * POLYGONS_SIZE;
	int* temp2 = cntxt->buffer0 + offset0;
	offset0 += 2 * POLYGONS_SIZE;
	int* temp3 = cntxt->buffer1 + offset1;
	offset1 += 2 * POLYGONS_SIZE;
	int* temp4 = cntxt->buffer0 + offset0;
	offset0 += 2 * POLYGONS_SIZE;
	int* temp5 = cntxt->buffer1 + offset1;
	offset1 += 2 * POLYGONS_SIZE;
	int* imageOffset = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	msdWaitDma(0);

	
#ifdef DEBUG
	if (offset0 > SIZE_BUFFER_NM1 || offset1 > SIZE_BUFFER_NM1) {
		printf("error!! \n");
		printf("readPolygonsT - over size of bank\n");
		return 0;
	}
#endif // DEBUG	
	int pointSize = cntxt->pointSize;
	nmppsSubC_32s(dataTmp->x0, pointSize / 2, minX, size);
	nmppsAddC_32s(dataTmp->x0, pointSize / 2, maxX, size);
	nmppsSubC_32s(dataTmp->y0, pointSize / 2, minY, size);
	nmppsAddC_32s(dataTmp->y0, pointSize / 2, maxY, size);

	nmppsSet_32s((nm32s*)cntxt->ppSrcPackPtrns, (int)&cntxt->patterns->pointPtrns[pointSize - 1], size);
	nmppsSet_32s((nm32s*)cntxt->nSizePtrn32, pointSize * WIDTH_PTRN / 16, size);
	int height = size / SMALL_SIZE;
	height = (size % SMALL_SIZE) ? height + 1 : height;
	nmppmCopy_32s((nm32s*)cntxt->ppPtrnsCombined_2s, 0, (nm32s*)cntxt->ppDstPackPtrns, SMALL_SIZE, height, SMALL_SIZE);
	int imageWidth = cntxt->smallColorBuff.getWidth();
	int imageHeight = cntxt->smallColorBuff.getHeight();
	nmppsClipCC_32s(minX, 0, imageWidth, temp0, size);
	nmppsClipCC_32s(minY, 0, imageHeight, temp1, size);
	nmppsMulC_AddV_AddC_32s(temp1, imageWidth, temp0, 0, imageOffset, size);
	if (pointSize != 1) {		
		nmppsClipCC_32s(maxX, 0, imageWidth, temp2, size);
		nmppsClipCC_32s(maxY, 0, imageHeight, temp3, size);
		nmppsSub_32s(temp2, temp0, temp4, size);		//width
		nmppsSub_32s(temp3, temp1, temp5, size);		//height
	}
	else {
		nmppsSet_32s(temp4, 1, size);		  //width
		nmppsSet_32s(temp5, 1, size);		  //height
	}
	nmppsAndC_32u((nm32u*)imageOffset, 1, (nm32u*)temp0, size);
	nmppsAndC_32u((nm32u*)imageOffset, 0xFFFFFFFE, (nm32u*)imageOffset, size);
	absIfNegElse0(minX, temp1, size);
	absIfNegElse0(minY, temp2, size);
	nmppsSub_32s(temp1, temp0, temp3, size);
	nmppsMerge_32s(temp3, temp2, (nm32s*)cntxt->ptrnInnPoints, size);
	nmppsAdd_32s(temp4, temp0, temp1, size);
	nmppsAddC_32s(temp4, 1, temp1, size);
	nmppsAndC_32u((nm32u*)temp1, 0xFFFFFFFE, (nm32u*)temp4, size);
	nmppsMerge_32s(temp4, temp5, (nm32s*)cntxt->ptrnSizes, size);
	baseAddrOffs_32s((nm32s*)cntxt->smallColorBuff.mData, imageOffset, cntxt->imagePoints, size);
	baseAddrOffs_32s((nm32s*)cntxt->smallDepthBuff.mData, imageOffset, cntxt->zBuffPoints, size);

	nmppsConvert_32s8s(dataTmp->color, (nm8s*)cntxt->valuesC, 4 * size);
	nmppsCopy_32s(dataTmp->z, cntxt->valuesZ, size);

	//���� ����� ���� �������� ��-����������� ���� ������� � �������� ����� ���������	
	/*for (int i = 0; i < size; i++) {
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
	}*/
	
	
	return 0;
}