

#include "nmgl.h"
#include "demo3d_nm0.h"
#include "malloc.h" //func malloc
#include "nmtype.h" //func malloc32
//#define DEBUG
SECTION(".textures_mipmap_mem") volatile NMGLubyte mipmap[MIPMAP_MEM_SIZE];

//Alternative: #pragma data_section ".data_imu1"
//     double x1[SIZE];

	
#pragma code_section ".text_nmgl"
//============================================================================================================
int isPowerOf2(NMGLint x);// check, if x is power of 2
int copyPixels(const void* pfrom,NMGLint format,NMGLint width,NMGLint height,void** pto);//mem allocation
void initLvls(NMGLuint name, NMGLsizei width, NMGLsizei height,NMGLenum format);
int getTexelSizeUbytes(NMGLint format);

void nmglTexImage2D(NMGLenum target, NMGLint level, NMGLint internalformat, NMGLsizei width, NMGLsizei height, NMGLint border, NMGLenum format, NMGLenum type, const void *pixels);
//============================================================================================================
/*
Определение размера текселя в 32-битных словах
*/
int getTexelSizeUbytes(NMGLint format)
{
	switch(format)
	{
		case NMGL_RGB:
			return RGB_TEXEL_SIZE_UBYTE;
		case NMGL_RGBA:
			return RGBA_TEXEL_SIZE_UBYTE;
		case NMGL_LUMINANCE_ALPHA:
			return LUMINANCE_ALPHA_TEXEL_SIZE_UBYTE;	
		case NMGL_LUMINANCE:
			return LUMINANCE_TEXEL_SIZE_UBYTE;
		case NMGL_ALPHA:
			return ALPHA_TEXEL_SIZE_UBYTE;
		default:
			return RGBA_TEXEL_SIZE_UBYTE;
	}
}
/*
	Инициализация указателей на мип мап уровни
*/
void initLvls(NMGLuint name,NMGL_Context_NM0 *cntxt)
{
	//MIPMAP_OBJ_SIZE
	int i=0;
	unsigned long cursize=NMGL_MAX_TEX_SIDE*NMGL_MAX_TEX_SIDE*UBYTES_PER_TEXEL;

	if(cntxt->texState.firstFreeTexByte==NULL) cntxt->texState.firstFreeTexByte=(void*)mipmap;
	cntxt->texState.texObjects[name-1].texImages2D[0].pixels=cntxt->texState.firstFreeTexByte;
	for(i=1;i<=NMGL_MAX_MIPMAP_LEVEL;i++)
	{
		cntxt->texState.texObjects[name-1].texImages2D[i].pixels=(void*)((NMGLubyte *)cntxt->texState.texObjects[name-1].texImages2D[i-1].pixels+cursize);
		cursize>>=2;
	}
	cntxt->texState.firstFreeTexByte=(void*)((NMGLubyte *)cntxt->texState.texObjects[name-1].texImages2D[NMGL_MAX_MIPMAP_LEVEL].pixels+UBYTES_PER_TEXEL);
}
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
    int size=getTexelSizeUbytes(format);
    int i=0;
    NMGLint xlen=0;
    NMGLint xw=width;
    NMGLint xh=height;
    //size=4;//32bit word
   if(*pto==NULL)
   {
	   DEBUG_PRINT(("nmglTexImage2D:copypixels:bad pto pointer!\n"));
	   return -1;
   }
    xlen=xw*xh*size;

    //if(*pto != NULL) free(*pto);

	//*pto=malloc(4*xlen);
	
   // if (*pto==0) return 0;

    for(i=0;i<xlen;i++)
    {
       //  DEBUG_PRINT(("ACTIVE_ARRAY="));
        *((NMGLubyte*)*pto+i)=*((NMGLubyte*)pfrom+i);

       //  DEBUG_PRINT(( "%d\n", *((NMGLint*)*pto+i) ));
        //REMAKE if cant have byte access
    }    
   // DEBUG_PRINT(("=========\n"));
    return 0;
}
//============================================================================================================
void nmglTexImage2D(NMGLenum target, NMGLint level, NMGLint internalformat, NMGLsizei width, NMGLsizei height, NMGLint border, NMGLenum format, NMGLenum type, const void *pixels)
{
	NMGLint cur=0;
	NMGLint ccc=0;
	int iter=0;
	int s=0;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (target !=NMGL_TEXTURE_2D) {cntxt->error=NMGL_INVALID_ENUM;return;}
	if (level < 0)  {cntxt->error=NMGL_INVALID_VALUE;return;}
	if (level > NMGL_MAX_MIPMAP_LVL)  {cntxt->error=NMGL_INVALID_VALUE;return;}
	if (border !=0) {cntxt->error=NMGL_INVALID_VALUE;return;}	
	if (type !=NMGL_UNSIGNED_BYTE) {cntxt->error=NMGL_INVALID_ENUM;return;}

	if ((width<0)||(height<0)||(width>NMGL_MAX_TEX_SIDE)||(height>NMGL_MAX_TEX_SIDE)) {cntxt->error=NMGL_INVALID_VALUE;return;}
	if((!isPowerOf2(width))||(!isPowerOf2(height))){cntxt->error=NMGL_INVALID_VALUE;return;}



	if(internalformat != format)  {cntxt->error=NMGL_INVALID_VALUE;return;}
	if((internalformat !=NMGL_RGBA)&&(internalformat !=NMGL_RGB)&&(internalformat !=NMGL_ALPHA)&&(internalformat !=NMGL_LUMINANCE)&&(internalformat !=NMGL_LUMINANCE_ALPHA)){cntxt->error=NMGL_INVALID_VALUE;return;}

	if( ActiveTexObjectP->imageIsSet == 1 ) 
	{
		
		//TODO: maybe just add a flag; 
		if((ActiveTexObjectP->texMinFilter == NMGL_NEAREST_MIPMAP_NEAREST) ||(ActiveTexObjectP->texMinFilter == NMGL_NEAREST_MIPMAP_LINEAR) ||(ActiveTexObjectP->texMinFilter == NMGL_LINEAR_MIPMAP_NEAREST)||(ActiveTexObjectP->texMinFilter == NMGL_LINEAR_MIPMAP_LINEAR))
		{
			//mipmapping is enabled
			//if(level>NMGL_MAX_MIPMAP_LVL){cntxt->error=NMGL_INVALID_VALUE;return;}
			if(ActiveTexObjectP->texImages2D[level].internalformat!=internalformat){cntxt->error=NMGL_INVALID_OPERATION;return;}
			if((ActiveTexObjectP->texImages2D[level].width != width)||(ActiveTexObjectP->texImages2D[level].height != height)){cntxt->error=NMGL_INVALID_OPERATION;return;}
			ActiveTexObjectP->texImages2D[level].width=width;//relia only;maybe delete
			ActiveTexObjectP->texImages2D[level].height=height;//relia only;maybe delete			
			/*
				s=fillpixels(pixels,internalformat,width,height,ActiveTexObjectP->texImages2D[level].pixels);
			*/
		}
		else{
			//mipmapping is disabled
			if(level != 0){cntxt->error=NMGL_INVALID_OPERATION;return;}
			if(ActiveTexObjectP->texImages2D[0].internalformat!=internalformat){cntxt->error=NMGL_INVALID_OPERATION;return;}
			if((ActiveTexObjectP->texImages2D[0].width!=width)||(ActiveTexObjectP->texImages2D[0].height!=height)) {cntxt->error=NMGL_INVALID_OPERATION;return;}
			/*
			s=fillpixels(pixels,internalformat,width,height,ActiveTexObjectP->texImages2D[0].pixels);
			*/

		}
	}
	else
	{
		//first run for this texture object;
		if(level==0)
		{
			initLvls(ActiveTexObjectP->name,cntxt);
			
			for(iter=0;iter<=NMGL_MAX_MIPMAP_LVL;iter++)
			{
				ActiveTexObjectP->texImages2D[iter].internalformat=internalformat;
				ActiveTexObjectP->texImages2D[iter].level=iter;//should be deleted as is not used
				ActiveTexObjectP->texImages2D[level].border=border;//should be deleted as is not used
				if(iter == 0)
				{
					ActiveTexObjectP->texImages2D[iter].width=width;
					ActiveTexObjectP->texImages2D[iter].height=height;
				}
				else{
					if( ActiveTexObjectP->texImages2D[0].width>>iter > 0  )
					{
						ActiveTexObjectP->texImages2D[iter].width=ActiveTexObjectP->texImages2D[0].width>>iter;
					}
					else{
						ActiveTexObjectP->texImages2D[iter].width=1;
					}
					if( ActiveTexObjectP->texImages2D[0].height>>iter > 0  )
					{
						ActiveTexObjectP->texImages2D[iter].height=ActiveTexObjectP->texImages2D[0].height>>iter;
					}
					else
					{
						ActiveTexObjectP->texImages2D[iter].height=1;
					}
				}
			}		
		}
		else
		{
			cntxt->error=NMGL_INVALID_OPERATION;return;// cant load levels other then 0
		}

	
		/*
		s=fillpixels(pixels,internalformat,width,height,ActiveTexObjectP->texImages2D[level].pixels);
		*/

	}
	s=copyPixels(pixels,internalformat,width,height,&ActiveTexObjectP->texImages2D[level].pixels);
	ActiveTexObjectP->imageIsSet=1;
}
//	cntxt->synchro.writeInstr(1, NMC1_SET_ACTIVE_TEXTURE, (unsigned int)activeTexUnitIndex);
