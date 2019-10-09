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
};

#define nmglRotatef				glRotatef		
#define nmglScalef				glScalef		
#define nmglTranslatef			glTranslatef
#define nmglOrthof 				glOrtho	
#define nmglFrustumf 			glFrustum
#define nmglFinish 				glFinish 		
#define nmglFrontFace 			glFrontFace 
#define nmglCullFace 			glCullFace 
#define nmglDepthFunc 			glDepthFunc 
#define nmglDepthMask 			glDepthMask 
#define nmglDepthRange 			glDepthRange 
#define nmglClearColor			glClearColor
#define nmglClearDepthf			glClearDepthf
#define nmglGetError			glGetError
#define nmglEnable				glEnable				
#define nmglDisable				glDisable				
#define nmglMatrixMode			glMatrixMode			
#define nmglPushMatrix			glPushMatrix			
#define nmglPopMatrix			glPopMatrix			
#define nmglLoadIdentity		glLoadIdentity		
#define nmglViewport			glViewport			
#define nmglClear				glClear				
#define nmglLightf				glLightf				
#define nmglLightfv				glLightfv				
#define nmglMaterialf			glMaterialf			
#define nmglMaterialfv			glMaterialfv			
#define nmglEnableClientState	glEnableClientState	
#define nmglDisableClientState	glDisableClientState	
#define nmglVertexPointer		glVertexPointer		
#define nmglNormalPointer		glNormalPointer		
#define nmglColorPointer		glColorPointer		
#define nmglDrawArrays			glDrawArrays			

#endif
