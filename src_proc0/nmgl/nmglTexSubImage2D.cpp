//#include "demo3d_nm0.h"
#if 0
#include "nmgl.h"


//#pragma code_section ".text_nmgl"


void nmglTexSubImage2D (NMGLenum target, NMGLint level, NMGLint xoffset, NMGLint yoffset, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const void *pixels)
{
	int size=0;
	int i=0,j=0;
	unsigned int xstart=0;
	unsigned int line=0;
	//unsigned int nline=0;
	//int ystart=0;
	if (target !=NMGL_TEXTURE_2D) {cntxt.error=NMGL_INVALID_ENUM;return;}
	if ((level < 0)||(level > NMGL_MAX_MIPMAP_LEVEL))  {cntxt.error=NMGL_INVALID_VALUE;return;}//TODO: переписать для максимального уровня конкретной текстуры
		
	if (type !=NMGL_UNSIGNED_BYTE) {cntxt.error=NMGL_INVALID_ENUM;return;}
	if((width<0)||(height<0)) {cntxt.error=NMGL_INVALID_VALUE;return;}
	if((xoffset<0)||(yoffset<0)||(xoffset+width > ActiveTexObject->texImages2D[level].width)||(yoffset+height > ActiveTexObject->texImages2D[level].height)) {cntxt.error=NMGL_INVALID_VALUE;return;}
	if(format != ActiveTexObject->texImages2D[level].internalformat){cntxt.error=NMGL_INVALID_OPERATION;return;}
	if(pixels == NULL){cntxt.error=NMGL_INVALID_VALUE;return;}

	size=getFormatSize(ActiveTexObject->texImages2D[level].internalformat);
	
	//line=size*ActiveTexObject->texImages2D[level].width;
	//xstart=xoffset*line+size*yoffset;
	//nline=size*width;
	line=ActiveTexObject->texImages2D[level].width;
	xstart=yoffset*line+xoffset;
	for(i=0;i<height;i++)
	{
		//for(j=0;j<width*size;j++)
		for(j=0;j<width;j++)
		{
			//*((unsigned char*)ActiveTexObject->texImages2D[level].pixels+xstart+i*line+j)=*((unsigned char*)pixels+i*nline+j);
			*((NMGLint*)ActiveTexObject->texImages2D[level].pixels+xstart+i*line+j)=*((NMGLint*)pixels+i*width+j);
		}	
	}
}
#endif