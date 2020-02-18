#pragma once
#include "nmtype.h"
#include "stdio.h"
#include "demo3d_common.h"
#include "demo3d_nm1.h"

#define NUM_NM_CORES 2

#define PROGRAM "main0.abs"
#define PROGRAM1 "main1.abs"

struct Models{
	char* sphere = "../../models/sphere_31.stl";
	char* test = "../../models/test.stl";
	char* cube = "../../models/small_cube.stl";
	char* two_sphere = "../../models/2_sphere.stl";
	char* rcm = "../../models/out_RCM_32.stl";
	char* nm = "../../models/NM_32.stl";
	char* snowman = "../../models/snow_white_32.stl";
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
};
//-------------------------------------------------------------------------------------------------------------------
void hostCreatePatterns(Patterns* patterns);

//---------------------------------------------------------------------------------------------

				 