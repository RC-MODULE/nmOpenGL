#include <iostream>
#include <cerrno>

#include "tests.h"
#include "nmtype.h"
#include "demo3d_common.h"  // rgb565 type
#include "nmplv/vSupport.h"
#include "malloc32.h"
#include "service.h"

// Performance testing
#include "time.h"

int convertABGR32_RGB565_singleInputMaxValue_SingleOutputMaxValue();
int convertABGR32_RGB565_singleInputMinValue_singleOutputMinValue();
int convertABGR32_RGB565_singleInputIntermediateValue_singleOutputIntermediateValue();
int convertABGR32_RGB565_4InputValues_4OutputValues();
int convertABGR32_RGB565_5InputValues_5OutputValues();
int convertABGR32_RGB565_6InputValues_6OutputValues();
int convertABGR32_RGB565_7InputValues_7OutputValues();
int convertABGR32_RGB565_13InputValues_13OutputValues();
int convertABGR32_RGB565_0_200_InputValues_0_200_OutputValues();

int convertABGR32_RGB565_nInputValues_nOutputValues(int n);

// Performance tests
clock_t convertABGR32_RGB565_perf_4InputValues_4OutputValues();

int main(int argc, char **argv)
{
    puts("convertABGR32_RGB565 tests: ");
    RUN_TEST(convertABGR32_RGB565_singleInputIntermediateValue_singleOutputIntermediateValue);
    RUN_TEST(convertABGR32_RGB565_singleInputMaxValue_SingleOutputMaxValue);
    RUN_TEST(convertABGR32_RGB565_singleInputMinValue_singleOutputMinValue);
    RUN_TEST(convertABGR32_RGB565_4InputValues_4OutputValues);
    RUN_TEST(convertABGR32_RGB565_5InputValues_5OutputValues);
    RUN_TEST(convertABGR32_RGB565_6InputValues_6OutputValues);
    RUN_TEST(convertABGR32_RGB565_7InputValues_7OutputValues);
    RUN_TEST(convertABGR32_RGB565_13InputValues_13OutputValues);
	RUN_TEST(convertABGR32_RGB565_0_200_InputValues_0_200_OutputValues);
	//RUN_ARG_TEST(convertABGR32_RGB565_nInputValues_nOutputValues, 30);

	clock_t dt;
	dt = convertABGR32_RGB565_perf_4InputValues_4OutputValues();

    puts("OK");
    return dt;
    return 0;
}


//@case Функция корректно обрабатывает n входных значений и не изменяет лишние данные в выходном массиве
	//@step Установить количество элементов во входном массиве равным n 
	//@step Задать количество untouched_count значений в конце выходного массива, которые не должны быть изменены функцией в процессе работы, равным 4
	//@step Задать количество processed_count элементов в выходном массиве, установленных функцией конвертации (processed_count - ближайшее справа число к n, кратное 4)
	//@step Создать входной массив цветов для хранения (processed_count + untouched_count) элементов в формате ABGR32.
	//@step Создать выходной массив цветов для хранения (processed_count + untouched_count) элементов.
	//@step Создать массив ожидаемых значений
	//@step Инициализировать входной массив с помощью следующих элементов: 0xAAAAAAAACCCCCCCC3333333355555555, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 0xFFFFFFFFA8000000D800000050000000, 0xFFFFFFFFB8000000E800000060000000
	//@step Инициализировать выходной массив нулевыми значениями.
	//@step Инициализировать массив ожидаемых значений с помощью следующих элементов: 0xAE6C, 0xFFFF, 0x2B08, 0x3518
	//@step Обнулить untouched_count последних элементов массива ожидаемых значений
	//@step Вызвать функцию convertABGR32_RGB565
	//@assert Сравнить массивы фактических и ожидаемых выходных значений
	//@pass Массивы фактических и ожидаемых значений, сравниваемые на шаге #assert, полностью совпадают
