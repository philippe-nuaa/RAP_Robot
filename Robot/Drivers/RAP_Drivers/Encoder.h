/** ============================================================================
 *  @file       Encoder.h
 *
 *  @brief      Encoder peripheral driver interface
 *
 *  Created on: 19/01/2018
 *      Author: Josepablo Cruz Baas
 *
 *  El encabezado debe ser incluido en una aplicacion como sigue:
 *  @code
 *  #include "Encoder.h"
 *  @endcode
 *
 *  # Operacion #
 *  El driver Encoder permite operar al periferico interno QEI para la
 *  lectura de encoders externos, comunmente utilizado para el monitoreo
 *  de velocidad y/o posicion de motores.
 *  Las APIs en este driver sirven como una interfaz para una aplicacion tipica
 *  TI-RTOS.
 *
 *  ## Abriendo el driver #
 *  @code
 *  Encoder_state      state;
 *  Encoder_Handle     handle;
 *
 *  handle = Encoder_open(QEI_index,&state);
 *  if(!handle){
 *      System_printf("Encoder no ha sido abierto");
 *  }
 *  @endcode
 *
 *  ## Lectura de velocidad del encoder #
 *  @code
 *  Encoder_vel    vel;
 *  vel = Encoder_ReadVelocity(handle);
 *  @endcode
 *
 *  ## Lectura de posicion del enconder #
 *  @code
 *  Encoder_pos    pos;
 *  pos = Encoder_ReadPosition(Encoder_handle);
 *  @endcode
 *
 *  ## Asignacion de eventos #
 *  @code
 *  Encoder_setCallback(Encoder_handle,SensorEvent,BOARD_Encoder_INT_PIN);
 *  Encoder_EnableEvents(Encoder_handle);
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
 *      #Encoder_Handle         Manejador del sensor Encoder.
 *      #Encoder_Params         Parametros de inicializacion del hardware.
 *      #Encoder_State          Buffer de estados del manejador.
 *  ============================================================================
 *  ESTRUCTURAS ESPECIFICAS DEL DRIVER
 *      #Encoder_vel            Velocidad con signo del encoder en radianes / segundo.
 *      #Encoder_pos            Posicion del encoder en radianes. (sin signo).
 *      #Encoder_EventType      Bandera/Selector de tipo de interrupcion
 *  ============================================================================
 *  FUNCIONES GENERICAS DEL DRIVER
 *      #Encoder_Params_init    Configura los parametros por defecto del hardware.
 *      #Encoder_init           Macro, Inicializa el driver sin manipular algun
 *                                  registro del hardware.
 *      #Encoder_open           Inicializa el hardware.
 *      #Encoder_close          Libera el hardware.
 *  ============================================================================
 *  FUNCIONES ESPECIFICAS DEL DRIVER
 *      #Encoder_ReadVelocity                   Leer la velocidad del encoder en radianes por segundo
 *      #Encoder_ReadPosition                   Leer la posicion del encoder en radianes
 *      #Encoder_Sleep                          Fuerza el modo de bajo consumo del periferico
 *      #Encoder_setCallback                    Asigna un evento de interrupcion al pin INT del Encoder.
 *      #Encoder_EnableEvents                   Activa los eventos de interrupcion
 *      #Encoder_DisableEvents                  Desactiva los eventos de interrupcion
 *  ============================================================================
 *  MACROS DEL DRIVER
 *      #Encoder_init          Inicializa el driver sin manipular algun registro
 *                              del hardware.
 *  ============================================================================
 */
/*
 * Encoder.h
 *
 *  Created on: 19/01/2018
 *      Author: josepablocb
 */

#ifndef RAP_DRIVERS_ENCODER_H_
#define RAP_DRIVERS_ENCODER_H_

#include <stdint.h>
#include <stdbool.h>
#include <driverlib/qei.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <inc/hw_gpio.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>

//#include "Utils/ManualReg.h"
//#include "inc/hw_qei.h"

//#define QEI_FUNC    6
#ifndef     ENCODER_CPR
    #define ENCODER_CPR             2797
#endif
//RTOS cpu freq
#ifndef     CPU_FREQ
    #define CPU_FREQ                80000000
#endif
//ENCODER SAMPLE PERIOD - Number of clock ticks over which to measure the velocity
#ifndef     ENCODER_PERIOD
    #define ENCODER_PERIOD          4000000
#endif
/*
 *  ============================================================================
    ESTRUCTURAS GENERICAS DEL DRIVER
 *  ============================================================================
*/

