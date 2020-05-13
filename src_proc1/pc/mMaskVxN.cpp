#include "nmtype.h"
#include "demo3d_nm1.h"

extern "C" {
	
	void mMaskVxN_32s(nm32s* pTriangles, 
		nm32s* pMask, 
		nm32s** pROI, 
		int imageStride, 
		Size* ptrnSizes, 
		int count)
	{
		for (int c = 0; c < count; c++) {
			for(int y = 0; y < ptrnSizes[c].height; y++){
				for(int x = 0; x < ptrnSizes[c].width; x++){
					int offsetSrc = y * ptrnSizes[c].width + x;
					int offsetDst = y * imageStride + x;
					if(pMask[offsetSrc] < 0)
						pROI[c][offsetDst] = pTriangles[offsetSrc];
				}
			}
			pMask += ptrnSizes[c].height * ptrnSizes[c].width;
			pTriangles += ptrnSizes[c].height * ptrnSizes[c].width;
		}
	}
	
	void mMaskVxN_16s(nm16s* pTriangles, 
		nm16s* pMask, 
		nm16s** pROI, 
		int imageStride, 
		Size* ptrnSizes, 
		int count)
	{
		for (int c = 0; c < count; c++) {
			for(int y = 0; y < ptrnSizes[c].height; y++){
				for(int x = 0; x < ptrnSizes[c].width; x++){
					int offsetSrc = y * ptrnSizes[c].width + x;
					int offsetDst = y * imageStride + x;
					if(pMask[offsetSrc] < 0)
						pROI[c][offsetDst] = pTriangles[offsetSrc];
				}
			}
			pMask += ptrnSizes[c].height * ptrnSizes[c].width;
			pTriangles += ptrnSizes[c].height * ptrnSizes[c].width;
		}
	}
}
