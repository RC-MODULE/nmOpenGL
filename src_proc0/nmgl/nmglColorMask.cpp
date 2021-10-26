#include "demo3d_nm0.h"
#include "nmgl.h"

void nmglColorMask (NMGLboolean red, NMGLboolean green, NMGLboolean blue, NMGLboolean alpha){

    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

    if(red != NMGL_FALSE) red = NMGL_TRUE;
    if(green != NMGL_FALSE) green = NMGL_TRUE;
    if(blue != NMGL_FALSE) blue = NMGL_TRUE;
    if(alpha != NMGL_FALSE) alpha = NMGL_TRUE;

    cntxt->color_write_mask_flags[0] = red;
    cntxt->color_write_mask_flags[1] = green;
    cntxt->color_write_mask_flags[2] = blue;
    cntxt->color_write_mask_flags[3] = alpha;

}
