#include "demo3d_nm0.h"


#pragma code_section ".text_nmgl"

extern NMGLint getTexelSizeUbytes(NMGLint format);
void nmglTexSubImage2D (NMGLenum target, NMGLint level, NMGLint xoffset, NMGLint yoffset, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const void *pixels)
{
	//int size=0;
	int i=0,j=0;
	NMGLint tSize=0;
	int paddings=0;
	NMGLint xstart=0;
	unsigned int line=0;
	//unsigned int nline=0;
	//int ystart=0;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	tSize=getTexelSizeUbytes(ActiveTexObjectP->texImages2D[level].internalformat);
	if (target !=NMGL_TEXTURE_2D) {cntxt->error=NMGL_INVALID_ENUM;return;}
	if ((level < 0)||(level > NMGL_MAX_MIPMAP_LVL))  {cntxt->error=NMGL_INVALID_VALUE;return;}//TODO: переписать для максимального уровня конкретной текстуры
		
	if (type != NMGL_UNSIGNED_BYTE) {cntxt->error=NMGL_INVALID_ENUM;return;}
	if((width<0)||(height<0)) {cntxt->error=NMGL_INVALID_VALUE;return;}
	if((xoffset<0)||(yoffset<0)||(xoffset+width > ActiveTexObjectP->texImages2D[level].width)||(yoffset+height > ActiveTexObjectP->texImages2D[level].height)) {cntxt->error=NMGL_INVALID_VALUE;return;}
	if(format != ActiveTexObjectP->texImages2D[level].internalformat){cntxt->error=NMGL_INVALID_OPERATION;return;}
	//if(pixels == NULL){cntxt->error=NMGL_INVALID_VALUE;return;}

	//size=getTexelSizeUbytes(ActiveTexObjectP->texImages2D[level].internalformat);
	
	//line=size*ActiveTexObjectP->texImages2D[level].width;
	//xstart=xoffset*line+size*yoffset;
	//nline=size*width;
	//line=ActiveTexObjectP->texImages2D[level].width*size;
	line=ActiveTexObjectP->texImages2D[level].width*tSize;

	paddings=((width*tSize) % cntxt->unpackAlignment == 0) ? 0 : (cntxt->unpackAlignment - (width*tSize) % cntxt->unpackAlignment);

//DEBUG_PRINT1(("width=%d height=%d alignment=%d paddings=%d\n",width, height,cntxt->unpackAlignment, paddings ));
//DEBUG_PRINT(("width=%d height=%d alignment=%d paddings=%d\n",width, height,cntxt->unpackAlignment, paddings ));
	//xstart=yoffset*line+xoffset*size;
//DEBUG_PRINT(("texelUbytes00=%x TheOne=%x\n",getTexelSizeUbytes(ActiveTexObjectP->texImages2D[level].internalformat)),1+getTexelSizeUbytes(ActiveTexObjectP->texImages2D[level].internalformat));
//DEBUG_PRINT(("format is=%x should be %x\n",ActiveTexObjectP->texImages2D[level].internalformat,NMGL_RGBA));
	xstart=(NMGLubyte)(yoffset * line + xoffset * tSize);
DEBUG_PRINT(("inside SubImage2D line=%d\n",line));
DEBUG_PRINT(("inside SubImage2D paddings=%d\n",paddings));
DEBUG_PRINT(("inside SubImage2D xstart=%d\n",xstart));
DEBUG_PRINT(("inside SubImage2D xoffset=%d\n",xoffset));
DEBUG_PRINT(("inside SubImage2D yoffset=%d\n",yoffset));
DEBUG_PRINT(("inside SubImage2D tSize=%d\n",tSize));
DEBUG_PRINT(("inside SubImage2D width=%d\n",width));
DEBUG_PRINT(("inside SubImage2D height=%d\n",height));
DEBUG_PRINT(("level=%d\n",level));
DEBUG_PRINT(("texelUbytes=%d\n", tSize));

/*	DEBUG_PRINT(("REZULT=%x\n",yoffset * line + xoffset * tSize));
	DEBUG_PRINT(("FITST=%x\n",yoffset * line)) ;
	DEBUG_PRINT(("SECOND=%x\n",xoffset * tSize ));
	DEBUG_PRINT(("xoffset=%x\n",xoffset)) ;
	DEBUG_PRINT(("yoffset=%x\n",yoffset ));
	DEBUG_PRINT(("yoffset+second=%x\n",yoffset + xoffset * tSize));
	DEBUG_PRINT(("yoffset+ubyte=%x\n",yoffset + tSize));
*/
	//xstart=yoffset*line+xoffset*4;
	for(i=0;i<height;i++)
	{
		//for(j=0;j<width*size;j++)
		for(j=0;j<width*tSize;j++)
		{
			//*((unsigned char*)ActiveTexObjectP->texImages2D[level].pixels+xstart+i*line+j)=*((unsigned char*)pixels+i*nline+j);
			*((NMGLubyte*)ActiveTexObjectP->texImages2D[level].pixels+xstart+i*line+j) = *((NMGLubyte*)pixels + i * (width*tSize + paddings)+j);
		}	
	}
	//DEBUG_PRINT1(("texObjects[0].pixels[%d][0,0]=%x\n",level,*((NMGLubyte*)ActiveTexObjectP->texImages2D[level].pixels)));
//DEBUG_PRINT(("texObjects[0].pixels[%d][0,0]=%x\n",level,*((NMGLubyte*))ActiveTexObjectP->texImages2D[level].pixels));
	//DEBUG_PRINT1(("level=%d\n",level));

	//DEBUG_PRINT1(("xstart=%d\n",xstart));

	//DEBUG_PRINT1(("line=%d\n",line));


}
