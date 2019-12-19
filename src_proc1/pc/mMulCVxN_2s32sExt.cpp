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
			nm32s* nextDst;

			offsets[c] = (offsets[c] < 0) ? 0 : offsets[c];
			nextDst = dst + heights[c] * width;
			x0 = offsets[c] % 32;
			/*if (offsets[c] == -1) {
				width+=2;
				for (int y = 0; y < heights[c]; y++) {
					dst[y*width] = 0;
				}
				nextDst = dst + heights[c] * width;
				dst++;
				x0 = 0;
			}
			else {
				x0 = offsets[c] % 32;
				nextDst = dst + heights[c] * width;
			}*/
			
			for(int y = 0; y < heights[c]; y++){
				temp = src[y];
				temp >>= (x0 * 2);
				for(int x = 0; x<widths[c];x++){
					int mul = ((temp & 0x3) * valueC[c]);
					int tmp = mul & 0x80000000;
					dst[y*width +x] = (mul + addC4DepthTest) & 0x7FFFFFFF | tmp;
					//dst[y*width + x] = (temp & 0x3) * valueC[c];
					temp >>= 2;
				}
			}
			dst = nextDst;
			
		}
	}

}