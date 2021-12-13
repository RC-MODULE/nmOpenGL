MEMORY
{
  C0_IMU0           : ORIGIN = 0x00000800, LENGTH = 0x0001F800
  C0_IMU1           : ORIGIN = 0x00020000, LENGTH = 0x00020000
  C0_IMU2           : ORIGIN = 0x00040000, LENGTH = 0x00020000
  C0_IMU3           : ORIGIN = 0x00060000, LENGTH = 0x00020000
  C0_IMU4           : ORIGIN = 0x00080000, LENGTH = 0x00020000
  C0_IMU5           : ORIGIN = 0x000A0000, LENGTH = 0x00020000
  C0_IMU6           : ORIGIN = 0x000C0000, LENGTH = 0x00020000
  C0_IMU7           : ORIGIN = 0x000E0000, LENGTH = 0x00020000
  SHMEM0            : ORIGIN = 0x002A0000, LENGTH = 0x00020000
  EMI0              : ORIGIN = 0x80000000, LENGTH = 0x8000000
  EMI2              : ORIGIN = 0x90000000, LENGTH = 0x8000000
  SYNCRO            : ORIGIN = 0x9FFFFF00, LENGTH = 0x00000100

}



SECTIONS
{


  .text_imu0 : ALIGN(0x8)
  {
    *(.text_int);
    *(.text_nmblas);
    *(.text_nmplv);
  } > C0_IMU0
  .data_imu0 : ALIGN(0x8)
  {
    *(.init);
    *(.fini);
    KEEP(*(.data_imu0))
    *(.data_nmplv);
    *(.data_demo3d);
    *(.data_nmblas);
  } > C0_IMU0
  .data_imu0.bss (NOLOAD) : ALIGN(0x8)
  {
    *(.bss.data_imu0)
    *(.bss.data_demo3d)
    *(.nobits)
  } > C0_IMU0 
  .stack : ALIGN(0x8)
  {
    *(.stack);
  } > C0_IMU0 

  .heap (NOLOAD) : ALIGN(0x8)
  {
    *(.heap);
    . = ORIGIN(C0_IMU0) + LENGTH(C0_IMU0);
    __HEAP_0_END = .; 
  } > C0_IMU0 


  .data_imu1 : ALIGN(0x8)
  {
    KEEP(*(.data_imu1))
  } > C0_IMU1
  .data_imu1.bss  (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu1)
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
  .data_imu2.bss (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu2)
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
  .data_imu3.bss (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu3)
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
  .data_imu4.bss (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu4)
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
  .data_imu5.bss (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu5)
  } > C0_IMU5
  .heap5 (NOLOAD): ALIGN(0x8)
  {
    *(.heap5);
    . = ORIGIN(C0_IMU5) + LENGTH(C0_IMU5);
    __HEAP_5_END = .;    
  } > C0_IMU5



  .data_imu6 : ALIGN(0x8)
  {
    *(.data_imu6);
  } > C0_IMU6
  .data_imu6.bss (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu6)
  } > C0_IMU6
  .heap6 (NOLOAD): ALIGN(0x8)
  {
    *(.heap6);
    . = ORIGIN(C0_IMU6) + LENGTH(C0_IMU6);
    __HEAP_6_END = .;
  } > C0_IMU6



  .data_imu7 : ALIGN(0x8)
  {
    KEEP(*(.data_imu7))
  } > C0_IMU7
  .data_imu7.bss (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_imu7)
  } > C0_IMU7
  .heap7 (NOLOAD): ALIGN(0x8)
  {
    *(.heap7);
    . = ORIGIN(C0_IMU7) + LENGTH(C0_IMU7);
    __HEAP_7_END = .;
  } > C0_IMU7


  .data_shmem0 : ALIGN(0x8)
  {
    KEEP(*(.data_shmem0))
  } > SHMEM0
  .data_shmem0.bss (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_shmem0);
  } > SHMEM0 
  .heap8 (NOLOAD): ALIGN(0x8)
  {
    *(.heap8);
    . = ORIGIN(SHMEM0) + LENGTH(SHMEM0);
    __HEAP_8_END = .;
  } > SHMEM0 




  .text : ALIGN(0x8)
  {
    /*---------Text----------- */
    _section_emi0_begin = .;
    *(.text_shared0)
    *(.text)
    *(.text_nmgl)
    *(.text_nmprofiler)
    *(.text_nmglvs)
    *(.text_demo3d)
    *(.text_printf)
    *(.text_hal_libc);
    *(.text_hal)
    *(.comment)
    *(SORT(.gnu*))
    *(SORT(.text.*))
  } > EMI0
  .data : ALIGN(0x8)
  {
    *(.data)
    *(data)
    *(SORT(.data.*))
    *(.data_asm)
    *(.data_hal)
    *(.data_DDR)
    *(.data_shared0)
    *(.gcc_except_table)
    *(SORT(.rodata.*))
    *(.bsst)
    /**(.rpc_services)
    *(.rpc_services.bss)*/
    _section_emi0_end = .;
  } > EMI0
  .bss (NOLOAD): ALIGN(0x8)
  {
    *(.bss.data_hal)
    *(.bss.data_shared0)
    *(.bss)
    *(SORT(.bss.*))
    *(COMMON)
  } > EMI0
  .heap10 (NOLOAD): ALIGN(0x8)
  {
    *(.heap10);
    . =  ORIGIN(EMI0) + LENGTH(EMI0);
    __HEAP_10_END = .;
  } > EMI0

  
  .data_emi2 : ALIGN(0x8)
  {
    KEEP(*(.data_emi2))
  } > EMI2
  .data_emi2.bss (NOLOAD): ALIGN(0x8)
  {
    KEEP(*(.bss.data_emi2))
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


  
}