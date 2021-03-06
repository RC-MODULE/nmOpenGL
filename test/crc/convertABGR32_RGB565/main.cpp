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
int convertABGR32_RGB565_manyInputValues_manyOutputValues();
int convertABGR32_RGB565_fiveInputValues_fiveOutputValues();
int convertABGR32_RGB565_sixInputValues_sixOutputValues();
int convertABGR32_RGB565_sevenInputValues_sevenOutputValues();

// Performance tests
clock_t convertABGR32_RGB565_fourInputValues_fourOutputValues();

int main(int argc, char **argv)
{
    puts("convertABGR32_RGB565 tests: ");
    RUN_TEST(convertABGR32_RGB565_singleInputIntermediateValue_singleOutputIntermediateValue);
    RUN_TEST(convertABGR32_RGB565_singleInputMaxValue_SingleOutputMaxValue);
    RUN_TEST(convertABGR32_RGB565_singleInputMinValue_singleOutputMinValue);
    RUN_TEST(convertABGR32_RGB565_manyInputValues_manyOutputValues);
    RUN_TEST(convertABGR32_RGB565_fiveInputValues_fiveOutputValues);
    RUN_TEST(convertABGR32_RGB565_sixInputValues_sixOutputValues);
    RUN_TEST(convertABGR32_RGB565_sevenInputValues_sevenOutputValues);

	clock_t dt;
	dt = convertABGR32_RGB565_fourInputValues_fourOutputValues();

    puts("OK");
    return dt;
}

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

