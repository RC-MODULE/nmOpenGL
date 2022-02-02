#include "nmtype.h"

extern "C" {
void selectColorChannel(v4nm8s* srcImage, int channel, nm8s* dst, int size){
	char* src = (char*)srcImage;
	for(int i=0; i< size; i++){
		dst[i] = src[4 * i + channel];
	}
}
}
