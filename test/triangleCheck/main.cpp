#include "VShell.h"
#define _USE_MATH_DEFINES
#include "math.h"

#define SCALE 20

#define CENTER_PIXEL_X 0.5
#define CENTER_PIXEL_Y 0.5

#define SIDE_PTRN 32
#define SIDE_PTRN 32

static void setPixel(int* dst, int x, int y, int color, int nID) {
	VS_Rectangle(nID, x * SCALE, y * SCALE, x * SCALE + SCALE, y * SCALE + SCALE, color, color);
	if(y >=0 && y < SIDE_PTRN &&
		x >= 0 && x < SIDE_PTRN)
		dst[y * SIDE_PTRN + x] = color;
}



static void drawLine(int* dst, int x1, int y1, int x2, int y2, int color) {
	int deltaX = abs(x2 - x1);
	int deltaY = abs(y2 - y1);
	int signX = x1 < x2 ? 1 : -1;
	int signY = y1 < y2 ? 1 : -1;

	if (deltaX > deltaY) {
		int deltaError = deltaY;
		int error = 0;
		while (x1 != x2) {
			setPixel(dst, x1, y1, color, 1);
			error += deltaError;
			if (error >= deltaX) {
				y1 += signY;
				error -= deltaX;
			}
			x1 += signX;
		}
	}
	else {
		int deltaError = deltaX;
		int error = 0;
		while (y1 != y2) {
			setPixel(dst, x1, y1, color, 1);
			error += deltaError;
			if (error >= deltaY) {
				x1 += signX;
				error -= deltaY;
			}
			y1 += signY;
		}
	}
}

static void fillSide(int* dst, int x1, int y1, int x2, int y2, int color, int topLeftSide, int nID) {
	float deltaY = (y2 - y1);
	float deltaX = (x2 - x1);
	float k = deltaY / deltaX;
	float b = y1 - k * x1;
	for (int y = 0; y < SIDE_PTRN; y++) {
		for (int x = 0; x < SIDE_PTRN; x++) {
			if (fabs(deltaX) > fabs(deltaY)) {
				float yRefp = k * ((float)x) + b;
				float yRefs = k * ((float)x) + b;
				if (topLeftSide == 0 && (yRefp <= y + 0.5) ||
					topLeftSide == 1 && (yRefs >= y + 0.5)) {
					setPixel(dst, x, y, color, nID);
				}
				else {
					setPixel(dst, x, y, 0, nID);
				}
			}
			else {
				float xRefp = ((float)(y - b)) / k;
				float xRefs = ((float)(y - b)) / k;
				if (deltaY / deltaX < 0 && topLeftSide == 0 && (xRefp <= x + 0.5) ||
					deltaY / deltaX < 1 && topLeftSide == 1 && (xRefp >= x + 0.5)) {
					setPixel(dst, x, y, color, nID);
				}
				else {
					setPixel(dst, x, y, 0, nID);
				}
			}
		}
	}
}

void lineRef(int nID, int x1, int y1, int x2, int y2, int color) {
	VS_Line(nID, 
		x1 * SCALE,// + SCALE / 2, 
		y1 * SCALE,// + SCALE / 2, 
		x2 * SCALE,// + SCALE / 2, 
		y2 * SCALE,// + SCALE / 2, 
		color);
}




void correctTriangle(int* dst, float xA, float yA, float xB, float yB, float xC, float yC, int color, int colorLine) {
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			float x0 = float(x) + 0.5;
			float y0 = float(y) + 0.5;
			float check0 = (xA - x0) * (yB - yA) - (xB - xA) * (yA - y0);
			float check1 = (xB - x0) * (yC - yB) - (xC - xB) * (yB - y0);
			float check2 = (xC - x0) * (yA - yC) - (xA - xC) * (yC - y0);
			float sign0 = (check0 < 0) ? -1 : 1;
			float sign1 = (check1 < 0) ? -1 : 1;
			float sign2 = (check2 < 0) ? -1 : 1;
			if (fabs(check0) < 0.05 || fabs(check1) < 0.05 || fabs(check2) < 0.05) {
				x0 = float(x) + 0.4;
				y0 = float(y) + 0.6;
				check0 = (xA - x0) * (yB - yA) - (xB - xA) * (yA - y0);
				check1 = (xB - x0) * (yC - yB) - (xC - xB) * (yB - y0);
				check2 = (xC - x0) * (yA - yC) - (xA - xC) * (yC - y0);
				sign0 = (check0 < 0) ? -1 : 1;
				sign1 = (check1 < 0) ? -1 : 1;
				sign2 = (check2 < 0) ? -1 : 1;
			}
			if (sign0 == sign1 == sign2) {
				setPixel(dst, x, y, color, 1);				
			}
			
		}
	}
	VS_LineF(1, xA * SCALE, yA * SCALE, xB * SCALE, yB * SCALE, colorLine);
	VS_LineF(1, xC * SCALE, yC * SCALE, xB * SCALE, yB * SCALE, colorLine);
	VS_LineF(1, xA * SCALE, yA * SCALE, xC * SCALE, yC * SCALE, colorLine);
}

