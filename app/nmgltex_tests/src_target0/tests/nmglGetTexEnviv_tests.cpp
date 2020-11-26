#include "demo3d_nm0.h"
#include "nmgldef.h"
#include "nmgltype.h"
#include "nmgl.h"

#include "nmgltex_common.h"
#include "tests.h"

#include <stdio.h>
#include <assert.h>


int nmglGetTexEnviv_wrongEnv_isError();
int nmglGetTexEnviv_wrongPname_isError();
int nmglGetTexEnviv_getParams_retCorrect();

int run_nmglGetTexEnviv_test()
{
	printf ("\nStart nmglGetTexEnviv tests\n\n");

	RUN_TEST(nmglGetTexEnviv_wrongEnv_isError);
	RUN_TEST(nmglGetTexEnviv_wrongPname_isError);
	RUN_TEST(nmglGetTexEnviv_getParams_retCorrect);
	
	printf ("\nEnd nmglGetTexEnviv tests\n");
	return 0;
}

int nmglGetTexEnviv_wrongEnv_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
    NMGLenum env;
    NMGLenum pname;
    NMGLint params[1] = {0};    
    
    cntxt->error = NMGL_NO_ERROR;
    env = NMGL_TEXTURE_ENV;
    pname = NMGL_TEXTURE_ENV_MODE;

    nmglGetTexEnviv (env, pname, params);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

    env = NMGL_TEXTURE_ENV + 1;
    nmglGetTexEnviv (env, pname, params);
    TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

    return 0;
}

int nmglGetTexEnviv_wrongPname_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
    NMGLenum env;
    NMGLenum pname;
    NMGLint params[1] = {0};

    cntxt->error = NMGL_NO_ERROR;
    env = NMGL_TEXTURE_ENV;
    pname = NMGL_TEXTURE_ENV_MODE;
    
    nmglGetTexEnviv (env, pname, params);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

    pname = NMGL_TEXTURE_ENV_MODE + 1;
    nmglGetTexEnviv (env, pname, params);
    TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);    

    return 0;        
}

int nmglGetTexEnviv_getParams_retCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
    const int nparams = 5;

    NMGLenum env;
    NMGLenum pname;
    NMGLint params[1] = {0};

    NMGLint expectedParams[nparams] = {NMGL_MODULATE, NMGL_REPLACE, NMGL_DECAL, NMGL_BLEND, NMGL_ADD};

    cntxt->error = NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit = NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex = 0;

    env = NMGL_TEXTURE_ENV;
    pname = NMGL_TEXTURE_ENV_MODE;
    for (int i = 0; i < nparams; i ++)
    {        
        cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName = expectedParams[i];
        nmglGetTexEnviv (env, pname, params);
        TEST_ASSERT(params[0] == expectedParams[i]);
    }    

    return 0;
}
