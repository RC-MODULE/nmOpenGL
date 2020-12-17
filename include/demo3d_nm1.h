#ifndef DEMO3D_NM1_H 
#define DEMO3D_NM1_H 

#include "nmtype.h"
#include "nmgl.h"
#include "demo3d_common.h"
#include "imagebuffer.h"
#include "myserverdma.h"
#include "nmgltex_nm1.h"

void selectPatterns(nm32s* dydxTable, nm32s* dX, nm32s* dY, nm32s* x0, nm32s* pPtrnPaintSide, nm32s** pSrcPack, int nSize, int* pTmp);

/*!
 *  \brief Контекст nmOpengl на ядре NMPU1
 *  
 *  \details Контекст сделан в виде статического класса и должен существовать только в единственном экземпляре.
 *  От значений различных параметров зависит работа функций Opengl
 */
struct NMGL_Context_NM1 {
private:
	static NMGL_Context_NM1 *context;
	//NMGL_Context_NM1() {};		// недоступный конструктор
	//~NMGL_Context_NM1() {};		// и деструктор
public:
	static void create() {
		//context = (NMGL_Context_NM1*)halMalloc32(sizeof32(NMGL_Context_NM1));
	}
	static void bind(NMGL_Context_NM1* cntxt) {
		context = cntxt;
	}
	inline static NMGL_Context_NM1 *getContext() {
		return context;
	}
	static void free() {
		//halFree(context);
	}

	Pattern polyImgTmp[SMALL_SIZE];		
	
	PatternsArray* patterns;			///< Указатель на массив всевозможных двухбитных паттернов

	nm32s** ppSrcPackPtrns;				///< Массив указателей нужных в данный момент двухбитных паттернов. Должен быть размера 3 * POLYGONS_SIZE
	nm32s** ppDstPackPtrns;				///< Массив указателей назначения копируемых двухбитных паттернов
	nm32s* nSizePtrn32;					///< Размеры копируемого куска паттерна (в 32-разрядных словах)

	int* buffer0;						///< Указатель на временный буфер 0. Размер должен быть не меньше SIZE_BUFFER_NM1
	int* buffer1;						///< Указатель на временный буфер 1. Размер должен быть не меньше SIZE_BUFFER_NM1
	int* buffer2;						///< Указатель на временный буфер 2. Размер должен быть не меньше SIZE_BUFFER_NM1
	int pointSize;						///< Диаметр точек в режиме отрисовки GL_POINTS. Не должен быть больше 32
	clock_t t0, t1;	

	NMGLSynchro synchro;				///< Структура для общения процессоров друг с другом
	ImageConnector* imageConnector;				///< Коннектор к кольцевому буферу изображений
	PolygonsConnector* polygonsConnectors;		///< Указатель на коннекторы структуры Polygons, необходимой для передачи данных с ядра на ядро

	DepthBuffer depthBuffer;			///< Структура для работы с целым буфером глубины
	IMAGE_BUFFER_CLASS colorBuffer;		///< Структура для работы с целым цветным буфером
	IMAGE_BUFFER_CLASS smallColorBuff;	///< Структура для работы с куском буфера цвета, лежащим во внутренней памяти
	IMAGE_BUFFER_CLASS smallDepthBuff;	///< Структура для работы с куском буфера глубины, лежащим во внутренней памяти

	IMAGE_BUFFER_CLASS smallClearColorBuff;		///< Структура для работы с куском буфера цвета, предназначенным для очистки буфера цвета
	IMAGE_BUFFER_CLASS smallClearDepthBuff;		///< Структура для работы с куском буфера глубины, предназначенным для очистки буфера глубины

	nm32s** zBuffPoints;				///
	nm32s** imagePoints;

	Pattern* ppPtrns1_2s[SMALL_SIZE];	///< Массив указателей на один пак локальных паттернов
	Pattern* ppPtrns2_2s[SMALL_SIZE];	///< Массив указателей на второй пак локальных паттернов
	Pattern* ppPtrnsCombined_2s[SMALL_SIZE]; ///< Массив указателей на комбинированный пак первых и вторых локальных паттернов
	nm32s minusOne[SMALL_SIZE];				///< массив со значения -1

