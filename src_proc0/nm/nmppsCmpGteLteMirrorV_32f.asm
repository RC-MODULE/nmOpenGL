// void nmppsCmpGteLteMirrorV_32f(const nm32f *srcArray, const nm32f *wArray, nm1* evenFlags, nm1* oddFlags, int size);

global _nmppsCmpGteLteMirrorV_32f: label;// объявление глобальной метки
extern ___errno: label;

begin ".text_demo3d"                // начало секции кода
<_nmppsCmpGteLteMirrorV_32f>    // определение глобальной метки
    ar5 = ar7 - 2;
    
    push ar0,gr0;
    push ar1,gr1;
    push ar2,gr2;
    push ar3,gr3;
    push ar4,gr4;
    push ar6,gr6;   // don't forget about pop
    
    ar0 = [--ar5];  // input array
    ar1 = [--ar5];  // delta array
    ar2 = [--ar5];  // output array of even flags (старшие слова: 0, 2, 4)
    ar3 = [--ar5];  // output array of odd flags (младшие слова: 1, 3, 5)
    gr4 = [--ar5];  // input array's size

    // Сейчас size должен быть чётным
    // Для нечётного на выходе в одной из масок будет на один флаг меньше
    // Определить, сколько 64-битных (2*float) значений надо обработать
    gr4 >>= 1;  // Size of 64-bit values to process

<Loop>    
	gr4;
	if <= goto Exit;
	// Clear (zero) fp0_lmask and fp0_hmask
	// because they should be filled with 0 when contains less than 32 flags
    sir = 0;
    fp0_lmask = sir;
    fp0_hmask = sir;

    gr6 = 32;           // if number of elements left is more than 32 
    gr4 - gr6;          // process 32 64-bit values
    if >= goto Compare; 
    gr6 = gr4;         // else process just the left elements
<Compare>
	gr3 = gr6;
	gr3--;
	vlen = gr3;
    // Загрузить очередные значения в векторные регистры 
    fpu 0 rep vlen vreg0 = [ar0++];         
    fpu 0 rep vlen vreg1 = [ar1++];
    
    // Проверить значения на выход за правую границу диапазона
    fpu 0 .float vreg0 - vreg1, set mask if <=;

    // Сохранить промежуточные значения масок 
    sir = fp0_lmask;
    gr0 = sir;
    sir = fp0_hmask;
    gr1 = sir;
    
    // Проверить значения на выход за левую границу диапазона
    fpu 0 .float vreg0 + vreg1, set mask if >=;    
    
    // Полученный результат перемножить на промежуточное значение
    sir = fp0_lmask;
    gr3 = sir;
    gr0 = gr0 and gr3;

    sir = fp0_hmask;
    gr3 = sir;
    gr1 = gr1 and gr3;

    // Получившиеся значения масок записать в выходной массив
    // Сейчас size должен быть чётным
    // Для нечётного на выходе в одной из масок будет на один флаг меньше
    [ar2++] = gr0;
    [ar3++] = gr1;

    goto Loop with gr4 = gr4 - gr6;	// Decrease overall count by the number of processed elements

<Exit>
    pop ar6,gr6;
    pop ar4,gr4;
    pop ar3,gr3;
    pop ar2,gr2;
    pop ar1,gr1;
    pop ar0,gr0;

    return;          // возврат из функции, возвращаемое значение хранится в gr7
end ".text_demo3d";      // признак окончания секции кода
