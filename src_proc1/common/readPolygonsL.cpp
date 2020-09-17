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
	int* dy = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* dx = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;

	int* minX = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* maxX = cntxt->buffer1 + offset1;
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

#ifdef DEBUG
	if (offset0 >= SIZE_BUFFER_NM1 || offset1 >= SIZE_BUFFER_NM1) {
		printf("error!! \n");
		printf("readPolygonsL - over size of bank\n");
		return 0;
	}
#endif // DEBUG
	nmppsMinMaxEvery_32s(dataTmp->x0, dataTmp->x1, minX, maxX, size);
	nmppsSub_32s(dataTmp->x1, dataTmp->x0, dx, size);
	nmppsSub_32s(dataTmp->y1, dataTmp->y0, dy, size);
	nmppsMulC_AddV_AddC_32s(dy, 2 * WIDTH_PTRN, dx, WIDTH_PTRN, temp0, size);
	nmppsRemap_32u((nm32u*)cntxt->patterns->lineTable, (nm32u*)temp1, temp0, size);
	nmppsMulC_32s(temp1, sizeof32(Pattern), temp2, size);
	baseAddrOffs_32s((nm32s*)cntxt->patterns->linePtrns, temp2, cntxt->ppSrcPackPtrns, size);

	nmppsCmpEq0_32u31b((nm32u31b*)dy, temp2, size, -1);
	nmppsSub_32s(dy, temp2, temp0, size);
	nmppsMulC_32s(temp0, WIDTH_PTRN / 16, cntxt->nSizePtrn32, size);

	nmppsClipCC_32s(minX, 0, cntxt->smallColorBuff.getWidth(), temp0, size);
	nmppsClipCC_32s(maxX, 0, cntxt->smallColorBuff.getWidth(), temp1, size);
	nmppsSub_32s(temp1, temp0, temp2, size);
	nmppsClipCC_32s(dataTmp->y0, 0, cntxt->smallColorBuff.getWidth(), temp1, size);
	nmppsMulC_AddV_AddC_32s(temp1, cntxt->smallColorBuff.getWidth(), temp0, 0, imageOffset, size);
	nmppsClipCC_32s(dataTmp->y1, 0, cntxt->smallColorBuff.getWidth(), temp0, size);
	nmppsSub_32s(temp0, temp1, temp3, size);
	nmppsAndC_32u((nm32u*)imageOffset, 1, (nm32u*)temp0, size);
	nmppsAndC_32u((nm32u*)imageOffset, 0xFFFFFFFE, (nm32u*)imageOffset, size);
	nmppsAdd_32s(temp0, temp2, temp1, size);
	nmppsAddC_32s(temp1, 1, temp2, size);
	nmppsAndC_32u((nm32u*)temp2, 0xFFFFFFFE, (nm32u*)temp1, size);
	nmppsMerge_32s(temp1, temp3, (nm32s*)cntxt->ptrnSizes, size);

	//nmppsCmpLt0_32s(dx, temp0, size);
	//nmppsAddC_32s(dx, WIDTH_PTRN, temp1, size);
	//nmppsAnd_32u((nm32u*)temp0, (nm32u*)temp1, (nm32u*)temp2, size);
	//absIfNegElse0(minX, temp0, size);
	//nmppsAdd_32s(temp0, temp2,  temp3, size);
	//absIfNegElse0(dataTmp->y0, temp1, size);
	//nmppsMerge_32s(temp3, temp1, (nm32s*)cntxt->ptrnInnPoints, size);
	

	nmppsConvert_32s8s(dataTmp->color, (nm8s*)cntxt->valuesC, 4 * size);
	nmppsCopy_32s(dataTmp->z, cntxt->valuesZ, size);

	for (int i = 0; i < size; i++) {
		cntxt->ppDstPackPtrns[i] = (nm32s*)cntxt->ppPtrnsCombined_2s[i % SMALL_SIZE];
		cntxt->ptrnInnPoints[i].x = (dx[i] < 0) ? WIDTH_PTRN + dx[i] : 0;

		if (minX[i] < 0) {
			cntxt->ptrnInnPoints[i].x += -minX[i];
			minX[i] = 0;
		}
		else {
			cntxt->ptrnInnPoints[i].x = cntxt->ptrnInnPoints[i].x;
		}
		int minY = dataTmp->y0[i];
		if (minY < 0) {
			cntxt->ptrnInnPoints[i].y = -minY;
			minY = 0;
		}
		else {
			cntxt->ptrnInnPoints[i].y = 0;
		}
		maxX[i] = MIN(cntxt->smallColorBuff.getWidth(), maxX[i]);
		int maxY = MIN(cntxt->smallColorBuff.getWidth(), dataTmp->y1[i]);
		//cntxt->ptrnSizes[i].width = maxX[i] - minX[i];
		//cntxt->ptrnSizes[i].height = maxY - minY;
		//if (cntxt->ptrnSizes[i].width == 0)
		//	cntxt->ptrnSizes[i].width++;
		//if (cntxt->ptrnSizes[i].height == 0)
		//	cntxt->ptrnSizes[i].height++;

		//imageOffset[i] = minY * cntxt->smallColorBuff.getWidth() + minX[i];
		int imageOffset2 = minY * cntxt->smallColorBuff.getWidth() + minX[i];
		if (imageOffset2 % 2) {
			//imageOffset[i]--;
			cntxt->ptrnInnPoints[i].x--;
			//cntxt->ptrnSizes[i].width++;
		}
		//if (cntxt->ptrnSizes[i].width % 2) {
//			cntxt->ptrnSizes[i].width++;
		//}

		//cntxt->imagePoints[i] = nmppsAddr_32s((nm32s*)cntxt->smallColorBuff.mData, imageOffset[i]);
		//cntxt->zBuffPoints[i] = nmppsAddr_32s((nm32s*)cntxt->smallDepthBuff.mData, imageOffset[i]);

		//cntxt->valuesC[i] = (data->color[4 * i + 0] & 0xFF);
		//cntxt->valuesC[i] |= (data->color[4 * i + 1] & 0xFF) << 8;
		//cntxt->valuesC[i] |= (data->color[4 * i + 2] & 0xFF) << 16;
		//cntxt->valuesC[i] |= (data->color[4 * i + 3] & 0xFF) << 24;

		//cntxt->valuesZ[i] = data->z[i];
	}
	

	//этот кусок кода является си-реализацией этой функции и является более наглядным	
	/*for (int i = 0; i < size; i++) {
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
		
		int imageOffset = minY * cntxt->smallColorBuff.getWidth() + minX;
		if (imageOffset % 2) {
			imageOffset--;
			cntxt->ptrnInnPoints[i].x--;
			cntxt->ptrnSizes[i].width++;
		}
		if (cntxt->ptrnSizes[i].width % 2) {
			cntxt->ptrnSizes[i].width++;
		}

		cntxt->imagePoints[i] = nmppsAddr_32s((nm32s*)cntxt->smallColorBuff.mData, imageOffset);
		cntxt->zBuffPoints[i] = nmppsAddr_32s((nm32s*)cntxt->smallDepthBuff.mData, imageOffset);

		cntxt->valuesC[i]  = (data->color[4 * i + 0] & 0xFF);
		cntxt->valuesC[i] |= (data->color[4 * i + 1] & 0xFF) << 8;
		cntxt->valuesC[i] |= (data->color[4 * i + 2] & 0xFF) << 16;
		cntxt->valuesC[i] |= (data->color[4 * i + 3] & 0xFF) << 24;

		cntxt->valuesZ[i] = data->z[i];
	}*/
	
	return 0;
}