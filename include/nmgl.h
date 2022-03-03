#ifndef __NMGL_H__
#define __NMGL_H__


/*! 
	\page intro Введение

Библиотека предназначена для построения графических OpenGL приложений на процессорном ядре NMC4, на плате МС121.01 . 
В состав библиотеки входят интерфейсные функции, начинающиеся с префикса nmgl и определяемые спецификацией OpenGL SC 1.0.1. 


*/


#include "nmgltype.h"

#include "nmgldef.h"

/*!
\defgroup  main Интерфейсные функции nmOpenGL
 */


/*!
\defgroup  opengl_api Интерфейсные функции nmOpenGL
 */

///@{ 

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

/*!
 \brief Функция выбирает текстурный модуль для последующей модификации функциями текстурирования.
 \param texture идентификатор текстурного модуля, устанавливаемого в качестве активного. 


 \details Значение аргумента texture представляет собой символическую константу вида NMGL_TEXTUREi. Значение константы NMGL_TEXTUREi определяется по формуле NMGL_TEXTUREi = NMGL_TEXTURE0 + i. i лежит в диапазоне 0..(k-1), где k-поддерживаемое число модулей текстур, зависящее от реализации. Значение k определяется значением параметра NMGL_MAX_TEXTURE_UNITS. По умолчанию активным модулем текстуры является модуль с идентификатором NMGL_TEXTURE0. Минимальное количество поддерживаемых модулей текстур равно 1. Если в функцию передано неподдерживаемое значение texture, должна быть сформирована ошибка NMGL_INVALID_ENUM.
 
 */
void nmglActiveTexture (NMGLenum texture);

void nmglAlphaFunc (NMGLenum func, NMGLclampf ref);

/*!
 \brief Задаёт ограничения для вершин примитива или группы одинаковых примитивов
 
   \param mode Определяет примитив(ы), которые будут созданы из вершин между glBegin и последующим [nmglEnd](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnd.xml). Приняты десять символьных констант: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN.


 
 \details nmglBegin и  <a href="https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnd.xml"> nmglEnd </a>  задают ограничения для вершин, которые определяют примитив, или группу одинаковых примитивов. Для nmglBegin допустим только один аргумент, который определяет, каким из десяти способов вершины будут считаны. Пусть n -- целое число, не менее 1, а N -- общее количество выбранных вершин. Интерпретации будут выглядеть следующим образом:

GL_POINTS
> Обрабатывает каждую вершину как отдельную точку Вершина n определяет точку n. Изображено N точек.

GL_LINES
> Обрабатывает каждую пару вершин как независимый сегмент линии. Вершины 2*n - 1 и 2*n определяют линию n. Будет отображено N/ 2 линий.

GL_LINE_STRIP

> Рисует связанную группу сегментов линии от первой вершины к последней. Вершины n и n + 1 определяют линию n. Будет отображено N - 1 линий.

GL_LINE_LOOP

> Рисует связанную группу сегментов линии от первой вершины к последней, а потом обратно. Вершины n и n + 1 определяют линию n. Эта линия, однако, будет определена вершинами N и 1. Будет нарисовано N линий.

GL_TRIANGLES

> Интерпретирует каждые три вершины как самостоятельный треугольник. Вершины 3*n - 2 , 3*n - 1 , и 3*n определяют треугольник n. Будет отображено N/ 3 треугольников.

GL_TRIANGLE_STRIP

> Рисует связанную группу треугольников. Один треугольник определяется для каждой вершины, представленной после двух предыдущих. Если n является нечётным числом, вершины n, n + 1 и n + 2 определяют треугольник n. Если n является чётным числом, вершины n+1, n и n + 2 определяют треугольник n. Будет отображено N - 2 треугольников.

GL_TRIANGLE_FAN

> Рисует связанную группу треугольников. Один треугольник определяется для каждой вершины, представленной после двух предыдущих. Вершины 1 , n + 1 , и n + 2 определяют треугольник n. Будет нарисовано N - 2 треугольников.

Только набор команд GL может быть применён между nmglBegin и [nmglEnd](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnd.xml). Он включает команды [nmglVertex](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glVertex.xml), [nmglColor](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColor.xml), [nmglSecondaryColor](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glSecondaryColor.xml), [nmglNormal](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glNormal.xml), [nmglTexCoord](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTexCoord.xml), [nmglMultiTexCoord](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMultiTexCoord.xml), [nmglArrayElement](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glArrayElement.xml) и [nmglMaterial](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMaterial.xml). Кроме того, можно использовать [nmglCallLists](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glCallLists.xml) для формирования дисплейных списков, включающих только предыдущие команды. Если какая-либо команда GL выполняется между nmglBegin и [nmglEnd](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnd.xml), появляется сообщение об ошибке и команда игнорируется.

Независимо от значения, выбранного для *mode*, отсутствуют ограничения для количества вершин, которые могут быть определены между nmglBegin и [nmglEnd](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnd.xml). Неполностью определённые линии, треугольники, четырёхугольники, и многоугольники не будут отображены. Объект считается неполностью определённым, если либо установлено слишком мало вершин даже для одного примитива, либо некорректно указано количество вершин. Неполные примитивы игнорируются, остальные будут отображены.

Предусмотрены минимальные количества вершин для каждого примитива: 1 для точки, 2 для линии, 3 для треугольника, 4 для четырёхугольника и 3 для многоугольника. Точного количества вершин требуют следующие режимы. GL_LINES (2), GL_TRIANGLES (3), GL_QUADS (4), и GL_QUAD_STRIP (2).



 */
 void nmglBegin(NMGLenum mode);



void nmglBindTexture (NMGLenum target, NMGLuint texture);


void nmglBitmap (NMGLsizei width, NMGLsizei height, NMGLfloat xorig, NMGLfloat yorig, NMGLfloat xmove, NMGLfloat ymove, const NMGLubyte *bitmap);
void nmglBlendFunc (NMGLenum sfactor, NMGLenum dfactor);
void nmglCallLists (NMGLsizei n, NMGLenum type, const NMGLvoid *lists);

/*!
 \brief Очищает буферы до предустановленных значений
 
 \param mask [in] Побитовый оператор OR для масок, который показывает, что буферам требуется очистка. Предусмотрено четыре буфера: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, и GL_STENCIL_BUFFER_BIT


 \details nmglClear устанавливает для области с битовым слоем в окне значения, ранее выбранные с помощью команд nmglClearColor, nmglClearDepth и nmglClearStencil.

Тесты pixel ownership test, scissor test, цветовое редуцирование, маски записи в буфере влияют на функционирование nmglClear. Обрезаемый фрагмент определяется очищенной в результате данной команды областью. Альфа-функция, функция плавного перехода, логические операции, шаблонная печать, текстурное отображение, буферизация глубины игнорируются командой nmglClear.

КомандаglClear применяет единственный аргумент, который представляет собой побитовый оператор OR между несколькими значениями, которые определяют, какой буфер должен быть очищен.

Значения предусмотрены следующие:

GL_COLOR_BUFFER_BIT

> Обозначает буфера, которые на настоящий момент активированы для записи цвета.

GL_DEPTH_BUFFER_BIT

> Обозначает буфер глубины.

GL_STENCIL_BUFFER_BIT

> Обозначает буфер шаблонов.

Значение, которое присваивается каждому буферу при очистке, зависит от настроек относительно значения для очистки, предусмотренных для данного буфера.
 
 */
void nmglClear (NMGLbitfield mask);

/*!
 \brief Определяет значения для очистки для буферов цвета.
 
 \param red,green,blue,alpha [in] Определяет значения уровней красного, зелёного, синего, альфа-канала, которые применяются при очистке буфера. Изначальные значения -- 0.


 \details nmglClearColor определяет значения уровней красного, зелёного, синего, альфа-канала, применяемые командой [nmglClear](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glClear.xml) при очистке буфера цвета. Значения, определённые командой nmglClearColor фиксируются в диапазоне 0 -- 1.
 
 */
void nmglClearColor (NMGLclampf red, NMGLclampf green, NMGLclampf blue, NMGLclampf alpha);

/*!
 \brief Определяет значения для очистки для буфера глубины
 
 \param depth [in] Определяет значение глубины, применяемое при очистке буфера глубины. Изначальное значение -- 1.


 \details nmglClearDepth определяет уровень глубины, применяемый командой [nmglClear](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glClear.xml) для очистки буфера глубины. Значения, определённые командой nmglClearDepth, фиксируются в диапазоне 0 -- 1.
 
 */
void nmglClearDepthf (NMGLclampf depth);
void nmglClearStencil (NMGLint s);
void nmglClientActiveTexture (NMGLenum texture);

/*!
 \defgroup nmglColor4 nmglColor4(f,ub)
 \brief устанавливает текущий цвет
 \param red, green, blue, alpha [in] Определяет новые уровни красного, зелёного, синего, альфа-канала для текущего цвета


 \details nmglColor устанавливает новый цвет RGBA с четырьмя значениями.

Значения текущего цвета сохраняются в виде чисел с зафиксированной или плавающей запятой. Если они сохраняются с плавающей запятой, указать степень и мантиссу невозможно.

Значения ни с плавающей, ни с зафиксированной запятой не фиксируются на уровне \[0, 1\] до обновления текущего цвета. Однако цветовые компоненты фиксируются на этом уровне до того, как они будут введены или записаны в буфер цветов.

Цветовые компоненты без знака байт, установленные с помощью команды nmglColor4ub последовательно преображаются в значения с плавающей запятой. Например, значение 255 преобразуется в 1,0 (полная интенсивность), а 0 преобразуется 0,0 (нулевая интенсивность).
 
 */
 //! \{
void nmglColor4f (NMGLfloat red, NMGLfloat green, NMGLfloat blue, NMGLfloat alpha);
void nmglColor4ub (NMGLubyte red, NMGLubyte green, NMGLubyte blue, NMGLubyte alpha);
 //! \}
void nmglColor4fv (const NMGLfloat *v);
void nmglColorMask (NMGLboolean red, NMGLboolean green, NMGLboolean blue, NMGLboolean alpha);
void nmglColorPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);

/*!
 \brief Функция обеспечивает возможность замены части палитры новыми значениями.
 \param target определяет тип текстуры, для которой задаётся палитра. Единственное поддерживаемое значение: NMGL_TEXTURE_2D.
 \param start определяет индекс внутри массива текселей активного текстурного объекта, начиная с которого будет осуществляться запись новых значений из масива, доступного через аргумент data. Представляет собой целое неотрицательное число.
 \param count определяет количество текселей, выбранных для копирования из массива, доступного через параметр data. В случае, если start+count превышает длину палитры активного текстурного объекта, то будут изменены все тексели, от текселя с индексом start до конца палитры.
 \param format определяет количество компонент в одном текселе. Единственное поддерживаемое значение: NMGL_RGBA.
 \param type определяет формат представления компонент текселя. Единственное поддерживаемое значение - NMGL_UNSIGNED_BYTE.
 \param data  указатель на массив данных текселей загружаемой палитры.


 \details Функция работает в соответствии со следующим алгоритмом:

1.  Получить указатель на контекст

2.  Проверить корректность входных аргументов:

    1.  Если аргумент target имеет значение, отличное от NMGL_TEXTURE_2D, то установить переменной ошибки контекста значение NMGL_INVALID_ENUM и завершить работу.

    2.  Если аргумент type имеет значение, отличное от NMGL_UNSIGNED_BYTE, то установить переменной ошибки контекста значение NMGL_INVALID_ENUM и завершить работу.

    3.  Если аргумент format имеет значение, отличное от NMGL_RGBA, то установить переменной ошибки контекста значение NMGL_INVALID_VALUE и завершить работу.

    4.  Если значение аргумента start меньше нуля или не меньше значения длины палитры активного текстурного объекта, то установить переменной ошибки контекста значение NMGL_INVALID_VALUE и завершить работу.

3.  Записать X текселей из массива, доступного через аргумент data в палитру активного текстурного объекта. Запись осуществлять, начиная с индекса start палитры активного текстурного объекта. X= start, в случае, если start+count не превышает длины палитры активного текстурного объекта. В противном случае X=width-start, где width -- лдина палитры активного текстурного объекта.
 
 */
void nmglColorSubTableEXT (NMGLenum target, NMGLsizei start, NMGLsizei count, NMGLenum format, NMGLenum type, const NMGLvoid *table);
void nmglColorTableEXT (NMGLenum target, NMGLenum internalformat, NMGLsizei width, NMGLenum format, NMGLenum type, const NMGLvoid *table);
void nmglCopyPixels (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height, NMGLenum type);

/*!
 \brief Определяет, где будет производиться отбраковка фрагментов: на лицевой или на задней стороне.
 
 \param mode [in] Определяет, где будет производиться отбор фрагментов-кандидатов на отбраковку: на лицевой или на задней стороне. Применяются следующие символьные константы GL_FRONT, GL_BACK, и GL_FRONT_AND_BACK . Начальное значение -- GL_BACK.


 \details nmglCullFace определяет, где будет производиться отбраковка фрагментов: на лицевой или на задней стороне (как определяется в соответствии с оператором *mode*) когда отбраковка фрагментов активирована. Изначально отбраковка фрагментов деактивирована. Чтобы активировать и деактивировать отбраковку фрагментов, следует применить команды [nmglEnable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnable.xml) и [nmglDisable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisable.xml) с аргументом GL_CULL_FACE. Подлежащие отбраковке фрагменты включают треугольники, многоугольники, прямоугольники, другие четырёхугольники.

[nmglFrontFace](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glFrontFace.xml) определяет, какие из фрагментов с оборотом по часовой стрелке и против часовой стрелки будут располагаться на лицевой, а какие на задней стороне. См. [nmglFrontFace](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glFrontFace.xml).
 
 */
void nmglCullFace (NMGLenum mode);

