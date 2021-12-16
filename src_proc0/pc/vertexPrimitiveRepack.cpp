//#include <iostream>
//#include <cerrno>

#include "service.h"
#include "nmgl.h"

int vertexPrimitiveRepack_triangle(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int vertCount);
int vertexPrimitiveRepack_triangle_strip(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int vertCount);
int vertexPrimitiveRepack_triangle_fan(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int vertCount);

int vertexPrimitiveRepack(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int mode, int vertCount)
{
	/* If vertCount is 0, inner functions will return 0 */
    switch (mode)
    {
        case NMGL_TRIANGLES:
            return vertexPrimitiveRepack_triangle(srcVertex, srcColor, dstVertex, dstColor, vertCount);
            break;

        case NMGL_TRIANGLE_STRIP:
            return vertexPrimitiveRepack_triangle_strip(srcVertex, srcColor, dstVertex, dstColor, vertCount);
            break;
        
        case NMGL_TRIANGLE_FAN:
            return vertexPrimitiveRepack_triangle_fan(srcVertex, srcColor, dstVertex, dstColor, vertCount);
            break;
    
        default:
            return -1;
            break;
    }
}

int vertexPrimitiveRepack_triangle(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int vertCount)
{
    //const int num_coords = 4; //Количество координат во входном векторе
    int trCount = vertCount/3;
    int stride = (trCount % 2) ? trCount + 1: trCount;
    for (int i = 0; i < trCount; i++)
    {       
        dstVertex[i + stride * 0]  = srcVertex[(i*3 + 0)].vec[0];
        dstVertex[i + stride * 1]  = srcVertex[(i*3 + 0)].vec[1];
        dstVertex[i + stride * 2]  = srcVertex[(i*3 + 0)].vec[2];
        dstVertex[i + stride * 3]  = srcVertex[(i*3 + 0)].vec[3];

        dstVertex[i + stride * 4]  = srcVertex[(i*3 + 1)].vec[0];
        dstVertex[i + stride * 5]  = srcVertex[(i*3 + 1)].vec[1];
        dstVertex[i + stride * 6]  = srcVertex[(i*3 + 1)].vec[2];
        dstVertex[i + stride * 7]  = srcVertex[(i*3 + 1)].vec[3];

        dstVertex[i + stride * 8]  = srcVertex[(i*3 + 2)].vec[0];
        dstVertex[i + stride * 9]  = srcVertex[(i*3 + 2)].vec[1];
        dstVertex[i + stride * 10] = srcVertex[(i*3 + 2)].vec[2];
        dstVertex[i + stride * 11] = srcVertex[(i*3 + 2)].vec[3];

        dstColor[i*3 + 0]  = srcColor[i*3 + 0];              
        dstColor[i*3 + 1]  = srcColor[i*3 + 1];       
        dstColor[i*3 + 2]  = srcColor[i*3 + 2];
    }
	
	if(trCount%2){
		dstVertex[trCount + stride * 0]  = dstVertex[(trCount - 1) + stride * 0];  
		dstVertex[trCount + stride * 1]  = dstVertex[(trCount - 1) + stride * 1];  
        dstVertex[trCount + stride * 2]  = dstVertex[(trCount - 1) + stride * 2];  
        dstVertex[trCount + stride * 3]  = dstVertex[(trCount - 1) + stride * 3];  
                                     
        dstVertex[trCount + stride * 4]  = dstVertex[(trCount - 1) + stride * 4];  
        dstVertex[trCount + stride * 5]  = dstVertex[(trCount - 1) + stride * 5];  
        dstVertex[trCount + stride * 6]  = dstVertex[(trCount - 1) + stride * 6];  
        dstVertex[trCount + stride * 7]  = dstVertex[(trCount - 1) + stride * 7];  
                                     
		dstVertex[trCount + stride * 8]  = dstVertex[(trCount - 1) + stride * 8];
		dstVertex[trCount + stride * 9]  = dstVertex[(trCount - 1) + stride * 9];
		dstVertex[trCount + stride * 10] = dstVertex[(trCount - 1) + stride * 10];
		dstVertex[trCount + stride * 11] = dstVertex[(trCount - 1) + stride * 11];
        
        dstColor[trCount*3 + 0]  = dstColor[(trCount-1)*3 + 0];
        dstColor[trCount*3 + 1]  = dstColor[(trCount-1)*3 + 1];
		dstColor[trCount*3 + 2]  = dstColor[(trCount-1)*3 + 2];
		trCount++;
	}
	
    return trCount;
}

