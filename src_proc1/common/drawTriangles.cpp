#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include <stdio.h>


SECTION(".data_imu0") Rectangle windows[NMGL_SIZE];

SECTION(".text_demo3d") void drawTriangles(NMGL_Context_NM1* context) {
	PolygonsConnector connector(context->polygonsData);
	Polygons* poly = connector.ptrTail();
	getAddrPtrnsT(context, poly);
	nm32s* mulZ = context->buffer0;
	nm32s* mulC = context->buffer0;
	nm32s* zMaskBuffer = context->buffer1;

	msdWaitDma(0);

	merge_v4nm32s(context->offsetTrX,
		context->offsetTrY,
		context->widths, 
		context->heights, 
		(v4nm32s*)windows, 
		poly->count);

	int point = 0;

	msdWaitDma(1);
	int countTrangles = poly->count;
	(*connector.pTail)++;

	while (countTrangles > 0) {
		int localSize = MIN(countTrangles, SMALL_SIZE);
		int point_x3 = point * 3;
		int* widths = context->widths + point;
		int* heights = context->heights + point;
		int* offsetsX = context->offsetTrX + point;
		int* valuesC = context->valuesC + point;
		int* valuesZ = context->valuesZ + point;

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
				windows + point,
				context->minusOne,
				zMaskBuffer,
				localSize);
		}
		else {
			//умножение бинарных масок на Z
			mMulCVxN_2s32s(
				context->polyImgTmp,
				windows + point,
				valuesZ,
				mulZ,
				localSize);


			//mulZ теперь хранит z-треугольники

			//функция теста глубины
			depthTest(context->zBuffPoints + point, WIDTH_SEG,
				(nm32s*)mulZ,
				(nm32s*)zMaskBuffer,
				heights,
				widths, localSize);
		}

		//color v4nm8s in imgOffset
		mMulCVxN_2s_RGB8888(
			context->polyImgTmp,
			windows + point,
			(v4nm8s*)valuesC,
			mulC,
			localSize);

		//mulBuffer теперь хранит цвет


		//функция накладывает маску на буфер с цветами 
		//и копирует треугольник в изображение
		mMaskVxN_32s((nm32s*)mulC,
			(nm32s*)zMaskBuffer,
			context->imagePoints + point, WIDTH_SEG,
			heights, widths, localSize);

		countTrangles -= SMALL_SIZE;
		point += SMALL_SIZE;
	}

	return;
}
