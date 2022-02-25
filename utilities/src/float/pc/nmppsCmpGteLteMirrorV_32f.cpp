#include "nmplv/vSupport.h"
#include "utility_float.h"

void nmppsCmpGteLteMirrorV_32f(const nm32f *srcArray, const nm32f *wArray, nm1* evenFlags, nm1* oddFlags, int size)
{
	int numOfFlags = size / 2; // Количество чётных и нечётных флагов
	
	// Количество полных int'ов для хранения флагов
	// Целая часть + 1, если есть остаток от деления на 32
	int numOfInts;
	numOfInts = numOfFlags / 32; 
	numOfInts += (numOfFlags % 32 != 0)? 1: 0;
	
	// Clear all flags
	int *intPtrEven = (int *) evenFlags;
	int *intPtrOdd = (int *) oddFlags;
	for (int i = 0; i < numOfInts; ++i){
		intPtrEven[i] = 0;
		intPtrOdd[i] = 0;
	}	
	// Set the flags
	for (int i = 0; i < size / 2; ++i){
	
		// Проверить чётные элементы
		if ((srcArray[2 * i] >= (-wArray[2 * i])) && 
		    (srcArray[2 * i] <= (wArray[2 * i]))) {
			nmppsPut_1(evenFlags, i, 1);
		} else {
			nmppsPut_1(evenFlags, i, 0);
		}
		
		// Проверить нечётные элементы
		if ((srcArray[2 * i + 1] >= (-wArray[2 * i + 1])) && 
		    (srcArray[2 * i + 1] <= (wArray[2 * i + 1]))) {
			nmppsPut_1(oddFlags, i, 1);
		} else {
			nmppsPut_1(oddFlags, i, 0);
		}
	}
}
