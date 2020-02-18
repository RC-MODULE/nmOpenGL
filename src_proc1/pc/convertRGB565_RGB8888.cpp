#include <cerrno>
#include <iostream>

#include "nmplv/vSupport.h"
#include "service.h"

void convertRGB565_RGB8888(const rgb565 *srcArray, rgb8888 *dstArray, int count)
{

	if ((srcArray == NULL) | (dstArray == NULL) | (count <= 0))
	{
		errno = EINVAL;
		return;
	}

	for (int i = 0; i < count; i++)
	{
		nm32u val;
		nm64u tempRes;
		nmppsGetVal_16s(srcArray, i, (int16b *) &val);
		//B
		tempRes = ((val << 3) & 0xF8) | ((val >>  2) & 0x7);
		//G
		tempRes = (tempRes & 0xFF) | ((val << 5) & 0xFC00) | ((val >> 1) & 0x300);
		//R
		tempRes = (tempRes & 0xFFFF) | ((val << 8) & 0xF80000) | ((val << 3) & 0x70000);
		
		// Одно выходное слово содержит 2 значения RGB8888
		// 1. Выяснить, в какое слово положить новое значение
		int vecNum = i / 2;
		// 2. Определить смещение внутри слова
		int vecOffset = 32 * (i & 1); // vec[0].high or vec[0].low
		// 3. Прочитать старое значение
		nm64u oldVal = dstArray[vecNum].vec[0];
		// 4. Обнулить часть слова для нового значения
		oldVal &= ~(((unsigned long long) 0xFFFFFFFF) << vecOffset);
		// 5. Записать новое значение в слово
		oldVal |= tempRes << vecOffset;
		// 6. Записать обновлённое значение в выходной массив
		dstArray[vecNum].vec[0] = oldVal;
	}

}
