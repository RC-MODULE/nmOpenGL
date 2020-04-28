#include "demo3d_nm1.h"

extern "C" {
	void mMulCVxN_2s_RGB565(Pattern* patterns, 
		Vector2* ptrnPoints,
		Size* ptrnSizes,
		int* valueC, 
		nm16s* pDstTreangle,  
		int count){
		long long int temp;
		nm16s* dst = pDstTreangle;
		nm32s* colors = (nm32s*)valueC;
		for(int c=0;c<count;c++){
			nm64s* src = (nm64s*) (patterns + c);
			src += ptrnPoints[c].y;

			int width = ptrnSizes[c].width;

			if (ptrnPoints[c].x < 0) {
				width += ptrnPoints[c].x;
			}
			
			for(int y = 0; y < ptrnSizes[c].height; y++){
				temp = src[y];
				nm16s* pDst = dst + y * ptrnSizes[c].width;
				if (ptrnPoints[c].x < 0) {
					for(int i = 0; i > ptrnPoints[c].x; i--){
						pDst[0] = 0;
						pDst++;
					}
				}
				else {
					temp >>= (ptrnPoints[c].x * 2);
				}
				
				for(int x = 0; x < width; x++){
					int mul = temp & 0x3;
					int red = (colors[c] >> 11) & 0x1F;
					int green = (colors[c] >> 5) & 0x3F;
					int blue = (colors[c]) & 0x1F;
					red = (red * mul) & 0x1F;
					green = (green * mul) & 0x3F;
					blue = (blue * mul) & 0x1F;
					int result = (red << 11) | (green << 5) | blue;
					pDst[0] = result;
					pDst++;
					temp >>= 2;
				}
			}
			dst += ptrnSizes[c].height * ptrnSizes[c].width;
		}
	}

}