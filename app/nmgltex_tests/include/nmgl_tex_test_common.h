#ifndef NMGL_TEX_TEST_COMMON_H_
#define NMGL_TEX_TEST_COMMON_H_
#include "tests.h"
#include "nmgltex_common.h"
#define USED_MIPMAP_LVL 10  //mst be <= MAX_MIPMAP_LVL
#define USED_SIDE (0x1<<(USED_MIPMAP_LVL))
#define MIPMAP_TESTOBJ_SIZE ((((0x1<<(2*(USED_MIPMAP_LVL+1)))-1)/3)*UBYTES_PER_TEXEL) //mipmap mem size in 4byte words	

/////////////////DATA///STRUCTURES///////////////////////////////////////
typedef struct internalformatdata
{
NMGLint type;
NMGLint size;
} internalformatdata;
//---------------------------------------------------------------------------

typedef struct TexImage2D_data
{
	NMGLenum target;//NMGL_TEXTURE_2D only
	NMGLint level;
	internalformatdata internalformat;//RGBA,RGB,LUMINANCE,ALPHA,LUMINANCE_ALPHA
	NMGLsizei width;//min=64 max=NMGL_MAX_TEX_SIZE
	NMGLsizei height;//min=64 max=NMGL_MAX_TEX_SIZE
	//NMGLint border;//0 only
	NMGLenum format;//must match internalformat
	NMGLenum type;//UNSIGNED_BYTE
	void *pixels;
} TexImage2D_data;
#define WAIT_PERIOD 200
#define wait_for_nm1_if_available if(nmpu1IsAccessible == 1) halSleep(WAIT_PERIOD)

#ifdef DEBUG

#define lvl(x) cntxt->texState.texObjects[objname].texImages2D[x]
#define pxl(x) cntxt->texState.texObjects[objname].texImages2D[x].pixels
//prints pointers only
template<class T> void PrintMipMapP(int objname,T *context)
{

    T* cntxt = context;
    printf("Texture object %d\n",cntxt->texState.texObjects[objname].name);
    for(int i=0;i<=NMGL_MAX_MIPMAP_LVL;i++)
    {
        printf("Mipmap level %d\n",i);
        printf("internalformat=0x%x\n",lvl(i).internalformat);
       // printf("level=%d\n",lvl(i).level);
        printf("width=%d\n",lvl(i).width);
        printf("height=%d\n",lvl(i).height);
        if(lvl(i).pixels == NULL) printf("pixels=NULL\n");
        else printf("pixels=%x\n",lvl(i).pixels);
    }
}
extern int getTexelSizeUbytes(NMGLint format);

//prints pixels contents
template<class T> void PrintMipMap(int objname,T *context)
{
    T* cntxt = context;
    printf("Texture object %d\n", cntxt->texState.texObjects[objname].name);
    int bpt=getTexelSizeUbytes(lvl(0).internalformat);
    int wbpt=0;
    for(int i=0;i<=NMGL_MAX_MIPMAP_LVL;i++)
    {
         wbpt=lvl(i).width*bpt;
         if(pxl(i) == NULL) printf("Lvl %d is NULL\n",i);
         else
         {
            for(int j=0;j<lvl(i).width*lvl(i).height*bpt;j++)
            {
                printf("%x",*((NMGLubyte*)pxl(i)+j));
                if(j<wbpt-1) printf(" ");
                else printf("\n");
            }
         }         
    }
}
//=========================================================================================
#define PrintMipMapP(x,y,z) PrintMipMapP<x>(y,z)
#else
    
    #define PrintMipMapP(x) do {} while (0)  
    
#endif
template<class T> int objInitEq(T *cntxt,NMGLuint texture, void *zeroObjPointer=NULL)
{
	int i=0;
	
	int _accum=0;
	TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].name == texture);
	DEBUG_PRINT(("have texture=%d should be %d\n",cntxt->texState.texObjects[texture].name,texture));
	
    TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].target == NMGL_TEXTURE_2D );
	DEBUG_PRINT(("have target=%d should be %d\n",cntxt->texState.texObjects[texture].target,NMGL_TEXTURE_2D));

	TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texWrapS == NMGL_REPEAT );
   	DEBUG_PRINT(("have texWrapS=%d should be %d\n",cntxt->texState.texObjects[texture].texWrapS,NMGL_REPEAT));

	TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texWrapT == NMGL_REPEAT );
    DEBUG_PRINT(("have texWrapT=%d should be %d\n",cntxt->texState.texObjects[texture].texWrapT,NMGL_REPEAT));

	TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texMinFilter ==NMGL_NEAREST_MIPMAP_LINEAR);
    DEBUG_PRINT(("have texMinFilter=%d should be %d\n",cntxt->texState.texObjects[texture].texMinFilter,NMGL_NEAREST_MIPMAP_LINEAR));
 
	TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texMagFilter == NMGL_LINEAR);
    DEBUG_PRINT(("have texMagFilter=%d should be %d\n",cntxt->texState.texObjects[texture].texMagFilter,NMGL_LINEAR));

	for(i=0;i<=USED_MIPMAP_LVL;i++)
	{	
		DEBUG_PRINT(("lvl=%d\n",i));
        TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texImages2D[i].internalformat==NMGL_RGBA );
        DEBUG_PRINT(("have internalformat=%d should be %d\n",cntxt->texState.texObjects[texture].texImages2D[i].internalformat,NMGL_RGBA));

		TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texImages2D[i].width==0 );
        DEBUG_PRINT(("have width=%d should be %d\n",cntxt->texState.texObjects[texture].texImages2D[i].width,0));

		TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texImages2D[i].height==0 );
        DEBUG_PRINT(("have height=%d should be %d\n",cntxt->texState.texObjects[texture].texImages2D[i].height,0));

		TEST_ASSERT_ACCUM(cntxt->texState.texObjects[texture].texImages2D[i].pixels==zeroObjPointer);
        DEBUG_PRINT(("have pixels=%d should be %d\n",cntxt->texState.texObjects[texture].texImages2D[i].pixels,zeroObjPointer));

	} 
	TEST_ASSERT(_accum == 0);
	return 0;
}


#endif //NMGL_TEX_TEST_COMMON_H_

