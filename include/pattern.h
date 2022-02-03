#pragma once
#include "utility.h"

#define MAX_SIDE_POLYGON				32
#define HEIGHT_PTRN						MAX_SIDE_POLYGON
#define WIDTH_PTRN						MAX_SIDE_POLYGON
#define OFFSETS 						WIDTH_PTRN
//#define AMOUNT_ANGLES					(2*WIDTH_PTRN + 2*HEIGHT_PTRN)
#define AMOUNT_ANGLES					180
#define FILL_PATTERNS_AMOUNT			AMOUNT_ANGLES * OFFSETS * 2
#define LINE_PATTERNS_AMOUNT 			AMOUNT_ANGLES
#define POINT_PATTERNS_AMOUNT 			MAX_SIDE_POLYGON
#define WIDTH_TABLE						(2 * WIDTH_PTRN)
#define OFFSET_TABLE					(WIDTH_TABLE / 2)
#define HEIGHT_TABLE					(HEIGHT_PTRN + 1)	
#define SIZE_TABLE 						WIDTH_TABLE * HEIGHT_TABLE
#define NPATTERNS 						FILL_PATTERNS_AMOUNT
#define GET_TABLE_VALUE(table, x, y)	table[(y) * 2 * WIDTH_PTRN + (x) + WIDTH_PTRN]

/*!
 *  \brief Тип, характеризирующий один двуразрядный паттерн
 */
typedef int Pattern[WIDTH_PTRN * HEIGHT_PTRN / 16];

/*!
 *  \brief Массив разннобразных двуразрядных паттернов. Имеются паттерны линий, точек и закрашенных по разный стороны от линий. Так же здесь хранятся таблица обращения к линейке паттернов определенного угла
 */
struct PatternsArray {
	Pattern ptrns[FILL_PATTERNS_AMOUNT];
	Pattern linePtrns[LINE_PATTERNS_AMOUNT];
	Pattern pointPtrns[POINT_PATTERNS_AMOUNT];
	int fillTable[SIZE_TABLE];
	int lineTable[SIZE_TABLE];
};

struct PatternPack {
    Pattern* patterns;
	Pattern** ppPattern;	/// Указатель на список указателей на паттерны
    Vector2* origins;   /// x, y
    Size* sizes;     /// width, height
	int* imagePositions;
    int count;
};

/*!
 *  \brief Функция инициализации 8-битных паттернов закрашенных с разных сторон от разнообразных линий
 *  
 *  \param dst [out] Выходной массив 8-битных паттернов
 *  \param table_dydx [out] Выходная таблица паттернов
 *  \param color [in] Цвет закрашенной стороны (незакрашенная всегда будет 0)
 *  
 *  \details Details
 *  
 */
void fillPtrnsInit(unsigned int* dst, int* table_dydx, unsigned int color);

/*!
 *  \brief Функция инициализации 8-битных паттернов линий
 *  
 *  \param dst [out] Выходной массив 8-битных паттернов
 *  \param table_dydx [out] Выходная таблица паттернов
 *  \param color [in] Цвет линии
 *  
 *  \details Details
 *  
 */
void linePtrnsInit(unsigned int* dst, int* table, unsigned int  color);

/*!
 *  \brief Функция инициализации 8-битных паттернов точек
 *  
 *  \param dst [out] Выходной массив 8-битных паттернов
 *  \param table_dydx [out] Выходная таблица паттернов
 *  \param color [in] Цвет точки
 *  
 *  \details Details
 *  
 */
void pointPtrnsInit(unsigned int* dst, unsigned int color);

void create_tabl_dydx(unsigned int* srcPatterns, int* dydx, int width, int height);
void hostCreatePatterns(PatternsArray* patterns);
