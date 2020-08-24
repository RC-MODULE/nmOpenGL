
#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "tests.h"
#include "nmgl_tex_test_common.h"

//#include <iostream>
//#include <assert.h>
//#include <malloc.h>
//#include <cstdlib>

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
void nmglTexImage2D (NMGLenum target, NMGLint level, NMGLint
internalformat, NMGLsizei width, NMGLsizei height, NMGLint border,
NMGLenum format, NMGLenum type, const void *pixels);
//////////////////////////////////////////////////////////////////

	The first call to TexImage2D for a texture object will set the
size of the texture object,and, once established, it cannot be
changed.(TODO)
	Once the size of a texture object has been set,subsequent calls
to TexImage2D for that texture object will have the following
restrictions:
	-	internalFormat must be identical to the internalFormat used
		for the first call to TexImage2D for the texture object.
	-	width must be max(2^(k-n),1), where n is the mipmap level
		for the call and k is the log2 of the width of the level
		zero mipmap as set by the first call to TexImage2D for the
		texture object.
	-	height must be max(2k-n,1), where n is the mipmap level for
		the call and k is the log2 of the height of the level zero
		mipmap as set by the first call to TexImage2D for the
		texture object.
		width/height min=64 max=NMGL_MAX_TEX_SIZE
	If mipmapping is disabled, subsequent calls to TexImage2D (for
level zero) must have the same internalFormat, width, and height
as the first call to TexImage2D for the texture object. If these
restrictions are violated, an INVALID_OPERATION is generated.
	Texture borders are not supported. The border parameter must be
zero, and an INVALID_VALUE error results if it is non-zero.
	CopyTexture, CopyTexSubImage and compressed textures are
	not supported.
	Wrap modes supported:	REPEAT,CLAMP_TO_EDGE 
	Texture priorities, LOD clamps, and explicit base and maximum
level specification are not supported.
	The remaining OpenGL 1.3 texture parameters are supported
including all filtering modes.
	Texture objects are supported, but proxy textures are not
supported. Multitexture is supported, but the COMBINE texture
environment mode is not.

TexParameteri(enum target, enum pname, int param)

pname:TEXTURE_MIN_FILTER,TEXTURE_MAG_FILTER
pname:TEXTURE_WRAP_S,TEXTURE_WRAP_T

*/
#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif
NMGL_Context_NM0 *cntxt;
extern void* cntxtAddr_nm1;
NMGL_Context_NM1 *nm1cntxt;
SECTION(".data_tex_tests") NMGLubyte texels[USED_SIDE*USED_SIDE*UBYTES_PER_TEXEL];
extern int getTexelSizeUbytes(NMGLint format);
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
//---------------------------------------------------------------------------
TexImage2D_data input;//main input data;	
	//-----------curs-------------------------
	//NMGLint cur_format;
	NMGLint cur_width;
	//----------------------------------------
	int status=0;
	int i=0;//index

//==============================================================================================
//Тестовые сценарии
int nmglTexImage2D_wrongTarget_isError();
int nmglTexImage2D_wrongLevel_isError();
int nmglTexImage2D_wrongLevelAndSize_isError();
int nmglTexImage2D_wrongInternalformatFormat_isError();
int nmglTexImage2D_wrongWidth_isError();
int nmglTexImage2D_wrongHeight_isError();
int nmglTexImage2D_wrongBorder_isError();
int nmglTexImage2D_wrongType_isError();
int nmglTexImage2D_setImage_contextSetCorrect();
//==============================================================================================

int cmpPixelsUbytes(void* from, void *to, NMGLint n_pixels);
//==============================================================================================

internalformatdata internalformats[5];


