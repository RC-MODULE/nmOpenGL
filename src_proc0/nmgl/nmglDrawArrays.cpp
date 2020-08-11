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

SECTION(".data_imu5")	float vertexX[3 * NMGL_SIZE];
SECTION(".data_imu6")	float vertexY[3 * NMGL_SIZE];
SECTION(".data_imu4")	float vertexZ[3 * NMGL_SIZE];

SECTION(".data_imu5")	v4nm32f vertexResult[3 * NMGL_SIZE];
SECTION(".data_imu6")	v4nm32f colorOrNormal[3 * NMGL_SIZE];

SECTION(".data_imu6")	ArrayManager<float> vertexAM;
SECTION(".data_imu6")	ArrayManager<float> normalAM;
SECTION(".data_imu6")	ArrayManager<v4nm32f> colorAM;



struct TrianglePrimitiveArrays3f {
	float* x0;
	float* y0;
	float* z0;
	float* x1;
	float* y1;
	float* z1;
	float* x2;
	float* y2;
	float* z2;
};

struct TrianglePrimitiveArrays4f {
	float* x0;
	float* y0;
	float* z0;
	float* w0;
	float* x1;
	float* y1;
	float* z1;
	float* w1;
	float* x2;
	float* y2;
	float* z2;
	float* w2;
};


template < typename T >
inline void copyVec(const void* src, void* dst, size_t size) {
	nmblas_scopy(size * sizeof32(T), (float*)src, 1, (float*)dst, 1);
}

/* функция nmglDrawArrays является функцией со статическим конвеером.
Значения переданные из массивов, которые задаются с помощью функций 
nmglVertexPointer, nmglNormalPointer, nmglColorPointer
проходят следующие этапы.

							Видовая
							матрица
Координаты вершин, нормалей ---------> Видовые координаты
*/


void printCrc(void* src, int size32, char* s = 0) {
	unsigned crc = 0;
	static unsigned counter = 0;
	halCrcAcc_32f((float *)src, size32, &crc);
	if (s == 0)
		printf("%d 0x%x\n", counter++, crc);
	else
		printf("%d %s 0x%x\n", counter++, s, crc);
}

SECTION(".text_nmgl")
void nmglDrawArrays(NMGLenum mode, NMGLint first, NMGLsizei count) {
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


	int maxInnerCount;
	int nAllPrimitives;
	switch (mode) {
	case NMGL_TRIANGLES:
		maxInnerCount = 3 * NMGL_SIZE;
		nAllPrimitives = count / 3;
		break;
	case NMGL_LINES:
		maxInnerCount = 2 * NMGL_SIZE;
		nAllPrimitives = count - 2;
		break;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}

	vertexAM.set(srcDDR_vertex, cntxt->vertexArray.size * count, cntxt->vertexArray.size * maxInnerCount, copyVec<float>);
	normalAM.set(srcDDR_normal, cntxt->normalArray.size * count, cntxt->normalArray.size * maxInnerCount, copyVec<float>);
	colorAM.set(srcDDR_color, count, maxInnerCount, copyVec<v4nm32f>);

	if (cntxt->isLighting) {
		mulC_v4nm32f(cntxt->lightAmbient, &cntxt->materialAmbient, cntxt->ambientMul, MAX_LIGHTS + 1);
		mulC_v4nm32f(cntxt->lightDiffuse, &cntxt->materialDiffuse, cntxt->diffuseMul, MAX_LIGHTS);
		mulC_v4nm32f(cntxt->lightSpecular, &cntxt->materialSpecular, cntxt->specularMul, MAX_LIGHTS);
		nmppsAdd_32f((float*)(cntxt->ambientMul + MAX_LIGHTS), 
			(float*)&cntxt->materialEmissive, 
			(float*)(cntxt->ambientMul + MAX_LIGHTS), 4);
	}
	reverseMatrix3x3in4x4(cntxt->modelviewMatrixStack.top(), &cntxt->normalMatrix);

	while (!vertexAM.isEmpty()) {
		//vertex
		int localSize = vertexAM.pop(cntxt->buffer0) / cntxt->vertexArray.size;
		switch (cntxt->vertexArray.size)
		{
		case 2:
			nmblas_dcopy(localSize, (double*)cntxt->buffer0, 1, (double*)cntxt->buffer1, 2);
			cntxt->tmp.vec[0] = 0;
			cntxt->tmp.vec[1] = 1;
			nmblas_dcopy(localSize, (double*)&cntxt->tmp, 0, (double*)(cntxt->buffer1 + 2), 2);
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
		mul_mat4nm32f_v4nm32f(cntxt->modelviewMatrixStack.top(), (v4nm32f*)cntxt->buffer1, vertexResult, localSize);

		//color
		if (cntxt->colorArray.enabled) {
			colorAM.pop(colorOrNormal);
			if (cntxt->colorArray.type == NMGL_UNSIGNED_BYTE) {
				nmppsConvert_32s32f((int*)colorOrNormal, cntxt->buffer0, cntxt->colorArray.size * localSize);
				nmppsMulC_32f(cntxt->buffer0, (float*)colorOrNormal, 1.0/255.0, cntxt->colorArray.size * localSize);
			}
		}

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

		for (int i = 0; i < localSize; i++) {
			//colorOrNormal[i].vec[0] = 0;
			//colorOrNormal[i].vec[1] = 0;
			//colorOrNormal[i].vec[2] = 0;
		}

		clamp_32f((float*)colorOrNormal, 0, 1, (float*)cntxt->buffer3, 4 * localSize);
		cntxt->tmp.vec[0] = RED_COEFF;
		cntxt->tmp.vec[1] = GREEN_COEFF;
		cntxt->tmp.vec[2] = BLUE_COEFF;
		cntxt->tmp.vec[3] = ALPHA_COEFF;
		mulC_v4nm32f((v4nm32f*)cntxt->buffer3, &cntxt->tmp, colorOrNormal, localSize);

		mul_mat4nm32f_v4nm32f(cntxt->projectionMatrixStack.top(), vertexResult, (v4nm32f*)vertexResult, localSize);



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

		nmppsConvert_32f32s_rounding(vertexX, (int*)cntxt->buffer0, 0, localSize);
		nmppsConvert_32s32f((int*)cntxt->buffer0, vertexX, localSize);
		nmppsConvert_32f32s_rounding(vertexY, (int*)cntxt->buffer0, 0, localSize);
		nmppsConvert_32s32f((int*)cntxt->buffer0, vertexY, localSize);



		//---------------rasterize------------------------------------
		v2nm32f *minXY = (v2nm32f*)cntxt->buffer4;
		v2nm32f *maxXY = (v2nm32f*)cntxt->buffer4 + NMGL_SIZE;
		switch (mode) {
		case NMGL_TRIANGLES: {
			pushToTriangles_t(vertexX, vertexY, vertexZ, colorOrNormal, cntxt->trianInner, localSize);

			if (cntxt->isCullFace) {
				cullFaceSortTriangles(cntxt->trianInner);
			}
			if (cntxt->trianInner.size == 0) {
				break;
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
		}

		
	}
}