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

SECTION(".data_imu5")	v4nm32f vertexResult[3 * NMGL_SIZE];
SECTION(".data_imu6")	v4nm32f colorOrNormal[3 * NMGL_SIZE];
SECTION(".data_imu6")	v2nm32f texResult[3 * NMGL_SIZE];

SECTION(".data_shared0") float saveTriangles[30 * NMGL_SIZE];

SECTION("text_nmgl") void pushPointers(TrianglePointers &pointers, int size) {
	nmblas_scopy(size, pointers.v0.x, 1, saveTriangles + 0 * NMGL_SIZE, 1);
	nmblas_scopy(size, pointers.v0.y, 1, saveTriangles + 1 * NMGL_SIZE, 1);
	nmblas_scopy(size, pointers.v0.z, 1, saveTriangles + 2 * NMGL_SIZE, 1);
	nmblas_scopy(size, pointers.v0.w, 1, saveTriangles + 3 * NMGL_SIZE, 1);
	nmblas_scopy(size, pointers.v1.x, 1, saveTriangles + 4 * NMGL_SIZE, 1);
	nmblas_scopy(size, pointers.v1.y, 1, saveTriangles + 5 * NMGL_SIZE, 1);
	nmblas_scopy(size, pointers.v1.z, 1, saveTriangles + 6 * NMGL_SIZE, 1);
	nmblas_scopy(size, pointers.v1.w, 1, saveTriangles + 7 * NMGL_SIZE, 1);
	nmblas_scopy(size, pointers.v2.x, 1, saveTriangles + 8 * NMGL_SIZE, 1);
	nmblas_scopy(size, pointers.v2.y, 1, saveTriangles + 9 * NMGL_SIZE, 1);
	nmblas_scopy(size, pointers.v2.z, 1, saveTriangles + 10 * NMGL_SIZE, 1);
	nmblas_scopy(size, pointers.v2.w, 1, saveTriangles + 11 * NMGL_SIZE, 1);

	nmblas_scopy(4 * size, (float*)pointers.v0.color, 1, saveTriangles + 12 * NMGL_SIZE, 1);
	nmblas_scopy(4 * size, (float*)pointers.v1.color, 1, saveTriangles + 16 * NMGL_SIZE, 1);
	nmblas_scopy(4 * size, (float*)pointers.v2.color, 1, saveTriangles + 20 * NMGL_SIZE, 1);

#ifdef TEXTURE_ENABLED
	nmblas_scopy(size, (float*)pointers.v0.s, 1, saveTriangles + 24 * NMGL_SIZE, 1);
	nmblas_scopy(size, (float*)pointers.v0.t, 1, saveTriangles + 25 * NMGL_SIZE, 1);
	nmblas_scopy(size, (float*)pointers.v1.s, 1, saveTriangles + 26 * NMGL_SIZE, 1);
	nmblas_scopy(size, (float*)pointers.v1.t, 1, saveTriangles + 27 * NMGL_SIZE, 1);
	nmblas_scopy(size, (float*)pointers.v2.s, 1, saveTriangles + 28 * NMGL_SIZE, 1);
	nmblas_scopy(size, (float*)pointers.v2.t, 1, saveTriangles + 29 * NMGL_SIZE, 1);
#endif //TEXTURE_ENABLED
}

