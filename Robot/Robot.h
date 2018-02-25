/*
 * Robot.h
 *
 *  Created on: 17/02/2018
 *      Author: josepablocb
 */

#ifndef ROBOT_ROBOT_H_
#define ROBOT_ROBOT_H_

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

//
//  TI-RTOS BIOS includes
//
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

//
//  TI-RTOS Drivers
//
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

//
//  Board/Hardware includes
//
#include "RAP_Robot_Board_Hw/Robot_Board.h"
#include "RAP_Robot_Board_Hw/Robot_Hw_Ctl.h"

//
//  Drivers a nivel de hardware
//
#include "Drivers/Drivers.h"    //Controlador de motores, IMU y Encoder
#include "Drivers/Tools.h"      //Logs y utilidades de depuración
#include "Control/Control.h"    //Control

#endif
