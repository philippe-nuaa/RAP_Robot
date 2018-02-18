/** ============================================================================
 *  @file       IMU.h
 *
 *  @brief      IMU sensor driver interface
 *
 *  Created on: 27/01/2018
 *      Author: Josepablo Cruz Baas
 *
 *  El encabezado debe ser incluido en una aplicacion como sigue:
 *  @code
 *  #include "IMU.h"
 *  @endcode
 *
 *  # Operacion #
 *  El driver IMU permite operar al sensor de inercia de multiples ejes MPU 9250,
 *  conectado a un periferico I2C en transferencia a bloques. Lo que significa
 *  que durante la transferencia de datos, la ejecución de la tarea es bloqueada
 *  hasta la finalizacion de la transferencia.
 *  Las APIs en este driver sirven como una interfaz para una aplicacion tipica
 *  TI-RTOS.
 **/
/*
 * IMU_hw_reg.h
 *
 *  Created on: 27/01/2018
 *      Author: josepablocb
 */

#ifndef RAP_DRIVERS_IMU_HW_MEMMAP_H_
#define RAP_DRIVERS_IMU_HW_MEMMAP_H_

/*
 *  ============================================================================
    DIRECCIONES DE REGISTROS
 *  ============================================================================
*/
    //
    //  Register Map for Gyroscope and Accelerometer
    //
    #define IMU_SELF_TEST_X_GYRO                    0x00
    #define IMU_SELF_TEST_Y_GYRO                    0x01
    #define IMU_SELF_TEST_Z_GYRO                    0x02
    #define IMU_SELF_TEST_X_ACCEL                   0x0D
    #define IMU_SELF_TEST_Y_ACCEL                   0x0E
    #define IMU_SELF_TEST_Z_ACCEL                   0x0F
    #define IMU_XG_OFFSET_H                         0x13
    #define IMU_XG_OFFSET_L                         0x14
    #define IMU_YG_OFFSET_H                         0x15
    #define IMU_YG_OFFSET_L                         0x16
    #define IMU_ZG_OFFSET_H                         0x17
    #define IMU_ZG_OFFSET_L                         0x18
    #define IMU_SMPLRT_DIV                          0x19
    #define IMU_CONFIG                              0x1A
    #define IMU_GYRO_CONFIG                         0x1B
    #define IMU_ACCEL_CONFIG                        0x1C
    #define IMU_ACCEL_CONFIG2                       0x1D
    #define IMU_LP_ACCEL_ODR                        0x1E
    #define IMU_WOM_THR                             0x1F
    #define IMU_FIFO_EN                             0x23
    #define IMU_I2C_MST_CTRL                        0x24
    #define IMU_I2C_SLV0_ADDR                       0x25
    #define IMU_I2C_SLV0_REG                        0x26
    #define IMU_I2C_SLV0_CTRL                       0x27
    #define IMU_I2C_SLV1_ADDR                       0x28
    #define IMU_I2C_SLV1_REG                        0x29
    #define IMU_I2C_SLV1_CTRL                       0x2A
    #define IMU_I2C_SLV2_ADDR                       0x2B
    #define IMU_I2C_SLV2_REG                        0x2C
    #define IMU_I2C_SLV2_CTRL                       0x2D
    #define IMU_I2C_SLV3_ADDR                       0x2E
    #define IMU_I2C_SLV3_REG                        0x2F
    #define IMU_I2C_SLV3_CTRL                       0x30
    #define IMU_I2C_SLV4_ADDR                       0x31
    #define IMU_I2C_SLV4_REG                        0x32
    #define IMU_I2C_SLV4_DO                         0x33
    #define IMU_I2C_SLV4_CTRL                       0x34

    #define IMU_I2C_SLV4_DI                         0x35
    #define IMU_I2C_MST_STATUS                      0x36
    #define IMU_INT_PIN_CFG                         0x37
    #define IMU_INT_ENABLE                          0x38
    #define IMU_INT_STATUS                          0x3A
    #define IMU_ACCEL_XOUT_H                        0x3B
    #define IMU_ACCEL_XOUT_L                        0x3C
    #define IMU_ACCEL_YOUT_H                        0x3D
    #define IMU_ACCEL_YOUT_L                        0x3E
    #define IMU_ACCEL_ZOUT_H                        0x3F
    #define IMU_ACCEL_ZOUT_L                        0x40
    #define IMU_TEMP_OUT_H                          0x41
    #define IMU_TEMP_OUT_L                          0x42
    #define IMU_GYRO_XOUT_H                         0x43
    #define IMU_GYRO_XOUT_L                         0x44
    #define IMU_GYRO_YOUT_H                         0x45
    #define IMU_GYRO_YOUT_L                         0x46
    #define IMU_GYRO_ZOUT_H                         0x47
    #define IMU_GYRO_ZOUT_L                         0x48
    #define IMU_EXT_SENS_DATA_00                    0x49
    #define IMU_EXT_SENS_DATA_01                    0x4A
    #define IMU_EXT_SENS_DATA_02                    0x4B
    #define IMU_EXT_SENS_DATA_03                    0x4C
    #define IMU_EXT_SENS_DATA_04                    0x4D
    #define IMU_EXT_SENS_DATA_05                    0x4E
    #define IMU_EXT_SENS_DATA_06                    0x4F
    #define IMU_EXT_SENS_DATA_07                    0x50
    #define IMU_EXT_SENS_DATA_08                    0x51
    #define IMU_EXT_SENS_DATA_09                    0x52
    #define IMU_EXT_SENS_DATA_10                    0x53
    #define IMU_EXT_SENS_DATA_11                    0x54
    #define IMU_EXT_SENS_DATA_12                    0x55
    #define IMU_EXT_SENS_DATA_13                    0x56
    #define IMU_EXT_SENS_DATA_14                    0x57
    #define IMU_EXT_SENS_DATA_15                    0x58
    #define IMU_EXT_SENS_DATA_16                    0x59
    #define IMU_EXT_SENS_DATA_17                    0x5A
    #define IMU_EXT_SENS_DATA_18                    0x5B
    #define IMU_EXT_SENS_DATA_19                    0x5C
    #define IMU_EXT_SENS_DATA_20                    0x5D
    #define IMU_EXT_SENS_DATA_21                    0x5E
    #define IMU_EXT_SENS_DATA_22                    0x5F
    #define IMU_EXT_SENS_DATA_23                    0x60
    #define IMU_I2C_SLV0_DO                         0x63
    #define IMU_I2C_SLV1_DO                         0x64
    #define IMU_I2C_SLV2_DO                         0x65

    #define IMU_I2C_SLV3_DO                         0x66
    #define IMU_I2C_MST_DELAY_CTRL                  0x67
    #define IMU_SIGNAL_PATH_RESET                   0x68
    #define IMU_MOT_DETECT_CTRL                     0x69
    #define IMU_USER_CTRL                           0x6A
    #define IMU_PWR_MGMT_1                          0x6B
    #define IMU_PWR_MGMT_2                          0x6C
    #define IMU_FIFO_COUNTH                         0x72
    #define IMU_FIFO_COUNTL                         0x73
    #define IMU_FIFO_R_W                            0x74
    #define IMU_WHO_AM_I                            0x75
    #define IMU_XA_OFFSET_H                         0x77
    #define IMU_XA_OFFSET_L                         0x78
    #define IMU_YA_OFFSET_H                         0x7A
    #define IMU_YA_OFFSET_L                         0x7B
    #define IMU_ZA_OFFSET_H                         0x7D
    #define IMU_ZA_OFFSET_L                         0x7E

    //
    //  Register Map for Magnetometer
    //
    #define IMU_MAGN_WIA                                 0x00
    #define IMU_MAGN_INFO                                0x01
    #define IMU_MAGN_ST1                                 0x02
    #define IMU_MAGN_HXL                                 0x03
    #define IMU_MAGN_HXH                                 0x04
    #define IMU_MAGN_HYL                                 0x05
    #define IMU_MAGN_HYH                                 0x06
    #define IMU_MAGN_HZL                                 0x07
    #define IMU_MAGN_HZH                                 0x08
    #define IMU_MAGN_ST2                                 0x09
    #define IMU_MAGN_CNTL                                0x0A
    #define IMU_MAGN_RSV                                 0x0B
    #define IMU_MAGN_ASTC                                0x0C
    #define IMU_MAGN_TS1                                 0x0D
    #define IMU_MAGN_TS2                                 0x0E
    #define IMU_MAGN_I2CDIS                              0x0F
    #define IMU_MAGN_ASAX                                0x10
    #define IMU_MAGN_ASAY                                0x11
    #define IMU_MAGN_ASAZ                                0x12

