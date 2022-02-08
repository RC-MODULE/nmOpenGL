#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"
#include "service.h"
#include "utility_fixed.h"
#include "imagebuffer.h"


//SECTION(".text_demo3d") void readPolygonsL(DataForNmpu1* data){
SECTION(".text_demo3d") int getAddrPtrnsL(DataForNmpu1* data) {
	NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
	PatternPack * patternPack = &cntxt->patternPack;
	int size = data->count;	
	int offset0 = 0;
	int** srcPackTmp01 = (int**)(cntxt->buffer0 + 0 * size);
	int** dstPackTmp01 = (int**)(cntxt->buffer0 + 1 * size);
	int* sizePackTmp01 = (cntxt->buffer0 + offset0 + 2 * size);
	offset0 += 3 * size;

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
		srcPackTmp01[i] = (nm32s*)&cntxt->patterns->linePtrns[ptrnNo];
		dstPackTmp01[i] = (nm32s*)(patternPack->patterns + i);
		if (dy == 0)
			dy++;
		sizePackTmp01[i] = dy * WIDTH_PTRN / 16;

		patternPack->origins[i].x = (dx < 0) ? WIDTH_PTRN + dx : 0;

		if (minX < 0) {
			patternPack->origins[i].x += -minX;
			minX = 0;
		}
		else {
			patternPack->origins[i].x = patternPack->origins[i].x;
			//cntxt->ptrnInnPoints[i].x = 0;
		}
		int minY = data->y0[i];
		if (minY < 0) {
			patternPack->origins[i].y = -minY;
			minY = 0;
		}
		else {
			patternPack->origins[i].y = 0;
		}		
		maxX = MIN(cntxt->smallColorBuff.getWidth(), maxX);
		int maxY = MIN(cntxt->smallColorBuff.getWidth(), data->y1[i]);		
		patternPack->sizes[i].width = maxX - minX;
		patternPack->sizes[i].height = maxY - minY;
		if (patternPack->sizes[i].width == 0)
			patternPack->sizes[i].width++;
		if (patternPack->sizes[i].height == 0)
			patternPack->sizes[i].height++;
		
		int imageOffset = minY * cntxt->smallColorBuff.getWidth() + minX;
		if (imageOffset % 2) {
			imageOffset--;
			patternPack->origins[i].x--;
			patternPack->sizes[i].width++;
		}
		if (patternPack->sizes[i].width % 2) {
			patternPack->sizes[i].width++;
		}

		cntxt->imagePoints[i] = nmppsAddr_32s((nm32s*)cntxt->smallColorBuff.mData, imageOffset);
		cntxt->zBuffPoints[i] = nmppsAddr_32s((nm32s*)cntxt->smallDepthBuff.mData, imageOffset);

		cntxt->valuesC[i]  = (data->color[4 * i + 0] & 0xFF);
		cntxt->valuesC[i] |= (data->color[4 * i + 1] & 0xFF) << 8;
		cntxt->valuesC[i] |= (data->color[4 * i + 2] & 0xFF) << 16;
		cntxt->valuesC[i] |= (data->color[4 * i + 3] & 0xFF) << 24;

		cntxt->valuesZ[i] = data->z[i];

	}
	copyPacket_32s(srcPackTmp01,
		dstPackTmp01,
		sizePackTmp01, size);
	return 0;
}