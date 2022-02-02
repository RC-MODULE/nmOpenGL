MEMORY
{
  C0_IMU0 : ORIGIN = 0x00000200<<2, LENGTH = 0x00007E00<<2
  C0_IMU1 : ORIGIN = 0x00008000<<2, LENGTH = 0x00008000<<2
  C0_IMU2 : ORIGIN = 0x00010000<<2, LENGTH = 0x00008000<<2
  C0_IMU3 : ORIGIN = 0x00018000<<2, LENGTH = 0x00008000<<2
  C0_IMU4 : ORIGIN = 0x00020000<<2, LENGTH = 0x00008000<<2
  C0_IMU5 : ORIGIN = 0x00028000<<2, LENGTH = 0x00008000<<2
  C0_IMU6 : ORIGIN = 0x00030000<<2, LENGTH = 0x00008000<<2
  C0_IMU7 : ORIGIN = 0x00038000<<2, LENGTH = 0x00008000<<2
  SHMEM0  : ORIGIN = 0x000A8000<<2, LENGTH = 0x00008000<<2
  SHMEM1  : ORIGIN = 0x000B0000<<2, LENGTH = 0x00010000<<2
  EMI1    : ORIGIN = 0x20000000<<2, LENGTH = 0x08000000<<2 /*????*/
}

SECTIONS
{
  .text_imu0 : ALIGN(0x8)
  {
    *(.text_nmplv);
    *(.text_nmplm);
    *(.text_nmvcore);
    *(.text_demo3d);
  } > C0_IMU0
  .init : ALIGN(0x8)
  {
    *(.init);
  } > C0_IMU0
  .fini : ALIGN(0x8)
  {
    *(.fini);
  } > C0_IMU0
  .data : ALIGN(0x8)
  {
    *(.data);
    *(SORT(.data._*))
    *(.data_demo3d);
  } > C0_IMU0
  .data_imu0 : ALIGN(0x8)
  {
    *(.data_imu0);
    *(.data_demo3d);
    *(.data_nmplv);
    *(.data_nmplv_L);
  } > C0_IMU0
  .bss : ALIGN(0x8)
  {
    *(.bss);
  } > C0_IMU0
  .stack : ALIGN(0x8)
  {
    *(.stack);
    *libmc12101_stub_nmc4_fixed.a:*(.stack);
  } > C0_IMU0


  .data_imu1 : ALIGN(0x8)
  {
    *(.data_imu1);
    *(.data_nmplv_G);
  } > C0_IMU1


  .data_imu2 : ALIGN(0x8)
  {
    *(.data_imu2);
  } > C0_IMU2
  .heap2 (NOLOAD): ALIGN(0x8)
  {
    *(.heap2);
  . = ORIGIN(C0_IMU2) + LENGTH(C0_IMU2);
    __HEAP_2_END = .;
  } > C0_IMU2


  .data_imu3 : ALIGN(0x8)
  {
    *(.data_imu3);
  } > C0_IMU3
  .heap3 (NOLOAD): ALIGN(0x8)
  {
    *(.heap3);
  . = ORIGIN(C0_IMU3) + LENGTH(C0_IMU3);
    __HEAP_3_END = .;
  } > C0_IMU3


  /*.data_shmem0 : ALIGN(0x8)
  {
    *(.data_shmem0);
  } > SHMEM0
  .heap8 (NOLOAD): ALIGN(0x8)
  {
    *(.heap8);
  . = ORIGIN(SHMEM0) + LENGTH(SHMEM0);
    __HEAP_8_END = .;
  } > SHMEM0*/


  .data_shmem1 : ALIGN(0x8)
  {
    *(.data_shmem1);
  } > SHMEM1
  .heap9 (NOLOAD): ALIGN(0x8)
  {
    *(.heap9);
  . = ORIGIN(SHMEM1) + LENGTH(SHMEM1);
    __HEAP_9_END = .;
  } > SHMEM1


  .text_emi1 : ALIGN(0x8)
  {
    *(.text_hal_libc);
    *(.text_hal);
  } > EMI1
  .text_nmprofiler : ALIGN(0x8)
  {
    *(.text_nmprofiler);
  } > EMI1
  .text : ALIGN(0x8)
  {
    *(.text);
    *(SORT(.text._*))
    *libmc12101_stub_nmc4_fixed.a:*(.text)
  } > EMI1
  .data_emi1 : ALIGN(0x8)
  {
    *(.data_emi1);
    *(.data_shared1);
    *(.data_shared3);
    *libmc12101_stub_nmc4_fixed.a:*(.data)
  } > EMI1
  .rpc_services : ALIGN(0x8)
  {
    *(.rpc_services);
  } > EMI1
  .rpc_services.bss : ALIGN(0x8)
  {
    *(.rpc_services.bss);
  } > EMI1
  .heap (NOLOAD): ALIGN(0x8)
  {
    *(.heap);
  } > EMI1
  .heap1 (NOLOAD): ALIGN(0x8)
  {
    *(.heap1);
  } > EMI1
  .heap11 (NOLOAD): ALIGN(0x8)
  {
    *(.heap11);
  . = ORIGIN(EMI1) + LENGTH(EMI1);
    __HEAP_11_END = .;
  } > EMI1
}