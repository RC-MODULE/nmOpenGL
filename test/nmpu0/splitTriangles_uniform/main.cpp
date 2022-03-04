#include <vector>
#include <iostream>
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <math.h>

#include "nmtype.h"
#include "nmblas.h"
#include "service.h"
#include "tests.h"
#include "demo3d_nm0.h"

// Performance testing
#include "time.h"

// Base test functions: correctness
int splitTriangles_uniform_EmptyInput_NoTrianglesAtOutput(NMGLenum hint);
int splitTriangles_uniform_EmptyInput_NoTreatedTriangles(NMGLenum hint);
int splitTriangles_uniform_oneInputTriangleNeedsDivision_dstSizeIsCorrect(NMGLenum hint);
int splitTriangles_uniform_oneInputTriangleNeedsDivision_srcTreatedIsCorrect(NMGLenum hint);
// Base test functions: performance
clock_t splitTriangles_uniform_isoscelesTrianglesNeedsDivision(float width, int *treated, int *dstSize, NMGLenum hint);
clock_t splitTriangles_uniform_narrowTriangleNeedsDivision(float width, int *treated, int *dstSize, NMGLenum hint);
clock_t splitTriangles_uniform_nInputsNoDivision_nOutputs(int n, NMGLenum hint);

// Correctness tests
int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_EmptyInput_NoTrianglesAtOutput();
int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_EmptyInput_NoTreatedTriangles();
int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_oneInputTriangleNeedsDivision_dstSizeIsCorrect();
int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_oneInputTriangleNeedsDivision_srcTreatedIsCorrect();

int splitTriangles_uniform_PERSPECTIVE_CORRECT_EmptyInput_NoTrianglesAtOutput();
int splitTriangles_uniform_PERSPECTIVE_CORRECT_EmptyInput_NoTreatedTriangles();
int splitTriangles_uniform_PERSPECTIVE_CORRECT_oneInputTriangleNeedsDivision_dstSizeIsCorrect();
int splitTriangles_uniform_PERSPECTIVE_CORRECT_oneInputTriangleNeedsDivision_srcTreatedIsCorrect();

int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_oneInputTrianglesNeedsDivision_dstIsCorrect();
int splitTriangles_uniform_PERSPECTIVE_CORRECT_oneInputTrianglesNeedsDivision_dstIsCorrect();

// Performance tests
int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_0_to_1024InputsNoDivision_0_to_1024Outputs();
int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_isoscelesTrianglesNeedsDivisionWidth_40_to_760();
int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_narrowTriangleNeedsDivisionWidth_40_to_760();

int splitTriangles_uniform_PERSPECTIVE_CORRECT_0_to_1024InputsNoDivision_0_to_1024Outputs();
int splitTriangles_uniform_PERSPECTIVE_CORRECT_isoscelesTrianglesNeedsDivisionWidth_40_to_760();
int splitTriangles_uniform_PERSPECTIVE_CORRECT_narrowTriangleNeedsDivisionWidth_40_to_760();

clock_t fillArray();

int main(int argc, char **argv)
{
	printf("\n%s functional tests: \n", __FILE__);
	// Perspective correction disabled
	RUN_TEST(splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_EmptyInput_NoTrianglesAtOutput);
	RUN_TEST(splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_EmptyInput_NoTreatedTriangles);
	RUN_TEST(splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_oneInputTriangleNeedsDivision_dstSizeIsCorrect);
	RUN_TEST(splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_oneInputTriangleNeedsDivision_srcTreatedIsCorrect);
	RUN_TEST(splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_oneInputTrianglesNeedsDivision_dstIsCorrect);

	// Perspective correction enabled
	RUN_TEST(splitTriangles_uniform_PERSPECTIVE_CORRECT_EmptyInput_NoTrianglesAtOutput);
	RUN_TEST(splitTriangles_uniform_PERSPECTIVE_CORRECT_EmptyInput_NoTreatedTriangles);
	RUN_TEST(splitTriangles_uniform_PERSPECTIVE_CORRECT_oneInputTriangleNeedsDivision_dstSizeIsCorrect);
	RUN_TEST(splitTriangles_uniform_PERSPECTIVE_CORRECT_oneInputTriangleNeedsDivision_srcTreatedIsCorrect);
	RUN_TEST(splitTriangles_uniform_PERSPECTIVE_CORRECT_oneInputTrianglesNeedsDivision_dstIsCorrect);

	printf("\n%s performance tests: \n", __FILE__);
	// Perspective correction disabled
	puts("\nN input small triangles, no division, N output triangles, perspective correction disabled:");
	RUN_PERF_TEST(splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_0_to_1024InputsNoDivision_0_to_1024Outputs);
	puts("\nN = 1 input triangles, needs division, M output triangles, perspective correction disabled:");
	RUN_PERF_TEST(splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_isoscelesTrianglesNeedsDivisionWidth_40_to_760);
	puts("\nN = 1 input narrow triangles (height is OK, width is big), needs division, M output triangles, perspective correction disabled:");
	RUN_PERF_TEST(splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_narrowTriangleNeedsDivisionWidth_40_to_760);

	// Perspective correction enabled
	puts("\nN input small triangles, no division, N output triangles, perspective correction enabled:");
	RUN_PERF_TEST(splitTriangles_uniform_PERSPECTIVE_CORRECT_0_to_1024InputsNoDivision_0_to_1024Outputs);
	puts("\nN = 1 input triangles, needs division, M output triangles, perspective correction enabled:");
	RUN_PERF_TEST(splitTriangles_uniform_PERSPECTIVE_CORRECT_isoscelesTrianglesNeedsDivisionWidth_40_to_760);
	puts("\nN = 1 input narrow triangles (height is OK, width is big), needs division, M output triangles, perspective correction enabled:");
	RUN_PERF_TEST(splitTriangles_uniform_PERSPECTIVE_CORRECT_narrowTriangleNeedsDivisionWidth_40_to_760);
	//RUN_PERF_TEST(fillArray);
	

	puts("OK");
	return 0;
}

SECTION(".data_imu0") nm32f 		x0_in[NMGL_SIZE];
SECTION(".data_imu0") nm32f 		y0_in[NMGL_SIZE];
SECTION(".data_imu0") nm32f 		z0_in[NMGL_SIZE];
SECTION(".data_imu0") nm32f 		w0_in[NMGL_SIZE];
SECTION(".data_imu0") nm32f 		s0_in[NMGL_SIZE];
SECTION(".data_imu0") nm32f 		t0_in[NMGL_SIZE];
SECTION(".data_imu0") v4nm32f color0_in[NMGL_SIZE];

SECTION(".data_imu1") nm32f 		x1_in[NMGL_SIZE];
SECTION(".data_imu1") nm32f 		y1_in[NMGL_SIZE];
SECTION(".data_imu1") nm32f 		z1_in[NMGL_SIZE];
SECTION(".data_imu1") nm32f 		w1_in[NMGL_SIZE];
SECTION(".data_imu1") nm32f 		s1_in[NMGL_SIZE];
SECTION(".data_imu1") nm32f 		t1_in[NMGL_SIZE];
SECTION(".data_imu1") v4nm32f color1_in[NMGL_SIZE];

SECTION(".data_imu2") nm32f 		x2_in[NMGL_SIZE];
SECTION(".data_imu2") nm32f 		y2_in[NMGL_SIZE];
SECTION(".data_imu2") nm32f 		z2_in[NMGL_SIZE];
SECTION(".data_imu2") nm32f 		w2_in[NMGL_SIZE];
SECTION(".data_imu2") nm32f 		s2_in[NMGL_SIZE];
SECTION(".data_imu2") nm32f 		t2_in[NMGL_SIZE];
SECTION(".data_imu2") v4nm32f color2_in[NMGL_SIZE];

SECTION(".data_imu4") nm32f 		x0_out[NMGL_SIZE];
SECTION(".data_imu4") nm32f 		y0_out[NMGL_SIZE];
SECTION(".data_imu4") nm32f 		z0_out[NMGL_SIZE];
SECTION(".data_imu4") nm32f 		w0_out[NMGL_SIZE];
SECTION(".data_imu4") nm32f 		s0_out[NMGL_SIZE];
SECTION(".data_imu4") nm32f 		t0_out[NMGL_SIZE];
SECTION(".data_imu4") v4nm32f color0_out[NMGL_SIZE];

SECTION(".data_imu5") nm32f 		x1_out[NMGL_SIZE];
SECTION(".data_imu5") nm32f 		y1_out[NMGL_SIZE];
SECTION(".data_imu5") nm32f 		z1_out[NMGL_SIZE];
SECTION(".data_imu5") nm32f 		w1_out[NMGL_SIZE];
SECTION(".data_imu5") nm32f 		s1_out[NMGL_SIZE];
SECTION(".data_imu5") nm32f 		t1_out[NMGL_SIZE];
SECTION(".data_imu5") v4nm32f color1_out[NMGL_SIZE];

SECTION(".data_imu6") nm32f 		x2_out[NMGL_SIZE];
SECTION(".data_imu6") nm32f 		y2_out[NMGL_SIZE];
SECTION(".data_imu6") nm32f 		z2_out[NMGL_SIZE];
SECTION(".data_imu6") nm32f 		w2_out[NMGL_SIZE];
SECTION(".data_imu6") nm32f 		s2_out[NMGL_SIZE];
SECTION(".data_imu6") nm32f 		t2_out[NMGL_SIZE];
SECTION(".data_imu6") v4nm32f color2_out[NMGL_SIZE];

SECTION(".data_imu3")	float nmglBuffer2[12 * NMGL_SIZE];
SECTION(".data_imu4")	float nmglBuffer3[12 * NMGL_SIZE];
SECTION(".data_imu7")	float nmglBuffer5[12 * NMGL_SIZE];
SECTION(".data_imu0") NMGL_Context_NM0 *NMGL_Context_NM0::context;

NMGLubyte* palettes_p; // texture palette memory
unsigned int* palettes_widths_p; // texture palettes widths memory