int main()
{
	if (!VS_Init())
		return 0;
	int width = SIDE_PTRN * SCALE;
	int height = SIDE_PTRN * SCALE;
	VS_CreateImage("triangle", 1, width, height, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("triangle2", 2, width, height, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	//VS_OpRunForward();

	int dst[SIDE_PTRN * SIDE_PTRN];
	for (int i = 0; i < SIDE_PTRN * SIDE_PTRN; i++) {
		dst[i] = 0;
	}
	int x = SIDE_PTRN; 
	int y = 0;
	int counter = 1; 
	float angle = 0;
	int x0 = 0;
	int y0 = 0;
	while(VS_Run())	{
		//сетка
		int offsetX = CENTER_PIXEL_X * SCALE;
		int offsetY = CENTER_PIXEL_Y * SCALE;
		float arctg = atan2(4, 31);
		float h = arctg * 180 / M_PI;
		//fillSide(dst, 0, 0, 31, 4, VS_GRAY, 1, 1);
		//lineRef(1, 0, 0, 31, 4,VS_WHITE);
		fillSide(dst, 0, 0, 4, 31, VS_GRAY, 1, 1);
		lineRef(1, 0, 0, 4, 31,VS_WHITE);
		

		//сетка пикселей
		for (int i = 0; i <= SIDE_PTRN * SCALE; i += SCALE) {
			VS_Line(1, 0, i, width, i, VS_WHITE);
			VS_Line(1, i, 0, i, height, VS_WHITE);
			VS_Line(2, 0, i, width, i, VS_WHITE);
			VS_Line(2, i, 0, i, height, VS_WHITE);
		}		

		/*//сетка центров пикселей
		for (int i = 0; i <= SIDE_PTRN * SCALE; i += SCALE) {
			VS_LineEx(1, 0, i + offsetY, width, i + offsetY, VS_GRAY, VS_DOT);
			VS_LineEx(1, i + offsetX, 0, i + offsetX, height, VS_GRAY, VS_DOT);
		}*/
		//ромбы
		for (int y = 0; y < SIDE_PTRN * SCALE; y += SCALE) {
			for (int x = 0; x < SIDE_PTRN * SCALE; x += SCALE) {
				VS_LineEx(1, x, y + offsetY, x + offsetX, y, 0x40, VS_DOT);
				VS_LineEx(1, x + offsetX, y, x + 2 * offsetX, y + offsetY, 0x40, VS_DOT);
				VS_LineEx(1, x + 2 * offsetX, y + offsetY, x + offsetX, y + 2 * offsetY, 0x40, VS_DOT);
				VS_LineEx(1, x + offsetX, y + 2 * offsetY, x, y + offsetY, 0x40, VS_DOT);
				VS_LineEx(2, x, y + offsetY, x + offsetX, y, 0x40, VS_DOT);
				VS_LineEx(2, x + offsetX, y, x + 2 * offsetX, y + offsetY, 0x40, VS_DOT);
				VS_LineEx(2, x + 2 * offsetX, y + offsetY, x + offsetX, y + 2 * offsetY, 0x40, VS_DOT);
				VS_LineEx(2, x + offsetX, y + 2 * offsetY, x, y + offsetY, 0x40, VS_DOT);
			}
		}

		
		VS_Draw(VS_DRAW_ALL);
	}
	
	return 0;
};
