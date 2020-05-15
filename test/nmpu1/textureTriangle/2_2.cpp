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

SECTION(TEXTURE_SECTION) unsigned char pixels_2_2[12] = {  
0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
};  

SECTION(TEXTURE_SECTION) image_t image_2_2 = { 
    2,
    2,
    RGB,
    UNSIGNED_BYTE,
    pixels_2_2
};  


#ifdef __cplusplus
}
#endif