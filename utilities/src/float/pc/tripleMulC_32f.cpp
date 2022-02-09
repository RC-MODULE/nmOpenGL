
extern "C"{
	
void tripleMulC_32f(float* src1, float* src2, float* src3, float C, float* dst1, float* dst2, float* dst3, int size){
	for(int i = 0; i < size; i++){
		dst1[i] = src1[i] * C;
		dst2[i] = src2[i] * C;
		dst3[i] = src3[i] * C;
	}
}

}