/*
 *  ============================================================================

    CONFIGURACIONES TIPICAS PARA LOS REGISTROS DEL SENSOR

 *  ============================================================================
*/
//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9250_O_CONFIG
// register.
//
//*****************************************************************************
#define MPU9250_CONFIG_FIFO_MODE                                         \
                                0x40        // No write when full
#define MPU9250_CONFIG_EXT_SYNC_SET_M                                         \
                                0x38        // FSYNC pin sample location
#define MPU9250_CONFIG_EXT_SYNC_SET_DIS                                       \
                                0x00        // FSYNC input disabled
#define MPU9250_CONFIG_EXT_SYNC_SET_TEMP_OUT_L                                \
                                0x08        // FSYNC on TEMP_OUT_L[0]
#define MPU9250_CONFIG_EXT_SYNC_SET_GYRO_XOUT_L                               \
                                0x10        // FSYNC on GYRO_XOUT_L[0]
#define MPU9250_CONFIG_EXT_SYNC_SET_GYRO_YOUT_L                               \
                                0x18        // FSYNC on GYRO_YOUT_L[0]
#define MPU9250_CONFIG_EXT_SYNC_SET_GYRO_ZOUT_L                               \
                                0x20        // FSYNC on GYRO_ZOUT_L[0]
#define MPU9250_CONFIG_EXT_SYNC_SET_ACCEL_XOUT_L                              \
                                0x28        // FSYNC on ACCEL_XOUT_L[0]
