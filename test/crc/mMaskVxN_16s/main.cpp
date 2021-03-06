#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"

#define		TRIANGLE_WIDTH 		32
#define		TRIANGLE_HEIGHT 	32
#define		TRIANGLE_SIZE		TRIANGLE_WIDTH * TRIANGLE_HEIGHT
#define		TRIANGLES_CNT		32
//#define		IMAGE_SIZE			TRIANGLES_CNT * TRIANGLE_SIZE
#define		IMAGE_WIDTH			128
#define		IMAGE_HEIGHT		128
#define		IMAGE_SIZE			IMAGE_WIDTH * IMAGE_HEIGHT

#define		TRIANGLES_SIZE		IMAGE_SIZE
#define		MASKS_SIZE			TRIANGLES_SIZE


SECTION(".data_imu1") nm32s image[IMAGE_SIZE+128];
SECTION(".data_imu2") nm32s triangles[TRIANGLES_CNT*TRIANGLE_WIDTH*TRIANGLE_HEIGHT/2];
SECTION(".data_imu2") nm32s trianglesHeights[TRIANGLES_CNT];
SECTION(".data_imu2") nm32s trianglesWidths[TRIANGLES_CNT];
SECTION(".data_imu3") nm32s masks[TRIANGLES_CNT*TRIANGLE_WIDTH*TRIANGLE_HEIGHT/2];
SECTION(".data_imu3") nm16s* roi[TRIANGLES_CNT];

	extern "C" void mMaskVxN_16s(nm16s* pTriangles, nm16s* pMask, nm16s** pROI, int imageStride, int* pTrianglesHeight, int* pTrianglesWidth, int count);
	
int crcTest()
{
	unsigned crc = 123;
	int maxImageSize= IMAGE_SIZE;
	int maxImgWidth = IMAGE_WIDTH;
	int maxImgHeight= IMAGE_HEIGHT;
	int maxWidth    = TRIANGLE_WIDTH;
	int maxHeight   = TRIANGLE_HEIGHT;
	int maxCount    = TRIANGLES_CNT;

	nm16s* img_16s       = (nm16s*)image;
	nm16s* masks_16s     = (nm16s*)masks;
	nm16s* triangles_16s = (nm16s*)triangles;

	nmppsRandUniform_16s(img_16s, maxImageSize+128*2);
	nmppsRandUniform_16s(masks_16s, maxCount*maxWidth*maxHeight);
	nmppsRandUniform_16s(triangles_16s, maxCount*maxWidth*maxHeight);


	for (int imgWidth = 64; imgWidth <= maxImgWidth; imgWidth += 4) {
		for(int count = 1; count <= maxCount; count++)
		{
			for (int i = 0; i<count; i++) {
				trianglesHeights[i] = nmppcRandMinMaxDiv(1, 32, 1);
				trianglesWidths[i]  = nmppcRandMinMaxDiv(4, 32, 4);
			}

			for (int i=0; i<count; i++) {
				int xImg       = nmppcRandMinMaxDiv(0, imgWidth - trianglesWidths[i] - 1,4); // xImg &= (~3);
				int yImg       = nmppcRandMinMax(0, maxImgHeight - trianglesHeights[i] - 1);
				nm16s* addrRoi = nmppsAddr_16s(img_16s, yImg * imgWidth + xImg);
				roi[i]         = addrRoi;
			}
			mMaskVxN_16s(triangles_16s, masks_16s, roi, imgWidth, trianglesHeights, trianglesWidths, count);
			nmppsCrcAcc_16s(img_16s, maxImageSize+128*2, &crc);
		}
	}
	return crc>>2;
}

int perfomanceTest()
{
	unsigned crc = 123;

	int maxImageSize = IMAGE_SIZE;
	int maxImgWidth  = IMAGE_WIDTH;
	int maxImgHeight = IMAGE_HEIGHT;
	int maxWidth     = TRIANGLE_WIDTH;
	int maxHeight    = TRIANGLE_HEIGHT;
	int maxCount     = TRIANGLES_CNT;

	nm16s* img_16s       = (nm16s*)image;
	nm16s* masks_16s     = (nm16s*)masks;
	nm16s* triangles_16s = (nm16s*)triangles;

	nmppsRandUniform_16s(img_16s, maxImageSize);
	nmppsRandUniform_16s(masks_16s, maxCount*maxWidth*maxHeight);
	nmppsRandUniform_16s(triangles_16s, maxCount*maxWidth*maxHeight);

	clock_t t0, t1;
	int imgWidth = maxImgWidth;
	//for (int count = 1; count <= maxCount; count++)
	{
		int count = maxCount;
		int w = 32;
		for (int h = 1; h <= 32; h++) {

			for (int i = 0; i < count; i++) {
				trianglesHeights[i] = h;
				trianglesWidths[i] = w;
			}

			for (int i = 0; i < count; i++) {
				int xImg       = nmppcRandMinMaxDiv(0, imgWidth - trianglesWidths[i] - 1, 4); // xImg &= (~3);
				int yImg       = nmppcRandMinMax(0, maxImgHeight - trianglesHeights[i] - 1);
				nm16s* addrRoi = nmppsAddr_16s(img_16s, yImg*imgWidth + xImg);
				roi[i]         = addrRoi;
			}
			int minT = 0xFFFFFFF;
			for (int i = 0; i < 16; i++) {
				t0 = clock();
				mMaskVxN_16s(triangles_16s, masks_16s, roi, imgWidth, trianglesHeights, trianglesWidths, count);
				t1 = clock();
				minT = (t1 - t0) < minT ? (t1 - t0) : minT;
			}
			printf("h=%d w=%d count=%d time=%d \n", h, w, count, minT);
		}
	}

	return 0;
}

