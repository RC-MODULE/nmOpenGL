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
unsigned char pixels_1_1[4] = {  
0x00, 0xff, 0xff, 0x00, 
};  

SECTION(TEXTURE_SECTION)
TexImage2D teximage_1_1 = { 
    //internal format
    NMGL_RGB,

    //pixels
    pixels_1_1,

    //width
    1,

    //height
    1
};  



#ifdef __cplusplus
}
#endif
