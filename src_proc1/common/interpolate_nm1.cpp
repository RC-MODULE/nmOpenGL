#include "nmtype.h"
#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgltex_nm1.h"
#include "nmgltex_common.h"
#include <stdio.h>
#include <math.h>

namespace nm1_version {


  SECTION(".text_demo3dExt")
    void edgeFunction(float x0, float y0, float x1, float y1, float x2, float y2, float* res)
    {
      *res = (x2 - x0) * (y1 - y0) - (y2 - y0) * (x1 - x0);
    }


} //end of namespace nm1_version 
