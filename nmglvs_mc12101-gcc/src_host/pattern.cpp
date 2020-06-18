#include "nmpp.h"
#include <cmath>
#include "demo3d_nm1.h"

#define PI 3.14159265359

enum FillMode {
	RIGHT,
	LEFT,
	LINE
};

struct Point {
	int x;
	int y;
};

struct Line {
	Point p0;
	Point p1;
};

void fill(nm8s* dst, int startX, int width, int y, int fillColor, FillMode fillMode) {
	int xst;
	switch (fillMode)
	{
	case RIGHT:
		xst = MAX(startX, 0);
		for (int xTmp = xst; xTmp<width; xTmp++)
			dst[y*width + xTmp] = fillColor;
		break;
	case LEFT:
		xst = MIN(startX + 1, width);
		for (int xTmp = 0; xTmp < xst; xTmp++)
			dst[y*width + xTmp] = fillColor;
		break;
	case LINE:
		xst = MAX(startX, 0);
		xst = MIN(xst, width);
		dst[y*width + xst] = fillColor;
		break;
	default:
		break;
	}
}

//Bresenham's line algorithm
void drawPattern(Line* line,
	nm8s* dst,
	int width,
	int height,	
	int fillColor,
	FillMode fillMode) 
{
	int deltaX = line->p1.x - line->p0.x;
	int deltaY = line->p1.y - line->p0.y;
	int dx = (deltaX > 0) ? 1 : -1;
	int dy = (deltaY > 0) ? 1 : -1;
	double k, err;
	if (abs(deltaX) >= abs(deltaY)) {
		k = 2 * (double)ABS(deltaY) / ABS(deltaX);
		err = 0;
		int y = line->p0.y;
		for (int x = line->p0.x; x != line->p1.x; x += dx) {
			if (err > 1) {
				y += dy;
				err -= 2;
			}
			err += k;
			fill(dst, x, width, y, fillColor, fillMode);
		}
	}
	else {
		k = 2 * (double)ABS(deltaX) / ABS(deltaY);
		err = 0;
		int x = line->p0.x;
		for (int y = line->p0.y; y != line->p1.y; y += dy) {
			if (err > 1) {
				x += dx;
				err -= 2;
			}
			err += k;
			fill(dst, x, width, y, fillColor, fillMode);
		}
	}
}

void fillPattern(nm8s* pDstSource,int width, int height)
{
	int x=0;
	int y=0;
	int size = width*height;		//size of one pattern
	int nOffSets_X = OFFSETS;
	int color = 1;
	int cnt = 0;
	Line line;
	//Point point;
	FillMode mode[2] = { RIGHT, LEFT };
		 
	for (int m = 0; m < 2; m++) {
		cnt = m * NPATTERNS / 2;
		//step on the left side (180..135 degrees)
		for (y = 0; y < height; y++)
		{
			for (int i = 0; i < OFFSETS; i++, cnt++)
			{
				nm8s* dsti = nmppsAddr_8s(pDstSource, cnt * size);
				line.p0.x = i;
				line.p0.y = 0;
				line.p1.x = i - width;
				line.p1.y = y;
				drawPattern(&line, (nm8s*)dsti, width, height, color, mode[m]);
				for (int i = y * width; i < size; i++) {
					dsti[i] = color;
				}
			}
		}
		//step on the up side (135..90 degrees)
		for (x = width - 1; x >= 0; x--)
		{
			for (int i = 0; i < OFFSETS; i++, cnt++)
			{
				nm8s* dsti = nmppsAddr_8s(pDstSource, cnt * size);
				line.p0.x = i;
				line.p0.y = 0;
				line.p1.x = i - x;
				line.p1.y = height;
				drawPattern(&line, (nm8s*)dsti, width, height, color, mode[m]);
			}
		}
		//(90..45 degrees)
		for (x = 0; x < width; x++)
		{
			for (int i = 0; i < OFFSETS; i++, cnt++)
			{
				nm8s* dsti = nmppsAddr_8s(pDstSource, cnt * size);
				line.p0.x = i;
				line.p0.y = 0;
				line.p1.x = x + i;
				line.p1.y = height;
				drawPattern(&line, (nm8s*)dsti, width, height, color, mode[m]);
			}
		}
		//step on the right side (45..0 degrees)
		for (y = height - 1; y >=0 ; y--)
		{
			for (int i = 0; i < OFFSETS; i++, cnt++)
			{
				nm8s* dsti = nmppsAddr_8s(pDstSource, cnt * size);
				line.p0.x = i;
				line.p0.y = 0;
				line.p1.x = x + i;
				line.p1.y = y;
				drawPattern(&line, (nm8s*)dsti, width, height, color, mode[m]);
			}
		}
		//------------------------------------
	}

}