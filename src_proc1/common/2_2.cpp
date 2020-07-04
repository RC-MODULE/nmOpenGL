#include "nmgldef.h"
#include "nmgltex_common.h"
#include "texture_config.h"

#ifdef __GNUC__
#define SECTION(sec) __attribute__((section(sec)))
#else
#define SECTION(sec)
#endif // __GNUC__

#ifdef __cplusplus
extern "C"
{
#endif


//alignment = 4

SECTION(TEXTURE_SECTION)
unsigned char pixels_2_2[16] = {  
0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 
};  

SECTION(TEXTURE_SECTION)
TexImage2D teximage_2_2 = { 
    //internal format
    NMGL_RGB,

    //level
    0,

    //pixels
    pixels_2_2,

    //width
    2,

    //height
    2,

    //border
    0
};  



#ifdef __cplusplus
}
#endif
