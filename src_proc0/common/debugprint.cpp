
#include "stdio.h"
#include "demo3d_nm0.h"

void printDataForNmpu1(DataForNmpu1* data, int elementsAmount) {
	printf("address=%p, count=%d\n", data, data->count);
	elementsAmount = MIN(elementsAmount, data->count);
	for (int i = 0; i < elementsAmount; i++) {
		printf("i=%d\n", i);
		printf("v0={%d,%d}, ", data->x0[i], data->y0[i]);
		printf("v1={%d,%d}, ", data->x1[i], data->y1[i]);
		printf("v2={%d,%d}\n", data->x2[i], data->y2[i]);
		// TEXTURING_PART
		printf("tex s0,t0={%d,%d}, ", data->s0[i], data->t0[i]);
		printf("tex s1,t1={%d,%d}, ", data->s1[i], data->t1[i]);
		printf("tex s2,t2={%d,%d}\n", data->s2[i], data->t2[i]);
		// TEXTURING_PART
	}

}

void printTriangles(Triangles* data, int elementsAmount) {
	printf("address=%p, count=%d\n", data, data->size);
	elementsAmount = MIN(elementsAmount, data->size);
	for (int i = 0; i < elementsAmount; i++) {
		printf("i=%d\n", i);
		printf("v0={%f,%f,%d}, ", data->x0[i], data->y0[i], data->z[i]);
		printf("v1={%f,%f,%d}, ", data->x1[i], data->y1[i], data->z[i]);
		printf("v2={%f,%f,%d}\n", data->x2[i], data->y2[i], data->z[i]);
		// TEXTURING_PART
		printf("tex s0,t0={%f,%f}, ", data->s0[i], data->t0[i]);
		printf("tex s1,t1={%f,%f}, ", data->s1[i], data->t1[i]);
		printf("tex s2,t2={%f,%f}\n", data->s2[i], data->t2[i]);
		// TEXTURING_PART
	}

}

void printTrianglePointers(TrianglePointers* data, int elementsAmount) {
	printf("address=%p\n", data);
	elementsAmount = MIN(elementsAmount, NMGL_SIZE);
	for (int i = 0; i < elementsAmount; i++) {
		printf("i=%d\n", i);
		printf("v0={%4.2f,%4.2f,%4.2f}, ", data->v0.x[i], data->v0.y[i], data->v0.z[i]);
		printf("v1={%4.2f,%4.2f,%4.2f}, ", data->v1.x[i], data->v1.y[i], data->v1.z[i]);
		printf("v2={%4.2f,%4.2f,%4.2f}\n", data->v2.x[i], data->v2.y[i], data->v2.z[i]);
		// TEXTURING_PART
		printf("tex s0,t0={%4.2f,%4.2f}, ", data->v0.s[i], data->v0.t[i]);
		printf("tex s1,t1={%4.2f,%4.2f}, ", data->v1.s[i], data->v1.t[i]);
		printf("tex s2,t2={%4.2f,%4.2f}\n", data->v2.s[i], data->v2.t[i]);
		// TEXTURING_PART
	}

}

void printWindowInfo(WindowInfo* info, int mode){
	printf("WindowInfo=%p\n", info);
	printf("Segment size={%d, %d}\n", info->segmentWidth, info->segmentHeight);
	printf("nColumns=%d, nRows=%d\n", info->nColumns, info->nRows);

	switch (mode) {
	case 1:
		printf("int\n");
		for (int y = 0; y < info->nRows; y++) {
			printf("lowerLeft : ");
			for (int x = 0; x < info->nColumns; x++) {
				printf("{%3d,%3d} ", info->x0[x], info->y0[y]);
			}
			printf("\n");
			printf("upperRight: ");
			for (int x = 0; x < info->nColumns; x++) {
				printf("{%3d,%3d} ", info->x1[x], info->y1[y]);
			}
			printf("\n\n");
		}
		break;
	case 2:
		printf("float\n");
		for (int y = 0, iSeg = 0; y < info->nRows; y++) {
			printf("lowerLeft : ");
			for (int x = 0; x < info->nColumns; x++, iSeg++) {
				printf("{%3.0f,%3.0f} ", info->x0_f[x], info->y0_f[y]);
			}
			printf("\n");
			iSeg -= info->nColumns;
			printf("upperRight: ");
			for (int x = 0; x < info->nColumns; x++, iSeg++) {
				printf("{%3.0f,%3.0f} ", info->x1_f[x], info->y1_f[y]);
			}
			printf("\n\n");
		}
		break;

	case 3:
		printf("v2nm32f\n");
		for (int y = 0, iSeg = 0; y < info->nRows; y++) {
			printf("lowerLeft : ");
			for (int x = 0; x < info->nColumns; x++, iSeg++) {
				printf("{%3.0f,%3.0f} ", info->lowerLeft[iSeg].v0, info->lowerLeft[iSeg].v1);
			}
			printf("\n");
			
			iSeg -= info->nColumns;
			printf("upperRight: ");
			for (int x = 0; x < info->nColumns; x++, iSeg++) {
				printf("{%3.0f,%3.0f} ", info->upperRight[iSeg].v0, info->upperRight[iSeg].v1);
			}
			printf("\n\n");
		}
		break;
	default:
		break;
	}

}

void printBitMask(BitMask *bitmask, int nSeg, int elementAmount){
	printf("BitMasks=%p\n", bitmask);
	for (int i = 0; i < nSeg; i++) {
		if (bitmask[i].hasNotZeroBits) {
			printf("    Segment No%d has non-zero bits\n", i);
			printf("        ");
			for(int s = 0; s < elementAmount;s+=32){
				printf("0x%x ", bitmask[i].bits[s/32]);
			}
			printf("\n");
		}
		else {
			printf("    Segment No%d hasn't non-zero bits\n", i);
			printf("        ");
			for(int s = 0; s < elementAmount;s+=32){
				printf("0x%x ", bitmask[i].bits[s/32]);
			}
			printf("\n");
		}
	}
	printf("--------\n");
}