/////////////////////////////////////////////////////////////////////////////////////////////////
void init_internalformats()
{
	internalformats[0].type=NMGL_RGBA;	
	internalformats[0].size=getTexelSizeUbytes(NMGL_RGBA);	
	internalformats[1].type=NMGL_RGB;	
	internalformats[1].size=getTexelSizeUbytes(NMGL_RGB);
	internalformats[2].type=NMGL_LUMINANCE;	
	internalformats[2].size=getTexelSizeUbytes(NMGL_LUMINANCE);
	internalformats[3].type=NMGL_ALPHA;	
	internalformats[3].size=getTexelSizeUbytes(NMGL_ALPHA);
	internalformats[4].type=NMGL_LUMINANCE_ALPHA;	
	internalformats[4].size=getTexelSizeUbytes(NMGL_LUMINANCE_ALPHA);
}
//----------------------------------------------------------------------------------------
int cmpPixelsUbytes(void* from, void *to, NMGLint n_pixels)
{
    int i=0;
  /*
   printf("n_pixels=%d\n",n_pixels);
   printf("from=0x%x\n",from);
   printf("to=0x%x\n",to);
   printf("firstBfrom=0x%x\n",*(NMGLubyte*)from);
   printf("firstBto=0x%x\n",*(NMGLubyte*)to);
   */
  for(i=0;i<n_pixels;i++)
  {
	  if(*((NMGLubyte*)from+i) != *((NMGLubyte*)to+i) )
	  {
		  printf("i=%d mismatch. from/to=0x%x/0x%x ",i,*((NMGLubyte*)from+i),	*((NMGLubyte*)to+i));
		  return 0;
	  }
  }
	return 1;
	
}
//----------------------------------------------------------------------------------------
void fillPixels (void **pixels,NMGLint size)
{
	static int t=0xAA;
	int i=0;
	NMGLubyte *p=(NMGLubyte*)*pixels;
	for(i=0;i<size;i++,t++)
	{
		*((NMGLubyte*)(p + i))=t;
	}
}
//----------------------------------------------------------------------------------------

NMGLint init_TexImage2D_input(TexImage2D_data* data,NMGLint width,internalformatdata internalformat,NMGLint lvl=0)
{
	
	NMGLint picture_size_;
	
	picture_size_=width*width*getTexelSizeUbytes(internalformat.type);	
	data->target=NMGL_TEXTURE_2D;//NMGL_TEXTURE_2D only
	data->level=lvl;
	data->type=NMGL_UNSIGNED_BYTE;//UNSIGNED_BYTE
	data->format=internalformat.type;//must match internalformat

	//cur_format=internalformat.type;	

	data->internalformat.type=internalformat.type;
	data->internalformat.size=internalformat.size;
	data->width=width;//min=64 max=NMGL_MAX_TEX_SIZE
	data->height=width;//min=64 max=NMGL_MAX_TEX_SIZE
	
	data->pixels=texels;
	
		fillPixels(&data->pixels,picture_size_);		
	
	return 0;
}
//----------------------------------------------------------------------------------------
/*
	Предварительная инициализация контекста
	Активным выбирается текстурный модуль 0,
	в котором делается ссылка на нулевой текстурный объект с именем 1.
*/
void _nmglTexImage2D_init_context()
{
	//int i=0;
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;	
	cntxt->texState.activeTexUnitIndex=0;

	//cntxt->texState.clientActiveTexUnit=NMGL_TEXTURE0;
	//cntxt->texState.clientActiveTexUnitIndex=0;

	cntxt->texState.texUnits[0].enabled=NMGL_TRUE;

	cntxt->texState.lastTexName=1;

	cntxt->texState.texObjects[0].name=0;
	cntxt->texState.texObjects[0].target=NMGL_TEXTURE_2D;
	cntxt->texState.texObjects[0].imageIsSet=0;
	/*
	for(i=0;i<=NMGL_MAX_MIPMAP_LEVEL;i++)
	{
		cntxt->texState.texObjects[0].texImages2D[i].internalformat=NMGL_RGBA;		
	}
	*/
	//cntxt->texState.texObjects[0].texImages2D[0].internalformat=NMGL_RGBA;		

	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];
}

