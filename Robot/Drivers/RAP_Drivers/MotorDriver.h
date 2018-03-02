/** ============================================================================
 *  @file       MotorDriver.h
 *
 *  @brief      H Bridge peripheral driver interface
 *
 *  Created on: 02/02/2018
 *      Author: Josepablo Cruz Baas
 *
 *  El encabezado debe ser incluido en una aplicacion como sigue:
 *  @code
 *  #include "MotorDriver.h"
 *  @endcode
 *
 *  # Operacion #
 *  El Motor Driver permite operar un puente H conectado a 4 salidas PWM,
 *  con la capcidad de controlar el sentido y velocidad de dos motores DC.
 *  Las APIs en este driver sirven como una interfaz para una aplicacion tipica
 *  TI-RTOS.
 *
 *  ## Abriendo el driver #
 *  @code
 *  MotorDriver_params     params;
 *  MotorDriver_state      state;
 *  MotorDriver_Handle     handle;
 *
 *  MotorDriver_params_init(&params);
 *  handle = MotorDriver_open(MotorR,&state,&params);
 *  if(!handle){
 *      System_printf("Motor Driver no ha sido abierto");
 *  }
 *  @endcode
 *
 *  ## Control de sentido y velocidad del Motor #
 *  @code
 *      MotorDriver_Stop(handle);
 *      MotorDriver_Left(handle,100);
 *      MotorDriver_Right(handle,-50);
 *      MotorDriver_Start(handle);
 *  @endcode
 *
 *  ## Lectura de Corriente#
 *  @code
 *      var = MotorDriver_ReadCurrent(handle);
 *  @endcode
 *
 *  En este fichero de encabezado se definen:
 *      # Constantes para la manipulacion del periferico interno.
 *      # Constantes para la manipulacion de parametros y configuraciones de esta API.
 *      # Funciones API genericas para el manejo del periferico.
 *      # Funciones API especificas para el manejo del periferico.
 *
 *  ============================================================================
 *  CONSTANTES PARA EL DRIVER
 *  En este fichero se han definido 2 tipos de constantes.
 *      1.- Configuraciones tipicas del periferico..
 *      2.- Parametros y factores de conversion de velocidad y posicion.
 *  Las configuraciones tipicas para los registros del periferico, son valores
 *  binarios que reflejan un comportamiento en el periferico. El programador utiliza
 *  estas constantes como parametros de configuracion del periferico ya
 *  abierto y funcionando.
 *  No todas las configuraciones son posibles, leer la hoja tecnica para
 *  mayor informacion.
 *  ============================================================================
 *  ESTRUCTURAS GENERICAS DEL DRIVER
 *      #MotorDriver_Handle         Manejador del sensor MotorDriver.
 *      #MotorDriver_state          Buffer de estados del manejador.
 *      #MotorDriver_params         Parametros de inicializacion del hardware.
 *  ============================================================================
 *  ESTRUCTURAS ESPECIFICAS DEL DRIVER
 *      #MotorDriver_Current        La corriente aproximada consumida en cada motor.
 *  ============================================================================
 *  FUNCIONES GENERICAS DEL DRIVER
 *      #MotorDriver_params_init    Inicializar los parametros de hardware a por defecto.
 *      #MotorDriver_open           Inicializa el hardware.
 *      #MotorDriver_close          Libera el hardware.
 *  ============================================================================
 *  FUNCIONES ESPECIFICAS DEL DRIVER
 *      #MotorDriver_Readcurrent                    Leer la corriente consumida por el puente H.
 *      #MotorDriver_Start                          Fuerza el modo de bajo consumo del periferico
 *      #MotorDriver_Stop                           Asigna un evento de interrupcion al pin INT del MotorDriver.
 *      #MotorDriver_Left                           Control de movimiento de motor izquierdo
 *      #MotorsDriver_Right                          Control de movimiento de motor derecho
 *  ============================================================================
 *  MACROS DEL DRIVER
 *      #MotorDriver_init          Inicializa el driver sin manipular algun registro
 *                                  del hardware.
 *  ============================================================================
 */
/*
 * MotorDriver.h
 *
 *  Created on: 02/02/2018
 *      Author: Josepablo Cruz Baas
 */
#ifndef RAP_DRIVERS_MotorDriver_H_
#define RAP_DRIVERS_MotorDriver_H_

#include "../../RAP_Robot_Board_Hw/Robot_Board.h"

