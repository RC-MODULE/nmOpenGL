#pragma once
#include "nmgl.h"
#include "demo3d_common.h"
#include "lighting.h"
#include "framebuffer.h"

struct NMGL_Context {

	alignas(8) NMGL_FrameBuffer defaultFrameBuffer;

	int isUseTwoSidedMode;
	int isCullFace;
	int cullFaceType;
	int frontFaceOrientation;
	int normalizeEnabled;
	float pointSize;


	MatrixStack *currentMatrixStack;
	alignas(8) mat4nm32f modelviewMatrix[16];
	alignas(8) mat4nm32f projectionMatrix[2];
	alignas(8) mat4nm32f normalMatrix;
	alignas(8) MatrixStack modelviewMatrixStack;
	alignas(8) MatrixStack projectionMatrixStack;

	Array vertexArray; ///< Класс для работы со значением координат вершинам в nmglDrawArrays
	Array normalArray; ///< Класс для работы с нормалями в nmglDrawArrays
	Array colorArray;  ///< Класс для работы с цветом в nmglDrawArrays

	alignas(8) LightingInfo lightingInfo;

	NmglBeginEndInfo beginEndInfo;
	v4nm32f currentColor;
	v4nm32f currentNormal;

	NMGL_Viewport viewport;
	NMGL_ScissorTest scissorTest;
	NMGL_AlphaTest alpha_test;
	NMGL_StencilTest stencil_test;
	NMGL_DepthTest depth_test;
	Point_cntxt_t point;
	Line_cntxt_t line;
	Polygon_cntxt_t polygon;
	Blend_cntxt_t blend;

	int shadeModel;
};

void NMGL_ContextInit(NMGL_Context *context);
NMGL_Context *NMGL_GetCurrentContext();