int main()
{

	int crc = crcTest();
	printf("crc=%x\n", crc);
	if (crc != 0x2ca3cfe0)
		return 1111;

	perfomanceTest();
	return 777;
}
/*
int main1()
{
	clock_t t1;
	clock_t t2;
	nmppsSet_16s((nm16s*)image,     0, IMAGE_SIZE);
	nmppsSet_16s((nm16s*)masks,     0, MASKS_SIZE * 2);
	nmppsSet_16s((nm16s*)triangles, 0, TRIANGLES_SIZE * 2);

	nm16s* image_16s     = (nm16s*)image;
	nm16s* masks_16s     = (nm16s*)masks;
	nm16s* triangles_16s = (nm16s*)triangles;

	nmppsSet_16s(masks_16s,    -1, IMAGE_SIZE*2);
	nmppsSet_16s(image_16s,   777, IMAGE_SIZE*2);
	nmppsSet_16s(triangles_16s, 2, IMAGE_SIZE*2);

	unsigned crc = 0;

	for(int height = 1; height < TRIANGLE_HEIGHT; height++) {
		int width = 4;
		for(int iTriangle = 0; iTriangle < TRIANGLES_CNT; iTriangle++) {
			trianglesHeights[iTriangle] = i;
			trianglesWidths[iTriangle]  = width;

			if(width == TRIANGLE_WIDTH) {
				width = 4;
			}

			for(int iTriangle = 0, offset = 0;
					iTriangle < TRIANGLES_CNT;
					iTriangle++, offset += i * width) {

				roi[iTriangle] = nmppsAddr_16s(image_16s, offset);

				// nm16s* nextMask     = nmppsAddr_16s(masks_16s, offset);
				// nm16s* nextTriangle = nmppsAddr_16s(triangles_16s, offset);

				for(int i = 0; i < i * width; i++) {
					nmppsPut_16s(triangles_16s, i + offset, 2 * i + 1);
				}

				for(int i = 0; i < i * width >> 1; i++) {
					nmppsPut_16s(masks_16s, 2 * i + offset, -2 * i);
				}

				for(int i = 0; i < i * width >> 1; i++) {
					nmppsPut_16s(masks_16s, 2 * i + 1 + offset, 2 * i + 1);
				}
			}
		}

		t1 = clock();
		mMaskVxN_16s(triangles_16s, masks_16s, roi, width, trianglesHeights, trianglesWidths, TRIANGLES_CNT);
		t2 = clock();
		nmppsCrcAcc_32s(image, IMAGE_SIZE  , &crc);
		width += 4;
	}




	for(int iTriangle = 0, offset = 0;
			iTriangle < TRIANGLES_CNT * 2;
			iTriangle++, offset += TRIANGLE_SIZE) {
		printf("Triangles[ %d ]\n", iTriangle);

		for (int j = 0; j < TRIANGLE_HEIGHT; j++) {
			for (int i = 0; i < TRIANGLE_WIDTH; i++) {
				int triangleValue;
				nmppsGetVal_16s(triangles_16s, j * TRIANGLE_WIDTH + i + offset, &triangleValue);
				printf("%d  ", triangleValue);
			}
			printf("\n");
		}

		printf("\n");
		printf("Masks\n");
		for (int j = 0; j < TRIANGLE_HEIGHT; j++) {
			for (int i = 0; i < TRIANGLE_WIDTH; i++) {
				int maskValue;
				nmppsGetVal_16s(masks_16s, j * TRIANGLE_WIDTH + i + offset, &maskValue);
				printf("%d  ", maskValue);
			}
			printf("\n");
		}
		printf("\n");
		printf("Image\n");
		for (int j = 0; j < TRIANGLE_HEIGHT; j++) {
			for (int i = 0; i < TRIANGLE_WIDTH; i++) {
				int imageValue;
				nmppsGetVal_16s(image_16s, j * TRIANGLE_WIDTH + i + offset, &imageValue);
				printf("%d  ", imageValue);
			}
			printf("\n");
		}
		printf("\n");
		printf("ROI\n");
		for (int j = 0; j < TRIANGLE_HEIGHT; j++) {
			for (int i = 0; i < TRIANGLE_WIDTH; i++) {
				int roiValue;
				nmppsGetVal_16s(roi[0], j * TRIANGLE_WIDTH + i + offset, &roiValue);
				printf("%d  ", roiValue);
			}
			printf("\n");
		}
		printf("\n");
		printf("\n");
	}
	printf("crc = 0x%x\n", crc >> 2);
	return t2 - t1;
}
*/