#include <stdint.h>
#include <stdbool.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/GPIO.h>

#include <driverlib/gpio.h>
#include <driverlib/adc.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_pwm.h>

/*
 *  ============================================================================
    MACROS DEL DRIVER
 *  ============================================================================
*/
/*!
    * @brief  Inicializacion del driver
    *
    *  Funcion para configurar el driver, activando los perifericos
    *   correspondientes pero sin modificar los registros correspondientes
    *
    */
    #define MotorDriver_init()  {Board_initGPIO();Board_initPWM();}

/*
 *  ============================================================================
    CONSTANTES DEL DRIVER
 *  ============================================================================
*/
//
//  Configuracion pwm
//      microseconds

    #define MotorDriver_Base                PWM1_BASE

    #define MotorDriver_PWMPeriod_Default   20000

    #define MotorDriver_PWMSCALAR_100       65535
    #define MotorDriver_PWMOff              0

    #define MotorDriver_EN_Off              0
    #define MotorDriver_EN_On               1
    #define MotorDriver_D1_Off              1
    #define MotorDriver_D1_On               0
//
//  Configuracion ADC
//
    //    #define DEFAULT_FB_RESISTOR             220.0
    //Resolution * Impedance = 220 * 4096
    #define MotorDriver_ADFACT              901120
//
//  GPIO
//
    #define MotorDriver_EN_Default          Board_B5
    #define MotorDriver_LD1_Default         Board_F1
    #define MotorDriver_RD1_Default         Board_B4
    #define MotorDriver_ML_SF_Default       Board_A5
    #define MotorDriver_MR_SF_Default       Board_A4
//
//  ADC
//
    #define MotorDriver_ML_FB_ADPeriph      SYSCTL_PERIPH_ADC0
    #define MotorDriver_ML_FB_ADBase        ADC0_BASE
    #define MotorDriver_ML_FB_ADch          ADC_CTL_CH4
    #define MotorDriver_ML_FB_PinBase       GPIO_PORTD_BASE
    #define MotorDriver_ML_FB_Pin           GPIO_PIN_3

    #define MotorDriver_MR_FB_ADPeriph      SYSCTL_PERIPH_ADC0
    #define MotorDriver_MR_FB_ADBase        ADC0_BASE
    #define MotorDriver_MR_FB_ADch          ADC_CTL_CH5
    #define MotorDriver_MR_FB_PinBase       GPIO_PORTD_BASE
    #define MotorDriver_MR_FB_Pin           GPIO_PIN_2
//
//  PWM
//
    #define MotorDriver_ML_F_Default        Board_PWM2
    #define MotorDriver_ML_R_Default        Board_PWM3

    #define MotorDriver_MR_F_Default        Board_PWM7
    #define MotorDriver_MR_R_Default        Board_PWM6


/*
 *  ============================================================================
    ESTRUCTURAS GENERICAS DEL DRIVER
 *  ============================================================================
*/

typedef struct MotorDriver_Params{
     unsigned int  PWM_period;
     //Salidas digitales
     unsigned char EN_Index;  //Enable commun
     unsigned char RD1_Index; //Right D1
     unsigned char LD1_Index; //Left D1

     //Salidas PWM
     unsigned char MR_F_Index;//Motor Right F
     unsigned char MR_R_Index;//Motor Right R
     // - - -
     unsigned char ML_F_Index;//Motor Left F
     unsigned char ML_R_Index;//Motor Left R

     //Entradas digitales
     unsigned char MR_SF_Index;//Motor Right Status Flag
     unsigned char ML_SF_Index;//Motor Left Status Flag
}MotorDriver_Params;

//
//  Los siguientes valores podrian ser definidos como constantes para
//      mantenerlos en la memoria de programa y asi ahorrar
//          memoria RAM
//
typedef struct MotorDriver_state{
    //Salidas digitales
    unsigned char EN_Index;  //Enable commun
    unsigned char RD1_Index; //Right D1
    unsigned char LD1_Index; //Left D1

    //Salidas PWM
    PWM_Handle MR_F_handle;//Motor Right F
    PWM_Handle MR_R_handle;//Motor Right R
    // - - -
    PWM_Handle ML_F_handle;//Motor Left F
    PWM_Handle ML_R_handle;//Motor Left R

    //Entradas digitales
    unsigned char MR_SF_Index;//Motor Right Status Flag
    unsigned char ML_SF_Index;//Motor Left Status Flag

    //Entradas Analogicas
    unsigned char MR_FB_Index;//Motor Right Feed Back
    unsigned char ML_FB_Index;//Motor Left Feed Back



    float PWM_fact;
}MotorDriver_state;

