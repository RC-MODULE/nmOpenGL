#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"
#include "service.h"
#include "imagebuffer.h"
#include "nmprofiler.h"

SECTION(".data_demo3d") static volatile int copyId[10];

//SECTION(".text_demo3d") void readPolygonsT(DataForNmpu1* data){
SECTION(".text_demo3d") int getAddrPtrnsT(DataForNmpu1* data) {
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	int size = data->count;
	int offset0 = 0;
	int offset1 = 0;
	int offset2 = 0;
	
	int imageWidth = cntxt->smallColorBuff.getWidth();
	int imageHeight = cntxt->smallColorBuff.getHeight();

	DataForNmpu1* dataTmp = (DataForNmpu1*)cntxt->buffer0;
	offset0 += sizeof32(DataForNmpu1);
	int* localTable = (int*)cntxt->buffer1 + offset1;
	offset1 += sizeof32(cntxt->patterns->table_dydx);

	msdAdd(data, dataTmp, sizeof32(DataForNmpu1), 0);
	msdAdd(data->z, cntxt->valuesZ, sizeof32(cntxt->patterns->table_dydx), 0);
	nmppsCopy_32s(cntxt->patterns->table_dydx, localTable, sizeof32(cntxt->patterns->table_dydx));
	

	int* dx02 = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* dx01 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* dx12 = cntxt->buffer2 + offset2;
	offset2 += POLYGONS_SIZE;
	int* dy02 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* dy01 = cntxt->buffer2 + offset2;
	offset2 += POLYGONS_SIZE;
	int* dy12 = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;

	int* minX = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* maxX = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;

	int* localX0 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* localX1 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;

	int* temp0 = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* temp1 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* temp2 = cntxt->buffer2 + offset2;
	offset2 += POLYGONS_SIZE;
	int* temp3 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* imageOffset = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* alignDistance = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;

	int** srcPackTmp02 = (int**)cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int** srcPackTmp01 = (int**)cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int** srcPackTmp12 = (int**)cntxt->buffer2 + offset2;
	offset2 += POLYGONS_SIZE;
	int** dstPackTmp02 = (int**)cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int** dstPackTmp01 = (int**)cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int** dstPackTmp12 = (int**)cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* sizePackTmp02 = (int*)cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* sizePackTmp01 = (int*)cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* sizePackTmp12 = (int*)cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
#ifdef DEBUG
	if (offset0 > SIZE_BUFFER_NM1 || offset1 > SIZE_BUFFER_NM1 || offset2 > SIZE_BUFFER_NM1) {
		printf("error!! \n");
		printf("readPolygonsT - over size of bank\n");
		data->count = 0;
		return 0;
	}
#endif // DEBUG	

#ifdef __GNUC__
	int height = (size + (SMALL_SIZE - 1)) / SMALL_SIZE;	//высота с учетом неполных строк
	
	nmppmCopy_32s((nm32s*)cntxt->ppPtrns1_2s, 0, (nm32s*)dstPackTmp02, SMALL_SIZE, height, SMALL_SIZE);
	nmppmCopy_32s((nm32s*)cntxt->ppPtrns2_2s, 0, (nm32s*)dstPackTmp01, SMALL_SIZE, height, SMALL_SIZE);
	nmppmCopy_32s((nm32s*)cntxt->ppPtrns2_2s, 0, (nm32s*)dstPackTmp12, SMALL_SIZE, height, SMALL_SIZE);	 
	 
#else 
	for (int i = 0; i < size; i++) {
		dstPackTmp02[i] = (nm32s*)cntxt->ppPtrns1_2s[i % SMALL_SIZE];
		dstPackTmp01[i] = (nm32s*)cntxt->ppPtrns2_2s[i % SMALL_SIZE];
		dstPackTmp12[i] = (nm32s*)cntxt->ppPtrns2_2s[i % SMALL_SIZE];
	}
#endif
	msdWaitDma(0);

	nmppsSub_32s(dataTmp->x1, dataTmp->x0, dx01, size);
	nmppsSub_32s(dataTmp->x2, dataTmp->x0, dx02, size);	
	nmppsSub_32s(dataTmp->x2, dataTmp->x1, dx12, size);	 
	nmppsMinMaxEvery_32s(dataTmp->x0, dataTmp->x1, temp0, temp1, size);
	nmppsMinEvery_32s(temp0, dataTmp->x2, minX, size);
	nmppsMaxEvery_32s(temp1, dataTmp->x2, maxX, size);
	nmppsSub_32s(dataTmp->x0, minX, localX0, size);
	nmppsSub_32s(dataTmp->x1, minX, localX1, size);
	
	nmppsSub_32s(dataTmp->y2, dataTmp->y0, dy02, size);	 
	nmppsSub_32s(dataTmp->y1, dataTmp->y0, dy01, size);	 
	nmppsSub_32s(dataTmp->y2, dataTmp->y1, dy12, size);
	// get ptrn sizes of int	 
	nmppsMulC_32s(dy02, WIDTH_PTRN / 16, sizePackTmp02, size);
	nmppsMulC_32s(dy12, WIDTH_PTRN / 16, sizePackTmp12, size);
	nmppsMulC_32s(dy01, WIDTH_PTRN / 16, sizePackTmp01, size);
	

	// get ptrnNumbers	 
	selectPaintSide(dataTmp->crossProducts, 0, NPATTERNS / 2, temp3, size);	 
	nmppsMulC_AddV_AddC_32s(dy02, WIDTH_TABLE, dx02, WIDTH_TABLE / 2, temp2, size);
	nmppsRemap_32u((nm32u*)localTable, (nm32u*)temp0, temp2, size);
	nmppsAdd_32s(temp0, localX0, temp2, size);
	nmppsAdd_32s(temp2, temp3, temp0, size);	 
	nmppsMulC_32s(temp0, sizeof32(Pattern), temp1, size);
	baseAddrOffs_32s((nm32s*)cntxt->patterns->ptrns, temp1, srcPackTmp02, size);

	selectPaintSide(dataTmp->crossProducts, NPATTERNS / 2, 0, temp3, size);	 
	nmppsMulC_AddV_AddC_32s(dy01, WIDTH_TABLE, dx01, WIDTH_TABLE / 2, temp0, size);
	nmppsRemap_32u((nm32u*)localTable, (nm32u*)temp2, temp0, size);	 
	nmppsAdd_32s(temp2, localX0, temp0, size);
	nmppsAdd_32s(temp0, temp3, temp2, size);
	nmppsMulC_32s(temp2, sizeof32(Pattern), temp0, size);
	baseAddrOffs_32s((nm32s*)cntxt->patterns->ptrns, temp0, srcPackTmp01, size);

	nmppsMulC_AddV_AddC_32s(dy12, WIDTH_TABLE, dx12, WIDTH_TABLE / 2, temp1, size);
	nmppsRemap_32u((nm32u*)localTable, (nm32u*)temp0, temp1, size);
	nmppsAdd_32s(temp0, localX1, temp2, size);
	nmppsAdd_32s(temp2, temp3, temp0, size);
	nmppsMulC_32s(temp0, sizeof32(Pattern), temp1, size);
	baseAddrOffs_32s((nm32s*)cntxt->patterns->ptrns, temp1, srcPackTmp12, size);



	// get imageOffset	 
	nmppsClipCC_32s(minX, 0, imageWidth, temp0, size);	 
	nmppsClipCC_32s(dataTmp->y0, 0, imageHeight, temp1, size);	 
	nmppsMulC_AddV_AddC_32s(temp1, imageWidth, temp0, 0, imageOffset, size);	 
	nmppsAndC_32u((nm32u*)imageOffset, 1, (nm32u*)alignDistance, size);	 
	nmppsAndC_32u((nm32u*)imageOffset, 0xFFFFFFFE, (nm32u*)imageOffset, size);

	//ptrnInner	 
	absIfNegElse0(minX, temp3, size);	 
	nmppsSub_32s(temp3, alignDistance, temp2, size);	 
	absIfNegElse0(dataTmp->y0, temp3, size);	 
	nmppsMerge_32s(temp2, temp3, (nm32s*)cntxt->ptrnInnPoints, size);

	//ptrnSizes	 
	nmppsClipCC_32s(maxX, 0, imageWidth, temp2, size);	 
	nmppsClipCC_32s(dataTmp->y2, 0, imageHeight, temp3, size);	 
	nmppsSub_32s(temp2, temp0, temp2, size);	 
	nmppsAdd_32s(temp2, alignDistance, temp0, size);	 
	nmppsAndC_32u((nm32u*)temp0, 1, (nm32u*)temp2, size);	 
	nmppsAdd_32s(temp2, temp0, temp0, size);	 
	nmppsSub_32s(temp3, temp1, temp1, size);	 	
	nmppsMerge_32s(temp0, temp1, (nm32s*)cntxt->ptrnSizes, size); 


	baseAddrOffs_32s((nm32s*)cntxt->smallColorBuff.mData, imageOffset, cntxt->imagePoints, size);	 
	baseAddrOffs_32s((nm32s*)cntxt->smallDepthBuff.mData, imageOffset, cntxt->zBuffPoints, size);
	 
	nmppsConvert_32s8s(dataTmp->color, (nm8s*)cntxt->valuesC, 4 * size);	 
	//nmppsCopy_32s(dataTmp->z, cntxt->valuesZ, size);
	 
	mergePtrnsAddr3(srcPackTmp02, srcPackTmp01, srcPackTmp12, SMALL_SIZE, cntxt->ppSrcPackPtrns, size);	 
#ifdef __GNUC__
	mergePtrnsAddr3((nm32s**)sizePackTmp02, (nm32s**)sizePackTmp01, (nm32s**)sizePackTmp12, SMALL_SIZE, (nm32s**)cntxt->nSizePtrn32, size);
#else
	int point = 0;
	int counter = 0;
	int count = size;
	int step = SMALL_SIZE;
	while (count > 0) {
		int localSize = MIN(step, count);
		for (int i = 0; i<localSize; i++) {
			cntxt->nSizePtrn32[3 * point + i] = sizePackTmp02[point + i];
			cntxt->nSizePtrn32[3 * point + localSize + i] = sizePackTmp01[point + i];
			cntxt->nSizePtrn32[3 * point + 2 * localSize + i] = sizePackTmp12[point + i];
		}
		point += step;
		count -= step;
	}
#endif

	msdWaitDma(0);

#ifdef __GNUC__
	nmppsAdd_32s((int*)dstPackTmp12, sizePackTmp01, (int*)dstPackTmp12, size);
#else
	for (int i = 0; i < size; i++) {
		dstPackTmp12[i] += sizePackTmp01[i];
	}
#endif
	mergePtrnsAddr3(dstPackTmp02, dstPackTmp01, dstPackTmp12, SMALL_SIZE, cntxt->ppDstPackPtrns, size);

#ifdef TEXTURE_ENABLED
	if (cntxt->texState.textureEnabled) {
		for (int i = 0; i < size; i++) {
			cntxt->x0[i] = (float)dataTmp->x0[i];
			cntxt->y0[i] = (float)dataTmp->y0[i];
			cntxt->x1[i] = (float)dataTmp->x1[i];
			cntxt->y1[i] = (float)dataTmp->y1[i];
			cntxt->x2[i] = (float)dataTmp->x2[i];
			cntxt->y2[i] = (float)dataTmp->y2[i];

			cntxt->texS0[i] = dataTmp->s0[i];
			cntxt->texT0[i] = dataTmp->t0[i];
			cntxt->texS1[i] = dataTmp->s1[i];
			cntxt->texT1[i] = dataTmp->t1[i];
			cntxt->texS2[i] = dataTmp->s2[i];
			cntxt->texT2[i] = dataTmp->t2[i];

			cntxt->w0[i] = dataTmp->w0[i];
			cntxt->w1[i] = dataTmp->w1[i];
			cntxt->w2[i] = dataTmp->w2[i];
		}
	}
#endif //TEXTURE_ENABLED

	//этот кусок кода является си-реализацией этой функции и является более наглядным	
	/*for (int i = 0; i < size; i++) {

		int ptrnNo01, ptrnNo02, ptrnNo12;
		if (data->crossProducts[i] < 0) {
			ptrnNo01 = cntxt->patterns->table_dydx[dy01[i] * 2 * WIDTH_PTRN + dx01[i] + WIDTH_PTRN];
			ptrnNo01 += data->x0[i] - minX[i];
			ptrnNo01 += NPATTERNS / 2;
			ptrnNo02 = cntxt->patterns->table_dydx[dy02[i] * 2 * WIDTH_PTRN + dx02[i] + WIDTH_PTRN];
			ptrnNo02 += data->x0[i] - minX[i];
			ptrnNo12 = cntxt->patterns->table_dydx[dy12[i] * 2 * WIDTH_PTRN + dx12[i] + WIDTH_PTRN];
			ptrnNo12 += data->x1[i] - minX[i];
			ptrnNo12 += NPATTERNS / 2;
		}
		else {
			ptrnNo01 = cntxt->patterns->table_dydx[dy01[i] * 2 * WIDTH_PTRN + dx01[i] + WIDTH_PTRN];
			ptrnNo01 += data->x0[i] - minX[i];
			ptrnNo02 = cntxt->patterns->table_dydx[dy02[i] * 2 * WIDTH_PTRN + dx02[i] + WIDTH_PTRN];
			ptrnNo02 += data->x0[i] - minX[i];
			ptrnNo02 += NPATTERNS / 2;
			ptrnNo12 = cntxt->patterns->table_dydx[dy12[i] * 2 * WIDTH_PTRN + dx12[i] + WIDTH_PTRN];
			ptrnNo12 += data->x1[i] - minX[i];
		}
		

		srcPackTmp02[i] = (nm32s*)cntxt->patterns->ptrns[ptrnNo02];
		srcPackTmp01[i] = (nm32s*)cntxt->patterns->ptrns[ptrnNo01];
		srcPackTmp12[i] = (nm32s*)cntxt->patterns->ptrns[ptrnNo12];

		sizePackTmp02[i] = dy02[i] * WIDTH_PTRN / 16;
		sizePackTmp01[i] = dy01[i] * WIDTH_PTRN / 16;
		sizePackTmp12[i] = dy12[i] * WIDTH_PTRN / 16;

		dstPackTmp02[i] = (nm32s*)cntxt->ppPtrns1_2s[i % SMALL_SIZE];
		dstPackTmp01[i] = (nm32s*)cntxt->ppPtrns2_2s[i % SMALL_SIZE];
		dstPackTmp12[i] = (nm32s*)cntxt->ppPtrns2_2s[i % SMALL_SIZE] +
			+ dy01[i] * WIDTH_PTRN / 16; // + cntxt->nSizePtrn32[3 * i + 1]

		if (minX[i] < 0) {
			cntxt->ptrnInnPoints[i].x = -minX[i];
			minX[i] = 0;
		}
		else {
			cntxt->ptrnInnPoints[i].x = 0;
		}
		int minY = data->y0[i];
		if (minY < 0) {
			cntxt->ptrnInnPoints[i].y = -minY;
			minY = 0;
		}
		else {
			cntxt->ptrnInnPoints[i].y = 0;
		}
		maxX[i] = MIN(imageWidth, maxX[i]);
		int maxY = MIN(imageWidth, data->y2[i]);
		cntxt->ptrnSizes[i].width = maxX[i] - minX[i];
		cntxt->ptrnSizes[i].height = maxY - minY;
		
		imageOffset[i] = data->y0[i] * cntxt->smallDepthBuff.getWidth() + minX[i];
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
	}*/
	return 0;
}