//void convertRGB565_RGB8888(const rgb565 *srcArray, rgb8888 *dstArray, int count);

global _convertRGB565_RGB8888: label; // объявление глобальной метки.
extern ___errno: label;

data ".data_demo3d"        // секция инициализированных данных
    // исходный вектор
    RGB565x4: long = 0FFFF55555555C979hl;
    // место для хранения результата вычислений
    R8: long = 0l;
    G8: long = 0l;
    B8: long = 0l;
    GB88h: long = 0l;
    GB88l: long = 0l;

    // Маска для входа X
    rMsk: long = 0f800f800f800f800hl;
    gMsk: long = 007e007e007e007e0hl;
    bMsk: long[2] = (0001f001f001f001fhl, 000FF00FF00FF00FFhl);
    gbMskh: long = 000FF00FFFFFFFFFFhl;
    gbMskl: long = 0FFFFFFFF00FF00FFhl;

    // массив rMatrix содержит матрицу весовых коэффициентов
    // для получения компонента r
    rMatrix: long[10] = (
                        00000000000000000hl,
                        00000000000000100hl,
                        00000000000000000hl,
                        00000000001000000hl,
                        00000000000000000hl,
                        00000000000000000hl,
                        00000010000000000hl,
                        00000000000000000hl,
                        00000000000000000hl,
                        00100000000000000hl
                        );
    // массив gMatrix содержит матрицу весовых коэффициентов
    // для получения компонента g
    gMatrix: long[24] = (
                        00000000000000000hl dup 3,
                        00000000000000004hl,
                        00000000000000010hl,
                        00000000000000000hl dup 4,
                        00000000000040000hl,
                        00000000000100000hl,
                        00000000000000000hl dup 4,
                        00000000400000000hl,
                        00000001000000000hl,
                        00000000000000000hl dup 4,
                        00004000000000000hl,
                        00010000000000000hl,
                        00000000000000000hl
                        );
    
    // массив bMatrix содержит матрицу весовых коэффициентов
    // для получения компонента b
    bMatrix: long[12] = (
                        00000000000000040hl,
                        00000000000000000hl dup 2,
                        00000000000400000hl,
                        00000000000000000hl dup 2,
                        00000004000000000hl,
                        00000000000000000hl dup 2,
                        00040000000000000hl,
                        00000000000000000hl dup 2
                        );

    rgbMatrix: long[32] =  (
                           00000000000000100hl,    // 00000000rrggrrgg -> 00rrgg0000rrgg00
                           00000000000010000hl,
                           00000010000000000hl,
                           00001000000000000hl,
                           00000000000000000hl dup 4,
                           00000000000000001hl,    // 0000000000bb00bb -> 000000bb000000bb
                           00000000000000000hl,
                           00000000100000000hl,
                           00000000000000000hl dup 5,
                           00000000000000000hl dup 4,     // rrggrrgg00000000 -> 00rrgg0000rrgg00
                           00000000000000100hl,
                           00000000000010000hl,
                           00000010000000000hl,
                           00001000000000000hl,
                           00000000000000000hl dup 4,     // 00bb00bb00000000 -> 000000bb000000bb
                           00000000000000001hl,
                           00000000000000000hl,
                           00000000100000000hl,
                           00000000000000000hl
                           );

end ".data_demo3d";  
    
begin ".text_demo3d"      // начало секции кода.
<_convertRGB565_RGB8888>    

    ar5 = ar7 - 2;
    push ar0,gr0;
    push ar1,gr1;
    push ar2,gr2;
    push ar3,gr3;
    push ar4,gr4;
    push ar6,gr6;

    ar0 = [--ar5];          // Адрес входного массива
    gr2 = [--ar5];          // Адрес выходного массива
    gr0 = [--ar5];          // Количество элементов во входном массиве

	gr0 >>= 2;
	gr0 <<= 2;
    // Проверить размер входного массива на равенство 0
    // Проверяется один раз при входе в функцию
    gr0;
    if <= goto Exit;

<Loop>
    ar1, gr1 = [ar0++];     // Переписать в регистровую пару входное значение
    [RGB565x4] = ar1, gr1;    // Переписать входное значение в переменную RGB565x4

