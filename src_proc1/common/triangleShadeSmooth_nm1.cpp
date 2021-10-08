#include "nmtype.h"
#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgltex_nm1.h"
#include "nmgltex_common.h"
#include <stdio.h>
#include <math.h>

#define TRIANGLE_SHADE_SECTION ".text_demo3dExt"

//#define USE_BARYCENTRIC
#define PERSPECTIVE_CORRECT

namespace nm1_version {

  typedef struct Vec4f {
    float x;
    float y;
    float z;
    float w;
  } Vec4f;


  void edgeFunction(float x0, float y0, float x1, float y1, float x2, float y2, float* res);

  SECTION(TRIANGLE_SHADE_SECTION)
    void triangleShadeSmooth(Pattern* patterns, 
        TrianglesInfo* triangles,
        nm32s** pROI,
        Vector2* ptrnPoints,
        Size* ptrnSizes,
        nm32s* pDstTriangle, 
        int count)
    {


#ifdef DEBUG
      // printf ("Start triangleShadeSmooth\n"); 
#endif //DEBUG

      NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();

      long long int temp;
      nm32s* dst = pDstTriangle;
      int winX0 = 0;
      int winY0 = 0;

      for(int cnt=0;cnt<count;cnt++){
        nm64s* pattern = (nm64s*) (patterns + cnt);
        pattern += ptrnPoints[cnt].y;

        int width = ptrnSizes[cnt].width;

        if (ptrnPoints[cnt].x < 0) {
          width += ptrnPoints[cnt].x;
        }

#ifdef __NM__
        int winOffset = ((int)(pROI[cnt]) - (int)cntxt->smallColorBuff.mData) /*>> 2*/;//>> 2 = divide by sizeof int = 4 bytes
#else //__NM__
        int winOffset = ((int)(pROI[cnt]) - (int)cntxt->smallColorBuff.mData) >> 2;//>> 2 = divide by sizeof int = 4 bytes
#endif //__NM__
        //TODO:remove magic number 2
        winY0 = cntxt->texState.segY0 + (winOffset >> 7); //>> 7 = divide by segment width 
        //winY0 = winY0 - 1; //-1 to get the same barycentric values as for OpenGL
        //TODO: remove magic number 7
        winX0 = cntxt->texState.segX0 + (winOffset & (cntxt->texState.segWidth-1));//get reminder  of division by segment width

        float area = 0;
        float x0 = triangles->x0[cnt];
        float y0 = triangles->y0[cnt];
        float x1 = triangles->x1[cnt];
        float y1 = triangles->y1[cnt];
        float x2 = triangles->x2[cnt];
        float y2 = triangles->y2[cnt];

        float scaleCoeff = 1.0/255.0;
        float a0 = ((triangles->c0[cnt] >> 24) & 0x000000ff) * scaleCoeff;
        float r0 = ((triangles->c0[cnt] >> 16) & 0x000000ff) * scaleCoeff;
        float g0 = ((triangles->c0[cnt] >> 8) & 0x000000ff) * scaleCoeff;
        float b0 = ((triangles->c0[cnt]) & 0x000000ff) * scaleCoeff;

        float a1 = ((triangles->c1[cnt] >> 24) & 0x000000ff) * scaleCoeff;
        float r1 = ((triangles->c1[cnt] >> 16) & 0x000000ff) * scaleCoeff;
        float g1 = ((triangles->c1[cnt] >> 8) & 0x000000ff) * scaleCoeff;
        float b1 = ((triangles->c1[cnt]) & 0x000000ff) * scaleCoeff;

        float a2 = ((triangles->c2[cnt] >> 24) & 0x000000ff) * scaleCoeff;
        float r2 = ((triangles->c2[cnt] >> 16) & 0x000000ff) * scaleCoeff;
        float g2 = ((triangles->c2[cnt] >> 8) & 0x000000ff) * scaleCoeff;
        float b2 = ((triangles->c2[cnt]) & 0x000000ff) * scaleCoeff;

        float z0 = triangles->z0[cnt];
        float z1 = triangles->z1[cnt];
        float z2 = triangles->z2[cnt];

#ifdef PERSPECTIVE_CORRECT        
        // Compute some coefficients.
        // Used for:
        // * something similar to linear-rational interpolation 
        // * to calculate scale factor
        // Formulas obtained by simlifying formulas for barycentric coordinates.
        float z12 = z1 * z2;
        float z02 = z0 * z2;
        float z10 = z1 * z0;

        float A2_12 = z12*(y2 - y1);
        float A2_02 = z02*(y0 - y2);
        float A2_10 = z10*(y1 - y0);

        float B2_12 = -z12*(x2 - x1);
        float B2_02 = -z02*(x0 - x2);
        float B2_10 = -z10*(x1 - x0);

        float D2_12 = z12*(x2*y1 - x1*y2);
        float D2_02 = z02*(x0*y2 - x2*y0);
        float D2_10 = z10*(x1*y0 - x0*y1);

        float A2 = A2_12 + A2_02 + A2_10;
        float B2 = B2_12 + B2_02 + B2_10;
        float D2 = D2_12 + D2_02 + D2_10;

        float A1_r = A2_12*r0 + A2_02*r1 + A2_10*r2;
        float B1_r = B2_12*r0 + B2_02*r1 + B2_10*r2;
        float D1_r = D2_12*r0 + D2_02*r1 + D2_10*r2;

        float A1_g = A2_12*g0 + A2_02*g1 + A2_10*g2;
        float B1_g = B2_12*g0 + B2_02*g1 + B2_10*g2;
        float D1_g = D2_12*g0 + D2_02*g1 + D2_10*g2;

        float A1_b = A2_12*b0 + A2_02*b1 + A2_10*b2;
        float B1_b = B2_12*b0 + B2_02*b1 + B2_10*b2;
        float D1_b = D2_12*b0 + D2_02*b1 + D2_10*b2;

        float A1_a = A2_12*a0 + A2_02*a1 + A2_10*a2;
        float B1_a = B2_12*a0 + B2_02*a1 + B2_10*a2;
        float D1_a = D2_12*a0 + D2_02*a1 + D2_10*a2;

        /*****************************************************************/
#else //PERSPECTIVE_CORRECT
        float oneOverTriSquare = 1.0/((x0-x1)*(y2-y1)-(y0-y1)*(x2-x1));
        float A0 = y2-y1;
        float A1 = y0-y2;
        float A2 = y1-y0;

        float B0 = x1-x2;
        float B1 = x2-x0;
        float B2 = x0-x1;

        float D0 = x2*y1-y2*x1;
        float D1 = y2*x0-x2*y0;
        float D2 = x1*y0-x0*y1;

        float A_r = oneOverTriSquare*(A0*r0 + A1*r1 + A2*r2);
        float B_r = oneOverTriSquare*(B0*r0 + B1*r1 + B2*r2);
        float D_r = oneOverTriSquare*(D0*r0 + D1*r1 + D2*r2);

        float A_g = oneOverTriSquare*(A0*g0 + A1*g1 + A2*g2);
        float B_g = oneOverTriSquare*(B0*g0 + B1*g1 + B2*g2);
        float D_g = oneOverTriSquare*(D0*g0 + D1*g1 + D2*g2);

        float A_b = oneOverTriSquare*(A0*b0 + A1*b1 + A2*b2);
        float B_b = oneOverTriSquare*(B0*b0 + B1*b1 + B2*b2);
        float D_b = oneOverTriSquare*(D0*b0 + D1*b1 + D2*b2);

        float A_a = oneOverTriSquare*(A0*a0 + A1*a1 + A2*a2);
        float B_a = oneOverTriSquare*(B0*a0 + B1*a1 + B2*a2);
        float D_a = oneOverTriSquare*(D0*a0 + D1*a1 + D2*a2);

#endif //PERSPECTIVE_CORRECT

#ifdef PERSPECTIVE_CORRECT
        //Precompute reciprocal of vertex z-coordinate.
        //Part of calculation perspective correct attribute values using barycentric coordinates.
        z0 = 1 / z0;
        z1 = 1 / z1;
        z2 = 1 / z2;

        // Prepare vertex attributes. Divde them biy their vertex z-coordinate
        // (though we use a multiplication here because v.z = 1 / v.z)
        //Part of calculation of perspective correct attribute values using barycentric coordinates.
        r0 *= z0;
        r1 *= z1;
        r2 *= z2;

        g0 *= z0;
        g1 *= z1;
        g2 *= z2;

        b0 *= z0;
        b1 *= z1;
        b2 *= z2;

        a0 *= z0;
        a1 *= z1;
        a2 *= z2;
#endif //PERSPECTIVE_CORRECT

#ifdef USE_BARYCENTRIC
        // Area of triangle.
        // Part of calculation attribute values using barycentric coordinates.
        edgeFunction(x0, y0, x1, y1, x2, y2, &area);
        float oneOverArea = 1.0/area;
        int pixelCnt = 0;
#endif //USE_BARYCENTRIC

        for(int y = 0; y < ptrnSizes[cnt].height; y++){
          temp = pattern[y];
          nm32s* pDst = (nm32s*)(dst + y * ptrnSizes[cnt].width);
          if (ptrnPoints[cnt].x < 0) {
            for(int i = 0; i > ptrnPoints[cnt].x; i--){
              pDst[0] = 0x00000000;
              pDst+=1;
            }
          }
          else {
            temp >>= (ptrnPoints[cnt].x * 2);
          }

          for(int x = 0; x < width; x++){
            int mul = temp & 0x3;

            if (mul > 0)//pixel belongs to triangle
            {
              //Calculate x and y of current pixel as float values
              //relative to triangle vertex coordinates inside segment
              float xf = winX0 + x + 0.5f; 
              float yf = winY0 + y + 0.5f;//TODO: Барицентрические координаты не соответствуют растеризованной картинке,
              //то есть растеризованные по шаблону пиксели не должны быть растеризованы, 
              //если использовать барицентрические координаты.
              //Но так как они растеризованы, то для них вычисляются неверные барицентрические
              //координаты и неправильные значения текстурных координат
              //Нужно как-то соотнести алгоритм растеризации и вычисление текстурных координат
              float r = 0.0;
              float g = 0.0;
              float b = 0.0;
              float a = 0.0;
#ifdef USE_BARYCENTRIC
              float w0 = 0;
              float w1 = 0;
              float w2 = 0;
              //TODO: it works only for triangle in CW order
              edgeFunction(x1, y1, x2, y2, xf, yf, &w0);
              edgeFunction(x2, y2, x0, y0, xf, yf, &w1);
              edgeFunction(x0, y0, x1, y1, xf, yf, &w2);

              w0 = w0*oneOverArea;
              w1 = w1*oneOverArea;
              w2 = w2*oneOverArea;

#ifdef PERSPECTIVE_CORRECT
              //Part of calculation of perspective correct attribute values using barycentric coordinates.
              float oneOverZ = z0 * w0 + z1 * w1 + z2 * w2;
              float z = 1 / oneOverZ;
#endif //PERSPECTIVE_CORRECT
              r = r0 * w0 + r1 * w1 + r2 * w2;
              g = g0 * w0 + g1 * w1 + g2 * w2;
              b = b0 * w0 + b1 * w1 + b2 * w2;
              a = a0 * w0 + a1 * w1 + a2 * w2;
#ifdef PERSPECTIVE_CORRECT
              r *= z;
              g *= z;
              b *= z;
              a *= z;
#endif //PERSPECTIVE_CORRECT
#else //USE_BARYCENTRIC
#ifdef PERSPECTIVE_CORRECT
              float oneOverDenominator = 1 / (A2*xf + B2*yf + D2);
              float rl_r = (A1_r*xf + B1_r*yf + D1_r) * oneOverDenominator;
              float rl_g = (A1_g*xf + B1_g*yf + D1_g) * oneOverDenominator;
              float rl_b = (A1_b*xf + B1_b*yf + D1_b) * oneOverDenominator;
              float rl_a = (A1_a*xf + B1_a*yf + D1_a) * oneOverDenominator;
              r = rl_r;
              g = rl_g;
              b = rl_b;
              a = rl_a;
#else //PERSPECTIVE_CORRECT                    
              r = A_r*xf + B_r*yf + D_r;
              g = A_g*xf + B_g*yf + D_g                   
                b = A_b*xf + B_b*yf + D_b;
              a = A_a*xf + B_a*yf + D_a;
#endif //PERSPECTIVE_CORRECT
#endif //USE_BARYCENTRIC
              Vec4f c;
              c.x = r;
              c.y = g;
              c.z = b;
              c.w = a;

              nm32s color = 0;
              //(nm32s)pDst[0] = 0xARGB
              color = color | (((nm32s)(c.w * 255) & 0xff) << 24);//a
              color = color | (((nm32s)(c.x * 255) & 0xff) << 16); //r
              color = color | (((nm32s)(c.y * 255) & 0xff) << 8);//g
              color = color | (((nm32s)(c.z * 255) & 0xff));//b
              pDst[0] = mul * color;
            }

            pDst += 1;
            temp >>= 2;
          }
        }
        dst += ptrnSizes[cnt].height * ptrnSizes[cnt].width;
      }
#ifdef DEBUG
      // printf ("End triangleShadeSmooth\n");     
#endif //DEBUG

      return;
    }

} //end of namespace nm1_version 
