#include "nmpp.h"
#include "demo3d_nm1.h"

extern "C" {
	

	void depthTestCore_N_rw(nm32s &buffZ, nm32s &trianSrcZ, nm32s &trianDstZ) {
		trianDstZ = 0;
	}

	void depthTestCore_Lt_rw(nm32s &buffZ, nm32s &trianSrcZ, nm32s &trianDstZ) {
		trianDstZ = trianSrcZ - buffZ;
		if (trianDstZ < 0) {
			buffZ = trianSrcZ;
		}
	}
	void depthTestCore_Gt_rw(nm32s &buffZ, nm32s &trianSrcZ, nm32s &trianDstZ) {
		trianDstZ = buffZ - trianSrcZ;
		if (trianDstZ < 0) {
			buffZ = trianSrcZ;
		}
	}
	void depthTestCore_A_rw(nm32s &buffZ, nm32s &trianSrcZ, nm32s &trianDstZ) {
		buffZ = trianSrcZ;
		trianDstZ = -1;
	}

	void depthTestCore_N_r(nm32s &buffZ, nm32s &trianSrcZ, nm32s &trianDstZ) {
		trianDstZ = 0;
	}
	void depthTestCore_Lt_r(nm32s &buffZ, nm32s &trianSrcZ, nm32s &trianDstZ) {
		trianDstZ = trianSrcZ - buffZ;
	}
	void depthTestCore_Gt_r(nm32s &buffZ, nm32s &trianSrcZ, nm32s &trianDstZ) {
		trianDstZ = buffZ - trianSrcZ;
	}
	void depthTestCore_A_r(nm32s &buffZ, nm32s &trianSrcZ, nm32s &trianDstZ) {
		trianDstZ = -1;
	}

	DepthCore* currentDepthTest = depthTestCore_Lt_rw;
}


extern "C" {
							
							
	void depthTest_32s(nm32s** pROI,
		int imageStride, 
		nm32s* depthTrians, 
		nm32s* dstMask,  
		Size* trianSizes, 
		int count){

		for (int c = 0; c < count; c++) {
			nm32s* pointInImage = pROI[c];
			nm32s* src1 = pointInImage;
			nm32s* src2 = depthTrians;
			nm32s* dst = dstMask;

			for (int y = 0; y < trianSizes[c].height; y++) {
				for (int x = 0; x <  trianSizes[c].width; x++) {
					currentDepthTest(src1[x], src2[x], dst[x]);
				}
				src1 += imageStride;
				src2 += trianSizes[c].width;
				dst += trianSizes[c].width;
			}

			depthTrians += trianSizes[c].width * trianSizes[c].height;
			dstMask += trianSizes[c].width * trianSizes[c].height;

		}
		
	}	
	
	
}