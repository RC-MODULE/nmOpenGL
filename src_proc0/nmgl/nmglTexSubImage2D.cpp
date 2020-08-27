#include "demo3d_nm0.h"


#pragma code_section ".text_nmgl"

extern int getTexelSizeUbytes(NMGLint format);
void nmglTexSubImage2D (NMGLenum target, NMGLint level, NMGLint xoffset, NMGLint yoffset, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const void *pixels)
{
	int size=0;
	int i=0,j=0;
	unsigned int xstart=0;
	unsigned int line=0;
	//unsigned int nline=0;
	//int ystart=0;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (target !=NMGL_TEXTURE_2D) {cntxt->error=NMGL_INVALID_ENUM;return;}
	if ((level < 0)||(level > NMGL_MAX_MIPMAP_LVL))  {cntxt->error=NMGL_INVALID_VALUE;return;}//TODO: переписать для максимального уровня конкретной текстуры
		
	if (type != NMGL_UNSIGNED_BYTE) {cntxt->error=NMGL_INVALID_ENUM;return;}
	if((width<0)||(height<0)) {cntxt->error=NMGL_INVALID_VALUE;return;}
	if((xoffset<0)||(yoffset<0)||(xoffset+width >= ActiveTexObjectP->texImages2D[level].width)||(yoffset+height >= ActiveTexObjectP->texImages2D[level].height)) {cntxt->error=NMGL_INVALID_VALUE;return;}
	if(format != ActiveTexObjectP->texImages2D[level].internalformat){cntxt->error=NMGL_INVALID_OPERATION;return;}
	if(pixels == NULL){cntxt->error=NMGL_INVALID_VALUE;return;}

	size=getTexelSizeUbytes(ActiveTexObjectP->texImages2D[level].internalformat);
	
	//line=size*ActiveTexObjectP->texImages2D[level].width;
	//xstart=xoffset*line+size*yoffset;
	//nline=size*width;
	line=ActiveTexObjectP->texImages2D[level].width;
	xstart=yoffset*line+xoffset;
	for(i=0;i<height;i++)
	{
		//for(j=0;j<width*size;j++)
		for(j=0;j<width;j++)
		{
			//*((unsigned char*)ActiveTexObjectP->texImages2D[level].pixels+xstart+i*line+j)=*((unsigned char*)pixels+i*nline+j);
			*((NMGLubyte*)ActiveTexObjectP->texImages2D[level].pixels+xstart+i*line+j)=*((NMGLubyte*)pixels+i*width+j);
		}	
	}
}
