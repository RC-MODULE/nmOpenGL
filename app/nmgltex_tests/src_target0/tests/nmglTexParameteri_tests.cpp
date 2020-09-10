#include "demo3d_nm0.h"
#include "nmgldef.h"
#include "nmgltype.h"
#include "nmgl.h"

#include "nmgltex_common.h"
#include "tests.h"

#include <stdio.h>
#include <assert.h>

//To check data at nmpu1
#include "demo3d_nm1.h"
extern NMGL_Context_NM1 *cntxt_nm1;
extern unsigned int nmpu1IsAccessible;


//Тестовые сценарии
int nmglTexParameteri_wrongTarget_isError();
int nmglTexParameteri_wrongPnameParam_isError();
int nmglTexParameteri_setParam_setContextCorrect();

//Главная тестовая функция
int run_nmglTexParameteri_test()
{
	printf ("\nStart nmglTexParameteri tests\n\n");

	RUN_TEST(nmglTexParameteri_wrongTarget_isError);
	RUN_TEST(nmglTexParameteri_wrongPnameParam_isError);
	RUN_TEST(nmglTexParameteri_setParam_setContextCorrect);

	printf ("\nEnd nmglTexParameteri tests\n");
	return 0;
}

int nmglTexParameteri_wrongTarget_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	cntxt->error = NMGL_NO_ERROR;

	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_S, NMGL_REPEAT);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	nmglTexParameteri(NMGL_TEXTURE_2D+1, NMGL_TEXTURE_WRAP_S, NMGL_REPEAT);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	return 0;
}


