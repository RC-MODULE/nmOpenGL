#include "nmtype.h"
extern "C" {
	void mMaskV_32s(nm32s* pTriangle, int srcStride1,
						nm32s* pImg, int srcStride2,
						nm32s* pMask, int maskStride,
						nm32s* pSameImg, int dstStride,
						int nHeight, int nWidth)
						{
	
								for(int y = 0; y < nHeight; y++) {
									int* triangles_32s = pTriangle + y * srcStride1;
									int* image_32s     = pImg      + y * srcStride2;
									int* masks_32s     = pMask     + y * maskStride;
									int* result_32s    = pSameImg  + y * dstStride;
									for(int x = 0; x < nWidth; x++) {
										if (masks_32s[x] < 0) {
											result_32s[x] = triangles_32s[x];
										}
										else
											result_32s[x] = image_32s[x];
									}
								}
	
	
							}
	
	void mMaskVxN_32s(nm32s* pTriangles, nm32s* pMask, nm32s** pROI, int imageStride, int* pTrianglesHeight, int* pTrianglesWidth, int count)
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
			mMaskV_32s(pTriangles + triangleOffset, triangleWidth,
						pROI[i], imageStride, pMask + maskOffset,
						triangleWidth, pROI[i], imageStride,
						triangleHeight, triangleWidth);
			maskOffset += maskSize;
			triangleOffset += triangleSize;
		}
	}
}
