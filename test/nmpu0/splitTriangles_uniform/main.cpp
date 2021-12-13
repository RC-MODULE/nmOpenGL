#include <vector>
#include <iostream>
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <math.h>

#include "nmtype.h"
#include "service.h"
#include "tests.h"
#include "demo3d_nm0.h"

// Performance testing
#include "time.h"

int splitTriangles_uniform_EmptyInput_NoTrianglesAtOutput();
int splitTriangles_uniform_EmptyInput_NoTreatedTriangles();
int splitTriangles_uniform_oneInputTriangleNeedsDivision_dstSizeIsCorrect();
int splitTriangles_uniform_oneInputTriangleNeedsDivision_srcTreatedIsCorrect();
int splitTriangles_uniform_oneInputTrianglesNeedsDivision_dstIsCorrect();

// Performance tests
clock_t splitTriangles_isoscelesTrianglesNeedsDivision(float width, int *treated, int *dstSize);
clock_t splitTriangles_narrowTriangleNeedsDivision(float width, int *treated, int *dstSize);
clock_t splitTriangles_nInputsNoDivision_nOutputs(int n);

int main(int argc, char **argv)
{
	puts("\nsplitTriangles_uniform functional tests: ");

	RUN_TEST(splitTriangles_uniform_EmptyInput_NoTrianglesAtOutput);
	RUN_TEST(splitTriangles_uniform_EmptyInput_NoTreatedTriangles);
	RUN_TEST(splitTriangles_uniform_oneInputTriangleNeedsDivision_dstSizeIsCorrect);
	RUN_TEST(splitTriangles_uniform_oneInputTriangleNeedsDivision_srcTreatedIsCorrect);
	RUN_TEST(splitTriangles_uniform_oneInputTrianglesNeedsDivision_dstIsCorrect);

	puts("\nsplitTriangles_uniform performance tests: ");

	puts("\nN input small triangles, no division, N output triangles:");
	puts("N\tTime");
	for (int i = -1; i <= 10; ++i){
		int n = (i < 0)? 0: (int) pow(2.0, (double) i);
		clock_t dt = splitTriangles_nInputsNoDivision_nOutputs(n);
		printf("%i\t%i\n\r", n, (int) dt);
	}

	puts("\nN = 1 input triangles, needs division, M output triangles:");
	puts("Treated\tSize\tM\tTime");
	for (float w = 40.0f; w <= 760.0f; w += 40.0f){
		int treatedCount = 0;
		int dstSize = 0;
		clock_t dt = splitTriangles_isoscelesTrianglesNeedsDivision(w, &treatedCount, &dstSize);
		printf("%i\t%.2f\t%i\t%i\n\r", treatedCount, w, dstSize, (int) dt);
	}
	
	puts("\nN = 1 input narrow triangles (height is OK, width is big), needs division, M output triangles:");
	puts("Treated\tSize\tM\tTime");
	for (float w = 40.0f; w <= 760.0f; w += 40.0f){
		int treatedCount = 0;
		int dstSize = 0;
		clock_t dt = splitTriangles_narrowTriangleNeedsDivision(w, &treatedCount, &dstSize);
		printf("%i\t%.2f\t%i\t%i\n\r", treatedCount, w, dstSize, (int) dt);
	}

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

// @case Количество треугольников в выходном массиве равно нулю при отсутствии треугольников на входе
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
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
int splitTriangles_uniform_EmptyInput_NoTrianglesAtOutput()
{
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

// @case Количество обработанных треугольников равно нулю при отсутствии треугольников на входе
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
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
int splitTriangles_uniform_EmptyInput_NoTreatedTriangles()
{
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

// @case Количество треугольников в выходном массиве корректно, если на входе один треугольник, требующий деления (места в выходном массиве достаточно)
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
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
int splitTriangles_uniform_oneInputTriangleNeedsDivision_dstSizeIsCorrect()
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
	int resExpected = (int) pow(ceil(std::max(width / maxWidth, height / maxHeight)), 2.0);

	//Act
	res = splitTriangles_uniform(&srcTriangles, srcCount, maxWidth, maxHeight, maxDstSize, &dstTriangles, &srcTreatedCount);

	//Assert
	TEST_ASSERT(res == resExpected);

	return 0;
}

// @case Количество обработанных треугольников корректно, если на входе один треугольник, требующий деления (места в выходном массиве достаточно)
	// @step Инициализировать структуры, используемые тестирумой функцией (контекст, вспомогательные буферы)
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
int splitTriangles_uniform_oneInputTriangleNeedsDivision_srcTreatedIsCorrect()
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

typedef enum {	
				V0_X, V0_Y, V0_Z, V0_W, V0_COLOR, V0_S, V0_T,
				V1_X, V1_Y, V1_Z, V1_W, V1_COLOR, V1_S, V1_T, 
				V2_X, V2_Y, V2_Z, V2_W, V2_COLOR, V2_S, V2_T, 
} attrib_t;

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
int splitTriangles_uniform_oneInputTrianglesNeedsDivision_dstIsCorrect()
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

	//nm32f   v0xExpected[] = 	{0.00, 32.00, 64.00, 96.00, 16.00, 48.00, 80.00, 32.00, 64.00, 48.00, 16.00, 48.00, 80.00, 32.00, 64.00, 48.00};
	//nm32f   v0yExpected[] = 	{0.00,  0.00,  0.00,  0.00, 32.00, 32.00, 32.00, 64.00, 64.00, 96.00, 32.00, 32.00, 32.00, 64.00, 64.00, 96.00};
	//nm32f   v0zExpected[] = 	{0.10,  0.20,  0.30,  0.40,  0.15,  0.25,  0.35,  0.20,  0.30,  0.25,  0.15,  0.25,  0.35,  0.20,  0.30,  0.25};
	//nm32f   v0wExpected[] = 	{0.20,  0.30,  0.40,  0.50,  0.25,  0.35,  0.45,  0.30,  0.40,  0.35,  0.25,  0.35,  0.45,  0.30,  0.40,  0.35};
	//nm32f   v0sExpected[] = 	{0.00, 16.00, 32.00, 48.00,  8.00, 24.00, 40.00, 16.00, 32.00, 24.00,  8.00, 24.00, 40.00, 16.00, 32.00, 24.00};
	//nm32f   v0tExpected[] = 	{0.00,  0.00,  0.00,  0.00, 16.00, 16.00, 16.00, 32.00, 32.00, 48.00, 16.00, 16.00, 16.00, 32.00, 32.00, 48.00};
	//v4nm32f v0ColorExpected[] = {{0.0, 0.5, 0.5, 1.0}, {0.125, 0.5, 0.375, 1.0}, {0.25, 0.5, 0.25, 1.0}, {0.375, 0.5, 0.125, 1.0}, {0.125, 0.375, 0.5, 1.0}, {0.25, 0.375, 0.375, 1.0}, {0.375, 0.375, 0.25, 1.0}, {0.25, 0.25, 0.5, 1.0}, {0.375, 0.25, 0.375, 1.0}, {0.375, 0.125, 0.5, 1.0}, {0.125, 0.375, 0.5, 1.0}, {0.25, 0.375, 0.375, 1.0}, {0.375, 0.375, 0.25, 1.0}, {0.25, 0.25, 0.5, 1.0}, {0.375, 0.25, 0.375, 1.0}, {0.375, 0.125, 0.5, 1.0}};

	//nm32f   v1xExpected[] = 	{16.00, 48.00, 80.00, 112.00, 32.00, 64.00, 96.00, 48.00, 80.00,  64.00, 48.00, 80.00, 112.00, 64.00, 96.00, 80.00};
	//nm32f   v1yExpected[] = 	{32.00, 32.00, 32.00,  32.00, 64.00, 64.00, 64.00, 96.00, 96.00, 128.00, 32.00, 32.00,  32.00, 64.00, 64.00, 96.00};
	//nm32f   v1zExpected[] = 	{ 0.15,  0.25,  0.35,   0.45,  0.20,  0.30,  0.40,  0.25,  0.35,   0.30,  0.25,  0.35,   0.45,  0.30,  0.40,  0.35};
	//nm32f   v1wExpected[] = 	{ 0.25,  0.35,  0.45,   0.55,  0.30,  0.40,  0.50,  0.35,  0.45,   0.40,  0.35,  0.45,   0.55,  0.40,  0.50,  0.45};
	//nm32f   v1sExpected[] = 	{ 8.00, 24.00, 40.00,  56.00, 16.00, 32.00, 48.00, 24.00, 40.00,  32.00, 24.00, 40.00,  56.00, 32.00, 48.00, 40.00};
	//nm32f   v1tExpected[] = 	{16.00, 16.00, 16.00,  16.00, 32.00, 32.00, 32.00, 48.00, 48.00,  64.00, 16.00, 16.00,  16.00, 32.00, 32.00, 48.00};
	//v4nm32f v1ColorExpected[] = {{0.125, 0.375, 0.5, 1.0}, {0.25, 0.375, 0.375, 1.0}, {0.375, 0.375, 0.25, 1.0}, {0.5, 0.375, 0.125, 1.0}, {0.25, 0.25, 0.5, 1.0}, {0.375, 0.25, 0.375, 1.0}, {0.5, 0.25, 0.25, 1.0}, {0.375, 0.125, 0.5, 1.0}, {0.5, 0.125, 0.375, 1.0}, {0.5, 0.0, 0.5, 1.0}, {0.25, 0.375, 0.375, 1.0}, {0.375, 0.375, 0.25, 1.0}, {0.5, 0.375, 0.125, 1.0}, {0.375, 0.25, 0.375, 1.0}, {0.5, 0.25, 0.25, 1.0}, {0.5, 0.125, 0.375, 1.0}};

	//nm32f   v2xExpected[] = 	{32.00, 64.00, 96.00, 128.00, 48.00, 80.00, 112.00, 64.00, 96.00, 80.00, 32.00, 64.00, 96.00, 48.00, 80.00, 64.00};
	//nm32f   v2yExpected[] = 	{ 0.00,  0.00,  0.00,   0.00, 32.00, 32.00,  32.00, 64.00, 64.00, 96.00,  0.00,  0.00,  0.00, 32.00, 32.00, 64.00};
	//nm32f   v2zExpected[] = 	{ 0.20,  0.30,  0.40,   0.50,  0.25,  0.35,   0.45,  0.30,  0.40,  0.35,  0.20,  0.30,  0.40,  0.25,  0.35,  0.30};
	//nm32f   v2wExpected[] = 	{ 0.30,  0.40,  0.50,   0.60,  0.35,  0.45,   0.55,  0.40,  0.50,  0.45,  0.30,  0.40,  0.50,  0.35,  0.45,  0.40};
	//nm32f   v2sExpected[] = 	{16.00, 32.00, 48.00,  64.00, 24.00, 40.00,  56.00, 32.00, 48.00, 40.00, 16.00, 32.00, 48.00, 24.00, 40.00, 32.00};
	//nm32f   v2tExpected[] = 	{ 0.00,  0.00,  0.00,   0.00, 16.00, 16.00,  16.00, 32.00, 32.00, 48.00,  0.00,  0.00,  0.00, 16.00, 16.00, 32.00};
	//v4nm32f v2ColorExpected[] = {{0.125, 0.5, 0.375, 1.0}, {0.25, 0.5, 0.25, 1.0}, {0.375, 0.5, 0.125, 1.0}, {0.5, 0.5, 0.0, 1.0}, {0.25, 0.375, 0.375, 1.0}, {0.375, 0.375, 0.25, 1.0}, {0.5, 0.375, 0.125, 1.0}, {0.375, 0.25, 0.375, 1.0}, {0.5, 0.25, 0.25, 1.0}, {0.5, 0.125, 0.375, 1.0}, {0.125, 0.5, 0.375, 1.0}, {0.25, 0.5, 0.25, 1.0}, {0.375, 0.5, 0.125, 1.0}, {0.25, 0.375, 0.375, 1.0}, {0.375, 0.375, 0.25, 1.0}, {0.375, 0.25, 0.375, 1.0}};

	nm32f v0xExpected[] = {  0.000000000000000f,  32.000000000000000f,  64.000000000000000f,  96.000000000000000f,  16.000000000000000f,  48.000000000000000f,  80.000000000000000f,  32.000000000000000f,  64.000000000000000f,  48.000000000000000f,  16.000000000000000f,  48.000000000000000f,  80.000000000000000f,  32.000000000000000f,  64.000000000000000f,  48.000000000000000f};
	nm32f v0yExpected[] = {  0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  32.000000000000000f,  32.000000000000000f,  32.000000000000000f,  64.000000000000000f,  64.000000000000000f,  96.000000000000000f,  32.000000000000000f,  32.000000000000000f,  32.000000000000000f,  64.000000000000000f,  64.000000000000000f,  96.000000000000000f};
	nm32f v0zExpected[] = {  0.100000001490116f,   0.200000002980232f,   0.300000011920929f,   0.400000005960464f,   0.150000005960464f,   0.250000000000000f,   0.349999994039536f,   0.200000002980232f,   0.300000011920929f,   0.250000000000000f,   0.150000005960464f,   0.250000000000000f,   0.349999994039536f,   0.200000002980232f,   0.300000011920929f,   0.250000000000000f};
	nm32f v0wExpected[] = {  0.200000002980232f,   0.300000011920929f,   0.400000035762787f,   0.500000000000000f,   0.250000000000000f,   0.350000023841858f,   0.450000017881393f,   0.300000011920929f,   0.400000035762787f,   0.350000023841858f,   0.250000000000000f,   0.350000023841858f,   0.450000017881393f,   0.300000011920929f,   0.400000035762787f,   0.350000023841858f};
	nm32f v0sExpected[] = {  0.000000000000000f,  16.000000000000000f,  32.000000000000000f,  48.000000000000000f,   8.000000000000000f,  24.000000000000000f,  40.000000000000000f,  16.000000000000000f,  32.000000000000000f,  24.000000000000000f,   8.000000000000000f,  24.000000000000000f,  40.000000000000000f,  16.000000000000000f,  32.000000000000000f,  24.000000000000000f};
	nm32f v0tExpected[] = {  0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  16.000000000000000f,  16.000000000000000f,  16.000000000000000f,  32.000000000000000f,  32.000000000000000f,  48.000000000000000f,  16.000000000000000f,  16.000000000000000f,  16.000000000000000f,  32.000000000000000f,  32.000000000000000f,  48.000000000000000f};
	v4nm32f v0ColorExpected[] = {{  0.000000000000000f,   0.500000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.125000000000000f,   0.500000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.250000000000000f,   0.500000000000000f,   0.250000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.500000000000000f,   0.125000000000000f,   1.000000000000000f}, {  0.125000000000000f,   0.375000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.250000000000000f,   0.375000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.375000000000000f,   0.250000000000000f,   1.000000000000000f}, {  0.250000000000000f,   0.250000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.250000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.125000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.125000000000000f,   0.375000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.250000000000000f,   0.375000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.375000000000000f,   0.250000000000000f,   1.000000000000000f}, {  0.250000000000000f,   0.250000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.250000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.125000000000000f,   0.500000000000000f,   1.000000000000000f}};

	nm32f v1xExpected[] = { 16.000000000000000f,  48.000000000000000f,  80.000000000000000f, 112.000000000000000f,  32.000000000000000f,  64.000000000000000f,  96.000000000000000f,  48.000000000000000f,  80.000000000000000f,  64.000000000000000f,  48.000000000000000f,  80.000000000000000f, 112.000000000000000f,  64.000000000000000f,  96.000000000000000f,  80.000000000000000f};
	nm32f v1yExpected[] = { 32.000000000000000f,  32.000000000000000f,  32.000000000000000f,  32.000000000000000f,  64.000000000000000f,  64.000000000000000f,  64.000000000000000f,  96.000000000000000f,  96.000000000000000f, 128.000000000000000f,  32.000000000000000f,  32.000000000000000f,  32.000000000000000f,  64.000000000000000f,  64.000000000000000f,  96.000000000000000f};
	nm32f v1zExpected[] = {  0.150000005960464f,   0.250000000000000f,   0.349999994039536f,   0.450000017881393f,   0.200000002980232f,   0.300000011920929f,   0.400000005960464f,   0.250000000000000f,   0.349999994039536f,   0.300000011920929f,   0.250000000000000f,   0.349999994039536f,   0.450000017881393f,   0.300000011920929f,   0.400000005960464f,   0.349999994039536f};
	nm32f v1wExpected[] = {  0.250000000000000f,   0.350000023841858f,   0.450000017881393f,   0.550000011920929f,   0.300000011920929f,   0.400000035762787f,   0.500000000000000f,   0.350000023841858f,   0.450000017881393f,   0.400000035762787f,   0.350000023841858f,   0.450000017881393f,   0.550000011920929f,   0.400000035762787f,   0.500000000000000f,   0.450000017881393f};
	nm32f v1sExpected[] = {  8.000000000000000f,  24.000000000000000f,  40.000000000000000f,  56.000000000000000f,  16.000000000000000f,  32.000000000000000f,  48.000000000000000f,  24.000000000000000f,  40.000000000000000f,  32.000000000000000f,  24.000000000000000f,  40.000000000000000f,  56.000000000000000f,  32.000000000000000f,  48.000000000000000f,  40.000000000000000f};
	nm32f v1tExpected[] = { 16.000000000000000f,  16.000000000000000f,  16.000000000000000f,  16.000000000000000f,  32.000000000000000f,  32.000000000000000f,  32.000000000000000f,  48.000000000000000f,  48.000000000000000f,  64.000000000000000f,  16.000000000000000f,  16.000000000000000f,  16.000000000000000f,  32.000000000000000f,  32.000000000000000f,  48.000000000000000f};
	v4nm32f v1ColorExpected[] = {{  0.125000000000000f,   0.375000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.250000000000000f,   0.375000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.375000000000000f,   0.250000000000000f,   1.000000000000000f}, {  0.500000000000000f,   0.375000000000000f,   0.125000000000000f,   1.000000000000000f}, {  0.250000000000000f,   0.250000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.250000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.500000000000000f,   0.250000000000000f,   0.250000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.125000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.500000000000000f,   0.125000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.500000000000000f,   0.000000000000000f,   0.500000000000000f,   1.000000000000000f}, {  0.250000000000000f,   0.375000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.375000000000000f,   0.250000000000000f,   1.000000000000000f}, {  0.500000000000000f,   0.375000000000000f,   0.125000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.250000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.500000000000000f,   0.250000000000000f,   0.250000000000000f,   1.000000000000000f}, {  0.500000000000000f,   0.125000000000000f,   0.375000000000000f,   1.000000000000000f}};

	nm32f v2xExpected[] = { 32.000000000000000f,  64.000000000000000f,  96.000000000000000f, 128.000000000000000f,  48.000000000000000f,  80.000000000000000f, 112.000000000000000f,  64.000000000000000f,  96.000000000000000f,  80.000000000000000f,  32.000000000000000f,  64.000000000000000f,  96.000000000000000f,  48.000000000000000f,  80.000000000000000f,  64.000000000000000f};
	nm32f v2yExpected[] = {  0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  32.000000000000000f,  32.000000000000000f,  32.000000000000000f,  64.000000000000000f,  64.000000000000000f,  96.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  32.000000000000000f,  32.000000000000000f,  64.000000000000000f};
	nm32f v2zExpected[] = {  0.200000002980232f,   0.300000011920929f,   0.400000005960464f,   0.500000000000000f,   0.250000000000000f,   0.349999994039536f,   0.450000017881393f,   0.300000011920929f,   0.400000005960464f,   0.349999994039536f,   0.200000002980232f,   0.300000011920929f,   0.400000005960464f,   0.250000000000000f,   0.349999994039536f,   0.300000011920929f};
	nm32f v2wExpected[] = {  0.300000011920929f,   0.400000035762787f,   0.500000000000000f,   0.600000023841858f,   0.350000023841858f,   0.450000017881393f,   0.550000011920929f,   0.400000035762787f,   0.500000000000000f,   0.450000017881393f,   0.300000011920929f,   0.400000035762787f,   0.500000000000000f,   0.350000023841858f,   0.450000017881393f,   0.400000035762787f};
	nm32f v2sExpected[] = { 16.000000000000000f,  32.000000000000000f,  48.000000000000000f,  64.000000000000000f,  24.000000000000000f,  40.000000000000000f,  56.000000000000000f,  32.000000000000000f,  48.000000000000000f,  40.000000000000000f,  16.000000000000000f,  32.000000000000000f,  48.000000000000000f,  24.000000000000000f,  40.000000000000000f,  32.000000000000000f};
	nm32f v2tExpected[] = {  0.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  16.000000000000000f,  16.000000000000000f,  16.000000000000000f,  32.000000000000000f,  32.000000000000000f,  48.000000000000000f,   0.000000000000000f,   0.000000000000000f,   0.000000000000000f,  16.000000000000000f,  16.000000000000000f,  32.000000000000000f};
	v4nm32f v2ColorExpected[] = {{  0.125000000000000f,   0.500000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.250000000000000f,   0.500000000000000f,   0.250000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.500000000000000f,   0.125000000000000f,   1.000000000000000f}, {  0.500000000000000f,   0.500000000000000f,   0.000000000000000f,   1.000000000000000f}, {  0.250000000000000f,   0.375000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.375000000000000f,   0.250000000000000f,   1.000000000000000f}, {  0.500000000000000f,   0.375000000000000f,   0.125000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.250000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.500000000000000f,   0.250000000000000f,   0.250000000000000f,   1.000000000000000f}, {  0.500000000000000f,   0.125000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.125000000000000f,   0.500000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.250000000000000f,   0.500000000000000f,   0.250000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.500000000000000f,   0.125000000000000f,   1.000000000000000f}, {  0.250000000000000f,   0.375000000000000f,   0.375000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.375000000000000f,   0.250000000000000f,   1.000000000000000f}, {  0.375000000000000f,   0.250000000000000f,   0.375000000000000f,   1.000000000000000f}};

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

clock_t splitTriangles_nInputsNoDivision_nOutputs(int n)
{
	size_t size = n;

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

clock_t splitTriangles_isoscelesTrianglesNeedsDivision(float width, int *treated, int *dstSize)
{
	size_t size = 1;

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

clock_t splitTriangles_narrowTriangleNeedsDivision(float width, int *treated, int *dstSize)
{
	size_t size = 1;

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