#define MPU9250_CONFIG_EXT_SYNC_SET_ACCEL_YOUT_L                              \
                                0x30        // FSYNC on ACCEL_YOUT_L[0]
#define MPU9250_CONFIG_EXT_SYNC_SET_ACCEL_ZOUT_L                              \
                                0x38        // FSYNC on ACCEL_ZOUT_L[0]
#define MPU9250_CONFIG_DLPF_CFG_M                                             \
                                0x07        // Digital low-pass filter
                                            // configuration
#define MPU9250_CONFIG_DLPF_CFG_G250_T4000                                       \
                                0x00        // 250 Hz gyro bandwidth,
                                            // 4000 Hz temperature bandwidth
#define MPU9250_CONFIG_DLPF_CFG_G184_T188                                       \
                                0x01        // 184 Hz gyro bandwidth,
                                            // 188 Hz temperature bandwidth
#define MPU9250_CONFIG_DLPF_CFG_G92_T98                                         \
                                0x02        // 92 Hz gyro bandwidth,
                                            // 98 Hz temperature bandwidth
#define MPU9250_CONFIG_DLPF_CFG_G41_T42                                         \
                                0x03        // 41 Hz gyro bandwidth,
                                            // 42 Hz temperature bandwidth
#define MPU9250_CONFIG_DLPF_CFG_G20_T20                                         \
                                0x04        // 20 Hz gyro bandwidth,
                                            // 20 Hz temperature bandwidth
#define MPU9250_CONFIG_DLPF_CFG_G10_T10                                         \
                                0x05        // 10 Hz gyro bandwidth,
                                            // 10 Hz temperature bandwidth
#define MPU9250_CONFIG_DLPF_CFG_G5_T5                                           \
                                0x06        // 5 Hz gyro bandwidth,
                                            // 5 Hz temperature bandwidth
#define MPU9250_CONFIG_DLPF_CFG_G3600_T4000                                           \
                                0x07        // 3600 Hz gyro bandwidth,
                                            // 4000 Hz temperature bandwidth
#define MPU9250_CONFIG_EXT_SYNC_SET_S                                         \
                                3
#define MPU9250_CONFIG_DLPF_CFG_S                                             \
                                0

//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9150_O_GYRO_CONFIG
// register.
//
//*****************************************************************************
#define MPU9250_GYRO_CONFIG_XG_ST                                             \
                                0x80        // X-axis gyro self-test enable
#define MPU9250_GYRO_CONFIG_YG_ST                                             \
                                0x40        // Y-axis gyro self-test enable
#define MPU9250_GYRO_CONFIG_ZG_ST                                             \
                                0x20        // Z-axis gyro self-test enable
#define MPU9250_GYRO_CONFIG_FS_SEL_M                                          \
                                0x18        // Gyro full-scale range
#define MPU9250_GYRO_CONFIG_FS_SEL_250                                        \
                                0x00        // Gyro full-scale range +/- 250
                                            // degrees/sec
#define MPU9250_GYRO_CONFIG_FS_SEL_500                                        \
                                0x08        // Gyro full-scale range +/- 500
                                            // degrees/sec
