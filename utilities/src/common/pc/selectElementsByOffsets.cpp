extern "C" {
  void selectElementsByOffsets(void* src, int* offsets, void* dst, int sizeOfElementInt, int size) {

    int* curSrcPos = (int*)src;
    int* curDstPos = (int*)dst;
    for (int i=0; i<size; i++) {
      curSrcPos = curSrcPos + offsets[i]*sizeOfElementInt;
      for (int j=0; j<sizeOfElementInt; j++) {
        curDstPos[j] = curSrcPos[j];
      }
      curDstPos = curDstPos + sizeOfElementInt;
    }
  }
}
