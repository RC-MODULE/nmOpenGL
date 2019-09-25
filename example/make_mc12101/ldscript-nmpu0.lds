MEMORY
{
  C0_IMU0 : ORIGIN = 0x00000800, LENGTH = 0x0001F800
  C0_IMU1 : ORIGIN = 0x00020000, LENGTH = 0x00020000
  C0_IMU2 : ORIGIN = 0x00040000, LENGTH = 0x00020000
  C0_IMU3 : ORIGIN = 0x00060000, LENGTH = 0x00020000
  C0_IMU4 : ORIGIN = 0x00080000, LENGTH = 0x00020000
  C0_IMU5 : ORIGIN = 0x000A0000, LENGTH = 0x00020000
  C0_IMU6 : ORIGIN = 0x000C0000, LENGTH = 0x00020000
  C0_IMU7 : ORIGIN = 0x000E0000, LENGTH = 0x00020000
  SHMEM0 : ORIGIN = 0x002A0000, LENGTH = 0x00020000
  EMI : ORIGIN = 0x80000000, LENGTH = 0x1BFFFFC0
  SYNCRO : ORIGIN = 0x9BFFFFC0, LENGTH = 0x00000040
}

SECTIONS
{
  .text : ALIGN(0x8)
  {
    *(.text);
  } > EMI
  .text_int : ALIGN(0x8)
  {
    *(.text_int);
  } > C0_IMU0
  .text_nmgl : ALIGN(0x8)
  {
    *(.text_nmgl);
  } > C0_IMU0
  .text_nmplv : ALIGN(0x8)
  {
    *(.text_nmplv);
  } > C0_IMU0
  .text_demo3d : ALIGN(0x8)
  {
    *(.text_demo3d);
  } > C0_IMU0
  .text_hal_libc : ALIGN(0x8)
  {
    *(.text_hal_libc);
  } > C0_IMU0
  .text_nmprofiler : ALIGN(0x8)
  {
    *(.text_nmprofiler);
  } > EMI
  .init : ALIGN(0x8)
  {
    *(.init);
  } > C0_IMU0
  .fini : ALIGN(0x8)
  {
    *(.fini);
  } > C0_IMU0
  .nobits : ALIGN(0x8)
  {
    *(.nobits);
  } > C0_IMU0
  .bss : ALIGN(0x8)
  {
    *(.bss);
  } > EMI
  .data_imu0 : ALIGN(0x8)
  {
    *(.data_imu0);
  } > C0_IMU0
  .data_imu1 : ALIGN(0x8)
  {
    *(.data_imu1);
  } > C0_IMU1
  .data_imu2 : ALIGN(0x8)
  {
    *(.data_imu2);
  } > C0_IMU2
  .data_imu3 : ALIGN(0x8)
  {
    *(.data_imu3);
  } > C0_IMU3
  .data_imu4 : ALIGN(0x8)
  {
    *(.data_imu4);
  } > C0_IMU4
  .data_imu5 : ALIGN(0x8)
  {
    *(.data_imu5);
  } > C0_IMU5
  .data_imu6 : ALIGN(0x8)
  {
    *(.data_imu6);
  } > C0_IMU6
  .data_imu7 : ALIGN(0x8)
  {
    *(.data_imu7);
  } > C0_IMU7
  .data_shmem0 : ALIGN(0x8)
  {
    *(.data_shmem0);
  } > SHMEM0
  .data_shared : ALIGN(0x8)
  {
    *(.data_shared);
  } > EMI
  .data_shared.bss : ALIGN(0x8)
  {
    *(.data_shared.bss);
  } > EMI
  .data_DDR : ALIGN(0x8)
  {
    *(.data_DDR);
  } > EMI
  .data : ALIGN(0x8)
  {
    *(.data);
  } > C0_IMU0
  .bss : ALIGN(0x8)
  {
    *(.bss);
  } > C0_IMU0
  .stack : ALIGN(0x8)
  {
    *(.stack);
  } > SHMEM0
  .data_hal_syncro : ALIGN(0x8)
  {
    *(.data_hal_syncro);
  } > SYNCRO
  .rpc_services.bss : ALIGN(0x8)
  {
    *(.rpc_services.bss);
  } > EMI
  .rpc_services : ALIGN(0x8)
  {
    *(.rpc_services);
  } > EMI
  .heap2 : ALIGN(0x8)
  {
    *(.heap2);
	. = ORIGIN(C0_IMU2) + LENGTH(C0_IMU2);
    __HEAP_2_END = .;
  } > C0_IMU2
  .heap3 : ALIGN(0x8)
  {
    *(.heap3);
	. = ORIGIN(C0_IMU3) + LENGTH(C0_IMU3);
    __HEAP_3_END = .;
  } > C0_IMU3
  .heap4 : ALIGN(0x8)
  {
    *(.heap4);
	. = ORIGIN(EMI) + LENGTH(EMI);
    __HEAP_4_END = .;
  } > EMI
}