/*!
 \brief Определяет значение, с которым будет сравниваться глубина буфера
 
 \param func [in] Определяет, функцию сравнения глубины. Применяются следующие символьные константы GL_LESS, GL_LEQUAL и GL_ALWAYS. Начальное значение -- GL_LESS.

 \details nmglDepthFunc определяет функцию, применяемую для сравнения значения глубины каждого входящего пикселя со значением глубины, представленным в буфере глубины. Сравнение происходит только, если активировано тестирование глубины. (См. [nmglEnable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnable.xml) и [nmglDisable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisable.xml) для GL_DEPTH_TEST.)

*func* определяет условия, в которых пиксель не будет изображён. Предусмотрено следующие функции сравнения:

GL_LESS

> Применяется, если входящее значение глубины меньше хранимого.

GL_LEQUAL

> Применяется, если входящее значение глубины меньше или равно хранимому.

GL_ALWAYS

> Применяется всегда.

Изначальное значение *func* -- GL_LESS. Изначально тестирование глубины деактивировано. Если тестирование глубины деактивировано или буфер глубины отсутствует, тест глубины всегда считается пройденным.
 
 */
void nmglDepthFunc (NMGLenum func);
void nmglDepthMask (NMGLboolean flag);
void nmglDepthRangef (NMGLclampf zNear, NMGLclampf zFar);
void nmglDisableClientState (NMGLenum array);

/*!
 \brief Визуализирует примитивы из массива данных
 
 \param mode [in] Определяет, какие примитивы будут визуализированы. Применяются следующие символьные константы GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, илиGL_TRIANGLE_FAN.
 \param first [in] Определяет начальный показатель в активированных массивах.
 \param count [in] Определяет, какие показатели будут визуализированы

 \details nmglDrawArrays определяет набор геометрических примитив с малым количеством обращений к подпрограмме. Вместо вызова процедуры GL для прохождения каждой конкретной вершины, нормали, текстурной координаты, флага ребра, цвета, можно заранее указать отдельные массивы вершин, нормалей и цветов и использовать их для конструирования последовательности примитивов, благодаря единственному вызову команды nmglDrawArrays.

Когда вызвана команда nmglDrawArrays, она применяет элемент*count* -- подсчёт следующих друг за другом элементов из каждого активного массива для конструирования последовательности геометрических примитивов, начиная с элемента *first*. Элемент *mode* определяет характер примитивов, которые будут сконструированы, и то, как элементы массива будут их конструировать. Если команда GL_VERTEX_ARRAY не активирована, геометрические примитивы сгенерированы не будут.

Значения атрибутов вершин, которые модифицируются командой nmglDrawArrays после действия команды nmglDrawArrays устанавливать будет нельзя. Например, если команда GL_COLOR_ARRAY активирована, после применения команды nmglDrawArrays значение текущего цвета установить будет нельзя. Немодифицируемые атрибуты останутся чётко сформулированными.
 
 */
void nmglDrawArrays (NMGLenum mode, NMGLint first, NMGLsizei count);
void nmglDrawElements (NMGLenum mode, NMGLsizei count, NMGLenum type, const NMGLvoid *indices);
void nmglDrawPixels (NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const NMGLvoid *pixels);

/*!
 \brief Активирует или деактивирует функции GL на стороне сервера
 
 \param cap [in] Определяет символьную константу, обозначающую функцию GL.


 \details nmglEnable и [nmglDisable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisable.xml) активируют и деактивируют различные функции. Следует применять [nmglIsEnabled](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glIsEnabled.xml) или [nmglGet](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glGet.xml) для определения текущих настроек любой из функций. Изначальное значение для каждой функции кроме GL_DITHER и GL_MULTISAMPLE -- GL_FALSE. Изначальное значение для GL_DITHER и GL_MULTISAMPLE -- GL_TRUE.

Команды nmglEnable и [nmglDisable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisable.xml) используют один аргумент, *cap*, который может принимать одно из следующих значений:

GL_ALPHA_TEST (не реализовано)

> Когда данная команда активирована, производится первый этап тестирования. См. [nmglAlphaFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glAlphaFunc.xml).

GL_BLEND (не реализовано)

> Когда данная команда активирована, значения цветов вычисленного фрагмента смешиваются со значениями в буфере цветов. См. [nmglBlendFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glBlendFunc.xml).

GL_CULL_FACE

> Когда данная команда активирована, в окне координат происходит отбрасывание многоугольников на основе их изломанной формы. См. [nmglCullFace](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glCullFace.xml).

GL_DEPTH_TEST

> Когда данная команда активна, происходят сравнения уровней глубины и обновляется буфер глубины. Следует обратить внимание на то, что даже если буфер глубины существует и маска глубины не равна нулю, буфер глубины не обновляется, если тестирование глубины деактивировано. См. [nmglDepthFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDepthFunc.xml) и [nmglDepthRange](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDepthRange.xml).

GL_LIGHT*i*

> Когда данная команда активирована, следует включить параметр света *i* в оценку балансировки света. См. [nmglLightModel](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLightModel.xml) и [nmglLight](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLight.xml).

GL_LIGHTING

> Когда данная команда включена, и нет активных вершинных шейдеров, следует применять текущие параметры освещения для вычисления цвета и индекса вершины. Вместо этого можно просто связать текущий цвет или индекс с каждой вершиной. См. [nmglMaterial](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMaterial.xml), [nmglLightModel](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLightModel.xml), и [nmglLight](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLight.xml).

GL_LINE_SMOOTH (не реализовано)

> Когда данная команда активирована, рисовать линии следует с корректным фильтрованием. В противном случае следует рисовать несглаженные линии. См. [nmglLineWidth](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLineWidth.xml).

GL_LINE_STIPPLE (не реализовано)

> Когда данная команда активирована, при рисовании линий следует использовать текущий образец штриховых элементов линии. См. [nmglLineStipple](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLineStipple.xml).

GL_NORMALIZE

> Когда данная команда включена, и нет активных вершинных шейдеров, векторы нормали нормализуются в соответствии с единичной длиной после трансформации и до освещения. Этот метод обычно менее эффективен, чем GL_RESCALE_NORMAL. См. [nmglNormal](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glNormal.xml) и [nmglNormalPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glNormalPointer.xml).

GL_POINT_SMOOTH (не реализовано)

> Когда данная команда активирована, рисовать точки следует с корректным фильтрованием. В противном случае следует рисовать несглаженные точки. См. [nmglPointSize](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPointSize.xml).

GL_RESCALE_NORMAL (не реализовано)

> Когда данная команда включена, и нет активных вершинных шейдеров, векторы нормали после трансформации и до освещения масштабируются в соответствии с фактором, который вычисляется с помощью матрицы моделирования. Если матрица моделирования единообразно масштабирует пространство, это возвращает трансформированную нормаль к единичной величине. Этот метод обычно более эффективен, чем GL_NORMALIZE. См. [nmglNormal](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glNormal.xml) и [nmglNormalPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glNormalPointer.xml).

GL_SCISSOR_TEST (не реализовано)

> Когда данная команда активна, происходит отбрасывание фрагментов, находящихся вне обрезанного четырёхугольника. См. [nmglScissor](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glScissor.xml).

GL_STENCIL_TEST (не реализовано)

> Когда данная команда активна, происходит тестирование шаблонов и обновляется буфер шаблонов. См. [nmglStencilFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilFunc.xml) и [nmglStencilOp](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilOp.xml).

GL_TEXTURE_2D

> Когда данная команда активна, и для фрагмента не предусмотрено активного шейдера, проводится двухмерное текстурирование (если не активно ещё и трёхмерное и кубическое текстурирование). См. [nmglTexImage2D]
 
 */
//! \{
void nmglEnable (NMGLenum cap);
void nmglDisable (NMGLenum cap);
//! \}

/*!
 \brief Brief description
 
 \param cap [in] Определяет, какие именно функции будут активированы. Применяются следующие символьные константы: GL_COLOR_ARRAY GL_NORMAL_ARRAY и GL_VERTEX_ARRAY .


 \details Команды nmglEnableClientState и [nmglDisableClientState](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisableClientState.xml) активируют или деактивирует отдельные функции клиента. По умолчанию все функции клиента деактивированы. Команды nmglEnableClientState и [nmglDisableClientState](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisableClientState.xml) используют один аргумент, *cap*, который может принимать одно из следующих значений:

GL_COLOR_ARRAY

> Когда данная команда применена, массив цветов активируется для записи и используется во время визуализации, если применена команда [nmglDrawArrays](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawArrays.xml) или [nmglDrawElements](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawElements.xml). См. [nmglColorPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColorPointer.xml).

GL_NORMAL_ARRAY

> Когда данная команда применена, массив нормалей активируется для записи и используется во время визуализации, если применена команда [nmglDrawArrays](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawArrays.xml) или [nmglDrawElements](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawElements.xml) См. [nmglNormalPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glNormalPointer.xml).

GL_TEXTURE_COORD_ARRAY

> Когда данная команда применена, массив текстурных координат активируется для записи и используется во время визуализации, если применена команда [nmglDrawArrays](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawArrays.xml) или [nmglDrawElements](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawElements.xml) См. [nmglTexCoordPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTexCoordPointer.xml).

GL_VERTEX_ARRAY

> Когда данная команда применена, массив вершин активируется для записи и используется во время визуализации, если применена команда [nmglDrawArrays](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawArrays.xml) или [nmglDrawElements](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawElements.xml) См. [nmglVertexPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glVertexPointer.xml).
 
 */
void nmglEnableClientState (NMGLenum cap);
void nmglEnd (void);
void nmglEndList (void);

/*!
 \brief Блокировка, которая действует, пока все процессы GL не будут завершены
 


 \details nmglFinish препятствует возврату из процедуры, пока действие всех ранее вызванных команд GL не завершено. Эффект таких действие включает все изменения состояния GL, все изменения состояния соединения и все изменения содержания буфера кадров.
 
 */
void nmglFinish (void);

/*!
 \brief Принудительно запускает команды GL в заданное время
 


 \details В различных вариантах исполнения GL команды вводятся в буфер в нескольких местах, включая сетевые буферы и сами ускорители выполнения графических операций. КомандаglFlush опустошает все эти буферы, в результате чего все сделанные команды выполняются, как только их принимают соответствующий механизм визуализации. Хотя такое выполнение не может быть завершено в любой заданный временной промежуток, в конечный временной промежуток оно завершается в установленный промежуток времени.

Поскольку любая программа GL может быть выполнена по сети или с помощью ускорителя, который добавляет команду в буфер, все программы должны вызывать команду nmglFlush, когда они планируют завершить все свои ранее вызванные команды. Например, следует вызвать команду nmglFlush перед тем, как ожидать пользовательский ввод, основанный на сгенерированном изображении.
 
 */
void nmglFlush (void);

/*!
 \brief nmglFrontFace
 
 \param mode [in] Определяет ориентацию лицевых многоугольников. Допустимы значения GL_CW и GL_CCW. Начальное значение -- GL_CCW.

 \details На «сцене», полностью составленной из непрозрачных закрытых поверхностей, задние многоугольники никогда не видны. Устранение этих невидимых многоугольников даёт очевидное преимущество в ускорении визуализации изображений. Чтобы активировать и деактивировать устранение задних многоугольников, следует применить [nmglEnable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnable.xml) и [nmglDisable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisable.xml) с аргументом GL_CULL_FACE.

Проекция многоугольника в окно координат представляется как имеющая оборот по часовой стрелке, если воображаемый объект, проходя путь от первой вершине к второй, третьей и так далее до последней, а потом снова к первой, будет двигаться по часовой стрелке по внутренней стороне многоугольника. Оборот многоугольника считается направленным против часовой стрелке, если воображаемый объект, проходя путь от первой вершине к второй, третьей и так далее до последней, а потом снова к первой, будет двигаться против часовой стрелки по внутренней стороне многоугольника. nmglFrontFace определяет, что пойдёт на лицевую сторону: многоугольники с оборотом по часовой стрелке в окне координат или многоугольники с оборотом против часовой стрелки в окне координат. Перевод команды GL_CCW в режим *mode* выводит многоугольники с оборотом против часовой стрелки на лицевую сторону; команда GL_CW выводит многоугольники с оборотом по часовой стрелке. По умолчанию на лицевую сторону выводятся многоугольники с поворотом против часовой стрелки.
 
 */
void nmglFrontFace (NMGLenum mode);

/*!
 \brief Умножает текущую матрицу на матрицу перспективы
 
 \param left,right [in] Определяет координаты для левой и правой вертикальных плоскостей отсечения.
 \param bottom, top [in] Определяет координаты для верхней и нижней горизонтальных плоскостей отсечения.
 \param zNear, zFar [in] Определяет расстояния для расположения плоскостей отсечения малой и большой глубины. Значения обоих расстояний должны быть положительными.

 \details nmglFrustum описывает матрицу перспективы, которая создаёт перспективную проекцию. Текущая матрица (см. [nmglMatrixMode](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMatrixMode.xml)) умножается на эту матрицу, и результат заменяет текущую матрицу, как если бы была применена команда [nmglMultMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMultMatrix.xml) с аргументом в виде следующей матрицы:

\f[
\begin{pmatrix}
\frac{2zNear}{right - left}  & 0 & A & 0 \\
0 & \frac{2zNear}{top - bottom} & B & 0 \\
0 & 0 & C & D \\
0 & 0 & -1 & 1 \\
\end{pmatrix}
\f]

\f[A = \frac{right + left}{right - left}\f]

\f[B = \frac{top + bottom}{top - bottom}\f]

\f[C = -\frac{zFar + zNear}{zFar - zNear}\f]

\f[D = -\frac{2zFarzNear}{zFar - zNear}\f]

Как правило, применяется режим работы с матрицами GL_PROJECTION, и операторы left bottom - nearVal and right top - nearVal определяют точки на ближней плоскости кадрирования, которые наносятся в нижний левый и верхний правый углы окна, если человеческий глаз находится на уровне (0, 0, 0). Оператор farVal определяет расположение дальней плоскости кадрирования. Значения *nearVal* и *farVal* должны быть положительными.

Следует применить [nmglPushMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml) и [nmglPopMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPopMatrix.xml) для сохранения и восстановления текущего стека матриц
 
 */
