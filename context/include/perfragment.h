#include "nmgltype.h"
#ifndef __PERFRAGMENT_H__ 
#define __PERFRAGMENT_H__ 

typedef struct{
	NMGLboolean enabled;
	NMGLenum func;
} NMGL_DepthTest;

typedef struct {
	NMGLboolean enabled;
	NMGLenum func;
	NMGLint ref;
	NMGLuint mask;
} NMGL_StencilTest;

typedef struct{
	NMGLboolean enabled;
	NMGLenum func;
	NMGLclampf ref;
} NMGL_AlphaTest;

typedef struct{
	//ImageSegments segments;
	Vector2 origin;
	Size size;
	int isEnabled;
	int dummy;
}  NMGL_ScissorTest ;

#endif //__PERFRAGMENT_H__ 
