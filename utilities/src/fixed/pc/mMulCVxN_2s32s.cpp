#include "demo3d_nm1.h"

int addC4DepthTest = ZBUFF_MAX;

extern "C" {
	
	void mMulCVxN_2s32s(Pattern* patterns, 
		Vector2* ptrnPoints,
		Size* ptrnSizes,
		int* valueC, 
		nm32s* pDstTreangle,
		int count){
		long long int temp;
		nm32s* dst = pDstTreangle;
		for(int c=0;c<count;c++){
			nm64s* src = (nm64s*) (patterns + c);
			src += ptrnPoints[c].y;

			int width = ptrnSizes[c].width;

			if (ptrnPoints[c].x < 0) {
				width += ptrnPoints[c].x;
			}
			
			for(int y = 0; y < ptrnSizes[c].height; y++){
				temp = src[y];
				nm32s* pDst = dst + y * ptrnSizes[c].width;
				if (ptrnPoints[c].x < 0) {
					for(int i = 0; i > ptrnPoints[c].x; i--){
						*pDst = addC4DepthTest;
						pDst++;
					}
				}
				else {
					temp >>= (ptrnPoints[c].x * 2);
				}
				
				for(int x = 0; x < width; x++){
					int mul = ((temp & 0x3) * valueC[c]);
					int tmp = mul & 0x80000000;
					*pDst = (mul + addC4DepthTest) & 0x7FFFFFFF | tmp;
					pDst++;
					temp >>= 2;
				}
			}
			dst += ptrnSizes[c].height * ptrnSizes[c].width;
		}
	}

}