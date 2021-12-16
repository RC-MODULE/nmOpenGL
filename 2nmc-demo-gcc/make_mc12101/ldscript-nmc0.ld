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
  EMI0 : ORIGIN = 0x80000000, LENGTH = 0x8000000
  EMI2 : ORIGIN = 0x90000000, LENGTH = 0x8000000
  SYNCRO : ORIGIN = 0x9FFFFF00, LENGTH = 0x00000100
}

SECTIONS
{
  .text_imu0 : ALIGN(0x8)
  {
    *(.text_int);
    *(.text_nmblas);
    *(.text_nmplv);
    *(.text_hal_libc);
  } > C0_IMU0
  .data_imu0 : ALIGN(0x8)
  {
    *(.data_imu0);
    *(.data_nmplv);
    *(.data_demo3d);
    *(.data_nmblas);
  } > C0_IMU0
  .init : ALIGN(0x8)
  {
    *(.init);
  } > C0_IMU0
  .fini : ALIGN(0x8)
  {
    *(.fini);
  } > C0_IMU0
  
  .data_imu1 : ALIGN(0x8)
  {
    *(.data_imu1);
  } > C0_IMU1
  .stack (NOLOAD): ALIGN(0x8)
  {
    *(.stack);
  } > C0_IMU1
  .heap1 (NOLOAD): ALIGN(0x8)
  {
    *(.heap1);
	. = ORIGIN(C0_IMU1) + LENGTH(C0_IMU1);
    __HEAP_1_END = .;
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


  .data_imu4 : ALIGN(0x8)
  {
    *(.data_imu4);
  } > C0_IMU4
  .heap4 (NOLOAD): ALIGN(0x8)
  {
    *(.heap4);
	. = ORIGIN(C0_IMU4) + LENGTH(C0_IMU4);
    __HEAP_4_END = .;
  } > C0_IMU4


  .data_imu5 : ALIGN(0x8)
  {
    *(.data_imu5);
  } > C0_IMU5
  .heap5 (NOLOAD) : ALIGN(0x8)
  {
    *(.heap5);
	. = ORIGIN(C0_IMU5) + LENGTH(C0_IMU5);
    __HEAP_5_END = .;
  } > C0_IMU5


  .data_imu6 : ALIGN(0x8)
  {
    *(.data_imu6);
  } > C0_IMU6
  .heap6 (NOLOAD): ALIGN(0x8)
  {
    *(.heap6);
	. = ORIGIN(C0_IMU6) + LENGTH(C0_IMU6);
    __HEAP_6_END = .;
  } > C0_IMU6


  .data_imu7 : ALIGN(0x8)
  {
    *(.data_imu7);
  } > C0_IMU7
  .heap7 (NOLOAD): ALIGN(0x8)
  {
    *(.heap7);
	. = ORIGIN(C0_IMU7) + LENGTH(C0_IMU7);
    __HEAP_7_END = .;
  } > C0_IMU7

  
  .data_shmem0 : ALIGN(0x8)
  {
    *(.data_shmem0);
  } > SHMEM0
  .heap8 (NOLOAD): ALIGN(0x8)
  {
    *(.heap8);
	. = ORIGIN(SHMEM0) + LENGTH(SHMEM0);
    __HEAP_8_END = .;
  } > SHMEM0


  .text : ALIGN(0x8)
  {
    *(.text);
    *(.text_nmgl);
    *(.text_demo3d);
    *(.text_nmglvs);
    *(.text_shared0);
    *(.text_hal);
    *(.text_nmprofiler);
    *(SORT(.text.*))
    *(SORT(.gnu.linkonce*))
  } > EMI0  

  .data : ALIGN(0x8)
  {
    *(.data_DDR);
    *(.data);
    *(data1);
    *(data);
    *(.bsst);
    *(.data_shared0);
    *(.data_hal);
    *(.comment);
    *(.data.locale_buf_C.2053);
    *(.data_asm);
    *(SORT(.data._*))
    *(SORT(.rodata.*))
  } > EMI0
  .gcc_except_table : ALIGN(0x8){
    *(.gcc_except_table)
  } > EMI0
  .bss (NOLOAD): ALIGN(0x8)
  {
    *(.bss);
    *(.bss.data_hal);
    *(.bss.data_demo3d); 
    *(.bss.num_lines.2054);
    *(SORT(.bss._*))
    *(.data_shared0.bss);
  } > EMI0
  .rpc_services.bss : ALIGN(0x8)
  {
    *(.rpc_services.bss);
  } > EMI0
  .rpc_services : ALIGN(0x8)
  {
    *(.rpc_services);
  } > EMI0
  .heap10 (NOLOAD): ALIGN(0x8)
  {
    *(.heap10);
	. = ORIGIN(EMI0) + LENGTH(EMI0);
    __HEAP_10_END = .;
  } > EMI0


  .data_shared2 : ALIGN(0x8)
  {
    *(.data_shared2);
  } > EMI2
  .data_shared2.bss (NOLOAD): ALIGN(0x8)
  {
    *(.data_shared2.bss);
  } > EMI2
  .heap12 (NOLOAD): ALIGN(0x8)
  {
    *(.heap12);
	. = ORIGIN(EMI2) + LENGTH(EMI2);
    __HEAP_12_END = .;
  } > EMI2

  .data_hal_syncro : ALIGN(0x8)
  {
    *(.data_hal_syncro);
  } > SYNCRO
  
  .heap : ALIGN(0x8)
  {
    *(.heap);
	. = ORIGIN(C0_IMU0) + LENGTH(C0_IMU0);
    __HEAP_0_END = .;
  } > C0_IMU0
  
  
  
  
 
  
  
  
  
  
  
  
}