SECTION("text_nmgl") void popPointers(TrianglePointers &pointers, int size) {
	nmblas_scopy(size, saveTriangles + 0 * NMGL_SIZE, 1 , pointers.v0.x, 1);
	nmblas_scopy(size, saveTriangles + 1 * NMGL_SIZE, 1 , pointers.v0.y, 1);
	nmblas_scopy(size, saveTriangles + 2 * NMGL_SIZE, 1 , pointers.v0.z, 1);
	nmblas_scopy(size, saveTriangles + 3 * NMGL_SIZE, 1 , pointers.v0.w, 1);
	nmblas_scopy(size, saveTriangles + 4 * NMGL_SIZE, 1 , pointers.v1.x, 1);
	nmblas_scopy(size, saveTriangles + 5 * NMGL_SIZE, 1 , pointers.v1.y, 1);
	nmblas_scopy(size, saveTriangles + 6 * NMGL_SIZE, 1 , pointers.v1.z, 1);
	nmblas_scopy(size, saveTriangles + 7 * NMGL_SIZE, 1 , pointers.v1.w, 1);
	nmblas_scopy(size, saveTriangles + 8 * NMGL_SIZE, 1 , pointers.v2.x, 1);
	nmblas_scopy(size, saveTriangles + 9 * NMGL_SIZE, 1 , pointers.v2.y, 1);
	nmblas_scopy(size, saveTriangles + 10 * NMGL_SIZE, 1, pointers.v2.z, 1);
	nmblas_scopy(size, saveTriangles + 11 * NMGL_SIZE, 1, pointers.v2.w, 1);

	nmblas_scopy(4 * size, saveTriangles + 12 * NMGL_SIZE, 1, (float*)pointers.v0.color, 1);
	nmblas_scopy(4 * size, saveTriangles + 16 * NMGL_SIZE, 1, (float*)pointers.v1.color, 1);
	nmblas_scopy(4 * size, saveTriangles + 20 * NMGL_SIZE, 1, (float*)pointers.v2.color, 1);

#ifdef TEXTURE_ENABLED
	nmblas_scopy(size, saveTriangles + 24 * NMGL_SIZE, 1, (float*)pointers.v0.s, 1);
	nmblas_scopy(size, saveTriangles + 25 * NMGL_SIZE, 1, (float*)pointers.v0.t, 1);
	nmblas_scopy(size, saveTriangles + 26 * NMGL_SIZE, 1, (float*)pointers.v1.s, 1);
	nmblas_scopy(size, saveTriangles + 27 * NMGL_SIZE, 1, (float*)pointers.v1.t, 1);
	nmblas_scopy(size, saveTriangles + 28 * NMGL_SIZE, 1, (float*)pointers.v2.s, 1);
	nmblas_scopy(size, saveTriangles + 29 * NMGL_SIZE, 1, (float*)pointers.v2.t, 1);
#endif //TEXTURE_ENABLED
}

SECTION(".data_imu6")	ArrayManager<float> vertexAM;
SECTION(".data_imu6")	ArrayManager<float> normalAM;
SECTION(".data_imu6")	ArrayManager<v4nm32f> colorAM;
#ifdef TEXTURE_ENABLED
SECTION(".data_imu6")	ArrayManager<float> texcoordAM;
#endif //TEXTURE_ENABLED

SECTION(".data_imu6")	BitDividedMask clipMask[10];

SECTION("text_nmgl") void perpectiveDivView(CombinePointers &vertex, WindowInfo &windowInfo, float* tmpBuf, int size) {
	nmppsDiv_32f(vertex.x, vertex.w, tmpBuf + 0 * NMGL_SIZE, size);
	nmppsDiv_32f(vertex.y, vertex.w, tmpBuf + 1 * NMGL_SIZE, size);
	nmppsDiv_32f(vertex.z, vertex.w, tmpBuf + 2 * NMGL_SIZE, size);


	nmppsMulC_AddC_32f(tmpBuf + 0 * NMGL_SIZE, windowInfo.viewportMulX, windowInfo.viewportAddX, vertex.x, size);		//X	
	nmppsMulC_AddC_32f(tmpBuf + 1 * NMGL_SIZE, windowInfo.viewportMulY, windowInfo.viewportAddY, vertex.y, size);		//Y	
	nmppsMulC_AddC_32f(tmpBuf + 2 * NMGL_SIZE, windowInfo.viewportMulZ, windowInfo.viewportAddZ, vertex.z, size);		//Z
}

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


SECTION("text_demo3d") void clipSelect(TrianglePointers *src, float* srcColor, int* indices, TrianglePointers * dst, float* dstColor, int size) {
	copyArraysByIndices((void**)&src->v0, indices, (void**)&dst->v0, 4, size);
	copyArraysByIndices((void**)&src->v1, indices, (void**)&dst->v1, 4, size);
	copyArraysByIndices((void**)&src->v2, indices, (void**)&dst->v2, 4, size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 12; j++) {
			dstColor[12 * i + j] = srcColor[12 * indices[i] + j];
		}
	}
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
	cntxt->vertexArray.offset = first;
	cntxt->normalArray.offset = first;
	cntxt->colorArray.offset = first;

