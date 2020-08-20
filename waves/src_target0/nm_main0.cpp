#include "nmgl.h"
#include "hal.h"
#include "nmglvs_nmc0.h"
#include "stdio.h"
#include "phillips.h"
#include "led.h"
#include "hal.h"
#include "float.h"

#include "nmpp.h"
#include <cstring>
#include <cmath>
#include "hal_target.h"
#include "dma.h"

#ifndef __OPEN_GL__

#endif


#define			STR_SIZE			256
#define			COL_SIZE			256
#define			FRAME_SIZE			STR_SIZE * COL_SIZE

struct Vertex {
	float x;
	float y;
	float z;
	float w;
};

struct Triangle {
	Vertex a;
	Vertex b;
	Vertex c;
};

struct Vector3f {
	float x;
	float y;
	float z;
};

SECTION(".data_shared0") float colorTriangles[4 * FRAME_SIZE];

SECTION(".data_imu1") nm32fcr h0_sram[STR_SIZE];
SECTION(".data_imu1") nm32f   re_im_sram[COL_SIZE];

SECTION(".data_imu1") nm32f   omega_sram[STR_SIZE];
SECTION(".data_imu1") nm32fcr h0_conj_sram[STR_SIZE];

SECTION(".data_imu5") nm32f   re_exp_sram[STR_SIZE];
SECTION(".data_imu5") nm32fcr fft_sram1[COL_SIZE];

SECTION(".data_imu4") nm32f   im_exp_sram[STR_SIZE];
SECTION(".data_imu4") nm32fcr h_sram[STR_SIZE];

SECTION(".data_imu5") nm32fcr exp_sram[STR_SIZE];
SECTION(".data_imu5") nm32f   omegat_sram[STR_SIZE];

SECTION(".data_imu6") nm32fcr fft_sram0[COL_SIZE];
SECTION(".data_imu6") nm32f   result_sram[COL_SIZE];

#define COUNT_TRIANGLES 2 * (STR_SIZE-1) * (COL_SIZE-1) 

SECTION(".data_DDR") nm32f    dispersion_ddr[FRAME_SIZE];
SECTION(".data_DDR") Triangle triangles[COUNT_TRIANGLES];
SECTION(".data_DDR") nm32fcr  h0_ddr[FRAME_SIZE];
SECTION(".data_DDR") nm32fcr  h0_conj_ddr[FRAME_SIZE];
SECTION(".data_DDR") nm32fcr  fft_ddr[FRAME_SIZE];
SECTION(".data_DDR") float    result_ddr[FRAME_SIZE];
SECTION(".data_DDR") float    result_ddr_color[12 * COUNT_TRIANGLES];
SECTION(".data_DDR") float    result_ddr_normal[12 * COUNT_TRIANGLES];

#ifdef __OPEN_GL__
Vector3f normal3[3 * COUNT_TRIANGLES];
#endif

extern "C" {
	void SumReImParts(const nm32fcr* complex_nums, nm32f* sum_re_im, int size);
}

void computeNormal(const Triangle* triangle, float* dstNormal, int size) {
	Vector3f BA;
	Vector3f CB;
	Vector3f normal;
	for (int i = 0; i < size; i++) {
		BA.x = triangle[i].a.x - triangle[i].b.x;
		BA.y = triangle[i].a.y - triangle[i].b.y;
		BA.z = triangle[i].a.z - triangle[i].b.z;
		CB.x = triangle[i].b.x - triangle[i].c.x;
		CB.y = triangle[i].b.y - triangle[i].c.y;
		CB.z = triangle[i].b.z - triangle[i].c.z;
		normal.x = (BA.y * CB.z - BA.z * CB.y);
		normal.y = (BA.z * CB.x - BA.x * CB.z);
		normal.z = (BA.x * CB.y - CB.x * BA.y);
		dstNormal[12 * i + 0] = normal.x;
		dstNormal[12 * i + 1] = normal.y;
		dstNormal[12 * i + 2] = normal.z;
		dstNormal[12 * i + 3] = 0;
		dstNormal[12 * i + 4] = normal.x;
		dstNormal[12 * i + 5] = normal.y;
		dstNormal[12 * i + 6] = normal.z;
		dstNormal[12 * i + 7] = 0;
		dstNormal[12 * i + 8] = normal.x;
		dstNormal[12 * i + 9] = normal.y;
		dstNormal[12 * i + 10] = normal.z;
		dstNormal[12 * i + 11] = 0;
	}
}

void halCopy_32s(void* srcVec, void* dstVec, int size) {
	int* src = (int*)srcVec;
	int* dst = (int*)dstVec;
	for (int i = 0; i < size; i++) {
		dst[i] = src[i];
	}
}

#define DY 0

