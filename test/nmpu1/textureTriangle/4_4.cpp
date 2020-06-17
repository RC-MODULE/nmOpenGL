#include "image.h"
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

SECTION(TEXTURE_SECTION)
unsigned char pixels_4_4[48] = {  
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8b, 0x00, 0xff, 0x8b, 0x00, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8b, 0x00, 0xff, 0x8b, 0x00, 0xff, 
0x8b, 0x00, 0xff, 0x8b, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0x8b, 0x00, 0xff, 0x8b, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};  

SECTION(TEXTURE_SECTION)
image_t image_4_4 = { 
    4,
    4,
    RGB,
    UNSIGNED_BYTE,
    1,
    pixels_4_4
};  



#ifdef __cplusplus
}
#endif
