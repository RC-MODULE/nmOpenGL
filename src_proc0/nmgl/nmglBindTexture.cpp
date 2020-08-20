
#include "demo3d_nm0.h"
//#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")

//============================================================================================================
//TexObject* texObjExist(NMGLuint texture,NMGL_Context_NM0 *cntxt);
TexObject* texObjCreate(NMGLenum target,NMGLuint texture, NMGL_Context_NM0 *cntxt);

void nmglBindTexture (NMGLenum target, NMGLuint texture);
//============================================================================================================
/*
TexObject* texObjExist(NMGLuint texture,NMGL_Context_NM0 *cntxt)
{
    //check
    if((texture<0)||(texture>=NMGL_MAX_TEX_OBJECTS)) return NULL;
    return &cntxt->texState.texObjects[texture];
}
*/

TexObject* texObjCreate(NMGLenum target,NMGLuint texture,NMGL_Context_NM0 *cntxt)
{    
   if(texture>=NMGL_MAX_TEX_OBJECTS) return NULL;
    return &cntxt->texState.texObjects[texture];
}

void nmglBindTexture (NMGLenum target, NMGLuint texture)
{
	TexObject* txp=NULL;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if(target != NMGL_TEXTURE_2D){cntxt->error=NMGL_INVALID_ENUM;return;}
	if(texture > cntxt->texState.lastTexName){cntxt->error=NMGL_INVALID_VALUE;return;}	
	txp=texObjCreate(target,texture,cntxt);//check for existence inside
//cntxt->synchro.writeInstr(1, NMC1_CREATE_TEX_OBJECT, (int)target,texture);

	if(txp==NULL){cntxt->error=NMGL_OUT_OF_MEMORY;return ;}
	if(txp->target != target) {cntxt->error=NMGL_INVALID_OPERATION;return;}
	ActiveTexObjectP=txp;
	DEBUG_PRINT(("NM0:Bind texture:%d pointer=%x\n",texture,txp));	
	cntxt->synchro.writeInstr(1, NMC1_BIND_ACTIVE_TEX_OBJECT, (int)target, (int)texture);

}
