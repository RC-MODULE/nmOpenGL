#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include <stdio.h>


SECTION(".data_imu0") Vector2 ptrnInnPoints[NMGL_SIZE];
SECTION(".data_imu0") Size ptrnSizes[NMGL_SIZE];

SECTION(".text_demo3d") void drawTriangles(NMGL_Context_NM1* context) {
	PolygonsConnector connector(context->polygonsData);
	Polygons* poly = connector.ptrTail();
	getAddrPtrnsT(context, poly);
	nm32s* mulZ = (nm32s*)context->buffer0;
	nm32s* mulC = (nm32s*)context->buffer0;
	nm32s* zMaskBuffer = (nm32s*)context->buffer1;
	int countTrangles = poly->count;

	msdWaitDma(0);
	nmppsMerge_32s(context->offsetTrX, context->offsetTrY, (nm32s*)ptrnInnPoints, countTrangles);
	nmppsMerge_32s(context->widths, context->heights, (nm32s*)ptrnSizes, countTrangles);

	int point = 0;

	msdWaitDma(1);
	
	(*connector.pTail)++;

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
		mAndVxN_32u((nm32u**)context->ppPtrns1_2s, 
			(nm32u**)context->ppPtrns2_2s, 
			(nm32u**)context->ppPtrnsCombined_2s, 
			context->nSizePtrn32 + point_x3, localSize);

		//проверка активирования теста глубины
		if (context->depthBuffer.enabled == NMGL_FALSE) {
			mMulCVxN_2s32s(
				context->polyImgTmp,
				ptrnInnPoints + point,
				ptrnSizes + point,
				context->minusOne,
				zMaskBuffer,
				localSize);
		}
		else {
			//умножение бинарных масок на Z
			mMulCVxN_2s32s(
				context->polyImgTmp,
				ptrnInnPoints + point,
				ptrnSizes + point,
				context->valuesZ + point,
				mulZ,
				localSize);


			//mulZ теперь хранит z-треугольники

			//функция теста глубины
			depthTest_32s((nm32s**)(context->zBuffPoints + point), 
				WIDTH_SEG,
				(nm32s*)mulZ,
				(nm32s*)zMaskBuffer,
				ptrnSizes + point, 
				localSize);
		}

		//color v4nm8s in imgOffset
		mMulCVxN_2s_RGB8888(
			context->polyImgTmp,
			ptrnInnPoints + point,
			ptrnSizes + point,
			(v4nm8s*)(context->valuesC + point),
			mulC,
			localSize);

		//mulBuffer теперь хранит цвет

		//функция накладывает маску на буфер с цветами 
		//и копирует треугольник в изображение
		mMaskVxN_32s(mulC,
			zMaskBuffer,
			(nm32s**)(context->imagePoints + point), WIDTH_SEG,
			ptrnSizes + point,
			localSize);

		countTrangles -= SMALL_SIZE;
		point += SMALL_SIZE;
	}

	return;
}
