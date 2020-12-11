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
	 *  \defgroup mMaskVxN mMaskVxN
	 *  \brief Функция маскирования пакетов треагольников
	 *  
	 *  \param pTriangles [in] Description for pTriangles
	 *  \param pMask [in] Description for pMask
	 *  \param pROI [in] Description for pROI
	 *  \param imageStride [in] Description for imageStride
	 *  \param pTriangsHeight [in] Description for pTriangsHeight
	 *  \param pTriangsWidth [in] Description for pTriangsWidth
	 *  \param count [in] Description for count
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
	 *  \brief Brief description
	 *  
	 *  \param pROI [in] Description for pROI
	 *  \param imageStride [in] Description for imageStride
	 *  \param pTriangles [in] Description for pTriangles
	 *  \param pDstMask [in] Description for pDstMask
	 *  \param pTriangSizes [in] Description for pTriangSizes
	 *  \param count [in] Description for count
	 *  \return Return description
	 *  \details Details
	 *  
	 */
	 //! \{
	void depthTest32(nm32s** pROI, int imageStride, nm32s* pTriangles, nm32s* pDstMask, Size* pTriangSizes, int count);
	void depthTest16(nm16s** pROI, int imageStride, nm16s* pTriangles, nm16s* pDstMask, Size* pTriangSizes, int count);
	 //! \}

	 /*!
	 *  \brief Brief description
	 *  
	 *  \param pSrc [in] Description for pSrc
	 *  \param X [in] Description for X
	 *  \param Y [in] Description for Y
	 *  \param pDst [in] Description for pDst
	 *  \param nSize [in] Description for nSize
	 *  \return Return description
	 *  \details Details
	 *  
	 */
	void selectPaintSide(nm32s* pSrc, int X, int Y, nm32s* pDst, int nSize);
	
	/*!
	 *  \brief Brief description
	 *  
	 *  \param patterns [in] Description for patterns
	 *  \param innerPoint [in] Description for innerPoint
	 *  \param sizes [in] Description for sizes
	 *  \param valueC [in] Description for valueC
	 *  \param pDstTreangle [in] Description for pDstTreangle
	 *  \param count [in] Description for count
	 *  
	 *  \details Details	
	 *  
	 */
	 //! \{
	void mMulCVxN_2s32s(Pattern* patterns, Vector2* innerPoint, Size* sizes, int* valueC, nm32s* pDstTreangle, int count);
	void mMulCVxN_2s_RGB8888(Pattern* patterns, Vector2* innerPoint, Size* sizes, int* valueC, nm32s* pDstTreangle_32s, int count);
	void mMulCVxN_2s16s(Pattern* patterns, Vector2* innerPoint, Size* sizes, int* valueC, nm16s* pDstTreangle_32s, int count);
	void mMulCVxN_2s_RGB565(Pattern* patterns, Vector2* innerPoint, Size* sizes, int* valueC, nm16s* pDstTreangle_32s, int count);	
	 //! \}
	
	
	void baseAddrOffs_32s(nm32s* baseAddr, int* offsets, nm32s** ppDst, int size);
	void baseAddrOffs_32u(nm32u* baseAddr, int* offsets, nm32u** ppDst, int size);
	void baseAddrOffs_16s(nm16s* baseAddr, int* offsets, nm16s** ppDst, int size);
	void baseAddrOffs_16u(nm16u* baseAddr, int* offsets, nm16u** ppDst, int size);

	void mAndVxN_32u(nm32u** pSrc1, nm32u** pSrc2, nm32u** pDst, int* size, int count);
	
	void copyPacket_32s(nm32s** ppSrc, nm32s** ppDst, int* size, int count);

	void mergePtrnsAddr3(nm32s** ppSrcCA, nm32s** ppSrcCB, nm32s** ppSrcBA, int step, nm32s** ppDst, int count);
	void mergePtrnsAddr2(nm32s** ppSrc1, nm32s** ppSrc2, int step, nm32s** ppDst, int count);

	/**
	 *  \defgroup selectColorChannel selectColorChannel
	 *  \brief Функция выборки канала из BGR_32(8-8-8-8) изображения
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

	void absIfNegElse0(int* src, int* dst, int size);

	int totalSum(nm32s* pVec, int size);

	void merge_v4nm32s(nm32s* src1, nm32s* src2, nm32s* src3, nm32s* src4, v4nm32s* dst, int size);

	void duplicate_16s(const nm16s *srcArray, nm32s *dstArray, int count);

	void inverse_v4nm8u(const v4nm8u *srcArray, v4nm8u *dstArray, int count);
	
}

void drawTriangles();
void drawLines();
void drawPoints();


int getAddrPtrnsT(PolygonsOld* poly);
int getAddrPtrnsL(PolygonsOld* poly);
int getAddrPtrnsP(PolygonsOld* poly);
int getAddrPtrnsT(DataForNmpu1* data);
int getAddrPtrnsL(DataForNmpu1* data);
int getAddrPtrnsP(DataForNmpu1* data);

#endif