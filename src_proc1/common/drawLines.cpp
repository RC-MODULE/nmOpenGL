#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include <stdio.h>
#include "imagebuffer.h"

SECTION(".text_demo3d") void drawLines(PolygonsConnector *connector) {
	NMGL_Context_NM1 *context = NMGL_Context_NM1::getContext();

	DataForNmpu1* poly = connector->ptrTail();

	getAddrPtrnsL(connector->ptrTail());
	COMMON_DRAW_TYPE* mulZ = (COMMON_DRAW_TYPE*)context->buffer0;
	COMMON_DRAW_TYPE* mulC = (COMMON_DRAW_TYPE*)context->buffer0;
	COMMON_DRAW_TYPE* zMaskBuffer = (COMMON_DRAW_TYPE*)context->buffer1;
	int countTrangles = poly->count;

	msdWaitDma(0);

	int point = 0;

	msdWaitDma(1);

	poly->count = 0;
	connector->incTail();
	for(int point = 0; countTrangles > 0; countTrangles -= SMALL_SIZE,	point += SMALL_SIZE){
		int localSize = MIN(countTrangles, SMALL_SIZE);

		//копирование паттернов во внутреннюю память. Паттерны копируются
		//не полностью, чтобы сэкономить время на пересылку
		copyPacket_32s(context->ppSrcPackPtrns + point,
			context->ppDstPackPtrns + point,
			context->nSizePtrn32 + point, localSize);

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

		
	}

	return;
}