void FillXy(Triangle* triangles, int width, int height) {
	int index = 0;
	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++, index += 2) {
			triangles[index].a.x = j;
			triangles[index].a.y = i + DY;
			triangles[index].a.w = 1;

			triangles[index].b.x = j + 1;
			triangles[index].b.y = i + DY;
			triangles[index].b.w = 1;

			triangles[index].c.x = j;
			triangles[index].c.y = i + 1 + DY;
			triangles[index].c.w = 1;

			triangles[index + 1].a.x = j + 1;
			triangles[index + 1].a.y = i + DY;
			triangles[index + 1].a.w = 1;

			triangles[index + 1].b.x = j + 1;
			triangles[index + 1].b.y = i + 1 + DY;
			triangles[index + 1].b.w = 1;

			triangles[index + 1].c.x = j;
			triangles[index + 1].c.y = i + 1 + DY;
			triangles[index + 1].c.w = 1;
		}
	}
}

void printMatrix() {
	double data[8];
	float* matrix = (float*)data;
	nmglGetFloatv(NMGL_MODELVIEW_MATRIX, matrix);
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			float var = matrix[4 * j + i];
			//float var = cntxt.modelviewMatrix[0].matr[4 * j + i];
			printf("%f ", var);
		}
		printf("\r\n");
	}
}

void FillZw(const float* z_mtr, Triangle* triangles, int width, int height)
{
	int index = 0;
	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++, index += 2) {
			triangles[index].a.z = z_mtr[i * height + j];
			//triangles[index].a.w = 1;

			triangles[index].b.z = z_mtr[i * height + j + 1];
			//triangles[index].b.w = 1;

			triangles[index].c.z = z_mtr[(i + 1) * height + j];
			//triangles[index].c.w = 1;

			triangles[index + 1].a.z = z_mtr[i * height + j + 1];
			//triangles[index + 1].a.w = 1;

			triangles[index + 1].b.z = z_mtr[(i + 1) * height + j + 1];
			//triangles[index + 1].b.w = 1;

			triangles[index + 1].c.z = z_mtr[(i + 1) * height + j];
			//triangles[index + 1].c.w = 1;
		}
	}
}


