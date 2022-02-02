#include <cerrno>
#include <iostream>

#include "nmplv/vSupport.h"
#include "service.h"

void convertABGR32_RGB565(const abgr32 *srcArray, rgb565 *dstArray, int count)
{
	// Increase count up to the nearest multiple of 4
	if (count % 4){
		count = count + (4 - count % 4);
#ifdef DEBUG
		printf("Warning: count should be a multiple of 4!");
#endif
	}

    for (int i = 0; i < count; i++)
    {
	int16b RGB565_temp; // Temporary storage for the RGB565 value
  	//R
   	RGB565_temp = ((int16b) srcArray[i].vec[0])  << 11;
   	//G
   	RGB565_temp = (RGB565_temp & 0xF800) | (int16b) (((srcArray[i].vec[0] >> 27) & 0x7E0));
   	//B
        RGB565_temp = (RGB565_temp & 0xFFE0) | (int16b) (srcArray[i].vec[1] & 0x1F);

        nmppsPut_16s(dstArray, i, RGB565_temp);
    }
}