int vertexPrimitiveRepack_triangle_strip(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int vertCount)
{
    int trCount = vertCount - 2;
	int stride = (trCount % 2) ? trCount + 1: trCount;
    for (int i = 0; i < trCount; i++)
    {
        switch (i % 2)
        {
            case 0:
                dstVertex[i + stride* 0]  = srcVertex[(i + 0)].vec[0];
                dstVertex[i + stride* 1]  = srcVertex[(i + 0)].vec[1];
                dstVertex[i + stride* 2]  = srcVertex[(i + 0)].vec[2];
                dstVertex[i + stride* 3]  = srcVertex[(i + 0)].vec[3];

                dstVertex[i + stride * 4]  = srcVertex[(i + 1)].vec[0];
                dstVertex[i + stride * 5]  = srcVertex[(i + 1)].vec[1];
                dstVertex[i + stride * 6]  = srcVertex[(i + 1)].vec[2];
                dstVertex[i + stride * 7]  = srcVertex[(i + 1)].vec[3];

                dstVertex[i + stride * 8]  = srcVertex[(i + 2)].vec[0];
                dstVertex[i + stride * 9]  = srcVertex[(i + 2)].vec[1];
                dstVertex[i + stride * 10] = srcVertex[(i + 2)].vec[2];
                dstVertex[i + stride * 11] = srcVertex[(i + 2)].vec[3];

                dstColor[i * 3 + 0]  = srcColor[(i + 0)];                
                dstColor[i * 3 + 1]  = srcColor[(i + 1)];                
                dstColor[i * 3 + 2]  = srcColor[(i + 2)];
                
                break;
            case 1:
                dstVertex[i + stride * 0]  = srcVertex[(i + 1)].vec[0];
                dstVertex[i + stride * 1]  = srcVertex[(i + 1)].vec[1];
                dstVertex[i + stride * 2]  = srcVertex[(i + 1)].vec[2];
                dstVertex[i + stride * 3]  = srcVertex[(i + 1)].vec[3];

                dstVertex[i + stride * 4]  = srcVertex[(i + 0)].vec[0];
                dstVertex[i + stride * 5]  = srcVertex[(i + 0)].vec[1];
                dstVertex[i + stride * 6]  = srcVertex[(i + 0)].vec[2];
                dstVertex[i + stride * 7]  = srcVertex[(i + 0)].vec[3];

                dstVertex[i + stride * 8]  = srcVertex[(i + 2)].vec[0];
                dstVertex[i + stride * 9]  = srcVertex[(i + 2)].vec[1];
                dstVertex[i + stride * 10] = srcVertex[(i + 2)].vec[2];
                dstVertex[i + stride * 11] = srcVertex[(i + 2)].vec[3];

                dstColor[i * 3 + 0]  = srcColor[(i + 1)];               
                dstColor[i * 3 + 1]  = srcColor[(i + 0)];               
                dstColor[i * 3 + 2]  = srcColor[(i + 2)];               

                break;
        
            default:
                break;
        }               
    }
	if(trCount%2){
		dstVertex[trCount + stride * 0] = dstVertex[(trCount - 1) + stride * 0];
		dstVertex[trCount + stride * 1] = dstVertex[(trCount - 1) + stride * 1];
		dstVertex[trCount + stride * 2] = dstVertex[(trCount - 1) + stride * 2];
		dstVertex[trCount + stride * 3] = dstVertex[(trCount - 1) + stride * 3];

		dstVertex[trCount + stride * 4] = dstVertex[(trCount - 1) + stride * 4];
		dstVertex[trCount + stride * 5] = dstVertex[(trCount - 1) + stride * 5];
		dstVertex[trCount + stride * 6] = dstVertex[(trCount - 1) + stride * 6];
		dstVertex[trCount + stride * 7] = dstVertex[(trCount - 1) + stride * 7];

		dstVertex[trCount + stride * 8] = dstVertex[(trCount - 1) + stride * 8];
		dstVertex[trCount + stride * 9] = dstVertex[(trCount - 1) + stride * 9];
		dstVertex[trCount + stride * 10] = dstVertex[(trCount - 1) + stride * 10];
		dstVertex[trCount + stride * 11] = dstVertex[(trCount - 1) + stride * 11];
        
        dstColor[trCount * 3 + 0]  = dstColor[(trCount-1)*3 + 0];
        dstColor[trCount * 3 + 1]  = dstColor[(trCount-1)*3 + 1];
		dstColor[trCount * 3 + 2]  = dstColor[(trCount-1)*3 + 2];
		trCount++;
	}
    return trCount;
}

