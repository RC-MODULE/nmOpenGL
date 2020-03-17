#ifndef __NMGL_H__
#define __NMGL_H__

#include "nmgltype.h"

#include "nmgldef.h"

#include "demo3d_common.h"

void nmglBegin(NMGLenum mode);
void nmglEnd();
void nmglVertex2f(NMGLfloat x, NMGLfloat y);
void nmglVertex2fv(const NMGLfloat *v);
void nmglVertex3f(NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglVertex3fv(const NMGLfloat *v);
void nmglNormal3f(NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglNormal3fv(const NMGLfloat *v);
void nmglColor4f(NMGLfloat red, NMGLfloat green, NMGLfloat blue, NMGLfloat alpha);
void nmglColor4ub(NMGLubyte red, NMGLubyte green, NMGLubyte blue, NMGLubyte alpha);
void nmglColor4fv(NMGLfloat *v);
void nmglColor4ub(NMGLubyte *v);
void nmglCullFace(NMGLenum mode);
void nmglRotatef(NMGLfloat angle, NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglScalef(NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglTranslatef(NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglOrthof (NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar);
//не использовать nmglFrustumf, так как не оформлено деление координат на W
void nmglFrustumf (NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar);
void nmglFinish ();
void nmglFrontFace (NMGLenum dir);
void nmglDepthRangef(NMGLclampf near, NMGLclampf far);
void nmglDepthFunc (NMGLenum func);
void nmglDepthMask (NMGLboolean flag);
void nmglClearColor(NMGLclampf red, NMGLclampf green, NMGLclampf blue, NMGLclampf alpha);
void nmglClearDepthf(NMGLclampf depth);
NMGLenum nmglGetError();
void nmglEnable(NMGLenum cap);
void nmglDisable(NMGLenum cap);
void nmglMatrixMode(NMGLenum mode);
void nmglPushMatrix();
void nmglPopMatrix();
void nmglLoadIdentity();
void nmglFlush();
void nmglViewport(NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height);
void nmglClear(NMGLbitfield mask);
void nmglLightf(NMGLenum color, NMGLenum pname, NMGLfloat param);
void nmglLightfv(NMGLenum color, NMGLenum pname, const NMGLfloat *params);
void nmglMaterialf(NMGLenum face, NMGLenum pname, NMGLfloat params);
void nmglMaterialfv(NMGLenum face, NMGLenum pname, const NMGLfloat *params);
void nmglEnableClientState(NMGLenum array);
void nmglDisableClientState(NMGLenum array);
void nmglVertexPointer(NMGLint size, NMGLenum type, NMGLsizei stride, const void *pointer);
void nmglNormalPointer(NMGLenum type, NMGLsizei stride, const void *pointer);
void nmglNormalPointerNM(NMGLenum type, NMGLsizei stride, const void *pointer);
void nmglColorPointer(NMGLint size, NMGLenum type, NMGLsizei stride, const void *pointer);
void nmglDrawArrays(NMGLenum mode, NMGLint first, NMGLsizei count);
void nmglGetFloatv(NMGLenum pname, NMGLfloat * params);
void nmglGetIntegerv(NMGLenum pname, NMGLint * params);
void nmglMultMatrixf(const NMGLfloat * m);

void nmglTexImage2D(NMGLenum target, NMGLint level, 
					NMGLint internalformat, NMGLsizei width, NMGLsizei height, 
					NMGLint border, NMGLenum format, NMGLenum type, const void *data );
void nmglActiveTexture (NMGLenum texture);
#endif
