#include "demo3d_nm0.h"
#include "nmblas.h"
#include "service.h"
#include "filter.h"
#include "utility_float.h"

//#define PERSPECTIVE_CORRECT

static void unpackColors(const v4nm32f *color, float *rr, float *gg, float *bb, float *aa, int size);
static  int pushPoints(const nm32f *x, const nm32f *y, const nm32f *z, const nm32f *w, 
						const nm32f *r, const nm32f *g, const nm32f *b, const nm32f *a, 
						const nm32f *s, const nm32f *t, 
						TrianglePointers *dst, int offset, int n);

#ifdef PERSPECTIVE_CORRECT
SECTION(".data") static float oneOverDenominator[NMGL_SIZE];
SECTION(".data") static float z12[NMGL_SIZE];
SECTION(".data") static float z02[NMGL_SIZE];
SECTION(".data") static float z10[NMGL_SIZE];
SECTION(".data") static float A2_12[NMGL_SIZE];
SECTION(".data") static float A2_02[NMGL_SIZE];
SECTION(".data") static float A2_10[NMGL_SIZE];
SECTION(".data") static float B2_12[NMGL_SIZE];
SECTION(".data") static float B2_02[NMGL_SIZE];
SECTION(".data") static float B2_10[NMGL_SIZE];
SECTION(".data") static float D2_12[NMGL_SIZE];
SECTION(".data") static float D2_02[NMGL_SIZE];
SECTION(".data") static float D2_10[NMGL_SIZE];
SECTION(".data") static float A2[NMGL_SIZE];
SECTION(".data") static float B2[NMGL_SIZE];
SECTION(".data") static float D2[NMGL_SIZE];
SECTION(".data") static float A1_r[NMGL_SIZE];
SECTION(".data") static float B1_r[NMGL_SIZE];
SECTION(".data") static float D1_r[NMGL_SIZE];
SECTION(".data") static float A1_g[NMGL_SIZE];
SECTION(".data") static float B1_g[NMGL_SIZE];
SECTION(".data") static float D1_g[NMGL_SIZE];
SECTION(".data") static float A1_b[NMGL_SIZE];
SECTION(".data") static float B1_b[NMGL_SIZE];
SECTION(".data") static float D1_b[NMGL_SIZE];
SECTION(".data") static float A1_a[NMGL_SIZE];
SECTION(".data") static float B1_a[NMGL_SIZE];
SECTION(".data") static float D1_a[NMGL_SIZE];
SECTION(".data") static float A1_s[NMGL_SIZE];
SECTION(".data") static float B1_s[NMGL_SIZE];
SECTION(".data") static float D1_s[NMGL_SIZE];
SECTION(".data") static float A1_t[NMGL_SIZE];
SECTION(".data") static float B1_t[NMGL_SIZE];
SECTION(".data") static float D1_t[NMGL_SIZE];
SECTION(".data") static float A1_z[NMGL_SIZE];
SECTION(".data") static float B1_z[NMGL_SIZE];
SECTION(".data") static float D1_z[NMGL_SIZE];
SECTION(".data") static float A1_w[NMGL_SIZE];
SECTION(".data") static float B1_w[NMGL_SIZE];
SECTION(".data") static float D1_w[NMGL_SIZE];
#else // PERSPECTIVE_CORRECT
SECTION(".data") static float oneOverTriSquare[NMGL_SIZE];
SECTION(".data") static float A0[NMGL_SIZE];
SECTION(".data") static float A1[NMGL_SIZE];
SECTION(".data") static float A2[NMGL_SIZE];
SECTION(".data") static float B0[NMGL_SIZE];
SECTION(".data") static float B1[NMGL_SIZE];
SECTION(".data") static float B2[NMGL_SIZE];
SECTION(".data") static float D0[NMGL_SIZE];
SECTION(".data") static float D1[NMGL_SIZE];
SECTION(".data") static float D2[NMGL_SIZE];
SECTION(".data") static float A_r[NMGL_SIZE];
SECTION(".data") static float B_r[NMGL_SIZE];
SECTION(".data") static float D_r[NMGL_SIZE];
SECTION(".data") static float A_g[NMGL_SIZE];
SECTION(".data") static float B_g[NMGL_SIZE];
SECTION(".data") static float D_g[NMGL_SIZE];
SECTION(".data") static float A_b[NMGL_SIZE];
SECTION(".data") static float B_b[NMGL_SIZE];
SECTION(".data") static float D_b[NMGL_SIZE];
SECTION(".data") static float A_a[NMGL_SIZE];
SECTION(".data") static float B_a[NMGL_SIZE];
SECTION(".data") static float D_a[NMGL_SIZE];
SECTION(".data") static float A_s[NMGL_SIZE];
SECTION(".data") static float B_s[NMGL_SIZE];
SECTION(".data") static float D_s[NMGL_SIZE];
SECTION(".data") static float A_t[NMGL_SIZE];
SECTION(".data") static float B_t[NMGL_SIZE];
SECTION(".data") static float D_t[NMGL_SIZE];
SECTION(".data") static float A_z[NMGL_SIZE];
SECTION(".data") static float B_z[NMGL_SIZE];
SECTION(".data") static float D_z[NMGL_SIZE];
SECTION(".data") static float A_w[NMGL_SIZE];
SECTION(".data") static float B_w[NMGL_SIZE];
SECTION(".data") static float D_w[NMGL_SIZE];
#endif // PERSPECTIVE_CORRECT

SECTION(".data") static nm32f x[NMGL_SIZE];
SECTION(".data") static nm32f y[NMGL_SIZE];
SECTION(".data") static nm32f z[NMGL_SIZE];
SECTION(".data") static nm32f w[NMGL_SIZE];
SECTION(".data") static nm32f r[NMGL_SIZE];
SECTION(".data") static nm32f g[NMGL_SIZE];
SECTION(".data") static nm32f b[NMGL_SIZE];
SECTION(".data") static nm32f a[NMGL_SIZE];
SECTION(".data") static nm32f s[NMGL_SIZE];
SECTION(".data") static nm32f t[NMGL_SIZE];

