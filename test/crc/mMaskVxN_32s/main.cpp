#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"

#define		TRIANGLE_WIDTH 		32
#define		TRIANGLE_HEIGHT 	32
#define		TRIANGLE_SIZE		TRIANGLE_WIDTH * TRIANGLE_HEIGHT
#define		TRIANGLES_CNT		16
//#define		IMAGE_SIZE			TRIANGLES_CNT * TRIANGLE_SIZE
#define		IMAGE_WIDTH			128
#define		IMAGE_HEIGHT		128
#define		IMAGE_SIZE			IMAGE_WIDTH * IMAGE_HEIGHT

#define		TRIANGLES_SIZE		IMAGE_SIZE
#define		MASKS_SIZE			TRIANGLES_SIZE


SECTION(".data_imu1") nm32s image[IMAGE_SIZE+128];
SECTION(".data_imu2") nm32s triangles[TRIANGLES_CNT*TRIANGLE_WIDTH*TRIANGLE_HEIGHT];
SECTION(".data_imu2") nm32s trianglesHeights[TRIANGLES_CNT];
SECTION(".data_imu2") nm32s trianglesWidths[TRIANGLES_CNT];
SECTION(".data_imu3") nm32s masks[TRIANGLES_CNT*TRIANGLE_WIDTH*TRIANGLE_HEIGHT];
SECTION(".data_imu3") nm32s* roi[TRIANGLES_CNT];

	extern "C" void mMaskVxN_32s(nm32s* pTriangles, nm32s* pMask, nm32s** pROI, int imageStride, int* pTrianglesHeight, int* pTrianglesWidth, int count);
	
