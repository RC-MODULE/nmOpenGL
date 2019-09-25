#include "demo3d_nm0.h"
#include "nmpp.h"
#include "nmblas.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "malloc.h"
#include "arraymanager.h"
#include "stdio.h"


SECTION(".data_imu6")	float x0[NMGL_SIZE];
SECTION(".data_imu6")	float y0[NMGL_SIZE];
SECTION(".data_imu5")	float x1[NMGL_SIZE];
SECTION(".data_imu5")	float y1[NMGL_SIZE];
SECTION(".data_imu4")	float x2[NMGL_SIZE];
SECTION(".data_imu4")	float y2[NMGL_SIZE];
SECTION(".data_imu6")	int z_int[NMGL_SIZE];
SECTION(".data_imu6")	v4nm32s lightsValues[NMGL_SIZE];
SECTION(".data_imu6")	Triangles trianglesStat = { x0, y0, x1, y1, x2, y2, z_int, lightsValues };
SECTION(".data_imu6")	Lines linesStat = { x0, y0,  x1, y1, z_int, lightsValues };

SECTION(".data_imu5")	float vertexX[3 * NMGL_SIZE];
SECTION(".data_imu6")	float vertexY[3 * NMGL_SIZE];
SECTION(".data_imu4")	float vertexZ[3 * NMGL_SIZE];

SECTION(".data_imu5")	v4nm32f vertexResult[3 * NMGL_SIZE];
SECTION(".data_imu6")	v4nm32f colorOrNormal[3 * NMGL_SIZE];

SECTION(".data_imu6")	ArrayManager<float> vertexAM;
SECTION(".data_imu6")	ArrayManager<float> normalAM;
SECTION(".data_imu6")	ArrayManager<float> colorAM;
SECTION(".data_imu6")   TrianglesDdr trianglesDdr;

extern float minXY[2 * NMGL_SIZE];
extern float maxXY[2 * NMGL_SIZE];
extern int maskRes[NMGL_SIZE / 32];
extern MasksSeg masks;
extern Triangles localTr;

inline int andMask(MasksSeg* srcMask, int* dstMask, int size) {
	int i = 0;
	int result = 0;
	while (size>0) {
		dstMask[i] = (srcMask->xLt[i] & srcMask->yLt[i]) & (srcMask->xGt[i] & srcMask->yGt[i]);
		result |= dstMask[i];
		i++;
		size -= 32;
	}
	return result;
}


template < typename T >
inline void copyVec(const T* src, T* dst, size_t size) {
	nmblas_scopy(size * sizeof32(T), (float*)src, 1, (float*)dst, 1);
}

SECTION(".text_nmgl")
void trianglesDdrInitAlloc(TrianglesDdr* trian, int size) {
#ifdef __NMC_GCC__
	nmc_malloc_set_heap(4);
#endif // __NMC_GCC__
	trian->mallocAddr = malloc(11 * size * sizeof(int));
	trian->x2 = (float*)trian->mallocAddr;
	trian->y2 = nmppsAddr_32f(trian->x2, size);
	trian->x1 = nmppsAddr_32f(trian->y2, size);
	trian->y1 = nmppsAddr_32f(trian->x1, size);
	trian->x0 = nmppsAddr_32f(trian->y1, size);
	trian->y0 = nmppsAddr_32f(trian->x0, size);
	trian->z = (int*)nmppsAddr_32f(trian->y0, size);
	trian->colors = (v4nm32s*)nmppsAddr_32s(trian->z, size);
	trian->offset = 0;
}

inline void trianglesDdrFree(TrianglesDdr* trian) {
	free(trian->mallocAddr);
}

