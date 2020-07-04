#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include <stdio.h>

#ifdef TEXTURE_ENABLED

#include "nmgltex_nm1.h"
#include "nmgltex_common.h"
#include "textureTriangle.h"

extern "C" TexImage2D teximage_256_256;
extern "C" TexImage2D teximage_mytexture;
extern "C" TexImage2D teximage_128_128;
extern "C" TexImage2D teximage_64_64;
extern "C" TexImage2D teximage_32_32;
extern "C" TexImage2D teximage_16_16;
extern "C" TexImage2D teximage_8_8;
extern "C" TexImage2D teximage_4_4;
extern "C" TexImage2D teximage_2_2;
extern "C" TexImage2D teximage_1_1;
#endif //TEXTURE_ENABLED


SECTION(".data_imu0") Rectangle windows[NMGL_SIZE];

SECTION(".text_demo3d") void drawTriangles(NMGL_Context_NM1* context) {
	PolygonsConnector connector(context->polygonsData);
	Polygons* poly = connector.ptrTail();
	getAddrPtrnsT(context, poly);
	nm32s* mulZ = context->buffer0;
	nm32s* mulC = context->buffer0;
	nm32s* zMaskBuffer = context->buffer1;
#ifdef TEXTURE_ENABLED
    nm32s* texTri = context->buffer0;
#endif //TEXTURE_ENABLED

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

		// printf("0\n\n");
		// printf("\n");
		// for (int i = 0; i < 32; i++)
		// {
			// printf("0x%016llx, \n", ((long long int*)context->ppPtrnsCombined_2s[0])[i]);
		// }
		// printf("\n");
		// for (int i = 0; i < 32; i++)
		// {
			// printf("0x%016llx, \n", ((long long int*)context->ppPtrns1_2s[0])[i]);
		// }
		// printf("\n");
		// for (int i = 0; i < 32; i++)
		// {
			// printf("0x%016llx, \n", ((long long int*)context->ppPtrns2_2s[0])[i]);
		// }

		// printf("1\n\n");
		// printf("\n");
		// for (int i = 0; i < 32; i++)
		// {
			// printf("0x%016llx, \n", ((long long int*)context->ppPtrnsCombined_2s[1])[i]);
		// }
		// printf("\n");
		// for (int i = 0; i < 32; i++)
		// {
			// printf("0x%016llx, \n", ((long long int*)context->ppPtrns1_2s[1])[i]);
		// }
		// printf("\n");
		// for (int i = 0; i < 32; i++)
		// {
			// printf("0x%016llx, \n", ((long long int*)context->ppPtrns2_2s[1])[i]);
		// }
		
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

#ifdef TEXTURE_ENABLED //for test
        //Массив данных о треугольниках (координаты вершин, текстурные координаты и т.д.)
        Triangles triangles;
        
        context->texState.activeTexUnitIndex = 0;
        unsigned int activeTexUnitIndex = context->texState.activeTexUnitIndex;
    
        context->texState.texUnits[activeTexUnitIndex].boundTexObject = &context->texState.texObjects[0];
        TexObject* boundTexObject = context->texState.texUnits[activeTexUnitIndex].boundTexObject;
        
        // boundTexObject->texImages2D[0] = teximage_256_256;
        boundTexObject->texImages2D[0] = teximage_mytexture;
        boundTexObject->texImages2D[1] = teximage_128_128;
        boundTexObject->texImages2D[2] = teximage_64_64;
        boundTexObject->texImages2D[3] = teximage_32_32;
        boundTexObject->texImages2D[4] = teximage_16_16;
        boundTexObject->texImages2D[5] = teximage_8_8;
        boundTexObject->texImages2D[6] = teximage_4_4;
        boundTexObject->texImages2D[7] = teximage_2_2;
        boundTexObject->texImages2D[8] = teximage_1_1;

        boundTexObject->texMinFilter = NMGL_NEAREST; //default NEAREST_MIPMAP_LINEAR
        boundTexObject->texMagFilter = NMGL_NEAREST; //default LINEAR
        boundTexObject->texWrapS = NMGL_REPEAT; // default REPEAT
        boundTexObject->texWrapT = NMGL_REPEAT;// default REPEAT

        context->texState.texUnits[activeTexUnitIndex].texFunctionName = NMGL_REPLACE; //default = NMGL_MODULATE
        
        //default texEnvColor = (0.0f, 0.0f, 0.0f, 0.0f)
        context->texState.texUnits[activeTexUnitIndex].texEnvColor[0] = 0.0f;
        context->texState.texUnits[activeTexUnitIndex].texEnvColor[1] = 0.0f;
        context->texState.texUnits[activeTexUnitIndex].texEnvColor[2] = 0.0f;
        context->texState.texUnits[activeTexUnitIndex].texEnvColor[3] = 0.0f;

        context->texState.unpackAlignment = 4;
        // printPattern(patterns, pSrcTriangle, TRIANGLE_AMOUNT);
#endif //TEXTURE_ENABLED

#ifdef TEXTURE_ENABLED
		if (context->texState.textureEnabled){
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
        	
        	triangles.z = context->zEye + point;

        	textureTriangle(context->polyImgTmp, &triangles, 
        	                context->imagePoints + point, 
        	                windows + point, 
        	                mulC, 
        	                mulC, 
        	                localSize);
		}
#endif //TEXTURE_ENABLED

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
