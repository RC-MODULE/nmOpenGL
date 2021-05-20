#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"

int totalSum(nm32s* pVec, int size);

#ifdef TEXTURE_ENABLED
using namespace tex_nm1; 
SECTION(".data_imu0") TrianglesInfo triangles;
#endif //TEXTURE_ENABLED

SECTION(".text_demo3d") void drawPrimitives(NMGL_Context_NM1 *context, int countPrimitives){
	nm32s* mulZ = (nm32s*)context->buffer0;
	nm32s* mulC = (nm32s*)context->buffer0;
	nm32s* zMaskBuffer = (nm32s*)context->buffer1;

	for(int point = 0; point < countPrimitives; point += SMALL_SIZE){
		int localSize = MIN(countPrimitives - point, SMALL_SIZE);
		//PROFILER_SIZE(localSize);
		//int fullSize = totalSum(context->nSizePtrn32 + point, localSize);

		//�������� ������������� ����� �������
		if (context->depthBuffer.enabled == NMGL_FALSE) {
			//PROFILER_SIZE(fullSize);
			mMulCVxN_2s32s(
				context->primitivePack_2s + point,
				context->ptrnInnPoints + point,
				context->ptrnSizes + point,
				context->minusOne,
				zMaskBuffer,
				localSize);
		}
		else {
			//��������� �������� ����� �� Z
			//PROFILER_SIZE(fullSize);
			mMulCVxN_2s32s(
				context->primitivePack_2s + point,
				context->ptrnInnPoints + point,
				context->ptrnSizes + point,
				context->valuesZ + point,
				mulZ,
				localSize);


			//mulZ ������ ������ z-������������

			//������� ����� �������
			//PROFILER_SIZE(fullSize);
			depthTest32((nm32s**)(context->zBuffPoints + point),
				WIDTH_SEG,
				mulZ,
				zMaskBuffer,
				context->ptrnSizes + point,
				localSize);
		}
		//color v4nm8s in imgOffset
		//PROFILER_SIZE(fullSize);
		mMulCVxN_2s_RGB8888(
			context->primitivePack_2s + point,
			context->ptrnInnPoints + point,
			context->ptrnSizes + point,
			context->valuesC + point,
			mulC,
			localSize);
		//mulBuffer ������ ������ ����

#ifdef TEXTURE_ENABLED
        if (context->texState.textureEnabled) {
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

			triangles.z0 = context->w0 + point;
			triangles.z1 = context->w1 + point;
			triangles.z2 = context->w2 + point; 

			textureTriangle(
				context->primitivePack_2s + point,
					&triangles,
					(nm32s**)(context->imagePoints + point),
                    context->ptrnInnPoints + point,
                    context->ptrnSizes + point,
                    mulC,
                    mulC,
                    localSize);
        }
#endif //TEXTURE_ENABLED

		//������� ����������� ����� �� ����� � ������� 
		//� �������� ����������� � �����������

		//PROFILER_SIZE(fullSize);
		mMaskVxN_32s(mulC,
			zMaskBuffer,
			(nm32s**)(context->imagePoints + point), WIDTH_SEG,
			context->ptrnSizes + point,
			localSize);

	}	
}