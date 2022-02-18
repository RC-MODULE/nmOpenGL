#ifndef __NMDATA_H__
#define __NMDATA_H__

#define DATA_IMU0 __attribute__((section(".data_imu0")))
#define DATA_IMU1 __attribute__((section(".data_imu1")))
#define DATA_IMU2 __attribute__((section(".data_imu2")))
#define DATA_IMU3 __attribute__((section(".data_imu3")))
#define DATA_IMU4 __attribute__((section(".data_imu4")))
#define DATA_IMU5 __attribute__((section(".data_imu5")))
#define DATA_IMU6 __attribute__((section(".data_imu6")))
#define DATA_IMU7 __attribute__((section(".data_imu7")))

#define DATA_SHMEM0 __attribute__((section(".data_shmem0")))
#define DATA_SHMEM1 __attribute__((section(".data_shmem1")))

#define DATA_EMI __attribute__((section(".data_emi")))


#define BSS_DATA_IMU0 __attribute__((section(".bss.data_imu0")))
#define BSS_DATA_IMU1 __attribute__((section(".bss.data_imu1")))
#define BSS_DATA_IMU2 __attribute__((section(".bss.data_imu2")))
#define BSS_DATA_IMU3 __attribute__((section(".bss.data_imu3")))
#define BSS_DATA_IMU4 __attribute__((section(".bss.data_imu4")))
#define BSS_DATA_IMU5 __attribute__((section(".bss.data_imu5")))
#define BSS_DATA_IMU6 __attribute__((section(".bss.data_imu6")))
#define BSS_DATA_IMU7 __attribute__((section(".bss.data_imu7")))

#endif //__NMDATA_H__