#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include <stdio.h>
#include "imagebuffer.h"
#include "nmprofiler.h"

int totalSum(nm32s* pVec, int size);

#ifdef TEXTURE_ENABLED
using namespace tex_nm1; 
SECTION(".data_imu0") TrianglesInfo triangles;
#endif //TEXTURE_ENABLED

SECTION(".text_demo3d") void drawTriangles() {
	NMGL_Context_NM1 *context = NMGL_Context_NM1::getContext();
	PolygonsConnector *connector = context->polygonsConnectors;

	Polygons* poly = connector->ptrTail();

	PROFILER_SIZE(poly->count);
	getAddrPtrnsT(context->polygonsConnectors->ptrTail());

	COMMON_DRAW_TYPE* mulZ = (COMMON_DRAW_TYPE*)context->buffer0;
	COMMON_DRAW_TYPE* mulC = (COMMON_DRAW_TYPE*)context->buffer0;
	COMMON_DRAW_TYPE* zMaskBuffer = (COMMON_DRAW_TYPE*)context->buffer1;
	int countTrangles = poly->count;

	//msdWaitDma(0);
	int point = 0;

	msdWaitDma(1);

	poly->count = 0;
	connector->incTail();

	while (countTrangles > 0) {
		int localSize = MIN(countTrangles, SMALL_SIZE);
		int point_x3 = point * 3;
		int fullSize = totalSum(context->nSizePtrn32, localSize);

		//копирование паттернов во внутреннюю память. Паттерны копируются
		//не полностью, чтобы сэкономить время на пересылку
		copyPacket_32s(context->ppSrcPackPtrns + point_x3, 
			context->ppDstPackPtrns + point_x3, 
			context->nSizePtrn32 + point_x3, 3 * localSize);

		//объединение паттернов сторон в паттерн треугольника
		//объединение происходит не полностью (только значимой части) для
		//оптимизации
		PROFILER_SIZE(fullSize);
		mAndVxN_32u((nm32u**)context->ppPtrns1_2s, 
			(nm32u**)context->ppPtrns2_2s, 
			(nm32u**)context->ppPtrnsCombined_2s, 
			context->nSizePtrn32 + point_x3, localSize);

		//проверка активирования теста глубины
		if (context->depthBuffer.enabled == NMGL_FALSE) {
			MUL_Z_FUNC(
				context->polyImgTmp,
				context->ptrnInnPoints + point,
				context->ptrnSizes + point,
				context->minusOne,
				zMaskBuffer,
				localSize);
		}
		else {
			//умножение бинарных масок на Z
			PROFILER_SIZE(fullSize);
			MUL_Z_FUNC(
				context->polyImgTmp,
				context->ptrnInnPoints + point,
				context->ptrnSizes + point,
				context->valuesZ + point,
				mulZ,
				localSize);


			//mulZ теперь хранит z-треугольники

			//функция теста глубины
			PROFILER_SIZE(fullSize);
			DEPTH_FUNC((COMMON_DRAW_TYPE**)(context->zBuffPoints + point),
				WIDTH_SEG,
				mulZ,
				zMaskBuffer,
				context->ptrnSizes + point,
				localSize);
		}

		//color v4nm8s in imgOffset
		PROFILER_SIZE(fullSize);
		MUL_C_FUNC(
			context->polyImgTmp,
			context->ptrnInnPoints + point,
			context->ptrnSizes + point,
			context->valuesC + point,
			mulC,
			localSize);

		//mulBuffer теперь хранит цвет

#ifdef TEXTURE_ENABLED
//		if (context->texState.textureEnabled) {
//			for (int i = 0; i < localSize; i++) {
//				printf("after getAddrPtrnsT ");
//				printf("x0 %f ", context->x0[i]);
//				printf("y0 %f ", context->y0[i]);
//				printf("x1 %f ", context->x1[i]);
//				printf("y1 %f ", context->y1[i]);
//				printf("x2 %f ", context->x2[i]);
//				printf("y2 %f ", context->y2[i]);
//
//				printf("w0 %f ", context->w0[i]);
//				printf("w1 %f ", context->w1[i]);
//				printf("w2 %f ", context->w2[i]);
//
//				printf("s0 %f ", context->texS0[i]);
//				printf("t0 %f ", context->texT0[i]);
//				printf("s1 %f ", context->texS1[i]);
//				printf("t1 %f ", context->texT1[i]);
//				printf("s2 %f ", context->texS2[i]);
//				printf("t2 %f ", context->texT2[i]);
//				printf("\n");
//			}
//		}
#endif //TEXTURE_ENABLED

#ifdef TEXTURE_ENABLED
#define TRIANGLE_AMOUNT 2
        if (context->texState.textureEnabled) {
            //TODO: Check: DataForNmpu1 contains window coordinates or segment coordinates?
            float x0[TRIANGLE_AMOUNT] = {415.0f,414.0f };
            float y0[TRIANGLE_AMOUNT] = {353.0f,354.0f };
            float x1[TRIANGLE_AMOUNT] = {384.0f,414.0f };
            float y1[TRIANGLE_AMOUNT] = {353.0f,384.0f };
            float x2[TRIANGLE_AMOUNT] = {384.0f,384.0f };
            float y2[TRIANGLE_AMOUNT] = {384.0f,384.0f };

            float s0[TRIANGLE_AMOUNT] = {1.0f,1.0f };
            float t0[TRIANGLE_AMOUNT] = {1.0f,1.0f };
            float s1[TRIANGLE_AMOUNT] = {0.0f,1.0f };
            float t1[TRIANGLE_AMOUNT] = {1.0f,0.0f };
            float s2[TRIANGLE_AMOUNT] = {0.0f,0.0f };
            float t2[TRIANGLE_AMOUNT] = {0.0f,0.0f };

            float z[TRIANGLE_AMOUNT] = {1.0f, 1.0f}; //w after matrix projection

            triangles.x0 = &x0[0];
            triangles.y0 = &y0[0];
            triangles.x1 = &x1[0];
            triangles.y1 = &y1[0];
            triangles.x2 = &x2[0];
            triangles.y2 = &y2[0];

            triangles.s0 = &s0[0];
            triangles.t0 = &t0[0];
            triangles.s1 = &s1[0];
            triangles.t1 = &t1[0];
            triangles.s2 = &s2[0];
            triangles.t2 = &t2[0];

            triangles.z0 = &z[0];
            triangles.z1 = &z[0];
            triangles.z2 = &z[0];

            textureTriangle(
                    context->polyImgTmp,
                    &triangles,
                    (COMMON_DRAW_TYPE**)(context->imagePoints + point),
                    context->ptrnInnPoints + point,
                    context->ptrnSizes + point,
                    mulC,
                    mulC,
                    localSize);
        }
#endif //TEXTURE_ENABLED

		//функция накладывает маску на буфер с цветами 
		//и копирует треугольник в изображение
		PROFILER_SIZE(fullSize);
		MASK_FUNC(mulC,
			zMaskBuffer,
			(COMMON_DRAW_TYPE**)(context->imagePoints + point), WIDTH_SEG,
			context->ptrnSizes + point,
			localSize);

		countTrangles -= SMALL_SIZE;
		point += SMALL_SIZE;
	}

	return;
}
