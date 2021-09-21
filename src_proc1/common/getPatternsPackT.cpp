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
SECTION(".text_demo3d") void getPatternsPackT(
		DataForNmpu1* data, 
		PatternsArray* patternsArray, 
		Size* imageSize, 
		PatternPack * patternPack, 
		SimpleBuffer<int> *buffers) {
	int* localTable = patternsArray->fillTable;

	int size = data->count;
	patternPack->count = size;
	Size segmentSize = *imageSize;
	
	int* dx02 = buffers[0].alloc(size);
	int* dx01 = buffers[1].alloc(size);
	int* dx12 = buffers[2].alloc(size);
	int* dy02 = buffers[1].alloc(size);
	int* dy01 = buffers[2].alloc(size);
	int* dy12 = buffers[0].alloc(size);

	int* minX = buffers[1].alloc(size);
	int* maxX = buffers[1].alloc(size);

	int* localX0 = buffers[1].alloc(size);
	int* localX1 = buffers[1].alloc(size);

	int* temp0 = buffers[0].alloc(size);
	int* temp1 = buffers[1].alloc(size);
	int* temp2 = buffers[2].alloc(size);
	int* temp3 = buffers[1].alloc(size);
	int* alignDistance = buffers[1].alloc(size);

	int** srcPackTmp02 = (int**)buffers[0].alloc(sizeof32(int*) * size);
	int** srcPackTmp01 = (int**)buffers[0].alloc(sizeof32(int*) * size);
	int** srcPackTmp12 = (int**)buffers[0].alloc(sizeof32(int*) * size);
	int** dstPackTmp02 = (int**)buffers[0].alloc(sizeof32(int*) * size);
	int** dstPackTmp01 = (int**)buffers[0].alloc(sizeof32(int*) * size);
	int** dstPackTmp12 = (int**)buffers[0].alloc(sizeof32(int*) * size);
	int* sizePackTmp02 = buffers[0].alloc(size);
	int* sizePackTmp01 = buffers[0].alloc(size);
	int* sizePackTmp12 = buffers[0].alloc(size);

#ifdef DEBUG
	if (!buffers[0].isCorrect() || !buffers[1].isCorrect() || !buffers[2].isCorrect()) {
		printf("error!! \n");
		printf("readPolygonsT - over size of bank\n");
		data->count = 0;
		return;
	}
#endif // DEBUG	

	nmppsSub_32s(data->x1, data->x0, dx01, size);
	nmppsSub_32s(data->x2, data->x0, dx02, size);	
	nmppsSub_32s(data->x2, data->x1, dx12, size);	 
	nmppsMinMaxEvery_32s(data->x0, data->x1, temp0, temp1, size);
	nmppsMinEvery_32s(temp0, data->x2, minX, size);
	nmppsMaxEvery_32s(temp1, data->x2, maxX, size);
	nmppsSub_32s(data->x0, minX, localX0, size);
	nmppsSub_32s(data->x1, minX, localX1, size);
	
	nmppsSub_32s(data->y2, data->y0, dy02, size);	 
	nmppsSub_32s(data->y1, data->y0, dy01, size);	 
	nmppsSub_32s(data->y2, data->y1, dy12, size);
	// get ptrn sizes of int	 
	nmppsMulC_32s(dy02, WIDTH_PTRN / 16, sizePackTmp02, size);
	nmppsMulC_32s(dy12, WIDTH_PTRN / 16, sizePackTmp12, size);
	nmppsMulC_32s(dy01, WIDTH_PTRN / 16, sizePackTmp01, size);



	for (int i = 0; i < size; i++) {
		dstPackTmp02[i] = (int*)((Pattern*)buffers[1].base() + i);
		dstPackTmp01[i] = (int*)((Pattern*)buffers[2].base() + i);
	}
#ifdef __GNUC__
	//nmppsCopy_32s((nm32s*)cntxt->ppPtrns1_2s, (nm32s*)dstPackTmp02, size);
	//nmppsCopy_32s((nm32s*)cntxt->ppPtrns2_2s, (nm32s*)dstPackTmp01, size);
	nmppsAdd_32s((nm32s*)dstPackTmp01, sizePackTmp01, (nm32s*)dstPackTmp12, size);
#else
	for(int i = 0; i < size; i++){
		//dstPackTmp02[i] = (int*)cntxt->ppPtrns1_2s[i];
		//dstPackTmp01[i] = (int*)cntxt->ppPtrns2_2s[i];
		dstPackTmp12[i] = dstPackTmp01[i] + sizePackTmp01[i];
	}
#endif


	// get ptrnNumbers	 
	selectPaintSide(data->crossProducts, 0, NPATTERNS / 2, temp3, size);	 
	nmppsMulC_AddV_AddC_32s(dy02, WIDTH_TABLE, dx02, WIDTH_TABLE / 2, temp2, size);
	nmppsRemap_32u((nm32u*)localTable, (nm32u*)temp0, temp2, size);
	nmppsAdd_32s(temp0, localX0, temp2, size);
	nmppsAdd_32s(temp2, temp3, temp0, size);	 
	nmppsMulC_32s(temp0, sizeof32(Pattern), temp1, size);
	baseAddrOffs_32s((nm32s*)patternsArray->ptrns, temp1, srcPackTmp02, size);

	selectPaintSide(data->crossProducts, NPATTERNS / 2, 0, temp3, size);	 
	nmppsMulC_AddV_AddC_32s(dy01, WIDTH_TABLE, dx01, WIDTH_TABLE / 2, temp0, size);
	nmppsRemap_32u((nm32u*)localTable, (nm32u*)temp2, temp0, size);	 
	nmppsAdd_32s(temp2, localX0, temp0, size);
	nmppsAdd_32s(temp0, temp3, temp2, size);
	nmppsMulC_32s(temp2, sizeof32(Pattern), temp0, size);
	baseAddrOffs_32s((nm32s*)patternsArray->ptrns, temp0, srcPackTmp01, size);

	nmppsMulC_AddV_AddC_32s(dy12, WIDTH_TABLE, dx12, WIDTH_TABLE / 2, temp1, size);
	nmppsRemap_32u((nm32u*)localTable, (nm32u*)temp0, temp1, size);
	nmppsAdd_32s(temp0, localX1, temp2, size);
	nmppsAdd_32s(temp2, temp3, temp0, size);
	nmppsMulC_32s(temp0, sizeof32(Pattern), temp1, size);
	baseAddrOffs_32s((nm32s*)patternsArray->ptrns, temp1, srcPackTmp12, size);
	
	// get imageOffset	 
	nmppsClipCC_32s(minX, 0, segmentSize.width, temp0, size);
	nmppsClipCC_32s(data->y0, 0, segmentSize.height, temp1, size);	 
	nmppsAndC_32u((nm32u*)temp0, 1, (nm32u*)alignDistance, size);
	nmppsAndC_32u((nm32u*)temp0, 0xFFFFFFFE, (nm32u*)temp0, size);
	nmppsMulC_AddV_AddC_32s(temp1, segmentSize.width, temp0, 0, patternPack->imagePositions, size);
	

	//ptrnInner	 
	absIfNegElse0(minX, temp3, size);	 
	nmppsSub_32s(temp3, alignDistance, temp2, size);	 
	absIfNegElse0(data->y0, temp3, size);	 
	nmppsMerge_32s(temp2, temp3, (nm32s*)patternPack->origins, size);

	//ptrnSizes	 
	nmppsClipCC_32s(maxX, 0, segmentSize.width, temp2, size);
	nmppsClipCC_32s(data->y2, 0, segmentSize.height, temp3, size);	 
	nmppsSub_32s(temp2, temp0, temp0, size); 
	nmppsAndC_32u((nm32u*)temp0, 1, (nm32u*)temp2, size);	 
	nmppsAdd_32s(temp2, temp0, temp0, size);	 
	nmppsSub_32s(temp3, temp1, temp1, size);	 	
	nmppsMerge_32s(temp0, temp1, (nm32s*)patternPack->sizes, size);

	int fullSize = totalSum(sizePackTmp02, 3 * size);

	buffers[0].free(6 * size + sizeof32(int*) * 6 * size);
	buffers[1].free(9 * size);
	buffers[2].free(3 * size);
	
	PROFILER_SIZE(fullSize);
	copyPacket_32s(srcPackTmp02,
		dstPackTmp02,
		sizePackTmp02, 3 * size);

	mAndVxN_32u((nm32u**)dstPackTmp02,
		(nm32u**)dstPackTmp01,
		(nm32u**)patternPack->ppPattern,
		sizePackTmp02, size);
}
