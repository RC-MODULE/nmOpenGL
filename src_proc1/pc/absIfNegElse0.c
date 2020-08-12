
//вычисляет модуль если число отрицательное, иначе записывает ноль

void absIfNegElse0(int* src, int* dst, int size){
	int i;
	for(i = 0; i < size; i++){
		dst[i] = (src[i] < 0) ?  -src[i] : 0;
	}
}