SECTION(".text_nmgl")
void trianglesDdrPush(Triangles* srcTrian, TrianglesDdr* dstTrian, int size) {
	copyVec(srcTrian->x2, dstTrian->x2 + dstTrian->offset, size);
	copyVec(srcTrian->y2, dstTrian->y2 + dstTrian->offset, size);
	copyVec(srcTrian->x1, dstTrian->x1 + dstTrian->offset, size);
	copyVec(srcTrian->y1, dstTrian->y1 + dstTrian->offset, size);
	copyVec(srcTrian->x0, dstTrian->x0 + dstTrian->offset, size);
	copyVec(srcTrian->y0, dstTrian->y0 + dstTrian->offset, size);
	copyVec(srcTrian->z, dstTrian->z + dstTrian->offset, size);
	copyVec(srcTrian->colors, dstTrian->colors + dstTrian->offset, size);
	dstTrian->offset += size;
}

SECTION(".text_nmgl")
void trianglesDdrPop(TrianglesDdr* srcTrian, Triangles* dstTrian, int size) {
	copyVec(srcTrian->x2 + srcTrian->offset, dstTrian->x2, size);
	copyVec(srcTrian->y2 + srcTrian->offset, dstTrian->y2, size);
	copyVec(srcTrian->x1 + srcTrian->offset, dstTrian->x1, size);
	copyVec(srcTrian->y1 + srcTrian->offset, dstTrian->y1, size);
	copyVec(srcTrian->x0 + srcTrian->offset, dstTrian->x0, size);
	copyVec(srcTrian->y0 + srcTrian->offset, dstTrian->y0, size);
	copyVec(srcTrian->z + srcTrian->offset, dstTrian->z, size);
	copyVec(srcTrian->colors + srcTrian->offset, dstTrian->colors, size);
	srcTrian->offset += size;
}

inline void trianglesDdrReset(TrianglesDdr* trian) {
	trian->offset = 0;
}


SECTION(".text_nmgl")
void copyVertex2(const float* src, float* dst, size_t size) {
	nmblas_dcopy(size, (double*)src, 1, (double*)dst, 2);
	v2nm32f zero_one = { 0,1 };
	nmblas_dcopy(size, (double*)&zero_one, 0, (double*)(dst + 2), 2);
}

//использует cntxt.buffer3 как временный буфер
SECTION(".text_nmgl")
void copyVertex3(const float* src, float* dst, size_t size) {
	nmblas_scopy(size, src, 1, (float*)cntxt.buffer3, 1);
	cnv32f_v3v4(cntxt.buffer3, dst, 1, size / 3);
}

SECTION(".text_nmgl")
void copyNormal(const float* src, float* dst, size_t size) {
	nmblas_scopy(size, src, 1, (float*)cntxt.buffer3, 1);
	cnv32f_v3v4(cntxt.buffer3, dst, 0, size / 3);
}