int splitTriangles_uniform_EmptyInput_NoTrianglesAtOutput(NMGLenum hint)
{
	NMGL_Context_NM0 *cntxt;
	NMGL_Context_NM0::create();
	cntxt = NMGL_Context_NM0::getContext();
	cntxt->buffer2 = (float*)nmglBuffer2;
	cntxt->buffer3 = (float*)nmglBuffer3;
	cntxt->buffer5 = (float*)nmglBuffer5;
	cntxt->perspectiveCorrectionHint = hint;

	// Arrange
	TrianglePointers srcTriangles = {
		{x0_in, y0_in, z0_in, w0_in, s0_in, t0_in, color0_in},
		{x1_in, y1_in, z1_in, w1_in, s1_in, t1_in, color1_in},
		{x2_in, y2_in, z2_in, w2_in, s2_in, t2_in, color2_in},
	};
	TrianglePointers dstTriangles = {
		{x0_out, y0_out, z0_out, w0_out, s0_out, t0_out, color0_out},
		{x1_out, y1_out, z1_out, w1_out, s1_out, t1_out, color1_out},
		{x2_out, y2_out, z2_out, w2_out, s2_out, t2_out, color2_out},
	};

	int srcCount = 0;
	int maxWidth = 32;
	int maxHeight = 32;
	int maxDstSize = NMGL_SIZE;
	int srcTreatedCount = -1;
	int res = -1;

	//Act
	res = splitTriangles_uniform(&srcTriangles, srcCount, maxWidth, maxHeight, maxDstSize, &dstTriangles, &srcTreatedCount);

	//Assert
	TEST_ASSERT(res == 0);

	return 0;
}

int splitTriangles_uniform_EmptyInput_NoTreatedTriangles(NMGLenum hint)
{
	NMGL_Context_NM0 *cntxt;
	NMGL_Context_NM0::create();
	cntxt = NMGL_Context_NM0::getContext();
	cntxt->buffer2 = (float*)nmglBuffer2;
	cntxt->buffer3 = (float*)nmglBuffer3;
	cntxt->buffer5 = (float*)nmglBuffer5;
	cntxt->perspectiveCorrectionHint = hint;

	// Arrange
	TrianglePointers srcTriangles = {
		{x0_in, y0_in, z0_in, w0_in, s0_in, t0_in, color0_in},
		{x1_in, y1_in, z1_in, w1_in, s1_in, t1_in, color1_in},
		{x2_in, y2_in, z2_in, w2_in, s2_in, t2_in, color2_in},
	};
	TrianglePointers dstTriangles = {
		{x0_out, y0_out, z0_out, w0_out, s0_out, t0_out, color0_out},
		{x1_out, y1_out, z1_out, w1_out, s1_out, t1_out, color1_out},
		{x2_out, y2_out, z2_out, w2_out, s2_out, t2_out, color2_out},
	};

	int srcCount = 0;
	int maxWidth = 32;
	int maxHeight = 32;
	int maxDstSize = NMGL_SIZE;
	int srcTreatedCount = -1;
	int res = -1;

	//Act
	res = splitTriangles_uniform(&srcTriangles, srcCount, maxWidth, maxHeight, maxDstSize, &dstTriangles, &srcTreatedCount);

	//Assert
	TEST_ASSERT(srcTreatedCount == 0);

	return 0;
}

int splitTriangles_uniform_oneInputTriangleNeedsDivision_dstSizeIsCorrect(NMGLenum hint)
{

	constexpr int size = 1;

	NMGL_Context_NM0 *cntxt;
	NMGL_Context_NM0::create();
	cntxt = NMGL_Context_NM0::getContext();
	cntxt->buffer2 = (float*)nmglBuffer2;
	cntxt->buffer3 = (float*)nmglBuffer3;
	cntxt->buffer5 = (float*)nmglBuffer5;
	cntxt->perspectiveCorrectionHint = hint;

	// Arrange
	TrianglePointers srcTriangles = {
		{x0_in, y0_in, z0_in, w0_in, s0_in, t0_in, color0_in},
		{x1_in, y1_in, z1_in, w1_in, s1_in, t1_in, color1_in},
		{x2_in, y2_in, z2_in, w2_in, s2_in, t2_in, color2_in},
	};
	TrianglePointers dstTriangles = {
		{x0_out, y0_out, z0_out, w0_out, s0_out, t0_out, color0_out},
		{x1_out, y1_out, z1_out, w1_out, s1_out, t1_out, color1_out},
		{x2_out, y2_out, z2_out, w2_out, s2_out, t2_out, color2_out},
	};

	float width = 128.0f;
	float height = width;

	for (size_t i = 0; i < size; ++i){
		x0_in[i] =  0.0f;
		y0_in[i] =  0.0f;

		x1_in[i] = width / 2;
		y1_in[i] = height;

		x2_in[i] = width;
		y2_in[i] =  0.0f;
	}

	int srcCount = size;
	int maxWidth = 32;
	int maxHeight = 32;
	int maxDstSize = NMGL_SIZE;
	int srcTreatedCount = 0;
	int res = -1;
	int resExpected = (int) pow(ceil(std::max(width / (maxWidth - 1), height / (maxHeight - 1))), 2.0);

	//Act
	res = splitTriangles_uniform(&srcTriangles, srcCount, maxWidth, maxHeight, maxDstSize, &dstTriangles, &srcTreatedCount);

	//Assert
	TEST_ASSERT(res == resExpected);

	return 0;
}

int splitTriangles_uniform_oneInputTriangleNeedsDivision_srcTreatedIsCorrect(NMGLenum hint)
{

	constexpr int size = 1;

	NMGL_Context_NM0 *cntxt;
	NMGL_Context_NM0::create();
	cntxt = NMGL_Context_NM0::getContext();
	cntxt->buffer2 = (float*)nmglBuffer2;
	cntxt->buffer3 = (float*)nmglBuffer3;
	cntxt->buffer5 = (float*)nmglBuffer5;
	cntxt->perspectiveCorrectionHint = hint;

	// Arrange
	TrianglePointers srcTriangles = {
		{x0_in, y0_in, z0_in, w0_in, s0_in, t0_in, color0_in},
		{x1_in, y1_in, z1_in, w1_in, s1_in, t1_in, color1_in},
		{x2_in, y2_in, z2_in, w2_in, s2_in, t2_in, color2_in},
	};
	TrianglePointers dstTriangles = {
		{x0_out, y0_out, z0_out, w0_out, s0_out, t0_out, color0_out},
		{x1_out, y1_out, z1_out, w1_out, s1_out, t1_out, color1_out},
		{x2_out, y2_out, z2_out, w2_out, s2_out, t2_out, color2_out},
	};

	float width = 128.0f;
	float height = width;

	for (size_t i = 0; i < size; ++i){
		x0_in[i] =  0.0f;
		y0_in[i] =  0.0f;

		x1_in[i] = width / 2;
		y1_in[i] = height;

		x2_in[i] = width;
		y2_in[i] =  0.0f;
	}

	int srcCount = size;
	int maxWidth = 32;
	int maxHeight = 32;
	int maxDstSize = NMGL_SIZE;
	int srcTreatedCount = 0;
	int srcTreatedCountExpected = size;
	int res = -1;

	//Act
	res = splitTriangles_uniform(&srcTriangles, srcCount, maxWidth, maxHeight, maxDstSize, &dstTriangles, &srcTreatedCount);

	//Assert
	TEST_ASSERT(srcTreatedCount == srcTreatedCountExpected);

	return 0;
}

// Correctness tests

// @case Количество треугольников в выходном массиве равно нулю при отсутствии треугольников на входе (перспективно-корректная интерполяция выключена)
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
	// @step Выключить перспективно-корректную интерполяцию, установив режим интерполяции равным NMGL_DONT_CARE
	// @step Создать входной и выходной массивы
	// @step Установить количество треугольников на входе равным 0 
	// @step Установить произвольное ограничение ширины треугольника
	// @step Установить произвольное ограничение высоты треугольника
	// @step Установить максимально допустимое количество треугольников на выходе равным NMGL_SIZE (1024)
	// @step Установить количество обработанных треугольников равным -1
	// @step Установить количество треугольников на выходе равным -1
	// @step Вызвать функцию splitTriangles_uniform с заданными аргументами
	// @assert Проверить, что в количество треугольников на выходе равно 0
	// @pass Количество треугольников на выходе, проверяемое на шаге #assert, равно 0
	// @use splitTriangles_uniform_EmptyInput_NoTrianglesAtOutput(NMGLenum hint)
int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_EmptyInput_NoTrianglesAtOutput()
{
	return splitTriangles_uniform_EmptyInput_NoTrianglesAtOutput(NMGL_DONT_CARE);
}

// @case Количество обработанных треугольников равно нулю при отсутствии треугольников на входе (перспективно-корректная интерполяция выключена)
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
	// @step Выключить перспективно-корректную интерполяцию, установив режим интерполяции равным NMGL_DONT_CARE
	// @step Создать входной и выходной массивы
	// @step Установить количество треугольников на входе равным 0 
	// @step Установить произвольное ограничение ширины треугольника
	// @step Установить произвольное ограничение высоты треугольника
	// @step Установить максимально допустимое количество треугольников на выходе равным NMGL_SIZE (1024)
	// @step Установить количество обработанных треугольников равным -1
	// @step Установить количество треугольников на выходе равным -1
	// @step Вызвать функцию splitTriangles_uniform с заданными аргументами
	// @assert Проверить, что в количество обработанных треугольников равно 0
	// @pass Количество обработанных треугольников, проверяемое на шаге #assert, равно 0
	// @use splitTriangles_uniform_EmptyInput_NoTreatedTriangles(NMGLenum hint)
int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_EmptyInput_NoTreatedTriangles()
{
	return splitTriangles_uniform_EmptyInput_NoTreatedTriangles(NMGL_DONT_CARE);
}

// @case Количество треугольников в выходном массиве корректно, если на входе один треугольник, требующий деления (места в выходном массиве достаточно, перспективно-корректная интерполяция выключена)
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
	// @step Выключить перспективно-корректную интерполяцию, установив режим интерполяции равным NMGL_DONT_CARE
	// @step Создать входной и выходной массивы
	// @step Установить количество треугольников на входе равным 1 
	// @step Во входном массиве создать треугольник с шириной 128 и высотой 128
	// @step Установить ограничение ширины треугольника равным 32
	// @step Установить ограничение высоты треугольника равным 32
	// @step Установить максимально допустимое количество треугольников на выходе равным NMGL_SIZE (1024)
	// @step Установить количество обработанных треугольников равным -1
	// @step Установить количество треугольников в выходном массиве равным 0 
	// @step Установить ожидаемое количество треугольников в выходном массиве равным 16
	// @step Вызвать функцию splitTriangles_uniform с заданными аргументами
	// @assert Сравнить фактическое и ожидаемое значения количества треугольников в выходном массиве
	// @pass Ожидаемое и фактическое значения количества треугольников в выходном массиве, сравниваемые на шаге #assert, совпадают
	// @use splitTriangles_uniform_oneInputTriangleNeedsDivision_dstSizeIsCorrect(NMGLenum hint)
