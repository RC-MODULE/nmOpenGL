#include "demo3d_nm0.h"
#include "nmblas.h"
#include "service.h"
#include "filter.h"

#define PERSPECTIVE_CORRECT

static void unpackColors(const v4nm32f *color, float *rr, float *gg, float *bb, float *aa, int size);
static  int pushPoints(const nm32f *x, const nm32f *y, const nm32f *z, const nm32f *w, 
						const nm32f *r, const nm32f *g, const nm32f *b, const nm32f *a, 
						const nm32f *s, const nm32f *t, 
						TrianglePointers *dst, int offset, int n);

// PERSPECTIVE_CORRECT arrays
SECTION(".data") static float oneOverDenominator[NMGL_SIZE];
SECTION(".data") static float A[NMGL_SIZE];
SECTION(".data") static float B[NMGL_SIZE];
SECTION(".data") static float D[NMGL_SIZE];

// NOT PERSPECTIVE_CORRECT arrays
SECTION(".data") static float oneOverTriSquare[NMGL_SIZE];

// Common arrays
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

	for (size_t kk = 0; kk < NMGL_SIZE; ++kk){
		ones[kk] = 1.0f;
	}

#ifdef PERSPECTIVE_CORRECT        
	// Calculate some coefficients to interpolate attribute values.
	// Formulas obtained by simplifying formulas for barycentric coordinates.

	nmppsMul_32f(z1, z2, buf2, srcCountEven);	// Calc z12 = z1 * z2
	// A0 = z12*(y2 - y1);
	nmppsSub_32f(y2, y1, buf, srcCountEven);
	nmppsMul_32f(buf2, buf, buf1, srcCountEven);	
	nmblas_scopy(srcCountEven, buf1, 1, A0, 1);
	// B0 = -z12*(x2 - x1) = z12*(x1 - x2);
	nmppsSub_32f(x1, x2, buf, srcCountEven);
	nmppsMul_32f(buf2, buf, buf1, srcCountEven);	
	nmblas_scopy(srcCountEven, buf1, 1, B0, 1);
	// D0 = z12*(x2*y1 - x1*y2);
	nmppsMul_Mul_Sub_32f(x2, y1, x1, y2, buf, srcCountEven);
	nmppsMul_32f(buf2, buf, buf1, srcCountEven);	
	nmblas_scopy(srcCountEven, buf1, 1, D0, 1);

	nmppsMul_32f(z0, z2, buf2, srcCountEven);	// Calc z02 = z0 * z2
	// A1 = z02*(y0 - y2);
	nmppsSub_32f(y0, y2, buf, srcCountEven);
	nmppsMul_32f(buf2, buf, buf1, srcCountEven);	
	nmblas_scopy(srcCountEven, buf1, 1, A1, 1);
	// B1 = -z02*(x0 - x2) = z02*(x2 - x0);
	nmppsSub_32f(x2, x0, buf, srcCountEven);
	nmppsMul_32f(buf2, buf, buf1, srcCountEven);	
	nmblas_scopy(srcCountEven, buf1, 1, B1, 1);
	// D1 = z02*(x0*y2 - x2*y0);
	nmppsMul_Mul_Sub_32f(x0, y2, x2, y0, buf, srcCountEven);
	nmppsMul_32f(buf2, buf, buf1, srcCountEven);	
	nmblas_scopy(srcCountEven, buf1, 1, D1, 1);

	nmppsMul_32f(z1, z0, buf2, srcCountEven);	// Calc z10 = z1 * z0
	// A2 = z10*(y1 - y0);
	nmppsSub_32f(y1, y0, buf, srcCountEven);
	nmppsMul_32f(buf2, buf, buf1, srcCountEven);	
	nmblas_scopy(srcCountEven, buf1, 1, A2, 1);
	// B2 = -z10*(x1 - x0) = z10*(x0 - x1);
	nmppsSub_32f(x0, x1, buf, srcCountEven);
	nmppsMul_32f(buf2, buf, buf1, srcCountEven);	
	nmblas_scopy(srcCountEven, buf1, 1, B2, 1);
	// D2 = z10*(x1*y0 - x0*y1);
	nmppsMul_Mul_Sub_32f(x1, y0, x0, y1, buf, srcCountEven);
	nmppsMul_32f(buf2, buf, buf1, srcCountEven);	
	nmblas_scopy(srcCountEven, buf1, 1, D2, 1);

	//A = A0 + A1 + A2;
	nm32f *A0_imu = cntxt->buffer3 +  0 * NMGL_SIZE;
	nm32f *A1_imu = cntxt->buffer3 +  1 * NMGL_SIZE;;
	nm32f *A2_imu = cntxt->buffer3 +  2 * NMGL_SIZE;;
	nmblas_scopy(srcCountEven, A0, 1, A0_imu, 1);
	nmblas_scopy(srcCountEven, A1, 1, A1_imu, 1);
	nmblas_scopy(srcCountEven, A2, 1, A2_imu, 1);
	nmppsAdd_32f(A0_imu, A1_imu, buf, srcCountEven);
	nmppsAdd_32f(  buf, A2_imu,  buf1, srcCountEven);
	nmblas_scopy(srcCountEven, buf1, 1, A, 1);
	//B = B0 + B1 + B2;
	nm32f *B0_imu = cntxt->buffer3 +  0 * NMGL_SIZE;
	nm32f *B1_imu = cntxt->buffer3 +  1 * NMGL_SIZE;;
	nm32f *B2_imu = cntxt->buffer3 +  2 * NMGL_SIZE;;
	nmblas_scopy(srcCountEven, B0, 1, B0_imu, 1);
	nmblas_scopy(srcCountEven, B1, 1, B1_imu, 1);
	nmblas_scopy(srcCountEven, B2, 1, B2_imu, 1);
	nmppsAdd_32f(B0_imu, B1_imu, buf, srcCountEven);
	nmppsAdd_32f(  buf, B2_imu,  buf1, srcCountEven);
	nmblas_scopy(srcCountEven, buf1, 1, B, 1);
	//D = D0 + D1 + D2;
	nm32f *D0_imu = cntxt->buffer3 +  0 * NMGL_SIZE;
	nm32f *D1_imu = cntxt->buffer3 +  1 * NMGL_SIZE;;
	nm32f *D2_imu = cntxt->buffer3 +  2 * NMGL_SIZE;;
	nmblas_scopy(srcCountEven, D0, 1, D0_imu, 1);
	nmblas_scopy(srcCountEven, D1, 1, D1_imu, 1);
	nmblas_scopy(srcCountEven, D2, 1, D2_imu, 1);
	nmppsAdd_32f(D0_imu, D1_imu, buf, srcCountEven);
	nmppsAdd_32f(  buf, D2_imu,  buf1, srcCountEven);
	nmblas_scopy(srcCountEven, buf1, 1, D, 1);

