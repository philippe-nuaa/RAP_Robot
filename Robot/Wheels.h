/*
 * Wheels.h
 *
 *  Created on: 17/02/2018
 *      Author: josepablocb
 */

#ifndef ROBOT_DRIVERS_WHEELS_H_
#define ROBOT_DRIVERS_WHEELS_H_

#include "Robot.h"

#define Default_L_P     2.0
#define Default_L_I     0.2
#define Default_L_D     0.8

#define Default_R_P     2.0
#define Default_R_I     0.2
#define Default_R_D     0.8

#ifndef RAD_DEF
#define RAD_DEF
    typedef float rad;
#endif
extern void Wheels_init(void);

extern void Wheels_start(void);
extern void Wheels_stop(void);

//
//  Control de rueda en radianes
//
extern void Right_Wheel_Speed(rad);

extern void Left_Wheel_Speed(rad);

extern rad  Right_Wheel_readSpeed();
extern rad  Left_Wheel_readSpeed();
#endif /* ROBOT_DRIVERS_WHEELS_H_ */
