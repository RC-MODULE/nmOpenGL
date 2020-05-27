#include "nmpp.h"
#include "demo3d_nm1.h"

extern "C" {
	

	void depthTestCore16_N_rw(nm16s &buffZ, nm16s &trianSrcZ, nm16s &trianDstZ) {
		trianDstZ = 0;
	}

	void depthTestCore16_Lt_rw(nm16s &buffZ, nm16s &trianSrcZ, nm16s &trianDstZ) {
		trianDstZ = trianSrcZ - buffZ;
		if (trianDstZ < 0) {
			buffZ = trianSrcZ;
		}
	}
	void depthTestCore16_Gt_rw(nm16s &buffZ, nm16s &trianSrcZ, nm16s &trianDstZ) {
		trianDstZ = buffZ - trianSrcZ;
		if (trianDstZ < 0) {
			buffZ = trianSrcZ;
		}
	}
	void depthTestCore16_A_rw(nm16s &buffZ, nm16s &trianSrcZ, nm16s &trianDstZ) {
		buffZ = trianSrcZ;
		trianDstZ = -1;
	}

	void depthTestCore16_N_r(nm16s &buffZ, nm16s &trianSrcZ, nm16s &trianDstZ) {
		trianDstZ = 0;
	}
	void depthTestCore16_Lt_r(nm16s &buffZ, nm16s &trianSrcZ, nm16s &trianDstZ) {
		trianDstZ = trianSrcZ - buffZ;
	}
	void depthTestCore16_Gt_r(nm16s &buffZ, nm16s &trianSrcZ, nm16s &trianDstZ) {
		trianDstZ = buffZ - trianSrcZ;
	}
	void depthTestCore16_A_r(nm16s &buffZ, nm16s &trianSrcZ, nm16s &trianDstZ) {
		trianDstZ = -1;
	}

	DepthCore16* currentDepthTest16 = depthTestCore16_Lt_rw;
}


extern "C" {
							
	
	void depthTest16(nm16s** pROI,
		int imageStride, 
		nm16s* depthTrians, 
		nm16s* dstMask,  
		Size* trianSizes, 
		int count){

		for (int c = 0; c < count; c++) {
			nm16s* pointInImage = pROI[c];
			nm16s* src1 = pointInImage;
			nm16s* src2 = depthTrians;
			nm16s* dst = dstMask;

			for (int y = 0; y < trianSizes[c].height; y++) {
				for (int x = 0; x <  trianSizes[c].width; x++) {
					currentDepthTest16(src1[x], src2[x], dst[x]);
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