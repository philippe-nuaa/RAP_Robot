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

IMU_axis    IMU_Accel;
float       Racc;
IMU_axis    IMU_Gyr;
float       Rgyr;

void IMUInterruptFx(unsigned int Index){
    Semaphore_post(IMUsemHandle);
}
Void IMUTask(UArg arg0, UArg arg1){
IMUTASK_LOOP:
    //Esperar interrupcion
    Semaphore_pend(IMUsemHandle, BIOS_WAIT_FOREVER);
        //Actualizar buffer
        IMU_Accel = IMU_ReadAccelerometer(Robot_IMU);
        IMU_Gyr   = IMU_ReadGyroscope(Robot_IMU);
        if(IMU_Accel.isValid)
            Racc = powf(powf(IMU_Accel.X_axis,2)+powf(IMU_Accel.Y_axis,2)+powf(IMU_Accel.Z_axis,2),0.5);
        else
            Racc = 0.0;

        if(IMU_Gyr.isValid)
            Rgyr = powf(powf(IMU_Gyr.X_axis,2)+powf(IMU_Gyr.Y_axis,2)+powf(IMU_Gyr.Z_axis,2),0.5);
        else
            Rgyr = 0.0;
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
}
void IMUSensor_Enable(){
    IMU_EnableEvents(Robot_IMU);
}
void IMUSensor_Disable(){
    IMU_DisableEvents(Robot_IMU);
}

rad IMUSensor_getRoll(){
    return acosf(IMU_Accel.Y_axis/Racc);
}
rad IMUSensor_getPitch(){
    return acosf(IMU_Accel.X_axis/Racc);
}
rad IMUSensor_getYaw(){
    return acosf(IMU_Accel.Z_axis/Racc);
}

IMU_axis IMUSensor_getTilt(){
    IMU_axis buffer = {0.0,0.0,0.0,1};
    buffer.X_axis = IMUSensor_getRoll();
    buffer.Y_axis = IMUSensor_getPitch();
    buffer.Z_axis = IMUSensor_getYaw();
    return buffer;
}

rad IMUSensor_getRacc(){
    return Racc;
}

rad IMUSensor_getRgyr(){
    return Rgyr;
}