/*
 * Robot.c
 *
 *  Created on: 17/02/2018
 *      Author: josepablocb
 */

#include "Robot_Hw_Ctl.h"

void Robot_init(){
    //Necesario para inicializar uart
    UART_Params                 uartParams;
    //Necesario para inicializar motores
    MotorDriver_Params          MotorParams;
//
//  Preparar perifericos
//
    Board_initGeneral();
    Board_initUART();
    Board_initGPIO();
    Encoder_init();
    MotorDriver_init();
    IMU_init();

//
//  Inicializar el modulo UART
//


    UART_Params_init(&uartParams);
    uartParams.writeDataMode    = UART_DATA_BINARY;
    uartParams.readDataMode     = UART_DATA_BINARY;
    uartParams.readReturnMode   = UART_RETURN_FULL;
    uartParams.readEcho         = UART_ECHO_OFF;
    uartParams.baudRate         = 9600;
    Robot_uart = UART_open(ROBOT_UART, &uartParams);
    if(!Robot_uart){
        System_abort("Error initializing board UART\n");
    }

    MotorDriver_Params_init(&MotorParams);
    Robot_Motor = MotorDriver_open(&Robot_MotorState,&MotorParams);
    if(!Robot_Motor){
        System_abort("Motor Driver no ha sido abierto\n");
    }

//
//  Inicializar Encoder
//
    Robot_ENCL = Encoder_open(Robot_Left_Encoder,&Robot_EncoderL);
    if(!Robot_ENCL){
        System_abort("Encoder no ha sido abierto\n");
    }
    Robot_ENCR = Encoder_open(Robot_Right_Encoder,&Robot_EncoderR);
    if(!Robot_ENCR){
        System_abort("Encoder no ha sido abierto\n");
    }
}

void Robot_EnableIMU(){
    //Necesario para inicializar imu
    IMU_Params                  imuParams;
    IMU_Sensor_Configuration    imuConfiguration;
    //
    //  Inicializar IMU
    //
        IMU_Params_init(&imuParams);
        imuParams.I2C_index = Robot_I2C;
        Robot_IMU = IMU_open(&Robot_IMUstate,&imuParams);
        if(!Robot_IMU){
            System_abort("IMU no ha sido abierto\n");
        }
        IMU_Sensor_Configuration_init(&imuConfiguration);

    //    IMU_setCallback(imu,SensorEvent,Board_BUTTON0);
    //    IMU_EnableEvents(imu);
        if(!IMU_Sensor_Init_Magnetometer(Robot_IMU)){
            System_abort("Magnetometro no activo!\n");
        }else{
            imuConfiguration.Address       =IMU_MAGN_CNTL;
            imuConfiguration.Configuration =0x12;//16bit out|Continuous measurement mode 1
            imuConfiguration.Type          =ByteConfiguration;
            IMU_Sensor_Configure_Magnetometer(Robot_IMU,&imuConfiguration);
        }

        //Limpíar interrupciones en cualquier lectura
        //La interrupicion se genera cuando haya datos listos!
        imuConfiguration.Address        = IMU_INT_PIN_CFG;
        imuConfiguration.Configuration  = 0xB2;
        imuConfiguration.Type    = ByteConfiguration;

        IMU_Sensor_Configure(Robot_IMU,&imuConfiguration);

        //Activar  interrupciones
        imuConfiguration.Address = IMU_INT_ENABLE;
        imuConfiguration.Configuration = 0x01;
        IMU_Sensor_Configure(Robot_IMU,&imuConfiguration);
}
