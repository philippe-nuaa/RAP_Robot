/*
 * IMU.h
 *
 *  Created on: 19/02/2018
 *      Author: josepablocb
 */

#ifndef _IMUSensor_H_
#define _IMUSensor_H_
    #include "Robot.h"
    #include <math.h>

    #define IMUTASKSTACKSIZE    256
    #define IMUSensor_Priority  1

    #define IMUSmoothFactor     0.5
#ifndef RAD_DEF
#define RAD_DEF
    typedef float rad;
#endif

extern void IMUSensor_init(void);
extern void IMUSensor_Enable(void);
extern void IMUSensor_Disable(void);

extern rad IMUSensor_getXRotation(void);
extern rad IMUSensor_getYRotation(void);
extern rad IMUSensor_getZRotation(void);

extern rad IMUSensor_getRoll(void);
extern rad IMUSensor_getPitch(void);
extern rad IMUSensor_getYaw(void);

extern rad IMUSensor_getRacc(void);

#endif
