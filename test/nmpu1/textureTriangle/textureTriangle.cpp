#include "nmtype.h"
#include "demo3d_common.h"
#include "nmgltex_nm1.h"
#include "textureTriangle.h"
#include <cstdio>

SECTION(".text_demo3d")
void textureTriangle(Pattern* patterns, 
                 Triangles* triangles,
                 Rectangle* windows, 
                 nm32s* pSrcTriangle, 
                 nm32s* pDstTriangle, 
                 int count)
{
    printf ("Start textureTriangle\n"); 
    
    int i = 0;
    int row = 0;
    int col = 0;
    
    for (i = 0; i < count; i++)
    {
        int width = windows[i].width;
        int height = windows[i].height;
        nm32s* srcTriangle = pSrcTriangle + i*width*height;//TODO: incorrect formula
        nm32s* dstTriangle = pDstTriangle + i*width*height;//TODO: incorrect formula
        nm32s pixel_pos = 0;
        nm32s pixel_value = 0;
        
        //copy pixels from pSrcTriangle to pDstTriangle
        for (row = 0; row < height; row++)
        {
            for (col = 0; col < width; col++)
            {
                pixel_pos = (row*width + col)*4;
                pixel_value = srcTriangle[row*width + col];
                ((unsigned char*)dstTriangle)[pixel_pos] = (pixel_value & 0x000000ff);
                ((unsigned char*)dstTriangle)[pixel_pos + 1] = (pixel_value & 0x0000ff00) >> 8;
                ((unsigned char*)dstTriangle)[pixel_pos+ 2] = (pixel_value & 0x00ff0000) >> 16;
                ((unsigned char*)dstTriangle)[pixel_pos + 3] = (pixel_value & 0xff000000) >> 24;
            }
        }

        // for (row = 0; row < height; row++)
        // {
            // for (col = 0; col < width; col++)
            // {
                // printf ("%08x ", ((int*)images[i].pixels)[row*width + col]);
            // }
        // printf ("\n\n");            
        // }
  
        // printf ("\n\n");   
    }
    
    printf ("End textureTriangle\n");     
    return;
}
