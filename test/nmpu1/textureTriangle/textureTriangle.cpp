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
    
    long long int temp;
    nm32s* dst = pDstTriangle;
    nm32s* src = pSrcTriangle;
    for(int c=0;c<count;c++){
        nm64s* pattern = (nm64s*) (patterns + c);
        pattern += windows[c].y;
        
        int width = windows[c].width;

        if (windows[c].x < 0) {
            width += windows[c].x;
        }
        
        for(int y = 0; y < windows[c].height; y++){
            temp = pattern[y];
            nm32s* pDst = (nm32s*)(dst + y * windows[c].width);
            nm32s* pSrc = (nm32s*)(src + y * windows[c].width);
            if (windows[c].x < 0) {
                for(int i = 0; i > windows[c].x; i--){
                    pDst[0] = 0x00000000;
                    pDst+=1;
                    pSrc+=1;
                }
            }
            else {
                temp >>= (windows[c].x * 2);
            }
            
            for(int x = 0; x < width; x++){
                int mul = temp & 0x3;
                pDst[0] = mul * pSrc[0];
                pDst += 1;
                pSrc += 1;
                temp >>= 2;
            }
        }
        src += windows[c].height * windows[c].width;
        dst += windows[c].height * windows[c].width;
    }
    
    printf ("End textureTriangle\n");     
    return;
}
