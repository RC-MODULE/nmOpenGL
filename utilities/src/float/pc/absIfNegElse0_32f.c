
//вычисляет модуль если число отрицательное, иначе записывает ноль

void absIfNegElse0_32f(float* src, float* dst, int size){
	int i;
	for(i = 0; i < size; i++){
		dst[i] = (src[i] < 0) ?  -src[i] : 0;
	}
}
