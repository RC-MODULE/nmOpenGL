#if 0
#include "demo3d_nm0.h"


//#include "nmgl_data0.h"


//#define DEBUG
//#include "tex_common.h"
//#include "tex_functions.h"
//#include "tex_support_functions.h"

//#include "tex_test_support_functions.h"

#include "tests.h"
#include <iostream>
#include <assert.h>
//#include <malloc.h>
#include <cstdlib>
//#include <cstring>


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
	NMGLint level;//max=10 , as we currently use textures 1024x1024 max
	internalformatdata internalformat;//RGBA,RGB,LUMINANCE,ALPHA,LUMINANCE_ALPHA
	NMGLsizei width;//min=64 max=NMGL_MAX_TEX_SIZE
	NMGLsizei height;//min=64 max=NMGL_MAX_TEX_SIZE
	NMGLint border;//0 only
	NMGLenum format;//must match internalformat
	NMGLenum type;//UNSIGNED_BYTE
	void *pixels;
} TexImage2D_data;
//---------------------------------------------------------------------------
TexImage2D_data input;//main input data;	
	//-----------curs-------------------------
	NMGLint cur_format;
	NMGLint cur_width;
	//----------------------------------------
	int status=0;
	int i=0;//index
/////////////////////////////////////////////////////////////////////////////////////////////////
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
internalformatdata internalformats[5];
/////////////////////////////////////////////////////////////////////////////////////////////////

void init_internalformats()
{
	internalformats[0].type=NMGL_RGBA;	
	internalformats[0].size=4;	
	internalformats[1].type=NMGL_RGB;	
	internalformats[1].size=3;
	internalformats[2].type=NMGL_LUMINANCE;	
	internalformats[2].size=1;
	internalformats[3].type=NMGL_ALPHA;	
	internalformats[3].size=1;
	internalformats[4].type=NMGL_LUMINANCE_ALPHA;	
	internalformats[4].size=2;
}
//----------------------------------------------------------------------------------------
/*
void copyPixels(void* pixels,NMGLint size)
{
	//int i=0;	
	for(i=0;i<size;i++)
	{
		*((unsigned char*)pixels+i)=i;
	}
}
*/
//----------------------------------------------------------------------------------------

