//void convertABGR32_RGB565(const abgr32  *srcArray, rgb565 *dstArray, int count);

global _convertABGR32_RGB565: label; // объявление глобальной метки.
    
data ".data_demo3d"        // секция инициализированных данных
    // массив для хранения промежуточного результата вычислений
    RGB565_array: long[32];
    
    // Маска для выделения пары 65 (gr или ab) из 64-битного слова
    rgMask: long = 00000003f0000001fhl;
    
    // матрица весовых коэффициентов для сдвига вправо 
    // младших 6 бит в каждом 32-битном слове
    // long (26, 6, 26, (5+1)) -> long (44, 6, (5+1))
    rgMatrix: long[8] = (

                        00000000000000800hl,    // shift r and g
                        00000000000000000hl,
                        00000000000000020hl,
                        00000000000000000hl,
                        00000000000000001hl,    // shift b
                        00000000000000000hl dup 3
                        );

    weights: long[16] = (
						00000000000000001hl,
						00000000000000000hl dup 3,
						00000000000010000hl,
						00000000000000000hl dup 3,
						00000000100000000hl,
						00000000000000000hl dup 3,
						00001000000000000hl,
						00000000000000000hl dup 3
                        );
end ".data_demo3d";  

macro convertN(N)
    // Сформировать массив из N неупакованных значений RGB565,
	// каждое хранится в 64-битном слове
    nb1l = 08410h;
    nb1h = 00h;
    sb = 082h;

    ar3 = rgMatrix;  
    rep 8 wfifo = [ar3++], ftw, wtw;

    ar3 = rgMask; 
    rep N ram = [ar3];

	ar3 = RGB565_array;
	rep N data = [ar0++gr0] with vsum ram, data, 0;
	ftw, wtw;
	rep N data = [ar2++gr2] with vsum ram, data, afifo;
	rep N [ar3++] = afifo;

	// Плотно упаковать N значений RGB565 (по 4 в 64-битном слове)
    nb1 = 080008000h;
    sb = 020002h;

    ar3 = weights;
    rep 16 wfifo = [ar3++], ftw, wtw;
    
    ar3 = RGB565_array;
	ar6 = ar3;
	gr6 = 8;
	rep N>>2 data = [ar6++gr6] with vsum , data, 0;
	ftw, wtw;
	ar6 = ar3 + 2;
	rep N>>2 data = [ar6++gr6] with vsum , data, afifo;
	ftw, wtw;
	ar6 = ar3 + 4;
	rep N>>2 data = [ar6++gr6] with vsum , data, afifo;
	ftw, wtw;
	ar6 = ar3 + 6;
	rep N>>2 data = [ar6++gr6] with vsum , data, afifo;
	rep N>>2 [ar1++] = afifo;
	nul;nul;
end convertN;

begin ".text_demo3d"      // начало секции кода.
<_convertABGR32_RGB565>    

	ConvertTail:label;

    ar5 = ar7 - 2;
    push ar0,gr0 with gr7 = gr5;
    push ar1,gr1;
    push ar2,gr2;
    push ar3,gr3;
    push ar4,gr4;
    push ar6,gr6;

    ar0 = [--ar5];          // Адрес входного массива
    ar1 = [--ar5];          // Адрес выходного массива
    gr4 = [--ar5];          // Количество элементов во входном массиве

    gr0 = 4 with gr5 = gr4 << 27;	// gr0 - step for RG, gr5 - offset for tail
    ar2 = ar0 + 2 with gr2 = gr0;	// BA (from RGBA) start and step for BA

    gr4;
    if =0 delayed goto Exit with gr5 >>=27;
		gr4 >>= 5;// get the number of 32 reps
		nul;
	
<Next32Reps>
	if =0 delayed goto StartTail;
	nul;
	nul;

	convertN(32);
	goto Next32Reps with gr4--;

<StartTail>
	// If remainder (gr5) is 0 goto Exit
	// Else calculate the number of tail reps and corresponding offset:
	// offset - (nearest multiple of remainder and 4) - 1
	// Subtraction of 1 is necessary because there are no calculations for the 
	// zero remainder and all calculations start from 0 offset
	// offset (gr5) = (remainder (gr5) + 3) / 4 - 1
	gr5;	
	if =0 delayed goto Exit;
		gr3 = 3;
		gr5 += gr3;
	gr5 >>= 2;
	gr5--;
	ar5 = ConvertTail with gr5 <<= 6;
	goto ar5 + gr5;
<ConvertTail>
	convertN(4); goto Exit;
	convertN(8); goto Exit;
	convertN(12); goto Exit;
	convertN(16); goto Exit;
	convertN(20); goto Exit;
	convertN(24); goto Exit;
	convertN(28); goto Exit;
	convertN(32); goto Exit;

<Exit>
    pop ar6,gr6;
    pop ar4,gr4;
    pop ar3,gr3;
    pop ar2,gr2;
    pop ar1,gr1;
    pop ar0,gr0 with gr5 = gr7;

    return; 
end ".text_demo3d";       // признак окончания секции кода.
