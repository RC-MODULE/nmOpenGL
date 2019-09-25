
extern "C"{
	void cnv32f_v2v4(float* srcVec, int stride, float value3, float value4, float* dstVec, int countVec){
		int i=0;
		int srcPoint = 0;
		stride++;
		for(i=0;i<countVec;i++){
			dstVec[4*i+0] = srcVec[2 * srcPoint + 0];
			dstVec[4*i+1] = srcVec[2 * srcPoint + 1];
			dstVec[4*i+2] = value3;
			dstVec[4*i+3] = value4;
			srcPoint += stride;
		}
	}
}