#define MPU9250_GYRO_CONFIG_FS_SEL_1000                                       \
                                0x10        // Gyro full-scale range +/- 1000
                                            // degrees/sec
#define MPU9250_GYRO_CONFIG_FS_SEL_2000                                       \
                                0x18        // Gyro full-scale range +/- 2000
                                            // degrees/sec
#define MPU9250_GYRO_CONFIG_FS_SEL_S                                          \
                                3

#define MPU9250_GYRO_CONFIG_FCHOICE_B_NO_DLPF_G8800                                       \
                                0x01

#define MPU9250_GYRO_CONFIG_FCHOICE_B_NO_DLPF_G3600                                       \
                                0x01

//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9250_O_ACCEL_CONFIG
// register.
//
//*****************************************************************************
#define MPU9250_ACCEL_CONFIG_XA_ST                                            \
                                0x80        // X-axis accelerometer self-test
                                            // enable
#define MPU9250_ACCEL_CONFIG_YA_ST                                            \
                                0x40        // Y-axis accelerometer self-test
                                            // enable
#define MPU9250_ACCEL_CONFIG_ZA_ST                                            \
                                0x20        // Z-axis accelerometer self-test
                                            // enable
#define MPU9250_ACCEL_CONFIG_AFS_SEL_M                                        \
                                0x18        // Accelerometer full-scale range
#define MPU9250_ACCEL_CONFIG_AFS_SEL_2G                                       \
                                0x00        // Accelerometer full-scale range 2
                                            // g
#define MPU9250_ACCEL_CONFIG_AFS_SEL_4G                                       \
                                0x08        // Accelerometer full-scale range 4
                                            // g
#define MPU9250_ACCEL_CONFIG_AFS_SEL_8G                                       \
                                0x10        // Accelerometer full-scale range 8
                                            // g
#define MPU9250_ACCEL_CONFIG_AFS_SEL_16G                                      \
                                0x18        // Accelerometer full-scale range
                                            // 16 g
#define MPU9150_ACCEL_CONFIG_ACCEL_HPF_M                                      \
                                0x07        // High-pass filter setting
#define MPU9150_ACCEL_CONFIG_ACCEL_HPF_RESET                                  \
                                0x00        // High-pass filter reset
#define MPU9150_ACCEL_CONFIG_ACCEL_HPF_5HZ                                    \
                                0x01        // High-pass filter at 5 Hz
#define MPU9150_ACCEL_CONFIG_ACCEL_HPF_2_5HZ                                  \
                                0x02        // High-pass filter at 2.5 Hz
#define MPU9150_ACCEL_CONFIG_ACCEL_HPF_1_25HZ                                 \
                                0x03        // High-pass filter at 1.25 Hz
#define MPU9150_ACCEL_CONFIG_ACCEL_HPF_0_63HZ                                 \
                                0x04        // High-pass filter at 0.63 Hz
#define MPU9150_ACCEL_CONFIG_ACCEL_HPF_HOLD                                   \
                                0x07        // High-pass filter hold
#define MPU9250_ACCEL_CONFIG_AFS_SEL_S                                        \
                                3
#define MPU9150_ACCEL_CONFIG_ACCEL_HPF_S                                      \
                                0

//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9250_O_ACCEL_CONFIG2
// register.
//
//*****************************************************************************
#define MPU9250_ACCEL_CONFIG2_ACCEL_NO_DLPF_A1130                             \
                                0x08
#define MPU9250_ACCEL_CONFIG2_A460                             \
                                0x00
#define MPU9250_ACCEL_CONFIG2_A184                             \
                                0x01
#define MPU9250_ACCEL_CONFIG2_A92                             \
                                0x02
#define MPU9250_ACCEL_CONFIG2_A41                             \
                                0x03
#define MPU9250_ACCEL_CONFIG2_A20                             \
                                0x04
#define MPU9250_ACCEL_CONFIG2_A10                             \
                                0x05
#define MPU9250_ACCEL_CONFIG2_A5                             \
                                0x06

//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9250_O_LOW_POWER_ACCEL
// register.
//
//*****************************************************************************
#define MPU9250_ACCEL_LOW_POWER_250                             \
                                0x0A

//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9150_O_I2C_MST_CTRL
// register.
//
//*****************************************************************************
#define MPU9250_I2C_MST_CTRL_MULT_MST_EN                                      \
                                0x80        // Multi-master enable
#define MPU9250_I2C_MST_CTRL_WAIT_FOR_ES                                      \
                                0x40        // Wait for external sensor data
