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
  .text_imu0 : ALIGN(0x8)
  {   
    *(.text_nmplv); 
    *(.text_nmplm); 
    *(.text_nmvcore);  
  } > C1_IMU0
  .data_imu0 : ALIGN(0x8)
  {   
    *(.data);  
    *(.data_imu0);  
    *(.data_demo3d);
    *(.data_nmplv); 
    *(.data_nmplv_L);  
    *(.data_nmplv_G);  
    *(.init);  
    *(.fini);  
    . = ALIGN(0x64);
    *(.data_imu0A); 
  } > C1_IMU0
  .bss : ALIGN(0x8)   { 
    *(.bss.data_imu0);  
    *(.bss.data_nmplv_G)  
    *(.bss);   
    *(.nobits)
  } > C1_IMU0
  .stack : ALIGN(0x8)
  {
    *(.stack);     
  } > C1_IMU0



  .data_imu1 : ALIGN(0x8)
  {   
    *(.data_imu1);  
  } > C1_IMU1
  .data_imu1.bss : ALIGN(0x8)
  {   
    *(.bss.data_imu1);  
  } > C1_IMU1


  .data_imu2 : ALIGN(0x8)
  {   
    *(.data_imu2);  
  } > C1_IMU2
  .data_imu2.bss : ALIGN(0x8)
  {
    *(.bss.data_imu2);  
    *(.heap2);
    . = ORIGIN(C1_IMU2) + LENGTH(C1_IMU2);
    __HEAP_2_END = .;
  } > C1_IMU2


  .data_imu3 : ALIGN(0x8)
  {
    *(.data_imu3);
  } > C1_IMU3  
  .data_imu3.bss : ALIGN(0x8)
  {
    *(.bss.data_imu3);  
    *(.heap3);
	 . = ORIGIN(C1_IMU3) + LENGTH(C1_IMU3);
    __HEAP_3_END = .;
  } > C1_IMU3


  .data_shmem1 : ALIGN(0x8)
  {
    *(.data_shmem1);
  } > SHMEM1
  .data_shmem1.bss : ALIGN(0x8)
  {
    *(.bss.data_shmem1);  
    *(.heap9);
	 . = ORIGIN(SHMEM1) + LENGTH(SHMEM1);
    __HEAP_9_END = .;
  } > SHMEM1


  

  .text : ALIGN(0x8)
  {    
    *(.text);  
    *(.text_hal_libc);  
    *(.text_demo3d);
    *(.text_nmglvs);
    *(.text_hal);
    *(.text_demo3dExt);
    *(.text_nmprofiler);
    *(.text_printf);
    *(text)
    *(SORT(.gnu*))
    *(.comment)
    *(SORT(.text.*))
  } > EMI1
  .data : ALIGN(0x8)  
  {    
    *(.data_shared1);  
    *(SORT(.data.*))
    *(SORT(.data_asm))
    *(SORT(.data_matr_4s2s))
    *(SORT(.data_hal))
    *(SORT(.rodata*))
    *(.bsst)
    *(.gcc_except_table)
    /**(.rpc_services);  
    *(.rpc_services.bss); */
  } > EMI1
  .data_emi1.bss : ALIGN(0x8)
  {
    *(.bss.data_shared1);  
    *(.heap);
    *(.heap1);
    *(.bss.data_hal);
    *(SORT(.bss._*))
    *(.heap11);
    . = ORIGIN(EMI1) + LENGTH(EMI1);
    __HEAP_11_END = .;
  } > EMI1
  



  .data_emi3.bss : ALIGN(0x8)
  {
    *(.data_emi3);
  } > EMI3
  .data_emi3.bss : ALIGN(0x8)
  {
    *(.bss.data_shared3);  
    *(.heap13);
    . = ORIGIN(EMI3) + LENGTH(EMI3);
    __HEAP_13_END = .;
  } > EMI3


  .data_hal_syncro : ALIGN(0x8)
  {
    *(.data_hal_syncro);
  } > SYNCRO
}