int main()
{
	nmglvsNm0Init();
	clock_t t0, t1;
	int timeClock = 0;

	FillXy(triangles, STR_SIZE, COL_SIZE);
#ifdef __OPEN_GL__
	computeOmega(dispersion_ddr, STR_SIZE, COL_SIZE);
	computeH0(h0_ddr, h0_conj_ddr, STR_SIZE, COL_SIZE);
#else
	halHostSync((int)dispersion_ddr); 			// send a frame address in DDR to PC
	halHostSync((int)h0_ddr);
	halHostSync((int)h0_conj_ddr);

	halHostSync(0x12300123);
#endif
	NmppsFFTSpec_32fcr* fft_coeffs;					// FFT variable
	int fftIsInitialized = FFTFwdInitAlloc_32fcr(&fft_coeffs, STR_SIZE);
#ifndef __OPEN_GL__
	if (fftIsInitialized == 0) {
		halHostSync(0x600D600D);
	}
	else {
		halHostSync(fftIsInitialized);
		return fftIsInitialized;
	}
#endif
	nmglClearColor(0, 0, 0.2f, 0.0f);

	
	nmglMatrixMode(NMGL_MODELVIEW);
	nmglTranslatef(-128, -128, 0);
	nmglRotatef(-60, 1, 0, 0);	
	nmglScalef(1, 1, 50);
	//nmglTranslatef(0, 100, 0);
	
	nmglMatrixMode(NMGL_PROJECTION);
	nmglOrthof(-128, 128, -128, 128, -200, 200);
	//nmglOrthof(-255, 256, -255, 256, -200, 200);
	nmglViewport(0, 0, 768, 768);

	nmglMatrixMode(NMGL_MODELVIEW);

	nmglEnable(NMGL_DEPTH_TEST);
	nmglEnable(NMGL_LIGHTING);
	nmglEnable(NMGL_LIGHT0);

	float diffuse[4] = { 1,1,1,1 };
	nmglLightfv(NMGL_LIGHT0, NMGL_DIFFUSE, diffuse);
	float position[4] = { 100, 100, 0, 0};
	nmglLightfv(NMGL_LIGHT0, NMGL_POSITION, position);
	float materialSpec[4] = { 0.6,0.6,0.6,1 };
	float materialDiffuse[4] = { 0,0,1,1 };
	nmglMaterialf(NMGL_FRONT_AND_BACK, NMGL_SHININESS, 15);
	nmglMaterialfv(NMGL_FRONT_AND_BACK, NMGL_DIFFUSE, materialDiffuse);
	nmglMaterialfv(NMGL_FRONT_AND_BACK, NMGL_SPECULAR, materialSpec);
	nmglEnable(NMGL_NORMALIZE);
	//nmglEnable(NMGL_RESCALE_NORMAL);
	//NMGLenum error = nmglGetError();

	nmglEnableClientState(NMGL_VERTEX_ARRAY);
	//nmglEnableClientState(NMGL_COLOR_ARRAY);
	nmglEnableClientState(NMGL_NORMAL_ARRAY);
	nmglVertexPointer(4, NMGL_FLOAT, 0, triangles);
#ifndef __OPEN_GL__
	nmglNormalPointerNM(NMGL_FLOAT, 0, result_ddr_normal);
#else
	nmglNormalPointer(NMGL_FLOAT, 0, normal3);
#endif
	nmglColorPointer(4, NMGL_FLOAT, 0, result_ddr_color);


	float time = 0;
	float invmax = (1 / 4.8) * 0.5;
	while(nmglvsNm0Run()){
		nmglClear(NMGL_COLOR_BUFFER_BIT | NMGL_DEPTH_BUFFER_BIT);

		for (int i = 0, offset = 0; i < STR_SIZE; i++, offset += STR_SIZE) {
			//memcpy((void*)omega_sram, (void*)(dispersion_ddr + offset), STR_SIZE);
			halCopy_32s((void*)(dispersion_ddr + offset), (void*)omega_sram, STR_SIZE);
			
			nmppsMulC_32f(omega_sram, omegat_sram, time, STR_SIZE);

			nmppsCos_32f(omegat_sram, re_exp_sram, STR_SIZE);
			nmppsSin_32f(omegat_sram, im_exp_sram, STR_SIZE);

			halCopy_32s((void*)(h0_conj_ddr + offset), (void*)h0_conj_sram, STR_SIZE << 1);

			//memcpy((void*)h0_conj_sram, (void*)(h0_conj_ddr + offset), STR_SIZE << 1);

			halCopy_32s((void*)(h0_ddr + offset), (void*)h0_sram, STR_SIZE << 1);
			//memcpy((void*)h0_sram, (void*)(h0_ddr + offset), STR_SIZE << 1);

			nmppsMerge_32f(re_exp_sram, im_exp_sram, (float*)exp_sram, STR_SIZE);

			nmppsMul_ConjMul_Add_32fcr(h0_sram, exp_sram, h0_conj_sram, exp_sram, h_sram, STR_SIZE);

			FFTFwd_32fcr(h_sram, fft_sram0, fft_coeffs, STR_SIZE);

			halCopy_32s((void*)fft_sram0, (void*)(fft_ddr + offset), STR_SIZE << 1);
			
			//memcpy((void*)(fft_ddr + offset), (void*)(fft_sram0), STR_SIZE << 1);
		}

		int columns = 0;
		int offs = 0;
		nm32f* tmp_buffer_sram = im_exp_sram;
		while (columns < COL_SIZE) {
			for (int i = 0, stride = 0; i < COL_SIZE; i++, stride += STR_SIZE) {
				halCopy_32s((void*)(fft_ddr + stride + columns), (void*)(fft_sram0 + i), 2);
				//memcpy((void*)(fft_sram0 + i), (void*)(fft_ddr + stride + columns), 2);
			}

			FFTFwd_32fcr(fft_sram0, fft_sram0, fft_coeffs, COL_SIZE);
            
			// Вычисление длины от комплексного z => |z| = sqrt(z.re^2 + z.im^2)
			nmppsMul_32f((nm32f*)fft_sram0, (nm32f*)fft_sram0, (nm32f*)fft_sram1, COL_SIZE << 1);
			SumReImParts(fft_sram1, re_im_sram, COL_SIZE << 1);
			nmppsSqrt_32f(re_im_sram, tmp_buffer_sram, result_sram, COL_SIZE);

			nmppsMulC_AddC_32f(tmp_buffer_sram, 0.1, 0.5, result_sram, COL_SIZE);
			halCopy_32s((void*)result_sram, (void*)(result_ddr + offs), COL_SIZE);
			//memcpy((void*)(result_ddr + offs), (void*)(result_sram), COL_SIZE);
			columns++;
			offs += COL_SIZE;
		}
		FillZw(result_ddr, triangles, STR_SIZE, COL_SIZE);
		time += 0.5;

		t0 = clock();
		computeNormal(triangles, result_ddr_normal, COUNT_TRIANGLES);
#ifdef __OPEN_GL__
		for (int i = 0; i < 3 * COUNT_TRIANGLES; i++) {
			normal3[i].x = result_ddr_normal[4 * i + 0];
			normal3[i].y = result_ddr_normal[4 * i + 1];
			normal3[i].z = result_ddr_normal[4 * i + 2];
		}
#endif
		t1 = clock();
		timeClock = t1 - t0;
		printf("computeNormal=%d\n", timeClock);
		int error = nmglGetError();

		t0 = clock();
		nmglDrawArrays(NMGL_POINTS, 0, COUNT_TRIANGLES*3);
		//nmglDrawArrays(NMGL_TRIANGLES, 0, COUNT_TRIANGLES * 3);
		t1 = clock();
		timeClock = t1 - t0;
		printf("nmglDrawArrays=%d\n", timeClock);
		

		nmglvsSwapBuffer();
		


	}
	nmglDisableClientState(NMGL_VERTEX_ARRAY);
	nmglDisableClientState(NMGL_COLOR_ARRAY);
	nmglDisableClientState(NMGL_NORMAL_ARRAY);
	nmglvsExit_mc12101();
	return 0x600D600D;
}