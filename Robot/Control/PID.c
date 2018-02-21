#include "PID.h"

//
//  Crear un nuevo buffer para calcular PID
//Es necesario especificar el registro de entrada y salida
//
PID_Handle newPIDComputer(pid_data* input,pid_data* output,PID_buffer* pid_buffer){
    pid_buffer->memory.input        = input;
    pid_buffer->memory.output       = output;
    pid_buffer->memory.delta        = 0;
    pid_buffer->memory.last_input   = 0;
    pid_buffer->memory.cum_sum  = 0;
    pid_buffer->memory.error    = 0;

    pid_buffer->Kp  = 0;
    pid_buffer->Ki  = 0;
    pid_buffer->Kd  = 0;
    pid_buffer->set_point = 0;
    pid_buffer->min = 0;
    pid_buffer->max = 255;

    return pid_buffer;
}
//
//  Calcular la nueva salida a partir del registro de entrada
//
void PID_Compute(PID_Handle pid){
    pid->memory.error  = pid->set_point - *pid->memory.input;

//  pid->memory.delta   = *pid->memory.input - pid->memory.last_input;

    pid->memory.delta   = pid->memory.error - pid->memory.last_input;

    pid->memory.cum_sum += (pid->Ki) * (pid->memory.error);
    //*pid->memory.cum_sum -= (*pid->Kp) * (*pid->memory->delta);

    *pid->memory.output = (pid->Kp)*(pid->memory.error);
//    *pid->memory.output +=  (pid->memory.cum_sum)-(pid->Kd)*(pid->memory.delta);
    *pid->memory.output += (pid->memory.cum_sum) + (pid->Kd)*(pid->memory.delta);

    if((*pid->memory.output) > (pid->max))
        *pid->memory.output = pid->max;
    else if((*pid->memory.output) < (pid->min))
        *pid->memory.output = pid->min;

//    pid->memory.last_input = *pid->memory.input;
    pid->memory.last_input = pid->memory.error;
}
