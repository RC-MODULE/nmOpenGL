
extern "C"{
void findMax2(float* src1, float* src2, float* dstMax, int size){
	for(int i=0;i<size; i++){
		if(src1[i] > src2[i]){
			dstMax[i] = src1[i];
		} else {
			dstMax[i] = src2[i];
		}
	}
}
}