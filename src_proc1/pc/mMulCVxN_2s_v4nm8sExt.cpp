#include "demo3d_nm1.h"
extern "C" {
	
	void mMulCVxN_2s_v4nm32sExt(nm2s** ppSrcTreangle_2s, int* offsets, int* widths, int* heights, nm32s* pDstTreangle, nm32s* valueC,  int count){
		long long int temp;
		nm32s* dst = pDstTreangle;
		for(int c=0;c<count;c++){
			long long int* src = (nm64s*) ppSrcTreangle_2s[c];
			int x0 = offsets[c]%32;
			for(int y = 0; y < heights[c]; y++){
				temp = src[y];
				temp >>= (x0 * 2);
				for(int x = 0; x<widths[c];x++){
					dst[y*widths[c]+x] = (temp & 0x3) * ((int*)valueC)[c];
					temp >>= 2;
				}
			}
			dst += heights[c] * widths[c];
			
		}
	}
	
	void mMulCVxN_2s_v4nm8sExt(nm2s** ppSrcTreangle_2s, int* offsets, int* widths, int* heights, v4nm8s* pDstTreangle, v4nm8s* valueC,  int count){
		mMulCVxN_2s_v4nm32sExt(ppSrcTreangle_2s, offsets, widths, heights, (nm32s*) pDstTreangle, (nm32s*) valueC, count);
	}

}