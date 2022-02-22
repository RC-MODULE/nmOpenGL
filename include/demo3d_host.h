#pragma once
#include "nmtype.h"
#include "stdio.h"

#define NUM_NM_CORES 2

#ifdef DEBUG
#define PROGRAM "main0d.abs"
#define PROGRAM1 "main1d.abs"
#define MAP0 "main0d.map"
#define MAP1 "main1d.map"
#else
#define PROGRAM "main0.abs"
#define PROGRAM1 "main1.abs"
#define MAP0 "main0.map"
#define MAP1 "main1.map"
#endif

struct Models{
    const char* sphere = "../../models/sphere_31.stl";
    const char* test = "../../models/test.stl";
    const char* cube = "../../models/small_cube.stl";
    const char* two_sphere = "../../models/2_sphere.stl";
    const char* rcm = "../../models/out_RCM_32.stl";
    const char* nm = "../../models/NM_32.stl";
    const char* snowman = "../../models/snow_white_32.stl";
};

extern "C"{
	int get_amm_poligone(FILE *fp);
	void create_array_block_sep_arrays(
		FILE *fp,
		float* Ax,
		float* Ay,
		float* Az,
		float* Bx,
		float* By,
		float* Bz,
		float* Cx,
		float* Cy,
		float* Cz,
		float* Nx,
		float* Ny,
		float* Nz,
		float scale
	);
	void createArrayVec4(
		FILE *fmodel,
		float* verticesVec4,
		float* normalVec4,
		float scale
	);
	void cnvTrianglesToLines(float* srcVertices, float* dstVertices, int size);
};
//-------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------

				 
