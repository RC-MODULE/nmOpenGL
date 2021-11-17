#include <iostream>
#include <cerrno>

#include "nmtype.h"
#include "demo3d_common.h"
#include "malloc32.h"
#include "service.h"
#include "nmplv/vSupport.h"
#include "tests.h"

// Performance testing
#include "time.h"

int convertRGB565_RGB8888_4InputMinValues_4OutputMinValues();
int convertRGB565_RGB8888_4InputMaxValues_4OutputMaxValues();
int convertRGB565_RGB8888_0_2048_InputValues_0_2048_OutputValues();

int convertRGB565_RGB8888_nInputValues_nOutputValues(int n);

// Performance tests
clock_t convertRGB565_RGB8888_perf_4InputValues_4OutputValues();

int main(int argc, char **argv)
{
    puts("convertRGB565_RGB8888 tests: ");
    RUN_TEST(convertRGB565_RGB8888_4InputMinValues_4OutputMinValues);
    RUN_TEST(convertRGB565_RGB8888_4InputMaxValues_4OutputMaxValues);
    RUN_TEST(convertRGB565_RGB8888_0_2048_InputValues_0_2048_OutputValues);     
	//RUN_ARG_TEST(convertRGB565_RGB8888_nInputValues_nOutputValues, 256);

	clock_t dt;
	dt = convertRGB565_RGB8888_perf_4InputValues_4OutputValues();
	
    puts("OK");

    return dt;
}

//@case Функция корректно обрабатывает n входных значений и не изменяет лишние данные в выходном массиве
	//@step Установить количество элементов во входном массиве равным n 
	//@step Задать количество untouched_count значений в конце выходного массива, которые не должны быть изменены функцией в процессе работы, равным 4
	//@step Задать количество processed_count элементов в выходном массиве, установленных функцией конвертации (processed_count - ближайшее слева число к n, кратное 4)
	//@step Создать входной массив цветов для хранения (processed_count + untouched_count) элементов в формате RGB565.
	//@step Создать выходной массив цветов для хранения (processed_count + untouched_count) элементов.
	//@step Создать массив ожидаемых значений
	//@step Инициализировать входной массив с помощью следующих элементов: 0xC979, 0xFFFF, 0xAAAA, 0x5555
	//@step Инициализировать выходной массив нулевыми значениями.
	//@step Инициализировать массив ожидаемых значений с помощью следующих элементов: 0x00CE2CCE, 0x00FFFFFF, 0x00AD5552, 0x0052AAAD
	//@step Обнулить untouched_count последних элементов массива ожидаемых значений
	//@step Вызвать функцию convertRGB565_RGB8888
	//@assert Сравнить массивы фактических и ожидаемых выходных значений
	//@pass Массивы фактических и ожидаемых значений, сравниваемые на шаге #assert, полностью совпадают
int convertRGB565_RGB8888_nInputValues_nOutputValues(int n)
{
    //Arrange
	int count = n;
    int processed_count = (count >> 2) << 2;	// nearest left multiple of 4
    int untouched_count = 4; 
    rgb565 *srcArray = nmppsMalloc_16s(processed_count + untouched_count);
    nm32u RGB565Values[4] = {0xC979, 0xFFFF, 0xAAAA, 0x5555};

    // The following arrays are used for comparison
#ifdef __NM__
    nm32u 			   dst[processed_count + untouched_count];
    nm32u expectedDstArray[processed_count + untouched_count];
#else
    nm32u 			   *dst = (nm32u *) calloc(sizeof(nm32u) * (processed_count + untouched_count), sizeof(nm32u));
    nm32u *expectedDstArray = (nm32u *) calloc(sizeof(nm32u) * (processed_count + untouched_count), sizeof(nm32u));
#endif	// __NM__

    rgb8888        *dstArray = (rgb8888 *) dst;
    nm32u   RGB8888Values[4] = {0x00CE2CCE, 0x00FFFFFF, 0x00AD5552, 0x0052AAAD};
	
    for (int i = 0; i < processed_count + untouched_count; ++i) {
        nmppsPut_16s(srcArray, i, RGB565Values[i % 4]);
        nmppsPut_32s((nm32s *) dstArray, i, 0);
        expectedDstArray[i] = RGB8888Values[i % 4];
    }

    for (int i = processed_count; i < processed_count + untouched_count; ++i){
        expectedDstArray[i] = 0;
    }

    //Act
    convertRGB565_RGB8888(srcArray, dstArray, count);

    //Assert
    TEST_ARRAYS_EQUALI(((nm32u *) dstArray), expectedDstArray, processed_count + untouched_count, "arg is %i", count);

	nmppsFree(srcArray);

    return 0;
}

int convertRGB565_RGB8888_0_2048_InputValues_0_2048_OutputValues()
{
	int res = 0;
	for (int i = 0; i <= 2048; ++i){
    	res += convertRGB565_RGB8888_nInputValues_nOutputValues(i);
	}
	return res;
}


