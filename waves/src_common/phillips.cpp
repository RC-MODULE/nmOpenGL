#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "phillips.h"
#include "nmpp.h"

#define		M_PI	3.1415926535

using namespace std;

int size = 256; 			// Number of verticies for each side of the field, determines square resolution
float g = 9.81; 			// Gravity
float fixsize = 1200.0f; 	// Fixed quad map length and width, regardless of square resolution
float L = 20;				// fixsize / 16; // 75
vector2 w(5.0, 5.0); 		// Wind speed
float A = 0.03f; 			// Spectrum parameter, affects output height
int N = 256; 				// Frequency map size, has to be some multiple of two

double gaussian() {
	double var1;
	double var2;
	double s;
	do {
		var1 = (rand() % 201 - 100) / static_cast<double>(100);
		var2 = (rand() % 201 - 100) / static_cast<double>(100);
		s = var1 * var1 + var2 * var2;
	} while (s >= 1 || s == 0);
	return var1 * sqrt(-log(s) / s);
}

// Phillips Spectrum modulated by wind speed and direction

float phillips(int n, int m) {
	vector2 k(M_PI * (2 * n - N) / L, M_PI * (2 * m - N) / L);
	float k_len = k.length();
	// Do nothing if frequency is excessively small
	if (k_len < 0.000001) {
		return 0.0;
	}

	// |k|^4
	float k_len2 = k_len * k_len;
	float k_len4 = k_len2 * k_len2;

	// |k dot w|^2
	float kw = k.normalize() * w.normalize();
	kw = kw * kw;

	// L, L^2
	float w_len = w.length();
	float Lsq = w_len * w_len / g;
	Lsq = Lsq * Lsq;

	// Damping term for eqation 24
	float damping = 0.001;
	float l2 = Lsq * damping * damping;

	float val = A * exp(-1.0f / (k_len2 * Lsq)) / k_len4 * kw * exp(-k_len2 * l2);
	return val;
}

// Generate the fourier amplitude of the height field at specified fequency vector k
// Produces results in the complex frequency domain

nm32fcr gaussian_complex() {
	// produces gaussian random draws with mean 0 and std dev 1
	nm32fcr t;
	t.re = gaussian();
	t.im = gaussian();
	//float a = 1;
	//float b = 0.1;
	//complex r(a, b);
	// Equation 25
	//return r * sqrt(phillips(n, m) / 2.0);
	return t;
}

void computeOmega(float* omega, int N, int M) {
	float kx, kz;
	float w0 = 0.0314159265358;
	for (int m = 0; m < M; m++) {

		kz = M_PI * (2 * m - N) / L;

		//kz = 0.04188790204786 * (2 * m - M);
		for (int n = 0; n < N; n++) {
			//kx = 0.04188790204786 * (2 * n - N);
			kx = M_PI * (2 * n - N) / L;
			omega[m * N + n] = w0 * floor(sqrt(234.19649875 * sqrt(kx * kx + kz * kz)));
		}
	}
}

void computeH0(nm32fcr* h0, nm32fcr* h0Conjugated, int N, int M) {
	for (int m = 0; m < M; ++m) {
		for (int n = 0; n < N; ++n) {
			int i = m * N + n;
			nm32fcr r          = gaussian_complex();
			h0[i].re           = r.re * sqrt(phillips(n, m) / 2.0);
			h0[i].im           = r.im * sqrt(phillips(n, m) / 2.0);
			h0Conjugated[i].re = r.re * sqrt(phillips(-n, -m) / 2.0);
			h0Conjugated[i].im = -r.im * sqrt(phillips(-n, -m) / 2.0);
		}
	}
}
