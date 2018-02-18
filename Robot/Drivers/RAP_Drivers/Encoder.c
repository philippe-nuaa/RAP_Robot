/*
 * Encoder.c
 *
 *  Created on: 19/01/2018
 *      Author: josepablocb
 */
#include "Encoder.h"

/*
 *  ============================================================================
    FUNCIONES ESPECIFICAS DEL DRIVER
 *  ============================================================================
*/
//         * @brief  Inicializacion del driver
 void Encoder_init(){
//     WR8(SYSCTL_RCGCQEI)  = 0x3;//Encender ambos QEI
//     WR8(SYSCTL_RCGCGPIO) = R3|R2;//Activar GPIO PD y PC
//     //Esperar activacion del periferico
//     while(!( WR8(SYSCTL_PRQEI) & 0x3 )){}
//     while(!( WR8(SYSCTL_PRGPIO) & (R3|R2) )){}
     //Configurar perifericos QEI
     SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
     //Configurar perifericos GPIO
     SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
     //Esperar al periferico QEI
     while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI0)){}
     while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI1)){}
     //Esperar al periferico GPIO
     while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}
     while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)){}

     //PUERTO D BLOQUEADO POR DEFECTO!
     HWREG(GPIO_PORTD_BASE | GPIO_O_LOCK)  = GPIO_LOCK_KEY;
     HWREG(GPIO_PORTD_BASE | GPIO_O_CR)   |= 0x80;
     HWREG(GPIO_PORTD_BASE | GPIO_O_LOCK)  = 0;
}
//         * @brief  configura y solicita el hardware para crear un manejador.
 Encoder_Handle   Encoder_open(QEI_index index, Encoder_state* state){
     uint32_t   PORT_BASE;
     uint32_t   PINS_PORT;
     uint32_t   QEI_CONFIG;
     //PORT C IS QEI1   |   PORT D IS QEI0
     //PORT F could be for QEI0
     PORT_BASE = (index)?GPIO_PORTC_BASE:GPIO_PORTD_BASE;
     state->PORT_BASE = PORT_BASE;
     //PINS C5-C6 are QEI1  phA-phB     |   PINS D6-D7 are QEI0 phA-phB
     //PINS F0-F1 could be for QEI0 phA-phB
     PINS_PORT = (index)?GPIO_PIN_5|GPIO_PIN_6:GPIO_PIN_6|GPIO_PIN_7;
     state->PINS_PORT = PINS_PORT;

     state->QEI_BASE = (index)? QEI1_BASE:QEI0_BASE;

     //Configurar los pines
     if(index){
         GPIOPinConfigure(GPIO_PC5_PHA1);
         GPIOPinConfigure(GPIO_PC6_PHB1);
         QEI_CONFIG = (QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET |
                 QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP);
     }else{
         //EL ENCODER 0 NECESITA INVERTIR LOS PINES POR SOFTWARE
         //YA QUE EL MOTOR ROTARA EN SENTIDO OPUESTO.
         GPIOPinConfigure(GPIO_PD6_PHA0);
         GPIOPinConfigure(GPIO_PD7_PHB0);
         QEI_CONFIG = (QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET |
                 QEI_CONFIG_QUADRATURE | QEI_CONFIG_SWAP);
     }
     GPIOPinTypeQEI(PORT_BASE,PINS_PORT);
     //Configurar el encoder
//     WR32(state->QEI_BASE | QEI_O_CTL)    = QEI_CTL_VELDIV_1 | QEI_CTL_VELEN | QEI_CTL_CAPMODE | QEI_CTL_SWAP;
//     WR32(state->QEI_BASE | QEI_O_LOAD)   = 4000000;//28602;//.8
     QEIConfigure(state->QEI_BASE, QEI_CONFIG, ENCODER_CPR - 1);
     QEIVelocityConfigure(state->QEI_BASE, QEI_VELDIV_1, ENCODER_PERIOD);
     QEIVelocityEnable(state->QEI_BASE);

//     Activar el encoder
//     WR32(state->QEI_BASE | QEI_O_CTL)   |= QEI_CTL_ENABLE;
     QEIPositionSet(state->QEI_BASE,0x00);//Configurar posicion inicial
     QEIEnable(state->QEI_BASE);
     return state;
}
//         * @brief  Libera el hardware solicitado.
void Encoder_close(Encoder_Handle encoder_handle){
    QEIDisable(encoder_handle->QEI_BASE);
}

/*
 *  ============================================================================
    FUNCIONES ESPECIFICAS DEL DRIVER
 *  ============================================================================
*/
//         * @brief  Recuperar velocidad angular del encoder
 Encoder_vel  Encoder_ReadVelocity(Encoder_Handle encoder_handle){
     //     uint8_t  QEI_GetDirection(uint32_t QEI_BASE){
     //         return WR32((QEI_BASE|QEI_O_STAT))>>1;
     //     }
     //return WR32(encoder_handle->QEI_BASE | QEI_O_SPEED);
     Encoder_vel    actual_vel;
     actual_vel = (float)QEIVelocityGet(encoder_handle->QEI_BASE);
     actual_vel *= (CPU_FREQ/ENCODER_PERIOD);//Convertir a frecuencia de ticks (Hz)
     actual_vel /= ENCODER_CPR;//Frecuencia en revoluciones por segundo.
     actual_vel *= 6.283185;//2pi para convertir en radianes por segundo
     actual_vel *= QEIDirectionGet(encoder_handle->QEI_BASE);//Considerar sentido del giro
     return actual_vel;
}
//         * @brief  Recuperar posicion en radianes del encoder
 Encoder_pos  Encoder_ReadPosition(Encoder_Handle encoder_handle){
//     return WR32(encoder_handle->QEI_BASE | QEI_O_POS);
     //El factor de conversion utilizado es 2pi / numero de ticks por revolucion.
     Encoder_pos    actual_pos;
     actual_pos = QEIPositionGet(encoder_handle->QEI_BASE);
     actual_pos *= (6.283185/(ENCODER_CPR));//Multiplicar por el factor de conversion
     return actual_pos;
 }
//         * @brief  Desactivar el encoder para entrar en modo de bajo consumo
 void Encoder_Sleep(Encoder_Handle encoder_handle){
     QEIDisable(encoder_handle->QEI_BASE);
 }
//         * @brief  Reactiva el encoder para entrar en modo de funcionamiento
void Encoder_Wake(Encoder_Handle encoder_handle){
      QEIDisable(encoder_handle->QEI_BASE);
}
//         * @brief  Establecer funcion de interrupcion
 void Encoder_setCallback(Encoder_Handle encoder_handle,Encoder_Event encoder_event){
     QEIIntRegister(encoder_handle->QEI_BASE,encoder_event);
 }
//         * @brief  Inicia la monitorizacion de algun evento.
 void Encoder_EnableEvents(Encoder_Handle encoder_handle){
     QEIIntEnable(encoder_handle->QEI_BASE , QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX);
}
//        * @brief  Desactiva la monitorizacion de algun evento.
 void Encoder_DisableEvents(Encoder_Handle encoder_handle){
     QEIIntDisable(encoder_handle->QEI_BASE,QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX);
}
