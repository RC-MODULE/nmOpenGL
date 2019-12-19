#include "demo3d_nm1.h"

int addC4DepthTest = ZBUFF_MAX;

extern "C" {
	
	void mMulCVxN_2s32sExt(nm2s** ppSrcTreangle_2s, int* offsets, int* widths, int* heights, nm32s* pDstTreangle, int* valueC,  int count){
		long long int temp;
		nm32s* dst = pDstTreangle;
		for(int c=0;c<count;c++){
			long long int* src = (nm64s*) ppSrcTreangle_2s[c];
			int x0;
			int width = widths[c];

			if (offsets[c] == -1) {
				width--;
			}
			
			for(int y = 0; y < heights[c]; y++){
				temp = src[y];
				nm32s* pDst = dst + y * widths[c];
				if (offsets[c] == -1) {
					*pDst = addC4DepthTest;
					pDst++;
				}
				else {
					temp >>= (offsets[c] * 2);
				}
				for(int x = 0; x < width; x++){
					int mul = ((temp & 0x3) * valueC[c]);
					int tmp = mul & 0x80000000;
					*pDst = (mul + addC4DepthTest) & 0x7FFFFFFF | tmp;
					pDst++;
					temp >>= 2;
				}
			}
			dst += heights[c] * widths[c];
			
		}
	}

}