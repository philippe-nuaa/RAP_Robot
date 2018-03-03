/*
 * IMU.c
 *
 *  Created on: 19/02/2018
 *      Author: josepablocb
 */

#include "IMUSensor.h"

Task_Struct     IMUTaskStruct;
Char IMUTask_Stack[IMUTASKSTACKSIZE];

Semaphore_Struct IMUsemStruct;
Semaphore_Handle IMUsemHandle;

IMU_axis    IMU_LastAccel;
IMU_axis    IMU_Accel;

IMU_axis    IMU_Gyr;

float       Racc;
float       Roll;
float       Pitch;
float       Yaw;


void IMUInterruptFx(unsigned int Index){
    Semaphore_post(IMUsemHandle);
}

#define Significant     100
#define OffSet          0.06

//
//Filtro promediador simple
//
#define alpha   IMUSmoothFactor
void Filter(){
    if(IMU_Accel.isValid){
        IMU_Accel.X_axis = alpha*IMU_Accel.X_axis + (1-alpha)*IMU_LastAccel.X_axis;
        IMU_Accel.Y_axis = alpha*IMU_Accel.Y_axis + (1-alpha)*IMU_LastAccel.Y_axis;
        IMU_Accel.Z_axis = alpha*IMU_Accel.Z_axis + (1-alpha)*IMU_LastAccel.Z_axis;

        IMU_LastAccel = IMU_Accel;
    }
    if(IMU_Gyr.isValid){
        IMU_Gyr.X_axis = (float)((int)(IMU_Gyr.X_axis*Significant))/Significant;
    }
}

Void IMUTask(UArg arg0, UArg arg1){
IMUTASK_LOOP:
    //Esperar interrupcion
    Semaphore_pend(IMUsemHandle, BIOS_WAIT_FOREVER);
        //Actualizar buffer
        IMU_Accel = IMU_ReadAccelerometer(Robot_IMU);
        IMU_Gyr   = IMU_ReadGyroscope(Robot_IMU);
        //Aplicar un filtro pasa bajas
        Filter();
        //Unicamente si son validos los datos actualizamos
        if(IMU_Accel.isValid){
            Racc = powf(powf(IMU_Accel.X_axis,2)+powf(IMU_Accel.Y_axis,2)+powf(IMU_Accel.Z_axis,2),0.5);
            Roll  = (atan2f(IMU_Accel.Z_axis,IMU_Accel.Y_axis))-OffSet;//acosf(IMU_Accel.Y_axis/Racc);
                Roll = (float)((int)(Significant*Roll))/Significant;

            //Off set for 0 rads calibration
            Roll -= 1.5;
            if(Roll < -3.14){
                Roll += 6.14;
            }
            //Off set for 0 rads calibration

            Pitch = acosf(IMU_Accel.X_axis/Racc);
            Yaw   = acosf(IMU_Accel.Z_axis/Racc);
        }
    goto IMUTASK_LOOP;
}

void IMUSensor_init(){
    Task_Params         taskParams;
    Semaphore_Params    semParams;

    //Dar de alta un evento de interrupcion
    IMU_setCallback(Robot_IMU, IMUInterruptFx, Robot_IMU_INT_PIN);

    //Configurar el sistema para atender a esas interrupciones
    Task_Params_init(&taskParams);
        taskParams.stackSize    = IMUTASKSTACKSIZE;
        taskParams.stack        = &IMUTask_Stack;
        taskParams.priority     = IMUSensor_Priority;
    Task_construct(&IMUTaskStruct, (Task_FuncPtr)IMUTask, &taskParams, NULL);

        //Crear un semaforo para sincronizar tareas
        Semaphore_Params_init(&semParams);
            //No hay recurso activo, se espera interrupcion una vez activado el sistema
        Semaphore_construct(&IMUsemStruct, 1, &semParams);
        //Obtener un manejador de semaforo
        IMUsemHandle = Semaphore_handle(&IMUsemStruct);

    //Limpiar variables buffer
    IMU_Accel.X_axis = 0.0;
    IMU_Accel.Y_axis = 0.0;
    IMU_Accel.Z_axis = 0.0;
    IMU_Accel.isValid = false;

    IMU_Gyr.X_axis = 0.0;
    IMU_Gyr.Y_axis = 0.0;
    IMU_Gyr.Z_axis = 0.0;
    IMU_Gyr.isValid = false;

    IMU_LastAccel.X_axis = 0.0;
    IMU_LastAccel.Y_axis = 0.0;
    IMU_LastAccel.Z_axis = 0.0;
    IMU_LastAccel.isValid = true;
}
void IMUSensor_Enable(){
    IMU_EnableEvents(Robot_IMU);
}
void IMUSensor_Disable(){
    IMU_DisableEvents(Robot_IMU);
}

rad IMUSensor_getXRotation(void){
    return IMU_Gyr.X_axis;
}
rad IMUSensor_getYRotation(void){
    return IMU_Gyr.Y_axis;
}
rad IMUSensor_getZRotation(void){
    return IMU_Gyr.Z_axis;
}


rad IMUSensor_getRoll(){
    return Roll;
}
rad IMUSensor_getPitch(){
    return Pitch;
}
rad IMUSensor_getYaw(){
    return Yaw;
}

//IMU_axis IMUSensor_getTilt(){
//    IMU_axis buffer = {0.0,0.0,0.0,1};
//    buffer.X_axis = IMUSensor_getRoll();
//    buffer.Y_axis = IMUSensor_getPitch();
//    buffer.Z_axis = IMUSensor_getYaw();
//    return buffer;
//}

rad IMUSensor_getRacc(){
    return Racc;
}
