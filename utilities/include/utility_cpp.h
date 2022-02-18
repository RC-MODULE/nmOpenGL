#ifndef __UTILITY_CPP_H__
#define __UTILITY_CPP_H__

/*!
 *  \brief Класс, хранящий побитовую маску.
 *  
 *  Побитовая маска используется для выставления и определения принадлежности примитива одному из сегментов изображения.
 *  
 *  Если в соответствующем бите стоит 1 - значит примитив принадлежит сегменту, иначе не принадлежит. Так же структура хранит в себе переменную, характеризующую
    наличие ненулевых битов в маске
	
 *  \author Жиленков Иван
 */
class BitMask {
public:
	int* bits;
	int hasNotZeroBits; 

	void init(nm1* mask) {
		bits = (int*)mask;
		hasNotZeroBits = 0;
	}

	int get(int index) {
		int word = bits[index / 32];
		word >>= index % 32;
		return word & 1;
	}

	void put(int index, int value) {
		int word = bits[index / 32];
		int mask = 1 << (index % 32);
		value <<= index % 32;
		word = ( word & ~mask ) | ( value & mask );
		bits[index / 32] = word;
	}
};


/*!
 *  \brief Класс, хранящий побитовую маску в раздельных массивах.
 *  Принцип схож с принципом структуры BitMask, однако в данной структуре маска разделена на четные и нечетные элементы
 *  \author Жиленков Иван
 */
class BitDividedMask{
public:
	BitMask even;
	BitMask odd;

	void init(nm1* evenMask, nm1* oddMask) {
		even.init(evenMask);
		odd.init(oddMask);
	}

	int get(int index) {
		if (index % 2) {
			return odd.get(index / 2);
		}
		else {
			return even.get(index / 2);
		}
	}

	void put(int index, int value) {
		if (index % 2) {
			odd.put(index / 2, value);
		}
		else {
			even.put(index / 2, value);
		}
	}
};

#endif //__UTILITY_H__