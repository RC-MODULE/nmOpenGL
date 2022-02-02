

void absIfNegElse0(int* src, int* dst, int size){
	for(int i=0; i<size; i++){
		if(src[i] < 0){
			dst[i] = -src[i];
		} else{
			dst[i] = 0;
		}
	}
}