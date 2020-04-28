#include "nmtype.h"
extern "C" {
	void mMaskV_16s(nm16s* pTriangle, int srcStride1,
						nm16s* pImg, int srcStride2,
						nm16s* pMask, int maskStride,
						nm16s* pSameImg, int dstStride,
						int nHeight, int nWidth)
						{
	
								for(int y = 0; y < nHeight; y++) {
									nm16s* triangles_32s = pTriangle + y * srcStride1;
									nm16s* image_32s     = pImg      + y * srcStride2;
									nm16s* masks_32s     = pMask     + y * maskStride;
									nm16s* result_32s    = pSameImg  + y * dstStride;
									for(int x = 0; x < nWidth; x++) {
										if (masks_32s[x] < 0) {
											result_32s[x] = triangles_32s[x];
										}
										else
											result_32s[x] = image_32s[x];
									}
								}
	
	
							}
	
	void mMaskVxN_16s(nm16s* pTriangles, nm16s* pMask, nm16s** pROI, int imageStride, int* pTrianglesHeight, int* pTrianglesWidth, int count)
	{
		int maskOffset     = 0;
		int triangleOffset = 0;
		int triangleWidth;
		int triangleHeight;
		int triangleSize;
		int maskSize;
		for (int i = 0; i < count; i++) {
			triangleWidth = pTrianglesWidth[i];
			triangleHeight = pTrianglesHeight[i];
			triangleSize = triangleWidth * triangleHeight;
			maskSize = triangleSize;
			mMaskV_16s(pTriangles + triangleOffset, triangleWidth,
						pROI[i], imageStride, pMask + maskOffset,
						triangleWidth, pROI[i], imageStride,
						triangleHeight, triangleWidth);
			maskOffset += maskSize;
			triangleOffset += triangleSize;
		}
	}
}
