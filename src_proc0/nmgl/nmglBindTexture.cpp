
#include "demo3d_nm0.h"
//#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")

//============================================================================================================
TexObject* texObjExist(NMGLuint texture,NMGL_Context_NM0 *cntxt);
TexObject* texObjCreate(NMGLenum target,NMGLuint texture, NMGL_Context_NM0 *cntxt);

void nmglBindTexture (NMGLenum target, NMGLuint texture);
//============================================================================================================

TexObject* texObjExist(NMGLuint texture,NMGL_Context_NM0 *cntxt)
{
    //check
    if((texture<1)||(texture>NMGL_MAX_TEX_OBJECTS)) return NULL;//Требуется только в данной реализации для обработки неправильных значений аргумента texture.При динамическом выделении памяти здесь должен быть поиск по структуре данных, хранящей имена созданных текстурных объектов  
    return &cntxt->texState.texObjects[texture-1];
}

TexObject* texObjCreate(NMGLenum target,NMGLuint texture,NMGL_Context_NM0 *cntxt)
{
    TexObject* cptr=NULL;
    if(texture>NMGL_MAX_TEX_OBJECTS) return NULL;
    return ( TexObject*) texObjExist(texture,cntxt);
}

void nmglBindTexture (NMGLenum target, NMGLuint texture)
{
	TexObject* txp=NULL;
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if(target != NMGL_TEXTURE_2D){cntxt->error=NMGL_INVALID_ENUM;return;}
	if(texture>cntxt->texState.lastTexName){cntxt->error=NMGL_INVALID_VALUE;return;}	
	txp=texObjCreate(target,texture,cntxt);//check for existence inside
	if(txp==NULL){cntxt->error=NMGL_OUT_OF_MEMORY;return ;}
	if(txp->target != target) {cntxt->error=NMGL_INVALID_OPERATION;return;}
	ActiveTexObjectP=txp;
}
