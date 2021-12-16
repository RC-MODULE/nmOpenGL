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


SECTION(".data_imu6")	ArrayManager<float> vertexAM;
SECTION(".data_imu6")	ArrayManager<float> normalAM;
SECTION(".data_imu6")	ArrayManager<v4nm32f> colorAM;
//TEXTURING_PART
SECTION(".data_imu6")	ArrayManager<float> texcoordAM;
//TEXTURING_PART

SECTION(".data_imu6")	BitDividedMask clipMask[10];

SECTION(".text_nmgl") void perpectiveDivView(CombinePointers &vertex, NMGL_Viewport &windowInfo, float* tmpBuf, int size) {
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


SECTION(".text_demo3d") void clipSelect(TrianglePointers *src, float* srcColor, int* indices, TrianglePointers * dst, float* dstColor, int size) {
	copyArraysByIndices((void**)&src->v0, indices, (void**)&dst->v0, 4, size);
	copyArraysByIndices((void**)&src->v1, indices, (void**)&dst->v1, 4, size);
	copyArraysByIndices((void**)&src->v2, indices, (void**)&dst->v2, 4, size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 12; j++) {
			dstColor[12 * i + j] = srcColor[12 * indices[i] + j];
		}
	}
}

void printMatrix(mat4nm32f* matrix);


