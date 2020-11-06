
extern "C"{

void abs_32f(float* src, float* dst, int size){
	for(int i=0;i<size; i++){
		if(src[i] < 0){
			dst[i] = -src[i];
		}
	}
}

}