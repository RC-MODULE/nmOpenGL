#pragma once
#include "nmtype.h"
#include "debugprint.h"

struct Vector2 {
	int x;
	int y;
};

struct Size {
	int width;
	int height;
};

struct Rectangle {
	int x;
	int y;
	int width;
	int height;
};

typedef struct s_mat4nm32f {
	float matr[16];
} mat4nm32f;

typedef struct s_v3nm32f {
	float v0;
	float v1;
	float v2;
} v3nm32f;

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

int cnvMaskToIndices(nm1* mask, int* indices, int size);

#ifdef __cplusplus
}
#endif //__cplusplus