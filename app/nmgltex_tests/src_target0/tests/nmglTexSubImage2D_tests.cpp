
#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "tests.h"
#include "nmgl_tex_test_common.h"
#include "tests.h"

#define FAST // comment to turn off fast test
#ifdef FAST
	#define XINC ((USED_SIDE>>k)-curw)
	#define YINC ((USED_SIDE>>k)-curh)
#else
	#define XINC 1
	#define YINC 1
#endif

#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif


int fillMipMap( int texture,NMGLint format,NMGLint width,NMGLint height);

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
////////////EXTERN/////////////////////////////////////////////////////////////////////////////////////
extern NMGLubyte mipmap[MIPMAP_MEM_SIZE];
extern NMGLubyte mipmap[MIPMAP_MEM_SIZE];
extern NMGL_Context_NM0 *cntxt;
extern void* cntxtAddr_nm1;
//========================================================================================
extern NMGLint init_TexImage2D_input(TexImage2D_data* data,NMGLint width,internalformatdata internalformat,NMGLint lvl=0);

extern void fillPixels (void **pixels,NMGLint ubytesPerTexel,NMGLint width,int sfiller=-1);

extern int copyPixels(const void* pfrom,NMGLint format,NMGLint width,NMGLint height,void** pto,NMGL_Context_NM0 *cntxt);//mem allocation

extern void initLvls(NMGLuint name,NMGL_Context_NM0 *cntxt);

extern int cmpPixelsUbytes(void* from, void *to, NMGLubyte n_pixels);
/////////////////////////////////////////////////////////////////////////////////////////////////




