#include <iostream>
#include <cerrno>

#include "service.h"
#include "nmgl.h"

int vertexPrimitiveRepack_triangle(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int vertCount);
int vertexPrimitiveRepack_triangle_strip(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int vertCount);
int vertexPrimitiveRepack_triangle_fan(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int vertCount);

int vertexPrimitiveRepack(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int mode, int vertCount)
{
    if ((srcVertex == NULL) | (srcColor == NULL) | (dstVertex == NULL) | (dstColor == NULL) | (vertCount <= 0))
	{
		errno = EINVAL;
		return -1;
	}

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
    
    for (int i = 0; i < trCount; i++)
    {       
        dstVertex[i + trCount * 0]  = srcVertex[(i*3 + 0)].vec[0];
        dstVertex[i + trCount * 1]  = srcVertex[(i*3 + 0)].vec[1];
        dstVertex[i + trCount * 2]  = srcVertex[(i*3 + 0)].vec[2];
        dstVertex[i + trCount * 3]  = srcVertex[(i*3 + 0)].vec[3];

        dstVertex[i + trCount * 4]  = srcVertex[(i*3 + 1)].vec[0];
        dstVertex[i + trCount * 5]  = srcVertex[(i*3 + 1)].vec[1];
        dstVertex[i + trCount * 6]  = srcVertex[(i*3 + 1)].vec[2];
        dstVertex[i + trCount * 7]  = srcVertex[(i*3 + 1)].vec[3];

        dstVertex[i + trCount * 8]  = srcVertex[(i*3 + 2)].vec[0];
        dstVertex[i + trCount * 9]  = srcVertex[(i*3 + 2)].vec[1];
        dstVertex[i + trCount * 10] = srcVertex[(i*3 + 2)].vec[2];
        dstVertex[i + trCount * 11] = srcVertex[(i*3 + 2)].vec[3];

        dstColor[i*3 + 0]  = srcColor[i*3 + 0];
        
        dstColor[i*3 + 1]  = srcColor[i*3 + 1];
       
        dstColor[i*3 + 2]  = srcColor[i*3 + 2];
        

        
    }
    return trCount;
}

int vertexPrimitiveRepack_triangle_strip(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int vertCount)
{
    int trCount = vertCount - 2;
    for (int i = 0; i < trCount; i++)
    {
        switch (i % 2)
        {
            case 0:
                dstVertex[i + trCount * 0]  = srcVertex[(i + 0)].vec[0];
                dstVertex[i + trCount * 1]  = srcVertex[(i + 0)].vec[1];
                dstVertex[i + trCount * 2]  = srcVertex[(i + 0)].vec[2];
                dstVertex[i + trCount * 3]  = srcVertex[(i + 0)].vec[3];

                dstVertex[i + trCount * 4]  = srcVertex[(i + 1)].vec[0];
                dstVertex[i + trCount * 5]  = srcVertex[(i + 1)].vec[1];
                dstVertex[i + trCount * 6]  = srcVertex[(i + 1)].vec[2];
                dstVertex[i + trCount * 7]  = srcVertex[(i + 1)].vec[3];

                dstVertex[i + trCount * 8]  = srcVertex[(i + 2)].vec[0];
                dstVertex[i + trCount * 9]  = srcVertex[(i + 2)].vec[1];
                dstVertex[i + trCount * 10] = srcVertex[(i + 2)].vec[2];
                dstVertex[i + trCount * 11] = srcVertex[(i + 2)].vec[3];

                dstColor[i * 3 + 0]  = srcColor[(i + 0)];
                
                dstColor[i * 3 + 1]  = srcColor[(i + 1)];
                
                dstColor[i * 3 + 2]  = srcColor[(i + 2)];
                
                break;
            case 1:
                dstVertex[i + trCount * 0]  = srcVertex[(i + 1)].vec[0];
                dstVertex[i + trCount * 1]  = srcVertex[(i + 1)].vec[1];
                dstVertex[i + trCount * 2]  = srcVertex[(i + 1)].vec[2];
                dstVertex[i + trCount * 3]  = srcVertex[(i + 1)].vec[3];

                dstVertex[i + trCount * 4]  = srcVertex[(i + 0)].vec[0];
                dstVertex[i + trCount * 5]  = srcVertex[(i + 0)].vec[1];
                dstVertex[i + trCount * 6]  = srcVertex[(i + 0)].vec[2];
                dstVertex[i + trCount * 7]  = srcVertex[(i + 0)].vec[3];

                dstVertex[i + trCount * 8]  = srcVertex[(i + 2)].vec[0];
                dstVertex[i + trCount * 9]  = srcVertex[(i + 2)].vec[1];
                dstVertex[i + trCount * 10] = srcVertex[(i + 2)].vec[2];
                dstVertex[i + trCount * 11] = srcVertex[(i + 2)].vec[3];

                dstColor[i * 3 + 0]  = srcColor[(i + 1)];
               
                dstColor[i * 3 + 1]  = srcColor[(i + 0)];
               
                dstColor[i * 3 + 2]  = srcColor[(i + 2)];               

                break;
        
            default:
                break;
        }               
    }
    return trCount;
}

int vertexPrimitiveRepack_triangle_fan(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int vertCount)
{
    int trCount = vertCount - 2;

    for (int i = 0; i < trCount; i++)
    {       
        dstVertex[i + trCount * 0]  = srcVertex[0].vec[0];
        dstVertex[i + trCount * 1]  = srcVertex[0].vec[1];
        dstVertex[i + trCount * 2]  = srcVertex[0].vec[2];
        dstVertex[i + trCount * 3]  = srcVertex[0].vec[3];

        dstVertex[i + trCount * 4]  = srcVertex[(i + 1)].vec[0];
        dstVertex[i + trCount * 5]  = srcVertex[(i + 1)].vec[1];
        dstVertex[i + trCount * 6]  = srcVertex[(i + 1)].vec[2];
        dstVertex[i + trCount * 7]  = srcVertex[(i + 1)].vec[3];

        dstVertex[i + trCount * 8]  = srcVertex[(i + 2)].vec[0];
        dstVertex[i + trCount * 9]  = srcVertex[(i + 2)].vec[1];
        dstVertex[i + trCount * 10] = srcVertex[(i + 2)].vec[2];
        dstVertex[i + trCount * 11] = srcVertex[(i + 2)].vec[3];

        dstColor[i * 3 + 0]  = srcColor[0];
        
        dstColor[i * 3 + 1]  = srcColor[(i + 1)];
        
        dstColor[i * 3 + 2]  = srcColor[(i + 2)];
        
    }
    return trCount;
}
