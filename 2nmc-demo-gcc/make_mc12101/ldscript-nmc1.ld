MEMORY
{
  C1_IMU0 : ORIGIN = 0x00000800, LENGTH = 0x0001F800
  C1_IMU1 : ORIGIN = 0x00020000, LENGTH = 0x00020000
  C1_IMU2 : ORIGIN = 0x00040000, LENGTH = 0x00020000
  C1_IMU3 : ORIGIN = 0x00060000, LENGTH = 0x00020000
  SHMEM1 : ORIGIN = 0x002C0000, LENGTH = 0x00040000
  SYNCRO : ORIGIN = 0x9BFFFFC0, LENGTH = 0x00000040
  EMI2 : ORIGIN = 0x9C000000, LENGTH = 0x02000000
  EMI3 : ORIGIN = 0x9E000000, LENGTH = 0x02000000
}

SECTIONS
{
  .text : ALIGN(0x8)
  {
    *(.text);
  } > EMI2
  .text_hal_libc : ALIGN(0x8)
  {
    *(.text_hal_libc);
  } > EMI2
  .text_demo3d : ALIGN(0x8)
  {
    *(.text_demo3d);
  } > C1_IMU0
  .text_nmglvs : ALIGN(0x8)
  {
    *(text_nmglvs);
  } > EMI2
  .text_hal : ALIGN(0x8)
  {
    *(.text_hal);
  } > EMI2
  .text_demo3dExt : ALIGN(0x8)
  {
    *(.text_demo3dExt);
  } > EMI2
  .text_nmprofiler : ALIGN(0x8)
  {
    *(.text_nmprofiler);
  } > EMI2
  .init : ALIGN(0x8)
  {
    *(.init);
  } > C1_IMU0
  .fini : ALIGN(0x8)
  {
    *(.fini);
  } > C1_IMU0
  .data : ALIGN(0x8)
  {
    *(.data);
  } > C1_IMU0
  .data_demo3d : ALIGN(0x8)
  {
    *(.data_demo3d);
  } > C1_IMU0
  .bss : ALIGN(0x8)
  {
    *(.bss);
  } > C1_IMU0
  .stack : ALIGN(0x8)
  {
    *(.stack);
  } > C1_IMU0
  .text_nmplv : ALIGN(0x8)
  {
    *(.text_nmplv);
  } > C1_IMU0
  .text_nmplm : ALIGN(0x8)
  {
    *(.text_nmplm);
  } > C1_IMU0
  .data_nmplv_L : ALIGN(0x8)
  {
    *(.data_nmplv_L);
  } > C1_IMU0
  .data_nmplv_G : ALIGN(0x8)
  {
    *(.data_nmplv_G);
  } > C1_IMU0
  .text_nmvcore : ALIGN(0x8)
  {
    *(.text_nmvcore);
  } > C1_IMU0
  .data_imu0 : ALIGN(0x8)
  {
    *(.data_imu0);
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
  .data_shared2 : ALIGN(0x8)
  {
    *(.data_shared2);
  } > EMI2
  .data_shared3 : ALIGN(0x8)
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
  } > EMI2
  .rpc_services : ALIGN(0x8)
  {
    *(.rpc_services);
  } > EMI2
  /*.heap : ALIGN(0x8)
  {
	*(.heap);
	. = ORIGIN(C1_IMU0) + LENGTH(C1_IMU0);
    __HEAP_0_END = .;
  } > C1_IMU0
  .heap1 : ALIGN(0x8)
  {
    *(.heap1);
	. = ORIGIN(C1_IMU1) + LENGTH(C1_IMU1);
    __HEAP_1_END = .;
  } > C1_IMU1*/
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
  .heap10 : ALIGN(0x8)
  {
    *(.heap10);
	. = ORIGIN(EMI2) + LENGTH(EMI2);
    __HEAP_10_END = .;
  } > EMI2
}