#define MIPMAP cntxt->texState.texObjects[texture].texImages2D
int fillMipMap( int texture,NMGLint format,NMGLint width,NMGLint height)
{
    int i=0,j=0;
    int c=0;
	static int filler=0x0;    
    int size=getTexelSizeUbytes(format);   
     
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
             DEBUG_PRINT1(("fillMipMap:level=%d format=0x%x Texelsize=%d\n",i, format,size));        
            for(j=0;j<MIPMAP[i].width*MIPMAP[i].height*size;j++)
            {
               // *((unsigned char*)MIPMAP[i].pixels+j)=(unsigned char)filler;
                *((NMGLubyte*)MIPMAP[i].pixels+j)=(NMGLubyte)filler%0xff;
				filler++;
            }
        }
    return 0;
}
//========================================================================================
//-----------------------------------------------------------------------------------------------------
int cmpRefreshPixels(void* active,void *initial,void *subimageWpaddings,int pixsizebytes,NMGLint width,NMGLint height,int xstart,int ystart,int wsub,int hsub)
{
    int i=0;
    int j=0;
    int line=width*pixsizebytes;
    int subline=wsub*pixsizebytes;
    //int startsubx=
   // int startsuby=

   int paddings=(subline % cntxt->texState.unpackAlignment == 0) ? 0 : (cntxt->texState.unpackAlignment - subline % cntxt->texState.unpackAlignment);
	DEBUG_PRINT1(("\n\ncmpRpixels:width=%d height=%d\n",width,height));
	DEBUG_PRINT1(("cmpRpixels:wsub=%d hsub=%d\n",wsub,hsub));
	DEBUG_PRINT1(("cmpRpixels:xstart=%d ystart=%d\n",xstart,ystart));
	DEBUG_PRINT1(("cmpRpixels:line=%d subline=%d\n",line,subline));
	DEBUG_PRINT1(("cmpRpixels:paddings=%d \n\n",paddings));


    for(i=0;i<height;i++)
    {
        for(j=0;j<line;j++)
        {
            if((i>=ystart)&&(i<ystart+hsub)&&(j>=xstart*pixsizebytes)&&(j<xstart*pixsizebytes+subline))
            {   //inside subimageWpaddings

               

               if( *((NMGLubyte *)active+i*line+j) == *((NMGLubyte *)subimageWpaddings+(i-ystart)*(subline+paddings)+j-xstart*pixsizebytes))
               {
                     *((NMGLubyte *)initial+i*line+j) = *((NMGLubyte *)subimageWpaddings+(i-ystart)*(subline+paddings)+j-xstart*pixsizebytes);
               }
               else
               {
                   DEBUG_PRINT(("Error! Pixels array was corrupted inside by TexsubimageWpaddings2D\n"));
				   DEBUG_PRINT1(("inside\n"));
               DEBUG_PRINT1(("acrive[%d,%d]_a=%x\n",i,j,*((NMGLubyte *)active+i*line+j) ));
               DEBUG_PRINT1(("initial[%d,%d]_t=%x\n",i,j,*((NMGLubyte *)initial+i*line+j) ));
               DEBUG_PRINT1(("subimage[%d,%d]_s=%x\n\n",i-ystart,j-xstart,*((NMGLubyte *)subimageWpaddings+(i-ystart)*(subline+paddings)+j-xstart*pixsizebytes) ));
				   return 0;
               }
            }
            else
            { //outside

            

               if( *((NMGLubyte *)active+i*line+j) != *((NMGLubyte *)initial+i*line+j))
               {
                   DEBUG_PRINT(("Error! Pixels array was corrupted outside by TexSubImage2D\n"));
				   DEBUG_PRINT1(("outside\n"));
                DEBUG_PRINT1(("active[%d,%d]_a=%x\n",i,j,*((NMGLubyte *)active+i*line+j) ));
                DEBUG_PRINT1(("initial[%d,%d]_t=%x\n",i,j,*((NMGLubyte *)initial+i*line+j) ));
                DEBUG_PRINT1(("subimage[%d,%d]_s=%x\n\n",i-ystart,j-xstart,*((NMGLubyte *)subimageWpaddings+(i-ystart)*(subline+paddings)+j-xstart) ));
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
	
	void *pixels=&mipmap[MIPMAP_MEM_SIZE-3*MIPMAP_OBJ_SIZE];
	NMGLubyte *testarray=&mipmap[MIPMAP_MEM_SIZE-MIPMAP_OBJ_SIZE];
	int sss=0;
	int i=0;
	int j=0;
	int k=0;
	int alignments[4]={4,1,2,8};
	int formats[5]={NMGL_RGBA,NMGL_RGB,NMGL_ALPHA,NMGL_LUMINANCE,NMGL_LUMINANCE_ALPHA};
	int curw=USED_SIDE>>1;
	int curh=USED_SIDE>>1;
	int curAlignment=0;
	int curFormat=0;
	//NMGLint curformat_size=getTexelSizeUbytes(NMGL_RGBA);
	int curB=0;	
	int sfiller=0;
	//int picSize=curw*curh*UBYTES_PER_TEXEL;
	TexImage2D texImages2D[NMGL_MAX_MIPMAP_LVL+1];
	int _accum=0;//for error accumulation
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;
	cntxt->texState.unpackAlignment=alignments[curAlignment];
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];
	cntxt->texState.texObjects[0].imageIsSet=1;
			
	for(curAlignment=0;curAlignment<4;curAlignment++)
	{
		DEBUG_PRINT1(("Use alignment=%d\n",alignments[curAlignment]));
		for(curFormat=0;curFormat<5;curFormat++)
		{
		//curFormat=2;
			DEBUG_PRINT1(("Use format=0x%x\n",formats[curFormat]));
			fillPixels (&pixels,getTexelSizeUbytes(formats[curFormat]),curw,sfiller++);

			DEBUG_PRINT1(("init pixels[0,0]=%x\n",*((NMGLubyte*)pixels)));

			fillMipMap(0,formats[curFormat],USED_SIDE,USED_SIDE);

			

			for(k=0;k<=USED_MIPMAP_LVL;k++) 
			{

				for(i=0;i<=(USED_SIDE>>k)-curw;i=i+XINC)
				{
					for(j=0;j<=(USED_SIDE>>k)-curh;j=j+YINC)
					{
#ifdef DEBUG
						if((k==0)&&(i<2)&&(j<2))
						{
						DEBUG_PRINT(("_______________________________________\n"));
						DEBUG_PRINT(("Level=%d	startx=%d  starty=%d\n",k,j,i));
						DEBUG_PRINT(("SubWidth=%d	SubHeight=%d \n",curw,curh));
						}
#endif
						cntxt->texState.unpackAlignment=1;
						copyPixels(ActiveTexObjectP->texImages2D[k].pixels,ActiveTexObjectP->texImages2D[k].internalformat,		ActiveTexObjectP->texImages2D[k].width,ActiveTexObjectP->texImages2D[k].height,(void**)&testarray,cntxt);
						if(cmpPixelsUbytes(ActiveTexObjectP->texImages2D[k].pixels, testarray, ActiveTexObjectP->texImages2D[k].		width*ActiveTexObjectP->texImages2D[k].height*getTexelSizeUbytes(formats[curFormat])) != 1)
						{
							DEBUG_PRINT1(("Active to initial arrays copy operation failed!\n"));
							DEBUG_PRINT1(("k=%d i=%d j=%d\n",k,i,j));
							return -1;
						}
						else {
							DEBUG_PRINT1(("Active to initial copy operation ok!\n"));
						}
						cntxt->texState.unpackAlignment=alignments[curAlignment];
						nmglTexSubImage2D (NMGL_TEXTURE_2D, k, i, j, curw, curh, formats[curFormat], NMGL_UNSIGNED_BYTE, pixels);

						DEBUG_PRINT1(("k=%d i=%d j=%d\n",k,i,j));						
						TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
						TEST_ASSERT(cmpRefreshPixels(ActiveTexObjectP->texImages2D[k].pixels,testarray,pixels,getTexelSizeUbytes		(ActiveTexObjectP->texImages2D[k].internalformat),ActiveTexObjectP->texImages2D[k].width,	ActiveTexObjectP->texImages2D	[k].height,i,j,curw,curh) == 1 );
						fillPixels (&pixels,getTexelSizeUbytes(ActiveTexObjectP->texImages2D[k].internalformat),curw,sfiller++);
						if((USED_SIDE>>k)-curh == 0) j=1;				
					}
					if((USED_SIDE>>k)-curw == 0) i=1;
				}
				if(curw > 1) curw>>=1;
				if(curh > 1) curh>>=1;
			//	picSize=curw*curh*UBYTES_PER_TEXEL;
				sfiller=0;
				fillPixels (&pixels,getTexelSizeUbytes(ActiveTexObjectP->texImages2D[k].internalformat),curw,sfiller++);
			}
		
			//TEST_ASSERT(_accum==0);	
			curw=USED_SIDE;
			curh=USED_SIDE;
			copyPixels(ActiveTexObjectP->texImages2D[0].pixels,ActiveTexObjectP->texImages2D[0].internalformat,ActiveTexObjectP->texImages2D[0].width,		ActiveTexObjectP->texImages2D[0].height,(void**)&testarray,cntxt);


			nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 0, 0, curw, curh, formats[curFormat], NMGL_UNSIGNED_BYTE, pixels);	

			TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
			TEST_ASSERT(cmpRefreshPixels(ActiveTexObjectP->texImages2D[0].pixels,testarray,pixels,getTexelSizeUbytes(ActiveTexObjectP->texImages2D[0].		internalformat),ActiveTexObjectP->texImages2D[0].width,ActiveTexObjectP->texImages2D[0].height,0,0,curw,curh) == 1 );
		}	
	}
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
		fillPixels(&pixels,getTexelSizeUbytes(NMGL_RGBA),USED_SIDE>>2);
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
