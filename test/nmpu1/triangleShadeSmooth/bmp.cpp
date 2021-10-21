#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "bmp.h"
#include "image.h"

// Uncomment to create header files with textures as image_t structures.
// #define WRITE_PIXELS_TO_FILE

int getColorIndex(unsigned int mask)
{
	switch (mask)
	{
	case 0xFF000000:
		return 3;
	case 0x00FF0000:
		return 2;
	case 0x0000FF00:
		return 1;
	case 0x000000FF:
		return 0;
	default:
		printf("Unsupported color mask value\n");
		return 0;
	}
}


	//in BMP every field begins with a lowest byte 
	int writeToBmp(uint32_t data, uint8_t size, FILE *fp) {

		int i = 0;
		uint32_t data_tmp = data;

		//lowest byte first
		for (i = 0; i < size; i++)
		{
			fputc(data_tmp & 0x000000FF, fp);
			data_tmp = data_tmp >> 8;
		}

		return 0;

}

	int getPixelValue(uint32_t x, uint32_t y, image_t image, color * pixelValue)
	{

		uint32_t rowDataSize = 0;
		uint32_t rawDataSize = 0;
		uint32_t rowPadding = 0;
		uint32_t bmpFileSize = 0;
		uint32_t bitsInPixel = 24;
		uint32_t bytesInPixel = 0;
		
		uint32_t width = image.width;
		uint32_t height = image.height;
		void * pixels = image.pixels;
		format_t format = image.format;
		type_t type = image.type;
		int alignment = image.alignment;
	
		if (checkAlign(alignment))
		{
			printf("%s: wrong alignment value (%d)", __func__, alignment);
			return -1;
		}


		switch (format)
		{
			case RGB:
			  bytesInPixel = 3;
			  break;
			
			case ARGB:
			case RGBA:
			  bytesInPixel = 4;
			  break;
		
			case ALPHA:
			case LUMINANCE:
			  bytesInPixel = 1;
			  break;
		
			case LUMINANCE_ALPHA:
			  bytesInPixel = 2;
			  break;
			  
			default:
			  bytesInPixel = 3;

			  break;
		}

		int rowPaddingBytes = (width * bytesInPixel) % alignment ? alignment - (width * bytesInPixel) % alignment : 0;	
		unsigned int imageRowWidthBytes = width * bytesInPixel + rowPaddingBytes; 


		if ((format == RGB) && (type == UNSIGNED_BYTE))
		{
			//Чтение производится из массива данных изображения bmp с учетом наличия в нём 
			//дополнительных байтов для выравнивания по границе 4 байтов
			pixelValue->r = ((unsigned char*)pixels)[y * imageRowWidthBytes + x * bytesInPixel];
			pixelValue->g = ((unsigned char*)pixels)[y * imageRowWidthBytes + x * bytesInPixel + 1];
			pixelValue->b = ((unsigned char*)pixels)[y * imageRowWidthBytes + x * bytesInPixel + 2];
			pixelValue->a = 255;
		}
		else if (((format == RGBA) && (type == UNSIGNED_BYTE)))
		{
			//Чтение производится из массива данных изображения bmp с учетом наличия в нём 
			//дополнительных байтов для выравнивания по границе 4 байтов
			pixelValue->r = ((unsigned char*)pixels)[y*imageRowWidthBytes + x*bytesInPixel];
			pixelValue->g = ((unsigned char*)pixels)[y*imageRowWidthBytes + x*bytesInPixel + 1];
			pixelValue->b = ((unsigned char*)pixels)[y*imageRowWidthBytes + x*bytesInPixel + 2];
			pixelValue->a = ((unsigned char*)pixels)[y*imageRowWidthBytes + x*bytesInPixel + 3];
		}
		else if (((format == RGBA) && (type == UNSIGNED_INT_8_8_8_8)))
		{
			//Чтение производится из массива данных изображения bmp с учетом наличия в нём 
			//дополнительных байтов для выравнивания по границе 4 байтов
			pixelValue->r = (((unsigned int*)pixels)[y * width + (rowPaddingBytes>>2) + x] & 0xFF000000) >> 24;
			pixelValue->g = (((unsigned int*)pixels)[y * width + (rowPaddingBytes>>2) + x] & 0x00FF0000) >> 16;
			pixelValue->b = (((unsigned int*)pixels)[y * width + (rowPaddingBytes>>2) + x] & 0x0000FF00) >> 8;
			pixelValue->a =  ((unsigned int*)pixels)[y * width + (rowPaddingBytes>>2) + x] & 0x000000FF;
		}
		else if (((format == ARGB) && (type == UNSIGNED_INT_8_8_8_8)))
		{
			//Чтение производится из массива данных изображения bmp с учетом наличия в нём 
			//дополнительных байтов для выравнивания по границе 4 байтов
			pixelValue->a = (((unsigned int*)pixels)[y * width + (rowPaddingBytes>>2) + x] & 0xFF000000) >> 24;
			pixelValue->r = (((unsigned int*)pixels)[y * width + (rowPaddingBytes>>2) + x] & 0x00FF0000) >> 16;
			pixelValue->g = (((unsigned int*)pixels)[y * width + (rowPaddingBytes>>2) + x] & 0x0000FF00) >> 8;
			pixelValue->b =  ((unsigned int*)pixels)[y * width + (rowPaddingBytes>>2) + x] & 0x000000FF;
		}
		else if ((format == ALPHA) && (type == UNSIGNED_BYTE))
		{
			//Чтение производится из массива данных изображения bmp с учетом наличия в нём 
			//дополнительных байтов для выравнивания по границе 4 байтов
			pixelValue->r = 0;
			pixelValue->g = 0;
			pixelValue->b = 0;
			pixelValue->a = ((unsigned char*)pixels)[y * imageRowWidthBytes + x * bytesInPixel];
		}
		else if ((format == LUMINANCE) && (type == UNSIGNED_BYTE))
		{
			//Чтение производится из массива данных изображения bmp с учетом наличия в нём 
			//дополнительных байтов для выравнивания по границе 4 байтов
			pixelValue->r = ((unsigned char*)pixels)[y * imageRowWidthBytes + x * bytesInPixel];
			pixelValue->g = ((unsigned char*)pixels)[y * imageRowWidthBytes + x * bytesInPixel];
			pixelValue->b = ((unsigned char*)pixels)[y * imageRowWidthBytes + x * bytesInPixel];
			pixelValue->a = 255; 
		}
		else if ((format == LUMINANCE_ALPHA) && (type == UNSIGNED_BYTE))
		{
			//Чтение производится из массива данных изображения bmp с учетом наличия в нём 
			//дополнительных байтов для выравнивания по границе 4 байтов
			pixelValue->r = ((unsigned char*)pixels)[y * imageRowWidthBytes + x * bytesInPixel];
			pixelValue->g = ((unsigned char*)pixels)[y * imageRowWidthBytes + x * bytesInPixel];
			pixelValue->b = ((unsigned char*)pixels)[y * imageRowWidthBytes + x * bytesInPixel];
			pixelValue->a = ((unsigned char*)pixels)[y * imageRowWidthBytes + x * bytesInPixel + 1];
		}
		return 0;
	}

	int savePixelData(uint8_t bitsInPixel, image_t image, uint32_t rowPadding, FILE *fp)
	{
		int i = 0;
		int j = 0;
		int k = 0;

		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
		unsigned char a = 0;
		unsigned char step = 0;

		uint32_t data_tmp = 0;

		uint32_t width = image.width;
		uint32_t height = image.height;
		void * pixels = image.pixels;
		format_t format = image.format;
		type_t type = image.type;
		unsigned int alignment = image.alignment;
		unsigned int bytesInPixel = 0;

		if (checkAlign(alignment))
		{
			printf("%s: wrong alignment value (%d)", __func__, alignment);
			return -1;
		}

		switch (format)
		{
			case RGB:
			  bytesInPixel = 3;
			  break;
			
			case ARGB:
			case RGBA:
			  bytesInPixel = 4;
			  break;
		
			case ALPHA:
			case LUMINANCE:
			  bytesInPixel = 1;
			  break;
		
			case LUMINANCE_ALPHA:
			  bytesInPixel = 2;
			  break;
			  
			default:
			  bytesInPixel = 3;

			  break;
		}

		int rowPaddingBytes = (width * bytesInPixel) % alignment ? alignment - (width * bytesInPixel) % alignment : 0;	
		unsigned int imageRowWidthBytes = width * bytesInPixel + rowPaddingBytes; 


		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				if ((format == ARGB) && (type == UNSIGNED_INT_8_8_8_8)) //(uint32_t*)pixels[x] = 0xARGB
				{
					data_tmp = ((uint32_t*)pixels)[i*width + j];
					r = (data_tmp & 0x00FF0000) >> 16;
					g = (data_tmp & 0x0000FF00) >> 8;
					b = data_tmp & 0x000000FF;
					a = (data_tmp & 0xFF000000) >> 24;
				}
				else if ((format == RGBA) && (type == UNSIGNED_INT_8_8_8_8)) //(uint32_t*)pixels[x] = 0xRGBA
				{
					data_tmp = ((uint32_t*)pixels)[i*width + j];
					r = (data_tmp & 0xFF000000) >> 24;
					g = (data_tmp & 0x00FF0000) >> 16;
					b = (data_tmp & 0x0000FF00) >> 8;
					a = data_tmp & 0x000000FF;
				}
				else if ((format == RGBA) && (type == UNSIGNED_BYTE)) //(unsigned char*)pixels[0/1/2/3/4/5/6...] = 0xR/0xG/0xB/0xA/0xR/0xG/0xB...
				{
					r = ((unsigned char*)pixels)[i*imageRowWidthBytes + j*bytesInPixel ];
					g = ((unsigned char*)pixels)[i*imageRowWidthBytes + j*bytesInPixel + 1];
					b = ((unsigned char*)pixels)[i*imageRowWidthBytes + j*bytesInPixel + 2];
					a = ((unsigned char*)pixels)[i*imageRowWidthBytes + j*bytesInPixel + 3];
				}
				else if ((format == RGB) && (type == UNSIGNED_BYTE)) //(unsigned char*)pixels[0/1/2] = r/g/b
				{
					r = ((unsigned char*)pixels)[i*imageRowWidthBytes + j*bytesInPixel];
					g = ((unsigned char*)pixels)[i*imageRowWidthBytes + j*bytesInPixel+ 1];
					b = ((unsigned char*)pixels)[i*imageRowWidthBytes + j*bytesInPixel+ 2];
				}


				//RGB. lowest byte first
				fputc(b, fp);
				fputc(g, fp);
				fputc(r, fp);

				if ((format == RGBA) || (format == ARGB)) //in file should be: B G R A B G R A to use in Windows
				{
					fputc(a, fp);
				}
			}

			for (k = 0; k < rowPadding; k++)
			{
				fputc(0x00, fp);
			}
		}

		return 0;


	}

    //format определяет формат хранения данных в массиве-источнике (image.pixels), а также в массиве(файле)-приёмнике (filePath)
    //Данные источника в формате format RGBA и ARGB хранятся в файле-приёмнике всегда в формате B G R A B G R A
    //Данные источника в формате format RGB хранятся в файле-приёмнике в формате B G R B G R
    //type задаёт тип данных, используемый для хранения данных в массиве-источнике в заданном формате
	int saveToBmp(uint8_t bitsInPixel, image_t image, const char* filePath) {

		int i = 0;
		int j = 0;
		FILE *fp;
		uint32_t bmpDataOffset = 0;
		uint32_t rowDataSize = 0;
		uint32_t rawDataSize = 0;
		uint32_t rowPadding = 0;
		uint32_t bmpFileSize = 0;
		uint32_t bytesInPixel = bitsInPixel / 8;


		uint32_t width = image.width;
		uint32_t height = image.height;
		void * pixels = image.pixels;
		format_t format = image.format;
		type_t type = image.type;

		if (format == RGB)
			bmpDataOffset = 14 + 40;
		else if ((format == RGBA) || (format == ARGB))
			bmpDataOffset = 14 + 108;
		else
		{
			printf ("Unsupported foramt to save in bmp\n");
			getchar();
			return -1;
		}

		rowDataSize = ceil((width*bitsInPixel) / 32.0) * 4; //width in bytes aligned by 4 bytes
		rowPadding = rowDataSize - (width*bitsInPixel) / 8; //bytes to align
		// printf ("%d\n", rowPadding);
		rawDataSize = rowDataSize*abs((int)height);
		bmpFileSize = bmpDataOffset + rawDataSize;

		fp = fopen(filePath, "wb");

		/*   BMP Header  (14 bytes)*/

		//BM
		fputc(0x42, fp);
		fputc(0x4D, fp);

		//Size of the BMP file
		writeToBmp(bmpFileSize, 4, fp);

		//Application specific
		fputc(0x00, fp);
		fputc(0x00, fp);

		//Application specific
		fputc(0x00, fp);
		fputc(0x00, fp);

		//Offset where the pixel array (bitmap data) can be found
		writeToBmp(bmpDataOffset, 4, fp);


		/*   DIB Header  */

		//Number of bytes in the DIB header (from this point) 
		if (format == RGB)
			writeToBmp(0x00000028, 4, fp); //(0x28 = 40)
		else if ((format == RGBA) || (format == ARGB))
			writeToBmp(0x0000006c, 4, fp); //(0x6c = 108)


		//Width of the bitmap in pixels
		writeToBmp(width, 4, fp);

		//Height of the bitmap in pixels
		writeToBmp(height, 4, fp);

		//Number of color planes being used (1 plane)
		fputc(0x01, fp);
		fputc(0x00, fp);

		//Number of bits per pixel
		fputc(bitsInPixel, fp);
		fputc(0x00, fp);

		if (format == RGB)
			//BI_RGB, no pixel array compression used
			writeToBmp(0x00000000, 4, fp);
		else if ((format == RGBA) || (format == ARGB))
			//BI_BITFIELDS, no pixel array compression used 
			writeToBmp(0x00000003, 4, fp);

		//Size of the raw bitmap data (including padding)
		writeToBmp(rawDataSize, 4, fp);

		//Print resolution of the image (2834.6472)
		writeToBmp(0x00000B13, 4, fp);
		writeToBmp(0x00000B13, 4, fp);

		//Number of colors in the palette
		writeToBmp(0x00000000, 4, fp);

		//0 means all colors are important
		writeToBmp(0x00000000, 4, fp);

		if ((format == RGBA) || (format == ARGB))
		{
			//Use (uint)ARGB to save in bmp because Windows show correctly only ARGB
			//Red channel bit mask
			writeToBmp(0x00FF0000, 4, fp);

			//Green channel bit mask
			writeToBmp(0x0000FF00, 4, fp);

			//Blue channel bit mask
			writeToBmp(0x000000FF, 4, fp);

			//Alpha channel bit mask
			writeToBmp(0xFF000000, 4, fp);


			//little-endian "Win ", LCS_WINDOWS_COLOR_SPACE
			writeToBmp(0x57696E20, 4, fp);

			//CIEXYZTRIPLE Color Space endpoints 
			for (i = 0; i < 9; i++) //0x24 = 36 0x00
			{
				writeToBmp(0x00000000, 4, fp);
			}

			//0 Red Gamma
			writeToBmp(0x00000000, 4, fp);

			//0 Green Gamma
			writeToBmp(0x00000000, 4, fp);

			//0 Blue Gamma
			writeToBmp(0x00000000, 4, fp);
		}



		//Pixel array (bitmap data)
		savePixelData(bitsInPixel, image, rowPadding, fp);

		fclose(fp);
		return 0;
	}

	/**
	* format - format which should be use to save pixels from bmp in data
	* type - type which should be use to save pixels from bmp in data
	*/
	unsigned int loadBMP_custom(const char * imagepath, format_t format, type_t type, image_t* image)
	{

		unsigned int dataPos;     // Position in the file where the actual data begins
		unsigned char bmpHeader[14]; // Each BMP file begins by a 14-bytes header
		unsigned char dibHeader[108]; // There is DIB header after BMP header (40 or 108 bytes)
		unsigned int imageSize;   // = width*height*3
		unsigned int dibHeaderSize = 0;
		uint32_t rowDataSize = 0;
		uint32_t rawDataSize = 0;
		uint32_t rowPadding = 0;
		uint32_t bitsInPixel = 24;
        	uint32_t bytesInPixel = 0;
		int intWidth = 0; //width for internal use
		int intHeight = 0; //height for internal use
		int i = 0;
		int j = 0;
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
		unsigned char a = 0;
		unsigned int redMask = 0;
		unsigned int greenMask = 0;
		unsigned int blueMask = 0;
		unsigned int alphaMask = 0;
		unsigned char isRGBA = 0;
		uint32_t compressionMethod = 0;
		unsigned int redPos = 0;
		unsigned int greenPos = 0;
		unsigned int bluePos = 0;
		unsigned int alphaPos = 0;
        int alignment = image->alignment;
		
		if (checkAlign(alignment) < 0)
		{		
			printf ("Default value 4 will be used.\n");
			image->alignment = 4;
			alignment = image->alignment;
		}

		// Open the file
		FILE* file = fopen(imagepath, "rb");
		if (!file) { 
			printf("BMP file '%s' could not be opened. Check file existance.\n", imagepath); 
            printf("Press any key to continue...\n"); 
            getchar();
            return 0; 

		}
		else
		{
			printf("BMP file '%s' was opened successfully.\n", imagepath);
		}

		if (fread(bmpHeader, 1, 14, file) != 14) { // If not 14 bytes read : problem
			printf("Trying to read BMP header... Not a correct BMP file\n");
			return 0;
		}

		if (bmpHeader[0] != 'B' || bmpHeader[1] != 'M') {
			printf("Not a correct BMP file\n");
			return 0;
		}

		dibHeaderSize = *(int*)&(bmpHeader[0x0A]) - 14; //14 = bmpHeader size

		if (fread(dibHeader, 1, dibHeaderSize, file) != dibHeaderSize) { // If not dibHeaderSize bytes read : problem
			printf("Trying to read DIB header... Not a correct BMP file\n");
			return 0;
		}

		// Read ints from the byte array
		dataPos = *(int*)&(bmpHeader[0x0A]);
		imageSize = *(int*)&(dibHeader[0x22-0x0E]); //Size of the raw bitmap data in bytes (including padding)
		intWidth = *(int*)&(dibHeader[0x12-0x0E]); //in pixels
		intHeight = *(int*)&(dibHeader[0x16-0x0E]);//in pixels
		bitsInPixel = *(unsigned char*)&(dibHeader[0x1C - 0x0E]);
		compressionMethod = *(int*)&(dibHeader[0x1E - 0x0E]);

		if (compressionMethod == 3)
		{
			isRGBA = 1; //BMP store R,G,B,A for each pixel
		}
		else if (compressionMethod == 0)
		{
			isRGBA = 0; //BMP store R,G,B for each pixel
		}
		else
		{
			printf("Unsupported compression method. Not a correct BMP file\n");
			return 0;
		}

		if (isRGBA)
		{
			printf ("BMP %s contains alpha component.", imagepath);

			redMask = *(int*)&(dibHeader[0x36 - 0x0E]);
			greenMask = *(int*)&(dibHeader[0x3A - 0x0E]);
			blueMask = *(int*)&(dibHeader[0x3E - 0x0E]);
			alphaMask = *(int*)&(dibHeader[0x42 - 0x0E]);

			redPos = getColorIndex(redMask);
			greenPos = getColorIndex(greenMask);
			bluePos = getColorIndex(blueMask);
			alphaPos = getColorIndex(alphaMask);

			if ((redMask != 0x00FF0000) || (greenMask != 0x0000FF00) || (blueMask != 0x000000FF) || (alphaMask != 0xFF000000))
			{
				printf("Unsupported BMP RGBA pixel format. In bytes should be B G R A B G R A. Not a correct BMP file\n");
				return 0;
			}
		}
		else 
		{
			printf ("BMP %s does not contain alpha component,\n", imagepath);
			printf ("so alpha value 255 will be used\n");
		}

		image->width = intWidth;
		image->height = intHeight;
		image->format = format;
		image->type = type;

        	bytesInPixel = bitsInPixel/8;
		rowDataSize = ceil((intWidth*bytesInPixel) / 4) * 4; //row width in bytes aligned by 4 bytes in BMP
		rowPadding = rowDataSize - (intWidth*bytesInPixel); //bytes to align in BMP

		// Some BMP files are misformatted, guess missing information for most common bmp (No compression, BI_RGB)
		if (imageSize == 0) imageSize = intWidth * intHeight * 3; // 3 : one byte for each Red, Green and Blue component. 
                                                                     // TODO: Alignment does not taking into accout!
                                                                     // TODO: show error if BMP is misformatted?
		if (dataPos == 0) dataPos = 54; // The BMP header is done that way

        //calc number of bytes for alignment for output image
		int outPixelBytes = 0;
		switch (format)
		{
 			case RGB:
			  outPixelBytes = 3;
			  break;
			
			case ARGB:
			case RGBA:
			  outPixelBytes = 4;
			  break;

			case ALPHA:
			case LUMINANCE:
			  outPixelBytes = 1;
			  break;

			case LUMINANCE_ALPHA:
			  outPixelBytes = 2;
			  break;
			  
			default:
			  outPixelBytes = 3;
			  break;
		}
		int outRowPaddingBytes = (intWidth*outPixelBytes)%alignment ? alignment - (intWidth*outPixelBytes)%alignment : 0;		
		unsigned int outRowWidthBytes = intWidth*outPixelBytes + outRowPaddingBytes; //including alignment bytes

		// Create a buffer for input image
		unsigned char* data;		
		data = (unsigned char*)malloc(imageSize * sizeof(unsigned char));
		
		// Create a buffer for output image
		unsigned int outImageSizeBytes = outRowWidthBytes * intHeight;
		image->pixels = (unsigned char*)malloc(outImageSizeBytes * sizeof(unsigned char));
		
		// Read the actual data from the file into the buffer
		fread(data, 1, imageSize, file);
		
		unsigned int pixelValue = 0;
		for (i = 0; i < intHeight; i++) //in pixels
		{
			for (j = 0; j < intWidth; j++) //in pixels
			{
				if (isRGBA == 0)
				{
					
					r = ((unsigned char*)data)[i*rowDataSize + j * 3 + 2];
					g = ((unsigned char*)data)[i*rowDataSize + j * 3 + 1];
					b = ((unsigned char*)data)[i*rowDataSize + j * 3];
					a = 255;

				}
				else //RGBA
				{
					unsigned int pixelValue = ((unsigned int*)data)[i*intWidth + j];
					r = (pixelValue & redMask) >> (redPos * 8);
					g = (pixelValue & greenMask) >> (greenPos * 8);
					b = (pixelValue & blueMask) >> (bluePos * 8);
					a = (pixelValue & alphaMask) >> (alphaPos * 8);	
				}
				
				if ((format == RGB) && (type == UNSIGNED_BYTE))
				{
					((unsigned char*)image->pixels)[i*outRowWidthBytes + j * bytesInPixel] = r;
					((unsigned char*)image->pixels)[i*outRowWidthBytes + j * bytesInPixel + 1] = g;
					((unsigned char*)image->pixels)[i*outRowWidthBytes + j * bytesInPixel + 2] = b;
				}
				else if ((format == RGBA) && (type == UNSIGNED_INT_8_8_8_8))
				{
					pixelValue = 0;
					pixelValue = pixelValue | ((unsigned int)r << 24);
					pixelValue = pixelValue | ((unsigned int)g << 16);
					pixelValue = pixelValue | ((unsigned int)b << 8);
					pixelValue = pixelValue | (unsigned int)a;
					
					((unsigned int*)image->pixels)[i*intWidth + j] = pixelValue;
				}
				else if ((format == ARGB) && (type == UNSIGNED_INT_8_8_8_8))
				{
					pixelValue = 0;
					pixelValue = pixelValue | ((unsigned int)a << 24);
					pixelValue = pixelValue | ((unsigned int)r << 16);
					pixelValue = pixelValue | ((unsigned int)g << 8);
					pixelValue = pixelValue | (unsigned int)b;

					((unsigned int*)image->pixels)[i*intWidth + j] = pixelValue;
				}
				else if ((format == RGBA) && (type == UNSIGNED_BYTE))
				{
					((unsigned char*)image->pixels)[i*outRowWidthBytes + j * outPixelBytes] = r;
					((unsigned char*)image->pixels)[i*outRowWidthBytes + j * outPixelBytes + 1] = g;
					((unsigned char*)image->pixels)[i*outRowWidthBytes + j * outPixelBytes + 2] = b;
					((unsigned char*)image->pixels)[i*outRowWidthBytes + j * outPixelBytes + 3] = a;
				}				
				else if ((format == ALPHA) && (type == UNSIGNED_BYTE))
				{
					((unsigned char*)image->pixels)[i*outRowWidthBytes + j*outPixelBytes] = a;
				}
				else if ((format == LUMINANCE) && (type == UNSIGNED_BYTE))
				{
					((unsigned char*)image->pixels)[i*outRowWidthBytes + j*outPixelBytes] = r;
				}
				else if ((format == LUMINANCE_ALPHA) && (type == UNSIGNED_BYTE))
				{
					((unsigned char*)image->pixels)[i*outRowWidthBytes + j*outPixelBytes] = r;
					((unsigned char*)image->pixels)[i*outRowWidthBytes + j*outPixelBytes] = a;
				}
				else
				{
					printf("Unsupported format and type when load from BMP\n"); //TODO: check it in the beginning of function
					printf("Each pixel in BMP contains 4 bytes. Try to use RGBA format.\n"); //TODO: check it in the beginning of function
				}

			}
		}
        
#ifdef WRITE_PIXELS_TO_FILE   
        char outFileName[256] = "";
        getNameFromPath(imagepath, outFileName);
        imageToHeader(image,outFileName);
#endif        
		//Everything is in memory now, the file can be closed
		fclose(file);

		return 0;
	}
    
    
