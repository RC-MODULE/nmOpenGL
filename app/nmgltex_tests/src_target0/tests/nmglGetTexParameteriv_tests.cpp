#include "demo3d_nm0.h"
#include "nmgldef.h"
#include "nmgltype.h"
#include "nmgl.h"

#include "nmgltex_common.h"
#include "tests.h"

#include <stdio.h>
#include <assert.h>

int nmglGetTexParameteriv_wrongTarget_isError();
int nmglGetTexParameteriv_wrongPname_isError();
int nmglGetTexParameteriv_getParam_retCorrect();


int run_nmglGetTexParameteriv_test()
{
	printf ("\nStart nmglGetTexParameteriv tests\n\n");
	RUN_TEST(nmglGetTexParameteriv_wrongTarget_isError);
	RUN_TEST(nmglGetTexParameteriv_wrongPname_isError);
	RUN_TEST(nmglGetTexParameteriv_getParam_retCorrect);
	printf ("\nEnd nmglGetTexParameteriv tests\n");

	return 0;
}

int nmglGetTexParameteriv_wrongTarget_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	NMGLenum target; 
	NMGLenum pname = NMGL_TEXTURE_MAG_FILTER; 
	NMGLint params[1] = {0};

	cntxt->error = NMGL_NO_ERROR;
	target = NMGL_TEXTURE_2D;

	nmglGetTexParameteriv (target, pname, params);	
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	target = NMGL_TEXTURE_2D + 1;

	nmglGetTexParameteriv (target, pname, params);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);



	return 0;
}

int nmglGetTexParameteriv_wrongPname_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	NMGLenum target; 
	NMGLenum pname[] = {NMGL_TEXTURE_MAG_FILTER, NMGL_TEXTURE_MIN_FILTER, NMGL_TEXTURE_WRAP_S, NMGL_TEXTURE_WRAP_T}; 
	NMGLint params[1] = {0};

	cntxt->error = NMGL_NO_ERROR;
	target = NMGL_TEXTURE_2D;

	for (int i = 0; i < 4; i ++)
	{
		nmglGetTexParameteriv (target, pname[i], params);
		TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
	}

	nmglGetTexParameteriv (target, pname[0]+10, params);

	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	return 0;
}

int nmglGetTexParameteriv_getParam_retCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	NMGLenum target; 	
	NMGLint params[1] = {0};
	NMGLenum pname[] = {NMGL_TEXTURE_MAG_FILTER, NMGL_TEXTURE_MIN_FILTER, NMGL_TEXTURE_WRAP_S, NMGL_TEXTURE_WRAP_T};
	NMGLint expectedParamsDefault[] = {NMGL_LINEAR, NMGL_NEAREST_MIPMAP_LINEAR, NMGL_REPEAT, NMGL_REPEAT}; 
	NMGLint expectedParams[] = {NMGL_NEAREST, NMGL_NEAREST, NMGL_CLAMP_TO_EDGE, NMGL_CLAMP_TO_EDGE}; 
	TexObject a,b;

	a.name=1;
    a.target=NMGL_TEXTURE_2D;

	a.texMagFilter=expectedParamsDefault[0];
	a.texMinFilter=expectedParamsDefault[1];
    a.texWrapS=expectedParamsDefault[2];
    a.texWrapT=expectedParamsDefault[3];
        
    a.imageIsSet=0;

	b.name=1;
    b.target=NMGL_TEXTURE_2D;

	b.texMagFilter=expectedParams[0];
	b.texMinFilter=expectedParams[1];
    b.texWrapS=expectedParams[2];
    b.texWrapT=expectedParams[3];
        
    b.imageIsSet=0;


	cntxt->texState.activeTexUnit = NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex = 0;
	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject = &a;

	cntxt->error = NMGL_NO_ERROR;
	target = NMGL_TEXTURE_2D;

	for (int i = 0; i < 4; i++)
	{
		nmglGetTexParameteriv (target, pname[i], params);
		TEST_ASSERT(params[0] == expectedParamsDefault[i]);		
	}

	cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject = &b;

	for (int i = 0; i < 4; i++)
	{
		nmglGetTexParameteriv (target, pname[i], params);
		TEST_ASSERT(params[0] == expectedParams[i]);		
	}
	
	return 0;
}