NMGLint init_TexImage2D_input(TexImage2D_data* data,NMGLint width,internalformatdata internalformat)
{
	NMGLint pixelsize=4;
	NMGLint picture_size_;
	static int ncalls=0;
	//picture_size_=internalformat.size*width*width;
	picture_size_=width*width;	
	data->target=NMGL_TEXTURE_2D;//NMGL_TEXTURE_2D only
	data->level=0;//max=10 , as we currently use textures 1024x1024 max
	data->border=0;//0 only
	data->type=NMGL_UNSIGNED_BYTE;//UNSIGNED_BYTE
	data->format=internalformat.type;//must match internalformat
	cur_format=internalformat.type;
	if(( data->internalformat.type == internalformat.type )&&( data->width == width ))
	{
		if(ncalls>0) fillPixels32(data->pixels,picture_size_);
	}
	else
	{
		data->internalformat.type=internalformat.type;
		data->internalformat.size=internalformat.size;
		data->width=width;//min=64 max=NMGL_MAX_TEX_SIZE
		data->height=width;//min=64 max=NMGL_MAX_TEX_SIZE
		if(ncalls>0)
		{
			free(data->pixels);
		}
		data->pixels=malloc(pixelsize*picture_size_);
	if (data->pixels!=0)
	{
		fillPixels32(data->pixels,picture_size_);
	}
	else
	{
		DEBUG_PRINT(("\nError! Cant allocate mem for input.pixels!\n"));
	}
		
	}
	ncalls++;
	return (NMGLint)data->pixels;
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
	cntxt.error=NMGL_NO_ERROR;
	cntxt.activeTexUnit=NMGL_TEXTURE0;	
	cntxt.activeTexUnitIndex=0;

	//cntxt.clientActiveTexUnit=NMGL_TEXTURE0;
	//cntxt.clientActiveTexUnitIndex=0;

	cntxt.texUnits[0].enabled=NMGL_TRUE;

	cntxt.lastTexName=1;

	cntxt.texObjects[0].name=0;
	cntxt.texObjects[0].target=NMGL_TEXTURE_2D;
	cntxt.texObjects[0].imageIsSet=0;
	/*
	for(i=0;i<=NMGL_MAX_MIPMAP_LEVEL;i++)
	{
		cntxt.texObjects[0].texImages2D[i].internalformat=NMGL_RGBA;		
	}
	*/
	cntxt.texObjects[0].texImages2D[0].internalformat=NMGL_RGBA;		

	cntxt.texUnits[0].boundTexObject=&cntxt.texObjects[0];
}
//----------------------------------------------------------------------------------------
/*
	Заполняет поля активного текстурного объекта значениями соответствующих полей входного текстурного объекта
*/
void _nmglTexImage2D_update_active_object(NMGLint level)
{
	void*  mstatus=0;
	//int i=0;
	//unsigned int n_bytes=0;
unsigned int n_pixels=0;

NMGLint pyxelsize=4;


	if ((level>=0)&&(level<=NMGL_MAX_MIPMAP_LEVEL))
	{
		cntxt.texUnits[0].boundTexObject->texImages2D[level].internalformat=input.internalformat.type;
		cntxt.texUnits[0].boundTexObject->texImages2D[level].level=input.level;
		cntxt.texUnits[0].boundTexObject->texImages2D[level].width=input.width;
		cntxt.texUnits[0].boundTexObject->texImages2D[level].height=input.height;
		cntxt.texUnits[0].boundTexObject->texImages2D[level].border=input.border;
		//n_bytes=input.internalformat.size*input.width*input.height;
		n_pixels=input.width*input.height;
		//mstatus=malloc(n_bytes);
		mstatus=malloc(n_pixels*pyxelsize);
		if(mstatus == 0)
		{
			DEBUG_PRINT(("ERROR!Can't allocate pixels array for active tex object!\n"));
			return;
		}
		free(ActiveTexObject->texImages2D[level].pixels);
		ActiveTexObject->texImages2D[level].pixels=mstatus;
		//for(i=0;i<n_bytes;i++)
		for(i=0;i<n_pixels;i++)
		{
			//*((unsigned char*)cntxt.texUnits[0].boundTexObject->texImages2D[level].pixels+i)=*((unsigned char *)input.pixels+i);
			*((NMGLint*)ActiveTexObject->texImages2D[level].pixels+i)=*((NMGLint *)input.pixels+i);
		}
		

	}
}
//-----------------------------------------------------------------------------------------
/*
	Сравнивает входной и активный текстурные объекты 
*/
int _nmglTexImage2D_check(NMGLint level)
{
//#if 0	
	if ((level>=0)&&(level<=NMGL_MAX_MIPMAP_LEVEL))
	{
		if (ActiveTexObject->target!=input.target)
		{
			printf("Error!Targets dont match!\n");
			return 1;
		}
		
		if (ActiveTexObject->texImages2D[level].internalformat!=input.internalformat.type)
		{
			printf("Error!Internal formats dont match!\n");
			return 1;
		}
		if (ActiveTexObject->texImages2D[level].level!=input.level)
		{
			printf("Error!Levels dont match!\n");
			return 1;
		}
		if (ActiveTexObject->texImages2D[level].width!=input.width)
		{
			printf("Error!Width dont match!\n");
			return 1;
		}
		if (ActiveTexObject->texImages2D[level].height!=input.height)
		{
			printf("Error!Heights formats dont match!\n");
			return 1;
		}
		if (ActiveTexObject->texImages2D[level].border!=input.border)
		{
			printf("Error!Heights formats dont match!\n");
			return 1;
		}
		if(!cmpPixels32(ActiveTexObject->texImages2D[level].pixels,input.pixels,input.width*input.height))
		{
			printf("Error!Pixels arrays dont match!\n");
			return 1;
		}
		
	}
	else
	{
		printf("Error! Wrong texture level!\n");
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
	cntxt.error=NMGL_NO_ERROR;
	ActiveTexObject->imageIsSet=0;
	for (i=0;i<=NMGL_MAX_MIPMAP_LEVEL;i++)
	{
		free(ActiveTexObject->texImages2D[i].pixels);
		ActiveTexObject->texImages2D[i].pixels=NULL;
	}
	

}
//----------------------------------------------------------------------------------------
void print_input()
{
	DEBUG_PRINT(("=========Input Contents====================\n"));
	DEBUG_PRINT(("Target=%d\n",input.target));
	DEBUG_PRINT(("level=%d\n",input.level));
	DEBUG_PRINT(("InternalFormat=%d,size=%d\n",input.internalformat.type,input.internalformat.size));
	DEBUG_PRINT(("Width=%d\n",input.width));
	DEBUG_PRINT(("Height=%d\n",input.height));
	DEBUG_PRINT(("Border=%d\n",input.border));
	DEBUG_PRINT(("Type=%d\n",input.type));
	/*
	if(input.pixels != NULL)
	{
		printf("pixels=\n",input.type);
		for(i=0;i<(input.internalformat.size*input.width*input.height);i++)
		{
		printf ("%d ",*((unsigned char*)input.pixels+i));
		if(i%10==0) printf("\n");
		}
	}
	else
	{
		printf ("pixels is NULL!\n");
	}
	*/
	DEBUG_PRINT(("===========================================\n"));
}

//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglTexImage2D_test()
{
	
	init_internalformats();
	cur_width=16;	
	cur_format=internalformats[0].type;

	status=init_TexImage2D_input(&input,cur_width,internalformats[0]);
	if(status==0) 
	{
		printf("ERROR!Can't allocate pixels array!\n");
		return 2;
	}
	
	print_input();
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
	RUN_TEST(nmglTexImage2D_wrongWidth_isError);
	RUN_TEST(nmglTexImage2D_wrongHeight_isError);
	RUN_TEST(nmglTexImage2D_wrongHeight_isError);
	RUN_TEST(nmglTexImage2D_wrongBorder_isError);
	RUN_TEST(nmglTexImage2D_wrongType_isError);
	RUN_TEST(nmglTexImage2D_setImage_contextSetCorrect);
//#endif
_nmglTexImage2D_prevent_internal_errors();
	free(input.pixels);
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
	//nmglTexImage2D(NMGLenum target, NMGLint level, NMGLint internalformat, NMGLsizei width, NMGLsizei height, NMGLint border, NMGLenum format, NMGLenum type, const void *pixels);
	cntxt.error=NMGL_NO_ERROR;
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.internalformat.type,input.type,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_NO_ERROR);
	//print_input();
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT
	//print_input();
	nmglTexImage2D(input.target+1,input.level,input.internalformat.type,input.width,input.height,input.border,input.internalformat.type,input.type,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_INVALID_ENUM);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT
	return 0;
}
//------------------------------------------------------------------------------
// проверка корректности  обработки значений аргумента level
int nmglTexImage2D_wrongLevel_isError()
{
	cntxt.error=NMGL_NO_ERROR;
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.internalformat.type,input.type,input.pixels);
	//_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT

	TEST_ASSERT(cntxt.error==NMGL_NO_ERROR);
	nmglTexImage2D(input.target,-1,input.internalformat.type,input.width,input.height,input.border,input.internalformat.type,input.type,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_INVALID_VALUE);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT

