#include <stdio.h> 
#include <stdlib.h>
#define PASS_outer_loop_vertex_words 	fscanf(fmodel,"%s",str);fscanf(fmodel,"%s",str);fscanf(fmodel,"%s",str);
#define PASS_vertex_word fscanf(fmodel,"%s",str);
void createArray_TS(
	FILE *fmodel,
	float* verticesVec4,
	float* normalVec4,
	float scale
)
{
	//float scale = 1.0f;
	char str[20];
	char str_nrm[] = "facet";
	char str_dots[] = "vertex";
	float temp;
	int pol_num = 0;
	while(fscanf(fmodel,"%s",str) != EOF){
		if(strcmp(str_nrm,str)==0){//str is facet
			fscanf(fmodel,"%s",str);//pass normal word 
			////////////////////////////////////////////////wrt norm to diff arrays 
			fscanf(fmodel,"%s",str);
			temp = atof(str);//Nx
			normalVec4[9 * pol_num + 0] = temp;
			normalVec4[9 * pol_num + 3] = temp;
			normalVec4[9 * pol_num + 6] = temp;
			fscanf(fmodel,"%s",str);
			temp = atof(str);//Ny
			normalVec4[9 * pol_num + 1] = temp;
			normalVec4[9 * pol_num + 4] = temp;
			normalVec4[9 * pol_num + 7] = temp;
			fscanf(fmodel,"%s",str);
			temp = atof(str);//Nz
			normalVec4[9 * pol_num + 2] = temp;
			normalVec4[9 * pol_num + 5] = temp;
			normalVec4[9 * pol_num + 8] = temp;

			////////////////////////////////////////////////wrt A point
			PASS_outer_loop_vertex_words
			fscanf(fmodel,"%s",str);
			temp = scale*atof(str);//Nx
			verticesVec4[12*pol_num+0] = temp;
			fscanf(fmodel,"%s",str);
			temp = scale*atof(str);//Ny
			verticesVec4[12*pol_num+1] = temp;
			fscanf(fmodel,"%s",str);
			temp = scale*atof(str);//Nz
			verticesVec4[12*pol_num+2] = temp;
			verticesVec4[12 * pol_num + 3] = 1;
			////////////////////////////////////////////////wrt B point
			PASS_vertex_word
			fscanf(fmodel,"%s",str);
			temp = scale*atof(str);//Nx
			verticesVec4[12*pol_num+4] = temp;
			fscanf(fmodel,"%s",str);
			temp = scale*atof(str);//Ny
			verticesVec4[12*pol_num+5] = temp;
			fscanf(fmodel,"%s",str);
			temp = scale*atof(str);//Nz
			verticesVec4[12*pol_num+6] = temp;
			verticesVec4[12 * pol_num + 7] = 1;
			////////////////////////////////////////////////wrt C point
			PASS_vertex_word
			fscanf(fmodel,"%s",str);
			temp = scale*atof(str);//Nx
			verticesVec4[12*pol_num+8] = temp;
			fscanf(fmodel,"%s",str);
			temp = scale*atof(str);//Ny
			verticesVec4[12*pol_num+9] = temp;
			fscanf(fmodel,"%s",str);
			temp = scale*atof(str);//Nz
			verticesVec4[12*pol_num+10] = temp;
			verticesVec4[12 * pol_num + 11] = 1;
			pol_num++;
			}
	}
	if(pol_num%2){
		//case if amm of poligones is odd to prevent rubbish at the end of double word of nm_6407
		for (int i = 0; i < 9; i++) {
			verticesVec4[9 * pol_num + i] = 0;
			normalVec4[9 * pol_num + i] = 0;
		}
		pol_num++;
	}
	fclose(fmodel);
	return;
}