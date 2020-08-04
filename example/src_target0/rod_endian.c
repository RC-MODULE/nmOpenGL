#include <rod_endian.h>
void EndianVariable(uint8_t* mem, size_t size)
{
  size_t size2 = (size --) >> 1;
  size_t i;
  for (i = 0; i < size2; i ++)
  {
    register uint8_t a = mem[i];
    register uint8_t b = mem[size - i];
    mem[i] = b;
    mem[size - i] = a;
  }
}
