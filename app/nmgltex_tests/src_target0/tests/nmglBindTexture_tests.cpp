
#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "tests.h"
#include "nmgl_tex_test_common.h"


//#include <iostream>
extern void* cntxtAddr_nm1;
#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif
extern NMGL_Context_NM1 *cntxt_nm1;
extern unsigned int nmpu1IsAccessible;
extern NMGLubyte* mipmap;
extern int getTexelSizeUbytes(NMGLint format);
int isPowerOf2(NMGLint x);



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
void nmglBindTexture (NMGLenum target, NMGLuint texture);
//////////////////////////////////////////////////////////////////
*/

/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int _accum=0;
int iterb=0;

//int cmpPixels32(void* from, void *to, NMGLint n_pixels);
int fillMipMap( int texture,NMGLint format,NMGLint width,NMGLint height,int filler);
int cmpTexObj(TexObject* first,TexObject* second);
int getMaxPower(NMGLuint x, NMGLuint y);


int nmglBindTexture_wrongArgs_isError();
int nmglBindTexture_bindTexture_contextStateCorrect();
/////////////////////////////////////////////////////////////////////////////////////////////////
/*
int objInitEq(NMGL_Context_NM0 *cntxt,NMGLuint texture)
{
	int i=0;
	
	int _accum=0;
	TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].name == texture);
	
	TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].target == NMGL_TEXTURE_2D );

	TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texWrapS == NMGL_REPEAT );

	TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texWrapT == NMGL_REPEAT );
 
	TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texMinFilter ==NMGL_NEAREST_MIPMAP_LINEAR); 
	TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texMagFilter == NMGL_LINEAR);
	for(i=0;i<=USED_MIPMAP_LVL;i++)
	{	
		TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texImages2D[i].internalformat==NMGL_RGBA );
		TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texImages2D[i].width==0 );
		TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texImages2D[i].height==0 );
		TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texImages2D[i].pixels==NULL );
	} 
	TEST_ASSERT(_accum == 0);
	return 0;
}
*/
//-----------------------------------------------------------------------------------------------------
int getMaxPower(NMGLuint x, NMGLuint y)
{
    int cx=0,cy=0;
    if(!isPowerOf2(x)) return 0;
    if(!isPowerOf2(y)) return 0;
    while(x>1)
    {
       cx++;x>>=1;  
    }
    while(y>1)
    {
       cy++;y>>=1; 
    }
    if(cx>cy) return cx; else return cy;
}

//-----------------------------------------------------------------------------------------------------