#define MPU9250_I2C_MST_CTRL_SLV3_FIFO_EN                                     \
                                0x20        // Slave 3 FIFO enable
#define MPU9250_I2C_MST_CTRL_I2C_MST_P_NSR                                    \
                                0x10        // No repeated start conditions
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_M                                    \
                                0x0F        // I2C master clock speed
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_348                                  \
                                0x00        // 348 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_333                                  \
                                0x01        // 333 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_320                                  \
                                0x02        // 320 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_308                                  \
                                0x03        // 308 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_296                                  \
                                0x04        // 296 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_286                                  \
                                0x05        // 286 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_276                                  \
                                0x06        // 276 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_267                                  \
                                0x07        // 267 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_258                                  \
                                0x08        // 258 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_500                                  \
                                0x09        // 500 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_471                                  \
                                0x0A        // 471 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_444                                  \
                                0x0B        // 444 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_421                                  \
                                0x0C        // 421 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_400                                  \
                                0x0D        // 400 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_381                                  \
                                0x0E        // 381 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_364                                  \
                                0x0F        // 364 kHz I2C master clock
#define MPU9250_I2C_MST_CTRL_I2C_MST_CLK_S                                    \
                                0

//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9250_O_INT_PIN_CFG
// register.
//
//*****************************************************************************
#define MPU9250_INT_PIN_CFG_INT_LEVEL                                         \
                                0x80        // INT pin active low
#define MPU9250_INT_PIN_CFG_INT_OPEN                                          \
                                0x40        // INT pin open-drain
#define MPU9250_INT_PIN_CFG_LATCH_INT_EN                                      \
                                0x20        // Latch INT pin output
#define MPU9250_INT_PIN_CFG_INT_RD_CLEAR                                      \
                                0x10        // Interrupt clear on any read
#define MPU9250_INT_PIN_CFG_FSYNC_INT_LEVEL                                   \
                                0x08        // FSYNC pin active low
#define MPU9250_INT_PIN_CFG_FSYNC_INT_EN                                      \
                                0x04        // FSYNC pin interrupt enable
#define MPU9250_INT_PIN_CFG_I2C_BYPASS_EN                                     \
                                0x02        // I2C bypass enable
#define MPU9250_INT_PIN_CFG_CLKOUT_EN                                         \
                                0x01        // CLKOUT enable

//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9150_O_INT_ENABLE
// register.
//
//*****************************************************************************
//#define MPU9150_INT_ENABLE_FF_EN                                              \
//                                0x80        // Free-fall interrupt enable
//#define MPU9150_INT_ENABLE_MOT_EN                                             \
//                                0x40        // Motion detection interrupt
//                                            // enable
//#define MPU9150_INT_ENABLE_ZMOT_EN                                            \
//                                0x20        // Zero motion interrupt enable
//#define MPU9150_INT_ENABLE_FIFO_OFLOW_EN                                      \
//                                0x10        // FIFO overflow interrupt enable
//#define MPU9150_INT_ENABLE_I2C_MST_INT_EN                                     \
//                                0x08        // I2C master interrupt enable
#define MPU9250_INT_ENABLE_DATA_RDY_EN                                        \
                                0x01        // Data ready interrupt enable

//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9250_O_I2C_SLV0_ADDR
// register.
//
//*****************************************************************************
#define MPU9250_I2C_SLV0_ADDR_RW                                              \
                                0x80        // Read/not write
#define MPU9250_I2C_SLV0_ADDR_M 0x7F        // Slave address
#define MPU9250_I2C_SLV0_ADDR_S 0

//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9250_O_I2C_SLV0_CTRL
// register.
//
//*****************************************************************************
#define MPU9250_I2C_SLV0_CTRL_EN                                              \
                                0x80        // Enable slave
#define MPU9250_I2C_SLV0_CTRL_BYTE_SW                                         \
                                0x40        // Byte-swap word pairs
#define MPU9250_I2C_SLV0_CTRL_REG_DIS                                         \
                                0x20        // Disable register number transfer
#define MPU9250_I2C_SLV0_CTRL_GRP                                             \
                                0x10        // Word pair grouping
#define MPU9250_I2C_SLV0_CTRL_LEN_M                                           \
                                0x0F        // Number of bytes to transfer
#define MPU9250_I2C_SLV0_CTRL_LEN_S                                           \
                                0

