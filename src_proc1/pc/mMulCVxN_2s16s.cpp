#include "demo3d_nm1.h"

int addC4DepthTest_15s = ZBUFF_MAX_15s;

extern "C" {
	
	void mMulCVxN_2s16s(Pattern* patterns, Rectangle* windows, int* valueC, nm16s* pDstTreangle,  int count){
		long long int temp;
		short* dst = pDstTreangle;
		for(int c=0;c<count;c++){
			long long int* src = (nm64s*) patterns[c];
			src += windows[c].y;
			int x0;
			int width = windows[c].width;

			if (windows[c].x < 0) {
				width += windows[c].x;
			}
			
			for(int y = 0; y < windows[c].height; y++){
				temp = src[y];
				nm16s* pDst = dst + y * windows[c].width;
				if (windows[c].x < 0) {
					for(int i = 0; i > windows[c].x; i--){
						*pDst = addC4DepthTest_15s;
						pDst++;
					}
				}
				else {
					temp >>= (windows[c].x * 2);
				}
				for(int x = 0; x < width; x++){
					int mul = ((temp & 0x3) * valueC[c]);
					int tmp = mul & 0x8000;
					*pDst = (mul + addC4DepthTest_15s) & 0x7FFF | tmp;
					pDst++;
					temp >>= 2;
				}
			}
			dst += windows[c].height * windows[c].width;
			
		}
	}

}