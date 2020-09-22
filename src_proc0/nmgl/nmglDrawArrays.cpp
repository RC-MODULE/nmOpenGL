#include "demo3d_nm0.h"
#include "nmpp.h"
#include "nmblas.h"
#include "nmgl.h"
#include "halcrc.h"

#include "malloc.h"
#include "arraymanager.h"
#include "stdio.h"
#include "imagebuffer.h"
#include "service.h"

#include "nmprofiler.h"

SECTION(".data_imu5")	float vertexX[3 * NMGL_SIZE];
SECTION(".data_imu6")	float vertexY[3 * NMGL_SIZE];
SECTION(".data_imu4")	float vertexZ[3 * NMGL_SIZE];

SECTION(".data_imu5")	v4nm32f vertexResult[3 * NMGL_SIZE];
SECTION(".data_imu6")	v4nm32f colorOrNormal[3 * NMGL_SIZE];

SECTION(".data_imu6")	ArrayManager<float> vertexAM;
SECTION(".data_imu6")	ArrayManager<float> normalAM;
SECTION(".data_imu6")	ArrayManager<v4nm32f> colorAM;

SECTION(".data_imu6")	BitDividedMask clipMask[10];


template < typename T >
inline void copyVec(const void* src, void* dst, size_t size) {
	nmblas_scopy(size * sizeof32(T), (float*)src, 1, (float*)dst, 1);
}

template < typename T >
inline void copyRisc(const void* src, void* dst, size_t size) {
	int* srcInt = (int*)src;
	int* dstInt = (int*)dst;
	for(int i = 0; i < size * sizeof32(T); i++){
		dstInt[i] = srcInt[i];
	}
}

/* функция nmglDrawArrays является функцией со статическим конвеером.
Значения переданные из массивов, которые задаются с помощью функций 
nmglVertexPointer, nmglNormalPointer, nmglColorPointer
проходят следующие этапы.

							Видовая
							матрица
Координаты вершин, нормалей ---------> Видовые координаты
*/

/*void getVertexPart(Array &vertex, int &point, float* dst, float* tmp, int size) {
	float* src = (float*)vertex.pointer + point * vertex.size;
	switch (vertex.size)
	{
	case 2:
		nmblas_dcopy(size, (double*)src, 1, (double*)dst, 2);
		tmp[0] = 0;
		tmp[1] = 1;
		nmblas_dcopy(size, (double*)tmp, 0, (double*)dst + 1, 2);
		break;
	case 3:
		cnv32f_v3v4(src, dst, 1, size);
		break;
	case 4:
		nmblas_scopy(4 * size, src, 1, dst, 1);
		break;
	default:
		break;
	}
}

void getNormalPart(Array &normal, int &point, float* dst, float* tmp, int size) {
	float* src = (float*)normal.pointer + point * normal.size;
	if (normal.size == 3) {
		normalAM.pop(tmp);
		cnv32f_v3v4(tmp, dst, 0, size);
	}
	else {
		normalAM.pop(dst);
	}
	mul_v4nm32f_mat4nm32f((v4nm32f*)cntxt->buffer1, &cntxt->normalMatrix, colorOrNormal, localSize);
	if (cntxt->normalizeEnabled) {
		nmblas_scopy(4 * localSize, (float*)colorOrNormal, 1, cntxt->buffer2, 1);
		dotV_v4nm32f(colorOrNormal, (v4nm32f*)cntxt->buffer2, (v2nm32f*)cntxt->buffer0, localSize);
		fastInvSqrt(cntxt->buffer0, cntxt->buffer1, 2 * localSize);
		dotMulV_v4nm32f((v2nm32f*)cntxt->buffer1, (v4nm32f*)cntxt->buffer2, colorOrNormal, localSize);
	}
}*/



