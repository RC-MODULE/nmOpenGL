#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include <stdio.h>
#include "imagebuffer.h"

SECTION(".text_demo3d") void drawTriangles() {
	NMGL_Context_NM1 *context = NMGL_Context_NM1::getContext();

	Polygons* poly = context->polygonsConnectors[0].ptrTail();
	
	//getAddrPtrnsT(context->polygonsConnectors->ptrTail());
	readPolygonsT(context->polygonsConnectors->ptrTail());
	COMMON_DRAW_TYPE* mulZ = (COMMON_DRAW_TYPE*)context->buffer0;
	COMMON_DRAW_TYPE* mulC = (COMMON_DRAW_TYPE*)context->buffer0;
	COMMON_DRAW_TYPE* zMaskBuffer = (COMMON_DRAW_TYPE*)context->buffer1;
	int countTrangles = poly->count;

	msdWaitDma(0);

	int point = 0;

	msdWaitDma(1);
	
	context->polygonsConnectors[0].incTail();

	while (countTrangles > 0) {
		int localSize = MIN(countTrangles, SMALL_SIZE);
		int point_x3 = point * 3;

		//копирование паттернов во внутреннюю память. Паттерны копируются
		//не полностью, чтобы сэкономить время на пересылку
		copyPacket_32s(context->ppSrcPackPtrns + point_x3, 
			context->ppDstPackPtrns + point_x3, 
			context->nSizePtrn32 + point_x3, 3 * localSize);

		//объединение паттернов сторон в паттерн треугольника
		//объединение происходит не полностью (только значимой части) для
		//оптимизации
		mAndVxN_32u((nm32u**)context->ppPtrns2_2s, 
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
			MUL_Z_FUNC(
				context->polyImgTmp,
				context->ptrnInnPoints + point,
				context->ptrnSizes + point,
				context->valuesZ + point,
				mulZ,
				localSize);


			//mulZ теперь хранит z-треугольники

			//функция теста глубины
			DEPTH_FUNC((COMMON_DRAW_TYPE**)(context->zBuffPoints + point),
				WIDTH_SEG,
				mulZ,
				zMaskBuffer,
				context->ptrnSizes + point,
				localSize);
		}

		//color v4nm8s in imgOffset
		MUL_C_FUNC(
			context->polyImgTmp,
			context->ptrnInnPoints + point,
			context->ptrnSizes + point,
			context->valuesC + point,
			mulC,
			localSize);

		//mulBuffer теперь хранит цвет

		//функция накладывает маску на буфер с цветами 
		//и копирует треугольник в изображение
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
