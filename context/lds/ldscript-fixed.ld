MEMORY
{
	C1_IMU0 : ORIGIN = 0x00000200<<2, LENGTH = 0x00007E00<<2
  	C1_IMU1 : ORIGIN = 0x00008000<<2, LENGTH = 0x00008000<<2
  	C1_IMU2 : ORIGIN = 0x00010000<<2, LENGTH = 0x00008000<<2
  	C1_IMU3 : ORIGIN = 0x00018000<<2, LENGTH = 0x00008000<<2
	SHMEM0  : ORIGIN = 0x000A8000<<2, LENGTH = 0x00008000<<2
  	SHMEM1  : ORIGIN = 0x000B0000<<2, LENGTH = 0x00010000<<2
	EMI 	: ORIGIN = 0x20000000<<2, LENGTH = 0x20000000<<2
}

SECTIONS
{
	.text_imu0 : ALIGN(0x8){
		*(.text_demo3d);
	} > C1_IMU0

	.init : ALIGN(0x8)
	{
		*(.init);
		*(.bsst);
	} > C1_IMU0

	.fini : ALIGN(0x8)
	{
		*(.fini);
	} > C1_IMU0

	.bss : ALIGN(0x8)
	{
		*(.bss);
		*(SORT(.bss.*))
	} > C1_IMU0

	.data_imu0 : ALIGN(0x8)
	{
		*(.data_imu0);
		*(.data_demo3d);
		*(.data_asm);
	} > C1_IMU0

	.stack : ALIGN(0x8)
	{
		*(.stack);
	} > C1_IMU1

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

	.data_shmem0 : ALIGN(0x8)
	{
		*(.data_shmem0);
	} > SHMEM0

	.data_shmem1 : ALIGN(0x8)
	{
		*(.data_shmem1);
	} > SHMEM1

	.text : ALIGN(0x8)
	{
		*(.text);
		*(SORT(.text._*))
	} > EMI

	.data : ALIGN(0x8)
	{
		*(.data);
		*(SORT(.data._*))
		*(SORT(.debug*))
		*(SORT(.rodata.*))
	} > EMI

	.data_emi : ALIGN(0x8)
	{
		*(.data_emi);
	} > EMI


	.rpc_services.bss : ALIGN(0x8)
	{
		*(.rpc_services.bss);
	} > EMI

	.rpc_services : ALIGN(0x8)
	{
		*(.rpc_services);
	} > EMI


}