int crcTest()
{
	unsigned crc = 123;
	int maxImageSize= IMAGE_SIZE;
	int maxImgWidth = IMAGE_WIDTH;
	int maxImgHeight= IMAGE_HEIGHT;
	int maxWidth    = TRIANGLE_WIDTH;
	int maxHeight   = TRIANGLE_HEIGHT;
	int maxCount    = TRIANGLES_CNT;

	nm32s* img_32s       = (nm32s*)image;
	nm32s* masks_32s     = (nm32s*)masks;
	nm32s* triangles_32s = (nm32s*)triangles;

	nmppsRandUniform_32s(img_32s, maxImageSize+128);
	nmppsRandUniform_32s(masks_32s, maxCount*maxWidth*maxHeight);
	nmppsRandUniform_32s(triangles_32s, maxCount*maxWidth*maxHeight);


	for (int imgWidth = 64; imgWidth <= maxImgWidth; imgWidth += 2) {
		for(int count = 1; count <= maxCount; count++)
		{
			for (int i = 0; i<count; i++) {
				trianglesHeights[i] = nmppcRandMinMaxDiv(1, 32, 1);
				trianglesWidths[i]  = nmppcRandMinMaxDiv(2, 32, 2);
			}

			for (int i=0; i<count; i++) {
				int xImg       = nmppcRandMinMaxDiv(0, imgWidth - trianglesWidths[i] - 1,2); // xImg &= (~3);
				int yImg       = nmppcRandMinMax(0, maxImgHeight - trianglesHeights[i] - 1);
				nm32s* addrRoi = nmppsAddr_32s(img_32s, yImg * imgWidth + xImg);
				roi[i]         = addrRoi;
			}
			mMaskVxN_32s(triangles_32s, masks_32s, roi, imgWidth, trianglesHeights, trianglesWidths, count);
			nmppsCrcAcc_32s(img_32s, maxImageSize+128, &crc);
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

	nm32s* img_32s       = (nm32s*)image;
	nm32s* masks_32s     = (nm32s*)masks;
	nm32s* triangles_32s = (nm32s*)triangles;

	nmppsRandUniform_32s(img_32s, maxImageSize);
	nmppsRandUniform_32s(masks_32s, maxCount*maxWidth*maxHeight);
	nmppsRandUniform_32s(triangles_32s, maxCount*maxWidth*maxHeight);

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
				int xImg       = nmppcRandMinMaxDiv(0, imgWidth - trianglesWidths[i] - 1, 2); // xImg &= (~3);
				int yImg       = nmppcRandMinMax(0, maxImgHeight - trianglesHeights[i] - 1);
				nm32s* addrRoi = nmppsAddr_32s(img_32s, yImg*imgWidth + xImg);
				roi[i]         = addrRoi;
			}
			int minT = 0xFFFFFFF;
			for (int i = 0; i < 16; i++) {
				t0 = clock();
				mMaskVxN_32s(triangles_32s, masks_32s, roi, imgWidth, trianglesHeights, trianglesWidths, count);
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
	//if (crc != 0x2ca3cfe0)
	//	return 1111;

	//perfomanceTest();
	return 777;
}
/*
int main1()
{
	clock_t t1;
	clock_t t2;
	nmppsSet_32s((nm32s*)image,     0, IMAGE_SIZE);
	nmppsSet_32s((nm32s*)masks,     0, MASKS_SIZE * 2);
	nmppsSet_32s((nm32s*)triangles, 0, TRIANGLES_SIZE * 2);

	nm32s* image_32s     = (nm32s*)image;
	nm32s* masks_32s     = (nm32s*)masks;
	nm32s* triangles_32s = (nm32s*)triangles;

	nmppsSet_32s(masks_32s,    -1, IMAGE_SIZE*2);
	nmppsSet_32s(image_32s,   777, IMAGE_SIZE*2);
	nmppsSet_32s(triangles_32s, 2, IMAGE_SIZE*2);

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

				roi[iTriangle] = nmppsAddr_32s(image_32s, offset);

				// nm32s* nextMask     = nmppsAddr_32s(masks_32s, offset);
				// nm32s* nextTriangle = nmppsAddr_32s(triangles_32s, offset);

				for(int i = 0; i < i * width; i++) {
					nmppsPut_32s(triangles_32s, i + offset, 2 * i + 1);
				}

				for(int i = 0; i < i * width >> 1; i++) {
					nmppsPut_32s(masks_32s, 2 * i + offset, -2 * i);
				}

				for(int i = 0; i < i * width >> 1; i++) {
					nmppsPut_32s(masks_32s, 2 * i + 1 + offset, 2 * i + 1);
				}
			}
		}

		t1 = clock();
		mMaskVxN_32s(triangles_32s, masks_32s, roi, width, trianglesHeights, trianglesWidths, TRIANGLES_CNT);
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
				nmppsGetVal_32s(triangles_32s, j * TRIANGLE_WIDTH + i + offset, &triangleValue);
				printf("%d  ", triangleValue);
			}
			printf("\n");
		}

		printf("\n");
		printf("Masks\n");
		for (int j = 0; j < TRIANGLE_HEIGHT; j++) {
			for (int i = 0; i < TRIANGLE_WIDTH; i++) {
				int maskValue;
				nmppsGetVal_32s(masks_32s, j * TRIANGLE_WIDTH + i + offset, &maskValue);
				printf("%d  ", maskValue);
			}
			printf("\n");
		}
		printf("\n");
		printf("Image\n");
		for (int j = 0; j < TRIANGLE_HEIGHT; j++) {
			for (int i = 0; i < TRIANGLE_WIDTH; i++) {
				int imageValue;
				nmppsGetVal_32s(image_32s, j * TRIANGLE_WIDTH + i + offset, &imageValue);
				printf("%d  ", imageValue);
			}
			printf("\n");
		}
		printf("\n");
		printf("ROI\n");
		for (int j = 0; j < TRIANGLE_HEIGHT; j++) {
			for (int i = 0; i < TRIANGLE_WIDTH; i++) {
				int roiValue;
				nmppsGetVal_32s(roi[0], j * TRIANGLE_WIDTH + i + offset, &roiValue);
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
