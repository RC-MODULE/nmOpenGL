#include <iostream>
#include <cerrno>

#include "nmtype.h"
#include "demo3d_common.h"
#include "malloc32.h"
#include "service.h"
#include "nmplv/vSupport.h"
#include "tests.h"

int convertRGB565_RGB8888_singleInputIntermediateValue_singleOutputIntermediateValue();
int convertRGB565_RGB8888_singleInputMaxValue_SingleOutputMaxValue();
int convertRGB565_RGB8888_singleInputMinValue_singleOutputMinValue();
int convertRGB565_RGB8888_manyInputValues_manyOutputValues();
int convertRGB565_RGB8888_FiveInputValues_FiveOutputValues();
int convertRGB565_RGB8888_SixInputValues_SixOutputValues();
int convertRGB565_RGB8888_SevenInputValues_SevenOutputValues();

int main(int argc, char **argv)
{
    puts("convertRGB565_RGB8888 tests: ");
    RUN_TEST(convertRGB565_RGB8888_singleInputIntermediateValue_singleOutputIntermediateValue);
    RUN_TEST(convertRGB565_RGB8888_singleInputMaxValue_SingleOutputMaxValue);
    RUN_TEST(convertRGB565_RGB8888_singleInputMinValue_singleOutputMinValue);
    RUN_TEST(convertRGB565_RGB8888_manyInputValues_manyOutputValues);
    RUN_TEST(convertRGB565_RGB8888_FiveInputValues_FiveOutputValues);       
    RUN_TEST(convertRGB565_RGB8888_SixInputValues_SixOutputValues);         
    RUN_TEST(convertRGB565_RGB8888_SevenInputValues_SevenOutputValues);     
    puts("OK");
    return 0;
}

int convertRGB565_RGB8888_singleInputIntermediateValue_singleOutputIntermediateValue()
{
    //Arrange
    int count = 1;
    rgb565 *srcArray = nmppsMalloc_16s(count);    
    rgb8888 *dstArray = new rgb8888[count];
    rgb8888 *expectedDstArray = new rgb8888[count];
    for (int i = 0; i < count; ++i) {
        dstArray[i].vec[0] = 0;
    }
    nmppsPut_16s(srcArray, 0, 0xC979);  
    expectedDstArray[0].vec[0] = 0x00CE2CCE;
    
    //Act
    convertRGB565_RGB8888(srcArray, dstArray, count);

    //Assert
    TEST_VEC1_ARRAYS_EQUALI(dstArray, expectedDstArray, count);

	nmppsFree(srcArray);
	delete dstArray;
	delete expectedDstArray;

    return 0;
}

int convertRGB565_RGB8888_singleInputMaxValue_SingleOutputMaxValue()
{
    //Arrange
    int count = 1;
    rgb565 *srcArray = nmppsMalloc_16s(count);   
    rgb8888 *dstArray = new rgb8888[count];
    rgb8888 *expectedDstArray = new rgb8888[count];
    for (int i = 0; i < count; ++i) {
        dstArray[i].vec[0] = 0;
    }
    nmppsPut_16s(srcArray, 0, 0xFFFF);   
    expectedDstArray[0].vec[0] = 0x00FFFFFF;

    //Act
    convertRGB565_RGB8888(srcArray, dstArray, count);

    //Assert
    TEST_VEC1_ARRAYS_EQUALI(dstArray, expectedDstArray, count);

	nmppsFree(srcArray);
	delete dstArray;
	delete expectedDstArray;

    return 0;
}

int convertRGB565_RGB8888_singleInputMinValue_singleOutputMinValue()
{
    //Arrange
    int count = 1;
    rgb565 *srcArray = nmppsMalloc_16s(count);    
    rgb8888 *dstArray = new rgb8888[count];
    rgb8888 *expectedDstArray = new rgb8888[count];
    for (int i = 0; i < count; ++i) {
        dstArray[i].vec[0] = 0;
    }
    nmppsPut_16s(srcArray, 0, 0);
    expectedDstArray[0].vec[0] = 0;

    //Act
    convertRGB565_RGB8888(srcArray, dstArray, count);

    //Assert
    TEST_VEC1_ARRAYS_EQUALI(dstArray, expectedDstArray, count);

	nmppsFree(srcArray);
	delete dstArray;
	delete expectedDstArray;

    return 0;
}

int convertRGB565_RGB8888_manyInputValues_manyOutputValues()
{
    //Arrange
    constexpr int count = 4;
    rgb565 *srcArray = nmppsMalloc_16s(count);
    nm32u RGB565Values[count] = {0xC979, 0xFFFF, 0xAAAA, 0x5555};
    rgb8888 *dstArray = new rgb8888[count];
    rgb8888 *expectedDstArray = new rgb8888[count/2];
    for (int i = 0; i < count; ++i) {
        dstArray[i].vec[0] = 0;
    }
    expectedDstArray[0].vec[0] = 0x00CE2CCE | (((unsigned long long) 0x00FFFFFF) << 32);
    expectedDstArray[1].vec[0] = 0x00AD5552 | (((unsigned long long) 0x0052AAAD) << 32);

    for (int i = 0; i < count; ++i){
        nmppsPut_16s(srcArray, i, RGB565Values[i]);
    }

    //Act
    convertRGB565_RGB8888(srcArray, dstArray, count);

    //Assert
    TEST_VEC1_ARRAYS_EQUALI(dstArray, expectedDstArray, count/2);

	nmppsFree(srcArray);
	delete dstArray;
	delete expectedDstArray;

    return 0;
}

