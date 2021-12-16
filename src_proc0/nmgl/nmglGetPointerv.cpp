#include "demo3d_nm0.h"
#include "nmgl.h"


void nmglGetPointerv (NMGLenum pname, NMGLvoid **params)
{
     NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
     NMGL_Context *context = NMGL_GetCurrentContext();
   
    switch(pname){
    case NMGL_VERTEX_ARRAY_POINTER:
        *params = (NMGLvoid *)context->vertexArray.pointer;
        break;
    case NMGL_NORMAL_ARRAY_POINTER:
        *params = (NMGLvoid *)context->normalArray.pointer;
        break;
    case NMGL_COLOR_ARRAY_POINTER:
        *params = (NMGLvoid *)context->colorArray.pointer;
        break;
    case NMGL_TEXTURE_COORD_ARRAY_POINTER:
        *params = (NMGLvoid*)cntxt->texState.texcoordArray[cntxt->texState.activeTexUnitIndex].pointer;
        break;
    default:
        cntxt->error = NMGL_INVALID_ENUM;
    }

}
