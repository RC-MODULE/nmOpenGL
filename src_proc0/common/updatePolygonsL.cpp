#include "demo3d_nm0.h"
#include "nmpp.h"

#include "nmblas.h"

#define sort() 	nmppsMerge_32f(lines->x0, lines->y0, temp0, count);	 \
	nmppsMerge_32f(lines->x1, lines->y1, temp1, count);				 \
	sortByY2(temp0, temp1, count);									 \
	split_v2nm32f((v2nm32f*)temp0, 1, lines->x0, lines->y0, count);	 \
	split_v2nm32f((v2nm32f*)temp1, 1, lines->x1, lines->y1, count);	 

SECTION(".text_demo3d")
void updatePolygonsL(Polygons* poly, Lines* lines, int count, int segX, int segY){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	nmppsConvert_32f32s_rounding((nm32f*)lines->colors, poly->color, 0, 4 * count);

	for (int i = 0; i < count; i++) {
		float x0, y0, x1, y1;
		if (lines->y0[i] < lines->y1[i]) {
			x0 = lines->x0[i];
			y0 = lines->y0[i];
			x1 = lines->x1[i];
			y1 = lines->y1[i];
		}
		else {
			x0 = lines->x1[i];
			y0 = lines->y1[i];
			x1 = lines->x0[i];
			y1 = lines->y0[i];
		}
		int dx = (int)(x1 - x0 + 0.5f);
		int dy = (int)(y1 - y0 + 0.5f);
		poly->numbersPattrns01[i] = cntxt->patterns->table_dydx[dy * 2 * WIDTH_PTRN + dx + WIDTH_PTRN];
		poly->ptrnSizesOf32_01[i] = dy * WIDTH_PTRN / 16;

		int minX, minY;

		if (MIN(x0, x1) < cntxt->windowInfo.x0_f[segX]) {
			poly->offsetsX[i] = (int)(cntxt->windowInfo.x0_f[segX] - MIN(x0, x1));
			minX = 0;
		}
		else {
			poly->offsetsX[i] = 0;
			minX = MIN(x0, x1) - cntxt->windowInfo.x0_f[segX];
		}
		if (MAX(x0, x1) > cntxt->windowInfo.x1_f[segX]) {
			poly->widths[i] = (int)(MAX(x0, x1) - cntxt->windowInfo.x1_f[segX]);
		}
		else {
			poly->widths[i] = MAX(x0, x1) - MIN(x0, x1);
		}

		if (y0 < cntxt->windowInfo.y0_f[segY]) {
			poly->offsetsY[i] = (int)(cntxt->windowInfo.y0_f[segY] - y0);
			minY = 0;
		}
		else {
			poly->offsetsY[i] = 0;
			minY = y0 - cntxt->windowInfo.y0_f[segY];
		}
		if (y1 > cntxt->windowInfo.y1_f[segY]) {
			poly->heights[i] = (int)(y1 - cntxt->windowInfo.y1_f[segY]);
		}
		else {
			poly->heights[i] = y1 - y0;
		}
		
		poly->pointInImage[i] = minY * WIDTH_SEG + minX;
		poly->z[i] = lines->z[i];

	}

	poly->count += count;
}