void nmglFrustumf (NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar);
NMGLuint nmglGenLists (NMGLsizei range);
void nmglGenTextures (NMGLsizei n, NMGLuint *textures);

/*!
 \brief Возвращает значение/значения выбранного параметра
 \param pname [in] Определяет значение параметра, которое должно быть возвращено. Допустимы символьные константы, представленные в списке ниже.
 \param params Возвращает значение/значения определённого параметра


 \details Эти четыре команды возвращают значения переменных простого состояния в GL. *pname* это символьная константа, обозначающая параметры состояния, которые должны быть восстановлены, и *params* это указатель на массив заданного типа, в котором требуется разместить восстановленные данные.

Конверсия типа происходит, если *params* имеет не тот тип, что значение параметра состояния, которое запрашивается. Если вызвана команда nmglGetBooleanv значение с плавающей запятой (или целое значение) конвертируется в GL_FALSE если и только если оно составляет 0,0 (или 0). В противном случае оно конвертируется в GL_TRUE. Если вызвана команда nmglGetIntegerv, логические значения возвращаются как GL_TRUE или GL_FALSE, и большинство значений с плавающей запятой округляются до ближайшего целого значения. Цвета и нормали, имеющие значения в виде числа с плавающей запятой, однако, возвращаются с помощью линейного преобразования, которое преобразует 1,0 в самое большое целое число а -1,0 в самое большое допустимое отрицательное целое число. Если вызвана команда nmglGetFloatv или nmglGetDoublev, логические значения возвращаются как GL_TRUE или GL_FALSE, и целые значения конвертируются в значения с плавающей запятой.

Для *pname*допустимы следующие символьные константы:

GL_ACTIVE_TEXTURE

> *params* возвращает одно значение, показывающее активный мультитекстурный блок. Начальное значение -- GL_TEXTURE0. См. [nmglActiveTexture](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glActiveTexture.xml).

GL_ALIASED_POINT_SIZE_RANGE

> *params* возвращает два значения, минимальный и максимальный поддерживаемые размеры несглаженных точек.

GL_ALIASED_LINE_WIDTH_RANGE

> *params* возвращает два значения, минимальная и максимальная ширина несглаженных линий.

GL_ALPHA_BITS

> *params* возвращает одно значение, количество битовых слоёв альфа в каждом цветовом буфере.

GL_ALPHA_TEST

> *params* возвращает одно логическое значение, показывающее, активирован ли первый этап тестирования фрагментов. Начальное значение -- GL_FALSE. См. [nmglAlphaFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glAlphaFunc.xml).

GL_ALPHA_TEST_FUNC *params* возвращает одно значение,

> символьное имя функции первого этапа тестирования. Изначальное значение -- GL_ALWAYS. См. [nmglAlphaFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glAlphaFunc.xml).

GL_ALPHA_TEST_REF

> *params* возвращает одно значение, ссылочное значение на результаты первого этапа тестирования. Изначальное значение -- 0. См. [nmglAlphaFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glAlphaFunc.xml). Если требуется целое значение, оно получается путём линейного преобразования из внутреннего представления в виде числа с плавающей запятой, например, 1,0 станет самым большим допустимым положительным целым числом, а -1,0 станет самым большим допустимым отрицательным целым числом.

GL_BLEND

> *params* возвращает одно логическое значение, показывающее, активирован ли плавный переход. Начальное значение -- GL_FALSE. См. [nmglBlendFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glBlendFunc.xml).

GL_BLUE_BITS

> *params* возвращает одно значение, количество синих битовых слоёв в каждом цветовом буфере.

GL_CLIENT_ACTIVE_TEXTURE

> *params* возвращает одно целое значение, показывающее активный мультитекстурный блок текущего клиента. Начальное значение -- GL_TEXTURE0. См. [nmglClientActiveTexture](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glClientActiveTexture.xml).

GL_COLOR_ARRAY

> *params* возвращает одно логическое значение, показывающее, активирован ли массив цветов. Начальное значение -- GL_FALSE. См. [nmglColorPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColorPointer.xml).

GL_COLOR_ARRAY_SIZE

> *params* возвращает одно значение, количество компонентов каждого цвета в массиве цветов Изначальное значение -- 4. См. [nmglColorPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColorPointer.xml).

GL_COLOR_ARRAY_STRIDE

> *params* возвращает одно значение, байтовое смещение между следующие друг за другом цветами в массиве цветов. Изначальное значение -- 0. См. [nmglColorPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColorPointer.xml).

GL_COLOR_ARRAY_TYPE

> *params* возвращает одно значение, тип данных каждого компонента в массиве цветов. Начальное значение -- GL_FLOAT. См. [nmglColorPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColorPointer.xml).

GL_COLOR_CLEAR_VALUE

> *params* возвращает четыре значения: уровни красного, зелёного, синего и альфа-канала, применяемые для очистки буферов цвета. Если требуется, целые значения образуются в результате последовательного преобразования из внутренних форм представления с плавающей запятой, так, что 1,0 станет максимальным допустимым целым числом, а -1,0 станет максимальным допустимым отрицательным целым числом. Изначальное значение -- (0, 0, 0, 0). См. [nmglClearColor](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glClearColor.xml).

GL_COLOR_MATERIAL

> *params* возвращает одно логическое значение, показывающее, зафиксирован ли в одном или нескольких параметрах материала текущий цвет. Начальное значение -- GL_FALSE. См. [nmglColorMaterial](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColorMaterial.xml).

GL_COLOR_WRITEMASK

> *params* возвращает четыре логических значения: уровни красного, зелёного, синего и альфа-канала, разрешённые для записи в буфера цвета. Изначальное значение: (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE). См. [nmglColorMask](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColorMask.xml).

GL_CULL_FACE

> *params* возвращает одно логическое значение, показывающее, активирована ли отбраковка многоугольников. Начальное значение -- GL_FALSE. См. [nmglCullFace](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glCullFace.xml).

GL_CULL_FACE_MODE

> *params* возвращает одно значение, символьную константу, показывающую, какие поверхности многоугольника будут отбракованы. Начальное значение -- GL_BACK. См. [nmglCullFace](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glCullFace.xml).

GL_CURRENT_COLOR

> *params* возвращает четыре значения: уровни красного, зелёного, синего и альфа-канала у текущего цвета. Если требуется, целые значения образуются в результате последовательного преобразования из внутренних форм представления с плавающей запятой, так, что 1,0 станет максимальным допустимым целым числом, а -1,0 станет максимальным допустимым отрицательным целым числом. Изначальное значение -- (1, 1, 1, 1). См. [nmglColor](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColor.xml).

GL_CURRENT_INDEX

> *params* возвращает одно значение, текущий цветовой индекс. Изначальное значение -- 1. См. [nmglIndex](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glIndex.xml).

GL_CURRENT_NORMAL

> *params* возвращает три значения: координаты текущей нормали по осям *x*, *y*, и *z*. Если требуется, целые значения образуются в результате последовательного преобразования из внутренних форм представления с плавающей запятой, так, что 1,0 станет максимальным допустимым целым числом, а -1,0 станет максимальным допустимым отрицательным целым числом. Изначальное значение -- (0, 0, 0). См. [nmglNormal](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glNormal.xml).

GL_CURRENT_RASTER_TEXTURE_COORDS

> *params* возвращает четыре значения: текстурные координаты *s*, *t*, *r*, и *q* текущей позиции в двумерном массиве точек. Изначальное значение -- (0, 0, 0, 1). См. [nmglRasterPos](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glRasterPos.xml) и [nmglMultiTexCoord](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMultiTexCoord.xml).

GL_CURRENT_TEXTURE_COORDS

> *params* возвращает четыре значения: текстурные координаты *s*, *t*, *r*, и *q*. Изначальное значение -- (0, 0, 0, 1). См. [nmglMultiTexCoord](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMultiTexCoord.xml).

GL_DEPTH_BITS

> *params* возвращает одно значение, количество битовых слоёв в буфере глубины.

GL_DEPTH_CLEAR_VALUE

> *params* возвращает одно значение, то, которое используется для очистки буфера глубины. Если требуется, целые значения образуются в результате последовательного преобразования из внутренних форм представления с плавающей запятой, так, что 1,0 станет максимальным допустимым целым числом, а -1,0 станет максимальным допустимым отрицательным целым числом. Изначальное значение -- 1. См. [nmglClearDepth](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glClearDepth.xml).

GL_DEPTH_FUNC

> *params* возвращает одно значение, символьную константу, которая определяет функцию сравнения длины. Начальное значение -- GL_LESS. См. [nmglDepthFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDepthFunc.xml).

GL_DEPTH_RANGE

> *params* возвращает два значения: минимальный и максимальный пределы преобразования для буфера глубины. Если требуется, целые значения образуются в результате последовательного преобразования из внутренних форм представления с плавающей запятой, так, что 1,0 станет максимальным допустимым целым числом, а -1,0 станет максимальным допустимым отрицательным целым числом. Изначальное значение -- (0, 1). См. [nmglDepthRange](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDepthRange.xml).

GL_DEPTH_TEST

> *params* возвращает одно логическое значение, показывающее, активировано ли тестирование глубины фрагментов. Начальное значение -- GL_FALSE. См. [nmglDepthFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDepthFunc.xml) и [nmglDepthRange](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDepthRange.xml).

GL_DEPTH_WRITEMASK

> *params* возвращает одно логическое значение, показывающее, активен ли для записи буфер глубины. Изначальное значение: GL_TRUE. См. [nmglDepthMask](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDepthMask.xml).

GL_FRONT_FACE

> *params* возвращает одно значение, символьную константу, показывающую, какая сторона будет считаться лицевой: та, где многоугольники переворачиваются по часовой стрелке, или где против часовой. Начальное значение -- GL_CCW. См. [nmglFrontFace](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glFrontFace.xml).

GL_GREEN_BITS

> *params* возвращает одно значение, количество зелёных битовых слоёв в каждом цветовом буфере.

GL_LIGHT*i*

> *params* возвращает одно логическое значение, показывающее, активировано ли заданное освещение. Начальное значение -- GL_FALSE. См. [nmglLight](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLight.xml) и [nmglLightModel](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLightModel.xml).

GL_LIGHTING

> *params* возвращает одно логическое значение, активировано ли освещение. Начальное значение -- GL_FALSE. См. [nmglLightModel](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLightModel.xml).

GL_LIGHT_MODEL_AMBIENT

> *params* возвращает четыре значения: уровни красного, зелёного, синего и альфа-канала, применяемые относительно интенсивности стандартного отображения всей сцены. Если требуется, целые значения образуются в результате последовательного преобразования из внутренних форм представления с плавающей запятой, так, что 1,0 станет максимальным допустимым целым числом, а -1,0 станет максимальным допустимым отрицательным целым числом. Изначальное значение -- (0,2, 0,2, 0,2, 1,0). См. [nmglLightModel](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLightModel.xml).

GL_LINE_SMOOTH

> *params* возвращает одно логическое значение, показывающее, активировано ли сглаживание линий. Начальное значение -- GL_FALSE. См. [nmglLineWidth](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLineWidth.xml).

GL_LINE_SMOOTH_HINT

> *params* возвращает одно значение, символьную константу, показывающую режим сглаживающей тени для линии. Изначальное значение -- GL_DONT_CARE. См. [nmglHint](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glHint.xml).

GL_LINE_STIPPLE

> *params* возвращает одно логическое значение, показывающее, активировано ли отображение линий пунктиром. Изначальное значение -- GL_FALSE. См. [nmglLineStipple](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLineStipple.xml).

GL_LINE_STIPPLE_PATTERN

> *params* возвращает одно значение, 16-битный образец штриховых элементов линии. Все изначальные значения -- 1. См. [nmglLineStipple](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLineStipple.xml).

GL_LINE_STIPPLE_REPEAT

> *params* возвращает одно значение, фактор повтора пунктирных штрихов элементов линии. Изначальное значение -- 1. См. [nmglLineStipple](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLineStipple.xml).

GL_LINE_WIDTH

> *params* возвращает одно значение, ширина линии в соответствии с командой [nmglLineWidth](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLineWidth.xml). Изначальное значение -- 1.

GL_LINE_WIDTH_GRANULARITY

> *params* возвращает одно значение, разницу между двумя соседними допустимыми уровнями ширины сглаженных линий. См. [nmglLineWidth](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLineWidth.xml).

GL_LINE_WIDTH_RANGE

> *params* возвращает два значения: минимальная и максимальная ширина сглаженных линий. См. [nmglLineWidth](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLineWidth.xml).

GL_LIST_BASE

> *params* возвращает одно значение, базовое смещение, добавляемое ко всем именам в массивах, представленных для [nmglCallLists](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glCallLists.xml). Изначальное значение -- 0. См. [nmglListBase](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glListBase.xml).

GL_LIST_INDEX

> *params* возвращает единственное значение, имя дисплейного списка, который на данный момент в стадии разработки. Возвращается значение 0, если на текущий момент нет дисплейных списков в стадии разработки. Изначальное значение -- 0. См. [nmglNewList](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glNewList.xml).

GL_MATRIX_MODE

> *params* возвращает одно значение, символьную константу, показывающую, какой стек матриц на настоящий момент является объектом всех матричных операций. Изначальное значение -- GL_MODELVIEW. См. [nmglMatrixMode](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMatrixMode.xml).

GL_MAX_ELEMENTS_INDICES

> *params* возвращает одно значение, рекомендуемое максимальное количество индексов массива вершин. См. [nmglDrawRangeElements](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawRangeElements.xml).

GL_MAX_ELEMENTS_VERTICES

> *params* возвращает одно значение, рекомендуемое максимальное количество вершин в массиве. См. [nmglDrawRangeElements](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawRangeElements.xml).

GL_MAX_LIGHTS

> *params* возвращает одно значение, максимальное количество источников света. Оно должно составлять не менее 8. См. [nmglLight](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLight.xml).

GL_MAX_LIST_NESTING

> *params* возвращает единственное значение, максимальную глубину рекурсии, допустимую при просмотре дисплейного списка. Значение должно составлять не менее 64. См. [nmglCallList](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glCallList.xml).

GL_MAX_MODELVIEW_STACK_DEPTH

> *params* возвращает одно значение, максимальную поддерживаемую глубину стека матриц моделирования. Значение должно составлять не менее 32. См. [nmglPushMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml).

GL_MAX_PROJECTION_STACK_DEPTH

> *params* возвращает единственное значение, максимальную поддерживаемую глубину стека матриц проецирования. Значение должно составлять не менее 2. См. [nmglPushMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml).

GL_MAX_TEXTURE_SIZE

> *params* возвращает одно значение. Значение представляет собой приблизительную оценку максимальной текстуры, с которой может работать GL. Значение должно составлять не менее 64. Если версия GL -- 1.1 или выше, следует использовать GL_PROXY_TEXTURE_1D или GL_PROXY_TEXTURE_2D, чтобы определить, не слишком ли велика текстура. См. [nmglTexImage1D](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTexImage1D.xml) и [nmglTexImage2D](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTexImage2D.xml).

GL_MAX_TEXTURE_UNITS

> *params* возвращает одно значение, показывающее количество поддерживаемых условных текстурных блоков. Каждый условный текстурный блок включает набор текстурных координат и блок текстурных изображений. Условные текстурные блоки могут использоваться для визуализации с зафиксированной функцией (бесшейдерной визуализации) Значение должно составлять не менее 2. Доступ к дополнительным наборам координат текстуры и блокам текстурных изображений можно получить с вершин и шейдеров фрагментов. См. [nmglActiveTexture](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glActiveTexture.xml) и [nmglClientActiveTexture](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glClientActiveTexture.xml).

GL_MAX_VIEWPORT_DIMS

> *params* возвращает два значения: максимальные поддерживаемые уровни ширины и высоты поля видимости. Эти значения должны быть таковы, чтобы результативный размер был не меньше размера экрана, на который будет производиться отображение. См. [nmglViewport](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glViewport.xml).

GL_MODELVIEW_MATRIX

> *params* возвращает шестнадцать значений, связанных с матрицей моделирования, находящейся на вершине стека таких матриц. Изначально эта матрица является матрицей тождественности. См. [nmglPushMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml).

GL_MODELVIEW_STACK_DEPTH

> *params* возвращает одно значение, количество матриц в стеке матриц моделирования. Изначальное значение -- 1. См. [nmglPushMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml).

GL_NORMAL_ARRAY

> *params* возвращает одно логическое значение, показывающее, активирован ли массив нормалей. Изначальное значение -- GL_FALSE. См. [nmglNormalPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glNormalPointer.xml).

GL_NORMAL_ARRAY_STRIDE

> *params* возвращает одно значение, байтовое смещение между следующими друг за другом нормалями в массиве нормалей. Изначальное значение -- 0. См. [nmglNormalPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glNormalPointer.xml).

GL_NORMAL_ARRAY_TYPE

> *params* возвращает одно значение, тип данных каждой координаты в массиве нормалей. Изначальное значение -- GL_FLOAT. См. [nmglNormalPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glNormalPointer.xml).

GL_NORMALIZE

> *params* возвращает единичное логическое значение, показывающее, производится ли автоматическое приведение нормалей в соответствие с единичной величиной после того, как они были трансформированы в систему координат, разработанной в соответствии с позицией пользователя. Изначальное значение -- GL_FALSE. См. [nmglNormal](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glNormal.xml).

GL_PACK_ALIGNMENT

> *params* возвращает одно значение, выравнивание по границе байта, применяемое для записи пиксельных данных в память. Изначальное значение -- 4. См. [nmglPixelStore](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPixelStore.xml).

GL_POINT_SIZE

> *params* возвращает одно значение, размер точки, установленный командой [nmglPointSize](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPointSize.xml). Изначальное значение -- 1.

GL_POINT_SIZE_GRANULARITY

> *params* возвращает одно значение, разницу между двумя соседними допустимыми размерами сглаженных точек. См. [nmglPointSize](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPointSize.xml).

GL_POINT_SIZE_RANGE

> *params* возвращает два значения: минимальный и максимальный поддерживаемые размеры сглаженных точек. Самый малый размер должен быть не более 1, и самый большой размер должен быть не менее 1. См. [nmglPointSize](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPointSize.xml).

GL_POINT_SMOOTH

> *params* возвращает одно логическое значение, показывающее, активировано ли сглаживание точек. Изначальное значение -- GL_FALSE. См. [nmglPointSize](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPointSize.xml).

GL_POINT_SMOOTH_HINT

> *params* возвращает одно значение, символьную константу, показывающую режим сглаживающей тени для точки. Изначальное значение -- GL_DONT_CARE. См. [nmglHint](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glHint.xml).

GL_POLYGON_OFFSET_FACTOR

> *params* возвращает одно значение, коэффициент масштабирования, применяемый для того, чтобы определить варьируемое изменение глубины каждого фрагмента, возникающего при разделении многоугольников на части. Изначальное значение -- 0. См. [nmglPolygonOffset](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPolygonOffset.xml).

GL_POLYGON_OFFSET_UNITS

> *params* возвращает одно значение. Это значение умножается на определяемое вариантом исполнения GL значение и затем добавляется к значению глубины каждого фрагмента, генерируемого при разделении многоугольника на части. Изначальное значение -- 0. См. [nmglPolygonOffset](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPolygonOffset.xml).

GL_POLYGON_OFFSET_FILL

> *params* возвращает одно логическое значение, показывающее, активировано ли смещение для многоугольников в режиме заливки. Изначальное значение -- GL_FALSE. См. [nmglPolygonOffset](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPolygonOffset.xml).

GL_POLYGON_STIPPLE

> *params* возвращает одно логическое значение, показывающее, активировано ли пунктирное отображение многоугольников. Изначальное значение -- GL_FALSE. См. [nmglPolygonStipple](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPolygonStipple.xml).

GL_PROJECTION_MATRIX

> *params* возвращает шестнадцать значений, связанных с матрицей проецирования, находящейся на вершине стека таких матриц. Изначально эта матрица является матрицей тождественности. См. [nmglPushMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml).

GL_PROJECTION_STACK_DEPTH

> *params* возвращает одно значение, количество матриц в стеке матриц проецирования. Изначальное значение -- 1. См. [nmglPushMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml).

GL_RED_BITS

> *params* возвращает одно значение, количество красных битовых слоёв в каждом цветовом буфере.

GL_RESCALE_NORMAL

> *params* возвращает одно логическое значение, показывающее, активировано ли изменение масштаба нормалей. См. [nmglEnable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnable.xml).

GL_SCISSOR_BOX

> *params* возвращает четыре значения: координаты вершин обрезаемого фрагмента по осям x и y , потом ширина и высота этого фрагмента. Изначально значения координат вершин окна по осям x и y будут по нулям, а значения ширины и высоты будут соответствовать размерам окна. См. [nmglScissor](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glScissor.xml).

GL_SCISSOR_TEST

> *params* возвращает одно логическое значение, показывающее, активировано ли разрезание. Изначальное значение -- GL_FALSE. См. [nmglScissor](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glScissor.xml).

GL_SHADE_MODEL

> *params* возвращает одно значение, символьную константу, показывающую, какой установлен режим шейдинга: плоскостной или плавный. Изначальное значение -- GL_SMOOTH. См. [nmglShadeModel](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glShadeModel.xml).

GL_SMOOTH_LINE_WIDTH_RANGE

> *params* возвращает два значения, минимальная и максимальная ширина сглаженных линий. См. [nmglLineWidth](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLineWidth.xml).

GL_SMOOTH_LINE_WIDTH_GRANULARITY

> *params* возвращает одно значение -- разрешающую способность для сглаженных линий по ширине. См. [nmglLineWidth](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLineWidth.xml).

GL_SMOOTH_POINT_SIZE_RANGE

> *params* возвращает два значения: минимальный и максимальный поддерживаемые уровни ширины сглаженных точек. См. [nmglPointSize](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPointSize.xml).

GL_SMOOTH_POINT_SIZE_GRANULARITY

> *params* возвращает одно значение, разрешающую способность для сглаженных точек по размеру. См. [nmglPointSize](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPointSize.xml).

GL_STENCIL_BITS

> *params* возвращает одно значение, количество битовых слоёв в буфере шаблонов.

GL_STENCIL_CLEAR_VALUE

> *params* возвращает одно значение, индекс, по которому битовые слои шаблонов очищаются. Изначальное значение -- 0. См. [nmglClearStencil](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glClearStencil.xml).

GL_STENCIL_FAIL

> *params* возвращает одно значение, символьную константу, показывающую, какое действие предпринимается, если тест шаблона завершён неудачей. Изначальное значение -- GL_KEEP. См. [nmglStencilOp](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilOp.xml). Если версия GL -- 2 или выше, возникающее в итоге состояние шаблона скажется только на фигурах, не являющиеся многоугольниками, и на многоугольниках лицевой стороны. Для многоугольников задней стороны предусмотрено другое состояние шаблона См. [nmglStencilOpSeparate](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilOpSeparate.xml).

GL_STENCIL_FUNC

> *params* возвращает одно значение, символьную константу, показывающую, какая функция используется для сравнения исходного значения шаблона с буферным. Изначальное значение -- GL_ALWAYS. См. [nmglStencilFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilFunc.xml). Если версия GL -- 2 или выше, возникающее в итоге состояние шаблона скажется только на фигурах, не являющиеся многоугольниками, и на многоугольниках лицевой стороны. Для многоугольников задней стороны предусмотрено другое состояние шаблона См. [nmglStencilFuncSeparate](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilFuncSeparate.xml).

GL_STENCIL_PASS_DEPTH_FAIL

> *params* возвращает одно значение, символьную константу, показывающую, какое действие предпринимается, если тест шаблона завершён успешно, но тест глубины завершён неудачей. Изначальное значение -- GL_KEEP. См. [nmglStencilOp](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilOp.xml). Если версия GL -- 2 или выше, возникающее в итоге состояние шаблона скажется только на фигурах, не являющиеся многоугольниками, и на многоугольниках лицевой стороны. Для многоугольников задней стороны предусмотрено другое состояние шаблона См. [nmglStencilOpSeparate](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilOpSeparate.xml).

GL_STENCIL_PASS_DEPTH_PASS

> *params* возвращает одно значение, символьную константу, показывающую, какое действие предпринимается, если тест шаблона и тест глубины завершены успешно. Изначальное значение -- GL_KEEP. См. [nmglStencilOp](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilOp.xml). Если версия GL -- 2 или выше, возникающее в итоге состояние шаблона скажется только на фигурах, не являющиеся многоугольниками, и на многоугольниках лицевой стороны. Для многоугольников задней стороны предусмотрено другое состояние шаблона См. [nmglStencilOpSeparate](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilOpSeparate.xml).

GL_STENCIL_REF

> *params* возвращает одно значение, то исходное значение, которое сравнивается со значением, содержащимся в буфере шаблонов. Изначальное значение -- 0. См. [nmglStencilFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilFunc.xml). Если версия GL -- 2 или выше, возникающее в итоге состояние шаблона скажется только на фигурах, не являющиеся многоугольниками, и на многоугольниках лицевой стороны. Для многоугольников задней стороны предусмотрено другое состояние шаблона См. [nmglStencilFuncSeparate](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilFuncSeparate.xml).

GL_STENCIL_TEST

> *params* возвращает одно логическое значение, показывающее, активировано ли тестирование шаблонов фрагментов. Изначальное значение -- GL_FALSE. См. [nmglStencilFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilFunc.xml) и [nmglStencilOp](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilOp.xml).

GL_STENCIL_VALUE_MASK

> *params* возвращает одно значение, маску, которая применяется и к исходном, и к буферному значениям шаблона перед их сравнением. Все изначальные значения -- 1. См. [nmglStencilFunc](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilFunc.xml). Если версия GL -- 2 или выше, возникающее в итоге состояние шаблона скажется только на фигурах, не являющиеся многоугольниками, и на многоугольниках лицевой стороны. Для многоугольников задней стороны предусмотрено другое состояние шаблона См. [nmglStencilFuncSeparate](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilFuncSeparate.xml).

GL_STENCIL_WRITEMASK

> *params* возвращает одно значение, маску, которая контролирует запись битовых слоёв шаблона. Все изначальные значения -- 1. См. [nmglStencilMask](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilMask.xml). Если версия GL -- 2 или выше, возникающее в итоге состояние шаблона скажется только на фигурах, не являющиеся многоугольниками, и на многоугольниках лицевой стороны. Для многоугольников задней стороны предусмотрено другое состояние шаблона См. [nmglStencilMaskSeparate](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilMaskSeparate.xml).

GL_SUBPIXEL_BITS

> *params* возвращает одно значение -- предварительный подсчёт количества битов субпиксельного разрешения, которые используются для определения координат в окне для расположения геометрических элементов раздробленных объектов. Значение должно составлять не менее 4.

GL_TEXTURE_2D

> *params* возвращает одно логическое значение, показывающее, активировано ли отображение текстуры 2D. Изначальное значение -- GL_FALSE. См. [nmglTexImage2D](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTexImage2D.xml).

GL_TEXTURE_BINDING_2D

> *params* returns a single value, the name of the texture currently bound to the target GL_TEXTURE_2D. Изначальное значение -- 0. См. [nmglBindTexture](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glBindTexture.xml).

GL_TEXTURE_COORD_ARRAY

> *params* возвращает одно логическое значение показывающее, активирован ли массив текстурных координат. Изначальное значение -- GL_FALSE. См. [nmglTexCoordPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTexCoordPointer.xml).

GL_TEXTURE_COORD_ARRAY_SIZE

> *params* возвращает одно значение, значение координат каждого элемента в массиве текстурных координат. Изначальное значение -- 4. См. [nmglTexCoordPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTexCoordPointer.xml).

GL_TEXTURE_COORD_ARRAY_STRIDE

> *params* возвращает одно значение, байтовое смещение между следующими друг за другом элементами в массиве текстурных координат. Изначальное значение -- 0. См. [nmglTexCoordPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTexCoordPointer.xml).

GL_TEXTURE_COORD_ARRAY_TYPE

> *params* возвращает одно значение, тип данных координат в массиве текстурных координат. Изначальное значение -- GL_FLOAT. См. [nmglTexCoordPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTexCoordPointer.xml).

GL_UNPACK_ALIGNMENT

> *params* возвращает одно значение, выравнивание по границе байта, применяемое для считывания пиксельных данных из памяти. Изначальное значение -- 4. См. [nmglPixelStore](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPixelStore.xml).

GL_UNPACK_ROW_LENGTH

> *params* возвращает одно значение, длину строки, используемую для считывания пиксельных данных из памяти. Изначальное значение -- 0. См. [nmglPixelStore](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPixelStore.xml).

GL_VERTEX_ARRAY

> *params* возвращает одно логическое значение показывающее, активирован ли массив вершин. Изначальное значение -- GL_FALSE. См. [nmglVertexPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glVertexPointer.xml).

GL_VERTEX_ARRAY_SIZE

> *params* возвращает одно значение, количество координат каждой вершины в массиве вершин. Изначальное значение -- 4. См. [nmglVertexPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glVertexPointer.xml).

GL_VERTEX_ARRAY_STRIDE

> *params* возвращает одно значение, байтовое смещение между следующими друг за другом вершинами в массиве вершин. Изначальное значение -- 0. См. [nmglVertexPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glVertexPointer.xml).

GL_VERTEX_ARRAY_TYPE

> *params* возвращает одно значение, тип данных каждой координаты в массиве вершин. Изначальное значение -- GL_FLOAT. См. [nmglVertexPointer](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glVertexPointer.xml).

GL_VERTEX_PROGRAM_POINT_SIZE

> *params* возвращает одно логическое значение, показывающее, активирован ли программный режим определения размера точки вершины. Когда данная команда активирована, и шейдер вершины действует, размер точки берётся из встроенного в шейдер оператора nmgl_PointSize. Когда данная команда деактивирована и шейдер вершины действует, размер точки соответствует состоянию точки, определённому командой [nmglPointSize](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPointSize.xml). Начальное значение -- GL_FALSE.

GL_VIEWPORT

> *params* возвращает четыре значения: координаты вершин поля видимости по осям x и y , потом ширина и высота поля видимости. Изначально значения координат вершин окна по осям x и y будут по нулям, а значения ширины и высоты будут соответствовать ширине и высоте окна, в котором GL будет осуществлять визуализацию. См. [nmglViewport](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glViewport.xml).
 
 */
 //! \{