//-----------------------------------------------------------------------------------------
/*
	Сравнивает входной и активный текстурные объекты 
*/
int _nmglTexImage2D_check(NMGLint level)
{
//#if 0	
	if ((level>=0)&&(level<=NMGL_MAX_MIPMAP_LVL))
	{
		if (ActiveTexObjectP->target!=input.target)
		{
			printf("Error!Targets dont match!\n");
			return 1;
		}
		
		if (ActiveTexObjectP->texImages2D[level].internalformat!=input.internalformat.type)
		{
			printf("Error!Internal formats dont match!\n");
			return 1;
		}
		
		if (ActiveTexObjectP->texImages2D[level].width!=input.width)
		{
			printf("Error!Width dont match!\n");
			return 1;
		}
		if (ActiveTexObjectP->texImages2D[level].height!=input.height)
		{
			printf("Error!Heights formats dont match!\n");
			return 1;
		}
		DEBUG_PRINT(("w=%d h=%d s=%d\n",input.width,input.height,getTexelSizeUbytes(input.internalformat.type)));
		if(!cmpPixelsUbytes(ActiveTexObjectP->texImages2D[level].pixels,input.pixels,input.width*input.height*getTexelSizeUbytes(input.internalformat.type)))
		{
			printf("Error!Pixels arrays dont match!\n");
			return 1;
		}
		
		
	}
	else
	{
		printf("Error! Wrong texture level!\n");
		return 1;
	}
//#endif
	return 0;
}
//----------------------------------------------------------------------------------------
/*
	Функция предотвращает появление ошибок, связанных с повторным вызовом nmglTexImage2D.

*/
void _nmglTexImage2D_prevent_internal_errors()
{
	cntxt->error=NMGL_NO_ERROR;
	ActiveTexObjectP->imageIsSet=0;
	//printf("firstByte was=0x%x  mipmap size is %d\n",cntxt->texState.firstFreeTexByte,MIPMAP_TESTOBJ_SIZE);
	//cntxt->texState.firstFreeTexByte = NULL;
	/*
	if ((int)cntxt->texState.firstFreeTexByte == MIPMAP_OBJ_SIZE)
	{
		cntxt->texState.firstFreeTexByte = NULL;
		printf("was MIPMAP_OBJ_SIZE\n");
	}
	else 
	{
		cntxt->texState.firstFreeTexByte=(void*)((int)cntxt->texState.firstFreeTexByte - MIPMAP_OBJ_SIZE);
		printf("was BIGGER\n");
	}
	*/
}
//----------------------------------------------------------------------------------------
/*
void print_input()
{
	DEBUG_PRINT(("=========Input Contents====================\n"));
	DEBUG_PRINT(("Target=%d\n",input.target));	
	DEBUG_PRINT(("InternalFormat=%d,size=%d\n",input.internalformat.type,input.internalformat.size));
	DEBUG_PRINT(("Width=%d\n",input.width));
	DEBUG_PRINT(("Height=%d\n",input.height));
	DEBUG_PRINT(("Type=%d\n",input.type));
	DEBUG_PRINT(("===========================================\n"));
}
*/
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglTexImage2D_test()
{
	cntxt = NMGL_Context_NM0::getContext();
	nm1cntxt = (NMGL_Context_NM1*)cntxtAddr_nm1;
	init_internalformats();
	cur_width=USED_SIDE;	
//	cur_format=internalformats[0].type;

	_nmglTexImage2D_init_context();
	
	
	//print_input();
	/*
	_nmglTexImage2D_init_context();
	//_nmglTexImage2D_update_active_object();
*/
DEBUG_PRINT(("--------------------------------------------------------------------------------\n"));

	printf ("\nStart nmglTexImage2D tests\n\n");
//#if 0
	RUN_TEST(nmglTexImage2D_wrongTarget_isError);
	RUN_TEST(nmglTexImage2D_wrongLevel_isError);
	RUN_TEST(nmglTexImage2D_wrongLevelAndSize_isError);
	RUN_TEST(nmglTexImage2D_wrongInternalformatFormat_isError);
	RUN_TEST(nmglTexImage2D_wrongWidth_isError);
	RUN_TEST(nmglTexImage2D_wrongHeight_isError);
	RUN_TEST(nmglTexImage2D_wrongBorder_isError);
	RUN_TEST(nmglTexImage2D_wrongType_isError);
//#endif	
	RUN_TEST(nmglTexImage2D_setImage_contextSetCorrect);
//
_nmglTexImage2D_prevent_internal_errors();
	
	printf ("\nEnd nmglTexImage2D tests\n");
	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================


// проверка корректности обработки значений аргумента target
//CHANGE_REPORT
int nmglTexImage2D_wrongTarget_isError()
{
	_nmglTexImage2D_prevent_internal_errors();
	status=init_TexImage2D_input(&input,cur_width,internalformats[0]);

	nmglTexImage2D(input.target+1,input.level,input.internalformat.type,input.width,input.height,0,input.internalformat.type,input.type,input.pixels);

	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
	
	_nmglTexImage2D_prevent_internal_errors();
//printf("Before:firstFreeTexByte=0x%x\n",cntxt->texState.firstFreeTexByte);
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.internalformat.type,input.type,input.pixels);
//printf("After:firstFreeTexByte=0x%x\n",cntxt->texState.firstFreeTexByte);

	
	
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

_nmglTexImage2D_prevent_internal_errors();

	nmglTexImage2D(input.target+1,input.level,input.internalformat.type,input.width,input.height,0,input.internalformat.type,input.type,input.pixels);

	TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
	
	_nmglTexImage2D_prevent_internal_errors();
	return 0;
}
//------------------------------------------------------------------------------
// проверка корректности  обработки значений аргумента level
int nmglTexImage2D_wrongLevel_isError()
{
	_nmglTexImage2D_prevent_internal_errors();
	nmglTexImage2D(input.target,-1,input.internalformat.type,input.width,input.height,0,input.internalformat.type,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);

	_nmglTexImage2D_prevent_internal_errors();
	nmglTexImage2D(input.target,1,input.internalformat.type,input.width,input.height,0,input.internalformat.type,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_OPERATION);

	_nmglTexImage2D_prevent_internal_errors();
	nmglTexImage2D(input.target,NMGL_MAX_MIPMAP_LVL+1,input.internalformat.type,input.width,input.height,0,input.internalformat.type,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);

	_nmglTexImage2D_prevent_internal_errors();
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.internalformat.type,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);

	nmglTexImage2D(input.target,NMGL_MAX_MIPMAP_LVL+1,input.internalformat.type,input.width,input.height,0,input.internalformat.type,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT

return 0;
}
//------------------------------------------------------------------------------
// проверка корректности обработки значений аргументов width, height и level при проверке на соответствие размеров изображения текстуры значению аргумента level.
int nmglTexImage2D_wrongLevelAndSize_isError()
{
	_nmglTexImage2D_prevent_internal_errors();
	//cntxt->texState.activeTexUnitIndex=0;
	
	//ActiveTexObjectP=&cntxt->texState.texObjects[0];
	ActiveTexObjectP->texMinFilter=NMGL_NEAREST_MIPMAP_NEAREST;
	status=init_TexImage2D_input(&input,cur_width,internalformats[0]);//CHANGE_REPORT
		nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.internalformat.type,input.type,input.pixels);
	status=init_TexImage2D_input(&input,cur_width>>1,internalformats[0],1);
	
		nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.internalformat.type,input.type,input.pixels);

	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
	input.level=2;
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.internalformat.type,input.type,input.pixels);
	//printf("cntxt->error=%x\n",cntxt->error);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_OPERATION);
	
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT
return 0;
}
//------------------------------------------------------------------------------
// проверка корректности обработки значений аргументов internalformat и format.
int nmglTexImage2D_wrongInternalformatFormat_isError()
{
	//_nmglTexImage2D_prevent_internal_errors();
	int i=0,j=0;
	
	for(i=0;i<=4;i++)
	{
		status=init_TexImage2D_input(&input,cur_width,internalformats[i]);
		
		nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.format,input.type,input.pixels);
		TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
		_nmglTexImage2D_prevent_internal_errors();	

	}
	
