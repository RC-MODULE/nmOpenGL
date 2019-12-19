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
			
			if (offsets[c] == -1) {
				width--;
			}

			for (int y = 0; y < heights[c]; y++) {
				temp = src[y];
				nm8s* pDst = dst + 4 * y * widths[c];
				if (offsets[c] == -1) {
					for (int i = 0; i < 4; i++) {
						*pDst = 0;
						pDst++;
					}					
				}
				else {
					temp >>= (offsets[c] * 2);
				}

				for (int x = 0; x<widths[c]; x++) {
					int position = y * widths[c] + x;
					for (int i = 0; i < 4; i++) {
						*pDst = (temp & 0x3) * colors[4 * c + i];
						pDst++;
					}
					temp >>= 2;
				}
			}
			dst += 4 * heights[c] * widths[c];

		}
	}

}