int vertexPrimitiveRepack_triangle_fan(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int vertCount)
{
    int trCount = vertCount - 2;
	int stride = (trCount % 2) ? trCount + 1: trCount;
    for (int i = 0; i < trCount; i++)
    {       
        dstVertex[i + stride * 0]  = srcVertex[0].vec[0];
        dstVertex[i + stride * 1]  = srcVertex[0].vec[1];
        dstVertex[i + stride * 2]  = srcVertex[0].vec[2];
        dstVertex[i + stride * 3]  = srcVertex[0].vec[3];

        dstVertex[i + stride * 4]  = srcVertex[(i + 1)].vec[0];
        dstVertex[i + stride * 5]  = srcVertex[(i + 1)].vec[1];
        dstVertex[i + stride * 6]  = srcVertex[(i + 1)].vec[2];
        dstVertex[i + stride * 7]  = srcVertex[(i + 1)].vec[3];

        dstVertex[i + stride * 8]  = srcVertex[(i + 2)].vec[0];
        dstVertex[i + stride * 9]  = srcVertex[(i + 2)].vec[1];
        dstVertex[i + stride * 10] = srcVertex[(i + 2)].vec[2];
        dstVertex[i + stride * 11] = srcVertex[(i + 2)].vec[3];

        dstColor[i * 3 + 0]  = srcColor[0];        
        dstColor[i * 3 + 1]  = srcColor[(i + 1)];        
        dstColor[i * 3 + 2]  = srcColor[(i + 2)];
    }
	if(trCount%2){
		dstVertex[trCount + stride * 0] = dstVertex[(trCount - 1) + stride * 0];
		dstVertex[trCount + stride * 1] = dstVertex[(trCount - 1) + stride * 1];
		dstVertex[trCount + stride * 2] = dstVertex[(trCount - 1) + stride * 2];
		dstVertex[trCount + stride * 3] = dstVertex[(trCount - 1) + stride * 3];

		dstVertex[trCount + stride * 4] = dstVertex[(trCount - 1) + stride * 4];
		dstVertex[trCount + stride * 5] = dstVertex[(trCount - 1) + stride * 5];
		dstVertex[trCount + stride * 6] = dstVertex[(trCount - 1) + stride * 6];
		dstVertex[trCount + stride * 7] = dstVertex[(trCount - 1) + stride * 7];

		dstVertex[trCount + stride * 8] = dstVertex[(trCount - 1) + stride * 8];
		dstVertex[trCount + stride * 9] = dstVertex[(trCount - 1) + stride * 9];
		dstVertex[trCount + stride * 10] = dstVertex[(trCount - 1) + stride * 10];
		dstVertex[trCount + stride * 11] = dstVertex[(trCount - 1) + stride * 11];
        
        dstColor[trCount*3 + 0]  = dstColor[(trCount-1)*3 + 0];
        dstColor[trCount*3 + 1]  = dstColor[(trCount-1)*3 + 1];
		dstColor[trCount*3 + 2]  = dstColor[(trCount-1)*3 + 2];
		trCount++;
	}
    return trCount;
}
