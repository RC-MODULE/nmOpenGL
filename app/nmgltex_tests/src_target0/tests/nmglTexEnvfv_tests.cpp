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
int nmglTexEnvfv_wrongTarget_isError();
int nmglTexEnvfv_wrongPnameParams_isError();
int nmglTexEnvfv_nullPointer_isError();
int nmglTexEnvfv_setParam_setContextCorrect();

//Главная тестовая функция
int run_nmglTexEnvfv_test()
{
	printf ("\nStart nmglTexEnvfv tests\n\n");

	RUN_TEST(nmglTexEnvfv_wrongTarget_isError);
	RUN_TEST(nmglTexEnvfv_wrongPnameParams_isError);
	RUN_TEST(nmglTexEnvfv_nullPointer_isError);
	RUN_TEST(nmglTexEnvfv_setParam_setContextCorrect);

	printf ("\nEnd nmglTexEnvfv tests\n");
	return 0;
}

int nmglTexEnvfv_wrongTarget_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	NMGLfloat array[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	cntxt->error = NMGL_NO_ERROR;

	nmglTexEnvfv(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_COLOR, array);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	nmglTexEnvfv(NMGL_TEXTURE_ENV+1, NMGL_TEXTURE_ENV_COLOR, array);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	return 0;
}


int nmglTexEnvfv_wrongPnameParams_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	NMGLfloat array[4] = {0.1f, 0.2f, 0.3f, 0.4f};

	cntxt->error = NMGL_NO_ERROR;
	
	cntxt->texState.activeTexUnit = NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex = 0;
	if (nmpu1IsAccessible){
		cntxt_nm1->texState.activeTexUnit = NMGL_TEXTURE0;
		cntxt_nm1->texState.activeTexUnitIndex = 0;
	}

	nmglTexEnvfv(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_COLOR, array);
	halSleep(500);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
	TEST_ARRAYS_EQUAL(array,cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor,4)
	if (nmpu1IsAccessible){
		TEST_ARRAYS_EQUAL(array,cntxt_nm1->texState.texUnits[cntxt_nm1->texState.activeTexUnitIndex].texEnvColor,4)
	}

	nmglTexEnvfv(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_COLOR+1, array);
	halSleep(500);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
	TEST_ARRAYS_EQUAL(array,cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor,4)
	if (nmpu1IsAccessible){
		TEST_ARRAYS_EQUAL(array,cntxt_nm1->texState.texUnits[cntxt_nm1->texState.activeTexUnitIndex].texEnvColor,4)
	}

	return 0;
}


int nmglTexEnvfv_nullPointer_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	NMGLfloat array[4] = {0.1f, 0.2f, 0.3f, 0.4f};

	cntxt->error = NMGL_NO_ERROR;

	nmglTexEnvfv(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_COLOR, array);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	nmglTexEnvfv(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_COLOR, NULL);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);
	return 0;
}


int nmglTexEnvfv_setParam_setContextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	NMGLfloat params[4] = {0.1f, 0.2f, 0.3f, 0.4f};

	cntxt->error = NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit = NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex = 0;
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[0] = 0.0f;
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[1] = 0.0f;
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[2] = 0.0f;
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[3] = 0.0f;
	
	if (nmpu1IsAccessible){
		cntxt_nm1->texState.activeTexUnit = NMGL_TEXTURE0;
		cntxt_nm1->texState.activeTexUnitIndex = 0;
		cntxt_nm1->texState.texUnits[cntxt_nm1->texState.activeTexUnitIndex].texEnvColor[0] = 0.0f;
		cntxt_nm1->texState.texUnits[cntxt_nm1->texState.activeTexUnitIndex].texEnvColor[1] = 0.0f;
		cntxt_nm1->texState.texUnits[cntxt_nm1->texState.activeTexUnitIndex].texEnvColor[2] = 0.0f;
		cntxt_nm1->texState.texUnits[cntxt_nm1->texState.activeTexUnitIndex].texEnvColor[3] = 0.0f;
	}
		
		

	nmglTexEnvfv(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_COLOR, params);
	halSleep(500);
	TEST_ARRAYS_EQUAL(params,cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor,4)
	if (nmpu1IsAccessible){
		TEST_ARRAYS_EQUAL(params,cntxt_nm1->texState.texUnits[cntxt_nm1->texState.activeTexUnitIndex].texEnvColor,4)
	}

	params[0] = -0.3f;
	params[1] = 1.4f;
	params[2] = -0.5f;
	params[3] = 1.6f;

	nmglTexEnvfv(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_COLOR, params);
	halSleep(500);

	params[0] = 0.0f;
	params[1] = 1.0f;
	params[2] = 0.0f;
	params[3] = 1.0f;

	TEST_ARRAYS_EQUAL(params,cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor,4)
	if (nmpu1IsAccessible){
		TEST_ARRAYS_EQUAL(params,cntxt_nm1->texState.texUnits[cntxt_nm1->texState.activeTexUnitIndex].texEnvColor,4)
	}

	return 0;
}