int convertABGR32_RGB565_nInputValues_nOutputValues(int n)
{
    // Arrange
    int count = n;
    int processed_count = count + (4 - count % 4) * ((count % 4) != 0); // Increase count up to the nearest multiple of 4
    int untouched_count = 4;

#ifdef __NM__
    abgr32 srcArray[processed_count + untouched_count];    
#else
    abgr32 *srcArray = (abgr32 *) calloc(sizeof(abgr32) * (processed_count + untouched_count), sizeof(abgr32));    
#endif	// __NM__

    abgr32 srcInit[4] = {    
	{0x3333333355555555, 0xAAAAAAAACCCCCCCC},
	{0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF},
	{0x000000D800000005, 0xFFFFFFFF000000A8},
	{0x000000E800000006, 0xFFFFFFFF000000B8},
    };

    rgb565 *dstArray = nmppsMalloc_16s(processed_count + untouched_count);

    rgb565 *expectedDstArray = nmppsMalloc_16s(processed_count + untouched_count);
    nm32u expectedRGB565Values[4] = {0xAE6C, 0xFFFF, 0x2B08, 0x3518};

    // The following arrays are used for comparison
#ifdef __NM__
    uint16b dstArray_uint16b[processed_count + untouched_count];
    uint16b expectedDstArray_uint16b[processed_count + untouched_count];
#else
    uint16b *dstArray_uint16b			 = (uint16b *) calloc(sizeof(uint16b) * (processed_count + untouched_count), sizeof(uint16b));
    uint16b *expectedDstArray_uint16b	 = (uint16b *) calloc(sizeof(uint16b) * (processed_count + untouched_count), sizeof(uint16b));
#endif	// __NM__

    for (int i = 0; i < processed_count + untouched_count; ++i){
	    srcArray[i].vec[0] = srcInit[i % 4].vec[0];
	    srcArray[i].vec[1] = srcInit[i % 4].vec[1];
    	nmppsPut_16s(dstArray, i, 0);

        nmppsPut_16s(expectedDstArray, i, expectedRGB565Values[i % 4]);
    }

    for (int i = processed_count; i < processed_count + untouched_count; ++i){
        nmppsPut_16s(expectedDstArray, i, 0);
    }

    //Act
    convertABGR32_RGB565(srcArray, dstArray, count);

    //Assert
    // Get the elements from the dstArray and expectedDstArray, 
    // convert them to nm32u for better comparison
    for(int i = 0; i < processed_count + untouched_count; ++i){
		nmppsGetVal_16u((nm16u *) dstArray, i, &(dstArray_uint16b[i]));
		nmppsGetVal_16u((nm16u *) expectedDstArray, i, &expectedDstArray_uint16b[i]);
    }

    TEST_ARRAYS_EQUALI(((nm32u *) dstArray_uint16b), 
			((nm32u *) expectedDstArray_uint16b), 
			processed_count + untouched_count,
			"args: %i", n);

    nmppsFree(dstArray);
    nmppsFree(expectedDstArray);

    return 0;
}

int convertABGR32_RGB565_0_200_InputValues_0_200_OutputValues()
{
	int res = 0;
	for (int i = 0; i < 10; ++i){
    	res += convertABGR32_RGB565_nInputValues_nOutputValues(i);
	}
	return res;
}

//@case Функция корректно преобразует одно максимальное значение цвета в представлении ABGR32
	//@step Установить количество элементов во входном массиве равным 1.
	//@step Первому элементу входного массива присвоить максимально возможное значение цвета 0xFFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF.
	//@step Создать выходной массив цветов для хранения одного значения цвета.
	//@step Вызвать функцию convertABGR32_RGB565.
	//@assert Проверить значение первого элемента в выходном массиве.
	//@pass Первый элемент выходного массива содержит значение 0xFFFF в представлении RGB565.
int convertABGR32_RGB565_singleInputMaxValue_SingleOutputMaxValue()
{
    // Arrange
    constexpr int count = 1;
    abgr32 *srcArray = new abgr32[count];    
    rgb565 *dstArray = nmppsMalloc_16s(count);
    rgb565 *expectedDstArray = nmppsMalloc_16s(count);
    // The following arrays are used for comparison
    int16b dstArray_int16b[count];
    int16b expectedDstArray_int16b[count];

    srcArray[0].vec[0] = 0xFFFFFFFFFFFFFFFF;
    srcArray[0].vec[1] = 0xFFFFFFFFFFFFFFFF;     
    nmppsPut_16s(expectedDstArray, 0, 0xFFFF);

    //Act
    convertABGR32_RGB565(srcArray, dstArray, count);

    //Assert
    // Get the elements from the dstArray and expectedDstArray, 
    // convert them to nm32u for better comparison
    for(int i = 0; i < count; ++i){
        nmppsGetVal_16s(dstArray, i, &dstArray_int16b[i]);
        nmppsGetVal_16s(expectedDstArray, i, &expectedDstArray_int16b[i]);
    }
    TEST_ARRAYS_EQUALI(((nm32u *) dstArray_int16b), ((nm32u *) expectedDstArray_int16b), count);
    
    delete srcArray;
    nmppsFree(dstArray);
    nmppsFree(expectedDstArray);

    return 0;
}

//@case Функция корректно преобразует одно минимальное значение цвета в представлении ABGR32
	//@step Установить количество элементов во входном массиве равным 1.
	//@step Первому элементу входного массива присвоить минимально возможное значение цвета 0.
	//@step Создать выходной массив цветов для хранения одного значения цвета.
	//@step Вызвать функцию convertABGR32_RGB565.
	//@assert Проверить значение первого элемента в выходном массиве.
	//@pass Первый элемент выходного массива содержит значение 0 в представлении RGB565.
