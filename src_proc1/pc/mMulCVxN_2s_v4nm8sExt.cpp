#include "demo3d_nm1.h"
extern "C" {
	
	void mMulCVxN_2s_v4nm8sExt(nm2s** ppSrcTreangle_2s, int* offsets, int* widths, int* heights, v4nm8s* pDstTreangle, v4nm8s* valueC,  int count){
		long long int temp;
		nm8s* dst = (nm8s*)pDstTreangle;
		nm8s* colors = (nm8s*)valueC;
		for (int c = 0; c<count; c++) {
			long long int* src = (nm64s*)ppSrcTreangle_2s[c];
			int x0;
			int width = widths[c];
			nm32s* nextDst;

			offsets[c] = (offsets[c] < 0) ? 0 : offsets[c];
			nextDst = (nm32s*)dst + heights[c] * width;
			x0 = offsets[c] % 32;

			/*if (offsets[c] == -1) {
				width++;
				for (int y = 0; y < heights[c]; y++) {
					((nm32s*)dst)[y*width] = 0;
				}
				nextDst = (nm32s*)dst + heights[c] * width;
				dst++;
				x0 = 0;
			}
			else {
				x0 = offsets[c] % 32;
				nextDst = (nm32s*)dst + heights[c] * width;
			}*/

			for (int y = 0; y < heights[c]; y++) {
				temp = src[y];
				temp >>= (x0 * 2);
				for (int x = 0; x<widths[c]; x++) {
					int position = y * widths[c] + x;
					for (int i = 0; i < 4; i++) {
						dst[4 * position + i] = (temp & 0x3) * colors[4 * c + i];
					}
					temp >>= 2;
				}
			}
			dst = (nm8s*)nextDst;

		}
	}

}