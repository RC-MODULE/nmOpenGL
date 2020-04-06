#include "demo3d_nm1.h"

extern "C" {
	void mMulCVxN_2s_RGB8888(Pattern* patterns, Rectangle* windows, v4nm8s* valueC, nm32s* pDstTreangle,  int count){
		long long int temp;
		nm32s* dst = pDstTreangle;
		nm8s* colors = (nm8s*)valueC;
		for(int c=0;c<count;c++){
			nm64s* src = (nm64s*) (patterns + c);
			src += windows[c].y;

			int width = windows[c].width;

			if (windows[c].x < 0) {
				width += windows[c].x;
			}
			
			for(int y = 0; y < windows[c].height; y++){
				temp = src[y];
				nm8s* pDst = (nm8s*)(dst + y * windows[c].width);
				if (windows[c].x < 0) {
					for(int i = 0; i > windows[c].x; i--){
						pDst[0] = 0;
						pDst[1] = 0;
						pDst[2] = 0;
						pDst[3] = 0;
						pDst+=4;
					}
				}
				else {
					temp >>= (windows[c].x * 2);
				}
				
				for(int x = 0; x < width; x++){
					int mul = temp & 0x3;
					pDst[0] = (mul * colors[4 * c + 0] & 0xFF);
					pDst[1] = (mul * colors[4 * c + 1] & 0xFF);
					pDst[2] = (mul * colors[4 * c + 2] & 0xFF);
					pDst[3] = (mul * colors[4 * c + 3] & 0xFF);
					pDst += 4;
					temp >>= 2;
				}
			}
			dst += windows[c].height * windows[c].width;
		}
	}

}