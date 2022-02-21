#ifndef __NMDATA_H__
#define __NMDATA_H__

#ifdef __NM__
#	define SEC(sec) __attribute__((section(#sec)))
#else
#	define SEC(sec) 
#endif

#define DATA_IMU0 SEC(.data_imu0)
#define DATA_IMU1 SEC(.data_imu1)
#define DATA_IMU2 SEC(.data_imu2)
#define DATA_IMU3 SEC(.data_imu3)
#define DATA_IMU4 SEC(.data_imu4)
#define DATA_IMU5 SEC(.data_imu5)
#define DATA_IMU6 SEC(.data_imu6)
#define DATA_IMU7 SEC(.data_imu7)

#define DATA_SHMEM0 SEC(.data_shmem0)
#define DATA_SHMEM1 SEC(.data_shmem1)

#define DATA_EMI SEC(.data_emi)


#define BSS_DATA_IMU0 SEC(.bss.data_imu0)
#define BSS_DATA_IMU1 SEC(.bss.data_imu1)
#define BSS_DATA_IMU2 SEC(.bss.data_imu2)
#define BSS_DATA_IMU3 SEC(.bss.data_imu3)
#define BSS_DATA_IMU4 SEC(.bss.data_imu4)
#define BSS_DATA_IMU5 SEC(.bss.data_imu5)
#define BSS_DATA_IMU6 SEC(.bss.data_imu6)
#define BSS_DATA_IMU7 SEC(.bss.data_imu7)

#endif //__NMDATA_H__