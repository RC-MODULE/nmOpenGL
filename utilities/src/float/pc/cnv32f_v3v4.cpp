#include "nmtype.h"

extern "C" {

void cnv32f_v3v4(nm32f* src_v3nm32f, nm32f* dst_v4nm32f, float value4, int size) {
	for (int i = 0; i < size; i++) {
		dst_v4nm32f[4 * i + 0] = src_v3nm32f[3 * i + 0];
		dst_v4nm32f[4 * i + 1] = src_v3nm32f[3 * i + 1];
		dst_v4nm32f[4 * i + 2] = src_v3nm32f[3 * i + 2];
		dst_v4nm32f[4 * i + 3] = value4;
	}
	
}

}