//void selectElementsByOffsets(void* src, int* offsets, void* dst, int sizeOfElementInt, int size)
//sizeOfElementInt must be < 2^26 to gr3*gr7 fit into word(32 bits)

begin ".text_demo3d"
global _selectElementsByOffsets: label;
<_selectElementsByOffsets>
  ar5 = sp - 2;
  push ar0, gr0;
  push ar1, gr1;
  push ar2, gr2;
  push ar3, gr3;
  push ar4, gr4;
  push ar5, gr5;
  ar0 = [--ar5];
  ar1 = [--ar5];
  ar2 = [--ar5];
  gr3 = [--ar5] with gr2=false;
  gr1 = [--ar5] with gr4=false;

<NextOffset>
  gr2 - gr1;
  if =0 delayed goto EndProgram;
    gr7 = [ar1++]; 
    with gr0 = gr3 *: gr7; //gr3 = sizeOfElementInt 
.repeat 15;
  with gr0 = gr3 * gr7;
.endrepeat;
  gr0 = gr7; //gr7 contains lower 32 bits of multiplication
  ar0 = ar0 + gr0;
<NextInt>
    gr4 - gr3;
    if =0 delayed goto AllIntsProcessed;
      gr0 = -gr3; 
      gr4++; 
    delayed goto NextInt;
      gr5 = [ar0++]; 
      [ar2++] = gr5; 
<AllIntsProcessed>
  ar0 = ar0 + gr0;
  delayed goto NextOffset;
    gr4 = false; 
    gr2++; 
<EndProgram>
  pop ar5, gr5;
  pop ar4, gr4;
  pop ar3, gr3;
  pop ar2, gr2;
  pop ar1, gr1;
  pop ar0, gr0;
return;
end ".text_demo3d";