int convertABGR32_RGB565_singleInputMinValue_singleOutputMinValue()
{
    // Arrange
    constexpr int count = 1;
    abgr32 *srcArray = new abgr32[count];    
    rgb565 *dstArray = nmppsMalloc_16s(count);
    rgb565 *expectedDstArray = nmppsMalloc_16s(count);
    // The following arrays are used for comparison
    int16b dstArray_int16b[count];
    int16b expectedDstArray_int16b[count];

    srcArray[0].vec[0] = 0;
    srcArray[0].vec[1] = 0; 
    nmppsPut_16s(expectedDstArray, 0, 0);
    
    //Act
    convertABGR32_RGB565(srcArray, dstArray, count);

    //Assert
    // Get the elements from the dstArray and expectedDstArray, 
    // convert them to nm32u for better comparison
    for(int i = 0; i < count; ++i){
        nmppsGetVal_16s(dstArray, i, &dstArray_int16b[i]);
        nmppsGetVal_16s(expectedDstArray, i, &expectedDstArray_int16b[i]);
    }
    TEST_ARRAYS_EQUALI(((nm32u *) dstArray_int16b), ((nm32u *) expectedDstArray_int16b), count);

    delete srcArray;
    nmppsFree(dstArray);
    nmppsFree(expectedDstArray);

    return 0;
}

//@case Функция корректно преобразует одно промежуточное значение цвета в представлении ABGR32
//@step Установить количество элементов во входном массиве равным 1.
//@step Первому элементу входного массива присвоить промежуточное значение цвета 0xAAAAAAAACCCCCCCC3333333355555555.
//@step Создать выходной массив цветов для хранения одного значения цвета.
//@step Вызвать функцию convertABGR32_RGB565.
//@assert Проверить значение первого элемента в выходном массиве.
//@pass Первый элемент выходного массива содержит значение 0xAE6C в представлении RGB565
int convertABGR32_RGB565_singleInputIntermediateValue_singleOutputIntermediateValue()
{
    // Arrange
    constexpr int count = 1;
    abgr32 *srcArray = new abgr32[count];    
    rgb565 *dstArray = nmppsMalloc_16s(count);
    rgb565 *expectedDstArray = nmppsMalloc_16s(count);
    // The following arrays are used for comparison
    int16b dstArray_int16b[count];
    int16b expectedDstArray_int16b[count];

    srcArray[0].vec[0] = 0x3333333355555555;
    srcArray[0].vec[1] = 0xAAAAAAAACCCCCCCC;
    nmppsPut_16s(expectedDstArray, 0, 0xAE6C);
    
    //Act
    convertABGR32_RGB565(srcArray, dstArray, count);

    //Assert
    // Get the elements from the dstArray and expectedDstArray, 
    // convert them to nm32u for better comparison
    for(int i = 0; i < count; ++i){
        nmppsGetVal_16s(dstArray, i, &dstArray_int16b[i]);
        nmppsGetVal_16s(expectedDstArray, i, &expectedDstArray_int16b[i]);
    }
    TEST_ARRAYS_EQUALI(((nm32u *) dstArray_int16b), ((nm32u *) expectedDstArray_int16b), count);

    delete srcArray;
    nmppsFree(dstArray);
    nmppsFree(expectedDstArray);

    return 0;
}

int convertABGR32_RGB565_4InputValues_4OutputValues()
{
    return convertABGR32_RGB565_nInputValues_nOutputValues(4);
}

int convertABGR32_RGB565_5InputValues_5OutputValues()
{
    return convertABGR32_RGB565_nInputValues_nOutputValues(5);
}

int convertABGR32_RGB565_6InputValues_6OutputValues()
{
    return convertABGR32_RGB565_nInputValues_nOutputValues(6);
}

int convertABGR32_RGB565_7InputValues_7OutputValues()
{
    return convertABGR32_RGB565_nInputValues_nOutputValues(7);
}

int convertABGR32_RGB565_13InputValues_13OutputValues()
{
	return convertABGR32_RGB565_nInputValues_nOutputValues(13);
}

clock_t convertABGR32_RGB565_perf_4InputValues_4OutputValues()
{
    // Arrange
    constexpr int count = 4;
    abgr32 *srcArray = new abgr32[count];    
    rgb565 *dstArray = nmppsMalloc_16s(count);

    srcArray[0].vec[0] = 0x3333333355555555;
    srcArray[0].vec[1] = 0xAAAAAAAACCCCCCCC;

    srcArray[1].vec[0] = 0xFFFFFFFFFFFFFFFF;
    srcArray[1].vec[1] = 0xFFFFFFFFFFFFFFFF; 

    srcArray[2].vec[0] = 0x000000D800000005;
    srcArray[2].vec[1] = 0xFFFFFFFF000000A8; 

    srcArray[3].vec[0] = 0x000000E800000006;
    srcArray[3].vec[1] = 0xFFFFFFFF000000B8; 

	clock_t t1, t2, dt;

    //Act
	t1 = clock();
    convertABGR32_RGB565(srcArray, dstArray, count);
	t2 = clock();
	dt = t2 - t1;

    //Assert

    delete srcArray;
    nmppsFree(dstArray);

    return dt;
}
