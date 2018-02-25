/*
 * Wheels.c
 *
 *  Created on: 17/02/2018
 *      Author: josepablocb
 */

#include "Wheels.h"

Clock_Struct        WheelsControlStruct;
Clock_Handle        WheelsControl_Handle;
//
//  Variables Globales
//
/////////////////////////////////
//Necesarias para PID
/////////////////////////////////
PID_buffer          buffer_left;
PID_buffer          buffer_right;
PID_Handle          pid_left;
PID_Handle          pid_right;
/////////////////////////////////
//Registros de Velocidad
/////////////////////////////////
volatile pid_data input_L  = 0;
volatile pid_data input_R  = 0;

volatile pid_data output_L = 0;
volatile pid_data output_R = 0;


Void __WheelsControl__(UArg arg){
    input_L = Encoder_ReadVelocity(Robot_ENCL);
    input_R = Encoder_ReadVelocity(Robot_ENCR);
    PID_Compute(pid_left);
    PID_Compute(pid_right);

    MotorDriver_Left(Robot_Motor,output_L);
    MotorDriver_Right(Robot_Motor,output_R);
}

void Wheels_init(){
    Clock_Params        clkParams;

    pid_left = newPIDComputer(&input_L,&output_L,&buffer_left);
    pid_left->max        =  100.0;
    pid_left->min        =  -100.0;
    pid_left->set_point  =  0;
    pid_left->Kp = Default_L_P;
    pid_left->Ki = Default_L_I;
    pid_left->Kd = Default_L_D;

    pid_right = newPIDComputer(&input_R,&output_R,&buffer_right);
    pid_right->max        =  100.0;
    pid_right->min        =  -100.0;
    pid_right->set_point  =  0;
    pid_right->Kp = Default_R_P;
    pid_right->Ki = Default_R_I;
    pid_right->Kd = Default_R_D;


    //
    //   Crear una tarea periodica
    //
    Clock_Params_init(&clkParams);
    clkParams.period = _Wheels_CTL_PERIOD_; //4 period of miliseconds
    clkParams.startFlag = FALSE;//Triggered!

    /* Construct a periodic Clock Instance with period = 4 system time units */
    Clock_construct(&WheelsControlStruct, (Clock_FuncPtr)__WheelsControl__,
                    1, &clkParams);

    WheelsControl_Handle = Clock_handle(&WheelsControlStruct);
}
void Wheels_start(){
    MotorDriver_Start(Robot_Motor);
    Clock_start(WheelsControl_Handle);
}
void Wheels_stop(){
    MotorDriver_Stop(Robot_Motor);
    Clock_stop(WheelsControl_Handle);
}

void Right_Wheel_Speed(rad speed){
    pid_right->set_point = speed;
}
void Left_Wheel_Speed(rad speed){
    pid_left->set_point = speed;
}
rad  Right_Wheel_readSpeed(){
    return Encoder_ReadVelocity(Robot_ENCR);
}
rad  Left_Wheel_readSpeed(){
    return Encoder_ReadVelocity(Robot_ENCL);
}


void Left_setKp(rad p){
    pid_left->Kp = p;
}
void Left_setKi(rad i){
    pid_left->Ki = i;
}
void Left_setKd(rad d){
    pid_left->Kd = d;
}

void Right_setKp(rad p){
    pid_right->Kp = p;
}
void Right_setKi(rad i){
    pid_right->Ki = i;
}
void Right_setKd(rad d){
    pid_right->Kd = d;
}