#else //PERSPECTIVE_CORRECT

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

#endif //PERSPECTIVE_CORRECT
	
	// For c = [r, g, b, a, s, t, z, w] and N = [A, B, D] calc N_c:
	// 1. Perspective correct interpolation
		//float N_c = N0 * c0 + N1 * c1 + N2 * c2
		// EXAMPLE for A_r (the same is for others)
			//float A_r = A0 * r0 + A1 * r1 + A2 * r2;
			//nmppsMul_32f(A2, r2, buf, srcCountEven);
			//nmppsMul_Add_32f(A1, r1, buf, buf1, srcCountEven);
			//nmppsMul_Add_32f(A0, r0, buf1, A_r, srcCountEven);
	// 2. Not perspective correct interpolation
		//float N_c = (N0 * c0 + N1 * c1 + N2 * c2) * oneOverTriSquare;
		// EXAMPLE For A_r (the same is for others)
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

			nmppsMul_32f(N2, c2, buf, srcCountEven);	
			nmppsMul_Add_32f(N1, c1, buf, buf1, srcCountEven);

#ifdef PERSPECTIVE_CORRECT
			nmppsMul_Add_32f(N0, c0, buf1, N_c, srcCountEven);
#else	//PERSPECTIVE_CORRECT
			nmppsMul_Add_32f(N0, c0, buf1, buf2, srcCountEven);
			nmppsMul_32f(oneOverTriSquare, buf2, N_c, srcCountEven);
