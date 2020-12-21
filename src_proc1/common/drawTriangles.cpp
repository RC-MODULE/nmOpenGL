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
        if (context->texState.textureEnabled) {
			triangles.x0 = context->x0 + point;
			triangles.y0 = context->y0 + point;
			triangles.x1 = context->x1 + point;
			triangles.y1 = context->y1 + point;
			triangles.x2 = context->x2 + point;
			triangles.y2 = context->y2 + point;

			triangles.s0 = context->texS0 + point;
			triangles.t0 = context->texT0 + point;
			triangles.s1 = context->texS1 + point;
			triangles.t1 = context->texT1 + point;
			triangles.s2 = context->texS2 + point;
			triangles.t2 = context->texT2 + point;

			triangles.z0 = context->w0 + point;
			triangles.z1 = context->w1 + point;
			triangles.z2 = context->w2 + point; 

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
