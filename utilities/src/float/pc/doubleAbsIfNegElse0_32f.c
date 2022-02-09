
//вычисляет модуль если число отрицательное, иначе записывает ноль
#ifdef __NM__
extern "C" {
#endif
void doubleAbsIfNegElse0_32f(float* src1, float* src2, float* dst1, float* dst2, int size){
	int i;
	for(i = 0; i < size; i++){
		dst1[i] = (src1[i] < 0) ?  -src1[i] : 0;
		dst2[i] = (src2[i] < 0) ?  -src2[i] : 0;
	}
}
#ifdef __NM__
}
#endif
