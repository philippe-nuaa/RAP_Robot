/*
 * Robot_Hw_Ctl.h
 *
 *  Created on: 17/02/2018
 *      Author: josepablocb
 */

#ifndef ROBOT_RAP_ROBOT_BOARD_HW_ROBOT_HW_CTL_H_
#define ROBOT_RAP_ROBOT_BOARD_HW_ROBOT_HW_CTL_H_

#include <xdc/runtime/System.h>
#include "../Drivers/Drivers.h"
#include "../Drivers/Tools.h"
#include "Robot_Board.h"

//
//  LOG/DEBUG UART
//
//
//  Uso exclusivo en tareas!
//
    #define ROBOT_UART  Board_UART0
    //Output Macros
    #define RobotLog(_MSG_)     {UART_puts(Robot_uart , _MSG_ );}
    #define RobotLogCh(_CHR_)   {UART_putch(Robot_uart , _CHR_ );}
    #define RobotLogInt(_INT_)  {UART_putInt(Robot_uart , _INT_ );}
    #define RobotLogHex(_HX_)   {UART_putHex(Robot_uart , _HX_ );}
    #define RobotLogFloat(_FT_) {UART_putFloat(Robot_uart , _FT_ );}

    //Input Macros
    #define RobotLogReadFloat() UART_readFloat(Robot_uart)


    UART_Handle                 Robot_uart;

//
//  Motor Driver
//
    MotorDriver_state           Robot_MotorState;
    MotorDriver_Handle          Robot_Motor;
//
//  Encoder Driver
//
    #define Robot_Left_Encoder  Board_QEI0
    #define Robot_Right_Encoder Board_QEI1

    Encoder_state               Robot_EncoderL,Robot_EncoderR;
    Encoder_Handle              Robot_ENCL,Robot_ENCR;
//
//  IMU Driver
//
    #define Robot_I2C           Board_I2C1
    #define Robot_IMU_INT_PIN   Board_B2
    IMU_State                   Robot_IMUstate;
    IMU_Handle                  Robot_IMU;
//
//  Prototipos
//
/*
    Inicializar los drivers y controladores,
    preparar su disposicion para el sistema
*/
extern void Robot_init(void);

/*
    Los controladores externos que requieren
    comunicacion solo pueden ser inicializados
    desde el contexto de una tarea.
*/
extern void Robot_EnableIMU(void);

#endif /* ROBOT_RAP_ROBOT_BOARD_HW_ROBOT_HW_CTL_H_ */
