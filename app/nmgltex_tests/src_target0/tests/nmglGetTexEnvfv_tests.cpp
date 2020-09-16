#include "demo3d_nm0.h"
#include "nmgldef.h"
#include "nmgltype.h"

#include "nmgltex_common.h"
#include "tests.h"

#include <stdio.h>
#include <assert.h>

int nmglGetTexEnvfv_wrongEnv_isError();
int nmglGetTexEnvfv_wrongPname_isError();
int nmglGetTexEnvfv_getParams_retCorrect();

int run_nmglGetTexEnvfv_test()
{
	printf ("\nStart nmglGetTexEnvfv tests\n\n");

	RUN_TEST(nmglGetTexEnvfv_wrongEnv_isError);
	RUN_TEST(nmglGetTexEnvfv_wrongPname_isError);
	RUN_TEST(nmglGetTexEnvfv_getParams_retCorrect);
	
	printf ("\nEnd nmglGetTexEnvfv tests\n");
	return 0;
}

int nmglGetTexEnvfv_wrongEnv_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
    NMGLenum env;
    NMGLenum pname;
    NMGLfloat params[4] = {0};    
    
    cntxt->error = NMGL_NO_ERROR;
    env = NMGL_TEXTURE_ENV;
    pname = NMGL_TEXTURE_ENV_COLOR;

    nmglGetTexEnvfv (env, pname, params);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

    env = NMGL_TEXTURE_ENV + 1;
    nmglGetTexEnvfv (env, pname, params);
    TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

    return 0;
}

int nmglGetTexEnvfv_wrongPname_isError()
{
	
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
    NMGLenum env;
    NMGLenum pname;
    NMGLfloat params[4] = {0};

    cntxt->error = NMGL_NO_ERROR;
    env = NMGL_TEXTURE_ENV;
    pname = NMGL_TEXTURE_ENV_COLOR;
    
    nmglGetTexEnvfv (env, pname, params);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

    pname = NMGL_TEXTURE_ENV_COLOR + 1;
    nmglGetTexEnvfv (env, pname, params);
    TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);    

    return 0;        
}

int nmglGetTexEnvfv_getParams_retCorrect()
{
	
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
    const int nparams = 2;

    NMGLenum env;
    NMGLenum pname;
    NMGLfloat params[4] = {0};

    NMGLfloat expectedParams[nparams][4] = {{0.3, 0.4, 0.5, 0.6}, {0.0f,1.0,0.0f,1.0}};

    cntxt->error = NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit = NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex = 0;

    env = NMGL_TEXTURE_ENV;
    pname = NMGL_TEXTURE_ENV_COLOR;
    for (int i = 0; i < nparams; i ++)
    {
        for (int j = 0; j < 4; j++)
        {
            cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[j] = expectedParams[i][j];
        }
        nmglGetTexEnvfv (env, pname, params);
        TEST_ARRAYS_EQUAL(params,expectedParams[i],4)
    }    

    return 0;
}