return 0;
}
//------------------------------------------------------------------------------
// проверка корректности обработки значений аргументов width, height и level при проверке на соответствие размеров изображения текстуры значению аргумента level.
int nmglTexImage2D_wrongLevelAndSize_isError()
{
	//CHANGE_REPORT
	cntxt.error=NMGL_NO_ERROR;
	cntxt.activeTexUnitIndex=0;
	_init_tex_obj(0);
	ActiveTexObject=&cntxt.texObjects[0];
	ActiveTexObject->texMinFilter=NMGL_NEAREST_MIPMAP_NEAREST;
	status=init_TexImage2D_input(&input,16,internalformats[0]);//CHANGE_REPORT
		nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.internalformat.type,input.type,input.pixels);
	status=init_TexImage2D_input(&input,8,internalformats[0]);
	input.level=1;
		nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.internalformat.type,input.type,input.pixels);

	TEST_ASSERT(cntxt.error==NMGL_NO_ERROR);
	input.level=2;
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.internalformat.type,input.type,input.pixels);
	//printf("cntxt.error=%x\n",cntxt.error);
	TEST_ASSERT(cntxt.error==NMGL_INVALID_OPERATION);
	
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT
return 0;
}
//------------------------------------------------------------------------------
// проверка корректности обработки значений аргументов internalformat и format.
int nmglTexImage2D_wrongInternalformatFormat_isError()
{
	cntxt.error=NMGL_NO_ERROR;
	
	for(i=0;i<6;i++)
	{
		status=init_TexImage2D_input(&input,16,internalformats[i]);
		if(status==0) 
		{
			printf("ERROR!Can't allocate pixels array!\n");
			return 2;
		}
		nmglTexImage2D(input.target,input.level,cur_format,input.width,input.height,input.border,cur_format,input.type,input.pixels);
		TEST_ASSERT(cntxt.error==NMGL_NO_ERROR);
		_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT		
	}
	input.internalformat.type=(internalformats[0].type<<1)+1;
	input.format=input.internalformat.type;
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_INVALID_VALUE);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT		
	input.format=input.internalformat.type+1;
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_INVALID_VALUE);
		_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	
		return 0;
}
//------------------------------------------------------------------------------
//проверка корректности обработки значений аргумента width.
int nmglTexImage2D_wrongWidth_isError()
{
	cntxt.error=NMGL_NO_ERROR;
	status=init_TexImage2D_input(&input,16,internalformats[0]);
		if(status==0) 
		{
			printf("ERROR!Can't allocate pixels array!\n");
			return 2;
		}
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_NO_ERROR);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	

	nmglTexImage2D(input.target,input.level,input.internalformat.type,-1,input.height,input.border,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_INVALID_VALUE);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	
	return 0;	
}
//------------------------------------------------------------------------------
//проверка корректности обработки значений аргумента height.
int nmglTexImage2D_wrongHeight_isError()
{
	cntxt.error=NMGL_NO_ERROR;
	status=init_TexImage2D_input(&input,16,internalformats[0]);
		if(status==0) 
		{
			printf("ERROR!Can't allocate pixels array!\n");
			return 2;
		}
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_NO_ERROR);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	

	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,-1,input.border,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_INVALID_VALUE);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	
	return 0;
}
//------------------------------------------------------------------------------
//проверка корректности обработки значений аргумента border.
int nmglTexImage2D_wrongBorder_isError()
{
cntxt.error=NMGL_NO_ERROR;
	status=init_TexImage2D_input(&input,16,internalformats[0]);
		if(status==0) 
		{
			printf("ERROR!Can't allocate pixels array!\n");
			return 2;
		}
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_NO_ERROR);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	

	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,5,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_INVALID_VALUE);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	
	return 0;
}
//------------------------------------------------------------------------------
//проверка корректности обработки значений аргумента type.
int nmglTexImage2D_wrongType_isError()
{
	cntxt.error=NMGL_NO_ERROR;
	status=init_TexImage2D_input(&input,16,internalformats[0]);
		if(status==0) 
		{
			printf("ERROR!Can't allocate pixels array!\n");
			return 2;
		}
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.format,input.type,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_NO_ERROR);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT	

	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.format,NMGL_FLOAT,input.pixels);
	TEST_ASSERT(cntxt.error==NMGL_INVALID_ENUM);
	_nmglTexImage2D_prevent_internal_errors();//CHANGE_REPORT
	return 0;
}

//проверка корректности установки соответствующих значений полей контекста.
int nmglTexImage2D_setImage_contextSetCorrect()
{
	cntxt.error=NMGL_NO_ERROR;
	status=init_TexImage2D_input(&input,16,internalformats[0]);
		if(status==0) 
		{
			printf("ERROR!Can't allocate pixels array!\n");
			return 2;
		}
	nmglTexImage2D(input.target,input.level,input.internalformat.type,input.width,input.height,input.border,input.format,input.type,input.pixels);
	status=_nmglTexImage2D_check(0);
	TEST_ASSERT(status==0);
	return 0;
}
#endif