typedef MotorDriver_state*  MotorDriver_Handle;

/*
 *  ============================================================================
    ESTRUCTURAS ESPECIFICAS DEL DRIVER
 *  ============================================================================
*/
//Corriente consumida por cada motor
typedef struct MotorDriver_Current{
    float MotorRight;
    float MotorLeft;
}MotorDriver_Current;

//Porcentaje de ciclo de trabajo del -100 al 100
typedef float MotorDriver_DutyCycle;
/*
 *  ============================================================================
    FUNCIONES ESPECIFICAS DEL DRIVER
 *  ============================================================================
*/
/*!
    * @brief  Inicializar parametros de configuracion del hardware.
    *
    *  Configurar parametros por defecto para inicializar el hardware.
    *
    *  @param  MotorDriver_state Apuntador a buffer de estados del manejador.
    *
    *  @return MotorDriver_Handle Manejador del hardware, o NULL en caso de error.
    *                        0 si hubo error.
    *
    */
    extern void MotorDriver_Params_init(MotorDriver_Params*);
/*!
    * @brief  Configuracion del hardware y reservación del mismo
    *
    *  Funcion para configurar el hardware y reservarlo a un manejador
    *
    *  @param  MotorDriver_state Apuntador a buffer de estados del manejador.
    *
    *  @return MotorDriver_Handle Manejador del hardware, o NULL en caso de error.
    *                        0 si hubo error.
    *
    */
    extern MotorDriver_Handle   MotorDriver_open(MotorDriver_state*,MotorDriver_Params*);
/*!
    * @brief  Liberamos el hardware para otro manejador
    *
    *  Funcion para liberar el manejador del hardware.
    *
    *  @param  MotorDriver_Handle Manejador del hardware.
    *
    */
    extern void MotorDriver_close(MotorDriver_Handle);
/*
 *  ============================================================================
    FUNCIONES ESPECIFICAS DEL DRIVER
 *  ============================================================================
*/
/*!
    * @brief  Obtener la corriente aproximada de cada motor
    *
    * Obtener una lectura de la corriente aproximada que
    * se consume en los motores
    *
    * @param  MotorDriver_Handle Manejador del hardware.
    *
    * @return MotorDriver_Current Estructura con datos de corriente de cada motor.
    *
    */
    extern MotorDriver_Current MotorDriver_Readcurrent(MotorDriver_Handle);
/*!
    * @brief  Activar el puente H.
    *
    * Activar el puente H para comenzar a producir señales PWM.
    *
    * @param  MotorDriver_Handle Manejador del hardware.
    *
    */
    extern void MotorDriver_Start(MotorDriver_Handle);
/*!
    * @brief  Desactivar el puente H.
    *
    * Desactivar el puente H y evitar que la señal PWM llegue al motor.
    *
    */
    extern void MotorDriver_Stop(MotorDriver_Handle);
/*!
    * @brief  Forzar la sincronizacion de señales PWM
    *
    * Forzar la sincronizacion del periferico que genera la señal PWM.
    *
    * @param  MotorDriver_Handle Manejador del hardware.
    *
    */
    extern void MotorDriver_ForceSync(MotorDriver_Handle);
/*!
    * @brief  Controlar el motor izquierdo
    *
    * Segun un porcentaje de -100 a 100, se genera
    * la señal PWM con el ciclo de trabajo correspondiente
    * para el motor izquierdo
    *
    * @param  MotorDriver_Handle Manejador del hardware.
    *
    * @param  MotorDriver_DutyCycle Ciclo de trabajo del -100 al 100
    *
    */
    extern void MotorDriver_Left(MotorDriver_Handle, MotorDriver_DutyCycle);
/*!
    * @brief  Controlar el motor derecho
    *
    * Segun un porcentaje de -100 a 100, se genera
    * la señal PWM con el ciclo de trabajo correspondiente
    * para el motor derecho
    *
    * @param  MotorDriver_Handle Manejador del hardware.
    *
    * @param  MotorDriver_DutyCycle Ciclo de trabajo del -100 al 100
    *
    */
    extern void MotorDriver_Right(MotorDriver_Handle, MotorDriver_DutyCycle);
#endif /* RAP_DRIVERS_MotorDriver_H_ */
