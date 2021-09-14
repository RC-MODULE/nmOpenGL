extern "C" {
  void deselectElementsByIndices(void* src, int* indices, void* dst, int sizeOfElementInt, int size) {

    int* curSrcPos = (int*)src;
    int* curDstPos = (int*)dst;
    for (int i=0; i<size; i++) {
      curDstPos = (int*)dst + indices[i]*sizeOfElementInt;
      for (int j=0; j<sizeOfElementInt; j++) {
        curDstPos[j] = curSrcPos[j];
      }
      curSrcPos = curSrcPos + sizeOfElementInt;
    }
  }
}
