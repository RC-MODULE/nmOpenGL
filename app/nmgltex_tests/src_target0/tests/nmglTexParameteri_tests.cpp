#include "demo3d_nm0.h"
#include "nmgldef.h"
#include "nmgltype.h"
#include "nmgl.h"

#include "nmgltex_common.h"
#include "tests.h"

#include <stdio.h>
#include <assert.h>

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
	TexObject boundTexObject;

	//set name and default values
	boundTexObject.texWrapS = wrapSParam;
	boundTexObject.texWrapT = wrapTParam;
	boundTexObject.texMinFilter = minFilterParam;
	boundTexObject.texMagFilter = magFilterParam;

	cntxt->texState.activeTexUnit = NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex = 0;
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject = &boundTexObject;
	cntxt->error = NMGL_NO_ERROR;




	for (i = 0; i < 2; i++)
	{
		wrapSParam = wrapValues[i];
		nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_S, wrapSParam);
		TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
		TEST_ASSERT(boundTexObject.texWrapS == wrapSParam);
	}

	for (i = 0; i < 2; i++)
	{
		wrapTParam = wrapValues[i];
		nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_T, wrapTParam);
		TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
		TEST_ASSERT(boundTexObject.texWrapT == wrapTParam);
	}


	for (i = 0; i < 6; i++)
	{
		minFilterParam = minFilterValues[i];
		nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MIN_FILTER, minFilterParam);
		TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
		TEST_ASSERT(boundTexObject.texMinFilter == minFilterParam);
	}

	for (i = 0; i < 2; i++)
	{
		magFilterParam = magFilterValues[i];
		nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MAG_FILTER, magFilterParam);
		TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
		TEST_ASSERT(boundTexObject.texMagFilter == magFilterParam);
	}

	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_2D, magFilterParam);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
	TEST_ASSERT(boundTexObject.texWrapS == wrapSParam);
	TEST_ASSERT(boundTexObject.texWrapT == wrapTParam);
	TEST_ASSERT(boundTexObject.texMinFilter == minFilterParam);
	TEST_ASSERT(boundTexObject.texMagFilter == magFilterParam);

	cntxt->error = NMGL_NO_ERROR;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_S, NMGL_TEXTURE_WRAP_S);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);

	cntxt->error = NMGL_NO_ERROR;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_T, NMGL_TEXTURE_WRAP_T);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);

	cntxt->error = NMGL_NO_ERROR;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MIN_FILTER, NMGL_TEXTURE_MIN_FILTER);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);

	cntxt->error = NMGL_NO_ERROR;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MAG_FILTER, NMGL_TEXTURE_MAG_FILTER);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);

	TEST_ASSERT(boundTexObject.texWrapS == wrapSParam);
	TEST_ASSERT(boundTexObject.texWrapT == wrapTParam);
	TEST_ASSERT(boundTexObject.texMinFilter == minFilterParam);
	TEST_ASSERT(boundTexObject.texMagFilter == magFilterParam);

	return 0;
}


int nmglTexParameteri_setParam_setContextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	int i = 0;
	TexObject boundTexObject;

	//default values
	NMGLint wrapSParam = NMGL_REPEAT;
	NMGLint wrapTParam = NMGL_REPEAT;
	NMGLint minFilterParam = NMGL_NEAREST_MIPMAP_LINEAR;
	NMGLint magFilterParam = NMGL_LINEAR;

	cntxt->texState.activeTexUnit = NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex = 0;
	cntxt->error = NMGL_NO_ERROR;

	//set name and default values
	boundTexObject.texWrapS = wrapSParam;
	boundTexObject.texWrapT = wrapTParam;
	boundTexObject.texMinFilter = minFilterParam;
	boundTexObject.texMagFilter = magFilterParam;

	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject = &boundTexObject;

	wrapSParam = NMGL_CLAMP_TO_EDGE;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_S, wrapSParam);

	TEST_ASSERT(boundTexObject.texWrapS == wrapSParam)
	TEST_ASSERT(boundTexObject.texWrapT == wrapTParam)
	TEST_ASSERT(boundTexObject.texMinFilter == minFilterParam)
	TEST_ASSERT(boundTexObject.texMagFilter == magFilterParam)

	wrapTParam = NMGL_CLAMP_TO_EDGE;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_T, wrapTParam);

	TEST_ASSERT(boundTexObject.texWrapS == wrapSParam)
	TEST_ASSERT(boundTexObject.texWrapT == wrapTParam)
	TEST_ASSERT(boundTexObject.texMinFilter == minFilterParam)
	TEST_ASSERT(boundTexObject.texMagFilter == magFilterParam)

	minFilterParam = NMGL_LINEAR;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MIN_FILTER, minFilterParam);

	TEST_ASSERT(boundTexObject.texWrapS == wrapSParam)
	TEST_ASSERT(boundTexObject.texWrapT == wrapTParam)
	TEST_ASSERT(boundTexObject.texMinFilter == minFilterParam)
	TEST_ASSERT(boundTexObject.texMagFilter == magFilterParam)

	magFilterParam = NMGL_NEAREST;
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MAG_FILTER, magFilterParam);

	TEST_ASSERT(boundTexObject.texWrapS == wrapSParam)
	TEST_ASSERT(boundTexObject.texWrapT == wrapTParam)
	TEST_ASSERT(boundTexObject.texMinFilter == minFilterParam)
	TEST_ASSERT(boundTexObject.texMagFilter == magFilterParam)

	return 0;
}
