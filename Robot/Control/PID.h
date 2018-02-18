#ifndef _PID_H_
#define _PID_H_
	#include <stdbool.h>
#ifndef _PID_DATA_TYPE_
    #define _PID_DATA_TYPE_ float
#endif
    typedef _PID_DATA_TYPE_ pid_data;
	typedef unsigned char pid_flags;

	//
	//	Estructura privada para PID
	//
	typedef struct _PID_private{
		pid_data *output;
		pid_data *input;
		pid_data error;
		pid_data last_input;
		pid_data cum_sum;
		pid_data delta;
	}_PID_private;
	
	//
	//	Estructura general para PID
	//
	typedef struct PID_t{
		_PID_private	memory;//RAM para el calculo
		pid_data 		Kp,Ki,Kd;//Constantes para calcular PID
		pid_data		min;
		pid_data		max;
		pid_flags		state;//Bits de estado del calculo
		pid_data		set_point;
	}PID_buffer;
	
	typedef PID_buffer* PID_Handle;
	
	//
	//	Crear un nuevo buffer para calcular PID
	//Es necesario especificar el registro de entrada y salida
	//
extern	PID_Handle newPIDComputer(pid_data*,pid_data*,PID_buffer*);
	
	//
	// 	Calcular la nueva salida a partir del registro de entrada
	//
extern	void PID_Compute(PID_Handle);
#endif