void nmglGetBooleanv (NMGLenum pname, NMGLboolean *params);
void nmglGetIntegerv (NMGLenum pname, NMGLint *params);
void nmglGetFloatv (NMGLenum pname, NMGLfloat *params);
 //! \}
 
 NMGLenum nmglGetError (void);
 
 /*!
 \brief Функция предназначена для получения указателя на массив текселей палитры активного текстурного объекта.
 \param target определяет тип текстуры, для которой задаётся палитра. Единственное поддерживаемое значение: NMGL_TEXTURE_2D.
 \param format определяет количество компонент в одном текселе. Единственное поддерживаемое значение: NMGL_RGBA.
 \param type определяет формат представления компонент текселя. Единственное поддерживаемое значение - NMGL_UNSIGNED_BYTE.
 \param data  указатель на массив данных текселей палитры.


 \details 
 */
void nmglGetColorTableEXT (NMGLenum target, NMGLenum format, NMGLenum type, NMGLvoid *data);

/*!
 **  \brief Функция предназначена для чтения характеристик реализации расширения EXT_paletted_texture.
 \param target определяет тип текстуры, для которой задаётся палитра. Единственное поддерживаемое значение: NMGL_TEXTURE_2D.
 \param pname код интересующей характеристики
 \param params  указатель на массив переменных для хранения считанных значений характеристик.


 \details 
 
 */