//@case Функция корректно преобразует 4 минимальных значения цвета, заданных в формате RGB565; функция не изменяет лишние данные в выходном массиве
	//@step Установить количество элементов во входном массиве равным 4
	//@step Создать массив входных элементов, содержащий 8 элементов цвета в формате RGB565
	//@step Создать выходной массив фактических значений для хранения 8 элементов в формате RGB8888
	//@step Создать массив ожидаемых значений цвета: 0, 0, 0, 0, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF
	//@step Инициализировать массив входных элементов нулевыми значениями
	//@step Инициализировать каждый элемент выходного массива значением 0x00FFFFFF
	//@step Вызвать функцию convertRGB565_RGB8888 для преобразования 4 элементов входного массива
	//@assert Сравнить массивы фактических и ожидаемых значений
	//@pass Массивы фактических и ожидаемых значений, сравниваемые на шаге #assert, полностью совпадают
int convertRGB565_RGB8888_4InputMinValues_4OutputMinValues()
{
    //Arrange
    constexpr int count = 4;
    rgb565 *srcArray = nmppsMalloc_16s(2 * count);
    rgb8888 *dstArray = (rgb8888 *) nmppsMalloc_32s(2 * count);
    nm32u expectedDstArray[2 * count] = {	0, 
											0,
											0, 
											0, 
											0x00FFFFFF,
											0x00FFFFFF,
											0x00FFFFFF,
											0x00FFFFFF
										};
    for (int i = 0; i < 2 * count; ++i) {
        nmppsPut_16s(srcArray, i, 0);
        nmppsPut_32s((nm32s *) dstArray, i, 0x00FFFFFF);
    }

    //Act
    convertRGB565_RGB8888(srcArray, dstArray, count);

    //Assert
    TEST_ARRAYS_EQUALI(((nm32u *) dstArray), expectedDstArray, 2 * count);

	nmppsFree(srcArray);
	nmppsFree(dstArray);

    return 0;
}

//@case Функция корректно преобразует 4 максимальных значения цвета, заданных в формате RGB565; функция не изменяет лишние данные в выходном массиве
	//@step Установить количество элементов во входном массиве равным 4
	//@step Создать массив входных элементов, содержащий 8 элементов цвета в формате RGB565
	//@step Создать выходной массив фактических значений для хранения 8 элементов в формате RGB8888
	//@step Создать массив ожидаемых значений цвета: 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0, 0, 0, 0, 
	//@step Инициализировать каждый элемент входного массива значением 0x00FFFFFF
	//@step Инициализировать выходной массив нулевыми значениями
	//@step Вызвать функцию convertRGB565_RGB8888 для преобразования 4 элементов входного массива
	//@assert Сравнить массивы фактических и ожидаемых значений
	//@pass Массивы фактических и ожидаемых значений, сравниваемые на шаге #assert, полностью совпадают
int convertRGB565_RGB8888_4InputMaxValues_4OutputMaxValues()
{
    //Arrange
    constexpr int count = 4;
    rgb565 *srcArray = nmppsMalloc_16s(2 * count);
    rgb8888 *dstArray = (rgb8888 *) nmppsMalloc_32s(2 * count);
    nm32u expectedDstArray[2 * count] = {
											0x00FFFFFF, 
											0x00FFFFFF, 
											0x00FFFFFF, 
											0x00FFFFFF, 
											0, 
											0, 
											0, 
											0
										};

    for (int i = 0; i < 2 * count; ++i) {
        nmppsPut_16s(srcArray, i, 0xFFFF);
        nmppsPut_32s((nm32s *) dstArray, i, 0);
    }

    //Act
    convertRGB565_RGB8888(srcArray, dstArray, count);

    //Assert
    TEST_ARRAYS_EQUALI(((nm32u *) dstArray), expectedDstArray, 2 * count);

	nmppsFree(srcArray);
	nmppsFree(dstArray);

    return 0;
}

clock_t convertRGB565_RGB8888_perf_4InputValues_4OutputValues()
{
    //Arrange
    constexpr int count = 4;
    rgb565 *srcArray = nmppsMalloc_16s(count);
    nm32u RGB565Values[count] = {0xC979, 0xFFFF, 0xAAAA, 0x5555};
    rgb8888 *dstArray = (rgb8888 *) nmppsMalloc_32s(count);

    for (int i = 0; i < count; ++i) {
        nmppsPut_16s(srcArray, i, RGB565Values[i]);
        nmppsPut_32s((nm32s *) dstArray, i, 0);
    }

	clock_t t1, t2, dt;

    //Act
	t1 = clock();
    convertRGB565_RGB8888(srcArray, dstArray, count);
	t2 = clock();
	dt = t2 - t1;

	nmppsFree(srcArray);
	delete dstArray;

    return dt;
}
