#ifndef __NMGL_H__
#define __NMGL_H__

#include "nmgltype.h"

#include "nmgldef.h"

#include "stdio.h"
#include "stdlib.h"

#define GLEW_STATIC
#define __OPEN_GL__
#include <GL\glew.h>

#include <GLFW\glfw3.h>

#define SECTION(a) 

#define setHeap(a)


struct Models {
	char* sphere = "../../models/sphere_31.stl";
	char* test = "../../models/test.stl";
	char* cube = "../../models/small_cube.stl";
	char* two_sphere = "../../models/2_sphere.stl";
	char* rcm = "../../models/out_RCM_32.stl";
	char* nm = "../../models/NM_32.stl";
	char* snowman = "../../models/snow_white_32.stl";
};

extern "C" {
	int get_amm_poligone(FILE *fp);
	void createArrayVec4(
		FILE *fmodel,
		float* verticesVec4,
		float* normalVec4,
		float scale
	);
	void cnvTrianglesToLines(float* srcVertices, float* dstVertices, int size);
}; 


#define nmglActiveTexture                              glActiveTexture 
#define nmglAlphaFunc                                  glAlphaFunc 
#define nmglBegin                                      glBegin
#define nmglBindTexture                                glBindTexture 
#define nmglBitmap                                     glBitmap 
#define nmglBlendFunc                                  glBlendFunc 
#define nmglCallLists                                  glCallLists 
#define nmglClear                                      glClear 
#define nmglClearColor                                 glClearColor 
#define nmglClearDepthf                                glClearDepthf 
#define nmglClearStencil                               glClearStencil 
#define nmglClientActiveTexture                        glClientActiveTexture 
#define nmglColor4f                                    glColor4f 
#define nmglColor4fv                                   glColor4fv 
#define nmglColor4ub                                   glColor4ub 
#define nmglColorMask                                  glColorMask 
#define nmglColorPointer                               glColorPointer 
#define nmglColorSubTableEXT                           glColorSubTableEXT 
#define nmglColorTableEXT                              glColorTableEXT 
#define nmglCopyPixels                                 glCopyPixels 
#define nmglCullFace                                   glCullFace 
#define nmglDepthFunc                                  glDepthFunc 
#define nmglDepthMask                                  glDepthMask 
#define nmglDepthRangef                                glDepthRangef 
#define nmglDisable                                    glDisable 
#define nmglDisableClientState                         glDisableClientState 
#define nmglDrawArrays                                 glDrawArrays 
#define nmglDrawElements                               glDrawElements 
#define nmglDrawPixels                                 glDrawPixels 
#define nmglEnable                                     glEnable 
#define nmglEnableClientState                          glEnableClientState 
#define nmglEnd                                        glEnd 
#define nmglEndList                                    glEndList 
#define nmglFinish                                     glFinish 
#define nmglFlush                                      glFlush 
#define nmglFrontFace                                  glFrontFace 
#define nmglFrustumf                                   glFrustum
#define nmglGenLists                                   glGenLists 
#define nmglGenTextures                                glGenTextures 
#define nmglGetError                                   glGetError 
#define nmglGetBooleanv                                glGetBooleanv 
#define nmglGetColorTableEXT                           glGetColorTableEXT 
#define nmglGetColorTableParameterivEXT                glGetColorTableParameterivEXT 
#define nmglGetFloatv                                  glGetFloatv 
#define nmglGetIntegerv                                glGetIntegerv 
#define nmglGetLightfv                                 glGetLightfv 
#define nmglGetMaterialfv                              glGetMaterialfv 
#define nmglGetPointerv                                glGetPointerv 
#define nmglGetPolygonStipple                          glGetPolygonStipple 
#define nmglGetTexEnvfv                                glGetTexEnvfv 
#define nmglGetTexEnviv                                glGetTexEnviv 
#define nmglGetTexParameteriv                          glGetTexParameteriv 
#define nmglGetString                                  glGetString 
#define nmglHint                                       glHint 
#define nmglIsEnabled                                  glIsEnabled 
#define nmglLightfv                                    glLightfv 
#define nmglLightModelfv                               glLightModelfv 
#define nmglLineStipple                                glLineStipple 
#define nmglLineWidth                                  glLineWidth 
#define nmglListBase                                   glListBase 
#define nmglLoadIdentity                               glLoadIdentity 
#define nmglLoadMatrixf                                glLoadMatrixf 
#define nmglMaterialf                                  glMaterialf 
#define nmglMaterialfv                                 glMaterialfv 
#define nmglMatrixMode                                 glMatrixMode 
#define nmglMultMatrixf                                glMultMatrixf 
#define nmglMultiTexCoord2f                            glMultiTexCoord2f 
#define nmglMultiTexCoord2fv                           glMultiTexCoord2fv 
#define nmglNewList                                    glNewList 
#define nmglNormal3f                                   glNormal3f 
#define nmglNormal3fv                                  glNormal3fv 
#define nmglNormalPointer                              glNormalPointer 
#define nmglNormalPointerNM                            glNormalPointerNM 
#define nmglOrthof                                     glOrtho
#define nmglPixelStorei                                glPixelStorei 
#define nmglPointSize                                  glPointSize 
#define nmglPolygonOffset                              glPolygonOffset 
#define nmglPolygonStipple                             glPolygonStipple
#define nmglPopMatrix                                  glPopMatrix 
#define nmglPushMatrix                                 glPushMatrix 
#define nmglRasterPos3f                                glRasterPos3f 
#define nmglReadPixels                                 glReadPixels 
#define nmglRotatef                                    glRotatef 
#define nmglScalef                                     glScalef 
#define nmglScissor                                    glScissor 
#define nmglShadeModel                                 glShadeModel 
#define nmglStencilFunc                                glStencilFunc 
#define nmglStencilMask                                glStencilMask 
#define nmglStencilOp                                  glStencilOp 
#define nmglTexCoordPointer                            glTexCoordPointer 
#define nmglTexEnvfv                                   glTexEnvfv 
#define nmglTexEnvi                                    glTexEnvi 
#define nmglTexImage2D                                 glTexImage2D 
#define nmglTexParameteri                              glTexParameteri 
#define nmglTexSubImage2D                              glTexSubImage2D 
#define nmglTranslatef                                 glTranslatef 
#define nmglVertex2f                                   glVertex2f 
#define nmglVertex2fv                                  glVertex2fv 
#define nmglVertex3f                                   glVertex3f 
#define nmglVertex3fv                                  glVertex3fv 
#define nmglVertexPointer                              glVertexPointer 
#define nmglViewport                                   glViewport 

#endif