void nmglGetColorTableParameterivEXT (NMGLenum target, NMGLenum pname, NMGLint *params);
void nmglGetLightfv (NMGLenum light, NMGLenum pname, NMGLfloat *params);
void nmglGetMaterialfv (NMGLenum face, NMGLenum pname, NMGLfloat *params);
void nmglGetPointerv (NMGLenum pname, NMGLvoid * *params);
void nmglGetPolygonStipple (NMGLubyte *mask);

/*!
 \brief Функция nmglGetTexEnvfv предназначена для получения текущих значений параметров текстурного окружения. Данная функция используется для получения текущего значения параметра NMGL_TEXTURE_ENV_COLOR, определяющего цвет текстурного окружения.
 \param env данный аргумент должен принимать значение NMGL_TEXTURE_ENV.
 \param pname символическая константа, определяющая наименование параметра, значение которого требуется получить. Данный аргумент может принимать единственное значение NMGL_TEXTURE_ENV_COLOR.
 \param params указатель на область памяти, в которую необходимо поместить значение считываемого параметра.


 \details 
 
 */
void nmglGetTexEnvfv (NMGLenum target, NMGLenum pname, NMGLfloat *params);
/*!
 \brief Функция nmglGetTexEnviv предназначена для получения текущих значений параметров текстурного окружения. Данная функция используется для получения текущего значения параметра NMGL_TEXTURE_ENV_MODE, определяющего вид функции текстурирования.
 \param env данный аргумент должен принимать значение NMGL_TEXTURE_ENV.
 \param pname символическая константа, определяющая наименование параметра, значение которого требуется получить. Данный аргумент может принимать единственное значение NMGL_TEXTURE_ENV_MODE.
 \param params указатель на область памяти, в которую необходимо поместить значение считываемого параметра.


 \details 
 
 */
void nmglGetTexEnviv (NMGLenum target, NMGLenum pname, NMGLint *params);

/*!
 \brief Функция nmglGetTexParameteriv предназначена для получения текущих значений параметров текстурирования. Данная функция используется для получения текущих значений режимов обработки границ и параметров фильтрации для двумерных текстур.
 \param target данный аргумент определяет тип текущего привязанного текстурного объекта и должен принимать значение NMGL_TEXTURE_2D.
 \param pname символическая константа, определяющая наименование параметра, значение которого требуется получить. Возможные значения:NMGL_TEXTURE_MIN_FILTER, NMGL_TEXTURE_MAG_FILTER, NMGL_TEXTURE_WRAP_S, NMGL_TEXTURE_WRAP_T.
 \param params указатель на область памяти, в которую необходимо поместить значение считываемого параметра текстурирования.


 \details 
 
 */
void nmglGetTexParameteriv (NMGLenum target, NMGLenum pname, NMGLint *params);
const NMGLubyte * nmglGetString (NMGLenum name);

 /*!
 \brief Позволяет задать режим интерполяции, режим сглаживания точек и режим сглаживания линий.
 
 \param target [in] Определяет режим, подлежащий настройке. Возможные варианты: GL_PERSPECTIVE_CORRECTION_HINT (режим интерполяции), GL_LINE_SMOOTH_HINT (режим сглаживания линий), и GL_POINT_SMOOTH_HINT (режим сглаживания точек).
 \param mode [in] Определяет значение режима. Должен иметь одно из следующих значений: GL_NICEST, GL_FASTEST, GL_DONT_CARE.


 \details Функция nmglHint имеет два аргумента. Первый, *target*, определяет, какой режим будет настраиваться: GL_PERSPECTIVE_CORRECTION_HINT, GL_LINE_SMOOTH_HINT, GL_POINT_SMOOTH_HINT. Второй, *mode*, определяет, само значение режима, определяющее те или иные вычисления:

GL_NICEST

> При target = GL_PERSPECTIVE_CORRECTION_HINT - включает перспективно-корректную интерполяцию. Перспективно-корректная интерполяция выполняется, например, при триангуляции (разбиении треугольников) или текстурировании.

GL_FASTEST

> При target = GL_PERSPECTIVE_CORRECTION_HINT - выключает перспективно-корректную интерполяцию. 

GL_DONT_CARE

> При target = GL_PERSPECTIVE_CORRECTION_HINT - выключает перспективно-корректную интерполяцию. 

 */
void nmglHint (NMGLenum target, NMGLenum mode);

NMGLboolean nmglIsEnabled (NMGLenum cap);

/*!
 \brief Устанавливает параметры источника освещения
 
 \param light [in] Определяет свет. Количество источников света зависит от варианта исполнения GL, но поддерживается не менее 8 источников. Они определяются символьными именами по форме GL_LIGHT i, где i варьируется от 0 до значения GL_MAX_LIGHTS - 1.
 \param pname [in] Определяет единственное значение параметра источника света для *light*. Допустимы: GL_SPOT_EXPONENT, GL_SPOT_CUTOFF, GL_CONSTANT_ATTENUATION, GL_LINEAR_ATTENUATION, и GL_QUADRATIC_ATTENUATION.
 \param params [in] Определяет значение параметра *pname*, которое будет установлено для источника света *light*.


 \details nmglLight устанавливает значения параметров отдельного источника света. *light* даёт наименование свету и представляет собой имя в символьной форме GL_LIGHT i, где i варьируется от 0 до значения GL_MAX_LIGHTS - 1. *Pname* определяет один из десяти параметров источника света, снова по символьному имени. *params* это либо единое значение, либо указатель на массив, содержащий новые значения.

Чтобы активировать и деактивировать расчёт освещения, следует применить [nmglEnable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnable.xml) и [nmglDisable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisable.xml) с аргументом GL_LIGHTING. Изначально освещение деактивировано. Когда оно активировано, активированные источники света учитываются в расчёте освещения. Чтобы активировать и деактивировать источник освещения i , следует применить [nmglEnable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnable.xml) и [nmglDisable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisable.xml) с аргументом GL_LIGHT i.

Предусмотрено следующие 10 параметров света:

GL_AMBIENT

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют интенсивность стандартного освещения в RGBA. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальная интенсивность стандартного освещения -- (0, 0, 0, 1).

GL_DIFFUSE

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют интенсивность рассеянности света в RGBA. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальное значение для GL_LIGHT0 -- (1, 1, 1, 1); для других освещений -- (0, 0, 0, 1).

GL_SPECULAR

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют интенсивность зеркального освещения в RGBA. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальное значение для GL_LIGHT0 -- (1, 1, 1, 1); для других освещений -- (0, 0, 0, 1).

GL_POSITION

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют координаты источника света в гомогенном объекте. Целые значения и значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются.
>
> Позиция трансформируется матрицей моделирования, когда вызвана команда nmglLight (как если бы это была точка), и сохраняется в системе координат, разработанной в соответствии с позицией пользователя. Если у позиции компонент «w» равен 0, свет обрабатывается как источник направленного излучения. Расчёт рассеянного и зеркального освещения учитывает направление света, но не текущее положение его источника, и затухание света деактивируется. Вместо этого расчёт рассеянного или зеркального освещения может быть основан на текущем расположении источника света по системе координат, разработанной в соответствии с позицией пользователя, и затухание активировано. Изначальная позиция -- (0, 0, 1, 0); таким образом, для изначального источника света будет определено направление, он будет параллелен оси z и направлен в её сторону.
 
 */
void nmglLightf (NMGLenum light, NMGLenum pname, const NMGLfloat params);


/*!
 \brief Устанавливает параметры источника освещения
 
 \param light [in] Определяет свет. Количество источников света зависит от варианта исполнения GL, но поддерживается не менее 8 источников. Они определяются символьными именами по форме GL_LIGHT i, где i варьируется от 0 до значения GL_MAX_LIGHTS - 1.
 \param pname [in] Определяет значение параметра источника света для *light*. Допустимы GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR, GL_POSITION, GL_SPOT_CUTOFF, GL_SPOT_DIRECTION, GL_SPOT_EXPONENT, GL_CONSTANT_ATTENUATION, GL_LINEAR_ATTENUATION, и GL_QUADRATIC_ATTENUATION.
 \param params [in] Определяет указатель значения или совокупности значений параметра *pname*, которое будет установлено для источника света *light*.


 \details nmglLight устанавливает значения параметров отдельного источника света. *light* даёт наименование свету и представляет собой имя в символьной форме GL_LIGHT i, где i варьируется от 0 до значения GL_MAX_LIGHTS - 1. *Pname* определяет один из десяти параметров источника света, снова по символьному имени. *params* это либо единое значение, либо указатель на массив, содержащий новые значения.

Чтобы активировать и деактивировать расчёт освещения, следует применить [nmglEnable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnable.xml) и [nmglDisable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisable.xml) с аргументом GL_LIGHTING. Изначально освещение деактивировано. Когда оно активировано, активированные источники света учитываются в расчёте освещения. Чтобы активировать и деактивировать источник освещения i , следует применить [nmglEnable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnable.xml) и [nmglDisable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisable.xml) с аргументом GL_LIGHT i.

Предусмотрено следующие 10 параметров света:

GL_AMBIENT

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют интенсивность стандартного освещения в RGBA. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальная интенсивность стандартного освещения -- (0, 0, 0, 1).

GL_DIFFUSE

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют интенсивность рассеянности света в RGBA. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальное значение для GL_LIGHT0 -- (1, 1, 1, 1); для других освещений -- (0, 0, 0, 1).

GL_SPECULAR

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют интенсивность зеркального освещения в RGBA. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальное значение для GL_LIGHT0 -- (1, 1, 1, 1); для других освещений -- (0, 0, 0, 1).

GL_POSITION

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют координаты источника света в гомогенном объекте. Целые значения и значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются.
>
> Позиция трансформируется матрицей моделирования, когда вызвана команда nmglLight (как если бы это была точка), и сохраняется в системе координат, разработанной в соответствии с позицией пользователя. Если у позиции компонент «w» равен 0, свет обрабатывается как источник направленного излучения. Расчёт рассеянного и зеркального освещения учитывает направление света, но не текущее положение его источника, и затухание света деактивируется. Вместо этого расчёт рассеянного или зеркального освещения может быть основан на текущем расположении источника света по системе координат, разработанной в соответствии с позицией пользователя, и затухание активировано. Изначальная позиция -- (0, 0, 1, 0); таким образом, для изначального источника света будет определено направление, он будет параллелен оси z и направлен в её сторону.
 
 */
