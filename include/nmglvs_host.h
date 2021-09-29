#ifndef __NMGLVS_HOST_H__
#define __NMGLVS_HOST_H__

int nmglvsGetWidth();
int nmglvsGetHeight();
int nmglvsHostReadImage(int* dstImage);
int nmglvsHostInit();
int nmglvsExit(unsigned *result);

#endif