#ifdef TEXTURE_ENABLED
	unsigned int clientActiveTexUnitIndex;
	float* srcDDR_texcoords;
	if (cntxt->texState.textureEnabled) {
		clientActiveTexUnitIndex = cntxt->texState.clientActiveTexUnitIndex;
		srcDDR_texcoords = (float*)cntxt->texState.texcoordArray[clientActiveTexUnitIndex].pointer + cntxt->texState.texcoordArray[clientActiveTexUnitIndex].size * first;
		cntxt->texState.texcoordArray[clientActiveTexUnitIndex].offset = first;
	}
#endif //TEXTURE_ENABLED

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
	case NMGL_LINE_STRIP:
		maxInnerCount = NMGL_SIZE + 1;
		break;
	case NMGL_LINE_LOOP:
		maxInnerCount = NMGL_SIZE;
		break;
	case NMGL_POINTS:
		maxInnerCount = NMGL_SIZE;
		break;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	if(first % 2 == 0 && count % 2 == 0){
		vertexAM.set(srcDDR_vertex, cntxt->vertexArray.size * count, cntxt->vertexArray.size * maxInnerCount, copyVec<float>);
		normalAM.set(srcDDR_normal, cntxt->normalArray.size * count, cntxt->normalArray.size * maxInnerCount, copyVec<float>);
		colorAM.set(srcDDR_color, count, maxInnerCount, copyVec<v4nm32f>);
#ifdef TEXTURE_ENABLED
		if (cntxt->texState.textureEnabled) {
			texcoordAM.set(srcDDR_texcoords, cntxt->texState.texcoordArray[clientActiveTexUnitIndex].size * count, 
				           cntxt->texState.texcoordArray[clientActiveTexUnitIndex].size * maxInnerCount, copyVec<float>);
		}
#endif //TEXTURE_ENABLED
	}else{
		vertexAM.set(srcDDR_vertex, cntxt->vertexArray.size * count, cntxt->vertexArray.size * maxInnerCount, copyRisc<float>);
		normalAM.set(srcDDR_normal, cntxt->normalArray.size * count, cntxt->normalArray.size * maxInnerCount, copyRisc<float>);
		colorAM.set(srcDDR_color, count, maxInnerCount, copyRisc<v4nm32f>);
#ifdef TEXTURE_ENABLED
		if (cntxt->texState.textureEnabled) {
			texcoordAM.set(srcDDR_texcoords, cntxt->texState.texcoordArray[clientActiveTexUnitIndex].size * count, 
				           cntxt->texState.texcoordArray[clientActiveTexUnitIndex].size * maxInnerCount, copyRisc<float>);
		}
#endif //TEXTURE_ENABLED
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
		switch (cntxt->vertexArray.size)
		{
		case 2:
			cnv32f_v2v4((v2nm32f*)cntxt->buffer0, (v4nm32f*)cntxt->buffer1, 0, 1, localSize);
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

		//texcoords
#ifdef TEXTURE_ENABLED
		if (cntxt->texState.textureEnabled) {
			//texture coordinates
			if (cntxt->texState.texcoordArray[clientActiveTexUnitIndex].enabled) {
				texcoordAM.pop(cntxt->buffer0);
				switch (cntxt->texState.texcoordArray[clientActiveTexUnitIndex].size) //Must be equal to 2
				{
				case 2:
					nmblas_dcopy(localSize, (double*)cntxt->buffer0, 1, (double*)cntxt->buffer1, 1);
					break;
				default:
					break;
				}
				nmblas_scopy(2 * localSize, cntxt->buffer1, 1, (float*)texResult, 1);
			}
		}
#endif //TEXTURE_ENABLED

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
		//nmprofiler_enable();
		if (cntxt->isLighting) {
			PROFILER_SIZE(localSize);
			light(vertexResult, colorOrNormal, localSize);
		}
		else {
			cntxt->tmp.vec[0] = 1;
			cntxt->tmp.vec[1] = 1;
			cntxt->tmp.vec[2] = 1;
			cntxt->tmp.vec[3] = 1;
			set_v4nm32f(colorOrNormal, &cntxt->tmp, localSize);
		}
		//nmprofiler_disable();
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

		switch (mode)
		{
		case NMGL_TRIANGLES:
		case NMGL_TRIANGLE_FAN:
		case NMGL_TRIANGLE_STRIP:
		{
			TrianglePointers trianPointers;

			//в tmp0 хранятся данные в порядке x0,y0,z0,w0,x1,y1,z1,w1,x2,y2,z2,w2 (остальные массивы неопределены)
			//в tmp1 хранятся данные в порядке x0,y0,z0,x1,y1,z1,x2,y2,z2,w0,w1,w2 (в buffer2), s0,t0,s1,t1,s2,t2 (в buffer5)
			trianPointers.v0.x = cntxt->buffer0 + 0 * NMGL_SIZE;
			trianPointers.v0.y = cntxt->buffer0 + 1 * NMGL_SIZE;
			trianPointers.v0.z = cntxt->buffer0 + 2 * NMGL_SIZE;
			trianPointers.v1.x = cntxt->buffer0 + 3 * NMGL_SIZE;
			trianPointers.v1.y = cntxt->buffer0 + 4 * NMGL_SIZE;
			trianPointers.v1.z = cntxt->buffer0 + 5 * NMGL_SIZE;
			trianPointers.v2.x = cntxt->buffer0 + 6 * NMGL_SIZE;
			trianPointers.v2.y = cntxt->buffer0 + 7 * NMGL_SIZE;
			trianPointers.v2.z = cntxt->buffer0 + 8 * NMGL_SIZE;
			trianPointers.v0.w = cntxt->buffer0 + 9 * NMGL_SIZE;
			trianPointers.v1.w = cntxt->buffer0 + 10 * NMGL_SIZE;
			trianPointers.v2.w = cntxt->buffer0 + 11 * NMGL_SIZE;
			trianPointers.v0.color = (v4nm32f*)(cntxt->buffer1 + 0 * NMGL_SIZE);
			trianPointers.v1.color = (v4nm32f*)(cntxt->buffer1 + 4 * NMGL_SIZE);
			trianPointers.v2.color = (v4nm32f*)(cntxt->buffer1 + 8 * NMGL_SIZE);

#ifdef TEXTURE_ENABLED
			trianPointers.v0.s = cntxt->buffer5 + 0 * NMGL_SIZE;
			trianPointers.v0.t = cntxt->buffer5 + 1 * NMGL_SIZE;
			trianPointers.v1.s = cntxt->buffer5 + 2 * NMGL_SIZE;
			trianPointers.v1.t = cntxt->buffer5 + 3 * NMGL_SIZE;
			trianPointers.v2.s = cntxt->buffer5 + 4 * NMGL_SIZE;
			trianPointers.v2.t = cntxt->buffer5 + 5 * NMGL_SIZE;
#endif //TEXTURE_ENABLED

#ifdef TEXTURE_ENABLED
			//FIX: Somehow localSize is changed after vertexPrimitiveRepack on mc12101
			//volatile prevent localSizeBackup to be optimized as localSize
			volatile int localSizeBackup = localSize;
#endif //TEXTURE_ENABLED

			int primCount = 0;
			switch (mode) {
			case NMGL_TRIANGLES:
				primCount = repackToPrimitives_t(vertexResult, colorOrNormal, texResult, &trianPointers, localSize);
				break;
			case NMGL_TRIANGLE_FAN:
				primCount = repackToPrimitives_tf(vertexResult, colorOrNormal, texResult, &trianPointers, localSize);
				break;
			case NMGL_TRIANGLE_STRIP:
				primCount = repackToPrimitives_ts(vertexResult, colorOrNormal, texResult, &trianPointers, localSize);
				break;
			}
			//------------clipping-------------------

			//------------perspective-division-and-viewport----------------
			perpectiveDivView(trianPointers.v0, cntxt->windowInfo, cntxt->buffer3, primCount);
			perpectiveDivView(trianPointers.v1, cntxt->windowInfo, cntxt->buffer3, primCount);
			perpectiveDivView(trianPointers.v2, cntxt->windowInfo, cntxt->buffer3, primCount);

			if (cntxt->isCullFace) {
				primCount = cullFaceSortTriangles(trianPointers, primCount);
				if (primCount == 0) {
					break;
				}
			}

#ifdef TRIANGULATION_ENABLED
			TrianglePointers tmp;
			//переинициализация tmp0
			tmp.v0.x = cntxt->buffer2 +  0 * NMGL_SIZE;
			tmp.v0.y = cntxt->buffer2 +  1 * NMGL_SIZE;
			tmp.v0.z = cntxt->buffer2 +  2 * NMGL_SIZE;
			tmp.v0.w = cntxt->buffer2 +  3 * NMGL_SIZE;
			tmp.v1.x = cntxt->buffer2 +  4 * NMGL_SIZE;
			tmp.v1.y = cntxt->buffer2 +  5 * NMGL_SIZE;
			tmp.v1.z = cntxt->buffer2 +  6 * NMGL_SIZE;
			tmp.v1.w = cntxt->buffer2 +  7 * NMGL_SIZE;
			tmp.v2.x = cntxt->buffer2 +  8 * NMGL_SIZE;
			tmp.v2.y = cntxt->buffer2 +  9 * NMGL_SIZE;
			tmp.v2.z = cntxt->buffer2 + 10 * NMGL_SIZE;
			tmp.v2.w = cntxt->buffer2 + 11 * NMGL_SIZE;

			tmp.v0.color = (v4nm32f*)(cntxt->buffer3 + 0 * NMGL_SIZE);
			tmp.v1.color = (v4nm32f*)(cntxt->buffer3 + 4 * NMGL_SIZE);
			tmp.v2.color = (v4nm32f*)(cntxt->buffer3 + 8 * NMGL_SIZE);

#ifdef TEXTURE_ENABLED
			tmp.v0.s = cntxt->buffer5 + 6 * NMGL_SIZE;
			tmp.v0.t = cntxt->buffer5 + 7 * NMGL_SIZE;
			tmp.v1.s = cntxt->buffer5 + 8 * NMGL_SIZE;
			tmp.v1.t = cntxt->buffer5 + 9 * NMGL_SIZE;
			tmp.v2.s = cntxt->buffer5 + 10 * NMGL_SIZE;
			tmp.v2.t = cntxt->buffer5 + 11 * NMGL_SIZE;
#endif //TEXTURE_ENABLED


			int srcThreated = 0;
			while (srcThreated < primCount) {
				static int counter = 0;
				PROFILER_SIZE(primCount);
				int currentCount = splitTriangles(&trianPointers, primCount, WIDTH_PTRN, HEIGHT_PTRN, NMGL_SIZE, &tmp, &srcThreated);
				pushPointers(trianPointers, primCount);
				if (currentCount % 2) {
					tmp.v0.x[currentCount] = tmp.v0.x[currentCount - 1];
					tmp.v0.y[currentCount] = tmp.v0.y[currentCount - 1];
					tmp.v0.z[currentCount] = tmp.v0.z[currentCount - 1];
					tmp.v0.w[currentCount] = tmp.v0.w[currentCount - 1];
					tmp.v0.color[currentCount] = tmp.v0.color[currentCount - 1];
					tmp.v1.x[currentCount] = tmp.v1.x[currentCount - 1];
					tmp.v1.y[currentCount] = tmp.v1.y[currentCount - 1];
					tmp.v1.z[currentCount] = tmp.v1.z[currentCount - 1];
					tmp.v1.w[currentCount] = tmp.v1.w[currentCount - 1];
					tmp.v1.color[currentCount] = tmp.v1.color[currentCount - 1];
					tmp.v2.x[currentCount] = tmp.v2.x[currentCount - 1];
					tmp.v2.y[currentCount] = tmp.v2.y[currentCount - 1];
					tmp.v2.z[currentCount] = tmp.v2.z[currentCount - 1];
					tmp.v2.w[currentCount] = tmp.v2.w[currentCount - 1];
					tmp.v2.color[currentCount] = tmp.v2.color[currentCount - 1];
					currentCount++;
				}

				pushToTriangles(tmp, cntxt->trianInner, currentCount);
				
				findMinMax3(cntxt->trianInner.x0, cntxt->trianInner.x1, cntxt->trianInner.x2,
					cntxt->buffer0, cntxt->buffer1,
					cntxt->trianInner.size);
				findMinMax3(cntxt->trianInner.y0, cntxt->trianInner.y1, cntxt->trianInner.y2,
					cntxt->buffer2, cntxt->buffer3,
					cntxt->trianInner.size);

				v2nm32f *minXY = (v2nm32f*)cntxt->buffer4;
				v2nm32f *maxXY = (v2nm32f*)cntxt->buffer4 + 3 * NMGL_SIZE;
				nmppsMerge_32f(cntxt->buffer0, cntxt->buffer2, (float*)minXY, cntxt->trianInner.size);
				nmppsMerge_32f(cntxt->buffer1, cntxt->buffer3, (float*)maxXY, cntxt->trianInner.size);
				setSegmentMask(minXY, maxXY, cntxt->segmentMasks, cntxt->trianInner.size);
				rasterizeT(&cntxt->trianInner, cntxt->segmentMasks);
				popPointers(trianPointers, primCount);
			}
#else
			pushToTriangles(trianPointers, cntxt->trianInner, primCount);

			findMinMax3(cntxt->trianInner.x0, cntxt->trianInner.x1, cntxt->trianInner.x2,
				cntxt->buffer0, cntxt->buffer1,
				cntxt->trianInner.size);
			findMinMax3(cntxt->trianInner.y0, cntxt->trianInner.y1, cntxt->trianInner.y2,
				cntxt->buffer2, cntxt->buffer3,
				cntxt->trianInner.size);

			v2nm32f *minXY = (v2nm32f*)cntxt->buffer4;
			v2nm32f *maxXY = (v2nm32f*)cntxt->buffer4 + 3 * NMGL_SIZE;
			nmppsMerge_32f(cntxt->buffer0, cntxt->buffer2, (float*)minXY, cntxt->trianInner.size);
			nmppsMerge_32f(cntxt->buffer1, cntxt->buffer3, (float*)maxXY, cntxt->trianInner.size);
			setSegmentMask(minXY, maxXY, cntxt->segmentMasks, cntxt->trianInner.size);
			rasterizeT(&cntxt->trianInner, cntxt->segmentMasks);
#endif //TRIANGULATION_ENABLED
			break;
		}
		case NMGL_LINE_STRIP:
		case NMGL_LINE_LOOP:
		case NMGL_LINES: {
			int primCount = 0;
			LinePointers linePointers;

			//в tmp0 хранятся данные в порядке x0,y0,z0,w0,x1,y1,z1,w1,x2,y2,z2,w2 (остальные массивы неопределены)
			//в tmp1 хранятся данные в порядке x0,y0,z0,x1,y1,z1,x2,y2,z2,w0,w1,w2 (в buffer2), s0,t0,s1,t1,s2,t2 (в buffer5)
			linePointers.v0.x = cntxt->buffer0 + 0 * NMGL_SIZE;
			linePointers.v0.y = cntxt->buffer0 + 1 * NMGL_SIZE;
			linePointers.v0.z = cntxt->buffer0 + 2 * NMGL_SIZE;
			linePointers.v1.x = cntxt->buffer0 + 3 * NMGL_SIZE;
			linePointers.v1.y = cntxt->buffer0 + 4 * NMGL_SIZE;
			linePointers.v1.z = cntxt->buffer0 + 5 * NMGL_SIZE;
			linePointers.v0.w = cntxt->buffer1 + 0 * NMGL_SIZE;
			linePointers.v1.w = cntxt->buffer1 + 1 * NMGL_SIZE;
			linePointers.v0.color = (v4nm32f*)(cntxt->buffer2 + 0 * NMGL_SIZE);
			linePointers.v1.color = (v4nm32f*)(cntxt->buffer2 + 4 * NMGL_SIZE);

#ifdef TEXTURE_ENABLED
			linePointers.v0.s = cntxt->buffer5 + 0 * NMGL_SIZE;
			linePointers.v0.t = cntxt->buffer5 + 1 * NMGL_SIZE;
			linePointers.v1.s = cntxt->buffer5 + 2 * NMGL_SIZE;
			linePointers.v1.t = cntxt->buffer5 + 3 * NMGL_SIZE;
#endif //TEXTURE_ENABLED
			switch (mode) {
			case NMGL_LINES:
				primCount = repackToPrimitives_l(vertexResult, colorOrNormal, texResult, &linePointers, localSize);
				break;
			case NMGL_LINE_LOOP:
				primCount = repackToPrimitives_ll(vertexResult, colorOrNormal, texResult, &linePointers, localSize);
				break;
			case NMGL_LINE_STRIP:
				primCount = repackToPrimitives_ls(vertexResult, colorOrNormal, texResult, &linePointers, localSize);
				break;
			}
			
			

			//------------clipping-------------------

			//------------perspective-division-and-viewport----------------
			perpectiveDivView(linePointers.v0, cntxt->windowInfo, cntxt->buffer3, primCount);
			perpectiveDivView(linePointers.v1, cntxt->windowInfo, cntxt->buffer3, primCount);

			pushToLines(linePointers, cntxt->lineInner, primCount);
			
			findMinMax2(cntxt->lineInner.x0, cntxt->lineInner.x1,
				cntxt->buffer0, cntxt->buffer1,
				cntxt->lineInner.size);
			findMinMax2(cntxt->lineInner.y0, cntxt->lineInner.y1,
				cntxt->buffer2, cntxt->buffer3,
				cntxt->lineInner.size);

			v2nm32f *minXY = (v2nm32f*)cntxt->buffer4;
			v2nm32f *maxXY = (v2nm32f*)cntxt->buffer4 + 3 * NMGL_SIZE;

			nmppsMerge_32f(cntxt->buffer0, cntxt->buffer2, (float*)minXY, cntxt->lineInner.size);
			nmppsMerge_32f(cntxt->buffer1, cntxt->buffer3, (float*)maxXY, cntxt->lineInner.size);
			setSegmentMask(minXY, maxXY, cntxt->segmentMasks, cntxt->lineInner.size);
			//PROFILER_SIZE(cntxt->lineInner.size);
			rasterizeL(&cntxt->lineInner, cntxt->segmentMasks);
			break;
		}
		case NMGL_POINTS: {
			v2nm32f *minXY = (v2nm32f*)cntxt->buffer4;
			v2nm32f *maxXY = (v2nm32f*)cntxt->buffer4 + 3 * NMGL_SIZE;

			split_v4nm32f(vertexResult, 1, cntxt->buffer0, cntxt->buffer1, cntxt->buffer2, cntxt->buffer3, localSize);

			nmppsDiv_32f(cntxt->buffer0, cntxt->buffer3, cntxt->buffer1 + NMGL_SIZE, localSize);
			nmppsDiv_32f(cntxt->buffer1, cntxt->buffer3, cntxt->buffer2 + NMGL_SIZE, localSize);
			nmppsDiv_32f(cntxt->buffer2, cntxt->buffer3, cntxt->buffer0 + NMGL_SIZE, localSize);

			nmppsMulC_AddC_32f(cntxt->buffer1 + NMGL_SIZE, cntxt->windowInfo.viewportMulX, cntxt->windowInfo.viewportAddX, cntxt->pointInner.x0, localSize);		//X
			nmppsMulC_AddC_32f(cntxt->buffer2 + NMGL_SIZE, cntxt->windowInfo.viewportMulY, cntxt->windowInfo.viewportAddY, cntxt->pointInner.y0, localSize);		//Y
			nmppsMulC_AddC_32f(cntxt->buffer0 + NMGL_SIZE, cntxt->windowInfo.viewportMulZ, cntxt->windowInfo.viewportAddZ, cntxt->buffer0, localSize);	//Z

			nmppsConvert_32f32s_rounding(cntxt->buffer0, cntxt->pointInner.z, 0, localSize);
			nmppsConvert_32f32s_rounding((float*)colorOrNormal, (int*)cntxt->pointInner.colors, 0, 4 * localSize);

			nmppsSubC_32f(cntxt->pointInner.x0, cntxt->buffer0, cntxt->pointRadius, localSize);
			nmppsSubC_32f(cntxt->pointInner.y0, cntxt->buffer1, cntxt->pointRadius, localSize);
			nmppsMerge_32f(cntxt->buffer0, cntxt->buffer1, (float*)minXY, localSize);
			nmppsAddC_32f(cntxt->pointInner.x0, cntxt->buffer0, cntxt->pointRadius, localSize);
			nmppsAddC_32f(cntxt->pointInner.y0, cntxt->buffer1, cntxt->pointRadius, localSize);
			nmppsMerge_32f(cntxt->buffer0, cntxt->buffer1, (float*)maxXY, localSize);

			cntxt->pointInner.size = localSize;
			setSegmentMask(minXY, maxXY, cntxt->segmentMasks, localSize);
			rasterizeP(&cntxt->pointInner, cntxt->segmentMasks);
			break;
		}
		}
		
	}

#ifdef DEBUG
	//printf("DrawArrays end\n\n");
#endif // DEBUG
}
