#include "demo3d_nm0.h"
#include "nmgldef.h"
#include "nmgltype.h"
#include "nmgl.h"

#include "nmgltex_common.h"
#include "tests.h"

#include <stdio.h>
#include <assert.h>

//Тестовые сценарии
int nmglTexEnvi_wrongTarget_isError();
int nmglTexEnvi_wrongPnameParams_isError();
int nmglTexEnvi_setParam_setContextCorrect();

//Главная тестовая функция
int run_nmglTexEnvi_test()
{
	printf ("\nStart nmglTexEnvi tests\n\n");

	RUN_TEST(nmglTexEnvi_wrongTarget_isError);
	RUN_TEST(nmglTexEnvi_wrongPnameParams_isError);
	RUN_TEST(nmglTexEnvi_setParam_setContextCorrect);

	printf ("\nEnd nmglTexEnvi tests\n");
	return 0;
}

int nmglTexEnvi_wrongTarget_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	cntxt->error = NMGL_NO_ERROR;

	nmglTexEnvi(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_MODE, NMGL_MODULATE);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	nmglTexEnvi(NMGL_TEXTURE_ENV+1, NMGL_TEXTURE_ENV_MODE, NMGL_MODULATE);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	return 0;
}


int nmglTexEnvi_wrongPnameParams_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	cntxt->error = NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit = NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex = 0;
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName = NMGL_MODULATE;

	NMGLint texFunctionNames [5] = {NMGL_REPLACE, NMGL_DECAL, NMGL_BLEND, NMGL_ADD, NMGL_MODULATE};
	NMGLint texFunctionName = NMGL_REPLACE; //last value
	int i = 0;

	for (i = 0; i < 5; i++)
	{
		texFunctionName = texFunctionNames[i];
		nmglTexEnvi(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_MODE, texFunctionName);
		TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
		TEST_ASSERT(cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName == texFunctionName);
	}

	cntxt->error = NMGL_NO_ERROR;

	nmglTexEnvi(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_MODE+1, NMGL_BLEND);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
	TEST_ASSERT(cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName == texFunctionName);

	nmglTexEnvi(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_MODE, NMGL_TEXTURE_ENV_MODE);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);
	TEST_ASSERT(cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName == texFunctionName);

	return 0;
}


int nmglTexEnvi_setParam_setContextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	cntxt->error = NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit = NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex = 0;
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName = NMGL_MODULATE;

	int i = 0;
	NMGLint texFunctionNames [5] = {NMGL_REPLACE, NMGL_DECAL, NMGL_BLEND, NMGL_ADD, NMGL_MODULATE};

	for (i = 0; i < 5; i++)
	{
		nmglTexEnvi(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_MODE, texFunctionNames[i]);
		TEST_ASSERT(cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName == texFunctionNames[i]);
	}

	return 0;
}
