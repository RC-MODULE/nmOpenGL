//void convertABGR32_RGB565(const abgr32  *srcArray, rgb565 *dstArray, int count);

global _convertABGR32_RGB565: label; // объявление глобальной метки.
    
extern ___errno: label;

data ".data_demo3d"        // секция инициализированных данных
    // массив для хранения промежуточного результата вычислений
    // [rgb565_0, rgb565_1, rgb565_2, rgb565_3]
    RGB565_tmp_array: long[4];
    // 64-битный вектор для хранения промежуточного результата
    RGB565x4_tmp: long;
    
    // Маска для входа X
    rgMsk: long = 00000003f0000001fhl;
    rgbMsk: long[4] = (0000000000000ffffhl, 000000000ffffffffhl, 00000ffffffffffffhl, 0ffffffffffffffffhl);
    
    // матрица весовых коэффициентов для сдвига вправо 
    // старших 6 бит в каждом 32-битном слове
    // long (6, 26, (5+1), 26) -> long (44, 6, (5+1))
    rgMatrix: long[8] = (
                        00000000000000800hl,    // shift r and g
                        00000000000000000hl,
                        00000000000000020hl,
                        00000000000000000hl,
                        00000000000000001hl,    // shift b
                        00000000000000000hl,
                        00000000000000000hl,
                        00000000000000000hl
                        );

    // матрица весовых коэффициентов для распределения
    // четырёх значений RGB565 в 64-битном слове
    // rgb565_3|rgb565_2|rgb565_1|rgb565_0|
    rgbMatrix: long[8] =    (
                            00000000000000001hl,
                            00000000000000000hl,
                            00000000000010000hl,
                            00000000000000000hl,
                            00000000100000000hl,
                            00000000000000000hl,
                            00001000000000000hl,
                            00000000000000000hl
                            );

end ".data_demo3d";  
    
begin ".text_demo3d"      // начало секции кода.
<_convertABGR32_RGB565>    

    ar5 = ar7 - 2;
    push ar0,gr0;
    push ar1,gr1;
    push ar2,gr2;
    push ar3,gr3;
    push ar4,gr4;
    push ar6,gr6;

    ar0 = [--ar5];          // Адрес входного массива
    ar1 = [--ar5];          // Адрес выходного массива
    gr4 = [--ar5];          // Количество элементов во входном массиве

    gr0 = 4;
    ar3 = ar0 + 2;
    gr3 = 4;

<Convert>
    gr4;
    if <= goto Exit;
    // Определить, сколько элементов осталось (>4 или 1-3)
    // По умолчанию, считается, что конвертировали 4 значения.
    // Если оставшихся элементов 1-3, то конвертируются только оставшиеся.
    // gr1 здесь хранит количество элементов, которые потом надо будет поместить
    // в выходной массив (чтобы не затирать имеющиеся)
    gr1 = 4;    // По умолчанию распределяем 4 элемента RGB565
    gr6 = 4;
    gr4 - gr6;  // Осталось меньше 4?
    if >= goto Convert4Values;
    gr1 = gr4;  // Распределить надо будет только оставшиеся (1-3)

<Convert4Values>
    gr6 = 4;    // Всегда обрабатываем 4 элемента, но разложить, возможно, надо будет меньше
    ar6 = RGB565_tmp_array;

<ConvertOneValue>

    // Выделить компоненты 65
    nb1l = 08410h; // разбиение матрицы весовых коэффициентов
    nb1h = 00h;
    sb = 082h;

    ar2 = rgMatrix;  
    rep 8 wfifo = [ar2++], ftw, wtw;

    ar2 = rgMsk; 
    // Загрузить маску в ram
    rep 1 ram = [ar2];

    // операция взвешенного суммирования
    rep 1 data = [ar0++gr0] with vsum ram, data, 0;
    ftw, wtw;
    rep 1 data = [ar3++gr3] with vsum ram, data, afifo;
    rep 1 [ar6++] = afifo;

    gr6 = gr6 - 1;      // Обработано ещё одно слово

if > goto ConvertOneValue;  // Если ещё не обработали 4 элемента

    // Упаковать 4 значения RGB565 из RGB565_tmp_array в 64-битное слово RGB565x4_tmp
    nb1 = 080008000h;
    sbl = 020000h;
    sbh = 00h;

    ar2 = rgbMatrix;
    rep 8 wfifo = [ar2++], ftw, wtw;
    
    ar6 = RGB565_tmp_array;
    ar4 = RGB565x4_tmp;

    // Получить промежуточный результат
    rep 1 data = [ar6++] with vsum , data, 0;
    ftw, wtw;
    rep 1 data = [ar6++] with vsum , data, afifo;
    ftw, wtw;
    rep 1 data = [ar6++] with vsum , data, afifo;
    ftw, wtw;
    rep 1 data = [ar6++] with vsum , data, afifo;
    rep 1 [ar4] = afifo;

    // Скопировать только нужные значения в выходное слово:
    // Выбрать маску
    gr1 - 1;
    if =0 goto UseMask1;
    gr6 = 2;
    gr1 - gr6;
    if =0 goto UseMask2;
    gr6 = 3;
    gr1 - gr6;
    if =0 goto UseMask3;
    ar2 = rgbMsk[3];
    goto LoadMask;
<UseMask1>    
    ar2 = rgbMsk[0];
    goto LoadMask;
<UseMask2>    
    ar2 = rgbMsk[1];
    goto LoadMask;
<UseMask3>    
    ar2 = rgbMsk[2];

<LoadMask>
    rep 1 ram = [ar2];
    // Загрузить исходное слово в afifo
    rep 1 data = [ar1] with data;
    rep 1 data = [ar4] with mask ram, data, afifo;
    rep 1 [ar1++] = afifo;
    
    goto Convert with gr4 = gr4 - gr1;

<Exit>
    pop ar6,gr6;
    pop ar4,gr4;
    pop ar3,gr3;
    pop ar2,gr2;
    pop ar1,gr1;
    pop ar0,gr0;

    return; 
end ".text_demo3d";       // признак окончания секции кода.
