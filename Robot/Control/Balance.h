/*
 * Balance.h
 *
 *  Created on: 21/02/2018
 *      Author: josepablocb
 */

#ifndef ROBOT_CONTROL_BALANCE_H_
#define ROBOT_CONTROL_BALANCE_H_

//Wheels incluye a Robot que incluye a PID
#include "../Wheels.h"
#include "../IMUSensor.h"

#define Balance_Kp      0.0
#define Balance_Ki      0.0
#define Balance_Kd      0.0

#define Balance_max_min 30.0

extern void Balance_Init();
extern void Balance_calibrate();
extern void Balance_start();
extern void Balance_stop();

extern void Balance_setKp(float);
extern void Balance_setKi(float);
extern void Balance_setKd(float);

#endif /* ROBOT_CONTROL_BALANCE_H_ */