SECTION(".text_nmgl")
void nmglDrawArrays(NMGLenum mode, NMGLint first, NMGLsizei count) {
	if (cntxt.vertexArray.enabled == NMGL_FALSE) {
		return;
	}
	if (count < 0) {
		cntxt.error = NMGL_INVALID_VALUE;
		return;
	}
	float* srcDDR_vertex = (float*)cntxt.vertexArray.pointer + cntxt.vertexArray.size * first;
	float* srcDDR_normal = (float*)cntxt.normalArray.pointer + cntxt.normalArray.size * first;
	float* srcDDR_color = (float*)cntxt.colorArray.pointer + cntxt.colorArray.size * first;

	int maxInnerCount;
	switch (mode) {
	case NMGL_TRIANGLES:
		maxInnerCount = 3 * NMGL_SIZE;
		break;
	case NMGL_LINES:
		maxInnerCount = 2 * NMGL_SIZE;
		break;
	default:
		cntxt.error = NMGL_INVALID_ENUM;
		return;
	}

	switch (cntxt.vertexArray.size)
	{
	case 2:
		vertexAM.set(srcDDR_vertex, cntxt.vertexArray.size * count, cntxt.vertexArray.size * maxInnerCount, copyVertex2);
		break;
	case 3:
		vertexAM.set(srcDDR_vertex, cntxt.vertexArray.size * count, cntxt.vertexArray.size * maxInnerCount, copyVertex3);
		break;
	case 4:
		vertexAM.set(srcDDR_vertex, cntxt.vertexArray.size * count, cntxt.vertexArray.size * maxInnerCount, copyVec<float>);
		break;
	}
	normalAM.set(srcDDR_normal, cntxt.normalArray.size * count, cntxt.normalArray.size * maxInnerCount, copyNormal);
	colorAM.set(srcDDR_color, cntxt.colorArray.size * count, cntxt.colorArray.size * maxInnerCount, copyVec<float>);

	if (cntxt.isLighting) {
		mulC_v4nm32f(cntxt.lightAmbient, &cntxt.materialAmbient, cntxt.ambientMul, MAX_LIGHTS + 1);
		mulC_v4nm32f(cntxt.lightDiffuse, &cntxt.materialDiffuse, cntxt.diffuseMul, MAX_LIGHTS);
		mulC_v4nm32f(cntxt.lightSpecular, &cntxt.materialSpecular, cntxt.specularMul, MAX_LIGHTS);
	}

	//trianglesDdrInitAlloc(&trianglesDdr, nPrim);
	int allRasterizeCount = 0;

	while (!vertexAM.isEmpty()) {
		
		int localSize = vertexAM.pop(cntxt.buffer0) / cntxt.vertexArray.size;
		mul_mat4nm32f_v4nm32f(cntxt.modelviewMatrixStack.top(), (v4nm32f*)cntxt.buffer0, vertexResult, localSize);

		if (cntxt.colorArray.enabled) {
			colorAM.pop((float*)colorOrNormal);
			if (cntxt.colorArray.type == NMGL_UNSIGNED_BYTE) {
				nmppsConvert_32s32f((int*)colorOrNormal, cntxt.buffer0, cntxt.colorArray.size * localSize);
				nmppsMulC_32f(cntxt.buffer0, (float*)colorOrNormal, 1.0/255.0, cntxt.colorArray.size * localSize);
			}
		}

		if (cntxt.normalArray.enabled) {
			normalAM.pop((float*)cntxt.buffer1);
			mul_mat4nm32f_v4nm32f(cntxt.modelviewMatrixStack.top(), (v4nm32f*)cntxt.buffer1, colorOrNormal, localSize);
			if (cntxt.normalizeEnabled) {
				nmblas_scopy(4 * localSize, (float*)colorOrNormal, 1, cntxt.buffer2, 1);
				dotV_v4nm32f(colorOrNormal, (v4nm32f*)cntxt.buffer2, (v2nm32f*)cntxt.buffer0, localSize);
				pow_32f(cntxt.buffer0, cntxt.buffer1, 0.5, 2 * localSize, cntxt.buffer3);
				nmppmCopy_32fc((double*)cntxt.buffer1, 1, (double*)cntxt.buffer3, 2, localSize, 1);
				nmppmCopy_32fc((double*)cntxt.buffer1, 1, (double*)(cntxt.buffer3 + 2), 2, localSize, 1);
				nmppsDiv_32f((float*)cntxt.buffer2, cntxt.buffer3, (float*)colorOrNormal, 4 * localSize);
			}
		}		


		//vertex in vertexResult
		//normal in colorOrNormal
		//Освещение или наложение цветов
		if (cntxt.isLighting) {
			light(vertexResult, colorOrNormal, localSize);
		}

		clamp_32f((float*)colorOrNormal, 0, 1, (float*)cntxt.buffer3, 4 * localSize);
		nmppsMulC_32f((float*)cntxt.buffer3, (float*)colorOrNormal, 255, 4 * localSize);
		//----------------------------------

		//vertex in vertexResult
		//color in colorOrNormal
		mul_mat4nm32f_v4nm32f(cntxt.projectionMatrixStack.top(), vertexResult, (v4nm32f*)vertexResult, localSize);
		//------------------------------srcX-----srcY-----srcZ-----srcW--------------
		split_v4nm32f(vertexResult, 1, cntxt.buffer0, cntxt.buffer1, cntxt.buffer2, cntxt.buffer3, localSize);

		//------------clipping-------------------

		//------------perspective-division-----------------
		nmppsDiv_32f(cntxt.buffer0, cntxt.buffer3, cntxt.buffer1 + localSize, localSize);
		nmppsDiv_32f(cntxt.buffer1, cntxt.buffer3, cntxt.buffer2 + localSize, localSize);
		nmppsDiv_32f(cntxt.buffer2, cntxt.buffer3, cntxt.buffer0 + localSize, localSize);
		//------------viewport------------------------
		nmppsMulC_AddC_32f(cntxt.buffer1 + localSize, cntxt.windowInfo.viewportMulX, cntxt.windowInfo.viewportAddX, vertexX, localSize);		//X
		nmppsMulC_AddC_32f(cntxt.buffer2 + localSize, cntxt.windowInfo.viewportMulY, cntxt.windowInfo.viewportAddY, vertexY, localSize);		//Y
		nmppsMulC_AddC_32f(cntxt.buffer0 + localSize, cntxt.windowInfo.viewportMulZ, cntxt.windowInfo.viewportAddZ, vertexZ, localSize);	//Z

		nmppsConvert_32f32s_rounding(vertexX, (int*)cntxt.buffer0, 0, localSize);
		nmppsConvert_32s32f((int*)cntxt.buffer0, vertexX, localSize);
		nmppsConvert_32f32s_rounding(vertexY, (int*)cntxt.buffer0, 0, localSize);
		nmppsConvert_32s32f((int*)cntxt.buffer0, vertexY, localSize);
		int localNPrim;
		//---------------rasterize------------------------------------
		switch (mode) {
		case NMGL_TRIANGLES:
			nmblas_dcopy(2 * localSize / 3, (double*)colorOrNormal, 3, (double*)cntxt.buffer0, 1);
			nmppsConvert_32f32s_rounding(cntxt.buffer0, (int*)lightsValues, 0, 4 * localSize / 3);

			cnv32f_v3v4(vertexX, cntxt.buffer3, 0, localSize);
			split_v4nm32f((v4nm32f*)cntxt.buffer3, 1, x0, x1, x2, cntxt.buffer3 + 4 * localSize / 3, localSize / 3);

			cnv32f_v3v4(vertexY, cntxt.buffer3, 0, localSize);
			split_v4nm32f((v4nm32f*)cntxt.buffer3, 1, y0, y1, y2, cntxt.buffer3 + 4 * localSize / 3, localSize / 3);

			cnv32f_v3v4(vertexZ, cntxt.buffer3, 0, localSize);
			split_v4nm32f((v4nm32f*)cntxt.buffer3, 1, cntxt.buffer0, cntxt.buffer1, cntxt.buffer2, cntxt.buffer3 + 4 * localSize / 3, localSize / 3);
			meanToInt3(cntxt.buffer0, cntxt.buffer1, cntxt.buffer2, z_int, localSize / 3);

			localNPrim = localSize / 3;

			if (cntxt.isCullFace) {
				localNPrim = cullFaceSortTriangles(&trianglesStat, localNPrim);
			}

			allRasterizeCount += localNPrim;
			rasterizeT(&trianglesStat, localNPrim);
			break;
		case NMGL_LINES:
			localNPrim = localSize / 2;
			nmblas_dcopy(2 * localNPrim, (double*)colorOrNormal, 2, (double*)cntxt.buffer0, 1);
			nmppsConvert_32f32s_rounding(cntxt.buffer0, (int*)lightsValues, 0, 4 * localNPrim);

			split_v2nm32f((v2nm32f*)vertexX, 1, x0, x1, localNPrim);
			split_v2nm32f((v2nm32f*)vertexY, 1, y0, y1, localNPrim);
			split_v2nm32f((v2nm32f*)vertexZ, 1, cntxt.buffer0, cntxt.buffer1, localNPrim);
			meanToInt2(cntxt.buffer0, cntxt.buffer1, z_int, localNPrim);
			rasterizeL(&linesStat, localNPrim);
			break;
		}
	}
}