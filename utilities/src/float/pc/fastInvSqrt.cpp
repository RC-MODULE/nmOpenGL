


extern "C"{
	
void fastInvSqrt(float* srcVec, float* dstVec, int size){
	int* srcInt = (int*)srcVec;
	int* dstInt = (int*)dstVec;
	for(int i = 0; i < size; i++){
		dstInt[i] = 0x5F3759DF - (srcInt[i] >> 1);
		dstVec[i] *= (1.5 - 0.5 * srcVec[i] * dstVec[i] * dstVec[i]);
	}
}

}
