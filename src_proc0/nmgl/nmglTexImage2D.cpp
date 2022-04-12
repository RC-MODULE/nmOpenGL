

//#include "nmgl.h"
#include "demo3d_nm0.h"
#include "debugprint.h"
//#include "malloc.h" //func malloc
//#include "nmtype.h" //func malloc32
//#define DEBUG
SECTION(".data_imu1")  NMGLubyte* mipmap; //texture memory

//Alternative: #pragma data_section ".data_imu1"
//     double x1[SIZE];

	
#pragma code_section ".text_nmgl"
//============================================================================================================
int isPowerOf2(NMGLint x);// check, if x is power of 2
int copyPixels(const void* pfrom,NMGLint format,NMGLint width,NMGLint height,void** pto,NMGL_Context_NM0 *cntxt);//mem allocation
void initLvls(NMGLuint name, NMGLsizei width, NMGLsizei height,NMGLenum format);

NMGLint getTexelSizeUbytes(NMGLint format); //moved to nmgltex_common.h


void nmglTexImage2D(NMGLenum target, NMGLint level, NMGLint internalformat, NMGLsizei width, NMGLsizei height, NMGLint border, NMGLenum format, NMGLenum type, const void *pixels);
//============================================================================================================
//---------------------------------------------------------------------
/*
Texel size in NMGLubytes
*/
NMGLint getTexelSizeUbytes(NMGLint format)
{
	DEBUG_PLOG_FUNCTION();
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
		case NMGL_COLOR_INDEX8_EXT: 
			return COLOR_TEXEL_SIZE_UBYTE;
		case NMGL_COLOR_INDEX: 
			return COLOR_TEXEL_SIZE_UBYTE;	
		default:
			return RGBA_TEXEL_SIZE_UBYTE;
	}

}//---------------------------------------------------------------------
/*
	Инициализация указателей на мип мап уровни
*/
void initLvls(NMGLuint name,NMGL_Context_NM0 *cntxt)
{
	//MIPMAP_OBJ_SIZE
	int i=0;
	unsigned long cursize=NMGL_MAX_TEX_SIDE*NMGL_MAX_TEX_SIDE*UBYTES_PER_TEXEL;
//DEBUG_PRINT(("Initlvls:texture:%d firstFreeTexByte=%x\n",name,cntxt->texState.firstFreeTexByte));
	#if 0
	/*
	if(cntxt->texState.firstFreeTexByte==NULL)
	{
		cntxt->texState.firstFreeTexByte=(void*)mipmap;	
		DEBUG_PRINT(("mipmap=0x%x\n",mipmap));	
	}
	cntxt->texState.texObjects[name].texImages2D[0].pixels=cntxt->texState.firstFreeTexByte;
	*/
/*sync*/
/*cntxt->synchro.writeInstr(1, NMC1_SET_MIPMAP_LVL_POINTER,(int)(name),0,(int)cntxt->texState.firstFreeTexByte);
	for(i=1;i<=NMGL_MAX_MIPMAP_LVL;i++)
	{
		cntxt->texState.texObjects[name].texImages2D[i].pixels=(void*)((NMGLubyte *)cntxt->texState.texObjects[name].texImages2D[i-1].pixels+cursize);
*/
/*sync*/
/*cntxt->synchro.writeInstr(1, NMC1_SET_MIPMAP_LVL_POINTER,(int)(name),i,(int)((NMGLubyte *)cntxt->texState.texObjects[name].texImages2D[i].pixels));

		cursize>>=2;
	}
	cntxt->texState.firstFreeTexByte=(void*)((NMGLubyte *)cntxt->texState.texObjects[name].texImages2D[NMGL_MAX_MIPMAP_LVL].pixels+UBYTES_PER_TEXEL);
	*/
	#endif

	cntxt->texState.texObjects[name].texImages2D[0].pixels=(void*)(mipmap + (name*MIPMAP_OBJ_SIZE));


	NM_Command command;
	command.instr = NMC1_SET_MIPMAP_LVL_POINTER;
	command.params[0] = CommandArgument(name);
	command.params[1] = CommandArgument(0);
	command.params[2] = CommandArgument(mipmap + (name * MIPMAP_OBJ_SIZE));
	cntxt->synchro.pushInstr(&command);
///*sync*/cntxt->synchro.writeInstr(1, NMC1_SET_MIPMAP_LVL_POINTER,(int)(name),0,(int)((void*)(mipmap + (name*MIPMAP_OBJ_SIZE))));

	for(i=1;i<=NMGL_MAX_MIPMAP_LVL;i++)
	{
		cntxt->texState.texObjects[name].texImages2D[i].pixels=(void*)((NMGLubyte *)cntxt->texState.texObjects[name].texImages2D[i-1].pixels+cursize);



		command.instr = NMC1_SET_MIPMAP_LVL_POINTER;
		command.params[0] = CommandArgument(name);
		command.params[1] = CommandArgument(i);
		command.params[2] = CommandArgument((NMGLubyte *)cntxt->texState.texObjects[name].texImages2D[i].pixels);
		cntxt->synchro.pushInstr(&command);
///*sync*/cntxt->synchro.writeInstr(1, NMC1_SET_MIPMAP_LVL_POINTER,(int)(name),i,(int)((NMGLubyte *)cntxt->texState.texObjects[name].texImages2D[i].pixels));



		cursize>>=2;
	}
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
int copyPixels(const void* pfrom,NMGLint format,NMGLint width,NMGLint height,void** pto,NMGL_Context_NM0 *cntxt)
{
    NMGLint size=getTexelSizeUbytes(format);
    int i=0,j=0;
    //NMGLint xlen=0;
    int line=width*size;
    NMGLint xw=width;
    NMGLint xh=height;
	NMGLubyte* pfrom_cur;
	
	int padLength=0;
	int unpackAlign=cntxt->unpackAlignment;
    //size=4;//32bit word
   if(*pto==NULL)
   {
	   DEBUG_PRINT(("nmglTexImage2D:copypixels:bad pto pointer!\n"));
	   return -1;
   }
    

	//printf("w=%d h=%d s=%d\n",xw,xh,size);
    //if(*pto != NULL) free(*pto);

	//*pto=malloc(4*xlen);
	
   // if (*pto==0) return 0;
	if(unpackAlign == 1)
	{
	    for(i=0;i<xw*xh*size;i++)
	    {
	       //  DEBUG_PRINT(("ACTIVE_ARRAY="));
	        *((NMGLubyte*)*pto+i)=*((NMGLubyte*)pfrom+i)&0xff;

	       //  DEBUG_PRINT(( "%d\n", *((NMGLint*)*pto+i) ));
	        //REMAKE if cant have byte access
	    }    
	   // DEBUG_PRINT(("=========\n"));
	}
	else
	{
		pfrom_cur=(NMGLubyte*)pfrom;
		padLength=(line % unpackAlign == 0) ? 0 : (unpackAlign - line % unpackAlign);
		DEBUG_PRINT1(("width=%d height=%d unpackAlign=%d padLength=%d\n",width,height, unpackAlign,padLength));
		for(j=0;j<height;j++)
	    {
	       for(i=0;i<line;i++)
	    	{
	       
	        	*((NMGLubyte*)*pto+j*line+i)=*((NMGLubyte*)pfrom_cur+i)&0xff;	       
	    	}
			pfrom_cur+=(line+padLength);
	    }
	}

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


	if((internalformat == NMGL_COLOR_INDEX8_EXT)&&(format == NMGL_COLOR_INDEX))
	{

	}
	else
	{
		if(internalformat != format)  {cntxt->error=NMGL_INVALID_VALUE;return;}
		if((internalformat !=NMGL_RGBA)&&(internalformat !=NMGL_RGB)&&(internalformat !=NMGL_ALPHA)&&(internalformat !=NMGL_LUMINANCE)&&(internalformat !=NMGL_LUMINANCE_ALPHA)){cntxt->error=NMGL_INVALID_VALUE;return;}

	}
	
	
	if( ActiveTexObjectP->imageIsSet == 1 ) 
	{			
		if(ActiveTexObjectP->texImages2D[level].internalformat!=internalformat){cntxt->error=NMGL_INVALID_OPERATION;return;}
		if((ActiveTexObjectP->texImages2D[level].width != width)||(ActiveTexObjectP->texImages2D[level].height != height)){cntxt->error=NMGL_INVALID_OPERATION;return;}		
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
				//ActiveTexObjectP->texImages2D[iter].level=iter;//should be deleted as is not used
				//ActiveTexObjectP->texImages2D[level].border=border;//should be deleted as is not used
				if(iter == 0)
				{
					ActiveTexObjectP->texImages2D[iter].width=width;
					ActiveTexObjectP->texImages2D[iter].height=height;		

					NM_Command command;
					command.instr = NMC1_SET_WHF;
					command.params[0] = CommandArgument(ActiveTexObjectP->name);
					command.params[1] = CommandArgument(width);
					command.params[2] = CommandArgument(height);
					command.params[3] = CommandArgument(internalformat);
					cntxt->synchro.pushInstr(&command);
///*sync*/cntxt->synchro.writeInstr(1, NMC1_SET_WHF,(int)(ActiveTexObjectP->name),width,height,internalformat);

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
	s=copyPixels(pixels,internalformat,width,height,&ActiveTexObjectP->texImages2D[level].pixels,cntxt);
	ActiveTexObjectP->imageIsSet=1;
}
//	cntxt->synchro.writeInstr(1, NMC1_SET_ACTIVE_TEXTURE, (unsigned int)activeTexUnitIndex);
