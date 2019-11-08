#include "nmpp.h"


int currentDepthTest;

int* depthTestCore_N_rw;
int* depthTestCore_Lt_rw;
int* depthTestCore_Gt_rw;
int* depthTestCore_A_rw;

int* depthTestCore_N_r;
int* depthTestCore_Lt_r;
int* depthTestCore_Gt_r;
int* depthTestCore_A_r;

int addC4DepthTest;


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
										dst[x] = src1[x] - src2[x];
										if(dst[x]<0){
											src1[x] = src2[x];
										}
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
			//rtr = nmppsAddr_32s(rtr,pTriangsHeight[i] * pTriangsWidth[i]);
			//dst = nmppsAddr_32s(dst,pTriangsHeight[i] * pTriangsWidth[i]);
		}
	}
	
	void mSubV_32s_r( nm32s* pSrc1, int srcStride1, 
						nm32s* pSrc2, int srcStride2, 
						nm32s* pDst, int dstStride,
						int nHeight, int nWidth){ 
								for(int y=0;y<nHeight;y++){
									int* src1 = pSrc1 + y*srcStride1;
									int* src2 = pSrc2 + y*srcStride2;
									int* dst  = pDst  + y*dstStride;
									for(int x=0;x<nWidth;x++){
										dst[x] = src1[x] - src2[x];
									}
								}
								
								
							}
							
	void depthFunc_r(nm32s** pROI, int imageStride, nm32s* pTriangles, nm32s* pDst,  int* pTriangsHeight, int* pTriangsWidth, int count){
		nm32s* rtr = pTriangles;
		nm32s* dst = pDst;
		for (int i = 0; i < count; i++) {
			mSubV_32s_r(pROI[i],imageStride,rtr,pTriangsWidth[i],dst,pTriangsWidth[i],pTriangsHeight[i], pTriangsWidth[i]);
			rtr += pTriangsHeight[i] * pTriangsWidth[i];
			dst += pTriangsHeight[i] * pTriangsWidth[i];
			//rtr = nmppsAddr_32s(rtr,pTriangsHeight[i] * pTriangsWidth[i]);
			//dst = nmppsAddr_32s(dst,pTriangsHeight[i] * pTriangsWidth[i]);
		}
	}
	
}