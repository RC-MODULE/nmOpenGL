
extern "C"{
	
void doubleSub_32f(float* src1, float* src2, float* srcSub1, float* srcSub2, float* dst1, float* dst2, int size){
	for(int i = 0; i < size; i++){
		dst1[i] = src1[i] - srcSub1[i];
		dst2[i] = src2[i] - srcSub2[i];
	}
}

}