int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_oneInputTriangleNeedsDivision_dstSizeIsCorrect()
{
	return splitTriangles_uniform_oneInputTriangleNeedsDivision_dstSizeIsCorrect(NMGL_DONT_CARE);
}

// @case Количество обработанных треугольников корректно, если на входе один треугольник, требующий деления (места в выходном массиве достаточно, перспективно-корректная интерполяция выключена)
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
	// @step Выключить перспективно-корректную интерполяцию, установив режим интерполяции равным NMGL_DONT_CARE
	// @step Создать входной и выходной массивы
	// @step Установить количество треугольников на входе равным 1 
	// @step Во входном массиве создать треугольник с шириной 128 и высотой 128
	// @step Установить ограничение ширины треугольника равным 32
	// @step Установить ограничение высоты треугольника равным 32
	// @step Установить максимально допустимое количество треугольников на выходе равным NMGL_SIZE (1024)
	// @step Установить количество обработанных треугольников равным -1
	// @step Установить ожидаемое значение количества обработанных треугольников равным 1
	// @step Установить количество треугольников в выходном массиве равным 0 
	// @step Вызвать функцию splitTriangles_uniform с заданными аргументами
	// @step Зафиксировать фактическое значения количества обработанных треугольников 
	// @assert Сравнить фактическое и ожидаемое значения количества обработанных треугольников
	// @pass Ожидаемое и фактическое значения количества обработанных треугольников, сравниваемые на шаге #assert, совпадают
	// @use splitTriangles_uniform_oneInputTriangleNeedsDivision_srcTreatedIsCorrect(NMGLenum hint)
int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_oneInputTriangleNeedsDivision_srcTreatedIsCorrect()
{
	return splitTriangles_uniform_oneInputTriangleNeedsDivision_srcTreatedIsCorrect(NMGL_DONT_CARE);
}

// @case Количество треугольников в выходном массиве равно нулю при отсутствии треугольников на входе (перспективно-корректная интерполяция включена)
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
	// @step Включить перспективно-корректную интерполяцию, установив режим интерполяции равным NMGL_NICEST
	// @step Создать входной и выходной массивы
	// @step Установить количество треугольников на входе равным 0 
	// @step Установить произвольное ограничение ширины треугольника
	// @step Установить произвольное ограничение высоты треугольника
	// @step Установить максимально допустимое количество треугольников на выходе равным NMGL_SIZE (1024)
	// @step Установить количество обработанных треугольников равным -1
	// @step Установить количество треугольников на выходе равным -1
	// @step Вызвать функцию splitTriangles_uniform с заданными аргументами
	// @assert Проверить, что в количество треугольников на выходе равно 0
	// @pass Количество треугольников на выходе, проверяемое на шаге #assert, равно 0
	// @use splitTriangles_uniform_EmptyInput_NoTrianglesAtOutput(NMGLenum hint)
int splitTriangles_uniform_PERSPECTIVE_CORRECT_EmptyInput_NoTrianglesAtOutput()
{
	return splitTriangles_uniform_EmptyInput_NoTrianglesAtOutput(NMGL_NICEST);
}

// @case Количество обработанных треугольников равно нулю при отсутствии треугольников на входе (перспективно-корректная интерполяция включена)
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
	// @step Включить перспективно-корректную интерполяцию, установив режим интерполяции равным NMGL_NICEST
	// @step Создать входной и выходной массивы
	// @step Установить количество треугольников на входе равным 0 
	// @step Установить произвольное ограничение ширины треугольника
	// @step Установить произвольное ограничение высоты треугольника
	// @step Установить максимально допустимое количество треугольников на выходе равным NMGL_SIZE (1024)
	// @step Установить количество обработанных треугольников равным -1
	// @step Установить количество треугольников на выходе равным -1
	// @step Вызвать функцию splitTriangles_uniform с заданными аргументами
	// @assert Проверить, что в количество обработанных треугольников равно 0
	// @pass Количество обработанных треугольников, проверяемое на шаге #assert, равно 0
	// @use splitTriangles_uniform_EmptyInput_NoTreatedTriangles(NMGLenum hint)
int splitTriangles_uniform_PERSPECTIVE_CORRECT_EmptyInput_NoTreatedTriangles()
{
	return splitTriangles_uniform_EmptyInput_NoTreatedTriangles(NMGL_NICEST);
}

// @case Количество треугольников в выходном массиве корректно, если на входе один треугольник, требующий деления (места в выходном массиве достаточно, перспективно-корректная интерполяция включена)
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
	// @step Включить перспективно-корректную интерполяцию, установив режим интерполяции равным NMGL_NICEST
	// @step Создать входной и выходной массивы
	// @step Установить количество треугольников на входе равным 1 
	// @step Во входном массиве создать треугольник с шириной 128 и высотой 128
	// @step Установить ограничение ширины треугольника равным 32
	// @step Установить ограничение высоты треугольника равным 32
	// @step Установить максимально допустимое количество треугольников на выходе равным NMGL_SIZE (1024)
	// @step Установить количество обработанных треугольников равным -1
	// @step Установить количество треугольников в выходном массиве равным 0 
	// @step Установить ожидаемое количество треугольников в выходном массиве равным 16
	// @step Вызвать функцию splitTriangles_uniform с заданными аргументами
	// @assert Сравнить фактическое и ожидаемое значения количества треугольников в выходном массиве
	// @pass Ожидаемое и фактическое значения количества треугольников в выходном массиве, сравниваемые на шаге #assert, совпадают
	// @use splitTriangles_uniform_oneInputTriangleNeedsDivision_dstSizeIsCorrect(NMGLenum hint)
int splitTriangles_uniform_PERSPECTIVE_CORRECT_oneInputTriangleNeedsDivision_dstSizeIsCorrect()
{
	return splitTriangles_uniform_oneInputTriangleNeedsDivision_dstSizeIsCorrect(NMGL_NICEST);
}

// @case Количество обработанных треугольников корректно, если на входе один треугольник, требующий деления (места в выходном массиве достаточно, перспективно-корректная интерполяция включена)
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
	// @step Включить перспективно-корректную интерполяцию, установив режим интерполяции равным NMGL_NICEST
	// @step Создать входной и выходной массивы
	// @step Установить количество треугольников на входе равным 1 
	// @step Во входном массиве создать треугольник с шириной 128 и высотой 128
	// @step Установить ограничение ширины треугольника равным 32
	// @step Установить ограничение высоты треугольника равным 32
	// @step Установить максимально допустимое количество треугольников на выходе равным NMGL_SIZE (1024)
	// @step Установить количество обработанных треугольников равным -1
	// @step Установить ожидаемое значение количества обработанных треугольников равным 1
	// @step Установить количество треугольников в выходном массиве равным 0 
	// @step Вызвать функцию splitTriangles_uniform с заданными аргументами
	// @step Зафиксировать фактическое значения количества обработанных треугольников 
	// @assert Сравнить фактическое и ожидаемое значения количества обработанных треугольников
	// @pass Ожидаемое и фактическое значения количества обработанных треугольников, сравниваемые на шаге #assert, совпадают
	// @use splitTriangles_uniform_oneInputTriangleNeedsDivision_srcTreatedIsCorrect(NMGLenum hint)
int splitTriangles_uniform_PERSPECTIVE_CORRECT_oneInputTriangleNeedsDivision_srcTreatedIsCorrect()
{
	return splitTriangles_uniform_oneInputTriangleNeedsDivision_srcTreatedIsCorrect(NMGL_NICEST);
}

// @case Данные в выходном массиве треугольников корректны, если на входе один треугольник, требующий деления (места в выходном массиве достаточно)
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
	// @step Создать входной и выходной массивы
	// @step Установить количество треугольников на входе равным 1 
	// @step Во входном массиве создать треугольник с шириной 128 и высотой 128, инициализировать атрибуты вершин треугольника
	// @step Установить ограничение ширины треугольника равным 32
	// @step Установить ограничение высоты треугольника равным 32
	// @step Установить максимально допустимое количество треугольников на выходе равным NMGL_SIZE (1024)
	// @step Установить количество обработанных треугольников равным -1
	// @step Установить количество треугольников в выходном массиве равным 0 
	// @step Установить ожидаемые значения атрибутов вершин выходных треугольников
	// @step Вызвать функцию splitTriangles_uniform с заданными аргументами
	// @assert Сравнить фактические и ожидаемые значения атрибутов вершин выходных треугольников
	// @pass Фактические и ожидаемые значения атрибутов вершин выходных треугольников, сравниваемые на шаге #assert, совпадают
