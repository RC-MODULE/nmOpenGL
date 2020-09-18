#include "demo3d_nm0.h"
#include "nmgldef.h"
#include "nmgltype.h"
#include "nmgl.h"

#include "nmgltex_common.h"
#include "tests.h"

#include <stdio.h>
#include <assert.h>

//Тестовые сценарии
int nmglMultiTexCoord2fv_wrongTexture_isError();
int nmglMultiTexCoord2fv_setCoords_setContextCorrect();
int nmglMultiTexCoord2fv_nullPointer_isError();

//Главная тестовая функция
int run_nmglMultiTexCoord2fv_test()
{
	printf ("\nStart nmglMultiTexCoord2fv tests\n\n");
	RUN_TEST(nmglMultiTexCoord2fv_wrongTexture_isError);
	RUN_TEST(nmglMultiTexCoord2fv_nullPointer_isError);
	RUN_TEST(nmglMultiTexCoord2fv_setCoords_setContextCorrect);
	printf ("\nEnd nmglMultiTexCoord2fv tests\n");
	return 0;
}

int nmglMultiTexCoord2fv_wrongTexture_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	NMGLfloat v[2] = {0.0f, 0.0f};

	cntxt->error = NMGL_NO_ERROR;

	nmglMultiTexCoord2fv (NMGL_TEXTURE0, v);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	nmglMultiTexCoord2fv (NMGL_TEXTURE0 + NMGL_MAX_TEX_UNITS, v);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	cntxt->error = NMGL_NO_ERROR;
	nmglMultiTexCoord2fv (NMGL_TEXTURE0 - 1, v);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	return 0;
}

int nmglMultiTexCoord2fv_nullPointer_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	NMGLfloat v[2] = {0.0f, 0.0f};

	cntxt->error = NMGL_NO_ERROR;

	nmglMultiTexCoord2fv (NMGL_TEXTURE0, v);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	nmglMultiTexCoord2fv (NMGL_TEXTURE0, NULL);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);

	return 0;
}

int nmglMultiTexCoord2fv_setCoords_setContextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	int i = 0;
	unsigned int texture0_index = 0;
	NMGLfloat v[2] = {0.0f, 0.0f};
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
		v[0] = etalon_values[i].s;
		v[1] = etalon_values[i].t;
		nmglMultiTexCoord2fv (NMGL_TEXTURE0, v);
		TEST_ASSERT((cntxt->texState.curTexCoords[texture0_index].s == etalon_values[i].s) &&
				    (cntxt->texState.curTexCoords[texture0_index].t == etalon_values[i].t))
	}

	return 0;	
}
