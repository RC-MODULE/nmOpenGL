#include "nmgltype.h"

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