//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9250_O_I2C_SLV4_CTRL
// register.
//
//*****************************************************************************
#define MPU9250_I2C_SLV4_CTRL_EN                                              \
                                0x80        // Enable slave
#define MPU9250_I2C_SLV4_CTRL_INT_EN                                          \
                                0x40        // Interrupt enable
#define MPU9250_I2C_SLV4_CTRL_REG_DIS                                         \
                                0x20        // Disable register number transfer
#define MPU9250_I2C_SLV4_CTRL_I2C_MST_DLY_M                                   \
                                0x1F        // Slave access delay
#define MPU9250_I2C_SLV4_CTRL_I2C_MST_DLY_S                                   \
                                0

//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9250_O_USER_CTRL
// register.
//
//*****************************************************************************
#define MPU9250_USER_CTRL_FIFO_EN                                             \
                                0x40        // FIFO enable
#define MPU9250_USER_CTRL_I2C_MST_EN                                          \
                                0x20        // I2C master mode enable
#define MPU9250_USER_CTRL_I2C_IF_DIS                                          \
                                0x10        // Write as zero
#define MPU9250_USER_CTRL_FIFO_RESET                                          \
                                0x04        // Reset FIFO buffer
#define MPU9250_USER_CTRL_I2C_MST_RESET                                       \
                                0x02        // Reset I2C master
#define MPU9250_USER_CTRL_SIG_COND_RESET                                      \
                                0x01        // Reset all sensors

//*****************************************************************************
//
// The following are defines for the bit fields in the
// MPU9250_O_I2C_MST_DELAY_CTRL register.
//
//*****************************************************************************
#define MPU9250_I2C_MST_DELAY_CTRL_DELAY_ES_SHADOW                            \
                                0x80        // Delay external sensor data
#define MPU9250_I2C_MST_DELAY_CTRL_I2C_SLV4_DLY_EN                            \
                                0x10        // I2C slave 4 delay enable
#define MPU9250_I2C_MST_DELAY_CTRL_I2C_SLV3_DLY_EN                            \
                                0x08        // I2C slave 3 delay enable
#define MPU9250_I2C_MST_DELAY_CTRL_I2C_SLV2_DLY_EN                            \
                                0x04        // I2C slave 2 delay enable
#define MPU9250_I2C_MST_DELAY_CTRL_I2C_SLV1_DLY_EN                            \
                                0x02        // I2C slave 1 delay enable
#define MPU9250_I2C_MST_DELAY_CTRL_I2C_SLV0_DLY_EN                            \
                                0x01        // I2C slave 0 delay enable

//*****************************************************************************
//
// The following are defines for the bit fields in the MPU9250_O_PWR_MGMT_1
// register.
//
//*****************************************************************************
#define MPU9250_PWR_MGMT_1_DEVICE_RESET                                       \
                                0x80        // Device reset
#define MPU9250_PWR_MGMT_1_AUTOSELECT_CLK                                              \
                                0x01 // 0x40        // Enter sleep mode //Inseguro del valor
#define MPU9150_PWR_MGMT_1_CYCLE                                              \
                                0x20        // Enable automatic sleep
#define MPU9150_PWR_MGMT_1_TEMP_DIS                                           \
                                0x08        // Disable temperature sensor
#define MPU9150_PWR_MGMT_1_CLKSEL_M                                           \
                                0x07        // Clock source select
#define MPU9150_PWR_MGMT_1_CLKSEL_INT                                         \
                                0x00        // Internal 8 MHz oscillator
#define MPU9250_PWR_MGMT_1_CLKSEL_XG                                          \
                                0x01        // PLL with X-axis gyro reference
#define MPU9150_PWR_MGMT_1_CLKSEL_YG                                          \
                                0x02        // PLL with Y-axis gyro reference
#define MPU9150_PWR_MGMT_1_CLKSEL_ZG                                          \
                                0x03        // PLL with Z-axis gyro reference
#define MPU9150_PWR_MGMT_1_CLKSEL_EXT32K                                      \
                                0x04        // PLL with external 32.768 kHz
                                            // reference
#define MPU9150_PWR_MGMT_1_CLKSEL_EXT19M                                      \
                                0x05        // PLL with external 19.2 MHz
                                            // reference
#define MPU9150_PWR_MGMT_1_CLKSEL_STOP                                        \
                                0x07        // Clock disable
#define MPU9150_PWR_MGMT_1_CLKSEL_S                                           \
                                0


#endif /* RAP_DRIVERS_IMU_HW_MEMMAP_H_ */