int convertRGB565_RGB8888_FiveInputValues_FiveOutputValues()
{
    //Arrange
    constexpr int count = 6;
    rgb565 *srcArray = nmppsMalloc_16s(count);
    nm32u RGB565Values[count] = {0xC979, 0xFFFF, 0xAAAA, 0x5555, 0xC979, 0xFFFF};   
    rgb8888 *dstArray = new rgb8888[count];
    rgb8888 *expectedDstArray = new rgb8888[count];
    for (int i = 0; i < count; ++i) {
        dstArray[i].vec[0] = 0;
    }

    expectedDstArray[0].vec[0] = 0x00CE2CCE | (((unsigned long long) 0x00FFFFFF) << 32);
    expectedDstArray[1].vec[0] = 0x00AD5552 | (((unsigned long long) 0x0052AAAD) << 32);
    expectedDstArray[2].vec[0] = 0x00CE2CCE | (((unsigned long long) 0x00000000) << 32);

    for (size_t i = 0; i < count; ++i){
        nmppsPut_16s(srcArray, i, RGB565Values[i]);
    }

    //Act
    convertRGB565_RGB8888(srcArray, dstArray, count-1);

    //Assert
    TEST_VEC1_ARRAYS_EQUALI(dstArray, expectedDstArray, count/2);

	nmppsFree(srcArray);
	delete dstArray;
	delete expectedDstArray;

    return 0;
}

int convertRGB565_RGB8888_SixInputValues_SixOutputValues()
{
    //Arrange
    constexpr int count = 7;
    rgb565 *srcArray = nmppsMalloc_16s(count);
    nm32u RGB565Values[count] = {0xC979, 0xFFFF, 0xAAAA, 0x5555, 0xC979, 0xAAAA, 0xFFFF};    
    rgb8888 *dstArray = new rgb8888[count];
    rgb8888 *expectedDstArray = new rgb8888[count];
    for (int i = 0; i < count; ++i) {
        dstArray[i].vec[0] = 0;
    }
    expectedDstArray[0].vec[0] = 0x00CE2CCE | (((unsigned long long) 0x00FFFFFF) << 32);
    expectedDstArray[1].vec[0] = 0x00AD5552 | (((unsigned long long) 0x0052AAAD) << 32);
    expectedDstArray[2].vec[0] = 0x00CE2CCE | (((unsigned long long) 0x00AD5552) << 32);
    expectedDstArray[3].vec[0] = 0x00000000;
    
    for (size_t i = 0; i < count; ++i){
        nmppsPut_16s(srcArray, i, RGB565Values[i]);
    }

    //Act
    convertRGB565_RGB8888(srcArray, dstArray, count-1);

    //Assert
    TEST_VEC1_ARRAYS_EQUALI(dstArray, expectedDstArray, 4);

	nmppsFree(srcArray);
	delete dstArray;
	delete expectedDstArray;

    return 0;
}

int convertRGB565_RGB8888_SevenInputValues_SevenOutputValues()
{
    //Arrange
    constexpr int count = 8;
    rgb565 *srcArray = nmppsMalloc_16s(count);
    nm32u RGB565Values[count] = {0xC979, 0xFFFF, 0xAAAA, 0x5555, 0xC979, 0xFFFF, 0x5555, 0xFFFF};     
    rgb8888 *dstArray = new rgb8888[count];
    rgb8888 *expectedDstArray = new rgb8888[count];
    for (int i = 0; i < count; ++i) {
        dstArray[i].vec[0] = 0;
    }
    expectedDstArray[0].vec[0] = 0x00CE2CCE | (((unsigned long long) 0x00FFFFFF) << 32);
    expectedDstArray[1].vec[0] = 0x00AD5552 | (((unsigned long long) 0x0052AAAD) << 32);
    expectedDstArray[2].vec[0] = 0x00CE2CCE | (((unsigned long long) 0x00FFFFFF) << 32);
    expectedDstArray[3].vec[0] = 0x0052AAAD | (((unsigned long long) 0x00000000) << 32);
    
    for (size_t i = 0; i < count; ++i){
        nmppsPut_16s(srcArray, i, RGB565Values[i]);
    }

    //Act
    convertRGB565_RGB8888(srcArray, dstArray, count-1);
    
    //Assert
    TEST_VEC1_ARRAYS_EQUALI(dstArray, expectedDstArray, count/2);

	nmppsFree(srcArray);
	delete dstArray;
	delete expectedDstArray;

    return 0;
}

