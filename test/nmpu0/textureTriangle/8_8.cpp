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
unsigned char pixels_8_8[192] = {  
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 
0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};  

SECTION(TEXTURE_SECTION)
TexImage2D teximage_8_8 = { 
    //internal format
    NMGL_RGB,

    //pixels
    pixels_8_8,

    //width
    8,

    //height
    8
};  



#ifdef __cplusplus
}
#endif