int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_oneInputTrianglesNeedsDivision_dstIsCorrect()
{
	constexpr int size = 1;

	NMGL_Context_NM0 *cntxt;
	NMGL_Context_NM0::create();
	cntxt = NMGL_Context_NM0::getContext();
	cntxt->buffer2 = (float*)nmglBuffer2;
	cntxt->buffer3 = (float*)nmglBuffer3;
	cntxt->buffer5 = (float*)nmglBuffer5;

	// Arrange
	TrianglePointers srcTriangles = {
		{x0_in, y0_in, z0_in, w0_in, s0_in, t0_in, color0_in},
		{x1_in, y1_in, z1_in, w1_in, s1_in, t1_in, color1_in},
		{x2_in, y2_in, z2_in, w2_in, s2_in, t2_in, color2_in},
	};
	TrianglePointers dstTriangles = {
		{x0_out, y0_out, z0_out, w0_out, s0_out, t0_out, color0_out},
		{x1_out, y1_out, z1_out, w1_out, s1_out, t1_out, color1_out},
		{x2_out, y2_out, z2_out, w2_out, s2_out, t2_out, color2_out},
	};

	float width = 128.0f;
	float height = width;

	for (size_t i = 0; i < size; ++i){
		x0_in[i] =  0.0f;
		y0_in[i] =  0.0f;
		z0_in[i] =  0.1f;
		w0_in[i] =  0.2f;
		s0_in[i] =  0.0f;
		t0_in[i] =  0.0f;
		color0_in[i] =  {0.0f, 0.5f, 0.5f, 1.0f};

		x1_in[i] = width / 2;
		y1_in[i] = height;
		z1_in[i] =  0.3f;
		w1_in[i] =  0.4f;
		s1_in[i] =  32.0f;
		t1_in[i] =  64.0f;
		color1_in[i] =  {0.5f, 0.0f, 0.5f, 1.0f};

		x2_in[i] = width;
		y2_in[i] =  0.0f;
		z2_in[i] =  0.5f;
		w2_in[i] =  0.6f;
		s2_in[i] =  64.0f;
		t2_in[i] =  0.0f;
		color2_in[i] =  {0.5f, 0.5f, 0.0f, 1.0f};
	}

	int srcCount = size;
	int maxWidth = 32;
	int maxHeight = 32;
	int maxDstSize = NMGL_SIZE;
	int srcTreatedCount = 0;
	int res = -1;

	nm32f v0xExpected[] = {  0.000000000000000f,  25.600000381469727f,  51.200000762939453f,  76.800003051757812f, 102.400001525878906f,  12.800000190734863f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f,  25.600000381469727f,  51.200000762939453f,  76.800003051757812f,  38.400001525878906f,  64.000000000000000f,  51.200000762939453f,  12.800000190734863f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f,  25.600000381469727f,  51.200000762939453f,  76.800003051757812f,  38.400001525878906f,  64.000000000000000f,  51.200000762939453f};
	nm32f v0yExpected[] = {  0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  51.200000762939453f,  51.200000762939453f,  51.200000762939453f,  76.800003051757812f,  76.800003051757812f, 102.400001525878906f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  51.200000762939453f,  51.200000762939453f,  51.200000762939453f,  76.800003051757812f,  76.800003051757812f, 102.400001525878906f};
	nm32f v0zExpected[] = {  0.100000001490116f,   0.180000007152557f,   0.260000020265579f,   0.340000003576279f,   0.420000016689301f,   0.140000000596046f,   0.220000013709068f,   0.300000011920929f,   0.380000025033951f,   0.180000007152557f,   0.260000020265579f,   0.340000003576279f,   0.220000013709068f,   0.300000011920929f,   0.260000020265579f,   0.140000000596046f,   0.220000013709068f,   0.300000011920929f,   0.380000025033951f,   0.180000007152557f,   0.260000020265579f,   0.340000003576279f,   0.220000013709068f,   0.300000011920929f,   0.260000020265579f};
	nm32f v0wExpected[] = {  0.200000002980232f,   0.280000001192093f,   0.360000014305115f,   0.440000027418137f,   0.520000040531158f,   0.240000009536743f,   0.320000022649765f,   0.400000035762787f,   0.480000048875809f,   0.280000001192093f,   0.360000014305115f,   0.440000027418137f,   0.320000022649765f,   0.400000035762787f,   0.360000014305115f,   0.240000009536743f,   0.320000022649765f,   0.400000035762787f,   0.480000048875809f,   0.280000001192093f,   0.360000014305115f,   0.440000027418137f,   0.320000022649765f,   0.400000035762787f,   0.360000014305115f};
	nm32f v0sExpected[] = {  0.000000000000000f,  12.800000190734863f,  25.600000381469727f,  38.400001525878906f,  51.200000762939453f,   6.400000095367432f,  19.200000762939453f,  32.000000000000000f,  44.800003051757812f,  12.800000190734863f,  25.600000381469727f,  38.400001525878906f,  19.200000762939453f,  32.000000000000000f,  25.600000381469727f,   6.400000095367432f,  19.200000762939453f,  32.000000000000000f,  44.800003051757812f,  12.800000190734863f,  25.600000381469727f,  38.400001525878906f,  19.200000762939453f,  32.000000000000000f,  25.600000381469727f};
	nm32f v0tExpected[] = {  0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  12.800000190734863f,  12.800000190734863f,  12.800000190734863f,  12.800000190734863f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  38.400001525878906f,  38.400001525878906f,  51.200000762939453f,  12.800000190734863f,  12.800000190734863f,  12.800000190734863f,  12.800000190734863f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  38.400001525878906f,  38.400001525878906f,  51.200000762939453f};
	v4nm32f v0ColorExpected[] = {{  0.000000000000000f,   0.500000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.100000001490116f,   0.500000000000000f,   0.400000005960464f,   1.000000000000000f}, {  0.200000002980232f,   0.500000000000000f,   0.300000011920929f,   1.000000000000000f}, {  0.300000011920929f,   0.500000000000000f,   0.199999988079071f,   1.000000000000000f}, {  0.400000005960464f,   0.500000000000000f,   0.099999994039536f,   1.000000000000000f}, {  0.100000001490116f,   0.400000005960464f,   0.500000000000000f,   1.000000000000000f}, {  0.200000002980232f,   0.400000005960464f,   0.400000005960464f,   1.000000000000000f}, {  0.300000011920929f,   0.400000005960464f,   0.300000011920929f,   1.000000000000000f}, {  0.400000035762787f,   0.400000005960464f,   0.199999973177910f,   1.000000000000000f}, {  0.200000002980232f,   0.300000011920929f,   0.500000000000000f,   1.000000000000000f}, {  0.300000011920929f,   0.300000011920929f,   0.400000005960464f,   1.000000000000000f}, {  0.400000005960464f,   0.300000011920929f,   0.299999982118607f,   1.000000000000000f}, {  0.300000011920929f,   0.199999988079071f,   0.500000000000000f,   1.000000000000000f}, {  0.400000005960464f,   0.199999988079071f,   0.400000005960464f,   1.000000000000000f}, {  0.400000005960464f,   0.099999994039536f,   0.500000000000000f,   1.000000000000000f}, {  0.100000001490116f,   0.400000005960464f,   0.500000000000000f,   1.000000000000000f}, {  0.200000002980232f,   0.400000005960464f,   0.400000005960464f,   1.000000000000000f}, {  0.300000011920929f,   0.400000005960464f,   0.300000011920929f,   1.000000000000000f}, {  0.400000035762787f,   0.400000005960464f,   0.199999973177910f,   1.000000000000000f}, {  0.200000002980232f,   0.300000011920929f,   0.500000000000000f,   1.000000000000000f}, {  0.300000011920929f,   0.300000011920929f,   0.400000005960464f,   1.000000000000000f}, {  0.400000005960464f,   0.300000011920929f,   0.299999982118607f,   1.000000000000000f}, {  0.300000011920929f,   0.199999988079071f,   0.500000000000000f,   1.000000000000000f}, {  0.400000005960464f,   0.199999988079071f,   0.400000005960464f,   1.000000000000000f}, {  0.400000005960464f,   0.099999994039536f,   0.500000000000000f,   1.000000000000000f}};

	nm32f v1xExpected[] = { 12.800000190734863f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f, 115.200004577636719f,  25.600000381469727f,  51.200000762939453f,  76.800003051757812f, 102.400001525878906f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f,  51.200000762939453f,  76.800003051757812f,  64.000000000000000f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f, 115.200004577636719f,  51.200000762939453f,  76.800003051757812f, 102.400001525878906f,  64.000000000000000f,  89.600006103515625f,  76.800003051757812f};
	nm32f v1yExpected[] = { 25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  51.200000762939453f,  51.200000762939453f,  51.200000762939453f,  51.200000762939453f,  76.800003051757812f,  76.800003051757812f,  76.800003051757812f, 102.400001525878906f, 102.400001525878906f, 128.000000000000000f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  51.200000762939453f,  51.200000762939453f,  51.200000762939453f,  76.800003051757812f,  76.800003051757812f, 102.400001525878906f};
	nm32f v1zExpected[] = {  0.140000000596046f,   0.220000013709068f,   0.300000011920929f,   0.380000025033951f,   0.460000008344650f,   0.180000007152557f,   0.260000020265579f,   0.340000003576279f,   0.420000016689301f,   0.220000013709068f,   0.300000011920929f,   0.380000025033951f,   0.260000020265579f,   0.340000003576279f,   0.300000011920929f,   0.220000013709068f,   0.300000011920929f,   0.380000025033951f,   0.460000008344650f,   0.260000020265579f,   0.340000003576279f,   0.420000016689301f,   0.300000011920929f,   0.380000025033951f,   0.340000003576279f};
	nm32f v1wExpected[] = {  0.240000009536743f,   0.320000022649765f,   0.400000035762787f,   0.480000048875809f,   0.560000061988831f,   0.280000001192093f,   0.360000014305115f,   0.440000027418137f,   0.520000040531158f,   0.320000022649765f,   0.400000035762787f,   0.480000048875809f,   0.360000014305115f,   0.440000027418137f,   0.400000035762787f,   0.320000022649765f,   0.400000035762787f,   0.480000048875809f,   0.560000061988831f,   0.360000014305115f,   0.440000027418137f,   0.520000040531158f,   0.400000035762787f,   0.480000048875809f,   0.440000027418137f};
	nm32f v1sExpected[] = {  6.400000095367432f,  19.200000762939453f,  32.000000000000000f,  44.800003051757812f,  57.600002288818359f,  12.800000190734863f,  25.600000381469727f,  38.400001525878906f,  51.200000762939453f,  19.200000762939453f,  32.000000000000000f,  44.800003051757812f,  25.600000381469727f,  38.400001525878906f,  32.000000000000000f,  19.200000762939453f,  32.000000000000000f,  44.800003051757812f,  57.600002288818359f,  25.600000381469727f,  38.400001525878906f,  51.200000762939453f,  32.000000000000000f,  44.800003051757812f,  38.400001525878906f};
	nm32f v1tExpected[] = { 12.800000190734863f,  12.800000190734863f,  12.800000190734863f,  12.800000190734863f,  12.800000190734863f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  38.400001525878906f,  38.400001525878906f,  38.400001525878906f,  51.200000762939453f,  51.200000762939453f,  64.000000000000000f,  12.800000190734863f,  12.800000190734863f,  12.800000190734863f,  12.800000190734863f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  38.400001525878906f,  38.400001525878906f,  51.200000762939453f};
	v4nm32f v1ColorExpected[] = {{  0.100000001490116f,   0.400000005960464f,   0.500000000000000f,   1.000000000000000f}, {  0.200000002980232f,   0.400000005960464f,   0.400000005960464f,   1.000000000000000f}, {  0.300000011920929f,   0.400000005960464f,   0.300000011920929f,   1.000000000000000f}, {  0.400000035762787f,   0.400000005960464f,   0.199999973177910f,   1.000000000000000f}, {  0.500000000000000f,   0.400000005960464f,   0.099999979138374f,   1.000000000000000f}, {  0.200000002980232f,   0.300000011920929f,   0.500000000000000f,   1.000000000000000f}, {  0.300000011920929f,   0.300000011920929f,   0.400000005960464f,   1.000000000000000f}, {  0.400000005960464f,   0.300000011920929f,   0.299999982118607f,   1.000000000000000f}, {  0.500000000000000f,   0.300000011920929f,   0.199999988079071f,   1.000000000000000f}, {  0.300000011920929f,   0.199999988079071f,   0.500000000000000f,   1.000000000000000f}, {  0.400000005960464f,   0.199999988079071f,   0.400000005960464f,   1.000000000000000f}, {  0.500000000000000f,   0.199999988079071f,   0.299999982118607f,   1.000000000000000f}, {  0.400000005960464f,   0.099999994039536f,   0.500000000000000f,   1.000000000000000f}, {  0.500000000000000f,   0.099999994039536f,   0.399999976158142f,   1.000000000000000f}, {  0.500000000000000f,   0.000000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.200000002980232f,   0.400000005960464f,   0.400000005960464f,   1.000000000000000f}, {  0.300000011920929f,   0.400000005960464f,   0.300000011920929f,   1.000000000000000f}, {  0.400000035762787f,   0.400000005960464f,   0.199999973177910f,   1.000000000000000f}, {  0.500000000000000f,   0.400000005960464f,   0.099999979138374f,   1.000000000000000f}, {  0.300000011920929f,   0.300000011920929f,   0.400000005960464f,   1.000000000000000f}, {  0.400000005960464f,   0.300000011920929f,   0.299999982118607f,   1.000000000000000f}, {  0.500000000000000f,   0.300000011920929f,   0.199999988079071f,   1.000000000000000f}, {  0.400000005960464f,   0.199999988079071f,   0.400000005960464f,   1.000000000000000f}, {  0.500000000000000f,   0.199999988079071f,   0.299999982118607f,   1.000000000000000f}, {  0.500000000000000f,   0.099999994039536f,   0.399999976158142f,   1.000000000000000f}};

	nm32f v2xExpected[] = { 25.600000381469727f,  51.200000762939453f,  76.800003051757812f, 102.400001525878906f, 128.000000000000000f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f, 115.200004577636719f,  51.200000762939453f,  76.800003051757812f, 102.400001525878906f,  64.000000000000000f,  89.600006103515625f,  76.800003051757812f,  25.600000381469727f,  51.200000762939453f,  76.800003051757812f, 102.400001525878906f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f,  51.200000762939453f,  76.800003051757812f,  64.000000000000000f};
	nm32f v2yExpected[] = {  0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  51.200000762939453f,  51.200000762939453f,  51.200000762939453f,  76.800003051757812f,  76.800003051757812f, 102.400001525878906f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  51.200000762939453f,  51.200000762939453f,  76.800003051757812f};
	nm32f v2zExpected[] = {  0.180000007152557f,   0.260000020265579f,   0.340000003576279f,   0.420000016689301f,   0.500000000000000f,   0.220000013709068f,   0.300000011920929f,   0.380000025033951f,   0.460000008344650f,   0.260000020265579f,   0.340000003576279f,   0.420000016689301f,   0.300000011920929f,   0.380000025033951f,   0.340000003576279f,   0.180000007152557f,   0.260000020265579f,   0.340000003576279f,   0.420000016689301f,   0.220000013709068f,   0.300000011920929f,   0.380000025033951f,   0.260000020265579f,   0.340000003576279f,   0.300000011920929f};
	nm32f v2wExpected[] = {  0.280000001192093f,   0.360000014305115f,   0.440000027418137f,   0.520000040531158f,   0.600000023841858f,   0.320000022649765f,   0.400000035762787f,   0.480000048875809f,   0.560000061988831f,   0.360000014305115f,   0.440000027418137f,   0.520000040531158f,   0.400000035762787f,   0.480000048875809f,   0.440000027418137f,   0.280000001192093f,   0.360000014305115f,   0.440000027418137f,   0.520000040531158f,   0.320000022649765f,   0.400000035762787f,   0.480000048875809f,   0.360000014305115f,   0.440000027418137f,   0.400000035762787f};
	nm32f v2sExpected[] = { 12.800000190734863f,  25.600000381469727f,  38.400001525878906f,  51.200000762939453f,  64.000000000000000f,  19.200000762939453f,  32.000000000000000f,  44.800003051757812f,  57.600002288818359f,  25.600000381469727f,  38.400001525878906f,  51.200000762939453f,  32.000000000000000f,  44.800003051757812f,  38.400001525878906f,  12.800000190734863f,  25.600000381469727f,  38.400001525878906f,  51.200000762939453f,  19.200000762939453f,  32.000000000000000f,  44.800003051757812f,  25.600000381469727f,  38.400001525878906f,  32.000000000000000f};
	nm32f v2tExpected[] = {  0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  12.800000190734863f,  12.800000190734863f,  12.800000190734863f,  12.800000190734863f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  38.400001525878906f,  38.400001525878906f,  51.200000762939453f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  12.800000190734863f,  12.800000190734863f,  12.800000190734863f,  25.600000381469727f,  25.600000381469727f,  38.400001525878906f};
	v4nm32f v2ColorExpected[] = {{  0.100000001490116f,   0.500000000000000f,   0.400000005960464f,   1.000000000000000f}, {  0.200000002980232f,   0.500000000000000f,   0.300000011920929f,   1.000000000000000f}, {  0.300000011920929f,   0.500000000000000f,   0.199999988079071f,   1.000000000000000f}, {  0.400000005960464f,   0.500000000000000f,   0.099999994039536f,   1.000000000000000f}, {  0.500000000000000f,   0.500000000000000f,   0.000000000000000f,   1.000000000000000f}, {  0.200000002980232f,   0.400000005960464f,   0.400000005960464f,   1.000000000000000f}, {  0.300000011920929f,   0.400000005960464f,   0.300000011920929f,   1.000000000000000f}, {  0.400000035762787f,   0.400000005960464f,   0.199999973177910f,   1.000000000000000f}, {  0.500000000000000f,   0.400000005960464f,   0.099999979138374f,   1.000000000000000f}, {  0.300000011920929f,   0.300000011920929f,   0.400000005960464f,   1.000000000000000f}, {  0.400000005960464f,   0.300000011920929f,   0.299999982118607f,   1.000000000000000f}, {  0.500000000000000f,   0.300000011920929f,   0.199999988079071f,   1.000000000000000f}, {  0.400000005960464f,   0.199999988079071f,   0.400000005960464f,   1.000000000000000f}, {  0.500000000000000f,   0.199999988079071f,   0.299999982118607f,   1.000000000000000f}, {  0.500000000000000f,   0.099999994039536f,   0.399999976158142f,   1.000000000000000f}, {  0.100000001490116f,   0.500000000000000f,   0.400000005960464f,   1.000000000000000f}, {  0.200000002980232f,   0.500000000000000f,   0.300000011920929f,   1.000000000000000f}, {  0.300000011920929f,   0.500000000000000f,   0.199999988079071f,   1.000000000000000f}, {  0.400000005960464f,   0.500000000000000f,   0.099999994039536f,   1.000000000000000f}, {  0.200000002980232f,   0.400000005960464f,   0.400000005960464f,   1.000000000000000f}, {  0.300000011920929f,   0.400000005960464f,   0.300000011920929f,   1.000000000000000f}, {  0.400000035762787f,   0.400000005960464f,   0.199999973177910f,   1.000000000000000f}, {  0.300000011920929f,   0.300000011920929f,   0.400000005960464f,   1.000000000000000f}, {  0.400000005960464f,   0.300000011920929f,   0.299999982118607f,   1.000000000000000f}, {  0.400000005960464f,   0.199999988079071f,   0.400000005960464f,   1.000000000000000f}};

	//Act
	res = splitTriangles_uniform(&srcTriangles, srcCount, maxWidth, maxHeight, maxDstSize, &dstTriangles, &srcTreatedCount);

	//Assert
	TEST_ARRAYS_EQUAL(dstTriangles.v0.x, v0xExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v0.y, v0yExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v0.z, v0zExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v0.w, v0wExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v0.s, v0sExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v0.t, v0tExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v0.t, v0tExpected, res);
	TEST_VEC_ARRAYS_EQUAL(dstTriangles.v0.color, v0ColorExpected, res);

	TEST_ARRAYS_EQUAL(dstTriangles.v1.x, v1xExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v1.y, v1yExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v1.z, v1zExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v1.w, v1wExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v1.s, v1sExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v1.t, v1tExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v1.t, v1tExpected, res);
	TEST_VEC_ARRAYS_EQUAL(dstTriangles.v1.color, v1ColorExpected, res);

	TEST_ARRAYS_EQUAL(dstTriangles.v2.x, v2xExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v2.y, v2yExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v2.z, v2zExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v2.w, v2wExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v2.s, v2sExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v2.t, v2tExpected, res);
	TEST_ARRAYS_EQUAL(dstTriangles.v2.t, v2tExpected, res);
	TEST_VEC_ARRAYS_EQUAL(dstTriangles.v2.color, v2ColorExpected, res);

	return 0;
}

