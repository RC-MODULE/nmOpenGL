//void _inverse_v4nm8u(const v4nm8u *srcArray, v4nm8u *dstArray, int count);

    

data ".data_demo3d"        // секция инициализированных данных
    Weights:	long[8] = (
		00000000001000000hl,
		00000000000010000hl,
		00000000000000100hl,
		00000000000000001hl,
		00100000000000000hl,
		00001000000000000hl,
		00000010000000000hl,
		00000000100000000hl);
end ".data_demo3d";  

extern vec_vsum_data_0: label;
    
global _inverse_v4nm8u: label; // объявление глобальной метки.
begin ".text_demo3d"      // начало секции кода.
<_inverse_v4nm8u>    
    ar5 = ar7 - 2;
    push ar0,gr0	with gr7 = gr5;
    push ar6,gr6	with gr0 = false;

    ar0 = [--ar5]	with gr0++;    					// Адрес входного массива
    ar6 = [--ar5]	with gr0++;          			// Адрес выходного массива
    gr5 = [--ar5]	with gr6 = gr0;       			// Количество элементов во входном массиве

    sir = 80808080h;
	nb1 = sir;
	sir = 2020202h ;
	sb = sir;
	ar5 = Weights;
	rep 8 wfifo = [ar5++], ftw, wtw;
	
	delayed call vec_vsum_data_0;
		gr5 >>= 1;
		nul;

    pop ar6,gr6;
    pop ar0,gr0	with gr5 = gr7;

    return; 
end ".text_demo3d";       // признак окончания секции кода.
