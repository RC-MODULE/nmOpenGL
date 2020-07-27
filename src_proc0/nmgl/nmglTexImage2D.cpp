//#include "demo3d_nm0.h"
#if 0
#include "nmgl.h"

#pragma code_section ".text_nmgl"

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

	if ((width<0)||(height<0)||(width>NMGL_MAX_TEX_SIZE)||(height>NMGL_MAX_TEX_SIZE)) {cntxt.error=NMGL_INVALID_VALUE;return;}
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
#endif