// @case Данные в выходном массиве треугольников корректны, если на входе один треугольник, требующий деления (места в выходном массиве достаточно). Используется перспективно-корректная интерполяция
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
	// @step Включить перпективно-корректную интерполяцию (установить переменную контекста, определяющую режим интерполяции, равной NMGL_NICEST).
	// @step Создать входной и выходной массивы
	// @step Установить количество треугольников на входе равным 1 
	// @step Во входном массиве создать треугольник с шириной 128 и высотой 128, инициализировать атрибуты вершин треугольника
	// @step Установить ограничение ширины треугольника равным 32
	// @step Установить ограничение высоты треугольника равным 32
	// @step Установить максимально допустимое количество треугольников на выходе равным NMGL_SIZE (1024)
	// @step Установить количество обработанных треугольников равным -1
	// @step Установить количество треугольников в выходном массиве равным 0 
	// @step Установить ожидаемые значения атрибутов вершин выходных треугольников
	// @step Вызвать функцию splitTriangles_uniform с заданными аргументами
	// @assert Сравнить фактические и ожидаемые значения атрибутов вершин выходных треугольников
	// @pass Фактические и ожидаемые значения атрибутов вершин выходных треугольников, сравниваемые на шаге #assert, совпадают
int splitTriangles_uniform_PERSPECTIVE_CORRECT_oneInputTrianglesNeedsDivision_dstIsCorrect()
{
	constexpr int size = 1;

	NMGL_Context_NM0 *cntxt;
	NMGL_Context_NM0::create();
	cntxt = NMGL_Context_NM0::getContext();
	cntxt->buffer2 = (float*)nmglBuffer2;
	cntxt->buffer3 = (float*)nmglBuffer3;
	cntxt->buffer5 = (float*)nmglBuffer5;
	cntxt->perspectiveCorrectionHint = NMGL_NICEST;

	// Arrange
	TrianglePointers srcTriangles = {
		{x0_in, y0_in, z0_in, w0_in, s0_in, t0_in, color0_in},
		{x1_in, y1_in, z1_in, w1_in, s1_in, t1_in, color1_in},
		{x2_in, y2_in, z2_in, w2_in, s2_in, t2_in, color2_in},
	};
	TrianglePointers dstTriangles = {
		{x0_out, y0_out, z0_out, w0_out, s0_out, t0_out, color0_out},
		{x1_out, y1_out, z1_out, w1_out, s1_out, t1_out, color1_out},
		{x2_out, y2_out, z2_out, w2_out, s2_out, t2_out, color2_out},
	};

	float width = 128.0f;
	float height = width;

	for (size_t i = 0; i < size; ++i){
		x0_in[i] =  0.0f;
		y0_in[i] =  0.0f;
		z0_in[i] =  0.1f;
		w0_in[i] =  0.2f;
		s0_in[i] =  0.0f;
		t0_in[i] =  0.0f;
		color0_in[i] =  {0.0f, 0.5f, 0.5f, 1.0f};

		x1_in[i] = width / 2.0f;	// width / 2.0f = 63.9999 (not 64.0000)
		y1_in[i] = height;
		z1_in[i] =  0.3f;
		w1_in[i] =  0.4f;
		s1_in[i] =  32.0f;
		t1_in[i] =  64.0f;
		color1_in[i] =  {0.5f, 0.0f, 0.5f, 1.0f};

		x2_in[i] = width;
		y2_in[i] =  0.0f;
		z2_in[i] =  0.5f;
		w2_in[i] =  0.6f;
		s2_in[i] =  64.0f;
		t2_in[i] =  0.0f;
		color2_in[i] =  {0.5f, 0.5f, 0.0f, 1.0f};
	}

	int srcCount = size;
	int maxWidth = 32;
	int maxHeight = 32;
	int maxDstSize = NMGL_SIZE;
	int srcTreatedCount = 0;
	int res = -1;

	nm32f v0xExpected[] = {  0.000000000000000f,  25.600000381469727f,  51.200000762939453f,  76.800003051757812f, 102.400001525878906f,  12.800000190734863f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f,  25.600000381469727f,  51.200000762939453f,  76.800003051757812f,  38.400001525878906f,  64.000000000000000f,  51.200000762939453f,  12.800000190734863f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f,  25.600000381469727f,  51.200000762939453f,  76.800003051757812f,  38.400001525878906f,  64.000000000000000f,  51.200000762939453f};
	nm32f v0yExpected[] = {  0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  51.200000762939453f,  51.200000762939453f,  51.200000762939453f,  76.800003051757812f,  76.800003051757812f, 102.400001525878906f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  51.200000762939453f,  51.200000762939453f,  51.200000762939453f,  76.800003051757812f,  76.800003051757812f, 102.400001525878906f};
	nm32f v0zExpected[] = {  0.100000001490116f,   0.119047619402409f,   0.147058829665184f,   0.192307695746422f,   0.277777791023254f,   0.115384608507156f,   0.141509428620338f,   0.182926833629608f,   0.258620709180832f,   0.136363640427589f,   0.174418628215790f,   0.241935491561890f,   0.166666671633720f,   0.227272734045982f,   0.214285746216774f,   0.115384608507156f,   0.141509428620338f,   0.182926833629608f,   0.258620709180832f,   0.136363640427589f,   0.174418628215790f,   0.241935491561890f,   0.166666671633720f,   0.227272734045982f,   0.214285746216774f};
	nm32f v0wExpected[] = {  0.200000002980232f,   0.219047620892525f,   0.247058838605881f,   0.292307704687119f,   0.377777785062790f,   0.215384602546692f,   0.241509437561035f,   0.282926857471466f,   0.358620762825012f,   0.236363634467125f,   0.274418622255325f,   0.341935515403748f,   0.266666650772095f,   0.327272742986679f,   0.314285755157471f,   0.215384602546692f,   0.241509437561035f,   0.282926857471466f,   0.358620762825012f,   0.236363634467125f,   0.274418622255325f,   0.341935515403748f,   0.266666650772095f,   0.327272742986679f,   0.314285755157471f};
	nm32f v0sExpected[] = {  0.000000000000000f,   3.047619104385376f,   7.529412746429443f,  14.769231796264648f,  28.444446563720703f,   2.461538314819336f,   6.641509532928467f,  13.268293380737305f,  25.379314422607422f,   5.818181991577148f,  11.906977653503418f,  22.709678649902344f,  10.666666984558105f,  20.363637924194336f,  18.285717010498047f,   2.461538314819336f,   6.641509532928467f,  13.268293380737305f,  25.379314422607422f,   5.818181991577148f,  11.906977653503418f,  22.709678649902344f,  10.666666984558105f,  20.363637924194336f,  18.285717010498047f};
	nm32f v0tExpected[] = {  0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   4.923076629638672f,   6.037735939025879f,   7.804878711700439f,  11.034484863281250f,  11.636363983154297f,  14.883723258972168f,  20.645162582397461f,  21.333333969116211f,  29.090909957885742f,  36.571434020996094f,   4.923076629638672f,   6.037735939025879f,   7.804878711700439f,  11.034484863281250f,  11.636363983154297f,  14.883723258972168f,  20.645162582397461f,  21.333333969116211f,  29.090909957885742f,  36.571434020996094f};
	v4nm32f v0ColorExpected[] = {{  0.000000000000000f,   0.500000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.023809524253011f,   0.500000000000000f,   0.476190477609634f,   1.000000000000000f}, {  0.058823537081480f,   0.500000000000000f,   0.441176503896713f,   1.000000000000000f}, {  0.115384623408318f,   0.500000000000000f,   0.384615391492844f,   1.000000000000000f}, {  0.222222238779068f,   0.499999970197678f,   0.277777791023254f,   0.999999940395355f}, {  0.038461536169052f,   0.461538434028625f,   0.500000000000000f,   1.000000000000000f}, {  0.075471699237823f,   0.452830165624619f,   0.471698135137558f,   1.000000000000000f}, {  0.134146332740784f,   0.439024418592453f,   0.426829278469086f,   1.000000000000000f}, {  0.241379335522652f,   0.413793146610260f,   0.344827532768250f,   1.000000000000000f}, {  0.090909093618393f,   0.409090936183929f,   0.500000000000000f,   1.000000000000000f}, {  0.151162803173065f,   0.383720964193344f,   0.465116351842880f,   1.000000119209290f}, {  0.258064538240433f,   0.338709652423859f,   0.403225839138031f,   1.000000000000000f}, {  0.166666671633720f,   0.333333343267441f,   0.500000000000000f,   1.000000000000000f}, {  0.272727280855179f,   0.272727251052856f,   0.454545468091965f,   1.000000000000000f}, {  0.285714328289032f,   0.214285746216774f,   0.500000059604645f,   1.000000119209290f}, {  0.038461536169052f,   0.461538434028625f,   0.500000000000000f,   1.000000000000000f}, {  0.075471699237823f,   0.452830165624619f,   0.471698135137558f,   1.000000000000000f}, {  0.134146332740784f,   0.439024418592453f,   0.426829278469086f,   1.000000000000000f}, {  0.241379335522652f,   0.413793146610260f,   0.344827532768250f,   1.000000000000000f}, {  0.090909093618393f,   0.409090936183929f,   0.500000000000000f,   1.000000000000000f}, {  0.151162803173065f,   0.383720964193344f,   0.465116351842880f,   1.000000119209290f}, {  0.258064538240433f,   0.338709652423859f,   0.403225839138031f,   1.000000000000000f}, {  0.166666671633720f,   0.333333343267441f,   0.500000000000000f,   1.000000000000000f}, {  0.272727280855179f,   0.272727251052856f,   0.454545468091965f,   1.000000000000000f}, {  0.285714328289032f,   0.214285746216774f,   0.500000059604645f,   1.000000119209290f}};
	
	nm32f v1xExpected[] = { 12.800000190734863f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f, 115.200004577636719f,  25.600000381469727f,  51.200000762939453f,  76.800003051757812f, 102.400001525878906f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f,  51.200000762939453f,  76.800003051757812f,  64.000000000000000f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f, 115.200004577636719f,  51.200000762939453f,  76.800003051757812f, 102.400001525878906f,  64.000000000000000f,  89.600006103515625f,  76.800003051757812f};
	nm32f v1yExpected[] = { 25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  51.200000762939453f,  51.200000762939453f,  51.200000762939453f,  51.200000762939453f,  76.800003051757812f,  76.800003051757812f,  76.800003051757812f, 102.400001525878906f, 102.400001525878906f, 128.000000000000000f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  51.200000762939453f,  51.200000762939453f,  51.200000762939453f,  76.800003051757812f,  76.800003051757812f, 102.400001525878906f};
	nm32f v1zExpected[] = {  0.115384608507156f,   0.141509428620338f,   0.182926833629608f,   0.258620709180832f,   0.441176563501358f,   0.136363640427589f,   0.174418628215790f,   0.241935491561890f,   0.394736886024475f,   0.166666671633720f,   0.227272734045982f,   0.357142925262451f,   0.214285746216774f,   0.326086997985840f,   0.300000041723251f,   0.141509428620338f,   0.182926833629608f,   0.258620709180832f,   0.441176563501358f,   0.174418628215790f,   0.241935491561890f,   0.394736886024475f,   0.227272734045982f,   0.357142925262451f,   0.326086997985840f};
	nm32f v1wExpected[] = {  0.215384602546692f,   0.241509437561035f,   0.282926857471466f,   0.358620762825012f,   0.541176617145538f,   0.236363634467125f,   0.274418622255325f,   0.341935515403748f,   0.494736939668655f,   0.266666650772095f,   0.327272742986679f,   0.457142919301987f,   0.314285755157471f,   0.426086992025375f,   0.400000065565109f,   0.241509437561035f,   0.282926857471466f,   0.358620762825012f,   0.541176617145538f,   0.274418622255325f,   0.341935515403748f,   0.494736939668655f,   0.327272742986679f,   0.457142919301987f,   0.426086992025375f};
	nm32f v1sExpected[] = {  2.461538314819336f,   6.641509532928467f,  13.268293380737305f,  25.379314422607422f,  54.588249206542969f,   5.818181991577148f,  11.906977653503418f,  22.709678649902344f,  47.157905578613281f,  10.666666984558105f,  20.363637924194336f,  41.142864227294922f,  18.285717010498047f,  36.173915863037109f,  32.000003814697266f,   6.641509532928467f,  13.268293380737305f,  25.379314422607422f,  54.588249206542969f,  11.906977653503418f,  22.709678649902344f,  47.157905578613281f,  20.363637924194336f,  41.142864227294922f,  36.173915863037109f};
	nm32f v1tExpected[] = {  4.923076629638672f,   6.037735939025879f,   7.804878711700439f,  11.034484863281250f,  18.823534011840820f,  11.636363983154297f,  14.883723258972168f,  20.645162582397461f,  33.684215545654297f,  21.333333969116211f,  29.090909957885742f,  45.714294433593750f,  36.571434020996094f,  55.652183532714844f,  64.000007629394531f,   6.037735939025879f,   7.804878711700439f,  11.034484863281250f,  18.823534011840820f,  14.883723258972168f,  20.645162582397461f,  33.684215545654297f,  29.090909957885742f,  45.714294433593750f,  55.652183532714844f};
	v4nm32f v1ColorExpected[] = {{  0.038461536169052f,   0.461538434028625f,   0.500000000000000f,   1.000000000000000f}, {  0.075471699237823f,   0.452830165624619f,   0.471698135137558f,   1.000000000000000f}, {  0.134146332740784f,   0.439024418592453f,   0.426829278469086f,   1.000000000000000f}, {  0.241379335522652f,   0.413793146610260f,   0.344827532768250f,   1.000000000000000f}, {  0.500000119209290f,   0.352941066026688f,   0.147058933973312f,   1.000000000000000f}, {  0.090909093618393f,   0.409090936183929f,   0.500000000000000f,   1.000000000000000f}, {  0.151162803173065f,   0.383720964193344f,   0.465116351842880f,   1.000000119209290f}, {  0.258064538240433f,   0.338709652423859f,   0.403225839138031f,   1.000000000000000f}, {  0.500000059604645f,   0.236842021346092f,   0.263157874345779f,   0.999999940395355f}, {  0.166666671633720f,   0.333333343267441f,   0.500000000000000f,   1.000000000000000f}, {  0.272727280855179f,   0.272727251052856f,   0.454545468091965f,   1.000000000000000f}, {  0.500000119209290f,   0.142857089638710f,   0.357142835855484f,   1.000000000000000f}, {  0.285714328289032f,   0.214285746216774f,   0.500000059604645f,   1.000000119209290f}, {  0.500000059604645f,   0.065217368304729f,   0.434782624244690f,   1.000000119209290f}, {  0.500000059604645f,   0.000000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.075471699237823f,   0.452830165624619f,   0.471698135137558f,   1.000000000000000f}, {  0.134146332740784f,   0.439024418592453f,   0.426829278469086f,   1.000000000000000f}, {  0.241379335522652f,   0.413793146610260f,   0.344827532768250f,   1.000000000000000f}, {  0.500000119209290f,   0.352941066026688f,   0.147058933973312f,   1.000000000000000f}, {  0.151162803173065f,   0.383720964193344f,   0.465116351842880f,   1.000000119209290f}, {  0.258064538240433f,   0.338709652423859f,   0.403225839138031f,   1.000000000000000f}, {  0.500000059604645f,   0.236842021346092f,   0.263157874345779f,   0.999999940395355f}, {  0.272727280855179f,   0.272727251052856f,   0.454545468091965f,   1.000000000000000f}, {  0.500000119209290f,   0.142857089638710f,   0.357142835855484f,   1.000000000000000f}, {  0.500000059604645f,   0.065217368304729f,   0.434782624244690f,   1.000000119209290f}};
	
	nm32f v2xExpected[] = { 25.600000381469727f,  51.200000762939453f,  76.800003051757812f, 102.400001525878906f, 128.000000000000000f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f, 115.200004577636719f,  51.200000762939453f,  76.800003051757812f, 102.400001525878906f,  64.000000000000000f,  89.600006103515625f,  76.800003051757812f,  25.600000381469727f,  51.200000762939453f,  76.800003051757812f, 102.400001525878906f,  38.400001525878906f,  64.000000000000000f,  89.600006103515625f,  51.200000762939453f,  76.800003051757812f,  64.000000000000000f};
	nm32f v2yExpected[] = {  0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  51.200000762939453f,  51.200000762939453f,  51.200000762939453f,  76.800003051757812f,  76.800003051757812f, 102.400001525878906f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  25.600000381469727f,  25.600000381469727f,  25.600000381469727f,  51.200000762939453f,  51.200000762939453f,  76.800003051757812f};
	nm32f v2zExpected[] = {  0.119047619402409f,   0.147058829665184f,   0.192307695746422f,   0.277777791023254f,   0.500000000000000f,   0.141509428620338f,   0.182926833629608f,   0.258620709180832f,   0.441176563501358f,   0.174418628215790f,   0.241935491561890f,   0.394736886024475f,   0.227272734045982f,   0.357142925262451f,   0.326086997985840f,   0.119047619402409f,   0.147058829665184f,   0.192307695746422f,   0.277777791023254f,   0.141509428620338f,   0.182926833629608f,   0.258620709180832f,   0.174418628215790f,   0.241935491561890f,   0.227272734045982f};
	nm32f v2wExpected[] = {  0.219047620892525f,   0.247058838605881f,   0.292307704687119f,   0.377777785062790f,   0.600000023841858f,   0.241509437561035f,   0.282926857471466f,   0.358620762825012f,   0.541176617145538f,   0.274418622255325f,   0.341935515403748f,   0.494736939668655f,   0.327272742986679f,   0.457142919301987f,   0.426086992025375f,   0.219047620892525f,   0.247058838605881f,   0.292307704687119f,   0.377777785062790f,   0.241509437561035f,   0.282926857471466f,   0.358620762825012f,   0.274418622255325f,   0.341935515403748f,   0.327272742986679f};
	nm32f v2sExpected[] = {  3.047619104385376f,   7.529412746429443f,  14.769231796264648f,  28.444446563720703f,  64.000000000000000f,   6.641509532928467f,  13.268293380737305f,  25.379314422607422f,  54.588249206542969f,  11.906977653503418f,  22.709678649902344f,  47.157905578613281f,  20.363637924194336f,  41.142864227294922f,  36.173915863037109f,   3.047619104385376f,   7.529412746429443f,  14.769231796264648f,  28.444446563720703f,   6.641509532928467f,  13.268293380737305f,  25.379314422607422f,  11.906977653503418f,  22.709678649902344f,  20.363637924194336f};
	nm32f v2tExpected[] = {  0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   6.037735939025879f,   7.804878711700439f,  11.034484863281250f,  18.823534011840820f,  14.883723258972168f,  20.645162582397461f,  33.684215545654297f,  29.090909957885742f,  45.714294433593750f,  55.652183532714844f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   6.037735939025879f,   7.804878711700439f,  11.034484863281250f,  14.883723258972168f,  20.645162582397461f,  29.090909957885742f};
	v4nm32f v2ColorExpected[] = {{  0.023809524253011f,   0.500000000000000f,   0.476190477609634f,   1.000000000000000f}, {  0.058823537081480f,   0.500000000000000f,   0.441176503896713f,   1.000000000000000f}, {  0.115384623408318f,   0.500000000000000f,   0.384615391492844f,   1.000000000000000f}, {  0.222222238779068f,   0.499999970197678f,   0.277777791023254f,   0.999999940395355f}, {  0.500000000000000f,   0.500000000000000f,   0.000000000000000f,   1.000000000000000f}, {  0.075471699237823f,   0.452830165624619f,   0.471698135137558f,   1.000000000000000f}, {  0.134146332740784f,   0.439024418592453f,   0.426829278469086f,   1.000000000000000f}, {  0.241379335522652f,   0.413793146610260f,   0.344827532768250f,   1.000000000000000f}, {  0.500000119209290f,   0.352941066026688f,   0.147058933973312f,   1.000000000000000f}, {  0.151162803173065f,   0.383720964193344f,   0.465116351842880f,   1.000000119209290f}, {  0.258064538240433f,   0.338709652423859f,   0.403225839138031f,   1.000000000000000f}, {  0.500000059604645f,   0.236842021346092f,   0.263157874345779f,   0.999999940395355f}, {  0.272727280855179f,   0.272727251052856f,   0.454545468091965f,   1.000000000000000f}, {  0.500000119209290f,   0.142857089638710f,   0.357142835855484f,   1.000000000000000f}, {  0.500000059604645f,   0.065217368304729f,   0.434782624244690f,   1.000000119209290f}, {  0.023809524253011f,   0.500000000000000f,   0.476190477609634f,   1.000000000000000f}, {  0.058823537081480f,   0.500000000000000f,   0.441176503896713f,   1.000000000000000f}, {  0.115384623408318f,   0.500000000000000f,   0.384615391492844f,   1.000000000000000f}, {  0.222222238779068f,   0.499999970197678f,   0.277777791023254f,   0.999999940395355f}, {  0.075471699237823f,   0.452830165624619f,   0.471698135137558f,   1.000000000000000f}, {  0.134146332740784f,   0.439024418592453f,   0.426829278469086f,   1.000000000000000f}, {  0.241379335522652f,   0.413793146610260f,   0.344827532768250f,   1.000000000000000f}, {  0.151162803173065f,   0.383720964193344f,   0.465116351842880f,   1.000000119209290f}, {  0.258064538240433f,   0.338709652423859f,   0.403225839138031f,   1.000000000000000f}, {  0.272727280855179f,   0.272727251052856f,   0.454545468091965f,   1.000000000000000f}};

	//Act
	res = splitTriangles_uniform(&srcTriangles, srcCount, maxWidth, maxHeight, maxDstSize, &dstTriangles, &srcTreatedCount);

	//Assert
	const float epsilon = 1.0e-06f;
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v0.x, v0xExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v0.y, v0yExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v0.z, v0zExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v0.w, v0wExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v0.s, v0sExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v0.t, v0tExpected, res, epsilon);
	TEST_VEC_ARRAYS_EQUAL_PRECISION(dstTriangles.v0.color, v0ColorExpected, res, epsilon);

	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v1.x, v1xExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v1.y, v1yExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v1.z, v1zExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v1.w, v1wExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v1.s, v1sExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v1.t, v1tExpected, res, epsilon);
	TEST_VEC_ARRAYS_EQUAL_PRECISION(dstTriangles.v1.color, v1ColorExpected, res, epsilon);

	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v2.x, v2xExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v2.y, v2yExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v2.z, v2zExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v2.w, v2wExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v2.s, v2sExpected, res, epsilon);
	TEST_ARRAYS_EQUAL_PRECISION(dstTriangles.v2.t, v2tExpected, res, epsilon);
	TEST_VEC_ARRAYS_EQUAL_PRECISION(dstTriangles.v2.color, v2ColorExpected, res, epsilon);

	return 0;
}

