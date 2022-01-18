#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"

template <typename T>
static T *getStaticObject()
{
#ifdef __GNUC__
	SECTION(".data_shmem0") static T obj;
	return &obj;
#else
	static T *obj = 0;
	if (obj == 0)
	{
		obj = (NMGL_ClearInfo_int *)halMalloc32(sizeof32(NMGL_ClearInfo_int));
	}
	return obj;
#endif
}

SECTION(".text_nmgl") void nmglClear(NMGLbitfield mask)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	NMGL_ClearInfo_int *clear_info = getStaticObject<NMGL_ClearInfo_int>();

	clear_info->color[0] = (int)(context->clear.color[0] * RED_COEFF);
	clear_info->color[1] = (int)(context->clear.color[1] * GREEN_COEFF);
	clear_info->color[2] = (int)(context->clear.color[2] * BLUE_COEFF);
	clear_info->color[3] = (int)(context->clear.color[3] * ALPHA_COEFF);

	clear_info->depth = (int)(context->clear.depth * ZBUFF_MAX);

	NM_Command command;
	command.instr = NMC1_CLEAR;
	command.params[0] = CommandArgument(mask);
	command.params[5] = CommandArgument(clear_info);
	if (context->scissorTest.isEnabled) {
		command.params[1] = CommandArgument(context->scissorTest.origin.x);
		command.params[2] = CommandArgument(context->scissorTest.origin.y);
		command.params[3] = CommandArgument(context->scissorTest.size.width);
		command.params[4] = CommandArgument(context->scissorTest.size.height);
	}
	else {
		command.params[1] = CommandArgument(0);
		command.params[2] = CommandArgument(0);
		command.params[3] = CommandArgument(context->defaultFrameBuffer.width);
		command.params[4] = CommandArgument(context->defaultFrameBuffer.height);
	}
	
	NMGL_SetValue(command);
}
