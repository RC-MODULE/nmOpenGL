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

extern "C" void clipCC_32s(
	nm32s*	pSrcVec,			// Input vec			:long Global[Size/2]
	int		NegThreshold,	// Upper threshold		:=[0...2^16-1]
	int		PosThreshold,	// Lower threshold		:=[-2^16....0]
	nm32s*	pDstVec,			// Output vec		:long Local[Size/2]
	int		nSize		// nSize of vec 32 bit elements. nSize=[0,2,4,6...]
);

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
	int offset0 = 0;
	int offset1 = 0;
	DataForNmpu1* dataTmp = (DataForNmpu1*)cntxt->buffer0;
	offset0 += sizeof32(DataForNmpu1);
	msdAdd(data, dataTmp, sizeof32(DataForNmpu1), 0);
	msdWaitDma(0);
	int* dx02 = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* dx01 = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* dx12 = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int* dy02 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* dy01 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* dy12 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;

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
	int* temp2 = cntxt->buffer1 + offset1;
	offset1 += POLYGONS_SIZE;
	int* imageOffset = cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;

	int** srcPackTmp02 = (int**)cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int** srcPackTmp01 = (int**)cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
	int** srcPackTmp12 = (int**)cntxt->buffer0 + offset0;
	offset0 += POLYGONS_SIZE;
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
	if (offset0 > SIZE_BANK || offset1 > SIZE_BANK) {
		printf("error!!\n");
		return 0;
	}
	nmppsSub_32s(dataTmp->x2, dataTmp->x0, dx02, size);
	nmppsSub_32s(dataTmp->x1, dataTmp->x0, dx01, size);
	nmppsSub_32s(dataTmp->x2, dataTmp->x1, dx12, size);
	nmppsSub_32s(dataTmp->y2, dataTmp->y0, dy02, size);
	nmppsSub_32s(dataTmp->y1, dataTmp->y0, dy01, size);
	nmppsSub_32s(dataTmp->y2, dataTmp->y1, dy12, size);

	nmppsMinMaxEvery_32s(dataTmp->x0, dataTmp->x1, temp0, temp1, size);
	nmppsMinEvery_32s(temp0, dataTmp->x2, minX, size);
	nmppsMaxEvery_32s(temp1, dataTmp->x2, maxX, size);

	nmppsSub_32s(dataTmp->x0, minX, localX0, size);
	nmppsSub_32s(dataTmp->x1, minX, localX1, size);
	
	// get ptrnNumbers
	selectPaintSide(dataTmp->crossProducts, 0, NPATTERNS / 2, temp2, size);
	nmppsMulC_AddV_AddC_32s(dy02, 2 * WIDTH_PTRN, dx02, WIDTH_PTRN, temp0, size);
	nmppsRemap_32u((nm32u*)cntxt->patterns->table_dydx, (nm32u*)temp1, temp0, size);
	nmppsAdd_32s(temp1, localX0, temp0, size);
	nmppsAdd_32s(temp0, temp2, temp1, size);
	nmppsMulC_32s(temp1, sizeof32(Pattern), temp0, size);
	baseAddrOffs_32s((nm32s*)cntxt->patterns->ptrns, temp0, srcPackTmp02, size);
	
	
	selectPaintSide(dataTmp->crossProducts, NPATTERNS / 2, 0, temp2, size);
	nmppsMulC_AddV_AddC_32s(dy01, 2 * WIDTH_PTRN, dx01, WIDTH_PTRN, temp0, size);
	nmppsRemap_32u((nm32u*)cntxt->patterns->table_dydx, (nm32u*)temp1, temp0, size);
	nmppsAdd_32s(temp1, localX0, temp0, size);
	nmppsAdd_32s(temp0, temp2, temp1, size);
	nmppsMulC_32s(temp1, sizeof32(Pattern), temp0, size);
	baseAddrOffs_32s((nm32s*)cntxt->patterns->ptrns, temp0, srcPackTmp01, size);
	

	nmppsMulC_AddV_AddC_32s(dy12, 2 * WIDTH_PTRN, dx12, WIDTH_PTRN, temp0, size);
	nmppsRemap_32u((nm32u*)cntxt->patterns->table_dydx, (nm32u*)temp1, temp0, size);
	nmppsAdd_32s(temp1, localX1, temp0, size);
	nmppsAdd_32s(temp0, temp2, temp1, size);
	nmppsMulC_32s(temp1, sizeof32(Pattern), temp0, size);
	baseAddrOffs_32s((nm32s*)cntxt->patterns->ptrns, temp0, srcPackTmp12, size);
	
	// get ptrn sizes of int
	nmppsMulC_32s(dy02, WIDTH_PTRN / 16, sizePackTmp02, size);
	nmppsMulC_32s(dy12, WIDTH_PTRN / 16, sizePackTmp12, size);
	nmppsMulC_32s(dy01, WIDTH_PTRN / 16, sizePackTmp01, size);

	// get imageOffset
	nmppsClipCC_32s(minX, 0, cntxt->smallColorBuff.getWidth(), temp0, size);
	nmppsClipCC_32s(dataTmp->y0, 0, cntxt->smallColorBuff.getHeight(), temp1, size);
	nmppsMulC_AddV_AddC_32s(temp1, cntxt->smallColorBuff.getWidth(), temp0, 0, imageOffset, size);

	absIfNegElse0(minX, temp0, size);
	absIfNegElse0(dataTmp->y0, temp1, size);
	nmppsMerge_32s(temp0, temp1, (nm32s*)cntxt->ptrnInnPoints, size);
	nmppsClipCC_32s(minX, 0, cntxt->smallColorBuff.getWidth(), temp0, size);
	nmppsClipCC_32s(maxX, 0, cntxt->smallColorBuff.getWidth(), temp1, size);	
	nmppsSub_32s(temp1, temp0, temp2, size);
	nmppsClipCC_32s(dataTmp->y0, 0, cntxt->smallColorBuff.getHeight(), temp1, size);
	nmppsMulC_AddV_AddC_32s(temp1, cntxt->smallColorBuff.getWidth(), temp0, 0, imageOffset, size);
	nmppsClipCC_32s(dataTmp->y2, 0, cntxt->smallColorBuff.getHeight(), temp0, size);
	nmppsSub_32s(temp0, temp1, temp1, size);
	nmppsMerge_32s(temp2, temp1, (nm32s*)cntxt->ptrnSizes, size);

	nmppsAndC_32u((nm32u*)imageOffset, 1, (nm32u*)temp2, size);
	nmppsAndC_32u((nm32u*)imageOffset, 0xFFFFFFFE, (nm32u*)imageOffset, size);
	nmppsSplit_32s((nm32s*)cntxt->ptrnInnPoints, temp0, temp1, 2 * size);
	nmppsSub_32s(temp0, temp2, temp0, size);
	nmppsMerge_32s(temp0, temp1, (nm32s*)cntxt->ptrnInnPoints, size);

	nmppsSplit_32s((nm32s*)cntxt->ptrnSizes, temp0, temp1, 2 *size);
	nmppsAdd_32s(temp0, temp2, temp0, size);
	nmppsAndC_32u((nm32u*)temp0, 1, (nm32u*)temp2, size);
	nmppsAdd_32s(temp0, temp2, temp0, size);
	nmppsMerge_32s(temp0, temp1, (nm32s*)cntxt->ptrnSizes, size);

	int height = size / SMALL_SIZE;
	nmppmCopy_32s((nm32s*)cntxt->ppPtrns1_2s, 0, (nm32s*)dstPackTmp02, SMALL_SIZE, height, SMALL_SIZE);
	nmppsCopy_32s((nm32s*)cntxt->ppPtrns1_2s, (nm32s*)(dstPackTmp02 + height * SMALL_SIZE), size % SMALL_SIZE);

	nmppmCopy_32s((nm32s*)cntxt->ppPtrns2_2s, 0, (nm32s*)dstPackTmp01, SMALL_SIZE, height, SMALL_SIZE);
	nmppsCopy_32s((nm32s*)cntxt->ppPtrns2_2s, (nm32s*)(dstPackTmp01 + height * SMALL_SIZE), size % SMALL_SIZE);

	nmppmCopy_32s((nm32s*)cntxt->ppPtrns2_2s, 0, (nm32s*)temp0, SMALL_SIZE, height, SMALL_SIZE);
	nmppsCopy_32s((nm32s*)cntxt->ppPtrns2_2s, (nm32s*)(temp0 + height * SMALL_SIZE), size % SMALL_SIZE);