int cmpTexObj(TexObject* first,TexObject* second)
{
    int i=0,j=0;
    int size=1;
    if (first->name != second->name) {DEBUG_PRINT(("Tex objects dont match!Names mismatch!\n"));return 0;}
    if (first->target != second->target){DEBUG_PRINT(("Tex objects dont match!Target mismatch!\n")); return 0;}
    if (first->texWrapS != second->texWrapS) {DEBUG_PRINT(("Tex objects dont match!texWrapS mismatch!\n"));return 0;}
    if (first->texWrapT != second->texWrapT) {DEBUG_PRINT(("Tex objects dont match!texWrapT mismatch!\n"));return 0;}
    if (first->texMinFilter != second->texMinFilter){DEBUG_PRINT(("Tex objects dont match!texMinFilter mismatch!\n")); return 0;}
    if (first->texMagFilter != second->texMagFilter) {DEBUG_PRINT(("Tex objects dont match!texMagFilter mismatch!\n"));return 0;}
    if (first->imageIsSet != second->imageIsSet) {DEBUG_PRINT(("Tex objects dont match!imageIsSet mismatch!\n"));return 0;}
    
    size=getTexelSizeUbytes(first->texImages2D[i].internalformat);
    
    for(i=0;i<=getMaxPower(first->texImages2D[0].width,first->texImages2D[0].height);i++)
    {
        if(first->texImages2D[i].internalformat !=second->texImages2D[i].internalformat) {DEBUG_PRINT(("Tex objects dont match!Level:%d internalformat mismatch!\n",i));return 0;}
        if(first->texImages2D[i].width !=second->texImages2D[i].width) {DEBUG_PRINT(("Tex objects dont match!Level:%d width mismatch!\n",i));return 0;}
        if(first->texImages2D[i].height !=second->texImages2D[i].height) {DEBUG_PRINT(("Tex objects dont match!Level:%d height mismatch!\n",i));return 0;}
       
 /*delete, as not used*/
 //if(first->texImages2D[i].level !=second->texImages2D[i].level) {DEBUG_PRINT(("Tex objects dont match!Level:%d level mismatch!\n",i));return 0;}
/*delete, as not used*/ //if(first->texImages2D[i].border !=second->texImages2D[i].border) {DEBUG_PRINT(("Tex objects dont match!Level:%d border mismatch!\n",i));return 0;}
        
        for(j=0;j<first->texImages2D[i].width*first->texImages2D[i].height*size;j++)
        {
            if(*((unsigned char *)first->texImages2D[i].pixels+j) !=*((unsigned char *)first->texImages2D[i].pixels+j)) {DEBUG_PRINT(("Tex objects dont match!Level:%d Pixels mismatch!\n",i));return 0;}
        }
    }
    return 1;
}
//-----------------------------------------------------------------------------------------------------
#define MIPMAP cntxt->texState.texObjects[texture].texImages2D
int fillMipMap( int texture,NMGLint format,NMGLint width,NMGLint height,NMGLubyte filler)
{
    int i=0,j=0;
    int c=0;
    //int size=getFormatSize(format);
    int sizeBtes=0;
    
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	

    
    for(i=0;i<=USED_MIPMAP_LVL;i++)
        {
           if(MIPMAP[i].pixels == NULL) {printf("fillmipMap error! Pixels[%d] is NULL!\n",i); return 2;}
		    MIPMAP[i].internalformat=format;
           // MIPMAP[i].level=i;
            if(i>0)c=width>>i;else c=width;
            if(c>1)MIPMAP[i].width=c;else MIPMAP[i].width=1;
            if(i>0) c=height>>i;else c=height;
           if(c>1)MIPMAP[i].height=c;else MIPMAP[i].height=1;
		   sizeBtes=MIPMAP[i].width*MIPMAP[i].height*getTexelSizeUbytes(format);
           // MIPMAP[i].border=0;
           /* if(MIPMAP[i].pixels!=NULL)
            {
                free(MIPMAP[i].pixels);
            }
                DEBUG_PRINT(("Allocating texture %d mipmap level %d\n",texture,i));
                DEBUG_PRINT(("Width %d height %d\n",MIPMAP[i].width,MIPMAP[i].height));
                MIPMAP[i].pixels=malloc(size*MIPMAP[i].width*MIPMAP[i].height);
				
                if(MIPMAP[i].pixels==0)
                {
                    DEBUG_PRINT(("Error! Cant alocate mem for mipmap of texture %d",texture)); return 2;
                }
             */
            //for(j=0;j<size*MIPMAP[i].width*MIPMAP[i].height;j++)
            for(j=0;j<sizeBtes;j++)
            {
               // *((unsigned char*)MIPMAP[i].pixels+j)=(unsigned char)filler;
                *((NMGLubyte*)MIPMAP[i].pixels+j)=filler+j;
            }
        }
    return 0;
}
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Main
int run_nmglBindTexture_test()
{	
	printf ("\nStart nmglBindTexture tests\n\n");
		RUN_TEST(nmglBindTexture_wrongArgs_isError);
		RUN_TEST(nmglBindTexture_bindTexture_contextStateCorrect);
	printf ("\nEnd nmglBindTexture tests\n");
	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================

//void nmglBindTexture (NMGLsizei n, NMGLuint *textures);
// проверка корректности обработки значений аргумента target
//CHANGE_REPORT
//#define AO cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject
int nmglBindTexture_wrongArgs_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->error=NMGL_NO_ERROR;
	nmglBindTexture(NMGL_TEXTURE_2D,cntxt->texState.lastTexName+1);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);
	cntxt->error=NMGL_NO_ERROR;
	nmglBindTexture(NMGL_TEXTURE_2D+1,cntxt->texState.lastTexName);	
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
	cntxt->error=NMGL_NO_ERROR;
	return 0;
}
//------------------------------------------------------------------------------
// проверка создания текстурного объекта с заданным именем,
//  изменения привязанного текстурного объекта и проверка состояния
// привязанного ранее текстурного объекта.
int nmglBindTexture_bindTexture_contextStateCorrect()
{
	
	//#define MAX_TEX_SIZE_UBYTE (RGBA_TEXEL_SIZE_UBYTE*NMGL_MAX_TEX_SIDE*NMGL_MAX_TEX_SIDE)
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->error=NMGL_NO_ERROR;
	NMGLubyte *testTexBytes=&mipmap[MIPMAP_MEM_SIZE-MIPMAP_OBJ_SIZE];;
	NMGLubyte *testTexBytes2=&mipmap[MIPMAP_MEM_SIZE-2*MIPMAP_OBJ_SIZE];;
	int object_in_init_state=0;
	//NMGLint* curpixels[NMGL_MAX_MIPMAP_LVL];
	//NMGLint curwidth[NMGL_MAX_MIPMAP_LVL];
	//NMGLint curheight[NMGL_MAX_MIPMAP_LVL];
	int _accum=0;
	int i=0;
	int _st=0;
	int curside=USED_SIDE;
	cntxt->texState.lastTexName=3;
	//_init_tex_obj(1);
	//_init_tex_obj(2);
			//_init_tex_obj(3);
	cntxt->texState.texObjects[1].texWrapS=NMGL_CLAMP_TO_EDGE;
	cntxt->texState.texObjects[1].texWrapT=NMGL_CLAMP_TO_EDGE;
	cntxt->texState.texObjects[1].texMinFilter=NMGL_LINEAR_MIPMAP_NEAREST;
	cntxt->texState.texObjects[1].texMagFilter=NMGL_NEAREST;
	cntxt->texState.texObjects[1].texImages2D[0].pixels=testTexBytes;
	for(int i=1;i<=USED_MIPMAP_LVL;i++)
	{
		/*!!!!!*/cntxt->texState.texObjects[1].texImages2D[i].pixels=(NMGLubyte *)cntxt->texState.texObjects[1].texImages2D[i-1].pixels+(USED_SIDE>>(i-1))*(USED_SIDE>>(i-1))*UBYTES_PER_TEXEL;
	}
	
		

	_st=fillMipMap( 1,NMGL_RGBA,USED_SIDE,USED_SIDE,0xAF);
	/*
	if(_st == 0)
	{
		for(i=0;i<NMGL_MAX_MIPMAP_LVL;i++)
		{
			curpixels[i]=cntxt->texState.texObjects[1].texImages2D[i].pixels;
			curwidth[i]=cntxt->texState.texObjects[1].texImages2D[i].width;
			curheight[i]=cntxt->texState.texObjects[1].texImages2D[i].height;
		}
	}
	else
	{		
		return 2;
	}
	*/
TEST_ASSERT(_st == 0);


	cntxt->texState.texObjects[2].texWrapS=NMGL_CLAMP_TO_EDGE;
	cntxt->texState.texObjects[2].texWrapT=NMGL_REPEAT;
	cntxt->texState.texObjects[2].texMinFilter=NMGL_NEAREST_MIPMAP_NEAREST;
	cntxt->texState.texObjects[2].texMagFilter=NMGL_LINEAR;

	cntxt->texState.texObjects[2].texImages2D[0].pixels=testTexBytes2;

for(int i=1;i<=USED_MIPMAP_LVL;i++)
	{
	/*!!!*/	cntxt->texState.texObjects[2].texImages2D[i].pixels=(NMGLubyte *)cntxt->texState.texObjects[2].texImages2D[i-1].pixels+(USED_SIDE>>(i-1))*(USED_SIDE>>(i-1))*UBYTES_PER_TEXEL;
	}
_st=fillMipMap(2,NMGL_RGBA,USED_SIDE,USED_SIDE,0xAA);
TEST_ASSERT(_st == 0);

	_st = objInitEq<NMGL_Context_NM0>(cntxt,0);// check zero object is initilized
	TEST_ASSERT(_st == 0);

	if (nmpu1IsAccessible){
		_st = objInitEq<NMGL_Context_NM1>(cntxt_nm1,0);// check zero object is initilized
		TEST_ASSERT(_st == 0);
	}

TEST_ASSERT(cmpTexObj(ActiveTexObjectP,&cntxt->texState.texObjects[0]));//check zero obj is binded

	nmglBindTexture(NMGL_TEXTURE_2D,1);
	_accum=0;
	TEST_ASSERT_ACCUM(cmpTexObj(ActiveTexObjectP,&cntxt->texState.texObjects[1]));
	{
		TEST_ASSERT_ACCUM(ActiveTexObjectP->name == 1); 
		TEST_ASSERT_ACCUM(ActiveTexObjectP->target == NMGL_TEXTURE_2D ); 
		TEST_ASSERT_ACCUM(ActiveTexObjectP->texWrapS == NMGL_CLAMP_TO_EDGE ); 
		TEST_ASSERT_ACCUM(ActiveTexObjectP->texWrapT == NMGL_CLAMP_TO_EDGE ); 
		TEST_ASSERT_ACCUM(ActiveTexObjectP->texMinFilter == NMGL_LINEAR_MIPMAP_NEAREST); 
		TEST_ASSERT_ACCUM(ActiveTexObjectP->texMagFilter == NMGL_NEAREST);		
		TEST_ASSERT( _accum == 0 );
	}

	nmglBindTexture(NMGL_TEXTURE_2D,2);
	TEST_ASSERT(cmpTexObj(ActiveTexObjectP,&cntxt->texState.texObjects[2]));
	nmglBindTexture(NMGL_TEXTURE_2D,3);
	//TEST_ASSERT(cmpTexObj(AO,&cntxt->texState.texObjects[3]));

_st =objInitEq<NMGL_Context_NM0>(cntxt,3);
	TEST_ASSERT(_st == 0);

	if (nmpu1IsAccessible)
	{
		_st = objInitEq<NMGL_Context_NM1>(cntxt_nm1,3);
		TEST_ASSERT(_st == 0);
	}

cntxt->texState.lastTexName=0;
return 0;
}
