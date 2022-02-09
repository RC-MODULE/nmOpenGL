
extern "C"{
	
void doubleSubC_32f(float* src1, float* src2, float C1, float C2, float* dst1, float* dst2, int size){
	for(int i = 0; i < size; i++){
		dst1[i] = src1[i] - C1;
		dst2[i] = src2[i] - C2;
	}
}

}