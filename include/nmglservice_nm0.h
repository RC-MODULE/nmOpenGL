#pragma once

#include "demo3d_nm0.h"
#include "nmsynchro.h"

void NMGL_SetValue(NM_Command &command);
NM_Command NMGL_GetValue(int mode);

void NMGL_PushSegment(Rectangle &rect, int segNo);
void NMGL_PopSegment(Rectangle &rect, int segNo);
void NMGL_PolygonsMoveNext(int mode, int segNo);
DataForNmpu1* NMGL_PolygonsCurrent(int mode, int segNo);
void NMGL_StartCalculateLight();

