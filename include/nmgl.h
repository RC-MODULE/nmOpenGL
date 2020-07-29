#ifndef __NMNMGL_H__
#define __NMNMGL_H__

#include "nmgltype.h"

#include "nmgldef.h"

#include "demo3d_common.h"
#define ActiveTexObject cntxt.texUnits[cntxt.activeTexUnitIndex].boundTexObject

void nmglActiveTexture (NMGLenum texture);
void nmglAlphaFunc (NMGLenum func, NMGLclampf ref);
void nmglBegin(NMGLenum mode);
void nmglBindTexture (NMGLenum target, NMGLuint texture);
void nmglBitmap (NMGLsizei width, NMGLsizei height, NMGLfloat xorig, NMGLfloat yorig, NMGLfloat xmove, NMGLfloat ymove, const NMGLubyte *bitmap);
void nmglBlendFunc (NMGLenum sfactor, NMGLenum dfactor);
void nmglCallLists (NMGLsizei n, NMGLenum type, const NMGLvoid *lists);
void nmglClear (NMGLbitfield mask);
void nmglClearColor (NMGLclampf red, NMGLclampf green, NMGLclampf blue, NMGLclampf alpha);
void nmglClearDepthf (NMGLclampf depth);
void nmglClearStencil (NMGLint s);
void nmglClientActiveTexture (NMGLenum texture);
void nmglColor4f (NMGLfloat red, NMGLfloat green, NMGLfloat blue, NMGLfloat alpha);
void nmglColor4fv (const NMGLfloat *v);
void nmglColor4ub (NMGLubyte red, NMGLubyte green, NMGLubyte blue, NMGLubyte alpha);
void nmglColorMask (NMGLboolean red, NMGLboolean green, NMGLboolean blue, NMGLboolean alpha);
void nmglColorPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);
void nmglColorSubTableEXT (NMGLenum target, NMGLsizei start, NMGLsizei count, NMGLenum format, NMGLenum type, const NMGLvoid *table);
void nmglColorTableEXT (NMGLenum target, NMGLenum internalformat, NMGLsizei width, NMGLenum format, NMGLenum type, const NMGLvoid *table);
void nmglCopyPixels (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height, NMGLenum type);
void nmglCullFace (NMGLenum mode);
void nmglDepthFunc (NMGLenum func);
void nmglDepthMask (NMGLboolean flag);
void nmglDepthRangef (NMGLclampf zNear, NMGLclampf zFar);
void nmglDisable (NMGLenum cap);
void nmglDisableClientState (NMGLenum array);
void nmglDrawArrays (NMGLenum mode, NMGLint first, NMGLsizei count);
void nmglDrawElements (NMGLenum mode, NMGLsizei count, NMGLenum type, const NMGLvoid *indices);
void nmglDrawPixels (NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const NMGLvoid *pixels);
void nmglEnable (NMGLenum cap);
void nmglEnableClientState (NMGLenum array);
void nmglEnd (void);
void nmglEndList (void);
void nmglFinish (void);
void nmglFlush (void);
void nmglFrontFace (NMGLenum mode);
void nmglFrustumf (NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar);
NMGLuint nmglGenLists (NMGLsizei range);
void nmglGenTextures (NMGLsizei n, NMGLuint *textures);
NMGLenum nmglGetError (void);
void nmglGetBooleanv (NMGLenum pname, NMGLboolean *params);
void nmglGetColorTableEXT (NMGLenum target, NMGLenum format, NMGLenum type, NMGLvoid *table);
void nmglGetColorTableParameterivEXT (NMGLenum target, NMGLenum pname, NMGLint *params);
void nmglGetFloatv (NMGLenum pname, NMGLfloat *params);
void nmglGetIntegerv (NMGLenum pname, NMGLint *params);
void nmglGetLightfv (NMGLenum light, NMGLenum pname, NMGLfloat *params);
void nmglGetMaterialfv (NMGLenum face, NMGLenum pname, NMGLfloat *params);
void nmglGetPointerv (NMGLenum pname, NMGLvoid * *params);
void nmglGetPolygonStipple (NMGLubyte *mask);
void nmglGetTexEnvfv (NMGLenum target, NMGLenum pname, NMGLfloat *params);
void nmglGetTexEnviv (NMGLenum target, NMGLenum pname, NMGLint *params);
void nmglGetTexParameteriv (NMGLenum target, NMGLenum pname, NMGLint *params);
const NMGLubyte * nmglGetString (NMGLenum name);
void nmglHint (NMGLenum target, NMGLenum mode);
NMGLboolean nmglIsEnabled (NMGLenum cap);
void nmglLightfv (NMGLenum light, NMGLenum pname, const NMGLfloat *params);
void nmglLightModelfv (NMGLenum pname, const NMGLfloat *params);
void nmglLineStipple (NMGLint factor, NMGLushort pattern);
void nmglLineWidth (NMGLfloat width);
void nmglListBase (NMGLuint base);
void nmglLoadIdentity (void);
void nmglLoadMatrixf (const NMGLfloat *m);
void nmglMaterialf (NMGLenum face, NMGLenum pname, NMGLfloat param);
void nmglMaterialfv (NMGLenum face, NMGLenum pname, const NMGLfloat *params);
void nmglMatrixMode (NMGLenum mode);
void nmglMultMatrixf (const NMGLfloat *m);
void nmglMultiTexCoord2f (NMGLenum target, NMGLfloat s, NMGLfloat t);
void nmglMultiTexCoord2fv (NMGLenum target, const NMGLfloat *v);
void nmglNewList (NMGLuint list, NMGLenum mode);
void nmglNormal3f (NMGLfloat nx, NMGLfloat ny, NMGLfloat nz);
void nmglNormal3fv (const NMGLfloat *v);
void nmglNormalPointer (NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);
void nmglNormalPointerNM (NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);
void nmglOrthof (NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar);
void nmglPixelStorei (NMGLenum pname, NMGLint param);
void nmglPointSize (NMGLfloat size);
void nmglPolygonOffset (NMGLfloat factor, NMGLfloat units);
void nmglPolygonStipple (const NMGLubyte *mask);
void nmglPopMatrix (void);
void nmglPushMatrix (void);
void nmglRasterPos3f (NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglReadPixels (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, NMGLvoid *pixels);
void nmglRotatef (NMGLfloat annmgle, NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglScalef (NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglScissor (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height);
void nmglShadeModel (NMGLenum mode);
void nmglStencilFunc (NMGLenum func, NMGLint ref, NMGLuint mask);
void nmglStencilMask (NMGLuint mask);
void nmglStencilOp (NMGLenum fail, NMGLenum zfail, NMGLenum zpass);
void nmglTexCoordPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);
void nmglTexEnvfv (NMGLenum target, NMGLenum pname, const NMGLfloat *params);
void nmglTexEnvi (NMGLenum target, NMGLenum pname, NMGLint param);
void nmglTexImage2D (NMGLenum target, NMGLint level, NMGLint internalformat, NMGLsizei width, NMGLsizei height, NMGLint border, NMGLenum format, NMGLenum type, const NMGLvoid *pixels);
void nmglTexParameteri (NMGLenum target, NMGLenum pname, NMGLint param);
void nmglTexSubImage2D (NMGLenum target, NMGLint level, NMGLint xoffset, NMGLint yoffset, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const NMGLvoid *pixels);
void nmglTranslatef (NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglVertex2f (NMGLfloat x, NMGLfloat y);
void nmglVertex2fv (const NMGLfloat *v);
void nmglVertex3f (NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglVertex3fv (const NMGLfloat *v);
void nmglVertexPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);
void nmglViewport (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height);

void nmglTexImage2D(NMGLenum target, NMGLint level, NMGLint internalformat, NMGLsizei width, NMGLsizei height, NMGLint border, NMGLenum format, NMGLenum type, const void *pixels );
void nmglTexSubImage2D (NMGLenum target, NMGLint level, NMGLint xoffset, NMGLint yoffset, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const void *pixels);
void nmglTexCoordPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const void *pointer);

#endif
