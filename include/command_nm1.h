#ifndef COMMAND_NM1_H
#define COMMAND_NM1_H

#include "demo3d_nm1.h"
#include "nmsynchro.h"


NM_Command *NMGL_ReadNM_Command();

typedef void(*NMGL_Command_nm1)(NMGL_Context_NM1 *context, NM_Command *command);

void NMGL_DrawTriangles(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_DrawLines(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_DrawPoints(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_Clear(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_CopySegmentFromImage(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_CopySegmentToImage(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_PointSize(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_SetColor(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_SetDepth(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_Depth(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_DepthMask(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_DepthFunc(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_SwapBuffer(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_Exit(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_SetActiveTexture(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_SetMipmapLvlPointer(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_BindActiveTexObject(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_SetWhf(NMGL_Context_NM1 *context, NM_Command *command);
//void NMGL_SetColorPalette(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_SetTexEnvColor(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_SetTexEnvMode(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_SetTexParami(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_Texture2d(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_Finish(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_UseSharedPalette(NMGL_Context_NM1 *context, NM_Command *command);
void NMGL_UseLocalPalette(NMGL_Context_NM1 *context, NM_Command *command);


#endif //COMMAND_NM1_H 