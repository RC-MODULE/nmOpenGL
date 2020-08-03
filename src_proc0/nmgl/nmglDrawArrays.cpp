#include "demo3d_nm0.h"
#include "nmpp.h"
#include "nmblas.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "malloc.h"
#include "arraymanager.h"
#include "stdio.h"

SECTION(".data_imu5")	float vertexX[3 * NMGL_SIZE];
SECTION(".data_imu6")	float vertexY[3 * NMGL_SIZE];
SECTION(".data_imu4")	float vertexZ[3 * NMGL_SIZE];

#ifdef TEXTURE_ENABLED
SECTION(".data_imu6")	float vertexS[3 * NMGL_SIZE];
SECTION(".data_imu6")	float vertexT[3 * NMGL_SIZE];
SECTION(".data_imu6")	float vertexZEye[3 * NMGL_SIZE];
#endif //TEXTURE_ENABLED

SECTION(".data_imu5")	v4nm32f vertexResult[3 * NMGL_SIZE];
SECTION(".data_imu6")	v4nm32f colorOrNormal[3 * NMGL_SIZE];

#ifdef TEXTURE_ENABLED
SECTION(".data_imu6")	v4nm32f texCoord[3 * NMGL_SIZE];
#endif //TEXTURE_ENABLED

SECTION(".data_imu6")	ArrayManager<float> vertexAM;
SECTION(".data_imu6")	ArrayManager<float> normalAM;
SECTION(".data_imu6")	ArrayManager<v4nm32f> colorAM;

#ifdef TEXTURE_ENABLED
SECTION(".data_imu6")	ArrayManager<float> texcoordAM;
#endif //TEXTURE_ENABLED

template < typename T >
inline void copyVec(const void* src, void* dst, size_t size) {
	nmblas_scopy(size * sizeof32(T), (float*)src, 1, (float*)dst, 1);
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
#ifdef TEXTURE_ENABLED
	float* srcDDR_texcoords = (float*)cntxt.texState.texcoordArray[0].pointer + cntxt.texState.texcoordArray[0].size * first;
#endif //TEXTURE_ENABLED
	v4nm32f* srcDDR_color = (v4nm32f*)cntxt.colorArray.pointer + first;


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
		cntxt.error = NMGL_INVALID_ENUM;
		return;
	}

	vertexAM.set(srcDDR_vertex, cntxt.vertexArray.size * count, cntxt.vertexArray.size * maxInnerCount, copyVec<float>);
	normalAM.set(srcDDR_normal, cntxt.normalArray.size * count, cntxt.normalArray.size * maxInnerCount, copyVec<float>);
#ifdef TEXTURE_ENABLED
	if (cntxt.texState.textureEnabled){
		texcoordAM.set(srcDDR_texcoords, cntxt.texState.texcoordArray[0].size * count, cntxt.texState.texcoordArray[0].size * maxInnerCount, copyVec<float>);
	}
