#ifndef __NMGL_H__
#define __NMGL_H__


/*! 
	\page intro Введение

Библиотека предназначена для построения графических OpenGL приложений на процессорном ядре NMC4, на плате МС121.01 . 
В состав библиотеки входят интерфейсные функции, начинающиеся с префикса nmgl и определяемые спецификацией OpenGL SC 1.0.1. 


*/


#include "nmgltype.h"

#include "nmgldef.h"

/**
 * \defgroup  main Интерфейсные функции nmOpenGL
 */


/**
 * \defgroup  opengl_api Интерфейсные функции nmOpenGL
 */

///@{ 


void nmglActiveTexture (NMGLenum texture);

void nmglAlphaFunc (NMGLenum func, NMGLclampf ref);

/**
 *  \brief Задаёт ограничения для вершин примитива или группы одинаковых примитивов
 *  
   \param mode Определяет примитив(ы), которые будут созданы из вершин между glBegin и последующим [nmglEnd](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnd.xml). Приняты десять символьных констант: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN.
 *  \return void
 *  
 *  \details nmglBegin и  <a href="https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnd.xml"> nmglEnd </a>  задают ограничения для вершин, которые определяют примитив, или группу одинаковых примитивов. Для nmglBegin допустим только один аргумент, который определяет, каким из десяти способов вершины будут считаны. Пусть n -- целое число, не менее 1, а N -- общее количество выбранных вершин. Интерпретации будут выглядеть следующим образом:

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
void nmglClear (NMGLbitfield mask);
void nmglClearColor (NMGLclampf red, NMGLclampf green, NMGLclampf blue, NMGLclampf alpha);
void nmglClearDepthf (NMGLclampf depth);
void nmglClearStencil (NMGLint s);
void nmglClientActiveTexture (NMGLenum texture);
void nmglColor4f (NMGLfloat red, NMGLfloat green, NMGLfloat blue, NMGLfloat alpha);
void nmglColor4fv (const NMGLfloat *v);
void nmglColor4ub (NMGLubyte red, NMGLubyte green, NMGLubyte blue, NMGLubyte alpha);
void nmglColorMask (NMGLboolean red, NMGLboolean green, NMGLboolean blue, NMGLboolean alpha);
void nmglColorPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);
void nmglColorSubTableEXT (NMGLenum target, NMGLsizei start, NMGLsizei count, NMGLenum format, NMGLenum type, const NMGLvoid *table);
void nmglColorTableEXT (NMGLenum target, NMGLenum internalformat, NMGLsizei width, NMGLenum format, NMGLenum type, const NMGLvoid *table);
void nmglCopyPixels (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height, NMGLenum type);
void nmglCullFace (NMGLenum mode);
void nmglDepthFunc (NMGLenum func);
void nmglDepthMask (NMGLboolean flag);
void nmglDepthRangef (NMGLclampf zNear, NMGLclampf zFar);
void nmglDisable (NMGLenum cap);
void nmglDisableClientState (NMGLenum array);
void nmglDrawArrays (NMGLenum mode, NMGLint first, NMGLsizei count);
void nmglDrawElements (NMGLenum mode, NMGLsizei count, NMGLenum type, const NMGLvoid *indices);
void nmglDrawPixels (NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const NMGLvoid *pixels);
void nmglEnable (NMGLenum cap);
void nmglEnableClientState (NMGLenum array);
void nmglEnd (void);
void nmglEndList (void);
void nmglFinish (void);
void nmglFlush (void);
void nmglFrontFace (NMGLenum mode);
void nmglFrustumf (NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar);
NMGLuint nmglGenLists (NMGLsizei range);
void nmglGenTextures (NMGLsizei n, NMGLuint *textures);
NMGLenum nmglGetError (void);
void nmglGetBooleanv (NMGLenum pname, NMGLboolean *params);
void nmglGetColorTableEXT (NMGLenum target, NMGLenum format, NMGLenum type, NMGLvoid *table);
void nmglGetColorTableParameterivEXT (NMGLenum target, NMGLenum pname, NMGLint *params);
void nmglGetFloatv (NMGLenum pname, NMGLfloat *params);
void nmglGetIntegerv (NMGLenum pname, NMGLint *params);
void nmglGetLightfv (NMGLenum light, NMGLenum pname, NMGLfloat *params);
void nmglGetMaterialfv (NMGLenum face, NMGLenum pname, NMGLfloat *params);
void nmglGetPointerv (NMGLenum pname, NMGLvoid * *params);
void nmglGetPolygonStipple (NMGLubyte *mask);
void nmglGetTexEnvfv (NMGLenum target, NMGLenum pname, NMGLfloat *params);
void nmglGetTexEnviv (NMGLenum target, NMGLenum pname, NMGLint *params);
void nmglGetTexParameteriv (NMGLenum target, NMGLenum pname, NMGLint *params);
const NMGLubyte * nmglGetString (NMGLenum name);
void nmglHint (NMGLenum target, NMGLenum mode);
NMGLboolean nmglIsEnabled (NMGLenum cap);
void nmglLightfv (NMGLenum light, NMGLenum pname, const NMGLfloat *params);
void nmglLightModelfv (NMGLenum pname, const NMGLfloat *params);
void nmglLineStipple (NMGLint factor, NMGLushort pattern);
void nmglLineWidth (NMGLfloat width);
void nmglListBase (NMGLuint base);
void nmglLoadIdentity (void);
void nmglLoadMatrixf (const NMGLfloat *m);
void nmglMaterialf (NMGLenum face, NMGLenum pname, NMGLfloat param);
void nmglMaterialfv (NMGLenum face, NMGLenum pname, const NMGLfloat *params);
void nmglMatrixMode (NMGLenum mode);
void nmglMultMatrixf (const NMGLfloat *m);
void nmglMultiTexCoord2f (NMGLenum target, NMGLfloat s, NMGLfloat t);
void nmglMultiTexCoord2fv (NMGLenum target, const NMGLfloat *v);
void nmglNewList (NMGLuint list, NMGLenum mode);
void nmglNormal3f (NMGLfloat nx, NMGLfloat ny, NMGLfloat nz);
void nmglNormal3fv (const NMGLfloat *v);
void nmglNormalPointer (NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);
void nmglNormalPointerNM (NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);
void nmglOrthof (NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar);
void nmglPixelStorei (NMGLenum pname, NMGLint param);
void nmglPointSize (NMGLfloat size);
void nmglPolygonOffset (NMGLfloat factor, NMGLfloat units);
void nmglPolygonStipple (const NMGLubyte *mask);
void nmglPopMatrix (void);
void nmglPushMatrix (void);
void nmglRasterPos3f (NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglReadPixels (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, NMGLvoid *pixels);
void nmglRotatef (NMGLfloat annmgle, NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglScalef (NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglScissor (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height);
void nmglShadeModel (NMGLenum mode);
void nmglStencilFunc (NMGLenum func, NMGLint ref, NMGLuint mask);
void nmglStencilMask (NMGLuint mask);
void nmglStencilOp (NMGLenum fail, NMGLenum zfail, NMGLenum zpass);
void nmglTexCoordPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);
void nmglTexEnvfv (NMGLenum target, NMGLenum pname, const NMGLfloat *params);
void nmglTexEnvi (NMGLenum target, NMGLenum pname, NMGLint param);
void nmglTexImage2D (NMGLenum target, NMGLint level, NMGLint internalformat, NMGLsizei width, NMGLsizei height, NMGLint border, NMGLenum format, NMGLenum type, const NMGLvoid *pixels);
void nmglTexParameteri (NMGLenum target, NMGLenum pname, NMGLint param);
void nmglTexSubImage2D (NMGLenum target, NMGLint level, NMGLint xoffset, NMGLint yoffset, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const NMGLvoid *pixels);
void nmglTranslatef (NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglVertex2f (NMGLfloat x, NMGLfloat y);
void nmglVertex2fv (const NMGLfloat *v);
void nmglVertex3f (NMGLfloat x, NMGLfloat y, NMGLfloat z);
void nmglVertex3fv (const NMGLfloat *v);
void nmglVertexPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer);
void nmglViewport (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height);

void nmglTexImage2D(NMGLenum target, NMGLint level, NMGLint internalformat, NMGLsizei width, NMGLsizei height, NMGLint border, NMGLenum format, NMGLenum type, const void *pixels );
void nmglTexSubImage2D (NMGLenum target, NMGLint level, NMGLint xoffset, NMGLint yoffset, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const void *pixels);
void nmglTexCoordPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const void *pointer);

/*EXT_paletted_texture*/
void nmglColorSubTableEXT (NMGLenum target, NMGLsizei start, NMGLsizei count, NMGLenum format, NMGLenum type, const void *data);
void nmglColorTableEXT (NMGLenum target, NMGLenum internalformat, NMGLsizei width, NMGLenum format, NMGLenum type, const void *data);
void nmglGetColorTableEXT (NMGLenum target, NMGLenum format, NMGLenum type,void *data);
void nmglGetColorTableParameterivEXT (NMGLenum target, NMGLenum pname, NMGLint *params);


///@}

/**
 * \defgroup  service_api Сервисные функции
 */


#endif