void nmglLightfv (NMGLenum light, NMGLenum pname, const NMGLfloat *params);
void nmglLightModelfv (NMGLenum pname, const NMGLfloat *params);
void nmglLineStipple (NMGLint factor, NMGLushort pattern);
void nmglLineWidth (NMGLfloat width);
void nmglListBase (NMGLuint base);

/*!
 \brief Заменяет текущую матрицу матрицей тождественности
 


 \details nmglLoadIdentity Заменяет текущую матрицу матрицей тождественности. Это семантически эквивалентно вызову команды [nmglLoadMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLoadMatrix.xml) с матрицей тождественности

\f[
\begin{pmatrix}
1 & 0 & 0 & x \\
0 & 1 & 0 & y \\
0 & 0 & 1 & z \\
0 & 0 & 0 & 1 \\
\end{pmatrix}
\f]

Но в некоторых случаях это более эффективно.
 
 */
void nmglLoadIdentity (void);

/*!
 \brief Заменяет текущую матрицу выбранной
 
 \param m [in] станавливает указатель для 16 следующих друг за другом значений, которые используются как элементы основной матрицы, имеющей 4x4 столбцов.


 \details nmglLoadMatrix заменяет текущую матрицу другой, элементы которой устанавливаются параметром m. Текущая матрица -- это матрица проецирования, матрица моделирования или текстурная матрица, в зависимости от активного режима работы с матрицами (см. [nmglMatrixMode](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMatrixMode.xml)).

Текущая матрица, М, определяет трансформацию координат. Рассмотрим принцип действия М-матрицы моделирования. Если \f$ v = (v[0],v[1],v[2],v[3]) \f$  это набор координат вершин, и m указывает на массив 16-символьных значений с плавающей запятой, одинарной или двойной точности: \f$ m = {m[0], m[1] ... m[15]} \f$  , тогда преобразование иллюстрации модели \f$M(v)\f$ будет следующим:

\f[M v =
\begin{pmatrix}
m[0] & m[4] & m[8]  & m[12] \\
m[1] & m[5] & m[9]  & m[13] \\
m[2] & m[6] & m[10] & m[14] \\
m[3] & m[7] & m[11] & m[15] \\
\end{pmatrix} 
\begin{pmatrix}
v[0] \\
v[1] \\
v[2] \\
v[3] \\
\end{pmatrix}
\f]
Проекционные и текстурные трансформации определяются аналогично.
 
 */
void nmglLoadMatrixf (const NMGLfloat *m);

/*!
 \brief Определяет параметры материала для модели распространения света.
 
 \param face [in] Определяет, какая поверхность будет обновлена: лицевая или задняя. Следует выбрать один из двух вариантов:GL_FRONT (передняя), GL_BACK (задняя), и GL_FRONT_AND_BACK (передняя и задняя).
 \param pname [in] Определяет единственное значение, устанавливающее материал для лицевой или задней поверхности, которая будет обновлена. Должно быть GL_SHININESS.
 \param param [in] Определяет значение параметра GL_SHININESS.


 \details nmglMaterial задаёт значения параметрам материала. Предусмотрено два подобранных комплекта параметров материала. Первый набор, *front-facing*, применяется для окраски точек, линий, точечных изображений и многоугольников (когда двустороннее освещение деактивировано), или для правильных лицевых многоугольников (когда двустороннее освещение активировано). Второй набор, *back-facing*, используется для окраски многоугольников задней поверхности, только когда применено двустороннее освещение. См. справочную страницу [nmglLightModel](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLightModel.xml) для получения расширенной информации о расчёте одностороннего и двустороннего освещения.

Функция nmglMaterial  имеет три аргумента. Первый *face*, определяет, какой материал будет модифицироваться: GL_FRONT, GL_BACK или GL_FRONT_AND_BACK. Второй, *pname*, определяет, какой из нескольких параметров одного или обоих наборов будет модифицирован. Третий, *params*, определяет, какое значение/значения будет присвоено модифицируемому параметру.

Параметры материала используются для балансировки света, которая опционально применяется к каждой вершине. Эта балансировка обсуждается на справочной странице [nmglLightModel](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLightModel.xml). Данные параметры могут быть определены с помощью команды nmglMaterial, и их интерпретации относительно балансировки света будут следующими:

GL_AMBIENT

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют стандартное RGBA-отображение материала. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальное стандартное отображение материалов лицевой или задней стороны определяется значениями (0,2; 0,2; 0,2; 1,0).

GL_DIFFUSE

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют рассеянное RGBA-отображение материала. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальное рассеянное отображение материалов лицевой или задней стороны определяется значениями (0,8; 0,8; 0,8; 1,0).

GL_SPECULAR

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют зеркальное RGBA-отображение материала. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальное зеркальное отображение материалов лицевой или задней стороны определяется значениями (0, 0, 0, 1).

GL_EMISSION

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют RGBA-отображение интенсивности света, излучаемого материалом. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальное отображение интенсивности света, излучаемого материалами лицевой или задней стороны определяется значениями (0, 0, 0, 1).

GL_SHININESS

> *params* это одно целое или с плавающей запятой значение, которое определяет степень зеркального отражения материала в RGBA. Целые значения и значения с плавающей запятой преобразуются напрямую. Допустимы значения только в диапазоне 0 -- 128. Изначальная степень зеркального отражения материалов лицевой или задней стороны определяется значением 0.

GL_AMBIENT_AND_DIFFUSE

> Равносильно двукратному вызову команды nmglMaterial с прежними параметрами, однократному её вызову со значением GL_AMBIENT и одному вызову этой команды со значением GL_DIFFUSE.

GL_COLOR_INDEXES

> *params* содержит три целых или с плавающей запятой значения, которые определяют цветовые индексы стандартного, рассеянного, зеркального освещений. Эти три значения и GL_SHININESS, являются единственными показателями материала, используемыми для балансировки света в режиме цветового индекса. См. справочную страницу [nmglLightModel](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLightModel.xml) для получения расширенной информации о цветовом индексе освещения.
 
 */
void nmglMaterialf (NMGLenum face, NMGLenum pname, NMGLfloat param);

 /*!
 \brief Определяет параметры материала для модели распространения света.
 
 \param face [in] Определяет, какая поверхность будет обновлена: лицевая или задняя. Следует выбрать один из двух вариантов:GL_FRONT (передняя), GL_BACK (задняя), и GL_FRONT_AND_BACK (передняя и задняя).
 \param pname [in] Определяет параметр материала обновляемой поверхности/поверхностей. Должен иметь одно из следующих значений: GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR, GL_EMISSION, GL_SHININESS, GL_AMBIENT_AND_DIFFUSE, GL_COLOR_INDEXES.
 \param param [in] Определяет указатель значения или совокупности значений параметра *pname*.


 \details nmglMaterial задаёт значения параметрам материала. Предусмотрено два подобранных комплекта параметров материала. Первый набор, *front-facing*, применяется для окраски точек, линий, точечных изображений и многоугольников (когда двустороннее освещение деактивировано), или для правильных лицевых многоугольников (когда двустороннее освещение активировано). Второй набор, *back-facing*, используется для окраски многоугольников задней поверхности, только когда применено двустороннее освещение. См. справочную страницу [nmglLightModel](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLightModel.xml) для получения расширенной информации о расчёте одностороннего и двустороннего освещения.

Функция nmglMaterial  имеет три аргумента. Первый *face*, определяет, какой материал будет модифицироваться: GL_FRONT, GL_BACK или GL_FRONT_AND_BACK. Второй, *pname*, определяет, какой из нескольких параметров одного или обоих наборов будет модифицирован. Третий, *params*, определяет, какое значение/значения будет присвоено модифицируемому параметру.

Параметры материала используются для балансировки света, которая опционально применяется к каждой вершине. Эта балансировка обсуждается на справочной странице [nmglLightModel](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLightModel.xml). Данные параметры могут быть определены с помощью команды nmglMaterial, и их интерпретации относительно балансировки света будут следующими:

GL_AMBIENT

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют стандартное RGBA-отображение материала. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальное стандартное отображение материалов лицевой или задней стороны определяется значениями (0,2; 0,2; 0,2; 1,0).

GL_DIFFUSE

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют рассеянное RGBA-отображение материала. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальное рассеянное отображение материалов лицевой или задней стороны определяется значениями (0,8; 0,8; 0,8; 1,0).

GL_SPECULAR

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют зеркальное RGBA-отображение материала. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальное зеркальное отображение материалов лицевой или задней стороны определяется значениями (0, 0, 0, 1).

GL_EMISSION

> *params* содержит четыре целых или с плавающей запятой значений, которые определяют RGBA-отображение интенсивности света, излучаемого материалом. Целые значения линейно преобразуются так, чтобы максимальное положительное из отображаемых значений было преобразовано в 1,0, а максимальное отрицательное из них -- в -1,0. Значения с плавающей запятой преобразуются напрямую. Ни целое значение, ни значение с плавающей запятой не фиксируются. Изначальное отображение интенсивности света, излучаемого материалами лицевой или задней стороны определяется значениями (0, 0, 0, 1).

GL_SHININESS

> *params* это одно целое или с плавающей запятой значение, которое определяет степень зеркального отражения материала в RGBA. Целые значения и значения с плавающей запятой преобразуются напрямую. Допустимы значения только в диапазоне 0 -- 128. Изначальная степень зеркального отражения материалов лицевой или задней стороны определяется значением 0.

GL_AMBIENT_AND_DIFFUSE

> Равносильно двукратному вызову команды nmglMaterial с прежними параметрами, однократному её вызову со значением GL_AMBIENT и одному вызову этой команды со значением GL_DIFFUSE.

GL_COLOR_INDEXES

> *params* содержит три целых или с плавающей запятой значения, которые определяют цветовые индексы стандартного, рассеянного, зеркального освещений. Эти три значения и GL_SHININESS, являются единственными показателями материала, используемыми для балансировки света в режиме цветового индекса. См. справочную страницу [nmglLightModel](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLightModel.xml) для получения расширенной информации о цветовом индексе освещения.
 
 */
void nmglMaterialfv (NMGLenum face, NMGLenum pname, const NMGLfloat *params);

/*!
 \brief Определяет, какая матрица является текущей
 
 \param mode [in] Определяет, какой стек матриц является объектом последующих матричных операций. Допустимы два значения: GL_MODELVIEW и GL_PROJECTION. Начальное значение -- GL_MODELVIEW.


 \details nmglMatrixMode устанавливает текущий режим работы с матрицами. mode может принимать одно из четырёх значений:

GL_MODELVIEW

> Применяет следующие друг за другом матричные операции к стеку матриц моделирования.

GL_PROJECTION

> Применяет следующие друг за другом матричные операции к матрице проецирования.

Чтобы определить, какой стек матриц станет текущим объектом всех матричных операций, следует вызвать команду [nmglGet](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glGet.xml) с аргументом GL_MATRIX_MODE. Начальное значение -- GL_MODELVIEW.
 
 */
void nmglMatrixMode (NMGLenum mode);

/*!
 \brief Умножает текущую матрицу на выбранную
 
 \param m [in] Указывает на 16 следующих друг за другом значений, которые используются как элементы основной матрицы, имеющей 4x4 столбцов.


 \details nmglMultMatrix умножает текущую матрицу на другую, определённую с помощью параметра m, и заменяет текущую матрицу результатом операции.
 
 \f[C' = CM =
\begin{pmatrix}
c[0] & c[4] & c[8]  & c[12] \\
c[1] & c[5] & c[9]  & c[13] \\
c[2] & c[6] & c[10] & c[14] \\
c[3] & c[7] & c[11] & c[15] \\
\end{pmatrix} 
\begin{pmatrix}
m[0] & m[4] & m[8]  & m[12] \\
m[1] & m[5] & m[9]  & m[13] \\
m[2] & m[6] & m[10] & m[14] \\
m[3] & m[7] & m[11] & m[15] \\
\end{pmatrix}
\f]

Текущая матрица определяется активным режимом работы с матрицами (см. [nmglMatrixMode](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMatrixMode.xml)). Это матрица проецирования, матрица моделирования или текстурная матрица.
 
 */
void nmglMultMatrixf (const NMGLfloat *m);

/*!
 \brief Функция nmglMultiTexCoord2f предназначена для установки текстурных координат при использовании мультитекстурирования. Функция устанавливает текущее значение однородных текстурных координат (s,t) для заданного модуля текстур.
 \param texture- символическая константа вида NMGL_TEXTUREi, где i определяет номер изменяемого набора текстурных координат. Значение константы NMGL_TEXTUREi определяется по формуле NMGL_TEXTUREi = NMGL_TEXTURE0 + i (i лежит в диапазоне от 0 до k минус 1, где k поддерживаемое число модулей текстур, зависящее от реализации. Значение k определяется значением параметра NMGL_MAX_TEXTURE_UNITS. Минимальное количество поддерживаемых модулей текстур равно 1. Если в аргументе texture передано некорректное значение, поведение функции не определено.
 \param s значение текстурной координаты s.
 \param t значение текстурной координаты t.


 \details 
 
 */
void nmglMultiTexCoord2f (NMGLenum target, NMGLfloat s, NMGLfloat t);