clock_t splitTriangles_uniform_nInputsNoDivision_nOutputs(int n, NMGLenum hint)
{
	size_t size = n;

	NMGL_Context_NM0 *cntxt;
	NMGL_Context_NM0::create();
	cntxt = NMGL_Context_NM0::getContext();
	cntxt->buffer2 = (float*)nmglBuffer2;
	cntxt->buffer3 = (float*)nmglBuffer3;
	cntxt->buffer5 = (float*)nmglBuffer5;
	cntxt->perspectiveCorrectionHint = hint;

	// Arrange
	TrianglePointers srcTriangles = {
		{x0_in, y0_in, z0_in, w0_in, s0_in, t0_in, color0_in},
		{x1_in, y1_in, z1_in, w1_in, s1_in, t1_in, color1_in},
		{x2_in, y2_in, z2_in, w2_in, s2_in, t2_in, color2_in},
	};
	TrianglePointers dstTriangles = {
		{x0_out, y0_out, z0_out, w0_out, s0_out, t0_out, color0_out},
		{x1_out, y1_out, z1_out, w1_out, s1_out, t1_out, color1_out},
		{x2_out, y2_out, z2_out, w2_out, s2_out, t2_out, color2_out},
	};

	for (size_t i = 0; i < size; ++i){
		x0_in[i] =  0.0f;
		y0_in[i] =  0.0f;

		x1_in[i] = 15.0f;
		y1_in[i] = 30.0f;

		x2_in[i] = 30.0f;
		y2_in[i] =  0.0f;
	}

	
	int srcCount = size;
	int maxWidth = 32;
	int maxHeight = 32;
	int maxDstSize = size;
	int srcTreatedCount = 0;
	int res;

	clock_t t1, t2, dt;
	//Act
	t1 = clock();
	res = splitTriangles_uniform(&srcTriangles, srcCount, maxWidth, maxHeight, maxDstSize, &dstTriangles, &srcTreatedCount);
	t2 = clock();
	dt = t2 - t1;

	return dt;
}

