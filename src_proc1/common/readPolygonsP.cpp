#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"
#include "service.h"
#include "imagebuffer.h"

//SECTION(".text_demo3d") void readPolygonsL(DataForNmpu1* data){
SECTION(".text_demo3d") int getAddrPtrnsP(DataForNmpu1* data) {
	/*NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	PatternPack * patternPack = &cntxt->patternPack;
	int size = data->count;
	int offset0 = 0;
	int offset1 = 0;
	DataForNmpu1* dataTmp = (DataForNmpu1*)cntxt->buffer0;
	offset0 += sizeof32(DataForNmpu1);
	nmppsCopy_32s((nm32s*)data, (nm32s*)dataTmp, sizeof32(DataForNmpu1));


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

	int** srcPackTmp = (int**)(cntxt->buffer0 + 0 * size);
	int** dstPackTmp = (int**)(cntxt->buffer0 + 1 * size);
	int* sizePackTmp = (cntxt->buffer0 + offset0 + 2 * size);
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

	nmppsSet_32s((nm32s*)srcPackTmp, (int)&cntxt->patterns->pointPtrns[pointSize - 1], size);
	nmppsSet_32s((nm32s*)sizePackTmp, pointSize * WIDTH_PTRN / 16, size);
	nmppsCopy_32s((nm32s*)cntxt->ppPtrnsCombined_2s, (int*)dstPackTmp, size);
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
	nmppsMerge_32s(temp3, temp2, (nm32s*)patternPack->origins, size);
	nmppsAdd_32s(temp4, temp0, temp1, size);
	nmppsAddC_32s(temp4, 1, temp1, size);
	nmppsAndC_32u((nm32u*)temp1, 0xFFFFFFFE, (nm32u*)temp4, size);
	nmppsMerge_32s(temp4, temp5, (nm32s*)patternPack->sizes, size);
	baseAddrOffs_32s((nm32s*)cntxt->smallColorBuff.mData, imageOffset, cntxt->imagePoints, size);
	baseAddrOffs_32s((nm32s*)cntxt->smallDepthBuff.mData, imageOffset, cntxt->zBuffPoints, size);

	nmppsConvert_32s8s(dataTmp->color, (nm8s*)cntxt->valuesC, 4 * size);
	nmppsCopy_32s(dataTmp->z, cntxt->valuesZ, size);

	copyPacket_32s(srcPackTmp,
		dstPackTmp,
		sizePackTmp, size);*/
		//этот кусок кода является си-реализацией этой функции и является более наглядным	
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	PatternPack * patternPack = &cntxt->patternPack;

	int size = data->count;
	patternPack->count = size;
	Size segmentSize = cntxt->segmentSize;
	Pattern** ppSrcPackPtrns = (Pattern**)cntxt->buffers[0].alloc(size * sizeof32(Pattern*));
	int* nSizePtrn32 = cntxt->buffers[2].alloc(size);
	for (int i = 0; i < size; i++) {
		int pointSize = cntxt->pointSize;
		int minX = data->x0[i] - pointSize / 2;
		int maxX = data->x0[i] + pointSize / 2;
		int minY = data->y0[i] - pointSize / 2;
		int maxY = data->y0[i] + pointSize / 2;
		if (pointSize % 2 == 1) {
			maxX++;
			maxY++;
		}
		ppSrcPackPtrns[i] = &cntxt->patterns->pointPtrns[pointSize - 1];
		nSizePtrn32[i] = pointSize * WIDTH_PTRN / 16;

		patternPack->origins[i].x = (minX < 0) ? -minX : 0;
		patternPack->origins[i].y = (minY < 0) ? -minY : 0;
		minX = MAX(minX, 0);
		maxX = MIN(maxX, segmentSize.width);
		minY = MAX(minY, 0);
		maxY = MIN(maxY, segmentSize.height);
		patternPack->sizes[i].width = maxX - minX;
		patternPack->sizes[i].height = maxY - minY;
		if (patternPack->sizes[i].height < 0) {
			patternPack->sizes[i].height = patternPack->sizes[i].height;
		}
		
		patternPack->imagePositions[i] = minY * segmentSize.width + minX;
		if (patternPack->imagePositions[i] % 2) {
			patternPack->imagePositions[i]--;
			patternPack->sizes[i].width++;
			patternPack->origins[i].x--;
		}
		if (patternPack->sizes[i].width % 2) {
			patternPack->sizes[i].width++;
		}
		cntxt->imagePoints[i] = (int*)cntxt->smallColorBuff.mData + patternPack->imagePositions[i];
		cntxt->zBuffPoints[i] = (int*)cntxt->smallDepthBuff.mData + patternPack->imagePositions[i];
	}
	static int counter = 0;
	if (counter == 45){
		counter = counter;
	}
	copyPacket_32s((int**)ppSrcPackPtrns, (int**)patternPack->ppPattern, nSizePtrn32, size);
	counter++;
	cntxt->buffers[0].free(size * sizeof32(Pattern*));
	cntxt->buffers[2].free(size);
	return 0;
}