<Convert>
    // Выделить компонент R
    ar1 = rMatrix;  
    nb1 = 088808880h; // разбиение матрицы весовых коэффициентов
    sbl = 020022000h;
    sbh = 022022202h;
    // весовые коэффициенты загружаются в буфер wfifo, одновременно с этим они 
    // транслируются в теневую матрицу, а затем и в рабочую.
    rep 10 wfifo = [ar1++], ftw, wtw;
    
    ar2 = RGB565x4;
    ar3 = rMsk; 
    ar4 = R8;
    
    // Загрузить маску в ram
    rep 1 ram = [ar3];
    
    // операция взвешенного суммирования
    rep 1 data = [ar2] with vsum ram, shift data, 0;
    rep 1 data = [ar2] with mask ram, data, afifo;
    rep 1 [ar4] = afifo;


    // Выделить компонент G
    ar1 = gMatrix;
    nb1 = 0a28aa28ah;   // разбиение матрицы весовых коэффициентов
    sb = 08a2a8a2ah;
    rep 24 wfifo = [ar1++], ftw, wtw;
   
    ar2 = RGB565x4;
    ar3 = gMsk; 
    ar4 = G8;
   
    // Загрузить маску в ram
    rep 1 ram = [ar3];

    // операция взвешенного суммирования
    rep 1 data = [ar2] with vsum ram, shift data, 0;
    rep 1 data = [ar2] with mask ram, shift data, afifo;
    
    // (можно оптимизировать с помощью взвешенного суммирования)
    rep 1 with mask , shift afifo, 0;
    rep 1 with mask , shift afifo, 0;
    rep 1 [ar4] = afifo;


    // Выделить компонент B
    ar1 = bMatrix;
    nb1 = 088208820h;   // разбиение матрицы весовых коэффициентов
    sb = 020822082h;
    rep 12 wfifo = [ar1++], ftw, wtw;
   
    ar2 = RGB565x4;
    ar3 = bMsk;
    // В B8 синий компонент начинается с бита 2 (0 - младший, справа) 
    ar4 = B8;
   
    // Загрузить маску в ram
    rep 1 ram = [ar3++];

    // Сдвинуть слово на 6 разрядов влево
    rep 1 data = [ar2] with vsum ram, data, 0;
    // Сдвинуть afifo на один разряд вправо
    rep 1 with mask , shift afifo, 0;
    // Логически сложить (or) afifo с data
    rep 1 data = [ar2] with mask ram, data, afifo;
    // Сдвинуть afifo на два разряда вправо
    // (можно оптимизировать с помощью взвешенного суммирования)
    rep 1 with mask , shift afifo, 0;
    rep 1 ram = [ar3++] with mask , shift afifo, 0;
    rep 1 with mask ram, afifo, 0;
    rep 1 [ar4] = afifo;

    
    // Обработать младшую часть результата
    ar1 = rgbMatrix;
    nb1 = 080808080h;   // разбиение матрицы на 8 столбцов по 8 бит
    sb  = 002020202h;   // разбиение матрицы на 8 строк по 8 бит
    rep 32 wfifo = [ar1++], ftw, wtw;   // 32 строки (8 строк для 4-х суммирований).
                                        //  Сначала загрузятся первые 8 строк
    ar2 = R8;
    ar3 = G8;
    ar4 = B8;
    ar5 = GB88l;
    ar6 = gbMskl;
    rep 1 ram = [ar6];
    
    // Загрузить в afifo R8
    rep 1 data = [ar2] with mask ram, 0, data;
    // Логически сложить R8 и G8
    rep 1 data = [ar3] with mask ram, data, afifo;
    // Сдвинуть R8 и G8
    rep 1 with vsum , afifo, 0;       
    // Сдвинуть B8 и сложить его с R8 и G8
    ftw, wtw;                           // Загрузить следующие 8 строк
    rep 1 data = [ar4] with vsum , data, afifo;
    rep 1 [ar5] = afifo;
    
    // Обработать старшую часть результата
    ar5 = GB88h;
    ftw,wtw;                            // Загрузить следующие 8 строк
    ar6 = gbMskh;
    rep 1 ram = [ar6];
    // Загрузить в afifo R8
    rep 1 data = [ar2] with mask ram, 0, data;
    // Логически сложить R8 и G8
    rep 1 data = [ar3] with mask ram, data, afifo;
    // Сдвинуть R8 и G8
    rep 1 with vsum , afifo, 0;       
    // Сдвинуть B8 и сложить его с R8 и G8
    ftw, wtw;                           // Загрузить следующие 8 строк
    rep 1 data = [ar4] with vsum , data, afifo;
    // Результат операции выгрузить из afifo в память
    rep 1 [ar5] = afifo;    // GB88h
    
    // Восстановить текущий адрес в выходном массиве
    ar2 = gr2;
    
    // Распределить полученные 64-битные значения по 32-битным
    ar6, gr6 = [GB88l];
    [ar2++] = ar6 with gr0 = gr0 - 1;
    if =0 goto Exit;
    [ar2++] = gr6 with gr0 = gr0 - 1;
    if =0 goto Exit;

    ar6, gr6 = [GB88h];
    [ar2++] = ar6 with gr0 = gr0 - 1;
    if =0 goto Exit;
    [ar2++] = gr6 with gr0 = gr0 - 1;
    
    // cохранить текущий адрес в выходном массиве
    // для использования его на следующей итерации
    gr2 = ar2;
    // Если количество значений, которые надо обработать, не равно 0,
    // продолжить обработку 
    if > goto Loop;
    // else
    goto Exit;

    // Примечание: флаги для "if > goto Loop" установлены операцией в правой части
    // последней скалярной операции, т.е. [ar2++] = gr6 with gr0 = gr0 - 1;
    // Установка флагов происходит только путем выполнения арифметическо-логической операции 
    // в правой части скалярной команды (rg_asm_ru.docx, 5.1.9.5)
    // gr2 = ar2; - команда копирования регистров, которая может располагаться только 
    // в левой части ассемблерной инструкции (rg_asm_ru.docx, 5.1.5), следовательно, эта операция
    // флагов не меняет.

<Exit>
    pop ar6,gr6;
    pop ar4,gr4;
    pop ar3,gr3;
    pop ar2,gr2;
    pop ar1,gr1;
    pop ar0,gr0;

    return; 
end ".text_demo3d";       // признак окончания секции кода.