clock_t splitTriangles_uniform_isoscelesTrianglesNeedsDivision(float width, int *treated, int *dstSize, NMGLenum hint)
{
	size_t size = 1;

	NMGL_Context_NM0 *cntxt;
	NMGL_Context_NM0::create();
	cntxt = NMGL_Context_NM0::getContext();
	cntxt->buffer2 = (float*)nmglBuffer2;
	cntxt->buffer3 = (float*)nmglBuffer3;
	cntxt->buffer5 = (float*)nmglBuffer5;
	cntxt->perspectiveCorrectionHint = hint;

	// Arrange

	TrianglePointers srcTriangles = {
		{x0_in, y0_in, z0_in, w0_in, s0_in, t0_in, color0_in},
		{x1_in, y1_in, z1_in, w1_in, s1_in, t1_in, color1_in},
		{x2_in, y2_in, z2_in, w2_in, s2_in, t2_in, color2_in},
	};
	TrianglePointers dstTriangles = {
		{x0_out, y0_out, z0_out, w0_out, s0_out, t0_out, color0_out},
		{x1_out, y1_out, z1_out, w1_out, s1_out, t1_out, color1_out},
		{x2_out, y2_out, z2_out, w2_out, s2_out, t2_out, color2_out},
	};

	float height = width;
	for (size_t i = 0; i < size; ++i){
		x0_in[i] =  0.0f;
		y0_in[i] =  0.0f;

		x1_in[i] = width / 2;
		y1_in[i] = height;

		x2_in[i] = width;
		y2_in[i] =   0.0f;
	}

	
	int srcCount = size;
	int maxWidth = 32;
	int maxHeight = 32;
	int maxDstSize = NMGL_SIZE;
	int srcTreatedCount = 0;

	clock_t t1, t2, dt;
	//Act
	t1 = clock();
	*dstSize = splitTriangles_uniform(&srcTriangles, srcCount, maxWidth, maxHeight, maxDstSize, &dstTriangles, &srcTreatedCount);
	*treated = srcTreatedCount;
	t2 = clock();
	dt = t2 - t1;

	return dt;
}

