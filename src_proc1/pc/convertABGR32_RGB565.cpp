#include <cerrno>
#include <iostream>

#include "nmplv/vSupport.h"
#include "service.h"

void convertABGR32_RGB565(const abgr32 *srcArray, rgb565 *dstArray, int count)
{

	if ((srcArray == NULL) | (dstArray == NULL) | (count <= 0))
	{
		errno = EINVAL;
		return;
	}

	/* */
    for (int i = 0; i < count; i++)
    {
    	int16b RGB565_temp; // Temporary storage for the RGB565 value
    	//R
    	RGB565_temp = (int16b) (srcArray[i].vec[0] >> 27);
    	//G
    	RGB565_temp = (RGB565_temp & 0xF81F) | (int16b) (((srcArray[i].vec[0] >> 53) & 0x7E0));
    	//B
        RGB565_temp = (RGB565_temp & 0x7FF) | (int16b) (((srcArray[i].vec[1] >> 16) & 0xF800));

        nmppsPut_16s(dstArray, i, RGB565_temp);
    }
}
