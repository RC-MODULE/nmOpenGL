#ifndef __NMGL_H__
#define __NMGL_H__

#include "nmgltype.h"

#include "nmgldef.h"

#include "demo3d_common.h"
#define ActiveTexObject cntxt.texUnits[cntxt.activeTexUnitIndex].boundTexObject

void nmglActiveTexture(NMGLenum texture);
void nmglBegin(NMGLenum mode); 
void nmglClear(NMGLbitfield mask);
void nmglClearColor(NMGLclampf red, NMGLclampf green, NMGLclampf blue, NMGLclampf alpha);
void nmglClearDepthf(NMGLclampf depth);
void nmglColorPointer(NMGLint size, NMGLenum type, NMGLsizei stride, const void *pointer);
void nmglColor4f(NMGLfloat red, NMGLfloat green, NMGLfloat blue, NMGLfloat alpha);
void nmglColor4fv(NMGLfloat *v);
void nmglColor4ub(NMGLubyte red, NMGLubyte green, NMGLubyte blue, NMGLubyte alpha);
void nmglColor4ubv(NMGLubyte *v);
void nmglCullFace(NMGLenum mode);
void nmglDepthFunc(NMGLenum func);
void nmglDepthMask(NMGLboolean flag);
void nmglDepthRangef(NMGLclampf near, NMGLclampf far);
void nmglDisable(NMGLenum cap);
void nmglDisableClientState(NMGLenum array);
void nmglDrawArrays(NMGLenum mode, NMGLint first, NMGLsizei count);
void nmglEnable(NMGLenum cap);
void nmglEnableClientState(NMGLenum array);
void nmglEnd();
void nmglFinish();
void nmglFlush();
void nmglFrontFace(NMGLenum dir);
void nmglFrustumf(NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar);
NMGLenum nmglGetError();
void nmglGetIntegerv(NMGLenum pname, NMGLint * params);
void nmglGetFloatv(NMGLenum pname, NMGLfloat * params);
void nmglLightf(NMGLenum color, NMGLenum pname, NMGLfloat param);
void nmglLightfv(NMGLenum color, NMGLenum pname, const NMGLfloat *params);
void nmglLoadIdentity();
void nmglLoadMatrixf(const NMGLfloat *m);
void nmglMaterialf(NMGLenum face, NMGLenum pname, NMGLfloat params);
void nmglMaterialfv(NMGLenum face, NMGLenum pname, const NMGLfloat *params);
void nmglMatrixMode(NMGLenum mode);
void nmglMultMatrixf(const NMGLfloat * m);
void nmglNormalPointer(NMGLenum type, NMGLsizei stride, const void *pointer);
void nmglNormalPointerNM(NMGLenum type, NMGLsizei stride, const void *pointer);
void nmglNormal3f(NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglNormal3fv(const NMGLfloat *v);
void nmglOrthof(NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar);
void nmglPopMatrix();
void nmglPushMatrix();
void nmglRotatef(NMGLfloat angle, NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglScalef(NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglTranslatef(NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglVertex2f(NMGLfloat x, NMGLfloat y);
void nmglVertex2fv(const NMGLfloat *v);
void nmglVertex3f(NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglVertex3fv(const NMGLfloat *v);
void nmglVertexPointer(NMGLint size, NMGLenum type, NMGLsizei stride, const void *pointer);
void nmglViewport(NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height);

void nmglTexImage2D(NMGLenum target, NMGLint level, NMGLint internalformat, NMGLsizei width, NMGLsizei height, NMGLint border, NMGLenum format, NMGLenum type, const void *pixels );
void nmglTexSubImage2D (NMGLenum target, NMGLint level, NMGLint xoffset, NMGLint yoffset, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const void *pixels);
void nmglTexCoordPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const void *pointer);

#endif
