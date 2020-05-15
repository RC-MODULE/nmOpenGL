#include <stdint.h>
#include "image.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct color {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	} color;

	int writeToBmp(uint32_t data, uint8_t size, FILE *fp);

	/**
	* Функция предназачена для получения значения пикселя из массива pixels размером width*height пикселей.
	* Предполагается, что изображение в массиве хранится по рядам, начиная с левого нижнего пикселя изображения.
	* На каждый пиксель отводится 24 бита. Массив pixels заполняется при вызове функции loadBMP_custom.
	*/
	int getPixelValue(uint32_t x, uint32_t y, image_t image, color* pixelValue);
	int savePixelData(uint8_t bitsInPixel,  image_t image, uint32_t rowPadding, FILE *fp);

	//format - формат хранения данных в массиве pixels
	//type - тип данных для хранения формата format в массиве pixels
	int saveToBmp(uint8_t bitsInPixel, image_t image, char* filePath);
	unsigned int loadBMP_custom(const char * imagepath, format_t format, type_t type, image_t* image);


    /**
    функция getNameFromPath предназначена для получения имени файла по значению его пути.
    
    \param image [in] Cтруктура, которую нужно сохранить.
    \param fileName [in] Имя заголовочного файла без расширения для сохранения
    \retval status [out] 0 - в случае успеха, -1 - в случае ошибки
    
    */
    int getNameFromPath (char* filePath, char* fileName);

    /**
    Функция imageToHeader предназначена для создания заголовочного файла на языке Си и сохранения в него содержимого структуры типа image_t в виде определения структуры.
    
    \param image [in] Cтруктура, которую нужно сохранить.
    \param fileName [in] Имя заголовочного файла без расширения для сохранения
    \retval status [out] 0 в случае успеха, -1 в случае ошибки
    
    */
    int imageToHeader(image_t* image, const char* fileName);

#ifdef __cplusplus
}
#endif