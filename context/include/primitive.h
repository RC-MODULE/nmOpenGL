#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

/*!
 *  \brief Структура, хранящая указатели на геометрические координаты, текстурные координаты и цвет вершины
 *  \author Жиленков Иван
 */
struct CombinePointers {
	float *x;
	float *y;
	float *z;
	float *w;
	// TEXTURING PART
	float *s;
	float *t;
	// TEXTURING PART
	v4nm32f* color;
	int dummy;
};


/*!
 *  \brief Структура, хранящая три вершины треугольника. Каждая вершина описывается структурой CombinePointers
 *  \author Жиленков Иван
 */
struct TrianglePointers {
	CombinePointers v0;
	CombinePointers v1;
	CombinePointers v2;
};

/*!
 *  \brief Структура, хранящая две вершины линии. Каждая вершина описывается структурой CombinePointers
 *  \author Жиленков Иван
 */
struct LinePointers {
	CombinePointers v0;
	CombinePointers v1;
};

/*!
 *  \ingroup service_api
 *  \defgroup repackToPrimitives_t repackToPrimitives_t
 *  \brief Функции переупаковки вершин TRIANGLES
 *  \author Жиленков Иван
 *
 *  \param srcVertex [in] Входной массив вершин
 *  \param srcColor [in] Входной массив цветов 
 *  \param srcTex [in] Входной массив текстурных координат 
 *  \param dstVertex [out] Выходной массив треугольников
 *  \param vertexAmount [in] Количество вершин во входных массивах для обработки 
 *  \return Возвращает количество треугольников в выходном массиве
 *  \details Details
 *  \par
 *  \xmlonly
 *  	<testperf>
 *   		<param name=" srcVertex "> im10 </param>
 *   		<param name=" srcColor ">  im20 </param>
 *   		<param name=" srcTex ">    im30 </param>
 *   		<param name=" dstVertex "> im40 </param>
 *   		<param name=" vertexAmount "> 48 96 192 384 </param>
 *			<init>
 *				initDstTrianglePointers(dstVertex, vertexAmount);
 *			</init>
 *			<size> vertexAmount </size>
 *  	</testperf>
 *  	<testperf>
 *   		<param name=" srcVertex "> im00 im10 im20 im30 im40 im50 im60 im70 </param>
 *   		<param name=" srcColor ">  im01 im11 im21 im31 im41 im51 im61 im71 </param>
 *   		<param name=" srcTex ">    im02 im12 im22 im32 im42 im52 im62 im72 </param>
 *			<param name=" dstVertex "> im03 im13 im23 im33 im43 im53 im63 im73 </param>
 *   		<param name=" vertexAmount "> 48 96 </param>
 *			<init>
 *				initDstTrianglePointers(dstVertex, vertexAmount);
 *			</init>
 *			<size> vertexAmount </size>
 *  	</testperf>
 *  \endxmlonly
 */
 //! \{
extern "C" int repackToPrimitives_t(const v4nm32f *srcVertex,
	const v4nm32f *srcColor,
	const v2nm32f *srcTex,
	TrianglePointers *dstVertex,
	int vertexAmount);
 //! \}

/*!
 *  \ingroup service_api
 *  \defgroup repackToPrimitives_ts repackToPrimitives_ts
 *  \brief Функции переупаковки вершин TRIANGLE_STRIP
 *
 *  \param srcVertex [in] Входной массив вершин
 *  \param srcColor [in] Входной массив цветов 
 *  \param srcTex [in] Входной массив текстурных координат 
 *  \param dstVertex [out] Выходной массив треугольников
 *  \param vertexAmount [in] Количество вершин во входных массивах для обработки 
 *  \return Возвращает количество треугольников в выходном массиве
 *  \details Details
 *  \par
 *  \xmlonly
 *  	<testperf>
 *   		<param name=" srcVertex "> im10 </param>
 *   		<param name=" srcColor ">  im20 </param>
 *   		<param name=" srcTex ">    im30 </param>
 *   		<param name=" dstVertex "> im40 </param>
 *   		<param name=" vertexAmount "> 34 66 136 264 </param>
 *			<init>
 *				initDstTrianglePointers(dstVertex, vertexAmount);
 *			</init>
 *			<size> vertexAmount </size>
 *  	</testperf>
 *  	<testperf>
 *   		<param name=" srcVertex "> im00 im10 im20 im30 im40 im50 im60 im70 </param>
 *   		<param name=" srcColor ">  im01 im11 im21 im31 im41 im51 im61 im71 </param>
 *   		<param name=" srcTex ">    im02 im12 im22 im32 im42 im52 im62 im72 </param>
 *			<param name=" dstVertex "> im03 im13 im23 im33 im43 im53 im63 im73 </param>
 *   		<param name=" vertexAmount "> 34 66 </param>
 *			<init>
 *				initDstTrianglePointers(dstVertex, vertexAmount);
 *			</init>
 *			<size> vertexAmount </size>
 *  	</testperf>
 *  \endxmlonly
 */
 //! \{
extern "C" int repackToPrimitives_ts(const v4nm32f *srcVertex,
	const v4nm32f *srcColor,
	const v2nm32f *srcTex,
	TrianglePointers *dstVertex,
	int vertexAmount);
 //! \}

/*!
 *  \ingroup service_api
 *  \defgroup repackToPrimitives_tf repackToPrimitives_tf
 *  \brief Функции переупаковки вершин треугольников TRIANGLE_FAN
 *
 *  \param srcVertex [in] Входной массив вершин
 *  \param srcColor [in] Входной массив цветов 
 *  \param srcTex [in] Входной массив текстурных координат 
 *  \param dstVertex [out] Выходной массив треугольников
 *  \param vertexAmount [in] Количество вершин во входных массивах для обработки 
 *  \return Возвращает количество треугольников в выходном массиве
 *  \details Details
 *  \par
 *  \xmlonly
 *  	<testperf>
 *   		<param name=" srcVertex "> im10 </param>
 *   		<param name=" srcColor ">  im20 </param>
 *   		<param name=" srcTex ">    im30 </param>
 *   		<param name=" dstVertex "> im40 </param>
 *   		<param name=" vertexAmount "> 34 66 136 264 </param>
 *			<init>
 *				initDstTrianglePointers(dstVertex, vertexAmount);
 *			</init>
 *			<size> vertexAmount </size>
 *  	</testperf>
 *  	<testperf>
 *   		<param name=" srcVertex "> im00 im10 im20 im30 im40 im50 im60 im70 </param>
 *   		<param name=" srcColor ">  im01 im11 im21 im31 im41 im51 im61 im71 </param>
 *   		<param name=" srcTex ">    im02 im12 im22 im32 im42 im52 im62 im72 </param>
 *			<param name=" dstVertex "> im03 im13 im23 im33 im43 im53 im63 im73 </param>
 *   		<param name=" vertexAmount "> 34 66 </param>
 *			<init>
 *				initDstTrianglePointers(dstVertex, vertexAmount);
 *			</init>
 *			<size> vertexAmount </size>
 *  	</testperf>
 *  \endxmlonly
 */
 //! \{
extern "C" int repackToPrimitives_tf(const v4nm32f *srcVertex,
	const v4nm32f *srcColor,
	const v2nm32f *srcTex,
	TrianglePointers *dstVertex,
	int vertexAmount);
 //! \}


#endif //__PRIMITIVE_H__
