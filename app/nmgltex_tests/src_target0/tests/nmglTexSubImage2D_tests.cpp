
#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "tests.h"
#include "nmgl_tex_test_common.h"
#include "tests.h"


#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif

extern NMGLubyte mipmap[MIPMAP_MEM_SIZE];
extern NMGLint init_TexImage2D_input(TexImage2D_data* data,NMGLint width,internalformatdata internalformat,NMGLint lvl=0);
//extern NMGLubyte texels[(USED_SIDE)*(USED_SIDE)*(UBYTES_PER_TEXEL)];
extern void fillPixels (void **pixels,NMGLint size,NMGLint width,int sfiller=-1);
extern int copyPixels(const void* pfrom,NMGLint format,NMGLint width,NMGLint height,void** pto,NMGL_Context_NM0 *cntxt);//mem allocation
extern void initLvls(NMGLuint name,NMGL_Context_NM0 *cntxt);
extern NMGLubyte mipmap[MIPMAP_MEM_SIZE];
int fillMipMap( int texture,NMGLint format,NMGLint width,NMGLint height);
extern NMGL_Context_NM0 *cntxt;
extern void* cntxtAddr_nm1;
NMGL_Context_NM1 *nm1_cntxt;
/*
From OpenGL SC 1.0.1 spec:
//////////////////////////////////////////////////////////////////
Basic restrictions for texturing(only supported items are listed):
//////////////////////////////////////////////////////////////////
Image type:			UNSIGNED_BYTE
Internal format:	RGBA,RGB,LUMINANCE,ALPHA,LUMINANCE_ALPHA


The format must match the base internal format (no conversions from
one format to another during texture image processing are supported.
//////////////////////////////////////////////////////////////////
void nmglTexSubImage2D (
NMGLenum target, 
NMGLint level, 
NMGLint xoffset, 
NMGLint yoffset, 
NMGLsizei width, 
NMGLsizei height, 
NMGLenum format, 
NMGLenum type, 
const void *pixels);
//////////////////////////////////////////////////////////////////
	
*/


/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии

int nmglTexSubImage2D_TexSubImage_contextStateCorrect();
int nmglTexSubImage2D_wrongArgs_isError();
/////////////////////////////////////////////////////////////////////////////////////////////////

#define MIPMAP cntxt->texState.texObjects[texture].texImages2D
int fillMipMap( int texture,NMGLint format,NMGLint width,NMGLint height)
{
    int i=0,j=0;
    int c=0;
	static int filler=0x0;    
    int size=4;   
    
    for(i=0;i<=NMGL_MAX_MIPMAP_LVL;i++)
        {
            MIPMAP[i].internalformat=format;           
            if(i>0)
			{
				c=width>>i;
				c=height>>i;
			}
			else{
				c=width;
				c=height;
			}
            if(c>1)
			{
				MIPMAP[i].width=c;
				MIPMAP[i].height=c;
			}
			else
			{
				MIPMAP[i].width=1;
				 MIPMAP[i].height=1;
			}     
                     
            for(j=0;j<MIPMAP[i].width*MIPMAP[i].height*UBYTES_PER_TEXEL;j++)
            {
               // *((unsigned char*)MIPMAP[i].pixels+j)=(unsigned char)filler;
                *((NMGLubyte*)MIPMAP[i].pixels+j)=(NMGLubyte)filler++;
            }
        }
    return 0;
}
//========================================================================================
//-----------------------------------------------------------------------------------------------------
int cmpRefreshPixels(void* active,void *initial,void *subimage,int pixsizebytes,NMGLint width,NMGLint height,int xstart,int ystart,int wsub,int hsub)
{
    int i=0;
    int j=0;
    int line=width;
    int subline=wsub;
    //int startsubx=
   // int startsuby=
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            if((i>=ystart)&&(i<ystart+hsub)&&(j>=xstart)&&(j<xstart+wsub))
            {   //inside subimage

               DEBUG_PRINT1(("inside\n"));
               DEBUG_PRINT1(("pixel[%d,%d]_a=%d\n",j,i,*((NMGLint *)active+i*line+j) ));
               DEBUG_PRINT1(("pixel[%d,%d]_t=%d\n",j,i,*((NMGLint *)active+i*line+j) ));
               DEBUG_PRINT1(("pixel[%d,%d]_s=%d\n\n",j-xstart,i-ystart,*((NMGLint *)subimage+(i-ystart)*subline+j-xstart) ));

               if( *((NMGLubyte *)active+i*line+j) == *((NMGLubyte *)subimage+(i-ystart)*subline+j-xstart))
               {
                     *((NMGLubyte *)initial+i*line+j)=*((NMGLubyte *)subimage+(i-ystart)*subline+j-xstart);
               }
               else
               {
                   return 0;
               }
            }
            else
            { //outside

            DEBUG_PRINT1(("outside\n"));
                DEBUG_PRINT1(("pixel[%d,%d]_a=%d\n",j,i,*((NMGLint *)active+i*line+j) ));
                DEBUG_PRINT1(("pixel[%d,%d]_t=%d\n",j,i,*((NMGLint *)active+i*line+j) ));
                DEBUG_PRINT1(("pixel[%d,%d]_s=%d\n\n",j-xstart,i-ystart,*((NMGLint *)subimage+(i-ystart)*subline+j-xstart) ));

               if( *((NMGLubyte *)active+i*line+j) != *((NMGLubyte *)initial+i*line+j))
               {
                   DEBUG_PRINT(("Error! Pixels array was corrupted by TexSubImage2D\n"));
                   return 0;
               } 
            }
        }
    }
    return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