#endif	//PERSPECTIVE_CORRECT

			nmblas_scopy(srcCountEven, N_c, 1, ABD_[i][j], 1);
		}
	}

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
				// Calculate attr = [r, g, b, a, s, t, z, w] for primWidth points
				// 1. For perspective correct interpolation:
				//    attr = (A_attr*x + B_attr*y + D_attr) * oneOverDenominator;
				// 2. For non perspective correct interpolation:
				//    attr = A_attr*x + B_attr*y + D_attr;

				int primWidth = k * k;
				
				// If shadeModel is FLAT then don't calculate r, g, b, a, 
				// just copy the values of the third triangle's vertex 
				// (see OpenGL 1.3 spec, ch. 2.13.7)

				int startAttr = 0;	// Process all the attributes by default

				if (NMGL_FLAT == cntxt->shadeModel) {
					// Set r, g, b, a in parts
					int cnt = 0;
					while (cnt < primWidth){
						int kk = ((primWidth - cnt) >= NMGL_SIZE)? NMGL_SIZE: (primWidth - cnt);

						nmppsMulC_AddC_32f(buf, 0, r2[i], buf1, kk);
						nmblas_scopy(kk, buf1, 1, r + cnt, 1);

						nmppsMulC_AddC_32f(buf, 0, g2[i], buf1, kk);
						nmblas_scopy(kk, buf1, 1, g + cnt, 1);

						nmppsMulC_AddC_32f(buf, 0, b2[i], buf1, kk);
						nmblas_scopy(kk, buf1, 1, b + cnt, 1);

						nmppsMulC_AddC_32f(buf, 0, a2[i], buf1, kk);
						nmblas_scopy(kk, buf1, 1, a + cnt, 1);

						cnt += kk;
					}

					// Skip r, g, b, a. 
					// Process other attributes starting from the s 
					startAttr = 4;	
				}
				else {
					// Process all the attributes starting from the beginning
					// i.e. keep startAttr equal to 0 (by default)
				}

#ifdef PERSPECTIVE_CORRECT
				// float oneOverDenominator = 1 / (A * x + B * y + D);
				nmblas_scopy(primWidth, y, 1, buf1, 1);	
				nmblas_scopy(primWidth, x, 1, buf2, 1);
				nmppsMulC_AddC_32f(buf1, B[i], D[i], buf, primWidth);
				nmppsMulC_AddV_32f(buf2, buf, buf1, A[i], primWidth);
				nmppsDiv_32f(ones, buf1, buf2, primWidth);
				nmblas_scopy(primWidth, buf2, 1, oneOverDenominator, 1);
#endif // PERSPECTIVE_CORRECT

				nm32f *attr[] = {r, g, b, a, s, t, z, w};
				// Process attributes in parts within a loop. 
				// There may be no space for the full x (y) in a buf1 (buf2)
				for (int j = startAttr; j < 8; ++j){
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
#ifdef PERSPECTIVE_CORRECT
						nmblas_scopy(kk, oneOverDenominator, 1, buf, 1);//Get oneOverDenominator
						nmppsMul_32f(buf1, buf, buf2, kk);
						nmblas_scopy(kk, buf2, 1, attr[j] + cnt, 1);
#else 	// PERSPECTIVE_CORRECT
						nmblas_scopy(kk, buf1, 1, attr[j] + cnt, 1);
#endif	// PERSPECTIVE_CORRECT
						cnt += kk;
					}
				}

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

