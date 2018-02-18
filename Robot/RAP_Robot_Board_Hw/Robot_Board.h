/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "RAP_Robot_TM4C123G.h"

#define Board_initGeneral           EK_TM4C123GXL_initGeneral
#define Board_initGPIO              EK_TM4C123GXL_initGPIO
#define Board_initI2C               EK_TM4C123GXL_initI2C
#define Board_initPWM               EK_TM4C123GXL_initPWM
#define Board_initSDSPI             EK_TM4C123GXL_initSDSPI
#define Board_initSPI               EK_TM4C123GXL_initSPI
#define Board_initUART              EK_TM4C123GXL_initUART

#define Board_A4                    EK_TM4C123GXL_A4
#define Board_A5                    EK_TM4C123GXL_A5
#define Board_B4                    EK_TM4C123GXL_B4
#define Board_B5                    EK_TM4C123GXL_B5
#define Board_F1                    EK_TM4C123GXL_F1

#define Board_I2C1                  EK_TM4C123GXL_I2C1

#define Board_PWM2                  EK_TM4C123GXL_PWM2
#define Board_PWM3                  EK_TM4C123GXL_PWM3
#define Board_PWM6                  EK_TM4C123GXL_PWM6
#define Board_PWM7                  EK_TM4C123GXL_PWM7

#define Board_SPI0                  EK_TM4C123GXL_SPI0
#define Board_SPI1                  EK_TM4C123GXL_SPI3

#define Board_UART0                 EK_TM4C123GXL_UART0

#define Board_QEI0                  0
#define Board_QEI1                  1

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */
