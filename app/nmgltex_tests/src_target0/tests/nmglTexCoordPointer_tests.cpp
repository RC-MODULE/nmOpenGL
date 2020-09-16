#include "demo3d_nm0.h"
#include "nmgldef.h"
#include "nmgltype.h"
#include "nmgl.h"

#include "nmgltex_common.h"
#include "tests.h"

#include <stdio.h>
#include <assert.h>

//Тестовые сценарии
int nmglTexCoordPointer_wrongSize_isError();
int nmglTexCoordPointer_wrongType_isError();
int nmglTexCoordPointer_wrongStride_isError();
int nmglTexCoordPointer_nullPointer_isError();
int nmglTexCoordPointer_setArray_setContextCorrect();

//Главная тестовая функция
int run_nmglTexCoordPointer_test()
{
	printf ("\nStart nmglTexCoordPointer tests\n\n");
	RUN_TEST(nmglTexCoordPointer_wrongSize_isError);
	RUN_TEST(nmglTexCoordPointer_wrongType_isError);
	RUN_TEST(nmglTexCoordPointer_wrongStride_isError);
	RUN_TEST(nmglTexCoordPointer_nullPointer_isError);
	RUN_TEST(nmglTexCoordPointer_setArray_setContextCorrect);
	printf ("\nEnd nmglTexCoordPointer tests\n");
	return 0;
}

int nmglTexCoordPointer_wrongSize_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	const NMGLfloat texCoordPointer[4] = {0.1f, 0.2f, 0.3f, 0.4f};

	NMGLint size = 2;
	NMGLenum type = NMGL_FLOAT;
	NMGLsizei stride = 0;

	cntxt->error = NMGL_NO_ERROR;
	nmglTexCoordPointer(size, type, stride, texCoordPointer);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	size = 3;
	nmglTexCoordPointer(size, type, stride, texCoordPointer);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);

	return 0;
}

int nmglTexCoordPointer_wrongType_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	NMGLfloat texCoordPointer[4] = {0.1f, 0.2f, 0.3f, 0.4f};

	NMGLint size = 2;
	NMGLenum type = NMGL_FLOAT;
	NMGLsizei stride = 0;

	cntxt->error = NMGL_NO_ERROR;
	nmglTexCoordPointer(size, type, stride, texCoordPointer);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	type = NMGL_UNSIGNED_BYTE;
	nmglTexCoordPointer(size, type, stride, texCoordPointer);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);

	return 0;
}

int nmglTexCoordPointer_wrongStride_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	NMGLfloat texCoordPointer[4] = {0.1f, 0.2f, 0.3f, 0.4f};

	NMGLint size = 2;
	NMGLenum type = NMGL_FLOAT;
	NMGLsizei stride = 0;

	cntxt->error = NMGL_NO_ERROR;
	nmglTexCoordPointer(size, type, stride, texCoordPointer);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	stride = 4;
	nmglTexCoordPointer(size, type, stride, texCoordPointer);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);

	return 0;
}

int nmglTexCoordPointer_nullPointer_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	NMGLfloat texCoordPointer[4] = {0.1f, 0.2f, 0.3f, 0.4f};

	NMGLint size = 2;
	NMGLenum type = NMGL_FLOAT;
	NMGLsizei stride = 0;

	cntxt->error = NMGL_NO_ERROR;
	nmglTexCoordPointer(size, type, stride, texCoordPointer);
	TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

	nmglTexCoordPointer(size, type, stride, NULL);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);

	return 0;
}

int nmglTexCoordPointer_setArray_setContextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	NMGLfloat pointer_1[4] = {0.1f, 0.2f, 0.3f, 0.4f};
	NMGLfloat pointer_2[4] = {-0.1f, -0.2f, 1.3f, 1.4f};

	NMGLint size = 2;
	NMGLenum type = NMGL_FLOAT;
	NMGLsizei stride = 0;

	cntxt->texState.clientActiveTexUnit = 0;
	cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnit].pointer = 0;

	nmglTexCoordPointer(size, type, stride, pointer_1);
	TEST_ASSERT(cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnit].size == size);
	TEST_ASSERT(cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnit].type == type);
	TEST_ASSERT(cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnit].stride == stride);
	TEST_ASSERT((const NMGLfloat*)cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnit].pointer == pointer_1);

	nmglTexCoordPointer(size, type, stride, pointer_2);
	TEST_ASSERT(cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnit].size == size);
	TEST_ASSERT(cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnit].type == type);
	TEST_ASSERT(cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnit].stride == stride);
	TEST_ASSERT((const NMGLfloat*)cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnit].pointer == pointer_2);

	return 0;	
}