typedef struct Encoder_state{
    unsigned int  QEI_BASE;
    unsigned int  PORT_BASE;
    unsigned int  PINS_PORT;
}Encoder_state;

typedef Encoder_state*  Encoder_Handle;

/*
 *  ============================================================================
    ESTRUCTURAS ESPECIFICAS DEL DRIVER
 *  ============================================================================
*/
//Radianes / Segundo
typedef float           Encoder_vel;
//Radianes
typedef float           Encoder_pos;
//Indice del QEI
typedef unsigned char   QEI_index;
//Formato de evento de interrupcion
typedef void (*Encoder_Event)(void);

//Parametros de configuracion del periferico
typedef struct Encoder_Configuration{

}Encoder_Configuration;
//Tipo de evento a registrar
typedef enum Encoder_EventType{
    Index_pulse=0,
    Velocity_timer_expiration,
    Direction_change,
    Quadrature_error_detection
}EventType;

/*
 *  ============================================================================
    FUNCIONES ESPECIFICAS DEL DRIVER
 *  ============================================================================
*/
/*!
         * @brief  Inicializacion del driver
         *
         *  Configuraciones previas necesarias para el driver
         *
        */
extern void             Encoder_init();
/*!
         * @brief  configura y solicita el hardware para crear un manejador.
         *
         *  Funcion para configurar el hardware y solicitar el manejador del mismo.
         *
         *  @param  Encoder_state*      Apuntador al registro de estado.
         *  @return Encoder_Handle      Manejador del hardware. NULL de haber habido un error.
         *
        */
extern Encoder_Handle   Encoder_open(QEI_index,Encoder_state*);
/*!
         * @brief  Libera el hardware solicitado.
         *
         *  Funcion para configurar el hardware y solicitar el manejador del mismo.
         *
         *  @param  Encoder_Handle      Manejador del hardware.
         *
        */
extern void             Encoder_close(Encoder_Handle);

/*
 *  ============================================================================
    FUNCIONES ESPECIFICAS DEL DRIVER
 *  ============================================================================
*/
/*!
         * @brief  Recuperar velocidad angular del encoder
         *
         *  Funcion que recupera y convierte la velocidad capturada del encoder.
         *
         *  @param  Encoder_Handle      Manejador del hardware.
         *  @return Encoder_vel         Velocidad del encoder en rad/seg.
         *
        */
extern Encoder_vel      Encoder_ReadVelocity(Encoder_Handle);
/*!
         * @brief  Recuperar posicion en radianes del encoder
         *
         *  Funcion que recupera la ultima posicion capturada del encoder.
         *
         *  @param  Encoder_Handle      Manejador del hardware.
         *  @return Encoder_pos         Posicion del encoder en rad.
         *
        */
extern Encoder_pos      Encoder_ReadPosition(Encoder_Handle);
/*!
         * @brief  Desactivar el encoder para entrar en modo de bajo consumo
         *
         *  Funcion que fuerza el modo de bajo consumo del periferico.
         *
         *  @param  Encoder_Handle              Manejador del hardware.
         *
        */
extern void             Encoder_Sleep(Encoder_Handle);
/*!
         * @brief  Reactiva el encoder para entrar en modo de funcionamiento
         *
         *  Funcion que reactiva el modulo QEI si esta en modo de bajo consumo.
         *
         *  @param  Encoder_Handle              Manejador del hardware.
         *
        */
extern void             Encoder_Wake(Encoder_Handle);
/*!
         * @brief  Establecer funcion de interrupcion
         *
         *  Funcion para registrar un evento de interrupcion segun el tipo.
         *
         *  @param  Encoder_Handle              Manejador del hardware.
         *  @param  Encoder_Event               Funcion evento.
         *  @param  Encoder_EventType           Tipo de evento a registrar.
         *
        */
extern void             Encoder_setCallback(Encoder_Handle,Encoder_Event);
/*!
         * @brief  Inicia la monitorizacion de algun evento.
         *
         *  Funcion para comenzar a monitorear un evento registrado.
         *
         *  @param  Encoder_Handle              Manejador del hardware.
         *  @param  Encoder_EventType           Tipo de evento a activar
         *
        */
extern void             Encoder_EnableEvents(Encoder_Handle);
/*!
         * @brief  Desactiva la monitorizacion de algun evento.
         *
         *  Funcion para desactivar un evento especifico
         *
         *  @param  Encoder_Handle              Manejador del hardware.
         *  @param  Encoder_EventType           Tipo de evento a activar
        */
extern void             Encoder_DisableEvents(Encoder_Handle);

#endif /* RAP_DRIVERS_ENCODER_H_ */
