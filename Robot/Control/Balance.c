/*
 * Balance.c
 *
 *  Created on: 21/02/2018
 *      Author: josepablocb
 */


#include "Balance.h"

PID_buffer          balance_buffer_acc;
PID_Handle          balance_pid_acc;
PID_buffer          balance_buffer_gyr;
PID_Handle          balance_pid_gyr;

volatile pid_data input_acc  = 0.0;
volatile pid_data output_acc = 0.0;

volatile pid_data input_gyr  = 0.0;
volatile pid_data output_gyr = 0.0;

Clock_Struct        BalanceControlStruct;
Clock_Handle        BalanceControl_Handle;


Void __Balance_Control__(UArg arg){
    input_acc = IMUSensor_getRoll();
    input_gyr = IMUSensor_getXRotation();

    PID_Compute(balance_pid_acc);
    PID_Compute(balance_pid_gyr);

//    Left_Wheel_Speed( output_acc  + output_gyr  );
//    Right_Wheel_Speed( output_acc + output_gyr );
    MotorDriver_Left(Robot_Motor,output_acc  + output_gyr);
    MotorDriver_Right(Robot_Motor,output_acc  + output_gyr);
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
//    Wheels_start();
    IMUSensor_Enable();

    //
    //  Inicializar buffer PID
    //
    balance_pid_acc = newPIDComputer(&input_acc,&output_acc,&balance_buffer_acc);
    balance_pid_acc->max        =  Balance_max_min;
    balance_pid_acc->min        =  -1*Balance_max_min;
    balance_pid_acc->set_point  =  0;
    balance_pid_acc->Kp = Balance_Kp;
    balance_pid_acc->Ki = Balance_Ki;
    balance_pid_acc->Kd = Balance_Kd;

    balance_pid_gyr = newPIDComputer(&input_gyr,&output_gyr,&balance_buffer_gyr);
    balance_pid_gyr->max        =  Balance_max_min;
    balance_pid_gyr->min        =  -1*Balance_max_min;
    balance_pid_gyr->set_point  =  0;
    balance_pid_gyr->Kp = Balance_Kp;
    balance_pid_gyr->Ki = Balance_Ki;
    balance_pid_gyr->Kd = Balance_Kd;


    //
    //   Crear una tarea periodica
    //
    Clock_Params_init(&clkParams);
    clkParams.period = 4; //4 period of miliseconds
    clkParams.startFlag = FALSE;//Triggered!

    /* Construct a periodic Clock Instance with period = 4 system time units */
    Clock_construct(&BalanceControlStruct, (Clock_FuncPtr)__Balance_Control__,
                    1, &clkParams);

    BalanceControl_Handle = Clock_handle(&BalanceControlStruct);
}

#define MAX_CALIB_ITER  100
//
//  Calibrar angulo de balance
//
void Balance_calibrate(){
    //Esperar a que se establezcan los valores en el IMU
    unsigned int i=0;
    rad buffer_acc=0.0;
    rad buffer_gyr=0.0;
    for(i=0;i<MAX_CALIB_ITER;i++){
        Task_sleep(5);
        buffer_acc += IMUSensor_getRoll()/MAX_CALIB_ITER;
        buffer_gyr += IMUSensor_getXRotation()/MAX_CALIB_ITER;//IMUSensor_getRoll()/MAX_CALIB_ITER;
    }
    balance_pid_acc->set_point = buffer_acc;
    balance_pid_gyr->set_point = buffer_gyr;
}

//
//  Activa el proceso de control
//Se recomienda no hacer uso de Speed cuando
//  este sistema esté activo
//
void Balance_start(){
    MotorDriver_Start(Robot_Motor);
//    Wheels_start();
    Clock_start(BalanceControl_Handle);
}
//
//  Desactiva el proceso de control
//
void Balance_stop(){
    MotorDriver_Stop(Robot_Motor);
//    Wheels_stop();
    Clock_stop(BalanceControl_Handle);
}

void Balance_setAccKp(float data){
    balance_pid_acc->Kp = data;
}
void Balance_setAccKi(float data){
    balance_pid_acc->Ki = data;
}
void Balance_setAccKd(float data){
    balance_pid_acc->Kd = data;
}

void Balance_setGyrKp(float data){
    balance_pid_gyr->Kp = data;
}
void Balance_setGyrKi(float data){
    balance_pid_gyr->Ki = data;
}
void Balance_setGyrKd(float data){
    balance_pid_gyr->Kd = data;
}