int nmglTexParameteri_wrongPnameParam_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	NMGLint wrapValues [2] = {NMGL_REPEAT, NMGL_CLAMP_TO_EDGE};
	NMGLint minFilterValues [6] = {	NMGL_NEAREST,
									NMGL_LINEAR,
									NMGL_NEAREST_MIPMAP_NEAREST,
									NMGL_NEAREST_MIPMAP_LINEAR,
									NMGL_LINEAR_MIPMAP_NEAREST,
									NMGL_LINEAR_MIPMAP_LINEAR};

	NMGLint magFilterValues [2] = {	NMGL_NEAREST,
									NMGL_LINEAR};

	//default values
	NMGLint wrapSParam = NMGL_REPEAT;
	NMGLint wrapTParam = NMGL_REPEAT;
	NMGLint minFilterParam = NMGL_NEAREST_MIPMAP_LINEAR;
	NMGLint magFilterParam = NMGL_LINEAR;

	int i = 0;
	TexObject* boundTexObject;
	TexObject* boundTexObject1;

	//set name and default values
	boundTexObject = &cntxt->texState.texObjects[0];
	
	boundTexObject->texWrapS = wrapSParam;
	boundTexObject->texWrapT = wrapTParam;
	boundTexObject->texMinFilter = minFilterParam;
	boundTexObject->texMagFilter = magFilterParam;

	cntxt->texState.activeTexUnit = NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex = 0;
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject = boundTexObject;
	cntxt->error = NMGL_NO_ERROR;

	if (nmpu1IsAccessible){
		boundTexObject1 = &cntxt_nm1->texState.texObjects[0];
		
		boundTexObject1->texWrapS = wrapSParam;
		boundTexObject1->texWrapT = wrapTParam;
		boundTexObject1->texMinFilter = minFilterParam;
		boundTexObject1->texMagFilter = magFilterParam;

		cntxt_nm1->texState.activeTexUnit = NMGL_TEXTURE0;
		cntxt_nm1->texState.activeTexUnitIndex = 0;
		cntxt_nm1->texState.texUnits[cntxt_nm1->texState.activeTexUnitIndex].boundTexObject = boundTexObject1;
	}


	for (i = 0; i < 2; i++)
	{
		wrapSParam = wrapValues[i];
		nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_S, wrapSParam);
		halSleep(500);
		TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
		TEST_ASSERT(boundTexObject->texWrapS == wrapSParam);
		if (nmpu1IsAccessible){
			TEST_ASSERT(boundTexObject1->texWrapS == wrapSParam);
		}
	}

	for (i = 0; i < 2; i++)
	{
		wrapTParam = wrapValues[i];
		nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_T, wrapTParam);
		halSleep(500);
		TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
		TEST_ASSERT(boundTexObject->texWrapT == wrapTParam);
		if (nmpu1IsAccessible){
			TEST_ASSERT(boundTexObject1->texWrapT == wrapTParam);
		}
	}


	for (i = 0; i < 6; i++)
	{
		minFilterParam = minFilterValues[i];
		nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MIN_FILTER, minFilterParam);
		halSleep(500);
		TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
		TEST_ASSERT(boundTexObject->texMinFilter == minFilterParam);
		if (nmpu1IsAccessible){
			TEST_ASSERT(boundTexObject1->texMinFilter == minFilterParam);
		}
	}

	for (i = 0; i < 2; i++)
	{
		magFilterParam = magFilterValues[i];
		nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MAG_FILTER, magFilterParam);
		halSleep(500);
		TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
		TEST_ASSERT(boundTexObject->texMagFilter == magFilterParam);
		if (nmpu1IsAccessible){
			TEST_ASSERT(boundTexObject1->texMagFilter == magFilterParam);
		}
	}

	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_2D, magFilterParam);
	halSleep(500);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
	TEST_ASSERT(boundTexObject->texWrapS == wrapSParam);
	TEST_ASSERT(boundTexObject->texWrapT == wrapTParam);
	TEST_ASSERT(boundTexObject->texMinFilter == minFilterParam);
	TEST_ASSERT(boundTexObject->texMagFilter == magFilterParam);
	if (nmpu1IsAccessible){
		TEST_ASSERT(boundTexObject1->texWrapS == wrapSParam);
		TEST_ASSERT(boundTexObject1->texWrapT == wrapTParam);
		TEST_ASSERT(boundTexObject1->texMinFilter == minFilterParam);
		TEST_ASSERT(boundTexObject1->texMagFilter == magFilterParam);		
	}

	cntxt->error = NMGL_NO_ERROR;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_S, NMGL_TEXTURE_WRAP_S);
	halSleep(500);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);

	cntxt->error = NMGL_NO_ERROR;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_T, NMGL_TEXTURE_WRAP_T);
	halSleep(500);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);

	cntxt->error = NMGL_NO_ERROR;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MIN_FILTER, NMGL_TEXTURE_MIN_FILTER);
	halSleep(500);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);

	cntxt->error = NMGL_NO_ERROR;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MAG_FILTER, NMGL_TEXTURE_MAG_FILTER);
	halSleep(500);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);

	TEST_ASSERT(boundTexObject->texWrapS == wrapSParam);
	TEST_ASSERT(boundTexObject->texWrapT == wrapTParam);
	TEST_ASSERT(boundTexObject->texMinFilter == minFilterParam);
	TEST_ASSERT(boundTexObject->texMagFilter == magFilterParam);
	if (nmpu1IsAccessible){
		TEST_ASSERT(boundTexObject1->texWrapS == wrapSParam);
		TEST_ASSERT(boundTexObject1->texWrapT == wrapTParam);
		TEST_ASSERT(boundTexObject1->texMinFilter == minFilterParam);
		TEST_ASSERT(boundTexObject1->texMagFilter == magFilterParam);
	}

	return 0;
}


