#include "nmpp.h"
extern "C" {
	void mergePtrnsAddr3(nm32s** ppSrcCA, nm32s** ppSrcCB, nm32s** ppSrcBA, int step, nm32s** ppDst, int count){
		int point=0;
		while(count > 0){
			int localSize = MIN(step, count);
			for(int i=0;i<localSize;i++){
				ppDst[3*point + i] = ppSrcCA[point + i];
			}
			for(int i=0;i<localSize;i++){
				ppDst[3*point + localSize + i] = ppSrcCB[point + i];
			}
			for(int i=0;i<localSize;i++){
				ppDst[3*point + 2 * localSize + i] = ppSrcBA[point + i];
			}
			point += step;
			count -= step;
		}
	}
}