SECTION(".text_nmgl")
void nmglDrawArrays(NMGLenum mode, NMGLint first, NMGLsizei count) {
#ifdef DEBUG
	printf("DrawArrays start ");
	switch (mode)
	{
	case NMGL_TRIANGLES:
		printf("triangles drawing\n");
		break;
	case NMGL_TRIANGLE_FAN:
		printf("triangle fan drawing\n");
		break;
	case NMGL_TRIANGLE_STRIP:
		printf("triangle strip drawing\n");
		break;
	case NMGL_POINTS:
		printf("points drawing\n");
		break;
	case NMGL_LINES:
		printf("lines drawing\n");
		break;
	case NMGL_LINE_LOOP:
		printf("line loop drawing\n");
		break;
	case NMGL_LINE_STRIP:
		printf("line strip drawing\n");
		break;
	default:
		printf(". Invalid error. Exit DrawArrays\n");
		return;
	}
#endif // DEBUG
	
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (cntxt->vertexArray.enabled == NMGL_FALSE) {
		return;
	}
	if (count < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	float* srcDDR_vertex = (float*)cntxt->vertexArray.pointer + cntxt->vertexArray.size * first;
	float* srcDDR_normal = (float*)cntxt->normalArray.pointer + cntxt->normalArray.size * first;
	v4nm32f* srcDDR_color = (v4nm32f*)cntxt->colorArray.pointer + first;

	cntxt->vertexArray.offset = first;
	cntxt->normalArray.offset = first;
	cntxt->colorArray.offset = first;

	int maxInnerCount;
	switch (mode) {
	case NMGL_TRIANGLES:
		maxInnerCount = 3 * NMGL_SIZE;
		break;
	case NMGL_TRIANGLE_FAN:
		maxInnerCount = NMGL_SIZE + 2;
		break;
	case NMGL_TRIANGLE_STRIP:
		maxInnerCount = NMGL_SIZE + 2;
		break;
	case NMGL_LINES:
		maxInnerCount = 2 * NMGL_SIZE;
		break;
	case NMGL_POINTS:
		maxInnerCount = 3 * NMGL_SIZE;
		break;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	if(first % 2 == 0 && count % 2 == 0){
		vertexAM.set(srcDDR_vertex, cntxt->vertexArray.size * count, cntxt->vertexArray.size * maxInnerCount, copyVec<float>);
		normalAM.set(srcDDR_normal, cntxt->normalArray.size * count, cntxt->normalArray.size * maxInnerCount, copyVec<float>);
		colorAM.set(srcDDR_color, count, maxInnerCount, copyVec<v4nm32f>);
	}else{
		vertexAM.set(srcDDR_vertex, cntxt->vertexArray.size * count, cntxt->vertexArray.size * maxInnerCount, copyRisc<float>);
		normalAM.set(srcDDR_normal, cntxt->normalArray.size * count, cntxt->normalArray.size * maxInnerCount, copyRisc<float>);
		colorAM.set(srcDDR_color, count, maxInnerCount, copyRisc<v4nm32f>);
	}

	if (cntxt->isLighting) {
		mulC_v4nm32f(cntxt->lightAmbient, &cntxt->materialAmbient, cntxt->ambientMul, MAX_LIGHTS + 1);
		mulC_v4nm32f(cntxt->lightDiffuse, &cntxt->materialDiffuse, cntxt->diffuseMul, MAX_LIGHTS);
		mulC_v4nm32f(cntxt->lightSpecular, &cntxt->materialSpecular, cntxt->specularMul, MAX_LIGHTS);
		nmppsAdd_32f((float*)(cntxt->ambientMul + MAX_LIGHTS), 
			(float*)&cntxt->materialEmissive, 
			(float*)(cntxt->ambientMul + MAX_LIGHTS), 4);
	}
	//reverseMatrix3x3in4x4(cntxt->modelviewMatrixStack.top(), &cntxt->normalMatrix);
	while (!vertexAM.isEmpty()) {
		//int localSize = MIN(count - pointVertex, maxInnerCount);
		int localSize = vertexAM.pop(cntxt->buffer0) / cntxt->vertexArray.size;
		int excessVertexCount = 0;
		/*if (localSize % 2) {
			localSize++;
			excessVertexCount++;
		}*/
		switch (cntxt->vertexArray.size)
		{
		case 2:
			nmblas_dcopy(localSize, (double*)cntxt->buffer0, 1, (double*)cntxt->buffer1, 2);
			cntxt->buffer0[0] = 0;
			cntxt->buffer0[1] = 1;
			nmblas_dcopy(localSize, (double*)cntxt->buffer0, 0, (double*)(cntxt->buffer1 + 2), 2);
			break;
		case 3:
			cnv32f_v3v4(cntxt->buffer0, cntxt->buffer1, 1, localSize);
			break;
		case 4:
			nmblas_scopy(4 * localSize, cntxt->buffer0, 1, cntxt->buffer1, 1);
			break;
		default:
			break;
		}
		//умножение на dидовую матрицу (Modelview matrix)
		mul_mat4nm32f_v4nm32f(cntxt->modelviewMatrixStack.top(), (v4nm32f*)cntxt->buffer1, vertexResult, localSize);
		//normal
		if (cntxt->normalArray.enabled) {
			if (cntxt->normalArray.size == 3) {
				normalAM.pop(cntxt->buffer3);
				cnv32f_v3v4(cntxt->buffer3, cntxt->buffer1, 0, localSize);
			}
			else {
				normalAM.pop(cntxt->buffer1);
			}
			mul_v4nm32f_mat4nm32f((v4nm32f*)cntxt->buffer1, &cntxt->normalMatrix, colorOrNormal, localSize);
			if (cntxt->normalizeEnabled) {
				nmblas_scopy(4 * localSize, (float*)colorOrNormal, 1, cntxt->buffer2, 1);
				dotV_v4nm32f(colorOrNormal, (v4nm32f*)cntxt->buffer2, (v2nm32f*)cntxt->buffer0, localSize);
				fastInvSqrt(cntxt->buffer0, cntxt->buffer1, 2 * localSize);
				dotMulV_v4nm32f((v2nm32f*)cntxt->buffer1, (v4nm32f*)cntxt->buffer2, colorOrNormal, localSize);
			}
		}
		//vertex in vertexResult
		//normal in colorOrNormal
		//Освещение или наложение цветов
		if (cntxt->isLighting) {
			light(vertexResult, colorOrNormal, localSize);
		}
		else {
			cntxt->tmp.vec[0] = 1;
			cntxt->tmp.vec[1] = 1;
			cntxt->tmp.vec[2] = 1;
			cntxt->tmp.vec[3] = 1;
			set_v4nm32f(colorOrNormal, &cntxt->tmp, localSize);
		}
		//color
		if (cntxt->colorArray.enabled) {
			colorAM.pop(colorOrNormal);
			if (cntxt->colorArray.type == NMGL_UNSIGNED_BYTE) {
				nmppsConvert_32s32f((int*)colorOrNormal, cntxt->buffer0, cntxt->colorArray.size * localSize);
				nmppsMulC_32f(cntxt->buffer0, (float*)colorOrNormal, 1.0 / 255.0, cntxt->colorArray.size * localSize);
			}
		}
		clamp_32f((float*)colorOrNormal, 0, 1, (float*)cntxt->buffer3, 4 * localSize);
		cntxt->tmp.vec[0] = RED_COEFF;
		cntxt->tmp.vec[1] = GREEN_COEFF;
		cntxt->tmp.vec[2] = BLUE_COEFF;
		cntxt->tmp.vec[3] = ALPHA_COEFF;

		mulC_v4nm32f((v4nm32f*)cntxt->buffer3, &cntxt->tmp, colorOrNormal, localSize);

		//умножение на матрицу проекции (Projection matrix)
		mul_mat4nm32f_v4nm32f(cntxt->projectionMatrixStack.top(), vertexResult, (v4nm32f*)vertexResult, localSize);

#ifndef TRIANGULATION_ENABLED
		//----------------------------------
		//vertex in vertexResult
		//color in colorOrNormal
		//------------------------------srcX-----srcY-----srcZ-----srcW--------------
		split_v4nm32f(vertexResult, 1, cntxt->buffer0, cntxt->buffer1, cntxt->buffer2, cntxt->buffer3, localSize);

		//------------clipping-------------------

		//------------perspective-division-----------------
		nmppsDiv_32f(cntxt->buffer0, cntxt->buffer3, cntxt->buffer1 + 3 * NMGL_SIZE, localSize);
		nmppsDiv_32f(cntxt->buffer1, cntxt->buffer3, cntxt->buffer2 + 3 * NMGL_SIZE, localSize);
		nmppsDiv_32f(cntxt->buffer2, cntxt->buffer3, cntxt->buffer0 + 3 * NMGL_SIZE, localSize);

		//------------viewport------------------------
		nmppsMulC_AddC_32f(cntxt->buffer1 + 3 * NMGL_SIZE, cntxt->windowInfo.viewportMulX, cntxt->windowInfo.viewportAddX, vertexX, localSize);		//X
		nmppsMulC_AddC_32f(cntxt->buffer2 + 3 * NMGL_SIZE, cntxt->windowInfo.viewportMulY, cntxt->windowInfo.viewportAddY, vertexY, localSize);		//Y
		nmppsMulC_AddC_32f(cntxt->buffer0 + 3 * NMGL_SIZE, cntxt->windowInfo.viewportMulZ, cntxt->windowInfo.viewportAddZ, vertexZ, localSize);	//Z

		nmppsConvert_32f32s_floor(vertexX, (int*)cntxt->buffer0, 0, localSize);
		nmppsConvert_32s32f((int*)cntxt->buffer0, vertexX, localSize);
		nmppsConvert_32f32s_floor(vertexY, (int*)cntxt->buffer0, 0, localSize);
		nmppsConvert_32s32f((int*)cntxt->buffer0, vertexY, localSize);

		//---------------rasterize------------------------------------
		v2nm32f *minXY = (v2nm32f*)cntxt->buffer4;
		v2nm32f *maxXY = (v2nm32f*)cntxt->buffer4 + 3 * NMGL_SIZE;
		switch (mode) {
		case NMGL_TRIANGLES: {
			pushToTriangles_t(vertexX, vertexY, vertexZ, colorOrNormal, cntxt->trianInner, localSize);

			if (cntxt->isCullFace) {
				cullFaceSortTriangles(cntxt->trianInner);
				if (cntxt->trianInner.size == 0) {
					break;
				}
			}

			findMinMax3(cntxt->trianInner.x0, cntxt->trianInner.x1, cntxt->trianInner.x2,
				cntxt->buffer0, cntxt->buffer1, cntxt->trianInner.size);
			findMinMax3(cntxt->trianInner.y0, cntxt->trianInner.y1, cntxt->trianInner.y2,
				cntxt->buffer2, cntxt->buffer3, cntxt->trianInner.size);
			nmppsMerge_32f(cntxt->buffer0, cntxt->buffer2, (float*)minXY, cntxt->trianInner.size);
			nmppsMerge_32f(cntxt->buffer1, cntxt->buffer3, (float*)maxXY, cntxt->trianInner.size);
			setSegmentMask(minXY, maxXY, cntxt->segmentMasks, cntxt->trianInner.size);
			rasterizeT(&cntxt->trianInner, cntxt->segmentMasks);
			break;
		}
		case NMGL_LINES:

			pushToLines_l(vertexX, vertexY, vertexZ, colorOrNormal, cntxt->lineInner, localSize);
			findMinMax2(cntxt->lineInner.x0, cntxt->lineInner.x1,
				cntxt->buffer0, cntxt->buffer1,
				cntxt->lineInner.size);
			findMinMax2(cntxt->lineInner.y0, cntxt->lineInner.y1,
				cntxt->buffer2, cntxt->buffer3,
				cntxt->lineInner.size);
			nmppsMerge_32f(cntxt->buffer0, cntxt->buffer2, (float*)minXY, cntxt->lineInner.size);
			nmppsMerge_32f(cntxt->buffer1, cntxt->buffer3, (float*)maxXY, cntxt->lineInner.size);
			setSegmentMask(minXY, maxXY, cntxt->segmentMasks, cntxt->lineInner.size);
			rasterizeL(&cntxt->lineInner, cntxt->segmentMasks);
			break;
		case NMGL_POINTS:
			nmblas_scopy(localSize, vertexX, 1, cntxt->pointInner.x0, 1);
			nmblas_scopy(localSize, vertexY, 1, cntxt->pointInner.y0, 1);
			nmppsConvert_32f32s_rounding((float*)colorOrNormal, (int*)cntxt->pointInner.colors, 0, 4 * localSize);
			nmppsConvert_32f32s_rounding(vertexZ, cntxt->pointInner.z, 0, localSize);
			nmppsSubC_32f(vertexX, cntxt->buffer0, cntxt->pointRadius, localSize);
			nmppsSubC_32f(vertexY, cntxt->buffer1, cntxt->pointRadius, localSize);
			nmppsMerge_32f(cntxt->buffer0, cntxt->buffer1, (float*)minXY, localSize);
			nmppsAddC_32f(vertexX, cntxt->buffer0, cntxt->pointRadius, localSize);
			nmppsAddC_32f(vertexY, cntxt->buffer1, cntxt->pointRadius, localSize);
			nmppsMerge_32f(cntxt->buffer0, cntxt->buffer1, (float*)maxXY, localSize);
			cntxt->pointInner.size = localSize;
			setSegmentMask(minXY, maxXY, cntxt->segmentMasks, localSize);
			rasterizeP(&cntxt->pointInner, cntxt->segmentMasks);
			break;
		}
#else
		switch (mode)
		{
		case NMGL_TRIANGLES:
		case NMGL_TRIANGLE_FAN:
		case NMGL_TRIANGLE_STRIP:
		{
			TrianglePointers tmp0;
			TrianglePointers tmp1;
			TrianglePointers tmp2;
			//TrianglePrimitiveArrays4f trian4f;
			//TrianglePrimitiveArrays3f trian3f_1;
			//TrianglePrimitiveArrays3f trian3f_2;
			localSize -= excessVertexCount;
			PROFILER_SIZE(localSize);
			int primCount = vertexPrimitiveRepack(vertexResult, colorOrNormal, cntxt->buffer0, (v4nm32f*)cntxt->buffer1, mode, localSize);

			//в tmp0 хранятся данные в порядке x0,y0,z0,w0,x1,y1,z1,w1,x2,y2,z2,w2 (остальные массивы неопределены)
			//в tmp1 хранятся данные в порядке x0,y0,z0,x1,y1,z1,x2,y2,z2 (остальные массивы неопределены)
			tmp0.v0.x = cntxt->buffer0 + 0 * primCount;
			tmp0.v0.y = cntxt->buffer0 + 1 * primCount;
			tmp0.v0.z = cntxt->buffer0 + 2 * primCount;
			tmp0.v0.w = cntxt->buffer0 + 3 * primCount;
			tmp0.v1.x = cntxt->buffer0 + 4 * primCount;
			tmp0.v1.y = cntxt->buffer0 + 5 * primCount;
			tmp0.v1.z = cntxt->buffer0 + 6 * primCount;
			tmp0.v1.w = cntxt->buffer0 + 7 * primCount;
			tmp0.v2.x = cntxt->buffer0 + 8 * primCount;
			tmp0.v2.y = cntxt->buffer0 + 9 * primCount;
			tmp0.v2.z = cntxt->buffer0 + 10 * primCount;
			tmp0.v2.w = cntxt->buffer0 + 11 * primCount;

			tmp1.v0.x = cntxt->buffer2 + 0 * primCount;
			tmp1.v0.y = cntxt->buffer2 + 1 * primCount;
			tmp1.v0.z = cntxt->buffer2 + 2 * primCount;
			tmp1.v1.x = cntxt->buffer2 + 3 * primCount;
			tmp1.v1.y = cntxt->buffer2 + 4 * primCount;
			tmp1.v1.z = cntxt->buffer2 + 5 * primCount;
			tmp1.v2.x = cntxt->buffer2 + 6 * primCount;
			tmp1.v2.y = cntxt->buffer2 + 7 * primCount;
			tmp1.v2.z = cntxt->buffer2 + 8 * primCount;

			//------------clipping-------------------
			/*for (int i = 0; i < 10; i++) {
				clipMask[i].even.bits = (int*)cntxt->buffer2 + i * NMGL_SIZE / 32;
				clipMask[i].odd.bits = (int*)cntxt->buffer2 + (10 + i) * NMGL_SIZE / 32;
			}
			nmppsCmpGteLteMirrorV_32f(tmp0.v0.x, tmp0.v0.w, clipMask[0].even.bits, clipMask[0].odd.bits, primCount);
			nmppsCmpGteLteMirrorV_32f(tmp0.v0.y, tmp0.v0.w, clipMask[1].even.bits, clipMask[1].odd.bits, primCount);
			nmppsCmpGteLteMirrorV_32f(tmp0.v0.z, tmp0.v0.w, clipMask[2].even.bits, clipMask[2].odd.bits, primCount);

			nmppsCmpGteLteMirrorV_32f(tmp0.v1.x, tmp0.v1.w, clipMask[3].even.bits, clipMask[3].odd.bits, primCount);
			nmppsCmpGteLteMirrorV_32f(tmp0.v1.y, tmp0.v1.w, clipMask[4].even.bits, clipMask[4].odd.bits, primCount);
			nmppsCmpGteLteMirrorV_32f(tmp0.v1.z, tmp0.v1.w, clipMask[5].even.bits, clipMask[5].odd.bits, primCount);

			nmppsCmpGteLteMirrorV_32f(tmp0.v2.x, tmp0.v2.w, clipMask[6].even.bits, clipMask[6].odd.bits, primCount);
			nmppsCmpGteLteMirrorV_32f(tmp0.v2.y, tmp0.v2.w, clipMask[7].even.bits, clipMask[7].odd.bits, primCount);
			nmppsCmpGteLteMirrorV_32f(tmp0.v2.z, tmp0.v2.w, clipMask[8].even.bits, clipMask[8].odd.bits, primCount);
			int nMasks = (primCount + 31) / 32;
			for (int i = 0; i < nMasks; i++) {
				clipMask[9].even.bits[i] = 0;
				clipMask[9].odd.bits[i] = 0;
				for (int = j = 0; j < 9; j++) {
					clipMask[9].even.bits[i] |= clipMask[j].even.bits[i];
					clipMask[9].odd.bits[i] |= clipMask[j].odd.bits[i];
				}
			}
			int* indicesEven = cntxt->buffer4;
			int* indicesOdd = cntxt->buffer4 + NMGL_SIZE / 2;
			int* indices = cntxt->buffer4 + NMGL_SIZE;*/

			//------------perspective-division-----------------
			//tmp0->tmp1
			nmppsDiv_32f(tmp0.v0.x, tmp0.v0.w, tmp1.v0.x, primCount);
			nmppsDiv_32f(tmp0.v0.y, tmp0.v0.w, tmp1.v0.y, primCount);
			nmppsDiv_32f(tmp0.v0.z, tmp0.v0.w, tmp1.v0.z, primCount);
			nmppsDiv_32f(tmp0.v1.x, tmp0.v1.w, tmp1.v1.x, primCount);
			nmppsDiv_32f(tmp0.v1.y, tmp0.v1.w, tmp1.v1.y, primCount);
			nmppsDiv_32f(tmp0.v1.z, tmp0.v1.w, tmp1.v1.z, primCount);
			nmppsDiv_32f(tmp0.v2.x, tmp0.v2.w, tmp1.v2.x, primCount);
			nmppsDiv_32f(tmp0.v2.y, tmp0.v2.w, tmp1.v2.y, primCount);
			nmppsDiv_32f(tmp0.v2.z, tmp0.v2.w, tmp1.v2.z, primCount);
			//------------viewport------------------------		

			//tmp1->tmp0
			nmppsMulC_AddC_32f(tmp1.v0.x, cntxt->windowInfo.viewportMulX, cntxt->windowInfo.viewportAddX, tmp0.v0.x, primCount);		//X
			nmppsMulC_AddC_32f(tmp1.v0.y, cntxt->windowInfo.viewportMulY, cntxt->windowInfo.viewportAddY, tmp0.v0.y, primCount);		//Y
			nmppsMulC_AddC_32f(tmp1.v0.z, cntxt->windowInfo.viewportMulZ, cntxt->windowInfo.viewportAddZ, tmp0.v0.z, primCount);		//Z		

			nmppsMulC_AddC_32f(tmp1.v1.x, cntxt->windowInfo.viewportMulX, cntxt->windowInfo.viewportAddX, tmp0.v1.x, primCount);		//X
			nmppsMulC_AddC_32f(tmp1.v1.y, cntxt->windowInfo.viewportMulY, cntxt->windowInfo.viewportAddY, tmp0.v1.y, primCount);		//Y
			nmppsMulC_AddC_32f(tmp1.v1.z, cntxt->windowInfo.viewportMulZ, cntxt->windowInfo.viewportAddZ, tmp0.v1.z, primCount);		//Z		

			nmppsMulC_AddC_32f(tmp1.v2.x, cntxt->windowInfo.viewportMulX, cntxt->windowInfo.viewportAddX, tmp0.v2.x, primCount);		//X
			nmppsMulC_AddC_32f(tmp1.v2.y, cntxt->windowInfo.viewportMulY, cntxt->windowInfo.viewportAddY, tmp0.v2.y, primCount);		//Y
			nmppsMulC_AddC_32f(tmp1.v2.z, cntxt->windowInfo.viewportMulZ, cntxt->windowInfo.viewportAddZ, tmp0.v2.z, primCount);		//Z			

			//в tmp0 теперь хранятся x,y в оконных координатах, z в диапазона 0..Z_BUFF_MAX, 
			//координаты w а так же текстурные координаты s,t, оставшиеся без изменений

			//округление x,y и z
			//tmp0->tmp1
			nmppsConvert_32f32s_rounding(tmp0.v0.x, (int*)tmp1.v0.x, 0, primCount);
			nmppsConvert_32f32s_rounding(tmp0.v0.y, (int*)tmp1.v0.y, 0, primCount);
			nmppsConvert_32f32s_rounding(tmp0.v0.z, (int*)tmp1.v0.z, 0, primCount);
			nmppsConvert_32f32s_rounding(tmp0.v1.x, (int*)tmp1.v1.x, 0, primCount);
			nmppsConvert_32f32s_rounding(tmp0.v1.y, (int*)tmp1.v1.y, 0, primCount);
			nmppsConvert_32f32s_rounding(tmp0.v1.z, (int*)tmp1.v1.z, 0, primCount);
			nmppsConvert_32f32s_rounding(tmp0.v2.x, (int*)tmp1.v2.x, 0, primCount);
			nmppsConvert_32f32s_rounding(tmp0.v2.y, (int*)tmp1.v2.y, 0, primCount);
			nmppsConvert_32f32s_rounding(tmp0.v2.z, (int*)tmp1.v2.z, 0, primCount);
			nmppsConvert_32s32f((int*)tmp1.v0.x, tmp1.v0.x, primCount);
			nmppsConvert_32s32f((int*)tmp1.v0.y, tmp1.v0.y, primCount);
			nmppsConvert_32s32f((int*)tmp1.v0.z, tmp1.v0.z, primCount);
			nmppsConvert_32s32f((int*)tmp1.v1.x, tmp1.v1.x, primCount);
			nmppsConvert_32s32f((int*)tmp1.v1.y, tmp1.v1.y, primCount);
			nmppsConvert_32s32f((int*)tmp1.v1.z, tmp1.v1.z, primCount);
			nmppsConvert_32s32f((int*)tmp1.v2.x, tmp1.v2.x, primCount);
			nmppsConvert_32s32f((int*)tmp1.v2.y, tmp1.v2.y, primCount);
			nmppsConvert_32s32f((int*)tmp1.v2.z, tmp1.v2.z, primCount);

			//данные хранятся в tmp1 (в buffer2), цвет в buffer1
			v2nm32f *minXY = (v2nm32f*)cntxt->buffer4;
			v2nm32f *maxXY = (v2nm32f*)cntxt->buffer4 + 3 * NMGL_SIZE;
			int srcThreated = 0;

			//переинициализация tmp0
			tmp0.v0.x = cntxt->buffer0 + 0 * NMGL_SIZE;
			tmp0.v0.y = cntxt->buffer0 + 1 * NMGL_SIZE;
			tmp0.v0.z = cntxt->buffer0 + 2 * NMGL_SIZE;
			tmp0.v1.x = cntxt->buffer0 + 3 * NMGL_SIZE;
			tmp0.v1.y = cntxt->buffer0 + 4 * NMGL_SIZE;
			tmp0.v1.z = cntxt->buffer0 + 5 * NMGL_SIZE;
			tmp0.v2.x = cntxt->buffer0 + 6 * NMGL_SIZE;
			tmp0.v2.y = cntxt->buffer0 + 7 * NMGL_SIZE;
			tmp0.v2.z = cntxt->buffer0 + 8 * NMGL_SIZE;
			
			while (srcThreated < primCount) {
				//nmprofiler_disable();
				//PROFILER_SIZE(primCount);
				int currentCount = triangulate(cntxt->buffer2, (v4nm32f*)cntxt->buffer1, primCount,
					WIDTH_PTRN, HEIGHT_PTRN,
					NMGL_SIZE, cntxt->buffer0, (v4nm32f*)cntxt->buffer3, &srcThreated);
				//nmprofiler_enable();			

				if (currentCount % 2) {					
					tmp0.v0.x[currentCount] = tmp0.v0.x[currentCount - 1];
					tmp0.v0.y[currentCount] = tmp0.v0.y[currentCount - 1];
					tmp0.v0.z[currentCount] = tmp0.v0.z[currentCount - 1];
					tmp0.v1.x[currentCount] = tmp0.v1.x[currentCount - 1];
					tmp0.v1.y[currentCount] = tmp0.v1.y[currentCount - 1];
					tmp0.v1.z[currentCount] = tmp0.v1.z[currentCount - 1];
					tmp0.v2.x[currentCount] = tmp0.v2.x[currentCount - 1];
					tmp0.v2.y[currentCount] = tmp0.v2.y[currentCount - 1];
					tmp0.v2.z[currentCount] = tmp0.v2.z[currentCount - 1];
					cntxt->buffer3[4 * (currentCount) + 0] = cntxt->buffer3[4 * (currentCount - 1) + 0];
					cntxt->buffer3[4 * (currentCount) + 1] = cntxt->buffer3[4 * (currentCount - 1) + 1];
					cntxt->buffer3[4 * (currentCount) + 2] = cntxt->buffer3[4 * (currentCount - 1) + 2];
					cntxt->buffer3[4 * (currentCount) + 3] = cntxt->buffer3[4 * (currentCount - 1) + 3];
					currentCount++;
				}

				//копирование каждого третьего цвета
				nmblas_dcopy(2 * currentCount, (double*)cntxt->buffer3, 6, (double*)colorOrNormal, 2);
				nmblas_dcopy(2 * currentCount, (double*)cntxt->buffer3 + 1, 6, (double*)colorOrNormal + 1, 2);				

				nmblas_scopy(currentCount, tmp0.v0.x, 1, cntxt->trianInner.x0, 1);
				nmblas_scopy(currentCount, tmp0.v0.y, 1, cntxt->trianInner.y0, 1);
				nmblas_scopy(currentCount, tmp0.v1.x, 1, cntxt->trianInner.x1, 1);
				nmblas_scopy(currentCount, tmp0.v1.y, 1, cntxt->trianInner.y1, 1);
				nmblas_scopy(currentCount, tmp0.v2.x, 1, cntxt->trianInner.x2, 1);
				nmblas_scopy(currentCount, tmp0.v2.y, 1, cntxt->trianInner.y2, 1);
				meanToInt3(tmp0.v0.z, tmp0.v1.z, tmp0.v2.z, cntxt->trianInner.z, currentCount);
				nmppsConvert_32f32s_rounding((float*)colorOrNormal, (int*)cntxt->trianInner.colors, 0, 4 * currentCount);
				cntxt->trianInner.size = currentCount;
				if (cntxt->isCullFace) {
					cullFaceSortTriangles(cntxt->trianInner);
					if (cntxt->trianInner.size == 0) {
						continue;
					}
				}
				findMinMax3(cntxt->trianInner.x0, cntxt->trianInner.x1, cntxt->trianInner.x2,
					cntxt->buffer0, cntxt->buffer1,
					cntxt->trianInner.size);
				findMinMax3(cntxt->trianInner.y0, cntxt->trianInner.y1, cntxt->trianInner.y2,
					cntxt->buffer2, cntxt->buffer3,
					cntxt->trianInner.size);
				nmppsMerge_32f(cntxt->buffer0, cntxt->buffer2, (float*)minXY, cntxt->trianInner.size);
				nmppsMerge_32f(cntxt->buffer1, cntxt->buffer3, (float*)maxXY, cntxt->trianInner.size);
				PROFILER_SIZE(cntxt->trianInner.size);
				setSegmentMask(minXY, maxXY, cntxt->segmentMasks, cntxt->trianInner.size);
				PROFILER_SIZE(cntxt->trianInner.size);
				rasterizeT(&cntxt->trianInner, cntxt->segmentMasks);
			}
			break;
		}
		case NMGL_LINE_STRIP:
		case NMGL_LINE_LOOP:
		case NMGL_LINES: {
			
			split_v4nm32f(vertexResult, 1, cntxt->buffer0, cntxt->buffer1, cntxt->buffer2, cntxt->buffer3, localSize);
			if (excessVertexCount) {
				cntxt->buffer0[localSize - 1] = cntxt->buffer0[localSize - 2];
				cntxt->buffer1[localSize - 1] = cntxt->buffer1[localSize - 2];
				cntxt->buffer2[localSize - 1] = cntxt->buffer2[localSize - 2];
				cntxt->buffer3[localSize - 1] = cntxt->buffer3[localSize - 2];
			}

			//------------clipping-------------------

			//------------perspective-division-----------------
			nmppsDiv_32f(cntxt->buffer0, cntxt->buffer3, cntxt->buffer1 + 3 * NMGL_SIZE, localSize);
			nmppsDiv_32f(cntxt->buffer1, cntxt->buffer3, cntxt->buffer2 + 3 * NMGL_SIZE, localSize);
			nmppsDiv_32f(cntxt->buffer2, cntxt->buffer3, cntxt->buffer0 + 3 * NMGL_SIZE, localSize);
			
			//------------viewport------------------------
			nmppsMulC_AddC_32f(cntxt->buffer1 + 3 * NMGL_SIZE, cntxt->windowInfo.viewportMulX, cntxt->windowInfo.viewportAddX, vertexX, localSize);		//X
			nmppsMulC_AddC_32f(cntxt->buffer2 + 3 * NMGL_SIZE, cntxt->windowInfo.viewportMulY, cntxt->windowInfo.viewportAddY, vertexY, localSize);		//Y
			nmppsMulC_AddC_32f(cntxt->buffer0 + 3 * NMGL_SIZE, cntxt->windowInfo.viewportMulZ, cntxt->windowInfo.viewportAddZ, vertexZ, localSize);	//Z

			nmppsConvert_32f32s_floor(vertexX, (int*)cntxt->buffer0, 0, localSize);
			nmppsConvert_32s32f((int*)cntxt->buffer0, vertexX, localSize);
			nmppsConvert_32f32s_floor(vertexY, (int*)cntxt->buffer0, 0, localSize);
			nmppsConvert_32s32f((int*)cntxt->buffer0, vertexY, localSize);
			v2nm32f *minXY = (v2nm32f*)cntxt->buffer4;
			v2nm32f *maxXY = (v2nm32f*)cntxt->buffer4 + 3 * NMGL_SIZE;
			pushToLines(vertexX, vertexY, vertexZ, colorOrNormal, cntxt->lineInner, mode, localSize);
			
			findMinMax2(cntxt->lineInner.x0, cntxt->lineInner.x1,
				cntxt->buffer0, cntxt->buffer1,
				cntxt->lineInner.size);
			findMinMax2(cntxt->lineInner.y0, cntxt->lineInner.y1,
				cntxt->buffer2, cntxt->buffer3,
				cntxt->lineInner.size);
			nmppsMerge_32f(cntxt->buffer0, cntxt->buffer2, (float*)minXY, cntxt->lineInner.size);
			nmppsMerge_32f(cntxt->buffer1, cntxt->buffer3, (float*)maxXY, cntxt->lineInner.size);
			setSegmentMask(minXY, maxXY, cntxt->segmentMasks, cntxt->lineInner.size);
			PROFILER_SIZE(cntxt->lineInner.size);
			rasterizeL(&cntxt->lineInner, cntxt->segmentMasks);
			break;
		}
		case NMGL_POINTS: {
			v2nm32f *minXY = (v2nm32f*)cntxt->buffer4;
			v2nm32f *maxXY = (v2nm32f*)cntxt->buffer4 + 3 * NMGL_SIZE;
			nmblas_scopy(localSize, vertexX, 1, cntxt->pointInner.x0, 1);
			nmblas_scopy(localSize, vertexY, 1, cntxt->pointInner.y0, 1);
			nmppsConvert_32f32s_rounding((float*)colorOrNormal, (int*)cntxt->pointInner.colors, 0, 4 * localSize);
			nmppsConvert_32f32s_rounding(vertexZ, cntxt->pointInner.z, 0, localSize);
			nmppsSubC_32f(vertexX, cntxt->buffer0, cntxt->pointRadius, localSize);
			nmppsSubC_32f(vertexY, cntxt->buffer1, cntxt->pointRadius, localSize);
			nmppsMerge_32f(cntxt->buffer0, cntxt->buffer1, (float*)minXY, localSize);
			nmppsAddC_32f(vertexX, cntxt->buffer0, cntxt->pointRadius, localSize);
			nmppsAddC_32f(vertexY, cntxt->buffer1, cntxt->pointRadius, localSize);
			nmppsMerge_32f(cntxt->buffer0, cntxt->buffer1, (float*)maxXY, localSize);
			cntxt->pointInner.size = localSize;
			setSegmentMask(minXY, maxXY, cntxt->segmentMasks, localSize);
			rasterizeP(&cntxt->pointInner, cntxt->segmentMasks);
			break;
		}
		}
		
#endif
	}

#ifdef DEBUG
	//printf("DrawArrays end\n\n");
#endif // DEBUG
}