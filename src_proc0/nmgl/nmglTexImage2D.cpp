#if 0
#include "nmgl.h"
#include  "nmgl_data0.h"
#pragma code_section ".text_nmgl"
//============================================================================================================
int isPowerOf2(NMGLint x);
int copyPixels(const void* pfrom,NMGLint format,NMGLint width,NMGLint height,void** pto);
//============================================================================================================

/*
1 - if x is power of 2
0 - if x is not power of 2
*/
int isPowerOf2(NMGLint x)
{
    return (x && !(x & (x-1))); 
}
/*
Writes rectangular array of pixels of specified format from pfrom to pto
1 - allocation was succesful
0 - failed
*/
int copyPixels(const void* pfrom,NMGLint format,NMGLint width,NMGLint height,void** pto)
{
    int size=0;
    int i=0;
    NMGLint xlen=0;
    NMGLint xw=width;
    NMGLint xh=height;
    size=4;//32bit word
    xlen=xw*xh;

    if(*pto != NULL) free(*pto);
    *pto=malloc(4*xlen);
    if (*pto==0) return 0;

    for(i=0;i<xlen;i++)
    {
       //  DEBUG_PRINT(("ACTIVE_ARRAY="));
        *((NMGLint*)*pto+i)=*((NMGLint*)pfrom+i);

       //  DEBUG_PRINT(( "%d\n", *((NMGLint*)*pto+i) ));
        //REMAKE if cant have byte access
    }    
   // DEBUG_PRINT(("=========\n"));
    return 1;
}
//============================================================================================================
void nmglTexImage2D(NMGLenum target, NMGLint level, NMGLint internalformat, NMGLsizei width, NMGLsizei height, NMGLint border, NMGLenum format, NMGLenum type, const void *pixels)
{
	NMGLint cur=0;
	NMGLint ccc=0;
	int iter=0;
	int s=0;
	if (target !=NMGL_TEXTURE_2D) {cntxt.error=NMGL_INVALID_ENUM;return;}
	if (level < 0)  {cntxt.error=NMGL_INVALID_VALUE;return;}
	if (border !=0) {cntxt.error=NMGL_INVALID_VALUE;return;}	
	if (type !=NMGL_UNSIGNED_BYTE) {cntxt.error=NMGL_INVALID_ENUM;return;}

	if ((width<0)||(height<0)||(width>NMGL_MAX_TEX_SIDE)||(height>NMGL_MAX_TEX_SIDE)) {cntxt.error=NMGL_INVALID_VALUE;return;}
	if((!isPowerOf2(width))||(!isPowerOf2(height))){cntxt.error=NMGL_INVALID_VALUE;return;}



	if(internalformat != format)  {cntxt.error=NMGL_INVALID_VALUE;return;}
	if((internalformat !=NMGL_RGBA)&&(internalformat !=NMGL_RGB)&&(internalformat !=NMGL_ALPHA)&&(internalformat !=NMGL_LUMINANCE)&&(internalformat !=NMGL_LUMINANCE_ALPHA)){cntxt.error=NMGL_INVALID_VALUE;return;}

	if( ActiveTexObject->imageIsSet == 1 ) 
	{
		
		//TODO: maybe just add a flag; 
		if((ActiveTexObject->texMinFilter == NMGL_NEAREST_MIPMAP_NEAREST) ||(ActiveTexObject->texMinFilter == NMGL_NEAREST_MIPMAP_LINEAR) ||(ActiveTexObject->texMinFilter == NMGL_LINEAR_MIPMAP_NEAREST)||(ActiveTexObject->texMinFilter == NMGL_LINEAR_MIPMAP_LINEAR))
		{
			//mipmapping is enabled
			if(level>NMGL_MAX_MIPMAP_LEVEL){cntxt.error=NMGL_INVALID_VALUE;return;}
			if(ActiveTexObject->texImages2D[level].internalformat!=internalformat){cntxt.error=NMGL_INVALID_OPERATION;return;}
			if((ActiveTexObject->texImages2D[level].width != width)||(ActiveTexObject->texImages2D[level].height != height)){cntxt.error=NMGL_INVALID_OPERATION;return;}
			ActiveTexObject->texImages2D[level].width=width;//relia only;maybe delete
			ActiveTexObject->texImages2D[level].height=height;//relia only;maybe delete			
			/*
				s=fillpixels(pixels,internalformat,width,height,ActiveTexObject->texImages2D[level].pixels);
			*/
		}
		else{
			//mipmapping is disabled
			if(level != 0){cntxt.error=NMGL_INVALID_OPERATION;return;}
			if(ActiveTexObject->texImages2D[0].internalformat!=internalformat){cntxt.error=NMGL_INVALID_OPERATION;return;}
			if((ActiveTexObject->texImages2D[0].width!=width)||(ActiveTexObject->texImages2D[0].height!=height)) {cntxt.error=NMGL_INVALID_OPERATION;return;}
			/*
			s=fillpixels(pixels,internalformat,width,height,ActiveTexObject->texImages2D[0].pixels);
			*/

		}
	}
	else
	{
		//first run for this texture object;
		if(level==0)
		{

			
			for(iter=0;iter<=NMGL_MAX_MIPMAP_LEVEL;iter++)
			{
				ActiveTexObject->texImages2D[iter].internalformat=internalformat;
				ActiveTexObject->texImages2D[iter].level=iter;//should be deleted as is not used
				ActiveTexObject->texImages2D[level].border=border;//should be deleted as is not used
				if(iter == 0)
				{
					ActiveTexObject->texImages2D[iter].width=width;
					ActiveTexObject->texImages2D[iter].height=height;
				}
				else{
					if( ActiveTexObject->texImages2D[0].width>>iter > 0  )
					{
						ActiveTexObject->texImages2D[iter].width=ActiveTexObject->texImages2D[0].width>>iter;
					}
					else{
						ActiveTexObject->texImages2D[iter].width=1;
					}
				}

				if( ActiveTexObject->texImages2D[0].height>>iter > 0  )
				{
					ActiveTexObject->texImages2D[iter].height=ActiveTexObject->texImages2D[0].height>>iter;
				}
				else
				{
					ActiveTexObject->texImages2D[iter].height=1;
				}

			}		
		}
		else
		{
			cntxt.error=NMGL_INVALID_OPERATION;return;// cant load levels other then 0
		}

	
		/*
		s=fillpixels(pixels,internalformat,width,height,ActiveTexObject->texImages2D[level].pixels);
		*/

	}
	s=copyPixels(pixels,internalformat,width,height,&ActiveTexObject->texImages2D[level].pixels);
	ActiveTexObject->imageIsSet=1;
}
//	cntxt.synchro.writeInstr(1, NMC1_SET_ACTIVE_TEXTURE, (unsigned int)activeTexUnitIndex);
#endif