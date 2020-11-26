#include "demo3d_nm0.h"
//include "nmgldef.h"
//#include "nmgltype.h"
//#include "nmgl_data0.h"

//#include "tex_common.h"
//#include "tex_functions.h"
#include "tests.h"

//#include <iostream>
//#include <cstdio>

#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif
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
void nmglGenTextures (NMGLsizei n, NMGLuint *textures);
//////////////////////////////////////////////////////////////////

TODO	
*/

/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglGenTextures_genTextures_correctCountAndValues();
int nmglGenTextures_genTextures_setContextCorrect();
//----------------------------------------------------------------
int iter=0;
unsigned int uTexName;
NMGLuint values[9];
/////////////////////////////////////////////////////////////////////////////////////////////////


//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglGenTextures_test()
{
	
	//std::cout << "Hello";
	printf ("\nStart nmglGenTextures tests\n\n");
	//NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	//printf("context pointer is %x\n",cntxt);
//cntxt->texState.init();
		RUN_TEST(nmglGenTextures_genTextures_correctCountAndValues);
		RUN_TEST(nmglGenTextures_genTextures_setContextCorrect);
	printf ("\nEnd nmglGenTextures tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================

//void nmglGenTextures (NMGLsizei n, NMGLuint *textures);
// проверка корректности формирования заданного количества текстурных объектов
//CHANGE_REPORT
int nmglGenTextures_genTextures_correctCountAndValues()
{
	
	//printf("Values[0]=%d",values[0]);
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	uTexName=cntxt->texState.lastTexName;
	cntxt->texState.lastTexName=0;
	//printf("context pointer is %x\n",cntxt);
	//printf("lastTexName=%d\n",cntxt->texState.lastTexName);
//cntxt->texState.lastTexName=0;
	nmglGenTextures(1,values);
		//printf("values[0]=%d\n",values[0]);

	//printf("Values[0]=%d",values[0]);
	nmglGenTextures(1,values+1);
	nmglGenTextures(3,values+2);
	nmglGenTextures(4,values+5);
	/*printf("Values are:\n");
	for(iter=0;iter<9;iter++)
	{
		printf("%d ",values[iter]);
	}
	printf("\n");
		*/
	for(iter=0;iter<9;iter++)
	{
		if(values[iter]!=(iter+1))
		{
			printf("values[iter]=%d iter=%d\n",values[iter],iter);
			TEST_ASSERT(0);
			#ifdef DEBUG
				for(iter=0;iter<9;iter++) DEBUG_PRINT(("%d ",values[iter]));
				DEBUG_PRINT(("\n"));
			#endif
			return 2;
		}
	}
	cntxt->texState.lastTexName=uTexName;
	TEST_ASSERT(1);
	return 0;
}
//------------------------------------------------------------------------------
// проверка корректности установки соответствующих полей контекста
int nmglGenTextures_genTextures_setContextCorrect()
{
	//CHANGE_REPORT
	/*
		1 вызов - 10
		2 вызов - 11
		3 вызов - 12,13,14
		4 вызов - 15,16,17,18
	*/
	int cs=0;
	//cntxt.lastTexName=0;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	uTexName=cntxt->texState.lastTexName;
	cntxt->texState.lastTexName=0;
	nmglGenTextures(1,values);
	if(cntxt->texState.lastTexName != 1) cs++;
	nmglGenTextures(1,values+1);
	if(cntxt->texState.lastTexName != 2) cs++;
	nmglGenTextures(3,values+2);
	if(cntxt->texState.lastTexName != 5) cs++;
	nmglGenTextures(4,values+5);
	if(cntxt->texState.lastTexName != 9) cs++;
	TEST_ASSERT(cs==0);
	cntxt->texState.lastTexName=uTexName;
return 0;
}