/*!
 \brief Функция nmglMultiTexCoord2fv предназначена для установки текстурных координат при использовании мультитекстурирования. Функция устанавливает текущее значение однородных текстурных координат s и t для заданного модуля текстур.
 \param texture- символическая константа вида NMGL_TEXTUREi, где i определяет номер изменяемого набора текстурных координат. Значение константы NMGL_TEXTUREi определяется по формуле NMGL_TEXTUREi = NMGL_TEXTURE0 + i (i лежит в диапазоне от 0 до k минус 1, где k  поддерживаемое число модулей текстур, зависящее от реализации. Значение k определяется значением параметра NMGL_MAX_TEXTURE_UNITS. Минимальное количество поддерживаемых модулей текстур равно 1. Если в аргументе texture передано некорректное значение, поведение функции не определено.
 \param v указатель на массив из двух элементов, определяющих значения текстурных координат s и t.


 \details 
 
 */
void nmglMultiTexCoord2fv (NMGLenum target, const NMGLfloat *v);
void nmglNewList (NMGLuint list, NMGLenum mode);

/*!
 \brief устанавливает текущий вектор нормали
 
 \param nx,ny,nz [in] Определяет координаты текущей нормали по осям x, y и z . Изначальное значение текущей нормали представлено в единичном векторе (0, 0, 1).
 


 \details DТекущая нормаль устанавливается в соответствии с заданными координатами всегда, когда применяется nmglNormal. Аргумент в байтах, аргумент в виде целого числа, аргумент в виде короткого целого числа конвертируются в формат с плавающей запятой с линейным преобразованием, которое представляет самое большое отображаемое положительное целое число как 1,0 а самое большое отображаемое отрицательное целое число как -1,0.

Для нормалей, определённых nmglNormal установление единичной длины не требуется. Если активирована команда GL_NORMALIZE , нормали любой длины, установленной nmglNormal нормализуются после трансформации. Если активирована команда GL_RESCALE_NORMAL нормали масштабируются в соответствии с коэффициентом масштабирования, который извлекается из матрицы моделирования. GL_RESCALE_NORMAL требует, чтобы изначально установленные нормали имели единичную длину, и чтобы матрица моделирования содержала пропорциональное масштабирование для достоверности результатов. Чтобы активировать или деактивировать нормализацию, следует активировать [nmglEnable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnable.xml) и [nmglDisable](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisable.xml) с помощью команды GL_NORMALIZE или GL_RESCALE_NORMAL. Изначально нормализация деактивирована.
 
 */
void nmglNormal3f (NMGLfloat nx, NMGLfloat ny, NMGLfloat nz);

void nmglNormal3fv (const NMGLfloat *v);

/*!
 \brief Определяет массив нормалей
 
 \param type [in] Определяет тип данных для каждой координаты в массиве. Принята символьная константа GL_FLOAT. Начальное значение -- GL_FLOAT.
 \param stride [in] Определяет байтовое смещение между следующими друг за другом нормалями. Должно быть 0 Изначальное значение -- 0.
 \param pointer [in] Устанавливает указатель для первой координаты для первой нормали массива. Изначальное значение -- 0.


 \details nmglNormalPointer определяет расположение и формат данных массива нормалей для использования при визуализации. *type* определяет тип данных для каждой координаты нормали *stride* определяет байтовый шаг от одной нормали до следующей, что позволяет вершины и атрибуты объединять в одном массиве или разделять между несколькими.

Чтобы активировать и деактивировать массив нормалей, следует применить [nmglEnableClientState](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnableClientState.xml) и [nmglDisableClientState](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisableClientState.xml) с аргументом GL_NORMAL_ARRAY. Активированный массив нормалей используется, если применена команда [nmglDrawArrays](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawArrays.xml) или [nmglDrawElements](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawElements.xml).
 
 */
void nmglNormalPointer (NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);

/*!
 \brief Определяет массив нормалей
 
 \param type [in] Определяет тип данных для каждой координаты в массиве. Принята символьная константа GL_FLOAT. Начальное значение -- GL_FLOAT.
 \param stride [in] Определяет байтовое смещение между следующими друг за другом нормалями. Должно быть 0 Изначальное значение -- 0.
 \param pointer [in] Устанавливает указатель для первой координаты для первой нормали массива. Изначальное значение -- 0.


 \details nmglNormalPointer определяет расположение и формат данных любого массива нормалей, используемого при визуализации. Размер нормали должен быть 4 (последний элемент -- 0). type определяет тип данных для каждой координаты нормали stride определяет байтовый шаг от одной нормали до следующей, что позволяет вершины и атрибуты объединять в одном массиве или разделять между несколькими.

Чтобы активировать и деактивировать массив нормалей, следует применить [nmglEnableClientState](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnableClientState.xml) и [nmglDisableClientState](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisableClientState.xml) с аргументом GL_NORMAL_ARRAY. Активированный массив нормалей используется, если применена команда [nmglDrawArrays](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawArrays.xml) или [nmglDrawElements](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawElements.xml).
 
 */
void nmglNormalPointerNM (NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);

/*!
 \brief Умножает текущую матрицу на ортогональную матрицу
 
 \param left,right [in] Определяет координаты для левой и правой вертикальных плоскостей отсечения.
 \param bottom, top [in] Определяет координаты для верхней и нижней горизонтальных плоскостей отсечения.
 \param zNear,zFar [in] Определяет расстояния расположения для плоскостей отсечения меньшей и большей глубины. Их значения будут негативными, если плоскость должна быть позади смотрящего


 \details nmglOrtho описывает трансформацию, которая создаёт параллельную проекцию. Текущая матрица (см. [nmglMatrixMode](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMatrixMode.xml)) умножается на эту матрицу, и результат заменяет текущую матрицу, как если бы была применена команда [nmglMultMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMultMatrix.xml) с аргументом в виде следующей матрицы:

\f[
\begin{pmatrix}
\frac{2}{right - left}  & 0 & 0 & -\frac{right + left}{right - left} \\
0 & \frac{2}{top - bottom} & 0 & -\frac{top + bottom}{top - bottom} \\
0 & 0 & -\frac{2}{zFar - zNear} & -\frac{zFar + bottom}{zFar - zNear} \\
0 & 0 & 0 & 1 \\
\end{pmatrix}
\f]

Как правило, применяется режим работы с матрицами GL_PROJECTION, и операторы left bottom - nearVal and right top - nearVal определяют точки на ближней плоскости кадрирования, которые наносятся в нижний левый и верхний правый углы окна соответственно, если человеческий глаз находится на уровне (0, 0, 0). Оператор farVal определяет расположение дальней плоскости кадрирования. Значения *nearVal* и *farVal* могут быть как положительными, так и отрицательными.

Следует применить [nmglPushMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml) и [nmglPopMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPopMatrix.xml) для сохранения и восстановления текущего стека матриц

20. nmglPushMatrix and nmglPopMatrix

Помещает текущую матрицу в стек и извлекает её оттуда
 
 */
void nmglOrthof (NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar);
void nmglPixelStorei (NMGLenum pname, NMGLint param);
void nmglPointSize (NMGLfloat size);
void nmglPolygonOffset (NMGLfloat factor, NMGLfloat units);
void nmglPolygonStipple (const NMGLubyte *mask);

/*!
 \brief Помещает текущую матрицу в стек и извлекает её оттуда
 
 \details Предусмотрены стеки матриц для каждого режима работы с матрицами. В режиме GL_MODELVIEW глубина стека составляет не менее 32. В других режимах GL_MODELVIEW глубина стека составляет не менее 2. Текущей матрицей в любом режиме считается матрица «на верхушке» стека, предусмотренного для данного режима.

nmglPushMatrix опускает текущую матрицу в стеке на один уровень вниз, дублируя её. То есть после вызова команды nmglPushMatrix матрица на вершине стека будет идентична матрице, находящейся ниже неё.

Команда [nmglPopMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPopMatrix.xml) извлекает текущую матрицу в стеке, заменяя её той, которая находится в стеке ниже неё.

Изначально каждый стек содержит одну матрицу, матрицу тождественности.

Будет ошибкой пытаться опустить матрицу в полностью заполненном стеке или пытаться поднять матрицу в стеке, содержащем только одну матрицу. В каждой из таких ситуаций появится сообщение об ошибке, и больше никаких изменений состояния GL произведено не будет.

GL_STACK_OVERFLOW генерируется, если активирована команда nmglPushMatrix, при этом текущий стек матриц полон.

GL_STACK_UNDERFLOW генерируется, если активирована команда [nmglPopMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPopMatrix.xml) , при этом текущий стек матриц содержит только одну матрицу.

GL_INVALID_OPERATION генерируется, если команда nmglPushMatrix или [nmglPopMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPopMatrix.xml) была выполнена между выполнением команды [nmglBegin](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml) и соответствующим выполнением команды [nmglEnd](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnd.xml).
 
 */
 //! \{
void nmglPopMatrix (void);
void nmglPushMatrix (void);
 //! \}