#ifdef __NM__
	nmppsAdd_32s(temp0, sizePackTmp01, (int*)dstPackTmp12, size);
#else 
	for (int i = 0; i < size; i++) {
		dstPackTmp12[i] = (nm32s*)temp0[i] + sizePackTmp01[i];
	}
#endif
	baseAddrOffs_32s((nm32s*)cntxt->smallColorBuff.mData, imageOffset, cntxt->imagePoints, size);
	baseAddrOffs_32s((nm32s*)cntxt->smallDepthBuff.mData, imageOffset, cntxt->zBuffPoints, size);
	nmppsConvert_32s8s(dataTmp->color, (nm8s*)cntxt->valuesC, 4 * size);
	nmppsCopy_32s(dataTmp->z, cntxt->valuesZ, size);
	mergePtrnsAddr3(srcPackTmp02, srcPackTmp01, srcPackTmp12, SMALL_SIZE, cntxt->ppSrcPackPtrns, size);
	mergePtrnsAddr3(dstPackTmp02, dstPackTmp01, dstPackTmp12, SMALL_SIZE, cntxt->ppDstPackPtrns, size);
	mergePtrnsAddr3((nm32s**)sizePackTmp02, (nm32s**)sizePackTmp01, (nm32s**)sizePackTmp12, SMALL_SIZE, (nm32s**)cntxt->nSizePtrn32, size);


	//���� ����� ���� �������� ��-����������� ���� ������� � �������� ����� ���������	
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
		maxX[i] = MIN(cntxt->smallColorBuff.getWidth(), maxX[i]);
		int maxY = MIN(cntxt->smallColorBuff.getWidth(), data->y2[i]);
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