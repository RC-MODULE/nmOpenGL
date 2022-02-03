#include "nmpp.h"

extern "C"{
void checkRectanglesOverlaps(v2nm32f* minXY, v2nm32f* maxXY, v2nm32f* upperRightLimit, v2nm32f* lowerLeftLimit, nm1* mask, int size){
	int* mask32 = (int*)mask;
	for (int i = 0, cnt = 0; cnt < size; i++, cnt += 32) {
		int localSize = MIN(32, size - cnt);
		int maskXLt, maskYLt, maskXGt, maskYGt;
		nmppsCmpLtC_v2nm32f(minXY + cnt, upperRightLimit, (nm1*)&maskXLt, (nm1*)&maskYLt, 1, localSize);
		nmppsCmpGtC_v2nm32f(maxXY + cnt, lowerLeftLimit, (nm1*)&maskXGt, (nm1*)&maskYGt, 1, localSize);
		mask32[i] = (maskXLt & maskYLt) & (maskXGt & maskYGt);
	}
}
}