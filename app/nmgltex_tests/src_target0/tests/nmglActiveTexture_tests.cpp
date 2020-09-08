#include "demo3d_nm0.h"
#include "nmgldef.h"
#include "nmgltype.h"
#include "nmgl.h"

#include "nmgltex_common.h"
#include "tests.h"

#include <stdio.h>
#include <assert.h>

//Тестовые сценарии
int nmglActiveTexture_wrongTexture_isError();
int nmglActiveTexture_setActiveTexture_setContextCorrect();

//Главная тестовая функция
int run_nmglActiveTexture_test()
{
	printf ("\nStart nmglActiveTexture tests\n\n");
	RUN_TEST(nmglActiveTexture_wrongTexture_isError);
	RUN_TEST(nmglActiveTexture_setActiveTexture_setContextCorrect);
	printf ("\nEnd nmglActiveTexture tests\n");
	return 0;
}

int nmglActiveTexture_wrongTexture_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	cntxt->error = NMGL_NO_ERROR;

	nmglActiveTexture(NMGL_TEXTURE0);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	nmglActiveTexture(NMGL_TEXTURE0 + NMGL_MAX_TEX_UNITS);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	cntxt->error = NMGL_NO_ERROR;
	nmglActiveTexture(NMGL_TEXTURE0 - 1);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	return 0;
}

int nmglActiveTexture_setActiveTexture_setContextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	cntxt->texState.activeTexUnit = NMGL_TEXTURE0 + 1;
	cntxt->texState.activeTexUnitIndex = 1;

	nmglActiveTexture(NMGL_TEXTURE0);
	TEST_ASSERT((cntxt->texState.activeTexUnit == NMGL_TEXTURE0) && (cntxt->texState.activeTexUnitIndex == 0));
	return 0;	
}
