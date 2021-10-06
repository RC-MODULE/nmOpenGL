
#include "demo3d_common.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgltex_nm0.h"
#include "nmgltex_common.h"
#include <stdio.h>
#include <math.h>
#include <float.h> //TODO: only FLT_EPSILON is used from float.h

#define TRIANGLE_SHADE_SECTION ".text"


#define PERSPECTIVE_CORRECT

namespace tex_nm0 {

  typedef struct Vec4f {
    float x;
    float y;
    float z;
    float w;
  } Vec4f;


  SECTION(".data_imu0") float initVecx [32] = {0.5,   1.5,  2.5,  3.5,  4.5,  5.5,  6.5,  7.5,  8.5,  9.5, 
    10.5, 11.5, 12.5, 13.5, 14.5, 15.5, 16.5, 17.5, 18.5, 19.5,
    20.5, 21.5, 22.5, 23.5, 24.5, 25.5, 26.5, 27.5, 28.5, 29.5,
    30.5, 31.5};
  SECTION(".data_imu0") float initVecy [32] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 
    0.5, 0.5};
  SECTION(".data_imu0") float vecxf [32];
  SECTION(".data_imu0") float vecyf [32];
  SECTION(".data_imu0") float vecx [32];
  SECTION(".data_imu0") float vecy [32];
  SECTION(".data_imu0") float vecr [32];
  SECTION(".data_imu0") float vecg [32];
  SECTION(".data_imu0") float vecb [32];
  SECTION(".data_imu0") float veca [32];

  SECTION(".data_imu0") float ones [32] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 
    1.0, 1.0};

  SECTION(".data_imu0") float oneOverDenominator [32];
  SECTION(".data_imu0") float derivOneOverDenom [32];

  SECTION(".data_imu0") float buf0 [32];

  SECTION(TRIANGLE_SHADE_SECTION)
    int minf(float a, float b)
    {
      return (b < a) ? b : a;
    }

  SECTION(TRIANGLE_SHADE_SECTION)
    int maxf(float a, float b)
    {
      return (b > a) ? b : a;
    }

  SECTION(TRIANGLE_SHADE_SECTION)
    void triangleShadeSmooth(TrianglesInfo* triangles, nm32s* pDstTriangle, int count)
    {

#ifdef DEBUG
      // printf ("Start triangleShadeSmooth\n"); 
#endif //DEBUG

      long long int temp;
      nm32s* dst = pDstTriangle;
      int winX0 = 0;
      int winY0 = 0;

      for(int cnt=0;cnt<count;cnt++){
        nm32s* dstTriagle = (nm32s*) (pDstTriangle + WIDTH_PTRN * HEIGHT_PTRN * cnt);

        float area = 0;
        float x0 = triangles->x0[cnt];
        float y0 = triangles->y0[cnt];
        float x1 = triangles->x1[cnt];
        float y1 = triangles->y1[cnt];
        float x2 = triangles->x2[cnt];
        float y2 = triangles->y2[cnt];

        //TODO:R,G,B,A order is used in c0,c1,c2. May be it will be necessary to use B,G,R,A order 
        float scaleCoeff = 1.0/255.0;
        float r0 = (float)( triangles->c0[cnt].vec[0]        & 0x00000000ffffffff) * scaleCoeff;
        float g0 = (float)((triangles->c0[cnt].vec[0] >> 32) & 0x00000000ffffffff) * scaleCoeff;
        float b0 = (float)( triangles->c0[cnt].vec[1]        & 0x00000000ffffffff) * scaleCoeff;
        float a0 = (float)((triangles->c0[cnt].vec[1] >> 32) & 0x00000000ffffffff) * scaleCoeff;

        float r1 = (float)( triangles->c1[cnt].vec[0]        & 0x00000000ffffffff) * scaleCoeff;
        float g1 = (float)((triangles->c1[cnt].vec[0] >> 32) & 0x00000000ffffffff) * scaleCoeff;
        float b1 = (float)( triangles->c1[cnt].vec[1]        & 0x00000000ffffffff) * scaleCoeff;
        float a1 = (float)((triangles->c1[cnt].vec[1] >> 32) & 0x00000000ffffffff) * scaleCoeff;

        float r2 = (float)( triangles->c2[cnt].vec[0]        & 0x00000000ffffffff) * scaleCoeff; 
        float g2 = (float)((triangles->c2[cnt].vec[0] >> 32) & 0x00000000ffffffff) * scaleCoeff; 
        float b2 = (float)( triangles->c2[cnt].vec[1]        & 0x00000000ffffffff) * scaleCoeff; 
        float a2 = (float)((triangles->c2[cnt].vec[1] >> 32) & 0x00000000ffffffff) * scaleCoeff; 

        float z0 = triangles->z0[cnt];
        float z1 = triangles->z1[cnt];
        float z2 = triangles->z2[cnt];

        winY0 = minf(y0, minf(y1, y2)); 
        winX0 = minf(x0, minf(x1, x2));

        unsigned int maxX = maxf(x0, maxf(x1, x2));
        unsigned int maxY = maxf(y0, maxf(y1, y2));

        unsigned int primHeight = maxY - winY0 + 1;
        unsigned int primWidth = maxX - winX0 + 1;

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

        nmblas_scopy(primWidth, initVecx, 1, vecxf, 1);
        nmblas_scopy(primWidth, initVecy, 1, vecyf, 1);
        nmppsAddC_32f(vecxf, vecxf, winX0, primWidth);
        nmppsAddC_32f(vecyf, vecyf, winY0, primWidth);

        for(int y = 0; y < primHeight; y++){

          nm32s* pDst = (nm32s*)(dstTriagle + y * WIDTH_PTRN);

#ifdef PERSPECTIVE_CORRECT
          //float oneOverDenominator = 1 / (A2*xf + B2*yf + D2);
          nmppsMulC_AddC_32f(vecyf, B2, D2, buf0, primWidth);
          nmppsMulC_AddV_32f(vecxf, buf0, buf0, A2, primWidth);
          nmppsDiv_32f(ones, buf0, oneOverDenominator, primWidth);

          //float r = (A1_r*xf + B1_r*yf + D1_r) * oneOverDenominator;
          nmppsMulC_AddC_32f(vecyf, B1_r, D1_r, buf0, primWidth);
          nmppsMulC_AddV_32f(vecxf, buf0, vecr, A1_r, primWidth);
          nmppsMul_AddC_32f(vecr, oneOverDenominator, 0.0, vecr, primWidth);

          //float g = (A1_g*xf + B1_g*yf + D1_g) * oneOverDenominator;
          nmppsMulC_AddC_32f(vecyf, B1_g, D1_g, buf0, primWidth);
          nmppsMulC_AddV_32f(vecxf, buf0, vecg, A1_g, primWidth);
          nmppsMul_AddC_32f(vecg, oneOverDenominator, 0.0, vecg, primWidth);

          //float b = (A1_b*xf + B1_b*yf + D1_b) * oneOverDenominator;
          nmppsMulC_AddC_32f(vecyf, B1_b, D1_b, buf0, primWidth);
          nmppsMulC_AddV_32f(vecxf, buf0, vecb, A1_b, primWidth);
          nmppsMul_AddC_32f(vecb, oneOverDenominator, 0.0, vecb, primWidth);

          //float a = (A1_a*xf + B1_a*yf + D1_a) * oneOverDenominator;
          nmppsMulC_AddC_32f(vecyf, B1_a, D1_a, buf0, primWidth);
          nmppsMulC_AddV_32f(vecxf, buf0, veca, A1_a, primWidth);
          nmppsMul_AddC_32f(veca, oneOverDenominator, 0.0, veca, primWidth);
#else //PERSPECTIVE_CORRECT                    
          //r = A_r*xf + B_r*yf + D_s;
          nmppsMulC_AddC_32f(vecyf, B_r, D_r, buf0, primWidth);
          nmppsMulC_AddV_32f(vecxf, buf0, vecr, A_r, primWidth);

          //g = A_g*xf + B_g*yf + D_g;
          nmppsMulC_AddC_32f(vecyf, B_g, D_g, buf0, primWidth);
          nmppsMulC_AddV_32f(vecxf, buf0, vecg, A_g, primWidth);

          //b = A_b*xf + B_b*yf + D_b;
          nmppsMulC_AddC_32f(vecyf, B_b, D_b, buf0, primWidth);
          nmppsMulC_AddV_32f(vecxf, buf0, vecb, A_b, primWidth);

          //a = A_a*xf + B_a*yf + D_a;
          nmppsMulC_AddC_32f(vecyf, B_a, D_a, buf0, primWidth);
          nmppsMulC_AddV_32f(vecxf, buf0, veca, A_a, primWidth);
#endif //PERSPECTIVE_CORRECT

          for(int x = 0; x < primWidth; x++){
            Vec4f c;
            c.x = vecr[x];
            c.y = vecg[x];
            c.z = vecb[x];
            c.w = veca[x];

            nm32s color = 0;
            //(nm32s)pDst[0] = 0xARGB
            color = color | (((nm32s)(c.w * 255) & 0xff) << 24);//a
            color = color | (((nm32s)(c.x * 255) & 0xff) << 16); //r
            color = color | (((nm32s)(c.y * 255) & 0xff) << 8);//g
            color = color | (((nm32s)(c.z * 255) & 0xff));//b
            pDst[0] = color;
            pDst += 1;
          }
          nmppsAddC_32f(vecyf, vecyf, 1.0, primWidth);
        }
      }
#ifdef DEBUG
      // printf ("End triangleShadeSmooth\n");     
#endif //DEBUG

      return;
    }

  
  SECTION(TRIANGLE_SHADE_SECTION)
    void edgeFunction(float x0, float y0, float x1, float y1, float x2, float y2, float* res)
    {
      *res = (x2 - x0) * (y1 - y0) - (y2 - y0) * (x1 - x0);
    }
  
} //end of namespace nm0