status=init_TexImage2D_input(&input,cur_width,internalformats[0]);
	input.internalformat.type=(internalformats[0].type<<1)+1;
	input.format=input.internalformat.type;

	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	

	input.format=input.internalformat.type+1;

	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
	_nmglTexImage2D_prevent_internal_errors();

	nmglTexImage2D(input.target,input.level,NMGL_RGBA,input.width,input.height,0,NMGL_RGB,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
	_nmglTexImage2D_prevent_internal_errors();

	nmglTexImage2D(input.target,input.level,NMGL_RGBA,input.width,input.height,0,NMGL_RGBA,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);

	nmglTexImage2D(input.target,input.level,NMGL_RGB,input.width,input.height,0,NMGL_RGB,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_OPERATION);

	_nmglTexImage2D_prevent_internal_errors();
		return 0;
}
//------------------------------------------------------------------------------
//проверка корректности обработки значений аргумента width.
int nmglTexImage2D_wrongWidth_isError()
{
	_nmglTexImage2D_prevent_internal_errors();
	status=init_TexImage2D_input(&input,cur_width,internalformats[0]);
		
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);

nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width<<1,input.height,0,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_OPERATION);

	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	

	nmglTexImage2D(input.target,input.level,input.internalformat.type,-1,input.height,0,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
	_nmglTexImage2D_prevent_internal_errors();
	return 0;	
}
//------------------------------------------------------------------------------
//проверка корректности обработки значений аргумента height.
int nmglTexImage2D_wrongHeight_isError()
{
	//_nmglTexImage2D_prevent_internal_errors();
	//status=init_TexImage2D_input(&input,cur_width,internalformats[0]);
	
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);

	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height<<1,0,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_OPERATION);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	

	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,-1,0,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	
	return 0;
}
//------------------------------------------------------------------------------
//проверка корректности обработки значений аргумента border.
int nmglTexImage2D_wrongBorder_isError()
{
	//_nmglTexImage2D_prevent_internal_errors();
	//status=init_TexImage2D_input(&input,cur_width,internalformats[0]);
		
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	

	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,5,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	
	return 0;
}
//------------------------------------------------------------------------------
//проверка корректности обработки значений аргумента type.
int nmglTexImage2D_wrongType_isError()
{
	//_nmglTexImage2D_prevent_internal_errors();
	//status=init_TexImage2D_input(&input,cur_width,internalformats[0]);
		
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
	_nmglTexImage2D_prevent_internal_errors();	

	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.format,NMGL_FLOAT,input.pixels);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT
	return 0;
}

//проверка корректности установки соответствующих значений полей контекста.
int nmglTexImage2D_setImage_contextSetCorrect()
{
	int i=0;

	_nmglTexImage2D_prevent_internal_errors();
ActiveTexObjectP=&cntxt->texState.texObjects[0];
	//cur_width=NMGL_MAX_TEX_SIDE;
	cur_width=USED_SIDE;
		status=init_TexImage2D_input(&input,cur_width,internalformats[0],i);
		//printf("cur_width=%d\n",cur_width);		
		//printf("format=%d should be %d\n",internalformats[0].type,NMGL_RGBA);		
		nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,0,input.format,input.type,input.pixels);
		status=_nmglTexImage2D_check(0);
		
	TEST_ASSERT(status==0);	
	
	
	
	_nmglTexImage2D_prevent_internal_errors();
	return 0;
}
