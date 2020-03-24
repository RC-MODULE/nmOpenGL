
extern "C"{
void merge_v4nm32s(int* src1, int* src2, int* src3, int* src4, int* dst, int size){
	for(int i = 0; i < size; i++){
		dst[4 * i + 0] = src1[i];
		dst[4 * i + 1] = src2[i];
		dst[4 * i + 2] = src3[i];
		dst[4 * i + 3] = src4[i];
	}
}
}