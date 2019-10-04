#include "VShell.h"
#include "demo3d_common.h"

void drawLinePolygonsPC(int nID, Polygons *polygons, int color) {
	//ok = halReadMemBlock(&polygons, polygonsNM, sizeof32(Polygons), 1);
	/*for (int i = 0; i < polygons->count; i++) {
		VS_Line(nID, polygons->x0[i], polygons->y0[i],
			polygons->x0[i] + polygons->dx02[i], polygons->y0[i] + polygons->dy02[i], color);
		VS_Line(nID, polygons->x0[i], polygons->y0[i],
			polygons->x0[i] + polygons->dx01[i], polygons->y0[i] + polygons->dy01[i], color);
		VS_Line(nID, polygons->x0[i] + polygons->dx01[i], polygons->y0[i] + polygons->dy01[i],
			polygons->x0[i] + polygons->dx02[i], polygons->y0[i] + polygons->dy02[i], color);
	}*/
}

void drawMeshSegments(int* srcImg){
	int x = 0;
	int y = 0;
	while (x < WIDTH_IMAGE) {
		//VS_Line();
	}
}