int run_nmglTexSubImage2D_test()
{
	int i=0;
	printf ("\nStart TexSubImage2D tests\n\n");
	cntxt = NMGL_Context_NM0::getContext();
	nm1_cntxt = (NMGL_Context_NM1*)cntxtAddr_nm1;

	for(i=0;i<NMGL_MAX_TEX_OBJECTS;i++)
	{
		initLvls(i,cntxt);
	}


	RUN_TEST(nmglTexSubImage2D_wrongArgs_isError);
	RUN_TEST(nmglTexSubImage2D_TexSubImage_contextStateCorrect);	
	
	printf ("\nEnd TexSubImage2D tests\n");
	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================


int nmglTexSubImage2D_TexSubImage_contextStateCorrect()
{
	
	void *pixels=&mipmap[MIPMAP_MEM_SIZE-2*MIPMAP_OBJ_SIZE];
	NMGLubyte *testarray=&mipmap[MIPMAP_MEM_SIZE-MIPMAP_OBJ_SIZE];
	int sss=0;
	int i=0;
	int j=0;
	int k=0;
	
	int curw=USED_SIDE>>1;
	int curh=USED_SIDE>>1;
	int curB=0;	
	int sfiller=0;
	int picSize=curw*curh*UBYTES_PER_TEXEL;
	TexImage2D texImages2D[NMGL_MAX_MIPMAP_LVL+1];
	int _accum=0;//for error accumulation
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;

	fillPixels (&pixels,picSize,curw,sfiller++);


	fillMipMap(0,NMGL_RGBA,USED_SIDE,USED_SIDE);

	cntxt->texState.texObjects[0].imageIsSet=1;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];
	
	for(k=0;k<=USED_MIPMAP_LVL;k++) 
	{
		
		for(i=0;i<=(USED_SIDE>>k)-curw;i=i+(USED_SIDE>>k)-curw)
		{
			for(j=0;j<=(USED_SIDE>>k)-curh;j=j+(USED_SIDE>>k)-curh)
			{
#ifdef DEBUG
				if((k==0)&&(i<2)&&(j<2))
				{
				DEBUG_PRINT(("_______________________________________\n"));
				DEBUG_PRINT(("Level=%d	startx=%d  starty=%d\n",k,j,i));
				DEBUG_PRINT(("SubWidth=%d	SubHeight=%d \n",curw,curh));
				}
#endif
				copyPixels(ActiveTexObjectP->texImages2D[k].pixels,ActiveTexObjectP->texImages2D[k].internalformat,ActiveTexObjectP->texImages2D[k].width,ActiveTexObjectP->texImages2D[k].height,(void**)&testarray,cntxt);


				nmglTexSubImage2D (NMGL_TEXTURE_2D, k, i, j, curw, curh, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
	
			DEBUG_PRINT(("k=%d i=%d j=%d\n",k,i,j));						
				TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
				TEST_ASSERT(cmpRefreshPixels(ActiveTexObjectP->texImages2D[k].pixels,testarray,pixels,getTexelSizeUbytes(ActiveTexObjectP->texImages2D[k].internalformat),ActiveTexObjectP->texImages2D[k].width,ActiveTexObjectP->texImages2D[k].height,i,j,curw,curh) == 1 );
				fillPixels (&pixels,picSize,curw,sfiller++);
				if((USED_SIDE>>k)-curh == 0) j=1;				
			}
			if((USED_SIDE>>k)-curw == 0) i=1;
		}
		if(curw > 1) curw>>=1;
		if(curh > 1) curh>>=1;
		picSize=curw*curh*UBYTES_PER_TEXEL;
		sfiller=0;
		fillPixels (&pixels,picSize,curw,sfiller++);
	}	
	TEST_ASSERT(_accum==0);	
	curw=USED_SIDE;
	curh=USED_SIDE;
	copyPixels(ActiveTexObjectP->texImages2D[0].pixels,ActiveTexObjectP->texImages2D[0].internalformat,ActiveTexObjectP->texImages2D[0].width,ActiveTexObjectP->texImages2D[0].height,(void**)&testarray,cntxt);


	nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 0, 0, curw, curh, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);	
						
	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
	TEST_ASSERT(cmpRefreshPixels(ActiveTexObjectP->texImages2D[0].pixels,testarray,pixels,getTexelSizeUbytes(ActiveTexObjectP->texImages2D[0].internalformat),ActiveTexObjectP->texImages2D[0].width,ActiveTexObjectP->texImages2D[0].height,0,0,curw,curh) == 1 );
	
return 0;
	
}
//-----------------------------------------------------------------
int nmglTexSubImage2D_wrongArgs_isError()
{
	void *pixels;
	int sss=0;
	int _accum=0;
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;
	
	fillMipMap(0,NMGL_RGBA,USED_SIDE,USED_SIDE);
	
	
	cntxt->texState.texObjects[0].imageIsSet=1;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];
	pixels=&mipmap[MIPMAP_MEM_SIZE-2*MIPMAP_OBJ_SIZE];
	
	if(pixels==0) {DEBUG_PRINT(("Error!Cant allocate mem for test mipmap!\n"));return 2;}
//__________________normal_________________________________________________
		fillPixels(&pixels,UBYTES_PER_TEXEL*((USED_SIDE*USED_SIDE)>>4),USED_SIDE>>2);
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_NO_ERROR);

//__________________bad target_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D+1, 0, 1, 1, USED_SIDE>>2,USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_ENUM);
		cntxt->error=NMGL_NO_ERROR;

//__________________bad level_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, -1, 1, 1, USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;	

		nmglTexSubImage2D (NMGL_TEXTURE_2D, NMGL_MAX_MIPMAP_LVL+1, 1, 1, USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;
		
//__________________bad width/height_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, -1, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;		

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, USED_SIDE>>2, -1, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, USED_SIDE, 8, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;		

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, 8, USED_SIDE, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;
			
//__________________bad x/yoffset_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, -1, 1, USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;	

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, -1,USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, USED_SIDE+1, 1,USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;	

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, USED_SIDE+1,USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;
//__________________bad format_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1,USED_SIDE>>2, USED_SIDE>>2, NMGL_RGB, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_OPERATION);
		cntxt->error=NMGL_NO_ERROR;	

//__________________bad type_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1,USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE+1, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_ENUM);

		TEST_ASSERT(_accum == 0);
		return 0;
}
