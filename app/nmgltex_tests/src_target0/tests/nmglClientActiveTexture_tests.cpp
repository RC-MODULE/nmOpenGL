#include "demo3d_nm0.h"
#include "nmgldef.h"
#include "nmgltype.h"
#include "nmgl.h"

#include "nmgltex_common.h"
#include "tests.h"

#include <stdio.h>
#include <assert.h>

//Тестовые сценарии
int nmglClientActiveTexture_wrongTexture_isError();
int nmglClientActiveTexture_setActiveTexture_setContextCorrect();

//Главная тестовая функция
int run_nmglClientActiveTexture_test()
{
	printf ("\nStart nmglClientActiveTexture tests\n\n");
	RUN_TEST(nmglClientActiveTexture_wrongTexture_isError);
	RUN_TEST(nmglClientActiveTexture_setActiveTexture_setContextCorrect);
	printf ("\nEnd nmglClientActiveTexture tests\n");
	return 0;
}

int nmglClientActiveTexture_wrongTexture_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	cntxt->error = NMGL_NO_ERROR;

	nmglClientActiveTexture(NMGL_TEXTURE0);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	nmglClientActiveTexture(NMGL_TEXTURE0 + NMGL_MAX_TEX_UNITS);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	cntxt->error = NMGL_NO_ERROR;
	nmglClientActiveTexture(NMGL_TEXTURE0 - 1);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	return 0;
}

int nmglClientActiveTexture_setActiveTexture_setContextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	cntxt->texState.clientActiveTexUnit = NMGL_TEXTURE0 + 1;
	cntxt->texState.clientActiveTexUnitIndex = 1;

	nmglClientActiveTexture(NMGL_TEXTURE0);
	TEST_ASSERT((cntxt->texState.clientActiveTexUnit == NMGL_TEXTURE0) && (cntxt->texState.clientActiveTexUnitIndex == 0));
	return 0;	
}
