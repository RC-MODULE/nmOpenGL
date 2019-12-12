#include "nmpp.h"
#include "demo3d_nm1.h"

extern "C" {
	

	void depthTestCore_N_rw(nm32s &trianSrcZ, nm32s &buffZ, nm32s &trianDstZ) {
		trianDstZ = 0;
	}

	void depthTestCore_Lt_rw(nm32s &trianSrcZ, nm32s &buffZ, nm32s &trianDstZ) {
		trianDstZ = trianSrcZ - buffZ;
		if (trianDstZ < 0) {
			buffZ = trianSrcZ;
		}
	}
	void depthTestCore_Gt_rw(nm32s &trianSrcZ, nm32s &buffZ, nm32s &trianDstZ) {
		trianDstZ = buffZ - trianSrcZ;
		if (trianDstZ < 0) {
			buffZ = trianSrcZ;
		}
	}
	void depthTestCore_A_rw(nm32s &trianSrcZ, nm32s &buffZ, nm32s &trianDstZ) {
		buffZ = trianSrcZ;
		trianDstZ = -1;
	}

	void depthTestCore_N_r(nm32s &trianSrcZ, nm32s &buffZ, nm32s &trianDstZ) {
		trianDstZ = 0;
	}
	void depthTestCore_Lt_r(nm32s &trianSrcZ, nm32s &buffZ, nm32s &trianDstZ) {
		trianDstZ = trianSrcZ - buffZ;
	}
	void depthTestCore_Gt_r(nm32s &trianSrcZ, nm32s &buffZ, nm32s &trianDstZ) {
		trianDstZ = buffZ - trianSrcZ;
	}
	void depthTestCore_A_r(nm32s &trianSrcZ, nm32s &buffZ, nm32s &trianDstZ) {
		trianDstZ = -1;
	}

	DepthCore* currentDepthTest = depthTestCore_Lt_rw;
}


extern "C" {
	
	
		void mSubV_32s_w( nm32s* pSrc1, int srcStride1, 
						nm32s* pSrc2, int srcStride2, 
						nm32s* pDst, int dstStride,
						int nHeight, int nWidth){ 
								for(int y=0;y<nHeight;y++){
									int* src1 = pSrc1 + y*srcStride1;
									int* src2 = pSrc2 + y*srcStride2;
									int* dst  = pDst  + y*dstStride;
									for(int x=0;x<nWidth;x++){
										currentDepthTest(src1[x], src2[x], dst[x]);
									}
								}
								
								
							}
							
							
	void depthTest(nm32s** pROI, int imageStride, nm32s* pTriangles, nm32s* pDst,  int* pTriangsHeight, int* pTriangsWidth, int count){
		nm32s* rtr = pTriangles;
		nm32s* dst = pDst;
		for (int i = 0; i < count; i++) {
			mSubV_32s_w(pROI[i],imageStride,rtr,pTriangsWidth[i],dst,pTriangsWidth[i],pTriangsHeight[i], pTriangsWidth[i]);
			rtr += pTriangsHeight[i] * pTriangsWidth[i];
			dst += pTriangsHeight[i] * pTriangsWidth[i];
		}
	}
	
	
	
}