SECTION(".text_nmgl")
void nmglDrawArrays(NMGLenum mode, NMGLint first, NMGLsizei count) {

	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if (context->vertexArray.enabled == NMGL_FALSE) {
		return;
	}
	if (count < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	float* srcDDR_vertex = (float*)context->vertexArray.pointer + context->vertexArray.size * first;
	float* srcDDR_normal = (float*)context->normalArray.pointer + context->normalArray.size * first;
	v4nm32f* srcDDR_color = (v4nm32f*)context->colorArray.pointer + first;
	context->vertexArray.offset = first;
	context->normalArray.offset = first;
	context->colorArray.offset = first;

//TEXTURING_PART
	unsigned int clientActiveTexUnitIndex;
	float* srcDDR_texcoords;
	if (cntxt->texState.textureEnabled) {
		clientActiveTexUnitIndex = cntxt->texState.clientActiveTexUnitIndex;
		srcDDR_texcoords = (float*)cntxt->texState.texcoordArray[clientActiveTexUnitIndex].pointer + cntxt->texState.texcoordArray[clientActiveTexUnitIndex].size * first;
		cntxt->texState.texcoordArray[clientActiveTexUnitIndex].offset = first;
	}
//TEXTURING_PART

	int maxInnerCount;
	switch (mode) {
	case NMGL_TRIANGLES:
		maxInnerCount = 3 * NMGL_SIZE;
		break;
	case NMGL_TRIANGLE_FAN:
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
	case NMGL_POINTS:
		maxInnerCount = NMGL_SIZE;
		break;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	if(first % 2 == 0 && count % 2 == 0){
		vertexAM.set(srcDDR_vertex, context->vertexArray.size * count, context->vertexArray.size * maxInnerCount, copyVec<float>);
		normalAM.set(srcDDR_normal, context->normalArray.size * count, context->normalArray.size * maxInnerCount, copyVec<float>);
		colorAM.set(srcDDR_color, count, maxInnerCount, copyVec<v4nm32f>);
//TEXTURING_PART
		if (cntxt->texState.textureEnabled) {
			texcoordAM.set(srcDDR_texcoords, cntxt->texState.texcoordArray[clientActiveTexUnitIndex].size * count, 
				           cntxt->texState.texcoordArray[clientActiveTexUnitIndex].size * maxInnerCount, copyVec<float>);
		}
//TEXTURING_PART
	}else{
		vertexAM.set(srcDDR_vertex, context->vertexArray.size * count, context->vertexArray.size * maxInnerCount, copyRisc<float>);
		normalAM.set(srcDDR_normal, context->normalArray.size * count, context->normalArray.size * maxInnerCount, copyRisc<float>);
		colorAM.set(srcDDR_color, count, maxInnerCount, copyRisc<v4nm32f>);
//TEXTURING_PART
		if (cntxt->texState.textureEnabled) {
			texcoordAM.set(srcDDR_texcoords, cntxt->texState.texcoordArray[clientActiveTexUnitIndex].size * count, 
				           cntxt->texState.texcoordArray[clientActiveTexUnitIndex].size * maxInnerCount, copyRisc<float>);
		}
//TEXTURING_PART
	}

	if (context->lightingInfo.isLighting) {
		context->lightingInfo.update();
	}

	for (int pointer = 0; pointer < count; pointer += maxInnerCount) {
		int localSize = MIN(count - pointer, maxInnerCount);
		vertexAM.pop(cntxt->buffer0);
		switch (context->vertexArray.size)
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
		mul_mat4nm32f_v4nm32f(context->modelviewMatrixStack.top(), (v4nm32f *)cntxt->buffer1, cntxt->vertexResult, localSize);
		//texcoords
//TEXTURING_PART
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
				nmblas_scopy(2 * localSize, cntxt->buffer1, 1, (float*)cntxt->texResult, 1);
			}
		}
//TEXTURING_PART
		//normal
		if (context->normalArray.enabled) {
			if (context->normalArray.size == 3) {
				normalAM.pop(cntxt->buffer3);
				cnv32f_v3v4(cntxt->buffer3, cntxt->buffer1, 0, localSize);
			}
			else {
				normalAM.pop(cntxt->buffer1);
			}
			mul_v4nm32f_mat4nm32f((v4nm32f *)cntxt->buffer1, &context->normalMatrix, cntxt->colorOrNormal, localSize);
			if (context->normalizeEnabled) {
				nmblas_scopy(4 * localSize, (float*)cntxt->colorOrNormal, 1, cntxt->buffer2, 1);
				dotV_v4nm32f(cntxt->colorOrNormal, (v4nm32f*)cntxt->buffer2, (v2nm32f*)cntxt->buffer0, localSize);
				fastInvSqrt(cntxt->buffer0, cntxt->buffer1, 2 * localSize);
				dotMulV_v4nm32f((v2nm32f*)cntxt->buffer1, (v4nm32f*)cntxt->buffer2, cntxt->colorOrNormal, localSize);
			}
		}

		//vertex in vertexResult
		//normal in colorOrNormal
		//Освещение или наложение цветов
		//nmprofiler_enable();
		if (context->lightingInfo.isLighting) {
			PROFILER_SIZE(localSize);
			light(cntxt->vertexResult, cntxt->colorOrNormal, localSize);
		}
		else {
			cntxt->tmp.vec[0] = 1;
			cntxt->tmp.vec[1] = 1;
			cntxt->tmp.vec[2] = 1;
			cntxt->tmp.vec[3] = 1;
			set_v4nm32f(cntxt->colorOrNormal, &cntxt->tmp, localSize);
		}
		//nmprofiler_disable();
		//color
		if (context->colorArray.enabled) {
			colorAM.pop(cntxt->colorOrNormal);
			if (context->colorArray.type == NMGL_UNSIGNED_BYTE) {
				nmppsConvert_32s32f((int*)cntxt->colorOrNormal, cntxt->buffer0, context->colorArray.size * localSize);
				nmppsMulC_32f(cntxt->buffer0, (float*)cntxt->colorOrNormal, 1.0 / 255.0, context->colorArray.size * localSize);
			}
		}
		clamp_32f((float*)cntxt->colorOrNormal, 0, 1, (float*)cntxt->buffer3, 4 * localSize);
		cntxt->tmp.vec[0] = RED_COEFF;
		cntxt->tmp.vec[1] = GREEN_COEFF;
		cntxt->tmp.vec[2] = BLUE_COEFF;
		cntxt->tmp.vec[3] = ALPHA_COEFF;

		mulC_v4nm32f((v4nm32f*)cntxt->buffer3, &cntxt->tmp, cntxt->colorOrNormal, localSize);

		//умножение на матрицу проекции (Projection matrix)
		mul_mat4nm32f_v4nm32f(context->projectionMatrixStack.top(), cntxt->vertexResult, (v4nm32f*)cntxt->vertexResult, localSize);

		switch (mode)
		{
		case NMGL_TRIANGLES:
		case NMGL_TRIANGLE_FAN:
		case NMGL_TRIANGLE_STRIP:
		{
			TrianglePointers trianPointers;
			TrianglePointers trianPointers2;

			//triantrianPointers = vertexResult2, colorOrNormal2, texResult2
			//triantrianPointers2 = vertexResult, colorOrNormal, texResult
			{
				
				trianPointers.v0.x = (float*)cntxt->vertexResult2 + 0 * NMGL_SIZE;
				trianPointers.v0.y = (float*)cntxt->vertexResult2 + 1 * NMGL_SIZE;
				trianPointers.v0.z = (float*)cntxt->vertexResult2 + 2 * NMGL_SIZE;
				trianPointers.v1.x = (float*)cntxt->vertexResult2 + 3 * NMGL_SIZE;
				trianPointers.v1.y = (float*)cntxt->vertexResult2 + 4 * NMGL_SIZE;
				trianPointers.v1.z = (float*)cntxt->vertexResult2 + 5 * NMGL_SIZE;
				trianPointers.v2.x = (float*)cntxt->vertexResult2 + 6 * NMGL_SIZE;
				trianPointers.v2.y = (float*)cntxt->vertexResult2 + 7 * NMGL_SIZE;
				trianPointers.v2.z = (float*)cntxt->vertexResult2 + 8 * NMGL_SIZE;
				trianPointers.v0.w = (float*)cntxt->vertexResult2 + 9 * NMGL_SIZE;
				trianPointers.v1.w = (float*)cntxt->vertexResult2 + 10 * NMGL_SIZE;
				trianPointers.v2.w = (float*)cntxt->vertexResult2 + 11 * NMGL_SIZE;

				trianPointers.v0.color = (cntxt->colorOrNormal2 + 0 * NMGL_SIZE);
				trianPointers.v1.color = (cntxt->colorOrNormal2 + 1 * NMGL_SIZE);
				trianPointers.v2.color = (cntxt->colorOrNormal2 + 2 * NMGL_SIZE);

				trianPointers.v0.s = (float*)cntxt->texResult2 + 0 * NMGL_SIZE;
				trianPointers.v0.t = (float*)cntxt->texResult2 + 1 * NMGL_SIZE;
				trianPointers.v1.s = (float*)cntxt->texResult2 + 2 * NMGL_SIZE;
				trianPointers.v1.t = (float*)cntxt->texResult2 + 3 * NMGL_SIZE;
				trianPointers.v2.s = (float*)cntxt->texResult2 + 4 * NMGL_SIZE;
				trianPointers.v2.t = (float*)cntxt->texResult2 + 5 * NMGL_SIZE;
			}
			{
				trianPointers2.v0.x = (float*)cntxt->vertexResult + 0 * NMGL_SIZE;
				trianPointers2.v0.y = (float*)cntxt->vertexResult + 1 * NMGL_SIZE;
				trianPointers2.v0.z = (float*)cntxt->vertexResult + 2 * NMGL_SIZE;
				trianPointers2.v1.x = (float*)cntxt->vertexResult + 3 * NMGL_SIZE;
				trianPointers2.v1.y = (float*)cntxt->vertexResult + 4 * NMGL_SIZE;
				trianPointers2.v1.z = (float*)cntxt->vertexResult + 5 * NMGL_SIZE;
				trianPointers2.v2.x = (float*)cntxt->vertexResult + 6 * NMGL_SIZE;
				trianPointers2.v2.y = (float*)cntxt->vertexResult + 7 * NMGL_SIZE;
				trianPointers2.v2.z = (float*)cntxt->vertexResult + 8 * NMGL_SIZE;
				trianPointers2.v0.w = (float*)cntxt->vertexResult + 9 * NMGL_SIZE;
				trianPointers2.v1.w = (float*)cntxt->vertexResult + 10 * NMGL_SIZE;
				trianPointers2.v2.w = (float*)cntxt->vertexResult + 11 * NMGL_SIZE;

				trianPointers2.v0.color = (cntxt->colorOrNormal + 0 * NMGL_SIZE);
				trianPointers2.v1.color = (cntxt->colorOrNormal + 1 * NMGL_SIZE);
				trianPointers2.v2.color = (cntxt->colorOrNormal + 2 * NMGL_SIZE);

				trianPointers2.v0.s = (float*)cntxt->texResult + 0 * NMGL_SIZE;
				trianPointers2.v0.t = (float*)cntxt->texResult + 1 * NMGL_SIZE;
				trianPointers2.v1.s = (float*)cntxt->texResult + 2 * NMGL_SIZE;
				trianPointers2.v1.t = (float*)cntxt->texResult + 3 * NMGL_SIZE;
				trianPointers2.v2.s = (float*)cntxt->texResult + 4 * NMGL_SIZE;
				trianPointers2.v2.t = (float*)cntxt->texResult + 5 * NMGL_SIZE;
			}

			int primCount = 0;
			switch (mode) {
			case NMGL_TRIANGLES:
				primCount = repackToPrimitives_t((v4nm32f*)cntxt->vertexResult, (v4nm32f*)cntxt->colorOrNormal, (v2nm32f*)cntxt->texResult, &trianPointers, localSize);
				break;
			case NMGL_TRIANGLE_FAN:
				primCount = repackToPrimitives_tf((v4nm32f*)cntxt->vertexResult, (v4nm32f*)cntxt->colorOrNormal, (v2nm32f*)cntxt->texResult, &trianPointers, localSize);
				break;
			case NMGL_TRIANGLE_STRIP:
				primCount = repackToPrimitives_ts((v4nm32f*)cntxt->vertexResult, (v4nm32f*)cntxt->colorOrNormal, (v2nm32f*)cntxt->texResult, &trianPointers, localSize);
				break;
			}
			//------------clipping-------------------

			//------------perspective-division-and-viewport----------------
			perpectiveDivView(trianPointers.v0, context->viewport, cntxt->buffer3, primCount);
			perpectiveDivView(trianPointers.v1, context->viewport, cntxt->buffer3, primCount);
			perpectiveDivView(trianPointers.v2, context->viewport, cntxt->buffer3, primCount);

			if (context->isCullFace) {
				primCount = cullFaceSortTriangles(trianPointers, primCount);
				if (primCount == 0) {
					break;
				}
			}

#ifdef TRIANGULATION_ENABLED

			int srcThreated = 0;
			while (srcThreated < primCount) {
				static int counter = 0; 
				PROFILER_SIZE(primCount);
				int currentCount = splitTriangles(&trianPointers, primCount, WIDTH_PTRN, HEIGHT_PTRN, NMGL_SIZE, &trianPointers2, &srcThreated);
				if (currentCount % 2) {
					trianPointers2.v0.x[currentCount] = trianPointers2.v0.x[currentCount - 1];
					trianPointers2.v0.y[currentCount] = trianPointers2.v0.y[currentCount - 1];
					trianPointers2.v0.z[currentCount] = trianPointers2.v0.z[currentCount - 1];
					trianPointers2.v0.w[currentCount] = trianPointers2.v0.w[currentCount - 1];
					trianPointers2.v0.color[currentCount] = trianPointers2.v0.color[currentCount - 1];
					trianPointers2.v1.x[currentCount] = trianPointers2.v1.x[currentCount - 1];
					trianPointers2.v1.y[currentCount] = trianPointers2.v1.y[currentCount - 1];
					trianPointers2.v1.z[currentCount] = trianPointers2.v1.z[currentCount - 1];
					trianPointers2.v1.w[currentCount] = trianPointers2.v1.w[currentCount - 1];
					trianPointers2.v1.color[currentCount] = trianPointers2.v1.color[currentCount - 1];
					trianPointers2.v2.x[currentCount] = trianPointers2.v2.x[currentCount - 1];
					trianPointers2.v2.y[currentCount] = trianPointers2.v2.y[currentCount - 1];
					trianPointers2.v2.z[currentCount] = trianPointers2.v2.z[currentCount - 1];
					trianPointers2.v2.w[currentCount] = trianPointers2.v2.w[currentCount - 1];
					trianPointers2.v2.color[currentCount] = trianPointers2.v2.color[currentCount - 1];
					currentCount++;
				}

				pushToTriangles(trianPointers2, cntxt->trianInner, currentCount);
				
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
				PROFILER_SIZE(cntxt->trianInner.size);
				setSegmentMask(minXY, maxXY, cntxt->segmentMasks, cntxt->trianInner.size);
				PROFILER_SIZE(cntxt->trianInner.size);
				rasterizeT(&cntxt->trianInner, cntxt->segmentMasks);

				//counter++;
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
			
			PROFILER_SIZE(cntxt->trianInner.size);
			setSegmentMask(minXY, maxXY, cntxt->segmentMasks, cntxt->trianInner.size);
			PROFILER_SIZE(cntxt->trianInner.size);
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

//TEXTURING_PART
			linePointers.v0.s = cntxt->buffer5 + 0 * NMGL_SIZE;
			linePointers.v0.t = cntxt->buffer5 + 1 * NMGL_SIZE;
			linePointers.v1.s = cntxt->buffer5 + 2 * NMGL_SIZE;
			linePointers.v1.t = cntxt->buffer5 + 3 * NMGL_SIZE;
//TEXTURING_PART
			switch (mode) {
			case NMGL_LINES:
				primCount = repackToPrimitives_l(cntxt->vertexResult, cntxt->colorOrNormal, cntxt->texResult, &linePointers, localSize);
				break;
			case NMGL_LINE_LOOP:
				primCount = repackToPrimitives_ll(cntxt->vertexResult, cntxt->colorOrNormal, cntxt->texResult, &linePointers, localSize);
				break;
			case NMGL_LINE_STRIP:
				primCount = repackToPrimitives_ls(cntxt->vertexResult, cntxt->colorOrNormal, cntxt->texResult, &linePointers, localSize);
				break;
			}
			
			

			//------------clipping-------------------

			//------------perspective-division-and-viewport----------------
			perpectiveDivView(linePointers.v0, context->viewport, cntxt->buffer3, primCount);
			perpectiveDivView(linePointers.v1, context->viewport, cntxt->buffer3, primCount);

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
			PROFILER_SIZE(cntxt->lineInner.size);
			setSegmentMask(minXY, maxXY, cntxt->segmentMasks, cntxt->lineInner.size);
			PROFILER_SIZE(cntxt->lineInner.size);
			rasterizeL(&cntxt->lineInner, cntxt->segmentMasks);
			break;
		}
		case NMGL_POINTS: {
			v2nm32f *minXY = (v2nm32f*)cntxt->buffer4;
			v2nm32f *maxXY = (v2nm32f*)cntxt->buffer4 + 3 * NMGL_SIZE;
			CombinePointers pointers;
			pointers.x = cntxt->pointInner.x;
			pointers.y = cntxt->pointInner.y;
			pointers.z = cntxt->buffer0;
			pointers.w = cntxt->buffer0 + NMGL_SIZE;
			pointers.color = (v4nm32f*)(cntxt->buffer2);
			pointers.s = cntxt->buffer5 + 0 * NMGL_SIZE;
			pointers.t = cntxt->buffer5 + 1 * NMGL_SIZE;

			split_v4nm32f(cntxt->vertexResult, 1, pointers.x, pointers.y, pointers.z, pointers.w, localSize);
			perpectiveDivView(pointers, context->viewport, cntxt->buffer0, localSize);

			nmppsConvert_32f32s_rounding(cntxt->buffer0, cntxt->pointInner.z, 0, localSize);
			nmppsConvert_32f32s_rounding((float*)cntxt->colorOrNormal, (int*)cntxt->pointInner.colors, 0, 4 * localSize);

			nmppsSubC_32f(cntxt->pointInner.x, cntxt->buffer0, context->pointSize / 2, localSize);
			nmppsSubC_32f(cntxt->pointInner.y, cntxt->buffer1, context->pointSize / 2, localSize);
			nmppsMerge_32f(cntxt->buffer0, cntxt->buffer1, (float*)minXY, localSize);
			nmppsAddC_32f(cntxt->pointInner.x, cntxt->buffer0, context->pointSize / 2, localSize);
			nmppsAddC_32f(cntxt->pointInner.y, cntxt->buffer1, context->pointSize / 2, localSize);
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