int nmglTexParameteri_setParam_setContextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	int i = 0;
	TexObject* boundTexObject;
	TexObject* boundTexObject1;

	//default values
	NMGLint wrapSParam = NMGL_REPEAT;
	NMGLint wrapTParam = NMGL_REPEAT;
	NMGLint minFilterParam = NMGL_NEAREST_MIPMAP_LINEAR;
	NMGLint magFilterParam = NMGL_LINEAR;

	cntxt->error = NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit = NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex = 0;

	if (nmpu1IsAccessible){
		cntxt_nm1->texState.activeTexUnit = NMGL_TEXTURE0;
		cntxt_nm1->texState.activeTexUnitIndex = 0;
	}

	boundTexObject = &cntxt->texState.texObjects[0];
	boundTexObject->texWrapS = wrapSParam;
	boundTexObject->texWrapT = wrapTParam;
	boundTexObject->texMinFilter = minFilterParam;
	boundTexObject->texMagFilter = magFilterParam;
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject = boundTexObject;

	if (nmpu1IsAccessible){
		boundTexObject1 = &cntxt_nm1->texState.texObjects[0];
		boundTexObject1->texWrapS = wrapSParam;
		boundTexObject1->texWrapT = wrapTParam;
		boundTexObject1->texMinFilter = minFilterParam;
		boundTexObject1->texMagFilter = magFilterParam;
		cntxt_nm1->texState.texUnits[cntxt_nm1->texState.activeTexUnitIndex].boundTexObject = boundTexObject1;
	}
	
	wrapSParam = NMGL_CLAMP_TO_EDGE;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_S, wrapSParam);
	halSleep(500);

	TEST_ASSERT(boundTexObject->texWrapS == wrapSParam)
	TEST_ASSERT(boundTexObject->texWrapT == wrapTParam)
	TEST_ASSERT(boundTexObject->texMinFilter == minFilterParam)
	TEST_ASSERT(boundTexObject->texMagFilter == magFilterParam)

	if (nmpu1IsAccessible){
		TEST_ASSERT(boundTexObject1->texWrapS == wrapSParam)
		TEST_ASSERT(boundTexObject1->texWrapT == wrapTParam)
		TEST_ASSERT(boundTexObject1->texMinFilter == minFilterParam)
		TEST_ASSERT(boundTexObject1->texMagFilter == magFilterParam)
	}
	
	wrapTParam = NMGL_CLAMP_TO_EDGE;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_T, wrapTParam);
	halSleep(500);

	TEST_ASSERT(boundTexObject->texWrapS == wrapSParam)
	TEST_ASSERT(boundTexObject->texWrapT == wrapTParam)
	TEST_ASSERT(boundTexObject->texMinFilter == minFilterParam)
	TEST_ASSERT(boundTexObject->texMagFilter == magFilterParam)

	if (nmpu1IsAccessible){
		TEST_ASSERT(boundTexObject1->texWrapS == wrapSParam)
		TEST_ASSERT(boundTexObject1->texWrapT == wrapTParam)
		TEST_ASSERT(boundTexObject1->texMinFilter == minFilterParam)
		TEST_ASSERT(boundTexObject1->texMagFilter == magFilterParam)
	}
	
	minFilterParam = NMGL_LINEAR;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MIN_FILTER, minFilterParam);
	halSleep(500);

	TEST_ASSERT(boundTexObject->texWrapS == wrapSParam)
	TEST_ASSERT(boundTexObject->texWrapT == wrapTParam)
	TEST_ASSERT(boundTexObject->texMinFilter == minFilterParam)
	TEST_ASSERT(boundTexObject->texMagFilter == magFilterParam)

	if (nmpu1IsAccessible){
		TEST_ASSERT(boundTexObject1->texWrapS == wrapSParam)
		TEST_ASSERT(boundTexObject1->texWrapT == wrapTParam)
		TEST_ASSERT(boundTexObject1->texMinFilter == minFilterParam)
		TEST_ASSERT(boundTexObject1->texMagFilter == magFilterParam)
	}
	
	magFilterParam = NMGL_NEAREST;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MAG_FILTER, magFilterParam);
	halSleep(500);

	TEST_ASSERT(boundTexObject->texWrapS == wrapSParam)
	TEST_ASSERT(boundTexObject->texWrapT == wrapTParam)
	TEST_ASSERT(boundTexObject->texMinFilter == minFilterParam)
	TEST_ASSERT(boundTexObject->texMagFilter == magFilterParam)

	if (nmpu1IsAccessible){
		TEST_ASSERT(boundTexObject1->texWrapS == wrapSParam)
		TEST_ASSERT(boundTexObject1->texWrapT == wrapTParam)
		TEST_ASSERT(boundTexObject1->texMinFilter == minFilterParam)
		TEST_ASSERT(boundTexObject1->texMagFilter == magFilterParam)
	}
	
	return 0;
}
