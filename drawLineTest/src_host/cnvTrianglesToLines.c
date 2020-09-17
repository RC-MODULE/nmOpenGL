#include <stdio.h> 
#include <stdlib.h>
#define PASS_outer_loop_vertex_words 	fscanf(fmodel,"%s",str);fscanf(fmodel,"%s",str);fscanf(fmodel,"%s",str);
#define PASS_vertex_word fscanf(fmodel,"%s",str);
void cnvTrianglesToLines(float* srcVertices,float* dstVertices,int size)
{
	for (int i = 0; i < size; i++) {
		dstVertices[24 * i + 0] = srcVertices[12 * i + 0];
		dstVertices[24 * i + 1] = srcVertices[12 * i + 1];
		dstVertices[24 * i + 2] = srcVertices[12 * i + 2];
		dstVertices[24 * i + 3] = srcVertices[12 * i + 3];
		dstVertices[24 * i + 4] = srcVertices[12 * i + 4];
		dstVertices[24 * i + 5] = srcVertices[12 * i + 5];
		dstVertices[24 * i + 6] = srcVertices[12 * i + 6];
		dstVertices[24 * i + 7] = srcVertices[12 * i + 7];

		dstVertices[24 * i + 8] = srcVertices[12 * i + 4];
		dstVertices[24 * i + 9] = srcVertices[12 * i + 5];
		dstVertices[24 * i + 10] = srcVertices[12 * i + 6];
		dstVertices[24 * i + 11] = srcVertices[12 * i + 7];
		dstVertices[24 * i + 12] = srcVertices[12 * i + 8];
		dstVertices[24 * i + 13] = srcVertices[12 * i + 9];
		dstVertices[24 * i + 14] = srcVertices[12 * i + 10];
		dstVertices[24 * i + 15] = srcVertices[12 * i + 11];

		dstVertices[24 * i + 16] = srcVertices[12 * i + 8];
		dstVertices[24 * i + 17] = srcVertices[12 * i + 9];
		dstVertices[24 * i + 18] = srcVertices[12 * i + 10];
		dstVertices[24 * i + 19] = srcVertices[12 * i + 11];
		dstVertices[24 * i + 20] = srcVertices[12 * i + 0];
		dstVertices[24 * i + 21] = srcVertices[12 * i + 1];
		dstVertices[24 * i + 22] = srcVertices[12 * i + 2];
		dstVertices[24 * i + 23] = srcVertices[12 * i + 3];
	}
	return;
}