SECTION(".data") static nm32f r0[NMGL_SIZE];
SECTION(".data") static	nm32f g0[NMGL_SIZE];
SECTION(".data") static	nm32f b0[NMGL_SIZE];
SECTION(".data") static	nm32f a0[NMGL_SIZE];

SECTION(".data") static	nm32f r1[NMGL_SIZE];
SECTION(".data") static	nm32f g1[NMGL_SIZE];
SECTION(".data") static	nm32f b1[NMGL_SIZE];
SECTION(".data") static	nm32f a1[NMGL_SIZE];

SECTION(".data") static	nm32f r2[NMGL_SIZE];
SECTION(".data") static	nm32f g2[NMGL_SIZE];
SECTION(".data") static	nm32f b2[NMGL_SIZE];
SECTION(".data") static	nm32f a2[NMGL_SIZE];

int splitTriangles_uniform(TrianglePointers *srcTriangles, 
			 int srcCount, 
			 int maxWidth, 
			 int maxHeight, 
			 int maxDstSize, 
			 TrianglePointers *dstTriangles, 
			 int *srcTreatedCount)
{
	if (0 == srcCount){
		*srcTreatedCount = 0;
		return 0;	
	} else {
		// Do nothing here and continue;
	}

	NMGL_Context_NM0 *cntxt 	= NMGL_Context_NM0::getContext();
	// Use these arrays when masks are used to decide if the triangles needs 
	// division
	//int widthMaskEven [16] 		= {0};
	//int widthMaskOdd  [16] 		= {0};
	//int heightMaskEven[16] 		= {0};
	//int heightMaskOdd [16] 		= {0};
	float *nf					= (float *)cntxt->buffer2 + 0 * NMGL_SIZE; // number of segments
	float *numberOfOutputTrs 	= (float *)cntxt->buffer2 + 1 * NMGL_SIZE; // number of output triangles 
	float *ab_dx				= (float *)cntxt->buffer2 + 2 * NMGL_SIZE; // max of width and height segment numbers
	float *ab_dy				= (float *)cntxt->buffer2 + 3 * NMGL_SIZE; // max of width and height segment numbers
	float *ac_dx				= (float *)cntxt->buffer2 + 4 * NMGL_SIZE; // max of width and height segment numbers
	float *ac_dy				= (float *)cntxt->buffer2 + 5 * NMGL_SIZE; // max of width and height segment numbers
	float *width 	 			= (float *)cntxt->buffer2 + 6 * NMGL_SIZE; 
	float *height	 			= (float *)cntxt->buffer2 + 7 * NMGL_SIZE; 
	int dstSize 				= 0;	// Number of triangles in dstTriangles

	int srcCountEven = srcCount + (srcCount & 1);

	{
		float *min = (float *)cntxt->buffer2 +  8 * NMGL_SIZE;
		float *max = (float *)cntxt->buffer2 +  9 * NMGL_SIZE; 
		float *nw  = (float *)cntxt->buffer2 + 10 * NMGL_SIZE; // number of segments the width to divide by
		float *nh  = (float *)cntxt->buffer2 + 11 * NMGL_SIZE; // number of segments the height to divide by

		// Get the width of triangles
		findMinMax3(srcTriangles->v0.x, srcTriangles->v1.x, srcTriangles->v2.x, min, max, srcCountEven);
		nmppsSub_32f(max, min, width, srcCountEven);

		// Get the height of triangles
		findMinMax3(srcTriangles->v0.y, srcTriangles->v1.y, srcTriangles->v2.y, min, max, srcCountEven);
		nmppsSub_32f(max, min, height, srcCountEven);

		// Check the width and height. Get the flag for each triangle: 
		// 1 - needs division, 0 - doesn't need division
		// Use the following two lines of code if the masks are used to decide if 
		// the triangles needs division
		//cmpGtC_f(width, maxWidth, (nm1 *) widthMaskEven, (nm1 *) widthMaskOdd, srcCountEven);
		//cmpGtC_f(height, maxHeight, (nm1 *) heightMaskEven, (nm1 *) heightMaskOdd, srcCountEven);

		// Get the number of segments (nf) for the width and height
		// 1. Calculate the number of segments for the width and height to divide by
		// and take the max of them
		nmppsMulC_32f(width, nw, 1.0 / maxWidth, srcCountEven);
		nmppsMulC_32f(height, nh, 1.0 / maxHeight, srcCountEven);
		findMax2(nw, nh, max, srcCountEven);

		// Round the max number of segments to the nearest right integer
		// (and convert it to float)
		nmppsConvert_32f32f_ceiling(max, nf, 0, srcCountEven);

		// Count the number of output triangles for each input triangle 
		nmppsMul_32f(nf, nf, numberOfOutputTrs, srcCountEven);	
	}

	{
		// Get ab_dx = (bx - ax)/n and ab_dy = (by - ay)/n
		float *ab_x	= (float *)cntxt->buffer2 +  8 * NMGL_SIZE; // max of width and height segment numbers
		float *ab_y	= (float *)cntxt->buffer2 +  9 * NMGL_SIZE; // max of width and height segment numbers
		// Get ab_x, ab_y
		nmppsSub_32f(srcTriangles->v1.x, srcTriangles->v0.x, ab_x, srcCountEven);
		nmppsSub_32f(srcTriangles->v1.y, srcTriangles->v0.y, ab_y, srcCountEven);
		// Get ab_dx, ab_dy
		nmppsDiv_32f(ab_x, nf, ab_dx, srcCountEven);
		nmppsDiv_32f(ab_y, nf, ab_dy, srcCountEven);
	}

	{
		// Get ac_dx = (cx - ax)/n and ac_dy = (cy - ay)/n
		float *ac_x	= (float *)cntxt->buffer2 +  8 * NMGL_SIZE; // max of width and height segment numbers
		float *ac_y	= (float *)cntxt->buffer2 +  9 * NMGL_SIZE; // max of width and height segment numbers
		// Get ac_x, ac_y
		nmppsSub_32f(srcTriangles->v2.x, srcTriangles->v0.x, ac_x, srcCountEven);
		nmppsSub_32f(srcTriangles->v2.y, srcTriangles->v0.y, ac_y, srcCountEven);
		// Get ac_dx, ac_dy
		nmppsDiv_32f(ac_x, nf, ac_dx, srcCountEven);
		nmppsDiv_32f(ac_y, nf, ac_dy, srcCountEven);
	}


	// Interpolation

	float *x0 = srcTriangles->v0.x;
	float *y0 = srcTriangles->v0.y;
	float *z0 = srcTriangles->v0.z;
	float *w0 = srcTriangles->v0.w;
	float *s0 = srcTriangles->v0.s;
	float *t0 = srcTriangles->v0.t;

	float *x1 = srcTriangles->v1.x;
	float *y1 = srcTriangles->v1.y;
	float *z1 = srcTriangles->v1.z;
	float *w1 = srcTriangles->v1.w;
	float *s1 = srcTriangles->v1.s;
	float *t1 = srcTriangles->v1.t;

	float *x2 = srcTriangles->v2.x;
	float *y2 = srcTriangles->v2.y;
	float *z2 = srcTriangles->v2.z;
	float *w2 = srcTriangles->v2.w;
	float *s2 = srcTriangles->v2.s;
	float *t2 = srcTriangles->v2.t;

	unpackColors(srcTriangles->v0.color, r0, g0, b0, a0, srcCountEven);
	unpackColors(srcTriangles->v1.color, r1, g1, b1, a1, srcCountEven);
	unpackColors(srcTriangles->v2.color, r2, g2, b2, a2, srcCountEven);

	nm32f *ones = cntxt->buffer2 +  8 * NMGL_SIZE;
	nm32f *buf  = cntxt->buffer2 +  9 * NMGL_SIZE;
	nm32f *buf1 = cntxt->buffer2 +  10 * NMGL_SIZE;;
	nm32f *buf2 = cntxt->buffer2 +  11 * NMGL_SIZE;;

#ifdef PERSPECTIVE_CORRECT        
	// Compute some coefficients to interpolate attribute values.
	// Formulas obtained by simplifying formulas for barycentric coordinates.

	for (size_t kk = 0; kk < NMGL_SIZE; ++kk){
		ones[kk] = 1.0f;
	}

	// z12 = z1 * z2
	// z02 = z0 * z2
	// z10 = z1 * z0
	nmppsMul_32f(z1, z2, z12, srcCountEven);	
	nmppsMul_32f(z0, z2, z02, srcCountEven);	
	nmppsMul_32f(z1, z0, z10, srcCountEven);	

	// A2_12 = z12*(y2 - y1);
	nmppsSub_32f(y2, y1, buf, srcCountEven);
	nmppsMul_32f(z12, buf, A2_12, srcCountEven);	
	// A2_02 = z02*(y0 - y2);
	nmppsSub_32f(y0, y2, buf, srcCountEven);
	nmppsMul_32f(z02, buf, A2_02, srcCountEven);	
	// A2_10 = z10*(y1 - y0);
	nmppsSub_32f(y1, y0, buf, srcCountEven);
	nmppsMul_32f(z10, buf, A2_10, srcCountEven);	

	// B2_12 = -z12*(x2 - x1) = z12*(x1 - x2);
	nmppsSub_32f(x1, x2, buf, srcCountEven);
	nmppsMul_32f(z12, buf, B2_12, srcCountEven);	
	// B2_02 = -z02*(x0 - x2) = z02*(x2 - x0);
	nmppsSub_32f(x2, x0, buf, srcCountEven);
	nmppsMul_32f(z02, buf, B2_02, srcCountEven);	
	// B2_10 = -z10*(x1 - x0) = z10*(x0 - x1);
	nmppsSub_32f(x0, x1, buf, srcCountEven);
	nmppsMul_32f(z10, buf, B2_10, srcCountEven);	

	// D2_12 = z12*(x2*y1 - x1*y2);
	nmppsMul_32f(x2, y1, buf1, srcCountEven);	
	nmppsMul_32f(x1, y2, buf2, srcCountEven);	
	nmppsSub_32f(buf1, buf2, buf, srcCountEven);
	nmppsMul_32f(z12, buf, D2_12, srcCountEven);	
	// D2_02 = z02*(x0*y2 - x2*y0);
	nmppsMul_32f(x0, y2, buf1, srcCountEven);	
	nmppsMul_32f(x2, y0, buf2, srcCountEven);	
	nmppsSub_32f(buf1, buf2, buf, srcCountEven);
	nmppsMul_32f(z02, buf, D2_02, srcCountEven);	
	// D2_10 = z10*(x1*y0 - x0*y1);
	nmppsMul_32f(x1, y0, buf1, srcCountEven);	
	nmppsMul_32f(x0, y1, buf2, srcCountEven);	
	nmppsSub_32f(buf1, buf2, buf, srcCountEven);
	nmppsMul_32f(z10, buf, D2_10, srcCountEven);	

	//A2 = A2_12 + A2_02 + A2_10;
	nmppsAdd_32f(A2_12, A2_02, buf, srcCountEven);
	nmppsAdd_32f(  buf, A2_10,  A2, srcCountEven);
	//B2 = B2_12 + B2_02 + B2_10;
	nmppsAdd_32f(B2_12, B2_02, buf, srcCountEven);
	nmppsAdd_32f(  buf, B2_10,  B2, srcCountEven);
	//D2 = D2_12 + D2_02 + D2_10;
	nmppsAdd_32f(D2_12, D2_02, buf, srcCountEven);
	nmppsAdd_32f(  buf, D2_10,  D2, srcCountEven);

	//float A1_r = A2_12*r0 + A2_02*r1 + A2_10*r2;
	nmppsMul_32f(A2_10, r2, buf, srcCountEven);	// buf = A2_10 * r2
	nmppsMul_Add_32f(A2_02, r1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(A2_12, r0, buf1, A1_r, srcCountEven);
	//float B1_r = B2_12*r0 + B2_02*r1 + B2_10*r2;
	nmppsMul_32f(B2_10, r2, buf, srcCountEven);
	nmppsMul_Add_32f(B2_02, r1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(B2_12, r0, buf1, B1_r, srcCountEven);
	//float D1_r = D2_12*r0 + D2_02*r1 + D2_10*r2;
	nmppsMul_32f(D2_10, r2, buf, srcCountEven);
	nmppsMul_Add_32f(D2_02, r1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(D2_12, r0, buf1, D1_r, srcCountEven);

	//float A1_g = A2_12*g0 + A2_02*g1 + A2_10*g2;
	nmppsMul_32f(A2_10, g2, buf, srcCountEven);	
	nmppsMul_Add_32f(A2_02, g1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(A2_12, g0, buf1, A1_g, srcCountEven);
	//float B1_g = B2_12*g0 + B2_02*g1 + B2_10*g2;
	nmppsMul_32f(B2_10, g2, buf, srcCountEven);	
	nmppsMul_Add_32f(B2_02, g1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(B2_12, g0, buf1, B1_g, srcCountEven);
	//float D1_g = D2_12*g0 + D2_02*g1 + D2_10*g2;
	nmppsMul_32f(D2_10, g2, buf, srcCountEven);
	nmppsMul_Add_32f(D2_02, g1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(D2_12, g0, buf1, D1_g, srcCountEven);

	//float A1_b = A2_12*b0 + A2_02*b1 + A2_10*b2;
	nmppsMul_32f(A2_10, b2, buf, srcCountEven);	
	nmppsMul_Add_32f(A2_02, b1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(A2_12, b0, buf1, A1_b, srcCountEven);
	//float B1_b = B2_12*b0 + B2_02*b1 + B2_10*b2;
	nmppsMul_32f(B2_10, b2, buf, srcCountEven);	
	nmppsMul_Add_32f(B2_02, b1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(B2_12, b0, buf1, B1_b, srcCountEven);
	//float D1_b = D2_12*b0 + D2_02*b1 + D2_10*b2;
	nmppsMul_32f(D2_10, b2, buf, srcCountEven);
	nmppsMul_Add_32f(D2_02, b1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(D2_12, b0, buf1, D1_b, srcCountEven);

	//float A1_a = A2_12*a0 + A2_02*a1 + A2_10*a2;
	nmppsMul_32f(A2_10, a2, buf, srcCountEven);	
	nmppsMul_Add_32f(A2_02, a1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(A2_12, a0, buf1, A1_a, srcCountEven);
	//float B1_a = B2_12*a0 + B2_02*a1 + B2_10*a2;
	nmppsMul_32f(B2_10, a2, buf, srcCountEven);	
	nmppsMul_Add_32f(B2_02, a1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(B2_12, a0, buf1, B1_a, srcCountEven);
	//float D1_a = D2_12*a0 + D2_02*a1 + D2_10*a2;
	nmppsMul_32f(D2_10, a2, buf, srcCountEven);
	nmppsMul_Add_32f(D2_02, a1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(D2_12, a0, buf1, D1_a, srcCountEven);
	
	//float A1_s = A2_12*s0 + A2_02*s1 + A2_10*s2;
	nmppsMul_32f(A2_10, s2, buf, srcCountEven);	
	nmppsMul_Add_32f(A2_02, s1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(A2_12, s0, buf1, A1_s, srcCountEven);
	//float B1_s = B2_12*s0 + B2_02*s1 + B2_10*s2;
	nmppsMul_32f(B2_10, s2, buf, srcCountEven);	
	nmppsMul_Add_32f(B2_02, s1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(B2_12, s0, buf1, B1_s, srcCountEven);
	//float D1_s = D2_12*s0 + D2_02*s1 + D2_10*s2;
	nmppsMul_32f(D2_10, s2, buf, srcCountEven);
	nmppsMul_Add_32f(D2_02, s1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(D2_12, s0, buf1, D1_s, srcCountEven);
	
	//float A1_t = A2_12*t0 + A2_02*t1 + A2_10*t2;
	nmppsMul_32f(A2_10, t2, buf, srcCountEven);	
	nmppsMul_Add_32f(A2_02, t1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(A2_12, t0, buf1, A1_t, srcCountEven);
	//float B1_t = B2_12*t0 + B2_02*t1 + B2_10*t2;
	nmppsMul_32f(B2_10, t2, buf, srcCountEven);	
	nmppsMul_Add_32f(B2_02, t1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(B2_12, t0, buf1, B1_t, srcCountEven);
	//float D1_t = D2_12*t0 + D2_02*t1 + D2_10*t2;
	nmppsMul_32f(D2_10, t2, buf, srcCountEven);
	nmppsMul_Add_32f(D2_02, t1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(D2_12, t0, buf1, D1_t, srcCountEven);

	//float A1_z = A2_12*z0 + A2_02*z1 + A2_10*z2;
	nmppsMul_32f(A2_10, z2, buf, srcCountEven);	
	nmppsMul_Add_32f(A2_02, z1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(A2_12, z0, buf1, A1_z, srcCountEven);
	//float B1_z = B2_12*z0 + B2_02*z1 + B2_10*z2;
	nmppsMul_32f(B2_10, z2, buf, srcCountEven);	
	nmppsMul_Add_32f(B2_02, z1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(B2_12, z0, buf1, B1_z, srcCountEven);
	//float D1_z = D2_12*z0 + D2_02*z1 + D2_10*z2;
	nmppsMul_32f(D2_10, z2, buf, srcCountEven);
	nmppsMul_Add_32f(D2_02, z1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(D2_12, z0, buf1, D1_z, srcCountEven);

	//float A1_w = A2_12*w0 + A2_02*w1 + A2_10*w2;
	nmppsMul_32f(A2_10, w2, buf, srcCountEven);	
	nmppsMul_Add_32f(A2_02, w1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(A2_12, w0, buf1, A1_w, srcCountEven);
	//float B1_w = B2_12*w0 + B2_02*w1 + B2_10*w2;
	nmppsMul_32f(B2_10, w2, buf, srcCountEven);	
	nmppsMul_Add_32f(B2_02, w1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(B2_12, w0, buf1, B1_w, srcCountEven);
	//float D1_w = D2_12*w0 + D2_02*w1 + D2_10*w2;
	nmppsMul_32f(D2_10, w2, buf, srcCountEven);
	nmppsMul_Add_32f(D2_02, w1, buf, buf1, srcCountEven);
	nmppsMul_Add_32f(D2_12, w0, buf1, D1_w, srcCountEven);
	/*****************************************************************/
#else //PERSPECTIVE_CORRECT
	for (size_t kk = 0; kk < NMGL_SIZE; ++kk){
		ones[kk] = 1.0f;
	}

	//float A0 = y2-y1;
	//float A1 = y0-y2;
	//float A2 = y1-y0;
	nmppsSub_32f(y2, y1, buf,  srcCountEven);
	nmppsSub_32f(y0, y2, buf1, srcCountEven);
	nmppsSub_32f(y1, y0, buf2, srcCountEven);
	nmblas_scopy(srcCountEven, buf,  1, A0, 1);
	nmblas_scopy(srcCountEven, buf1, 1, A1, 1);
	nmblas_scopy(srcCountEven, buf2, 1, A2, 1);

	//float B0 = x1-x2;
	//float B1 = x2-x0;
	//float B2 = x0-x1;
	nmppsSub_32f(x1, x2, buf,  srcCountEven);
	nmppsSub_32f(x2, x0, buf1, srcCountEven);
	nmppsSub_32f(x0, x1, buf2, srcCountEven);
	nmblas_scopy(srcCountEven, buf,  1, B0, 1);
	nmblas_scopy(srcCountEven, buf1, 1, B1, 1);
	nmblas_scopy(srcCountEven, buf2, 1, B2, 1);

	//float D0 = x2*y1-y2*x1;
	//float D1 = y2*x0-x2*y0;
	//float D2 = x1*y0-x0*y1;
	nmppsMul_Mul_Sub_32f(x2, y1, y2, x1, buf,  srcCountEven);	
	nmppsMul_Mul_Sub_32f(y2, x0, x2, y0, buf1, srcCountEven);	
	nmppsMul_Mul_Sub_32f(x1, y0, x0, y1, buf2, srcCountEven);	
	nmblas_scopy(srcCountEven, buf,  1, D0, 1);
	nmblas_scopy(srcCountEven, buf1, 1, D1, 1);
	nmblas_scopy(srcCountEven, buf2, 1, D2, 1);

	//float oneOverTriSquare = 1.0/((x0-x1)*(y2-y1)-(y0-y1)*(x2-x1));
	nm32f *A0_imu  = cntxt->buffer3 + 0 * NMGL_SIZE;
	nm32f *A2_imu  = cntxt->buffer3 + 1 * NMGL_SIZE;
	nm32f *B0_imu  = cntxt->buffer3 + 2 * NMGL_SIZE;
	nm32f *B2_imu  = cntxt->buffer3 + 3 * NMGL_SIZE;
	nmblas_scopy(srcCountEven, A0, 1, A0_imu, 1);
	nmblas_scopy(srcCountEven, A2, 1, A2_imu, 1);
	nmblas_scopy(srcCountEven, B0, 1, B0_imu, 1);
	nmblas_scopy(srcCountEven, B2, 1, B2_imu, 1);
	nmppsMul_Mul_Sub_32f(B2_imu, A0_imu, A2_imu, B0_imu, buf, srcCountEven);
	nmppsDiv_32f(ones, buf, buf1, srcCountEven);
	nmblas_scopy(srcCountEven, buf1, 1, oneOverTriSquare, 1);

	nm32f *ABD[][3] = {
		{A0, A1, A2},
		{B0, B1, B2},
		{D0, D1, D2},
	};

	nm32f *ABD_[][3] = {
		{A_r, B_r, D_r},
		{A_g, B_g, D_g},
		{A_b, B_b, D_b},
		{A_a, B_a, D_a},
		{A_s, B_s, D_s},
		{A_t, B_t, D_t},
		{A_z, B_z, D_z},
		{A_w, B_w, D_w},
	};

	nm32f *c[][3] = {
		{r0, r1, r2},
		{g0, g1, g2},
		{b0, b1, b2},
		{a0, a1, a2},
		{s0, s1, s2},
		{t0, t1, t2},
		{z0, z1, z2},
		{w0, w1, w2},
	};

	// For c = [r, g, b, a, s, t, z, w] calc N = [A, B, D]:
		//float N_c = oneOverTriSquare*(N0*c0 + N1*c1 + N2*c2);
	// Example for A_r (the same is for others)
		//float A_r = oneOverTriSquare*(A0*r0 + A1*r1 + A2*r2);
		//nmppsMul_32f(A2, r2, buf, srcCountEven);	
		//nmppsMul_Add_32f(A1, r1, buf, buf1, srcCountEven);
		//nmppsMul_Add_32f(A0, r0, buf1, buf2, srcCountEven);
		//nmppsMul_32f(oneOverTriSquare, buf2, A_r, srcCountEven);
	for (int i = 0; i < (sizeof(ABD_) / sizeof(ABD_[0])); ++i){
		nm32f *c0 = cntxt->buffer3 +  0 * NMGL_SIZE;
		nm32f *c1 = cntxt->buffer3 +  1 * NMGL_SIZE;;
		nm32f *c2 = cntxt->buffer3 +  2 * NMGL_SIZE;;
		nmblas_scopy(srcCountEven, c[i][0], 1, c0, 1);
		nmblas_scopy(srcCountEven, c[i][1], 1, c1, 1);
		nmblas_scopy(srcCountEven, c[i][2], 1, c2, 1);
		for (int j = 0; j < 3; ++j){
			nm32f *N0  = cntxt->buffer3 +  3 * NMGL_SIZE;
			nm32f *N1  = cntxt->buffer3 +  4 * NMGL_SIZE;
			nm32f *N2  = cntxt->buffer3 +  5 * NMGL_SIZE;
			nm32f *N_c = cntxt->buffer3 +  6 * NMGL_SIZE;
			nmblas_scopy(srcCountEven, ABD[j][0], 1, N0, 1);
			nmblas_scopy(srcCountEven, ABD[j][1], 1, N1, 1);
			nmblas_scopy(srcCountEven, ABD[j][2], 1, N2, 1);
			nmblas_scopy(srcCountEven, ABD_[i][j], 1, N_c, 1);

			nmppsMul_32f(N2, c2, buf, srcCountEven);	
			nmppsMul_Add_32f(N1, c1, buf, buf1, srcCountEven);
			nmppsMul_Add_32f(N0, c0, buf1, buf2, srcCountEven);
			nmppsMul_32f(oneOverTriSquare, buf2, N_c, srcCountEven);

			nmblas_scopy(srcCountEven, N_c, 1, ABD_[i][j], 1);
		}
	}
#endif //PERSPECTIVE_CORRECT

	// Analyze triangles
	// Use masks here	
	int i = *srcTreatedCount;
	for (i = *srcTreatedCount; i < srcCount; ++i){
		if (width[i] > maxWidth || height[i] > maxHeight){
			if ((numberOfOutputTrs[i] + dstSize) <= maxDstSize){	// There is space in output buffer

				int k = ((int)nf[i] + 1) + ((int)nf[i] + 1) % 2;

				/***** Calculate x and y *****/

				// init x: 0, 1, 2, 3, ..., (nf - 1), Cx
				// init y: 0, 1, 2, 3, ..., (nf - 1), Cy
				for (int j = 0; j < (int)nf[i]; ++j){
					x[j] = y[j] = (float) j;
				} 

				// Calc the first row
				// x = {Ax + ac_dx * x[0:nf - 1], Cx}
				// y = {Ay + ac_dy * y[0:nf - 1], Cy}
				nmblas_scopy(k, x, 1, buf, 1);
				nmppsMulC_AddC_32f(x, ac_dx[i], srcTriangles->v0.x[i], buf1, k);
				nmblas_scopy(k, buf1, 1, x, 1);

				nmblas_scopy(k, y, 1, buf, 1);
				nmppsMulC_AddC_32f(y, ac_dy[i], srcTriangles->v0.y[i], buf1, k);
				nmblas_scopy(k, buf1, 1, y, 1);
				x[(int)nf[i]] = srcTriangles->v2.x[i];
				y[(int)nf[i]] = srcTriangles->v2.y[i];

				// Build x and y matrices
				nm32f *src = x;
				nm32f *dst = src + k;
				for (int l = 0; l < k - 1; ++l){
					nmblas_scopy(k, src, 1, buf, 1);
					nmppsAddC_32f(buf, buf1, ab_dx[i], k);
					nmblas_scopy(k, buf1, 1, dst, 1);
					src += k;
					dst += k;
				}
				src = y;
				dst = src + k;
				for (int l = 0; l < k - 1; ++l){
					nmblas_scopy(k, src, 1, buf, 1);
					nmppsAddC_32f(buf, buf1, ab_dy[i], k);
					nmblas_scopy(k, buf1, 1, dst, 1);
					src += k;
					dst += k;
				}

				/***** Calculate r, g, b, a, s, t, z, w *****/

#ifdef PERSPECTIVE_CORRECT
				int primWidth = k * k;
				//float oneOverDenominator = 1 / (A2*xf + B2*yf + D2);
				nmppsMulC_AddC_32f(y, B2[i], D2[i], buf, primWidth);
				nmppsMulC_AddV_32f(x, buf, buf, A2[i], primWidth);
				nmppsDiv_32f(ones, buf, oneOverDenominator, primWidth);

				//float r = (A1_r*xf + B1_r*yf + D1_r) * oneOverDenominator;
				nmppsMulC_AddC_32f(y, B1_r[i], D1_r[i], buf, primWidth);
				nmppsMulC_AddV_32f(x, buf, r, A1_r[i], primWidth);
				nmppsMul_AddC_32f(r, oneOverDenominator, 0.0, r, primWidth);

				//float g = (A1_g*xf + B1_g*yf + D1_g) * oneOverDenominator;
				nmppsMulC_AddC_32f(y, B1_g[i], D1_g[i], buf, primWidth);
				nmppsMulC_AddV_32f(x, buf, g, A1_g[i], primWidth);
				nmppsMul_AddC_32f(g, oneOverDenominator, 0.0, g, primWidth);

				//float b = (A1_b*xf + B1_b*yf + D1_b) * oneOverDenominator;
				nmppsMulC_AddC_32f(y, B1_b[i], D1_b[i], buf, primWidth);
				nmppsMulC_AddV_32f(x, buf, b, A1_b[i], primWidth);
				nmppsMul_AddC_32f(b, oneOverDenominator, 0.0, b, primWidth);

				//float a = (A1_a*xf + B1_a*yf + D1_a) * oneOverDenominator;
				nmppsMulC_AddC_32f(y, B1_a[i], D1_a[i], buf, primWidth);
				nmppsMulC_AddV_32f(x, buf, a, A1_a[i], primWidth);
				nmppsMul_AddC_32f(a, oneOverDenominator, 0.0, a, primWidth);

				//float s = (A1_s*xf + B1_s*yf + D1_s) * oneOverDenominator;
				nmppsMulC_AddC_32f(y, B1_s[i], D1_s[i], buf, primWidth);
				nmppsMulC_AddV_32f(x, buf, s, A1_s[i], primWidth);
				nmppsMul_AddC_32f(s, oneOverDenominator, 0.0, s, primWidth);

				//float t = (A1_t*xf + B1_t*yf + D1_t) * oneOverDenominator;
				nmppsMulC_AddC_32f(y, B1_t[i], D1_t[i], buf, primWidth);
				nmppsMulC_AddV_32f(x, buf, t, A1_t[i], primWidth);
				nmppsMul_AddC_32f(t, oneOverDenominator, 0.0, t, primWidth);

				//float z = (A1_z*xf + B1_z*yf + D1_z) * oneOverDenominator;
				nmppsMulC_AddC_32f(y, B1_z[i], D1_z[i], buf, primWidth);
				nmppsMulC_AddV_32f(x, buf, z, A1_z[i], primWidth);
				nmppsMul_AddC_32f(z, oneOverDenominator, 0.0, z, primWidth);

				//float w = (A1_w*xf + B1_w*yf + D1_w) * oneOverDenominator;
				nmppsMulC_AddC_32f(y, B1_w[i], D1_w[i], buf, primWidth);
				nmppsMulC_AddV_32f(x, buf, w, A1_w[i], primWidth);
				nmppsMul_AddC_32f(w, oneOverDenominator, 0.0, w, primWidth);
#else //PERSPECTIVE_CORRECT                    
				int primWidth = k * k;
				int startAttr = 4;	// Skip r, g, b, a
				if (NMGL_SMOOTH == cntxt->shadeModel) {
					startAttr = 0;	// Start from the beginning (r, g, b, a, ...)
				}
				else {
					nmppsMulC_AddC_32f(buf, 0, r2[i], buf1, primWidth);
					nmblas_scopy(primWidth, buf1, 1, r, 1);
					
					nmppsMulC_AddC_32f(buf, 0, g2[i], buf1, primWidth);
					nmblas_scopy(primWidth, buf1, 1, g, 1);

					nmppsMulC_AddC_32f(buf, 0, b2[i], buf1, primWidth);
					nmblas_scopy(primWidth, buf1, 1, b, 1);

					nmppsMulC_AddC_32f(buf, 0, a2[i], buf1, primWidth);
					nmblas_scopy(primWidth, buf1, 1, a, 1);
				}

				nm32f *attr[] = {r, g, b, a, s, t, z, w};
				// Process attributes partially in a loop. 
				// There may be no space for the full x (y) in a buf1 (buf2)
				for (int j = startAttr; j < 8; ++j){
					//attr = A_attr*x + B_attr*y + D_attr;
					nm32f *A_ = ABD_[j][0];
					nm32f *B_ = ABD_[j][1];
					nm32f *D_ = ABD_[j][2];
					int cnt = 0;
					while (cnt < primWidth){
						int kk = ((primWidth - cnt) >= NMGL_SIZE)? NMGL_SIZE: (primWidth - cnt);
						nmblas_scopy(kk, y + cnt, 1, buf1, 1);	
						nmblas_scopy(kk, x + cnt, 1, buf2, 1);
						nmppsMulC_AddC_32f(buf1, B_[i], D_[i], buf, kk);
						nmppsMulC_AddV_32f(buf2, buf,   buf1, A_[i], kk);
						nmblas_scopy(kk, buf1, 1, attr[j] + cnt, 1);
						cnt += kk;
					}
				}
#endif //PERSPECTIVE_CORRECT

				int inc = pushPoints(x, y, z, w, r, g, b, a, s, t, dstTriangles, dstSize, (int) nf[i]);
				dstSize += inc;
			} else {
				// There is no space in output buffer
				// Stop splitting
				break;
			}
		} else {
			if (dstSize < maxDstSize){
				// Triangle doesn't need division
				dstTriangles->v0.x[dstSize] = srcTriangles->v0.x[i];
				dstTriangles->v0.y[dstSize] = srcTriangles->v0.y[i];
				dstTriangles->v0.z[dstSize] = srcTriangles->v0.z[i];
				dstTriangles->v0.w[dstSize] = srcTriangles->v0.w[i];
				dstTriangles->v0.s[dstSize] = srcTriangles->v0.s[i];
				dstTriangles->v0.t[dstSize] = srcTriangles->v0.t[i];
				dstTriangles->v1.x[dstSize] = srcTriangles->v1.x[i];
				dstTriangles->v1.y[dstSize] = srcTriangles->v1.y[i];
				dstTriangles->v1.z[dstSize] = srcTriangles->v1.z[i];
				dstTriangles->v1.w[dstSize] = srcTriangles->v1.w[i];
				dstTriangles->v1.s[dstSize] = srcTriangles->v1.s[i];
				dstTriangles->v1.t[dstSize] = srcTriangles->v1.t[i];
				dstTriangles->v2.x[dstSize] = srcTriangles->v2.x[i];
				dstTriangles->v2.y[dstSize] = srcTriangles->v2.y[i];
				dstTriangles->v2.z[dstSize] = srcTriangles->v2.z[i];
				dstTriangles->v2.w[dstSize] = srcTriangles->v2.w[i];
				dstTriangles->v2.s[dstSize] = srcTriangles->v2.s[i];
				dstTriangles->v2.t[dstSize] = srcTriangles->v2.t[i];

				dstTriangles->v0.color[dstSize] = srcTriangles->v0.color[i];
				dstTriangles->v1.color[dstSize] = srcTriangles->v1.color[i];
				dstTriangles->v2.color[dstSize] = srcTriangles->v2.color[i];

				dstSize += 1;
			} else {
				break;
			}
		}
	}
	*srcTreatedCount = i;
	return dstSize;
}

static void unpackColors(const v4nm32f *color, float *rr, float *gg, float *bb, float *aa, int size)
{
	for (int i = 0; i < size; ++i){
		rr[i] = color[i].vec[0];
		gg[i] = color[i].vec[1];
		bb[i] = color[i].vec[2];
		aa[i] = color[i].vec[3];
	}
}

static int pushPoints(
		const nm32f *x, const nm32f *y, const nm32f *z, const nm32f *w,
		const nm32f *r, const nm32f *g, const nm32f *b, const nm32f *a, 
		const nm32f *s, const nm32f *t,
		TrianglePointers *dst, 
		int offset, 
		int n)
{
	int k = (n + 1) + (n + 1) % 2;
	int dsti = 0;

	// Extract the first set of triangles
	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n - i; ++j, dsti++){
			dst->v0.x[offset + dsti] = x[i * k + j];
			dst->v0.y[offset + dsti] = y[i * k + j];
			dst->v0.z[offset + dsti] = z[i * k + j];
			dst->v0.w[offset + dsti] = w[i * k + j];
			dst->v0.s[offset + dsti] = s[i * k + j];
			dst->v0.t[offset + dsti] = t[i * k + j];
			dst->v1.x[offset + dsti] = x[(i + 1) * k + j];
			dst->v1.y[offset + dsti] = y[(i + 1) * k + j];
			dst->v1.z[offset + dsti] = z[(i + 1) * k + j];
			dst->v1.w[offset + dsti] = w[(i + 1) * k + j];
			dst->v1.s[offset + dsti] = s[(i + 1) * k + j];
			dst->v1.t[offset + dsti] = t[(i + 1) * k + j];
		    dst->v2.x[offset + dsti] = x[i * k + (j + 1)];
			dst->v2.y[offset + dsti] = y[i * k + (j + 1)];
			dst->v2.z[offset + dsti] = z[i * k + (j + 1)];
			dst->v2.w[offset + dsti] = w[i * k + (j + 1)];
			dst->v2.s[offset + dsti] = s[i * k + (j + 1)];
			dst->v2.t[offset + dsti] = t[i * k + (j + 1)];

			dst->v0.color[offset + dsti] = {r[i * k + j],
											g[i * k + j],
											b[i * k + j],
											a[i * k + j]};
			dst->v1.color[offset + dsti] = {r[(i + 1) * k + j],
											g[(i + 1) * k + j],
											b[(i + 1) * k + j],
											a[(i + 1) * k + j]};
		    dst->v2.color[offset + dsti] = {r[i * k + (j + 1)],
											g[i * k + (j + 1)],
											b[i * k + (j + 1)],
											a[i * k + (j + 1)]};
		}
	}
	// Extract the second set of triangles
	// continue with dsti
	for (int i = 1; i < n; ++i){
		for (int j = 0; j < n - i; ++j, dsti++){
			dst->v0.x[offset + dsti] = x[i * k + j];
			dst->v0.y[offset + dsti] = y[i * k + j];
			dst->v0.z[offset + dsti] = z[i * k + j];
			dst->v0.w[offset + dsti] = w[i * k + j];
			dst->v0.s[offset + dsti] = s[i * k + j];
			dst->v0.t[offset + dsti] = t[i * k + j];
			dst->v1.x[offset + dsti] = x[i * k + (j + 1)];
			dst->v1.y[offset + dsti] = y[i * k + (j + 1)];
			dst->v1.z[offset + dsti] = z[i * k + (j + 1)];
			dst->v1.w[offset + dsti] = w[i * k + (j + 1)];
			dst->v1.s[offset + dsti] = s[i * k + (j + 1)];
			dst->v1.t[offset + dsti] = t[i * k + (j + 1)];
			dst->v2.x[offset + dsti] = x[(i - 1) * k + (j + 1)];
			dst->v2.y[offset + dsti] = y[(i - 1) * k + (j + 1)];
			dst->v2.z[offset + dsti] = z[(i - 1) * k + (j + 1)];
			dst->v2.w[offset + dsti] = w[(i - 1) * k + (j + 1)];
			dst->v2.s[offset + dsti] = s[(i - 1) * k + (j + 1)];
			dst->v2.t[offset + dsti] = t[(i - 1) * k + (j + 1)];

			dst->v0.color[offset + dsti] = {r[i * k + j],
											g[i * k + j],
											b[i * k + j],
											a[i * k + j]};
			dst->v1.color[offset + dsti] = {r[i * k + (j + 1)],
											g[i * k + (j + 1)],
											b[i * k + (j + 1)],
											a[i * k + (j + 1)]};
		    dst->v2.color[offset + dsti] = {r[(i - 1) * k + (j + 1)],
											g[(i - 1) * k + (j + 1)],
											b[(i - 1) * k + (j + 1)],
											a[(i - 1) * k + (j + 1)]};
		}
	}
	return dsti;
}

