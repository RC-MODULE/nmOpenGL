#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglGetFloatv(NMGLenum pname, NMGLfloat * params) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	switch(pname){
	case NMGL_MATRIX_MODE:
		params[0] = cntxt->currentMatrixStack->type;
		break;
	case NMGL_LIGHT_MODEL_AMBIENT:
		params[0] = cntxt->lightingInfo.lightAmbient[MAX_LIGHTS].vec[0];
		params[1] = cntxt->lightingInfo.lightAmbient[MAX_LIGHTS].vec[1];
		params[2] = cntxt->lightingInfo.lightAmbient[MAX_LIGHTS].vec[2];
		params[3] = cntxt->lightingInfo.lightAmbient[MAX_LIGHTS].vec[3];
		break;
	case NMGL_MODELVIEW_MATRIX:
		for (int i = 0; i < 16; i++) {
			params[i] = cntxt->modelviewMatrixStack.top()->matr[i];
		}
		//nmblas_scopy(16, (float*)cntxt->modelviewMatrixStack.top(),1, params,1);
		break;
	case NMGL_PROJECTION_MATRIX:
		for (int i = 0; i < 16; i++) {
			params[i] = cntxt->projectionMatrixStack.top()->matr[i];
		}
		//nmblas_scopy(16, (float*)cntxt->projectionMatrixStack.top(), 1, params, 1);
		break;
	case NMGL_SHARED_TEXTURE_PALETTE_EXT:		
		break;
	case NMGL_CURRENT_COLOR:
		break;
	case NMGL_CURRENT_TEXTURE_COORDS:
		break;
	case NMGL_CURRENT_NORMAL:
		break;	
	case NMGL_CURRENT_RASTER_COLOR:
		break;	
	case NMGL_CURRENT_RASTER_TEXTURE_COORDS:
		break;	
	case NMGL_DEPTH_RANGE:
		break;
	case NMGL_POINT_SIZE:
		break;
	case NMGL_LINE_WIDTH:
		break;
	case NMGL_POLYGON_OFFSET_FACTOR:
		break;
	case NMGL_POLYGON_OFFSET_UNITS:
		break;
	case NMGL_ALPHA_TEST_REF:
		break;
	case NMGL_COLOR_CLEAR_VALUE:
		break;
	case NMGL_DEPTH_CLEAR_VALUE:
		break;
	case NMGL_ALIASED_POINT_SIZE_RANGE:
		break;
	case NMGL_SMOOTH_POINT_SIZE_RANGE:
		break;
	case NMGL_SMOOTH_POINT_SIZE_GRANULARITY:
		break;
	case NMGL_ALIASED_LINE_WIDTH_RANGE:
		break;
	case NMGL_SMOOTH_LINE_WIDTH_RANGE:
		break;
	case NMGL_SMOOTH_LINE_WIDTH_GRANULARITY:
	break;

	default:
		cntxt->error = NMGL_INVALID_ENUM;
	}
}
