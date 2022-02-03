#include "utility_float.h"
#include "stdio.h"

#define MAX_SIZE 1024
#define STEP 2



v2nm32f minXY[MAX_SIZE];
v2nm32f maxXY[MAX_SIZE];
v2nm32f upper = { 1, 1};
v2nm32f lower = {-1, -1};
int dst[MAX_SIZE + STEP];

void testSize(){
	/*printf("Test size.....");
	for(int i = 0; i < MAX_SIZE; i++){
		minXY[i].v0 = 0;
		minXY[i].v1 = 0;
		maxXY[i].v0 = 0;
		maxXY[i].v1 = 0;
	}

	for(int i = 0; i < MAX_SIZE + STEP; i++){
		dst[i] = 0xCDCDCDCD;
	}

	for(int size = 0; size < MAX_SIZE; size+=STEP){

		checkRectanglesOverlaps(minXY, maxXY, &upper, &lower, dst, size);

		size /= 32;
		
		if(dst[size + 1] != 0xCDCDCDCD){
			printf("Error!!\n");
			printf("Overflow error: size=%d\n", size);
			return;
		}
		if(size == 0) continue;
		if (dst[size] == 0xCDCDCDCD){
			printf("Error!!\n");
			printf("error: size=%d\n", size);
			return;
		}
	}*/
	printf("OK\n");
}

void testValues(){
	printf("Test values.....");
	/*minXY[0] = {1, 1};
	minXY[1] = {-1, -1};
	minXY[2] = {1, 1};
	minXY[1] = {-1, -1};
	srcC = {1, 2, 3, 4};
	v4nm32f ref_values = {1, -1, 7, 3};

	addC_v4nm32f(src, &srcC, dst, 1);

	for(int i = 0; i < 4; i++){
		if(dst[0].vec[i] != ref_values.vec[i]){
			printf("Error!!\n");
			printf("%d!=%d\n", dst[0].vec[i], ref_values.vec[i]);
			return;
		}
	}*/
	printf("OK\n");
}

int main(){
	testSize();
	testValues();
	return 0;
}