	Vector2* ptrnInnPoints;				///< Указатель на левую нижнюю точку копируемой в изображение части паттернов
	Size* ptrnSizes;					///< Указатель на размеры копируемой в изображение части паттернов
	nm32s* valuesZ;						///< Значения Z примитива для теста глубины
	nm32s* valuesC;						///< Значения цвета примитивов
	
	NMGL_Context_NM1_Texture texState;

};


extern "C" {
	
	/**
	 *  \ingroup service_api
	 *  \defgroup mMaskVxN mMaskVxN
	 *  \brief Функция маскирования пакетов треагольников
	 *  \author Жиленков Иван
	 *  
	 *  \param pTriangles [in] Массив изображений треугольников, расположенных друг за другом
	 *  \param pMask [in] Массив масок треугольников, аналогичные изображения
	 *  \param pROI [in] Массив указателей на точки изображения соответствующие левому верхнему углу изображения треугольника
	 *  \param imageStride [in] Ширина изображения
	 *  \param ptrnSizes [in] Массив размеров треугольников
	 *  \param count [in] Число треугольников
	 *  
	 *  \details Изображения идут сплошным массивом. Для разграничения их друг от друга используется *ptrnSizes*
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> pTriangles </param> <values>  </values>
	 *          <param> pMask </param> <values>  </values>
	 *          <param> pROI </param> <values>  </values>
	 *          <param> imageStride </param> <values>  </values>
	 *          <param> pTriangsHeight </param> <values>  </values>
	 *          <param> pTriangsWidth </param> <values>  </values>
	 *          <param> count </param> <values>  </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void mMaskVxN_32s(nm32s* pTriangles, nm32s* pMask, nm32s** pROI, int imageStride, Size* ptrnSizes, int count);
	void mMaskVxN_16s(nm16s* pTriangles, nm16s* pMask, nm16s** pROI, int imageStride, Size* ptrnSizes, int count);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция осуществления теста глубины
	 *  \author Жиленков Иван
	 *  \warning Функция имеет внутренние параметры, которые определяют по какому правилу происходит тест
	 *  
	 *  \param pROI [in] Массив указателей на точки изображения соответствующие левому верхнему углу изображения треугольника
	 *  \param imageStride [in] Ширина изображения
	 *  \param pTriangles [in] Массив изображений треугольников, расположенных друг за другом
	 *  \param pDstMask [in] Выходной массив с масками треугольников
	 *  \param pTriangSizes [in] Массив размеров треугольников
	 *  \param count [in] Число треугольников
	 *  
	 *  \details Изображения идут сплошным массивом. Для разграничения их друг от друга используется *ptrnSizes*
	 *  
	 */
	 //! \{
	void depthTest32(nm32s** pROI, int imageStride, nm32s* pTriangles, nm32s* pDstMask, Size* pTriangSizes, int count);
	void depthTest16(nm16s** pROI, int imageStride, nm16s* pTriangles, nm16s* pDstMask, Size* pTriangSizes, int count);
	 //! \}

	 /*!
	 *  \ingroup service_api
	 *  \brief Функция выборки из двух констант по знаку входного элемента
	 *  \author Жиленков Иван
	 *  
	 *  \param pSrc [in] Входной массив
	 *  \param X [in] Выходное значение при входном элементе меньше нуля
	 *  \param Y [in] Выходное значение при входном элементе больше или равном нулю
	 *  \param pDst [out] Выходной массив
	 *  \param nSize [in] Число элементов
	 *  
	 */
	void selectPaintSide(nm32s* pSrc, int X, int Y, nm32s* pDst, int nSize);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция умножения кусков двухбитного паттерна на константу
	 *  \author Жиленков Иван
	 *  
	 *  \param patterns [in] Массив двухразрядных паттернов
	 *  \param innerPoint [in] Координаты левого нижнего угла умножаемого куска паттерна
	 *  \param sizes [in] Размеры умножаемого куска паттерна
	 *  \param valueC [in] Умножаемая константа
	 *  \param pDstTreangle [out] Выходной массив кусков изображения в нужной разрядности
	 *  \param count [in] Число паттернов
	 *  
	 *  \details Функция берет из паттерна некий прямоугольный кусок с начальной координатой *innerPoint(x,y)* и размерами *sizes(width, height)* и умножает его на константу *valueC* (это может быть как число, так и вектор цвета, состоящий из красной, зеленой и синей компоненты). Получившееся изображение кладется в выходной массив *pDstTreangle* таким образом, что после конца одного изображения сразу следует начало другого.
	 *  
	 *  Прямоугольник характеризуемый переменным *innerPoint* и *sizes* не должен выходить за пределы квадрата 32х32. Однако допускается, что *innerPoint.x* может быть равен -1, при этом изображение строится таким образом, чтобы быть выровненным по 64-битному слову для точной вставки в буфер изображения.
	 *  
	 *  16-разрядные функции в данный момент не используются и не подцепляются.
	 *  
	 */
	 //! \{
	void mMulCVxN_2s32s(Pattern* patterns, Vector2* innerPoint, Size* sizes, int* valueC, nm32s* pDstTreangle, int count);
	void mMulCVxN_2s_RGB8888(Pattern* patterns, Vector2* innerPoint, Size* sizes, int* valueC, nm32s* pDstTreangle_32s, int count);
	void mMulCVxN_2s16s(Pattern* patterns, Vector2* innerPoint, Size* sizes, int* valueC, nm16s* pDstTreangle_32s, int count);
	void mMulCVxN_2s_RGB565(Pattern* patterns, Vector2* innerPoint, Size* sizes, int* valueC, nm16s* pDstTreangle_32s, int count);	
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функции массового смещения адресов на одно и то же смещения
	 *  \author Жиленков Иван
	 *  
	 *  \param baseAddr [in] Входной массив адресов
	 *  \param offsets [in] Смещения
	 *  \param ppDst [in] Выходной массив адресов
	 *  \param size [in] Число элементов
	 *  
	 *  
	 */
	 //! \{
	void baseAddrOffs_32s(nm32s* baseAddr, int* offsets, nm32s** ppDst, int size);
	void baseAddrOffs_32u(nm32u* baseAddr, int* offsets, nm32u** ppDst, int size);
	void baseAddrOffs_16s(nm16s* baseAddr, int* offsets, nm16s** ppDst, int size);
	void baseAddrOffs_16u(nm16u* baseAddr, int* offsets, nm16u** ppDst, int size);
	 //! \}

	/*!
	 *  \ingroup service_api
	 *  \brief Brief description
	 *  \author Жиленков Иван
	 *  
	 *  \param pSrc1 [in] Description for pSrc1
	 *  \param pSrc2 [in] Description for pSrc2
	 *  \param pDst [in] Description for pDst
	 *  \param size [in] Description for size
	 *  \param count [in] Description for count
	 *  
	 *  \details Details
	 *  
	 */
	void mAndVxN_32u(nm32u** pSrc1, nm32u** pSrc2, nm32u** pDst, int* size, int count);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция копирования нескольких кусков.
	 *  \author Жиленков Иван
	 *  \warning Максимальный размер каждого куска не должен превышать 64 32-разрядных слов
	 *  
	 *  \param ppSrc [in] Массив указателей входных данных
	 *  \param ppDst [in] Массив указателей выходных данных
	 *  \param size [in] Массив размеров данных (в 32-разрядных словах)
	 *  \param count [in] Число копирований
	 *  
	 *  
	 */
	void copyPacket_32s(nm32s** ppSrc, nm32s** ppDst, int* size, int count);

	/*!
	 *  \ingroup service_api
	 *  \brief Brief description
	 *  \author Жиленков Иван
	 *  
	 *  \param ppSrcCA [in] Description for ppSrcCA
	 *  \param ppSrcCB [in] Description for ppSrcCB
	 *  \param ppSrcBA [in] Description for ppSrcBA
	 *  \param step [in] Description for step
	 *  \param ppDst [in] Description for ppDst
	 *  \param count [in] Description for count
	 *  \return Return description
	 *  \details Details
	 *  
	 */
	void mergePtrnsAddr3(nm32s** ppSrcCA, nm32s** ppSrcCB, nm32s** ppSrcBA, int step, nm32s** ppDst, int count);
	void mergePtrnsAddr2(nm32s** ppSrc1, nm32s** ppSrc2, int step, nm32s** ppDst, int count);

	/**
	 *  \ingroup service_api
	 *  \defgroup selectColorChannel selectColorChannel
	 *  \brief Функция выборки канала из BGR_32(8-8-8-8) изображения
	 *  \author Жиленков Иван
	 *  
	 *  \param srcImage [in] Входное изображение
	 *  \param channel [in] Канал ( 0 - blue, 1 - green, 2 -red)
	 *  \param dst [out] Выходной массив
	 *  \param size [in] Размер изображения в пикселях
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> srcImage </param> <values>  </values>
	 *          <param> channel </param> <values>  </values>
	 *          <param> dst </param> <values>  </values>
	 *          <param> size </param> <values>  </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void selectColorChannel(v4nm8s* srcImage, int channel, nm8s* dst, int size);

	/*!
	 *  \ingroup service_api
	 *  \brief Функция нахождения отрицательных смещений
	 *  \author Жиленков Иван
	 *  
	 *  \param src [in] Входной массив
	 *  \param dst [out] Выходной массив
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция вычисляет следующее выражение
	 *  \f[f(x)= 
	 *  \begin{cases}
	 *  |x| & x < 0 \\
	 *  0 & x >= 0
	 *  \end{cases}
	 *  \f]
	 *  
	 */
	void absIfNegElse0(int* src, int* dst, int size);

	/*!
	 *  \ingroup service_api
	 *  \brief Суммирования всех элементов
	 *  \author Жиленков Иван
	 *  
	 *  \param pVec [in] Массив значений
	 *  \param size [in] Число элементов
	 *  \retval Сумма всех заданных чисел
	 *  
	 *  \details Функция считает \f[ f=\Sigma_{i = 0}^n i\f]
	 *  
	 */
	int totalSum(nm32s* pVec, int size);

	/*!
	 *  \ingroup service_api
	 *  \brief Brief description
	 *  \author Жиленков Иван
	 *  
	 *  \param src1 [in] Description for src1
	 *  \param src2 [in] Description for src2
	 *  \param src3 [in] Description for src3
	 *  \param src4 [in] Description for src4
	 *  \param dst [in] Description for dst
	 *  \param size [in] Description for size
	 *  
	 *  \details Details
	 *  
	 */
	void merge_v4nm32s(nm32s* src1, nm32s* src2, nm32s* src3, nm32s* src4, v4nm32s* dst, int size);

	/*!
	 *  \ingroup service_api
	 *  \brief Brief description
	 *  \author Жиленков Иван
	 *  
	 *  \param srcArray [in] Description for srcArray
	 *  \param dstArray [in] Description for dstArray
	 *  \param count [in] Description for count
	 *  
	 *  \details Details
	 *  
	 */
	void duplicate_16s(const nm16s *srcArray, nm32s *dstArray, int count);

	/*!
	 *  \ingroup service_api
	 *  \brief Brief description
	 *  
	 *  \param srcArray [in] Description for srcArray
	 *  \param dstArray [in] Description for dstArray
	 *  \param count [in] Description for count
	 *  
	 *  \details Details
	 *  
	 */
	void inverse_v4nm8u(const v4nm8u *srcArray, v4nm8u *dstArray, int count);
	
}

/*!
 *  \ingroup service_api
 *  \brief Brief description
 *  \author Жиленков Иван
 *  \warning Функции используют контекст NMGL_Context_NM1
 *  
 *  \return Return description
 *  \details Details
 *  
 */
 //! \{
void drawTriangles();
void drawLines();
void drawPoints();
 //! \}

/*!
 *  \ingroup service_api
 *  \brief Brief description
 *  \author Жиленков Иван
 *  
 *  \param poly,data [in] Description for poly
 *  
 *  \details Details
 *  
 */
 //! \{
int getAddrPtrnsT(PolygonsOld* poly);
int getAddrPtrnsL(PolygonsOld* poly);
int getAddrPtrnsP(PolygonsOld* poly);
int getAddrPtrnsT(DataForNmpu1* data);
int getAddrPtrnsL(DataForNmpu1* data);
int getAddrPtrnsP(DataForNmpu1* data);
 //! \}

#endif