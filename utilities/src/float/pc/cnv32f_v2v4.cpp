
extern "C"{
	void cnv32f_v2v4(float* srcVec, float* dstVec, float value3, float value4, int countVec){
		int i=0;
		for(i=0;i<countVec;i++){
			dstVec[4 * i + 0] = srcVec[2 * i + 0];
			dstVec[4 * i + 1] = srcVec[2 * i + 1];
			dstVec[4 * i + 2] = value3;
			dstVec[4 * i + 3] = value4;
		}
	}
}