int convertABGR32_RGB565_manyInputValues_manyOutputValues()
{
    // Arrange
    constexpr int count = 4;
    abgr32 *srcArray = new abgr32[count];    
    rgb565 *dstArray = nmppsMalloc_16s(count);
    nm32u expectedRGB565Values[count] = {0xAE6C, 0xFFFF, 0x2B08, 0x3518};
    rgb565 *expectedDstArray = nmppsMalloc_16s(count);
    // The following arrays are used for comparison
    int16b dstArray_int16b[count];
    int16b expectedDstArray_int16b[count];

    srcArray[0].vec[0] = 0x3333333355555555;
    srcArray[0].vec[1] = 0xAAAAAAAACCCCCCCC;

    srcArray[1].vec[0] = 0xFFFFFFFFFFFFFFFF;
    srcArray[1].vec[1] = 0xFFFFFFFFFFFFFFFF; 

    srcArray[2].vec[0] = 0x000000D800000005;
    srcArray[2].vec[1] = 0xFFFFFFFF000000A8; 

    srcArray[3].vec[0] = 0x000000E800000006;
    srcArray[3].vec[1] = 0xFFFFFFFF000000B8; 

    // Populate expectedDstArray by expected values
    for (int i = 0; i < count; ++i){
        nmppsPut_16s(expectedDstArray, i, expectedRGB565Values[i]);
    }

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

int convertABGR32_RGB565_fiveInputValues_fiveOutputValues()
{
    // Arrange
    constexpr int count = 8;
    abgr32 *srcArray = new abgr32[count];    
    rgb565 *dstArray = nmppsMalloc_16s(count);
    nm32u expectedRGB565Values[count] = {0xAE6C, 0xFFFF, 0x2B08, 0x3518, 0xAE6C, 0x1010, 0x1010, 0x1010};
    rgb565 *expectedDstArray = nmppsMalloc_16s(count);
    // The following arrays are used for comparison
    int16b dstArray_int16b[count];
    int16b expectedDstArray_int16b[count];

    srcArray[0].vec[0] = 0x3333333355555555;
    srcArray[0].vec[1] = 0xAAAAAAAACCCCCCCC;

    srcArray[1].vec[0] = 0xFFFFFFFFFFFFFFFF;
    srcArray[1].vec[1] = 0xFFFFFFFFFFFFFFFF; 

    srcArray[2].vec[0] = 0x000000D800000005;
    srcArray[2].vec[1] = 0xFFFFFFFF000000A8; 

    srcArray[3].vec[0] = 0x000000E800000006;
    srcArray[3].vec[1] = 0xFFFFFFFF000000B8;

    srcArray[4].vec[0] = 0x3333333355555555;
    srcArray[4].vec[1] = 0xAAAAAAAACCCCCCCC;

    // Populate expectedDstArray by expected values
    for (int i = 0; i < count; ++i){
        nmppsPut_16s(expectedDstArray, i, expectedRGB565Values[i]);
        nmppsPut_16s(dstArray, i, expectedRGB565Values[i]);
    }
    //Act
    convertABGR32_RGB565(srcArray, dstArray, 5);

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

int convertABGR32_RGB565_sixInputValues_sixOutputValues()
{
    // Arrange
    constexpr int count = 8;
    abgr32 *srcArray = new abgr32[count];    
    rgb565 *dstArray = nmppsMalloc_16s(count);
    nm32u expectedRGB565Values[count] = {0xAE6C, 0xFFFF, 0x2B08, 0x3518, 0xAE6C, 0x2B08,0x1010, 0x1010};
    rgb565 *expectedDstArray = nmppsMalloc_16s(count);
    // The following arrays are used for comparison
    int16b dstArray_int16b[count];
    int16b expectedDstArray_int16b[count];

    srcArray[0].vec[0] = 0x3333333355555555;
    srcArray[0].vec[1] = 0xAAAAAAAACCCCCCCC;

    srcArray[1].vec[0] = 0xFFFFFFFFFFFFFFFF;
    srcArray[1].vec[1] = 0xFFFFFFFFFFFFFFFF; 

    srcArray[2].vec[0] = 0x000000D800000005;
    srcArray[2].vec[1] = 0xFFFFFFFF000000A8; 

    srcArray[3].vec[0] = 0x000000E800000006;
    srcArray[3].vec[1] = 0xFFFFFFFF000000B8;

    srcArray[4].vec[0] = 0x3333333355555555;
    srcArray[4].vec[1] = 0xAAAAAAAACCCCCCCC;

    srcArray[5].vec[0] = 0x000000D800000005;
    srcArray[5].vec[1] = 0xFFFFFFFF000000A8;

    // Populate expectedDstArray by expected values
    for (int i = 0; i < count; ++i){
        nmppsPut_16s(expectedDstArray, i, expectedRGB565Values[i]);
        nmppsPut_16s(dstArray, i, expectedRGB565Values[i]);
    }

    //Act
    convertABGR32_RGB565(srcArray, dstArray, 6);

    //Assert
    // Get the elements from the dstArray and expectedDstArray, 
    // convert them to nm32u for better comparison
    for(int i = 0; i < count; ++i){
        nmppsGetVal_16s(dstArray, i, &dstArray_int16b[i]);
        nmppsGetVal_16s(expectedDstArray, i, &expectedDstArray_int16b[i]);
    }

    TEST_ARRAYS_EQUALI(((nm32u *) dstArray_int16b), ((nm32u *) expectedDstArray_int16b), 8);

    delete srcArray;
    nmppsFree(dstArray);
    nmppsFree(expectedDstArray);

    return 0;
}

int convertABGR32_RGB565_sevenInputValues_sevenOutputValues()
{
    // Arrange
    constexpr int count = 8;
    abgr32 *srcArray = new abgr32[count];    
    rgb565 *dstArray = nmppsMalloc_16s(count);
    nm32u expectedRGB565Values[count] = {0xAE6C, 0xFFFF, 0x2B08, 0x3518, 0xAE6C, 0x2B08, 0x3518, 0x1010};
    rgb565 *expectedDstArray = nmppsMalloc_16s(count);
    // The following arrays are used for comparison
    int16b dstArray_int16b[count];
    int16b expectedDstArray_int16b[count];

    srcArray[0].vec[0] = 0x3333333355555555;
    srcArray[0].vec[1] = 0xAAAAAAAACCCCCCCC;

    srcArray[1].vec[0] = 0xFFFFFFFFFFFFFFFF;
    srcArray[1].vec[1] = 0xFFFFFFFFFFFFFFFF; 

    srcArray[2].vec[0] = 0x000000D800000005;
    srcArray[2].vec[1] = 0xFFFFFFFF000000A8; 

    srcArray[3].vec[0] = 0x000000E800000006;
    srcArray[3].vec[1] = 0xFFFFFFFF000000B8;

    srcArray[4].vec[0] = 0x3333333355555555;
    srcArray[4].vec[1] = 0xAAAAAAAACCCCCCCC;

    srcArray[5].vec[0] = 0x000000D800000005;
    srcArray[5].vec[1] = 0xFFFFFFFF000000A8;

    srcArray[6].vec[0] = 0x000000E800000006;
    srcArray[6].vec[1] = 0xFFFFFFFF000000B8;

    srcArray[7].vec[0] = 0x000000E800000006;
    srcArray[7].vec[1] = 0xFFFFFFFF000000B8;

    // Populate expectedDstArray by expected values
    for (int i = 0; i < count; ++i){
        nmppsPut_16s(expectedDstArray, i, expectedRGB565Values[i]);
        nmppsPut_16s(dstArray, i, expectedRGB565Values[i]);
    }
    convertABGR32_RGB565(srcArray, dstArray, 7);

    //Assert
    // Get the elements from the dstArray and expectedDstArray, 
    // convert them to nm32u for better comparison
    for(int i = 0; i < count; ++i){
        nmppsGetVal_16s(dstArray, i, &dstArray_int16b[i]);
        nmppsGetVal_16s(expectedDstArray, i, &expectedDstArray_int16b[i]);
    }

    TEST_ARRAYS_EQUALI(((nm32u *) dstArray_int16b), ((nm32u *) expectedDstArray_int16b), 8);

    delete srcArray;
    nmppsFree(dstArray);
    nmppsFree(expectedDstArray);

    return 0;
}

clock_t convertABGR32_RGB565_fourInputValues_fourOutputValues()
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
