//#include <iostream>
//#include <cerrno>

#include "service.h"
#include "nmgl.h"

//TEXTURING_PART
int texCoordsRepack_triangle(const v2nm32f *srcTexCoords, nm32f *dstTexCoords, int vertCount);
int texCoordsRepack_triangle_strip(const v2nm32f *srcTexCoords,  nm32f *dstTexCoords, int vertCount);
int texCoordsRepack_triangle_fan(const v2nm32f *srcTexCoords,  nm32f *dstTexCoords, int vertCount);

int texCoordsRepack(const v2nm32f *srcTexCoords,  nm32f *dstTexCoords, int mode, int vertCount)
{
	/* If vertCount is 0, inner functions will return 0 */
    switch (mode)
    {
        case NMGL_TRIANGLES:
            return texCoordsRepack_triangle(srcTexCoords, dstTexCoords, vertCount);
            break;

        case NMGL_TRIANGLE_STRIP:
            return texCoordsRepack_triangle_strip(srcTexCoords,dstTexCoords, vertCount);
            break;
        
        case NMGL_TRIANGLE_FAN:
            return texCoordsRepack_triangle_fan(srcTexCoords,dstTexCoords, vertCount);
            break;
    
        default:
            return -1;
            break;
    }
}

int texCoordsRepack_triangle(const v2nm32f *srcTexCoords,  nm32f *dstTexCoords, int vertCount)
{
    //const int num_coords = 4; //Количество координат во входном векторе
    int trCount = vertCount/3;
    int stride = (trCount % 2) ? trCount + 1: trCount;
    for (int i = 0; i < trCount; i++)
    {       
        dstTexCoords[i + stride * 0]  = srcTexCoords[(i*3 + 0)].v0;
        dstTexCoords[i + stride * 1]  = srcTexCoords[(i*3 + 0)].v1;

        dstTexCoords[i + stride * 2]  = srcTexCoords[(i*3 + 1)].v0;
        dstTexCoords[i + stride * 3]  = srcTexCoords[(i*3 + 1)].v1;

        dstTexCoords[i + stride * 4]  = srcTexCoords[(i*3 + 2)].v0;
        dstTexCoords[i + stride * 5]  = srcTexCoords[(i*3 + 2)].v1;
    }
	
	if(trCount%2){
		dstTexCoords[trCount + stride * 0]  = dstTexCoords[(trCount - 1) + stride * 0];  
		dstTexCoords[trCount + stride * 1]  = dstTexCoords[(trCount - 1) + stride * 1];  
                                     
        dstTexCoords[trCount + stride * 2]  = dstTexCoords[(trCount - 1) + stride * 2];  
        dstTexCoords[trCount + stride * 3]  = dstTexCoords[(trCount - 1) + stride * 3];  
                                     
		dstTexCoords[trCount + stride * 4]  = dstTexCoords[(trCount - 1) + stride * 4];
		dstTexCoords[trCount + stride * 5]  = dstTexCoords[(trCount - 1) + stride * 5];
        
		trCount++;
	}
    return trCount;
}

int texCoordsRepack_triangle_strip(const v2nm32f *srcTexCoords,  nm32f *dstTexCoords, int vertCount)
{
    int trCount = vertCount - 2;
	int stride = (trCount % 2) ? trCount + 1: trCount;
    for (int i = 0; i < trCount; i++)
    {
        switch (i % 2)
        {
            case 0:
                dstTexCoords[i + stride* 0]  = srcTexCoords[(i + 0)].v0;
                dstTexCoords[i + stride* 1]  = srcTexCoords[(i + 0)].v1;
 
                dstTexCoords[i + stride * 2]  = srcTexCoords[(i + 1)].v0;
                dstTexCoords[i + stride * 3]  = srcTexCoords[(i + 1)].v1;

                dstTexCoords[i + stride * 4]  = srcTexCoords[(i + 2)].v0;
                dstTexCoords[i + stride * 5]  = srcTexCoords[(i + 2)].v1;
               
                break;
            case 1:
                dstTexCoords[i + stride * 0]  = srcTexCoords[(i + 1)].v0;
                dstTexCoords[i + stride * 1]  = srcTexCoords[(i + 1)].v1;

                dstTexCoords[i + stride * 2]  = srcTexCoords[(i + 0)].v0;
                dstTexCoords[i + stride * 3]  = srcTexCoords[(i + 0)].v1;

                dstTexCoords[i + stride * 4]  = srcTexCoords[(i + 2)].v0;
                dstTexCoords[i + stride * 5]  = srcTexCoords[(i + 2)].v1;

                break;
        
            default:
                break;
        }               
    }
	if(trCount%2){
		dstTexCoords[trCount + stride * 0] = dstTexCoords[(trCount - 1) + stride * 0];
		dstTexCoords[trCount + stride * 1] = dstTexCoords[(trCount - 1) + stride * 1];

		dstTexCoords[trCount + stride * 2] = dstTexCoords[(trCount - 1) + stride * 2];
		dstTexCoords[trCount + stride * 3] = dstTexCoords[(trCount - 1) + stride * 3];

		dstTexCoords[trCount + stride * 4] = dstTexCoords[(trCount - 1) + stride * 4];
		dstTexCoords[trCount + stride * 5] = dstTexCoords[(trCount - 1) + stride * 5];
 
        trCount++;
	}
    return trCount;
}

int texCoordsRepack_triangle_fan(const v2nm32f *srcTexCoords,  nm32f *dstTexCoords, int vertCount)
{
    int trCount = vertCount - 2;
	int stride = (trCount % 2) ? trCount + 1: trCount;
    for (int i = 0; i < trCount; i++)
    {       
        dstTexCoords[i + stride * 0]  = srcTexCoords[0].v0;
        dstTexCoords[i + stride * 1]  = srcTexCoords[0].v1;

        dstTexCoords[i + stride * 2]  = srcTexCoords[(i + 1)].v0;
        dstTexCoords[i + stride * 3]  = srcTexCoords[(i + 1)].v1;
 
        dstTexCoords[i + stride * 4]  = srcTexCoords[(i + 2)].v0;
        dstTexCoords[i + stride * 5]  = srcTexCoords[(i + 2)].v1;
    }
	if(trCount%2){
		dstTexCoords[trCount + stride * 0] = dstTexCoords[(trCount - 1) + stride * 0];
		dstTexCoords[trCount + stride * 1] = dstTexCoords[(trCount - 1) + stride * 1];

		dstTexCoords[trCount + stride * 2] = dstTexCoords[(trCount - 1) + stride * 2];
		dstTexCoords[trCount + stride * 3] = dstTexCoords[(trCount - 1) + stride * 3];

		dstTexCoords[trCount + stride * 4] = dstTexCoords[(trCount - 1) + stride * 4];
		dstTexCoords[trCount + stride * 5] = dstTexCoords[(trCount - 1) + stride * 5];

		trCount++;
	}
    return trCount;
}

//TEXTURING_PART