clock_t splitTriangles_uniform_narrowTriangleNeedsDivision(float width, int *treated, int *dstSize, NMGLenum hint)
{
	size_t size = 1;

	NMGL_Context_NM0 *cntxt;
	NMGL_Context_NM0::create();
	cntxt = NMGL_Context_NM0::getContext();
	cntxt->buffer2 = (float*)nmglBuffer2;
	cntxt->buffer3 = (float*)nmglBuffer3;
	cntxt->buffer5 = (float*)nmglBuffer5;
	cntxt->perspectiveCorrectionHint = hint;

	// Arrange

	TrianglePointers srcTriangles = {
		{x0_in, y0_in, z0_in, w0_in, s0_in, t0_in, color0_in},
		{x1_in, y1_in, z1_in, w1_in, s1_in, t1_in, color1_in},
		{x2_in, y2_in, z2_in, w2_in, s2_in, t2_in, color2_in},
	};
	TrianglePointers dstTriangles = {
		{x0_out, y0_out, z0_out, w0_out, s0_out, t0_out, color0_out},
		{x1_out, y1_out, z1_out, w1_out, s1_out, t1_out, color1_out},
		{x2_out, y2_out, z2_out, w2_out, s2_out, t2_out, color2_out},
	};

	float height = 30;
	for (size_t i = 0; i < size; ++i){
		x0_in[i] =  0.0f;
		y0_in[i] =  0.0f;

		x1_in[i] = width / 2;
		y1_in[i] = height;

		x2_in[i] = width;
		y2_in[i] =   0.0f;
	}

	
	int srcCount = size;
	int maxWidth = 32;
	int maxHeight = 32;
	int maxDstSize = NMGL_SIZE;
	int srcTreatedCount = 0;

	clock_t t1, t2, dt;
	//Act
	t1 = clock();
	*dstSize = splitTriangles_uniform(&srcTriangles, srcCount, maxWidth, maxHeight, maxDstSize, &dstTriangles, &srcTreatedCount);
	*treated = srcTreatedCount;
	t2 = clock();
	dt = t2 - t1;

	return dt;
}

// Performance tests

int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_0_to_1024InputsNoDivision_0_to_1024Outputs()
{
	puts("N\tTime");
	for (int i = -1; i <= 10; ++i){
		int n = (i < 0)? 0: (int) pow(2.0, (double) i);
		clock_t dt = splitTriangles_uniform_nInputsNoDivision_nOutputs(n, NMGL_DONT_CARE);
		printf("%i\t%i\r\n", n, (int) dt);
	}
	return 0;
}

int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_isoscelesTrianglesNeedsDivisionWidth_40_to_760()
{
	puts("Treated\tSize\tM\tTime");
	for (float w = 40.0f; w <= 760.0f; w += 40.0f){
		int treatedCount = 0;
		int dstSize = 0;
		clock_t dt = splitTriangles_uniform_isoscelesTrianglesNeedsDivision(w, &treatedCount, &dstSize, NMGL_DONT_CARE);
		printf("%i\t%.2f\t%i\t%i\n\r", treatedCount, w, dstSize, (int) dt);
	}
	return 0;
}

int splitTriangles_uniform_NON_PERSPECTIVE_CORRECT_narrowTriangleNeedsDivisionWidth_40_to_760()
{
	puts("Treated\tSize\tM\tTime");
	for (float w = 40.0f; w <= 760.0f; w += 40.0f){
		int treatedCount = 0;
		int dstSize = 0;
		clock_t dt = splitTriangles_uniform_narrowTriangleNeedsDivision(w, &treatedCount, &dstSize, NMGL_DONT_CARE);
		printf("%i\t%.2f\t%i\t%i\n\r", treatedCount, w, dstSize, (int) dt);
	}
	return 0;
}

int splitTriangles_uniform_PERSPECTIVE_CORRECT_0_to_1024InputsNoDivision_0_to_1024Outputs()
{
	puts("N\tTime");
	for (int i = -1; i <= 10; ++i){
		int n = (i < 0)? 0: (int) pow(2.0, (double) i);
		clock_t dt = splitTriangles_uniform_nInputsNoDivision_nOutputs(n, NMGL_NICEST);
		printf("%i\t%i\r\n", n, (int) dt);
	}
	return 0;
}

int splitTriangles_uniform_PERSPECTIVE_CORRECT_isoscelesTrianglesNeedsDivisionWidth_40_to_760()
{
	puts("Treated\tSize\tM\tTime");
	for (float w = 40.0f; w <= 760.0f; w += 40.0f){
		int treatedCount = 0;
		int dstSize = 0;
		clock_t dt = splitTriangles_uniform_isoscelesTrianglesNeedsDivision(w, &treatedCount, &dstSize, NMGL_NICEST);
		printf("%i\t%.2f\t%i\t%i\n\r", treatedCount, w, dstSize, (int) dt);
	}
	return 0;
}

int splitTriangles_uniform_PERSPECTIVE_CORRECT_narrowTriangleNeedsDivisionWidth_40_to_760()
{
	puts("Treated\tSize\tM\tTime");
	for (float w = 40.0f; w <= 760.0f; w += 40.0f){
		int treatedCount = 0;
		int dstSize = 0;
		clock_t dt = splitTriangles_uniform_narrowTriangleNeedsDivision(w, &treatedCount, &dstSize, NMGL_NICEST);
		printf("%i\t%.2f\t%i\t%i\n\r", treatedCount, w, dstSize, (int) dt);
	}
	return 0;
}

clock_t fillArray()
{
	NMGL_Context_NM0 *cntxt;
	NMGL_Context_NM0::create();
	cntxt = NMGL_Context_NM0::getContext();
	cntxt->buffer2 = (float*)nmglBuffer2;

	nm32f *ones = cntxt->buffer2 +  8 * NMGL_SIZE;


	clock_t t1, t2, dt;
	//Act
	t1 = clock();
#if 0
	for (size_t kk = 0; kk < NMGL_SIZE; ++kk){
		ones[kk] = 1.0f;
	}
#else
	nm32f one = 1.0f;
	nmblas_scopy(NMGL_SIZE, &one, 0, ones, 1);
#endif
	t2 = clock();
	dt = t2 - t1;

	printf("%i\n", (int) dt);

	return dt;
}
