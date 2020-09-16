#include "demo3d_nm0.h"
#include "nmgldef.h"
#include "nmgltype.h"
#include "nmgl.h"

#include "nmgltex_common.h"
#include "tests.h"

#include <stdio.h>
#include <assert.h>

//Тестовые сценарии
int nmglMultiTexCoord2f_wrongTexture_isError();
int nmglMultiTexCoord2f_setCoords_setContextCorrect();

//Главная тестовая функция
int run_nmglMultiTexCoord2f_test()
{
	printf ("\nStart nmglMultiTexCoord2f tests\n\n");
	RUN_TEST(nmglMultiTexCoord2f_wrongTexture_isError);
	RUN_TEST(nmglMultiTexCoord2f_setCoords_setContextCorrect);
	printf ("\nEnd nmglMultiTexCoord2f tests\n");
	return 0;
}

int nmglMultiTexCoord2f_wrongTexture_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	cntxt->error = NMGL_NO_ERROR;

	nmglMultiTexCoord2f (NMGL_TEXTURE0, 0.0f, 0.0f);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	nmglMultiTexCoord2f (NMGL_TEXTURE0 + NMGL_MAX_TEX_UNITS, 0.0f, 0.0f);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	cntxt->error = NMGL_NO_ERROR;
	nmglMultiTexCoord2f (NMGL_TEXTURE0 - 1, 0.0f, 0.0f);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	return 0;
}

int nmglMultiTexCoord2f_setCoords_setContextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	int i = 0;
	unsigned int texture0_index = 0;
	TexCoords etalon_values[4] = {
	{ 0.1f,  0.2f},
	{ 0.3f,  0.4f},
	{-0.1f, -0.1f},
	{ 1.3f,  1.4f}
	};

	cntxt->texState.curTexCoords[texture0_index].s = 0.0f;
	cntxt->texState.curTexCoords[texture0_index].t = 0.0f;

	for (i = 0; i < 4; i++)
	{
		nmglMultiTexCoord2f (NMGL_TEXTURE0, etalon_values[i].s, etalon_values[i].t);
		TEST_ASSERT((cntxt->texState.curTexCoords[texture0_index].s == etalon_values[i].s) &&
				    (cntxt->texState.curTexCoords[texture0_index].t == etalon_values[i].t))
	}

	return 0;	
}
