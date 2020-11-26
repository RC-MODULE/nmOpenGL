#include "nmpp.h"
#include "time.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "demo3d_nm1.h"
#include "service.h"

__attribute__((section(".mem_bank0"))) long long im00[2048];
__attribute__((section(".mem_bank0"))) long long im01[2048];
__attribute__((section(".mem_bank1"))) long long im10[2048];
__attribute__((section(".mem_bank1"))) long long im11[2048];
__attribute__((section(".mem_bank1"))) long long im13[2048];
__attribute__((section(".mem_bank2"))) long long im20[2048];
__attribute__((section(".mem_bank2"))) long long im21[2048];
__attribute__((section(".mem_bank3"))) long long im30[2048];
__attribute__((section(".mem_bank3"))) long long im31[2048];

__attribute__((section(".data_DDR"))) long long emi0[4096];
__attribute__((section(".data_DDR"))) long long emi1[4096];

int main()
{
  return 0;
}