int imageToHeader(image_t* image, const char* fileName)
{
    int i = 0;
    int j = 0;
    char outFilePath[256] = "";
   

	unsigned int alignment = image->alignment;
	
	if (checkAlign(alignment))
	{
		printf("%s: wrong alignment value (%d)", __func__, alignment);
		return -1;
	}

	unsigned int bytesInPixel = 0;
	switch (image->format)
	{
		case RGB:
		  bytesInPixel = 3;
		  break;
		
		case ARGB:
		case RGBA:
		  bytesInPixel = 4;
		  break;
	
		case ALPHA:
		case LUMINANCE:
		  bytesInPixel = 1;
		  break;
	
		case LUMINANCE_ALPHA:
		  bytesInPixel = 2;
		  break;
		  
		default:
		  bytesInPixel = 3;
	
		  break;
	}
	
	int rowPaddingBytes = (image->width * bytesInPixel) % alignment ? alignment - (image->width * bytesInPixel) % alignment : 0;	
	unsigned int imageRowWidthBytes = image->width * bytesInPixel + rowPaddingBytes; 

    strcat(outFilePath, fileName);
    strcat(outFilePath, ".cpp");
    
    FILE* outFile = fopen(outFilePath, "w");
    if (!outFile) { 
        printf("File '%s' could not be opened to write.\n", outFilePath); 
        printf("Press any key to continue...\n"); 
        getchar();
        return 0; 

    }
    else
    {
        printf("File '%s' was opened to write successfully.\n", outFilePath);
    }
    
    
    int array_size = 0;
 
	array_size = image->height * imageRowWidthBytes;
    
	fprintf ( outFile, "#include \"image.h\"\n");
	fprintf ( outFile, "#include \"texture_config.h\"\n");
	fprintf ( outFile, "\n");
	fprintf ( outFile, "#ifdef __GNUC__\n");
	fprintf ( outFile, "#define SECTION(sec) __attribute__((section(sec)))\n");
	fprintf ( outFile, "#else\n");
	fprintf ( outFile, "#define SECTION(sec)\n");
	fprintf ( outFile, "#endif // __GNUC__\n");
	fprintf ( outFile, "\n");
	fprintf ( outFile, "#ifdef __cplusplus\n");
	fprintf ( outFile, "extern \"C\"\n");
	fprintf ( outFile, "{\n");
	fprintf ( outFile, "#endif\n");
	fprintf ( outFile, "\n");
	fprintf ( outFile, "SECTION(TEXTURE_SECTION)\n");
    
    fprintf (outFile, "unsigned char pixels_%s[%d] = {  \n", fileName, array_size);
    
    
    
    for (i = 0; i < image->height; i++)
    {
        for (j = 0; j < image->width; j++)
        {
            if (image->format == RGB)
            {
                fprintf (outFile, "0x%02x, ", ((unsigned char*)image->pixels)[i*imageRowWidthBytes + j * bytesInPixel]);
                fprintf (outFile, "0x%02x, ", ((unsigned char*)image->pixels)[i*imageRowWidthBytes + j * bytesInPixel + 1]);
                fprintf (outFile, "0x%02x, ", ((unsigned char*)image->pixels)[i*imageRowWidthBytes + j * bytesInPixel + 2]);
            }
            else if ((image->format == RGBA) || (image->format == ARGB))
            {
                fprintf (outFile, "0x%02x, ", ((unsigned char*)image->pixels)[i*imageRowWidthBytes + j * bytesInPixel]);
                fprintf (outFile, "0x%02x, ", ((unsigned char*)image->pixels)[i*imageRowWidthBytes + j * bytesInPixel + 1]);
                fprintf (outFile, "0x%02x, ", ((unsigned char*)image->pixels)[i*imageRowWidthBytes + j * bytesInPixel + 2]);
                fprintf (outFile, "0x%02x, ", ((unsigned char*)image->pixels)[i*imageRowWidthBytes + j * bytesInPixel + 3]);
            }
			else if (image->format == ALPHA)
            {
                fprintf (outFile, "0x%02x, ", ((unsigned char*)image->pixels)[i*imageRowWidthBytes + j * bytesInPixel]);
            }
			else if (image->format == LUMINANCE)
            {
                fprintf (outFile, "0x%02x, ", ((unsigned char*)image->pixels)[i*imageRowWidthBytes + j * bytesInPixel]);
            }
			else if (image->format == LUMINANCE_ALPHA)
            {
                fprintf (outFile, "0x%02x, ", ((unsigned char*)image->pixels)[i*imageRowWidthBytes + j * bytesInPixel]);
                fprintf (outFile, "0x%02x, ", ((unsigned char*)image->pixels)[i*imageRowWidthBytes + j * bytesInPixel + 1]);
            }

            else
            {
                printf("Unsupported format when write pixels to file\n");
            }
        }
		
		for (j = 0; j < rowPaddingBytes; j++)
		{
			fprintf (outFile, "0x%02x, ", 0);
		}

		fprintf (outFile, "\n");

    }
    
    fprintf (outFile, "};  \n\n");
    
    fprintf ( outFile, "SECTION(TEXTURE_SECTION)\n");
    fprintf (outFile, "image_t image_%s = { \n", fileName);
    fprintf (outFile, "    %d,\n", image->width);
    fprintf (outFile, "    %d,\n", image->height);
    
    switch (image->format)
    {
        case RGB:
            fprintf (outFile, "    RGB,\n");
            break;
        
        case RGBA:
            fprintf (outFile, "    RGBA,\n");
            break;
            
        case ARGB:
            fprintf (outFile, "    ARGB,\n");
            break;

        case ALPHA:
            fprintf (outFile, "    ALPHA,\n");
            break;

        case LUMINANCE:
            fprintf (outFile, "    LUMINANCE,\n");
            break;

        case LUMINANCE_ALPHA:
            fprintf (outFile, "    LUMINANCE_ALPHA,\n");
            break;

        default:
            printf ("Unsupported format\n");
            return -1;
    }
    
    switch (image->type)
    {
        case UNSIGNED_INT_8_8_8_8:
            fprintf (outFile, "    UNSIGNED_INT_8_8_8_8,\n");
            break;
        
        case UNSIGNED_BYTE:
            fprintf (outFile, "    UNSIGNED_BYTE,\n");
            break;

        default:
            printf ("Unsupported type\n");
            return -1;
    }
    
    fprintf (outFile, "    pixels_%s\n", fileName);
    fprintf (outFile, "};  \n\n");

    fprintf ( outFile, "\n\n");
	fprintf ( outFile, "#ifdef __cplusplus\n");
	fprintf ( outFile, "}\n");
	fprintf ( outFile, "#endif\n");
        
    return 0;
}

int getNameFromPath (char* filePath, char* fileName)
{
    char slashes[] = "\\/";
    
    char* pch = 0;
    
    //search last slash
    char* last_slash = 0;
    last_slash = (char*)filePath;
    pch = strpbrk(filePath, slashes);
 
    while (pch != 0)
    {
        last_slash = pch;
        pch = strpbrk(pch+1, slashes);
    }
    
    //search first point from right
    
    char* last_point = 0;
    
    last_point = strrchr(filePath, '.');
    if (last_point == 0) //NULL
    {
        printf ("Error. No point in image path. Extension not found.\n\n");
        return -1;
    }
    
    strncpy(fileName, last_slash + 1,last_point - last_slash - 1);
    // printf ("%s\n", outFileName);
    
    return 0;
}

int checkAlign (unsigned int alignment)
{
	if ((alignment != 1) && (alignment != 2) && 
		(alignment != 4) && (alignment != 8)) 
	{
		printf ("Unsupported alignment value for image (%d).\n", alignment);
		printf ("Supported values are 1, 2, 4, 8.\n");
		return -1;
	}
	return 0;
}