void nmglRasterPos3f (NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglReadPixels (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, NMGLvoid *pixels);

/*!
 \brief Умножает текущую матрицу на матрицу вращения
 
 \param angle [in] Определяет угол поворота в градусах.
 \param x,y,z [in] Определяет, соответственно, координаты вектора по осям x, y и z.


 \details nmglRotate обеспечивает поворот элемента *angle* в градусах относительно заданного вектора по осям x y z. Текущая матрица (см. [nmglMatrixMode](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMatrixMode.xml)) умножается на матрицу вращения, и продукт этой замены текущей матрицы будет таким, как если бы была применена команда [nmglMultMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMultMatrix.xml)] с аргументом в виде следующей матрицы:

\f[
\begin{pmatrix}
x^2(1-c)+c & xy(1-c)-zs & xz(1-c)+ys & 0 \\
xy(1-c)+zs & y^2(1-c)+c & yz(1-c)-xs & 0 \\
xz(1-c)-ys & yz(1-c)+xs & z^2(1-c)+c & 0 \\
0 & 0 & 0 & 1 \\
\end{pmatrix}
\f]

При этом *c* = косинус угла, *s* = синус угла , и || x y z ||= 1 (если не так, GL нормализует данный вектор).

Если выбран режим работы с матрицами GL_MODELVIEW или GL_PROJECTION, все объекты, изображённые после применения команды nmglRotate будут повёрнуты. Следует применить [nmglPushMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml) и [nmglPopMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPopMatrix.xml) для сохранения и восстановления неповёрнутой системы координат
 
 */
void nmglRotatef (NMGLfloat angle, NMGLfloat x, NMGLfloat y, NMGLfloat z);

/*!
 \brief Умножает текущую матрицу на общую масштабирующую матрицу
 
 \param x,y,z [in] Определяет величины коэффициентов масштабирования по осям *x*, *y*, и *z* соответственно.


 \details nmglScale проводит неравномерное масштабирование по осям *x*, *y*, и *z*. Три параметра определяют необходимый коэффициент масштабирования по каждой из трёх осей.

Текущая матрица (см. [nmglMatrixMode](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMatrixMode.xml )умножается на эту матрицу масштабирования, и продукт операции заменяет текущую матрицу, как если бы была применена команда [nmglMultMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMultMatrix.xml)с аргументом в виде следующей матрицы:

\f[
\begin{pmatrix}
x & 0 & 0 & 0 \\
0 & y & 0 & 0 \\
0 & 0 & z & 0 \\
0 & 0 & 0 & 1 \\
\end{pmatrix}
\f]

  
  
Если выбран режим работы с матрицами GL_MODELVIEW или GL_PROJECTION, все объекты, изображённые после применения команды nmglScale будут подвергнуты масштабированию.

Следует применить [nmglPushMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml) и [nmglPopMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPopMatrix.xml) для сохранения и восстановления неотмасштабированной системы координат
 
 */
void nmglScalef (NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglScissor (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height);
void nmglShadeModel (NMGLenum mode);
void nmglStencilFunc (NMGLenum func, NMGLint ref, NMGLuint mask);
void nmglStencilMask (NMGLuint mask);
void nmglStencilOp (NMGLenum fail, NMGLenum zfail, NMGLenum zpass);

/*!
 \brief Функция nmglTexCoordPointer задает расположение и формат данных массива текстурных координат для использования при отрисовке. 
 \param size число координат для каждого элемента массива, должно быть равно 2. Если в функцию передано другое значение параметра, генерируется ошибка NMGL_INVALID_VALUE.
 \param type тип данных для каждой координаты текстуры, должен иметь значение NMGL_FLOAT.
 \param stride смещение в байтах между последовательными наборами координат текстуры. Если шаг равен 0, то элементы массива считаются плотно упакованными. Единственное доступное значение - 0.
 \param pointer указатель на первую координату первого набора текстурных координат в массиве. Начальное значение равно 0.


 \details При указании массива координат текстуры размер, тип, шаг и указатель сохраняются как состояние на стороне клиента в дополнение к текущей привязке буферного объекта массива вершин.

Чтобы включить или отключить массив координат текстуры, необходимо вызвать функции nmglEnableClientState или nmglDisableClientState с аргументом NMGL_TEXTURE_COORD_ARRAY. Если этот параметр включен, массив координат текстуры используется, когда вызываются функции nmglDrawArrays, nmglDrawElements.
 */
void nmglTexCoordPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);

/*!
 **  \brief Функция nmglTexEnvfv предназначена для установки параметров текстурного окружения, определяющего, каким образом интерпретируются значения текстуры при текстурировании фрагментов. Данная функция устанавливает текущее значение компонент цвета текстурного окружения, обозначаемого С~c~ и A~c~. Значения С~c~ и A~c~ используются в качестве аргументов функций текстурирования, устанавливаемых с помощью функции nmglTexEnvi с параметром NMGL_TEXTURE_ENV_MODE.
 \param target данный аргумент должен принимать значение NMGL_TEXTURE_ENV.
 \param pname символическая константа, определяющая наименование устанавливаемого параметра. Данный аргумент может принимать единственное значение NMGL_TEXTURE_ENV_COLOR.
 \param params указатель на массив значений параметра. Значение текущего цвета текстурного окружения задается массивом из четырех чисел, каждое из которых устанавливает одну из компонент (RGBA). Значения должны лежать в диапазоне \[0, 1\]. Значения, выходящие за диапазон, приводятся к ближайшему граничному значению диапазона (0 или 1).


 \details 
 
 */
void nmglTexEnvfv (NMGLenum target, NMGLenum pname, const NMGLfloat *params);

/*!
   \brief Функция nmglTexEnvi предназначена для установки параметров текстурного окружения, определяющего, каким образом интерпретируются значения текстуры при текстурировании фрагментов. Данная функция устанавливает вид функции текстурирования, определяющей способ вычисления результирующего цвета фрагмента при наложении текстуры.
 \param target данный аргумент должен принимать значение NMGL_TEXTURE_ENV.
 \param pname символическая константа, определяющая наименование устанавливаемого параметра. Данный аргумент может принимать единственное значение NMGL_TEXTURE_ENV_MODE.
 \param param новое значение параметра. Возможные значения: NMGL_MODULATE, NMGL_REPLACE, NMGL_DECAL, NMGL_BLEND, NMGL_ADD.


 \details 
 
 */
void nmglTexEnvi (NMGLenum target, NMGLenum pname, NMGLint param);
void nmglTexImage2D (NMGLenum target, NMGLint level, NMGLint internalformat, NMGLsizei width, NMGLsizei height, NMGLint border, NMGLenum format, NMGLenum type, const NMGLvoid *pixels);
/*!
 \brief Функция nmglTexParameteri устанавливает параметры текстурирования, определяющие способы обработки текстурного массива в процессе применения его к фрагментам.
 \param target тип текстуры. Данный аргумент должен принимать значение NMGL_TEXTURE_2D.
 \param pname символическая константа, определяющая наименование устанавливаемого параметра. Возможные значения: NMGL_TEXTURE_WRAP_S, NMGL_TEXTURE_WRAP_T, NMGL_TEXTURE_MIN_FILTER, NMGL_TEXTURE_MAG_FILTER.
 \param param новое значение параметра. Параметры обработки границ NMGL_TEXTURE_WRAP_S и NMGL_TEXTURE_WRAP_T могут принимать только значения NMGL_REPEAT и NMGL_CLAMP_TO_EDGE. Поддерживаются все режимы фильтрации. Возможные значения параметра NMGL_TEXTURE_MIN_FILTER: NMGL_NEAREST, NMGL_LINEAR, NMGL_NEAREST_MIPMAP_NEAREST, NMGL_NEAREST_MIPMAP_LINEAR, NMGL_LINEAR_MIPMAP_NEAREST, NMGL_LINEAR_MIPMAP_LINEAR. Возможные значения параметра NMGL_TEXTURE_MAG_FILTER: NMGL_NEAREST, NMGL_LINEAR. Если значение передаваемого аргумента отличается от допустимого, генерируется ошибка NMGL_INVALID_VALUE.


 \details
 
 */
void nmglTexParameteri (NMGLenum target, NMGLenum pname, NMGLint param);
void nmglTexSubImage2D (NMGLenum target, NMGLint level, NMGLint xoffset, NMGLint yoffset, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const NMGLvoid *pixels);

/*!
 \brief Умножает текущую матрицу на матрицу трансляции
 
 \param x,y,z [in] Определяет, соответственно, координаты вектора по осям *x*, *y* и *z*.


 \details nmglTranslate производит трансляцию по осям *x* *y* *z*. Текущая матрица (см. [nmglMatrixMode](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMatrixMode.xml)) умножается на матрицу трансляции, и продукт этой операции заменяет текущую матрицу, как если бы была применена команда [nmglMultMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMultMatrix.xml)с аргументом в виде следующей матрицы:

\f[
\begin{pmatrix}
1 & 0 & 0 & x \\
0 & 1 & 0 & y \\
0 & 0 & 1 & z \\
0 & 0 & 0 & 1 \\
\end{pmatrix}
\f]

Если выбран режим работы с матрицами GL_MODELVIEW или GL_PROJECTION, все объекты, изображённые после применения команды nmglTranslate будут транслированы.

Следует применить [nmglPushMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml) и [nmglPopMatrix](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPopMatrix.xml) для сохранения и восстановления нетранслированной системы координат.
 
 */
void nmglTranslatef (NMGLfloat x, NMGLfloat y, NMGLfloat z);

/*!
 \defgroup Vertexf Vertexf
 \brief Определяет вершину
 
 \param x,y,z,w  [in] Определяет координаты *x*, *y*, *z*, и *w* для вершины. Не все параметры присутствуют во всех формах данной команды.

 
 \details Команды nmglVertex используются в рамках пар [nmglBegin](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml)/[nmglEnd](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnd.xml) для определения вершин точки, линии, многоугольника. Текущие цвет, координаты нормали, текстурные координаты, облачные координаты связаны с вершиной, когда вызвана команда nmglVertex.

Когда установлены координаты только по осям *x* и *y* , по оси *z* по умолчанию устанавливается значение 0, а по оси *w* по умолчанию устанавливается значение 1. Когда установлены координаты по осям *x*, *y* и *z*, по оси *w* по умолчанию устанавливается значение 1.
 
 */
 //! \{
void nmglVertex2f (NMGLfloat x, NMGLfloat y);
void nmglVertex3f (NMGLfloat x, NMGLfloat y, NMGLfloat z);
 //! \}

/*!
 \defgroup Vertexfv Vertexfv
 \brief Определяет вершину
 
 \param v Определяет указатель для массива из двух, трёх или четырёх элементов. Элементы массива из двух элементов: *x* и *y*; массива из трёх элементов: *x*, *y*, и *z*; массива из четырёх элементов: *x*, *y*, *z* и *w*.
 


 
 \details Команды nmglVertex используются в рамках пар [nmglBegin](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml)/[nmglEnd](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnd.xml) для определения вершин точки, линии, многоугольника. Текущие цвет, координаты нормали, текстурные координаты, облачные координаты связаны с вершиной, когда вызвана команда nmglVertex.

Когда установлены координаты только по осям x и y , по оси z по умолчанию устанавливается значение 0, а по оси w по умолчанию устанавливается значение 1. Когда установлены координаты по осям x, y и z, по оси w по умолчанию устанавливается значение 1.
 
 */
 //! \{
void nmglVertex2fv (const NMGLfloat *v);
void nmglVertex3fv (const NMGLfloat *v);
 //! \}

/*!
 \brief определяет массив данных о вершине
 
 \param size [in] Определяет количество осей координат для вершины. Должно быть 2, 3 или 3. Изначальное значение -- 4.
 \param type [in] Определяет тип данных для каждой координаты в массиве. Принята символьная константа GL_FLOAT. Начальное значение -- GL_FLOAT.
 \param stride [in] Определяет байтовое смещение между следующими друг за другом байтовыми вершинами. Должно быть 0. Изначальное значение -- 0.
 \param pointer [in] Устанавливает указатель для первой координаты для первой вершины массива. Изначальное значение -- 0.


 \details nmglVertexPointer определяет расположение и формат данных массива координат вершины для использования при визуализации. *size* определяет количество координат для вершины и должен иметь значение 2, 3, или 4. *type* определяет тип данных для каждой координаты *stride* определяет байтовый шаг от одной вершины до следующей, что позволяет вершины и атрибуты объединять в одном массиве или разделять между несколькими.

Чтобы активировать и деактивировать массив, следует применить [nmglEnableClientState](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnableClientState.xml) и [nmglDisableClientState](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDisableClientState.xml) с аргументом GL_VERTEX_ARRAY. Активированный массив используется, если применена команда [nmglDrawArrays](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawArrays.xml) или [nmglDrawElements](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawElements.xml).
 
 */
void nmglVertexPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);

/*!
 \brief устанавливает поле видимости
 
 \param x,y [in] Определяет в пикселях нижний левый угол прямоугольника поля видимости. Изначальное значение -- (0,0).
 \param width, height [in] Определяет ширину и высоту поля видимости. Когда контекст GL впервые прикрепляется к окну, значения параметров, *width* и *height* устанавливаются в соответствии с размерами этого окна.


 \details Команда nmglViewport определяет аффинное преобразование координат x и y от нормализованных координат устройства в координаты окна. Пусть \f$ x_n \f$  и \f$ y_n \f$ -- это нормализуемые координаты устройства. Тогда координаты окна \f$ x_w \f$ \f$ y_w \f$ будут вычислены следующим образом:

\f$ x_w = (x_n*width/2) + (x+width/2)) \f$ 

\f$ y_w = (y_n*height/2) + (y+height/2)) \f$ 

Ширина и высота поля видимости молча закрепляются за уровнем, зависящим от варианта исполнения GL. Чтобы запросить такой уровень, следует вызвать команду [nmglGet](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glGet.xml) с аргументом GL_MAX_VIEWPORT_DIMS.
 
 */
void nmglViewport (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height);

/*!
 \brief Функция обеспечивает загрузку текстурного изображения в активный текстурный объект.
 \param target определяет тип текстуры и должен иметь значение NMGL_TEXTURE_2D.
 \param level определяет mipmap уровень, для которого осуществляется загрузка текстурного изображения. Значения данного аргумента должны лежать в диапазоне от 0 до NMGL_MAX_MIPMAP_LVL, где NMGL_MAX_MIPMAP_LVL -- максимальный уровень mipmap, поддерживаемый реализацией для целевой архитектуры.
 \param internalformat определяет количество цветовых компонент текселя, и может принимать значения: NMGL_RGBA (4 компоненты), NMGL_RGB (3 компоненты), NMGL_ALPHA (1 компонента), NMGL_LUMINANCE (1 компонента), NMGL_LUMINANCE_ALPHA (две компоненты) и NMGL_COLOR_INDEX8_EXT (1 компонента). Каждая компонента должна иметь тип NMGLubyte.
 \param width определяет соответственно длину строки (ширину) текстурного изображения в текселях.
 \param height определяет соответственно длину столбца (высоту) текстурного изображения в текселях.
 \param border определяет ширину границы текстуры. Должен иметь значение 0.
 \param format определяет формат данных текселя. Если значение аргумента **internalformat** равно NMGL_COLOR_INDEX8_EXT, то значение аргумента **format** должно быть равно NMGL_COLOR_INDEX. В противном случае значение аргумента **format** должно совпадать со значением аргумента **internalformat.**
 \param type определяет тип данных текселя. Должен иметь значение NMGL_UNSIGNED_BYTE.
 \param pixels указатель на текстурное изображение, хранящееся в памяти.


 \details 
 
 */
void nmglTexImage2D(NMGLenum target, NMGLint level, NMGLint internalformat, NMGLsizei width, NMGLsizei height, NMGLint border, NMGLenum format, NMGLenum type, const void *pixels );

/*!
 \brief Функция осуществляет запись фрагмента текстурного ищображения в активный текстурный объект.
 \param target определяет тип текстуры и должен иметь значение NMGL_TEXTURE_2D.
 \param level определяет mipmap уровень, для которого осуществляется загрузка фрагмента текстурного изображения. Значения данного аргумента должны лежать в диапазоне от 0 до NMGL_MAX_MIPMAP_LVL, где NMGL_MAX_MIPMAP_LVL -- максимальный уровень mipmap, поддерживаемый реализацией для целевой архитектуры.
 \param xoffset определяет смещение в текселях по оси x левого нижнего угла фрагмента текстурного изображения внутри текстурного изображения, куда осуществляется запись.
 \param yoffset определяет смещение в текселях по оси y левого нижнего угла фрагмента текстурного изображения внутри текстурного изображения, куда осуществляется запись.
 \param width определяет ширину фрагмента текстурного изображения в текселях.
 \param height определяет высоту фрагмента текстурного изображения в текселях.
 \param format определяет формат данных текселя. Возможные значения: NMGL_RGBA, NMGL_RGB, NMGL_ALPHA, NMGL_LUMINANCE и NMGL_LUMINANCE_ALPHA.
 \param type определяет тип данных текселя. Должен иметь значение NMGL_UNSIGNED_BYTE.
 \param pixels указатель на фрагмент текстурного изображения в памяти.


 \details 
 
 */
void nmglTexSubImage2D (NMGLenum target, NMGLint level, NMGLint xoffset, NMGLint yoffset, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const void *pixels);
void nmglTexCoordPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const void *pointer);

/*EXT_paletted_texture*/
void nmglColorSubTableEXT (NMGLenum target, NMGLsizei start, NMGLsizei count, NMGLenum format, NMGLenum type, const void *data);

/*!
 \brief Функция предназначена для задания палитры цветов для текстурного объекта.

 \param target определяет тип текстуры, для которой задаётся палитра. Единственное поддерживаемое значение: NMGL_TEXTURE_2D.
 \param internalformat определяет формат и точность задания палитры, хранимой в текстурном объекте. Единственное поддерживаемое значение: NMGL_RGBA.
 \param width определяет длину палитры в текселях. Длина должна быть степенью двойки и больше либо равна 1. По умолчанию каждый текстурный объект имеет палитру из одного текселя, каждая компонента которого имеет значение 1. Реализация ограничивает максимальное возможное значения аргумента width константой NMGL_MAX_PALETTE_WIDTH.
 \param format определяет количество компонент в одном текселе. Единственное поддерживаемое значение: NMGL_RGBA.
 \param type определяет формат представления компонент текселя. Единственное поддерживаемое значение  NMGL_UNSIGNED_BYTE.
 \param data указатель на массив данных текселей загружаемой палитры.


 \details Если длина палитры больше, чем диапазон цветов индексов цвета в текстурной памяти, то некоторые элементы палитры не будут использоваться.

Если наоборот, длина палитры меньше диапазона индексов цвета, то старшие биты текстурных данных будут игнорироваться, и при доступе к палитре будет использоваться только нужное количество бит.
 
 */
void nmglColorTableEXT (NMGLenum target, NMGLenum internalformat, NMGLsizei width, NMGLenum format, NMGLenum type, const void *data);
void nmglGetColorTableParameterivEXT (NMGLenum target, NMGLenum pname, NMGLint *params);


///@}

/*!
\defgroup  service_api Сервисные функции
 */

#ifdef __cplusplus
}
#endif //__cplusplus
#endif
