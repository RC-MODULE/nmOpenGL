data ".data_demo3d"
  dupC: long = 0hl;
  dupVal: long = 0hl;
end ".data_demo3d";

//void cmpLtCSelVC_f(float* src, float C, int* val1, int val2, float* dst, int size);
//Calculate dst[i] = src[i] < C ? val1[i] : val2
global _cmpLtCSelVC_f: label;

begin ".text_demo3d"
<_cmpLtCSelVC_f>
  ar5 = ar7 - 2;
  push ar4, gr4;
  push ar3, gr3;
  push ar2, gr2;
  push ar1, gr1;
  push ar0, gr0;

  ar0 = [--ar5]; // src
  ar3 = [--ar5]; // C
  ar1 = [--ar5]; // val1 
  ar4 = [--ar5]; // val2 
  ar2 = [--ar5]; // dst 
  gr1 = [--ar5]; // size
  
  with gr1;
  if =0 delayed goto exit_cmpLtCSelVC_f;
    gr3 = ar3;
    gr4 = ar4;

  [dupC] = ar3,gr3;
  ar3 = dupC;

  [dupVal] = ar4,gr4;
  ar4 = dupVal with gr3 = false;

  fpu 0 rep 32 vreg1 = [ar3];
  fpu 0 rep 32 vreg3 = [ar4];
  ar3 = gr3;
  sir = ar3, gr3 with gr2 = gr1 >> 6; 
  if =0 delayed goto less64_cmpLtCSelVC_f;
    fp0_lmask = sir with gr3++; //also set gr3=1
    fp0_hmask = sir;

<cmpLtCSelVC_f>
  fpu 0 rep 32 vreg0 = [ar0++];// read 64 elements from src
  fpu 0 rep 32 vreg2 = [ar1++];// read 64 elements form val1 
  fpu 0 .float vreg0 - .retrive(vreg1), set mask if <;
  fpu 0 .float vreg2 = not mask ? vreg3 : vreg2;
  fpu 0 rep 32 [ar2++] = vreg2;
  gr2--;
  if > delayed goto cmpLtCSelVC_f;
    nul;
    nul;
  gr1 = gr1 << 26; // calc residue 
  gr1 = gr1 >> 26; // calc residue  
  if =0 delayed goto exit_cmpLtCSelVC_f;// if residue is 0 then all elements are processed
    nul;
    nul;

<less64_cmpLtCSelVC_f>
  gr0 = gr1 - 1; //calc vlen (count = gr1/2 - 1 = (gr1-1)/2, vlen = count - 1)
  gr2 = gr1 with gr0 = gr0 >> 1; //remember residue count and calc vlen (count = gr1/2 - 1 = (gr1-1)/2, vlen = count - 1)
  vlen = gr0;
  fpu 0 rep vlen vreg0 = [ar0++];
  fpu 0 rep vlen vreg2 = [ar1++];
  fpu 0 rep vlen vreg3 = [ar4];
  fpu 0 .float vreg4 = vreg0 - .retrive(vreg1), set mask if <;
  
  //If residue count is odd then extra odd word (starting from 0) in dst will be corrupted.
  //So it is necessary to restore that word. 
  gr2 and gr3; //check residue count
  if =0 delayed goto residueCountIsEven; //else - residue count is odd - need to backup odd word in dst 
    ar0=ar2+gr2;
  gr0=[ar0];
  fpu 0 .float vreg2 = not mask ? vreg3 : vreg2;
  fpu 0 rep vlen [ar2++] = vreg2;
  delayed goto exit_cmpLtCSelVC_f;
    [ar0]=gr0;
    nul;

<residueCountIsEven>
  fpu 0 .float vreg2 = not mask ? vreg3 : vreg2;
  fpu 0 rep vlen [ar2++] = vreg2;

<exit_cmpLtCSelVC_f>
  pop ar0, gr0;
  pop ar1, gr1;
  pop ar2, gr2;
  pop ar3, gr3;
  pop ar4, gr4;
  return;
end ".text_demo3d";
