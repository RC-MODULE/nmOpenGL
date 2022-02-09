
extern "C"{
void findMinMax3(float* src1, float* src2, float* src3, float* dstMin, float* dstMax, int size){
	for(int i=0;i<size; i++){
		float min = src1[i];
		float max = src1[i];
		if (min > src2[i]) {
			min = src2[i];
		}
		if (min > src3[i]) {
			min = src3[i];
		}
		if (max < src2[i]) {
			max = src2[i];
		}
		if (max < src3[i]) {
			max = src3[i];
		}
		dstMin[i] = min;
		dstMax[i] = max;
	}
}

void findMinMax2(float* src1, float* src2, float* dstMin, float* dstMax, int size){
	for(int i=0;i<size; i++){
		if(src1[i] > src2[i]){
			dstMin[i] = src2[i];
			dstMax[i] = src1[i];
		} else {
			dstMin[i] = src1[i];
			dstMax[i] = src2[i];
		}
	}
}
}