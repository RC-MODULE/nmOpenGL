MEMORY
{
  C1_IMU0 : ORIGIN = 0x00000800, LENGTH = 0x0001F800
  C1_IMU1 : ORIGIN = 0x00020000, LENGTH = 0x00020000
  C1_IMU2 : ORIGIN = 0x00040000, LENGTH = 0x00020000
  C1_IMU3 : ORIGIN = 0x00060000, LENGTH = 0x00020000
  SHMEM1 : ORIGIN = 0x002C0000, LENGTH = 0x00040000
  EMI1 : ORIGIN = 0x88000000, LENGTH = 0x8000000
  EMI3 : ORIGIN = 0x98000000, LENGTH = 0x7FFFF00
  SYNCRO : ORIGIN = 0x9FFFFF00, LENGTH = 0x00000100
}

SECTIONS
{
  .text : ALIGN(0x8)
  {
    *(.text);
    *(.text_hal_libc);
    *(.text_demo3d);
    *(.text_hal);
    *(.text_demo3dExt);
    *(.text_nmprofiler);
    *(.text_stub);
  } > EMI1
  .init : ALIGN(0x8)
  {
    *(.init);
  } > C1_IMU0
  .fini : ALIGN(0x8)
  {
    *(.fini);
  } > C1_IMU0
  .data_shared1 : ALIGN(0x8)
  {
    *(.data_shared1);
    *(.data_stub);
  } > EMI1
  .data : ALIGN(0x8)
  {
    *(.data);
    *(.data_demo3d);
    *(.data_nmplv);
    *(.data_nmplv_L);
    *(.data_nmplv_G);
  } > C1_IMU0
  .bss : ALIGN(0x8)
  {
    *(.bss);
    *(.bss_stub);
  } > EMI1
  .stack : ALIGN(0x8)
  {
    *(.stack);
  } > C1_IMU0
  .text_nmplv : ALIGN(0x8)
  {
    *(.text_nmplv);
    *(.text_nmplm);
    *(.text_nmvcore);
    *(.data_imu0);
    . = ALIGN(0x64);
    *(.data_imu0A);
  } > C1_IMU0


  .data_imu1 : ALIGN(0x8)
  {
    *(.data_imu1);
  } > C1_IMU1
  .data_imu2 : ALIGN(0x8)
  {
    *(.data_imu2);
  } > C1_IMU2
  .data_imu3 : ALIGN(0x8)
  {
    *(.data_imu3);
  } > C1_IMU3
  .data_shmem1 : ALIGN(0x8) 
  {
    *(.data_shmem1);
  } > SHMEM1
  .data_shared3 : ALIGN(0x8)
  {
    *(.data_shared3);
  } > EMI3
  .trace.bss : ALIGN(0x8)
  {
    *(.data_shared3);
  } > EMI3
  .data_hal_syncro : ALIGN(0x8)
  {
    *(.data_hal_syncro);
  } > SYNCRO
  .rpc_services.bss : ALIGN(0x8)
  {
    *(.rpc_services.bss);
  } > EMI1
  .rpc_services : ALIGN(0x8)
  {
    *(.rpc_services);
  } > EMI1
  .heap : ALIGN(0x8)
  {
	*(.heap);
  } > EMI1
  .heap1 : ALIGN(0x8)
  {
    *(.heap1);
  } > EMI1
  .heap2 : ALIGN(0x8)
  {
    *(.heap2);
	. = ORIGIN(C1_IMU2) + LENGTH(C1_IMU2);
    __HEAP_2_END = .;
  } > C1_IMU2
  .heap3 : ALIGN(0x8)
  {
    *(.heap3);
	. = ORIGIN(C1_IMU3) + LENGTH(C1_IMU3);
    __HEAP_3_END = .;
  } > C1_IMU3
  .heap9 : ALIGN(0x8)
  {
    *(.heap9);
	. = ORIGIN(SHMEM1) + LENGTH(SHMEM1);
    __HEAP_9_END = .;
  } > SHMEM1
  .heap11 : ALIGN(0x8)
  {
    *(.heap11);
	. = ORIGIN(EMI1) + LENGTH(EMI1);
    __HEAP_11_END = .;
  } > EMI1
  .heap13 : ALIGN(0x8)
  {
    *(.heap13);
	. = ORIGIN(EMI3) + LENGTH(EMI3);
    __HEAP_13_END = .;
  } > EMI3
}