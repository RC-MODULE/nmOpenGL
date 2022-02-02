
extern "C"{
void copyOddOffset(float* src, float* dst, int first, int size){
	for(int i = first; i < size + first; i++ ){
		dst[i] = src[i];
	}
}
}