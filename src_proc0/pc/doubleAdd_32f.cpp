
extern "C"{
	
void doubleAdd_32f(float* src1, float* src2, float* srcAdd1, float* srcAdd2, float* dst1, float* dst2, int size){
	for(int i = 0; i < size; i++){
		dst1[i] = src1[i] + srcAdd1[i];
		dst2[i] = src2[i] + srcAdd2[i];
	}
}

}