#endif //TEXTURE_ENABLED
	colorAM.set(srcDDR_color, count, maxInnerCount, copyVec<v4nm32f>);


	if (cntxt.isLighting) {
		mulC_v4nm32f(cntxt.lightAmbient, &cntxt.materialAmbient, cntxt.ambientMul, MAX_LIGHTS + 1);
		mulC_v4nm32f(cntxt.lightDiffuse, &cntxt.materialDiffuse, cntxt.diffuseMul, MAX_LIGHTS);
		mulC_v4nm32f(cntxt.lightSpecular, &cntxt.materialSpecular, cntxt.specularMul, MAX_LIGHTS);
		nmppsAdd_32f((float*)(cntxt.ambientMul + MAX_LIGHTS), 
			(float*)&cntxt.materialEmissive, 
			(float*)(cntxt.ambientMul + MAX_LIGHTS), 4);
	}

	reverseMatrix3x3in4x4(cntxt.modelviewMatrixStack.top(), &cntxt.normalMatrix);

	while (!vertexAM.isEmpty()) {
		//vertex
		int localSize = vertexAM.pop(cntxt.buffer0) / cntxt.vertexArray.size;
		switch (cntxt.vertexArray.size)
		{
		case 2:
			nmblas_dcopy(localSize, (double*)cntxt.buffer0, 1, (double*)cntxt.buffer1, 2);
			cntxt.tmp.vec[0] = 0;
			cntxt.tmp.vec[1] = 1;
			nmblas_dcopy(localSize, (double*)&cntxt.tmp, 0, (double*)(cntxt.buffer1 + 2), 2);
			break;
		case 3:
			cnv32f_v3v4(cntxt.buffer0, cntxt.buffer1, 1, localSize);
			break;
		case 4:
			nmblas_scopy(4 * localSize, cntxt.buffer0, 1, cntxt.buffer1, 1);
			break;
		default:
			break;
		}
		mul_mat4nm32f_v4nm32f(cntxt.modelviewMatrixStack.top(), (v4nm32f*)cntxt.buffer1, vertexResult, localSize);

#ifdef TEXTURE_ENABLED
	if (cntxt.texState.textureEnabled){
		//texture coordinates
		if (cntxt.texState.texcoordArray[0].enabled) {
			texcoordAM.pop(cntxt.buffer0);
			switch (cntxt.texState.texcoordArray[0].size) //Must be equal to 2
			{
				case 2:
					nmblas_dcopy(localSize, (double*)cntxt.buffer0, 1, (double*)cntxt.buffer1, 2);
					cntxt.tmp.vec[0] = 0;
					cntxt.tmp.vec[1] = 1;
					nmblas_dcopy(localSize, (double*)&cntxt.tmp, 0, (double*)(cntxt.buffer1 + 2), 2);
					break;
				case 3:
					cnv32f_v3v4(cntxt.buffer0, cntxt.buffer1, 1, localSize);
					break;
				case 4:
					nmblas_scopy(4 * localSize, cntxt.buffer0, 1, cntxt.buffer1, 1);
					break;
				default:
					break;
			}
			nmblas_scopy(4 * localSize, cntxt.buffer1, 1, (float*)texCoord, 1);
		}
	}
#endif //TEXTURE_ENABLED
		
		//color
		if (cntxt.colorArray.enabled) {
			colorAM.pop(colorOrNormal);
			if (cntxt.colorArray.type == NMGL_UNSIGNED_BYTE) {
				nmppsConvert_32s32f((int*)colorOrNormal, cntxt.buffer0, cntxt.colorArray.size * localSize);
				nmppsMulC_32f(cntxt.buffer0, (float*)colorOrNormal, 1.0/255.0, cntxt.colorArray.size * localSize);
			}
		}

		//normal
		if (cntxt.normalArray.enabled) {
			if (cntxt.normalArray.size == 3) {
				normalAM.pop(cntxt.buffer3);
				cnv32f_v3v4(cntxt.buffer3, cntxt.buffer1, 0, localSize);
			}
			else {
				normalAM.pop(cntxt.buffer1);
			}
			mul_v4nm32f_mat4nm32f((v4nm32f*)cntxt.buffer1, &cntxt.normalMatrix, colorOrNormal, localSize);
			if (cntxt.normalizeEnabled) {
				nmblas_scopy(4 * localSize, (float*)colorOrNormal, 1, cntxt.buffer2, 1);
				dotV_v4nm32f(colorOrNormal, (v4nm32f*)cntxt.buffer2, (v2nm32f*)cntxt.buffer0, localSize);
				fastInvSqrt(cntxt.buffer0, cntxt.buffer1, 2 * localSize);
				dotMulV_v4nm32f((v2nm32f*)cntxt.buffer1, (v4nm32f*)cntxt.buffer2, colorOrNormal, localSize);
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
		//texcoords in texCoord
		mul_mat4nm32f_v4nm32f(cntxt.projectionMatrixStack.top(), vertexResult, (v4nm32f*)vertexResult, localSize);
		//------------------------------srcX-----srcY-----srcZ-----srcW--------------
		split_v4nm32f(vertexResult, 1, cntxt.buffer0, cntxt.buffer1, cntxt.buffer2, cntxt.buffer3, localSize);

#ifdef TEXTURE_ENABLED
    	//----------------------------------- Save vertexZEye for texturing-------------------------------------
		if (cntxt.texState.textureEnabled){
        	int countPrim = localSize / 3;
       		float* temp0 = cntxt.buffer0 + 3 * NMGL_SIZE;
        	float* temp1 = cntxt.buffer1 + 6 * NMGL_SIZE;
        	float* temp2 = cntxt.buffer2 + 3 * NMGL_SIZE;
        	cnv32f_v3v4(cntxt.buffer2, cntxt.buffer3, 0, localSize);
        	split_v4nm32f((v4nm32f*)cntxt.buffer3, 1, temp0, temp1, temp2, cntxt.buffer3 + 6 * NMGL_SIZE, countPrim);
        	meanToFloat3(temp0, temp1, temp2, vertexZEye, countPrim);
        	//restore srcX-----srcY-----srcZ-----srcW in buffers
        	split_v4nm32f(vertexResult, 1, cntxt.buffer0, cntxt.buffer1, cntxt.buffer2, cntxt.buffer3, localSize);
        }
#endif //TEXTURE_ENABLED
        
		//------------clipping-------------------

		//------------perspective-division-----------------		static unsigned time = 0;
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

#ifdef TEXTURE_ENABLED
		if (cntxt.texState.textureEnabled){
			//------------------------------texCoordX-----texCoordY-----texCoordZ-----texCoordW--------------
			split_v4nm32f(texCoord, 1, cntxt.buffer0, cntxt.buffer1, cntxt.buffer2, cntxt.buffer3, localSize);
			nmblas_scopy(localSize, cntxt.buffer0, 1, vertexS, 1);
			nmblas_scopy(localSize, cntxt.buffer1, 1, vertexT, 1);
		}
#endif //TEXTURE_ENABLED

		//---------------rasterize------------------------------------
		switch (mode) {
		case NMGL_TRIANGLES:
#ifdef TEXTURE_ENABLED
			pushToTriangles_t(vertexX, vertexY, vertexZ, vertexS, vertexT, vertexZEye, colorOrNormal, cntxt.trianInner, localSize);
#else //TEXTURE_ENABLED
			pushToTriangles_t(vertexX, vertexY, vertexZ, colorOrNormal, cntxt.trianInner, localSize);
#endif //TEXTURE_ENABLED
			if (cntxt.isCullFace) {
				cullFaceSortTriangles(cntxt.trianInner);
			}
			setSegmentMask(cntxt, cntxt.trianInner, cntxt.segmentMasks);
			rasterizeT(&cntxt.trianInner, cntxt.segmentMasks);
			break;
		case NMGL_LINES:
			break;
		}
	}

}
