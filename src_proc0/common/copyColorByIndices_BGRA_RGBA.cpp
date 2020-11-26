#include "demo3d_common.h"
#include "nmtype.h"


SECTION(".text_demo3d")
void copyColorByIndices_BGRA_RGBA(v4nm32s* srcColor, int* indices, v4nm32s* dstColor, int size) {
	int* src = (int*)srcColor;
	int* dst = (int*)dstColor;
	for (int i = 0; i < size; i++) {
		dst[4 * i + 0] = src[4 * indices[i] + 2];
		dst[4 * i + 1] = src[4 * indices[i] + 1];
		dst[4 * i + 2] = src[4 * indices[i] + 0];
		dst[4 * i + 3] = src[4 * indices[i] + 3];
	}
}
