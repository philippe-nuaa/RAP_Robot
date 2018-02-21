/*
 * Balance.c
 *
 *  Created on: 21/02/2018
 *      Author: josepablocb
 */


#include "Balance.h"

PID_buffer          balance_buffer;
PID_Handle          balance_pid;

volatile pid_data input  = 0;
volatile pid_data output  = 0;

Clock_Struct        BalanceControlStruct;
Clock_Handle        BalanceControl_Handle;


Void __Balance_Control__(UArg arg){
    input = IMUSensor_getRoll();
    PID_Compute(balance_pid);

    Left_Wheel_Speed(-1*output);
    Right_Wheel_Speed(-1*output);
}


//
//  Activar los drivers necesarios
//  Por la naturaleza de IMUSensor,
//  esta funcion solo puede ser
//  llamada desde el contexto de una tarea
//
void Balance_Init(){
    Clock_Params        clkParams;

    //
    //  Activar perifericos necesarios
    //
    Wheels_init();
    Robot_EnableIMU();
    IMUSensor_init();

    //
    //  Encender hardware
    //
    Left_Wheel_Speed(0.0);
    Right_Wheel_Speed(0.0);
    Wheels_start();
    IMUSensor_Enable();

    //
    //  Inicializar buffer PID
    //
    balance_pid = newPIDComputer(&input,&output,&balance_buffer);
    balance_pid->max        =  Balance_max_min;
    balance_pid->min        =  -1*Balance_max_min;
    balance_pid->set_point  =  0;
    balance_pid->Kp = Balance_Kp;
    balance_pid->Ki = Balance_Ki;
    balance_pid->Kd = Balance_Kd;


    //
    //   Crear una tarea periodica
    //
    Clock_Params_init(&clkParams);
    clkParams.period = 4; //4 period of miliseconds
    clkParams.startFlag = FALSE;//Triggered!

    /* Construct a periodic Clock Instance with period = 4 system time units */
    Clock_construct(&BalanceControlStruct, (Clock_FuncPtr)__Balance_Control__,
                    4, &clkParams);

    BalanceControl_Handle = Clock_handle(&BalanceControlStruct);
}

//
//  Calibrar angulo de balance
//
void Balance_calibrate(){
    //Esperar a que se establezcan los valores en el IMU
    Task_sleep(2);
    balance_pid->set_point = IMUSensor_getRoll();
}

//
//  Activa el proceso de control
//Se recomienda no hacer uso de Speed cuando
//  este sistema esté activo
//
void Balance_start(){
    Clock_start(BalanceControl_Handle);
}
//
//  Desactiva el proceso de control
//
void Balance_stop(){
    Wheels_stop();
    Clock_stop(BalanceControl_Handle);
}

void Balance_setKp(float data){
    balance_pid->Kp = data;
}
void Balance_setKi(float data){
    balance_pid->Ki = data;
}
void Balance_setKd(float data){
    balance_pid->Kd = data;
}

