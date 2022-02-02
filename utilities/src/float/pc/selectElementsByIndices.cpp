extern "C" {
  void selectElementsByIndices(void* src, int* indices, void* dst, int sizeOfElementInt, int size) {

    int* curSrcPos = (int*)src;
    int* curDstPos = (int*)dst;
    for (int i=0; i<size; i++) {
      curSrcPos = (int*)src + indices[i]*sizeOfElementInt;
      for (int j=0; j<sizeOfElementInt; j++) {
        curDstPos[j] = curSrcPos[j];
      }
      curDstPos = curDstPos + sizeOfElementInt;
    }
  }
}
