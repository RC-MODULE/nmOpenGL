#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"
#include "service.h"
#include "imagebuffer.h"

SECTION(".data_demo3d") unsigned int points[14];
SECTION(".data_demo3d") MyDmaTask task;

const int maxTmpCount0 = (SIZE_BANK - sizeof32(DataForNmpu1)) / POLYGONS_SIZE;
const int maxTmpCount1 = SIZE_BANK / POLYGONS_SIZE;

#define CHECK_STATUS(a) while (!msdGetStatusCopy(points[a], 0))

inline void ADD_COPY(const void* src, void* dst, int size, int i) {
	task.src = src;
	task.dst = dst;
	task.size = size;
	points[i] = msdAdd(task, 0);
}


//SECTION(".text_demo3d") void readPolygonsT(DataForNmpu1* data){
SECTION(".text_demo3d") int getAddrPtrnsT(DataForNmpu1* data) {
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	int size = data->count;
	int** srcPackTmp02 = (int**)(cntxt->buffer0 + 0 * POLYGONS_SIZE);
	int** srcPackTmp01 = (int**)(cntxt->buffer0 + 1 * POLYGONS_SIZE);
	int** srcPackTmp12 = (int**)(cntxt->buffer0 + 2 * POLYGONS_SIZE);
	int** dstPackTmp02 = (int**)(cntxt->buffer0 + 3 * POLYGONS_SIZE);
	int** dstPackTmp01 = (int**)(cntxt->buffer0 + 4 * POLYGONS_SIZE);
	int** dstPackTmp12 = (int**)(cntxt->buffer0 + 5 * POLYGONS_SIZE);
	int* sizePackTmp02 = (int*)(cntxt->buffer1 + 0 * POLYGONS_SIZE);
	int* sizePackTmp01 = (int*)(cntxt->buffer1 + 1 * POLYGONS_SIZE);
	int* sizePackTmp12 = (int*)(cntxt->buffer1 + 2 * POLYGONS_SIZE);

	for (int i = 0; i < size; i++) {
		int minX = MIN(data->x0[i], data->x1[i]);
		minX = MIN(minX, data->x2[i]);
		int maxX = MAX(data->x0[i], data->x1[i]);
		maxX = MAX(maxX, data->x2[i]);

		int dx01 = data->x1[i] - data->x0[i];
		int dy01 = data->y1[i] - data->y0[i];
		int dx12 = data->x2[i] - data->x1[i];
		int dy12 = data->y2[i] - data->y1[i];
		int dx02 = data->x2[i] - data->x0[i];
		int dy02 = data->y2[i] - data->y0[i];

		int ptrnNo01, ptrnNo02, ptrnNo12;
		if (data->crossProducts[i] < 0) {
			ptrnNo01 = cntxt->patterns->table_dydx[dy01 * 2 * WIDTH_PTRN + dx01 + WIDTH_PTRN];
			ptrnNo01 += data->x0[i] - minX;
			ptrnNo01 += NPATTERNS / 2;
			ptrnNo02 = cntxt->patterns->table_dydx[dy02 * 2 * WIDTH_PTRN + dx02 + WIDTH_PTRN];
			ptrnNo02 += data->x0[i] - minX;
			ptrnNo12 = cntxt->patterns->table_dydx[dy12 * 2 * WIDTH_PTRN + dx12 + WIDTH_PTRN];
			ptrnNo12 += data->x1[i] - minX;
			ptrnNo12 += NPATTERNS / 2;
		}
		else {
			ptrnNo01 = cntxt->patterns->table_dydx[dy01 * 2 * WIDTH_PTRN + dx01 + WIDTH_PTRN];
			ptrnNo01 += data->x0[i] - minX;
			ptrnNo02 = cntxt->patterns->table_dydx[dy02 * 2 * WIDTH_PTRN + dx02 + WIDTH_PTRN];
			ptrnNo02 += data->x0[i] - minX;
			ptrnNo02 += NPATTERNS / 2;
			ptrnNo12 = cntxt->patterns->table_dydx[dy12 * 2 * WIDTH_PTRN + dx12 + WIDTH_PTRN];
			ptrnNo12 += data->x1[i] - minX;
		}
		

		srcPackTmp02[i] = (nm32s*)cntxt->patterns->ptrns[ptrnNo02];
		srcPackTmp01[i] = (nm32s*)cntxt->patterns->ptrns[ptrnNo01];
		srcPackTmp12[i] = (nm32s*)cntxt->patterns->ptrns[ptrnNo12];

		sizePackTmp02[i] = dy02 * WIDTH_PTRN / 16;
		sizePackTmp01[i] = dy01 * WIDTH_PTRN / 16;
		sizePackTmp12[i] = dy12 * WIDTH_PTRN / 16;

		dstPackTmp02[i] = (nm32s*)cntxt->ppPtrns1_2s[i % SMALL_SIZE];
		dstPackTmp01[i] = (nm32s*)cntxt->ppPtrns2_2s[i % SMALL_SIZE];
		dstPackTmp12[i] = (nm32s*)cntxt->ppPtrns2_2s[i % SMALL_SIZE] +
			+ dy01 * WIDTH_PTRN / 16; // + cntxt->nSizePtrn32[3 * i + 1]

		if (minX < 0) {
			cntxt->ptrnInnPoints[i].x = -minX;
			minX = 0;
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
		maxX = MIN(cntxt->smallColorBuff.getWidth(), maxX);
		int maxY = MIN(cntxt->smallColorBuff.getWidth(), data->y2[i]);
		cntxt->ptrnSizes[i].width = maxX - minX;
		cntxt->ptrnSizes[i].height = maxY - minY;
		
		int imageOffset = minY * cntxt->smallDepthBuff.getWidth() + minX;
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
	}
	mergePtrnsAddr3(srcPackTmp02, srcPackTmp01, srcPackTmp12, SMALL_SIZE, cntxt->ppSrcPackPtrns, size);
	mergePtrnsAddr3(dstPackTmp02, dstPackTmp01, dstPackTmp12, SMALL_SIZE, cntxt->ppDstPackPtrns, size);
	mergePtrnsAddr3((nm32s**)sizePackTmp02, (nm32s**)sizePackTmp01, (nm32s**)sizePackTmp12, SMALL_SIZE, (nm32s**)cntxt->nSizePtrn32, size);
	/*DataForNmpu1* dataTmp = (DataForNmpu1*)cntxt->buffer0;
	offset0 += sizeof32(DataForNmpu1);	
	nm32s* temp0 = nmppsAddr_32s(cntxt->buffer0, offset0);
	offset0 += POLYGONS_SIZE;
	nm32s* dydx = nmppsAddr_32s(cntxt->buffer0, offset0);
	offset0 += sizeof32(cntxt->patterns->table_dydx);
	nm32s* minX = nmppsAddr_32s(cntxt->buffer0, offset0);
	offset0 += POLYGONS_SIZE;
	nm32s* minY = nmppsAddr_32s(cntxt->buffer0, offset0);
	offset0 += POLYGONS_SIZE;
	if (offset0 > SIZE_BANK) {
		printf("error!! offset0=%d\n", offset0);
		return;
	}

	nm32s* temp1 = cntxt->buffer1;
	offset1 += POLYGONS_SIZE;
	nm32s* temp2 = nmppsAddr_32s(cntxt->buffer1, offset1);
	if (offset1 > SIZE_BANK) {
		printf("error!! offset0=%d\n", offset0);
		return;
	}

	msdAdd(data, dataTmp, sizeof32(DataForNmpu1), 0);
	msdWaitDma(0);

	int size = data->count;*/

	/*nmppsMinEvery_32s(dataTmp->x0, dataTmp->x1, temp1, size);
	nmppsMinEvery_32s(temp1, dataTmp->x2, minY, size);
	nmppsMinEvery_32s(dataTmp->y0, dataTmp->y1, temp1, size);
	nmppsMinEvery_32s(temp1, dataTmp->y2, minY, size);*/

	return 0;
}