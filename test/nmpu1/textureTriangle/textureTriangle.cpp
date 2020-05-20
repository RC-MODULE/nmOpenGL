#include "nmtype.h"
#include "demo3d_common.h"
#include "nmgltex_nm1.h"
#include "textureTriangle.h"
#include <cstdio>

SECTION(".text_demo3d")

void textureTriangle(Pattern* patterns, 
                 Triangles* triangles,
                 Rectangle* windows, 
                 nm32s* pSrcTriangle, 
                 nm32s* pDstTriangle, 
                 int count)
{
    printf ("Start textureTriangle\n"); 
    
    long long int temp;
    nm32s* dst = pDstTriangle;
    nm32s* src = pSrcTriangle;
    for(int c=0;c<count;c++){
        nm64s* pattern = (nm64s*) (patterns + c);
        pattern += windows[c].y;
        
        int width = windows[c].width;

        if (windows[c].x < 0) {
            width += windows[c].x;
        }
        
//start calculate pixel value for texturing
        
        float area = 0;
        float x0 = triangles->x0[c];
        float y0 = triangles->y0[c];
        float x1 = triangles->x1[c];
        float y1 = triangles->y1[c];
        float x2 = triangles->x2[c];
        float y2 = triangles->y2[c];

        float s0 = triangles->s0[c];
        float t0 = triangles->t0[c];
        float s1 = triangles->s1[c];
        float t1 = triangles->t1[c];
        float s2 = triangles->s2[c];
        float t2 = triangles->t2[c];
        
        float z0 = triangles->z[c];
        float z1 = triangles->z[c];
        float z2 = triangles->z[c];
        
        
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

		float A1_s = A2_12*s0 + A2_02*s1 + A2_10*s2;
		float B1_s = B2_12*s0 + B2_02*s1 + B2_10*s2;
		float D1_s = D2_12*s0 + D2_02*s1 + D2_10*s2;

		float A1_t = A2_12*t0 + A2_02*t1 + A2_10*t2;
		float B1_t = B2_12*t0 + B2_02*t1 + B2_10*t2;
		float D1_t = D2_12*t0 + D2_02*t1 + D2_10*t2;
        
		/*****************************************************************/

		//Precompute reciprocal of vertex z-coordinate.
		//Part of calculation perspective correct attribute values using barycentric coordinates.
		z0 = 1 / z0;
		z1 = 1 / z1;
		z2 = 1 / z2;

		// Prepare vertex attributes. Divde them by their vertex z-coordinate
		// (though we use a multiplication here because v.z = 1 / v.z)
		//Part of calculation of perspective correct attribute values using barycentric coordinates.
        s0 *= z0;
        s1 *= z1;
        s2 *= z2;
        t0 *= z0;
        t1 *= z1;
        t2 *= z2;
        
        // Area of triangle.
        // Part of calculation attribute values using barycentric coordinates.
        edgeFunction(x0, y0, x1, y1, x2, y2, &area);
        
        for(int y = 0; y < windows[c].height; y++){
            temp = pattern[y];
            nm32s* pDst = (nm32s*)(dst + y * windows[c].width);
            nm32s* pSrc = (nm32s*)(src + y * windows[c].width);
            if (windows[c].x < 0) {
                for(int i = 0; i > windows[c].x; i--){
                    pDst[0] = 0x00000000;
                    pDst+=1;
                    pSrc+=1;
                }
            }
            else {
                temp >>= (windows[c].x * 2);
            }
            
            for(int x = 0; x < width; x++){
                int mul = temp & 0x3;

                if (mul > 0)//pixel belongs to triangle
                {
                    //Calculate x and y of current pixel as float values
                    //TODO: Incorrect. xf and yf should be relative to triangle vertex coordinates
                    //(inside pattern or inside segment)
                    float xf = x + 0.5f; 
                    float yf = y + 0.5f;

                    float w0 = 0;
                    float w1 = 0;
                    float w2 = 0;
                    edgeFunction(x1, y1, x2, y2, xf, yf, &w0);
                    edgeFunction(x2, y2, x0, y0, xf, yf, &w1);
                    edgeFunction(x0, y0, x1, y1, xf, yf, &w2);
                    
                    //Part of calculation of perspective correct attribute values using barycentric coordinates.
                    float oneOverZ = z0 * w0 + z1 * w1 + z2 * w2;
                    float z = 1 / oneOverZ;
                    
                    float s = 0.0;
                    float t = 0.0;
#ifdef USE_BARYCENTRIC
                    s = s0 * w0 + s1 * w1 + s2 * w2;
                    t = t0 * w0 + t1 * w1 + t2 * w2;
                    s *= z;
                    t *= z;
#else
                    float oneOverDenominator = 1 / (A2*xf + B2*yf + D2);
                    float rl_s = (A1_s*xf + B1_s*yf + D1_s) * oneOverDenominator;
                    float rl_t = (A1_t*xf + B1_t*yf + D1_t) * oneOverDenominator;
                    s = rl_s;
                    t = rl_t;
#endif
                    pDst[0] = mul * pSrc[0];
                }

                pDst += 1;
                pSrc += 1;
                temp >>= 2;
            }
        }
        src += windows[c].height * windows[c].width;
        dst += windows[c].height * windows[c].width;
    }
    
    printf ("End textureTriangle\n");     
    return;
}


void edgeFunction(float x0, float y0, float x1, float y1, float x2, float y2, float* res)
{
    *res = (x2 - x0) * (y1 - y0) - (y2 - y0) * (x1 - x0);
}