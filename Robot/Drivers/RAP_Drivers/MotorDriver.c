/*
 * MotorDriver.c
 *
 *  Created on: 15/01/2018
 *      Author: josepablocb
 */
#include "MotorDriver.h"

/*
    HACE FALTA PROGRAMAR LA PARTE ANALOGICA
*/


//
//  Macros para manipulacion de registros
//

/*
 *  ============================================================================
    FUNCIONES ESPECIFICAS DEL DRIVER
 *  ============================================================================
*/

//  * @brief  Inicializar parametros de configuracion del hardware.
    void MotorDriver_Params_init(MotorDriver_Params* params){
        //INICIALIZACION POR DEFECTO DE TODOS LOS PARAMETROS
        params->EN_Index    = MotorDriver_EN_Default;
        params->LD1_Index   = MotorDriver_LD1_Default;
        params->RD1_Index   = MotorDriver_RD1_Default;
        params->ML_F_Index  = MotorDriver_ML_F_Default;
        params->ML_R_Index  = MotorDriver_ML_R_Default;
        params->ML_SF_Index = MotorDriver_ML_SF_Default;
        params->MR_F_Index  = MotorDriver_MR_F_Default;
        params->MR_R_Index  = MotorDriver_MR_R_Default;
        params->MR_SF_Index = MotorDriver_MR_SF_Default;
        params->PWM_period  = MotorDriver_PWMPeriod_Default;
    }

//  * @brief  Configuracion del hardware y reservación del mismo
    MotorDriver_Handle   MotorDriver_open(MotorDriver_state* state, MotorDriver_Params* params){
        PWM_Params      pwm_params;
        PWM_Handle      pwm_temp_handle;

        state->EN_Index     = params->EN_Index;
        state->LD1_Index    = params->LD1_Index;
        state->RD1_Index    = params->RD1_Index;
        state->ML_SF_Index  = params->ML_SF_Index;
        state->MR_SF_Index  = params->MR_SF_Index;

    /*
    *
    * CONFIGURACION DE PINES DIGITALES
    *
    * */

        GPIO_setConfig(state->EN_Index   ,GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW);
        GPIO_setConfig(state->LD1_Index  ,GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_HIGH);//Activado en bajo
        GPIO_setConfig(state->RD1_Index  ,GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_HIGH);//Activado en bajo
        GPIO_setConfig(state->ML_SF_Index,GPIO_CFG_INPUT);
        GPIO_setConfig(state->MR_SF_Index,GPIO_CFG_INPUT);
        //
        // Desactivar el puente H
        //
        GPIO_write(state->EN_Index, MotorDriver_EN_Off);
        GPIO_write(state->LD1_Index,MotorDriver_D1_Off);
        GPIO_write(state->RD1_Index,MotorDriver_D1_Off);

   /*
    *
    * CONFIGURACION DE PINES ANALOGICOS
    *
    * */
        SysCtlPeripheralEnable(MotorDriver_ML_FB_ADPeriph);
        SysCtlPeripheralEnable(MotorDriver_MR_FB_ADPeriph);

        while(!SysCtlPeripheralReady(MotorDriver_ML_FB_ADPeriph)){}
        while(!SysCtlPeripheralReady(MotorDriver_MR_FB_ADPeriph)){}

        GPIOPinTypeADC(MotorDriver_ML_FB_PinBase,MotorDriver_ML_FB_Pin);
        GPIOPinTypeADC(MotorDriver_MR_FB_PinBase,MotorDriver_MR_FB_Pin);

        ADCReferenceSet(MotorDriver_ML_FB_ADBase, ADC_REF_EXT_3V);
        ADCReferenceSet(MotorDriver_MR_FB_ADBase, ADC_REF_EXT_3V);

        ADCSequenceConfigure(MotorDriver_ML_FB_ADBase,0
                             ,ADC_TRIGGER_PROCESSOR,0); //Sequence 0
        ADCSequenceConfigure(MotorDriver_MR_FB_ADBase,1
                             ,ADC_TRIGGER_PROCESSOR,0); //Sequence 1

        ADCSequenceStepConfigure(MotorDriver_ML_FB_ADBase, 0, 0, \
                                 MotorDriver_ML_FB_ADch | ADC_CTL_END);
        ADCSequenceStepConfigure(MotorDriver_MR_FB_ADBase, 1, 0, \
                                 MotorDriver_MR_FB_ADch | ADC_CTL_END);
        //
        // Enable the sequence but do not start it yet.
        //
        ADCSequenceEnable(MotorDriver_ML_FB_ADBase, 0);
        ADCSequenceEnable(MotorDriver_MR_FB_ADBase, 1);
    /*
    *
    * CONFIGURACION PWM
    *
    * */

        PWM_Params_init(&pwm_params);
        pwm_params.period   = params->PWM_period;
        pwm_params.dutyMode = PWM_DUTY_SCALAR;//0 - 100 = 0 - 65535

        state->PWM_fact = MotorDriver_PWMSCALAR_100/100;

        pwm_temp_handle = PWM_open(params->ML_F_Index,&pwm_params);
        if(pwm_temp_handle){
            state->ML_F_handle = pwm_temp_handle;
        }else{
            return NULL;
        }
        pwm_temp_handle = PWM_open(params->ML_R_Index,&pwm_params);
        if(pwm_temp_handle){
            state->ML_R_handle = pwm_temp_handle;
        }else{
            return NULL;
        }
        pwm_temp_handle = PWM_open(params->MR_F_Index,&pwm_params);
        if(pwm_temp_handle){
            state->MR_F_handle = pwm_temp_handle;
        }else{
            return NULL;
        }
        pwm_temp_handle = PWM_open(params->MR_R_Index,&pwm_params);
        if(pwm_temp_handle){
            state->MR_R_handle = pwm_temp_handle;
        }else{
            return NULL;
        }

        //
        // Desactivar las salidas PWM
        //
        PWM_setDuty(state->ML_F_handle,MotorDriver_PWMOff);
        PWM_setDuty(state->ML_R_handle,MotorDriver_PWMOff);
        PWM_setDuty(state->MR_F_handle,MotorDriver_PWMOff);
        PWM_setDuty(state->MR_R_handle,MotorDriver_PWMOff);
        return state;
    }

//  * @brief  Liberamos el hardware para otro manejador
    void MotorDriver_close(MotorDriver_Handle handle){
        PWM_close(handle->ML_F_handle);
        PWM_close(handle->ML_R_handle);
        PWM_close(handle->MR_F_handle);
        PWM_close(handle->MR_R_handle);
    }

/*
 *  ============================================================================
    FUNCIONES ESPECIFICAS DEL DRIVER
 *  ============================================================================
*/

//  * @brief  Obtener la corriente aproximada de cada motor
    MotorDriver_Current MotorDriver_Readcurrent(MotorDriver_Handle handle){
        MotorDriver_Current current;
        unsigned int    AD_buffer;
        current.MotorLeft  = 0.0;
        current.MotorRight = 0.0;

        ADCProcessorTrigger(MotorDriver_ML_FB_ADBase, 0);
            //Esperar a
        while(ADCBusy(MotorDriver_ML_FB_ADBase)){}
        ADCSequenceDataGet(MotorDriver_ML_FB_ADBase,0,&AD_buffer);
        current.MotorLeft = (float)AD_buffer;

        ADCProcessorTrigger(MotorDriver_MR_FB_ADBase, 1);
            //Esperar a
        while(ADCBusy(MotorDriver_MR_FB_ADBase)){}
        ADCSequenceDataGet(MotorDriver_MR_FB_ADBase,1,&AD_buffer);
        current.MotorRight = (float)AD_buffer;

        current.MotorLeft *= 1250;
        current.MotorLeft /= MotorDriver_ADFACT;

        current.MotorRight*= 1250;
        current.MotorRight/= MotorDriver_ADFACT;
        return current;
    }

//  * @brief  Activar el puente H.
    void MotorDriver_Start(MotorDriver_Handle handle){
            //Activar EN y D1
        GPIO_write(handle->EN_Index ,MotorDriver_EN_On);
        GPIO_write(handle->LD1_Index,MotorDriver_D1_On);
        GPIO_write(handle->RD1_Index,MotorDriver_D1_On);
    }

//  * @brief  Desactivar el puente H.
    void MotorDriver_Stop(MotorDriver_Handle handle){
        GPIO_write(handle->EN_Index ,MotorDriver_EN_Off);
        GPIO_write(handle->LD1_Index,MotorDriver_D1_Off);
        GPIO_write(handle->RD1_Index,MotorDriver_D1_Off);
    }

//  * @brief  Forzar la sincronizacion de señales PWM
    void MotorDriver_ForceSync(MotorDriver_Handle handle){
        HWREGB(MotorDriver_Base|PWM_O_SYNC) = 0xA;
    }

//  * @brief  Controlar el motor izquierdo
    void MotorDriver_Left(MotorDriver_Handle handle, MotorDriver_DutyCycle DutyCycle){
        if(DutyCycle>0){
            PWM_setDuty(handle->ML_F_handle, MotorDriver_PWMOff);
            if(DutyCycle < 100)
                PWM_setDuty(handle->ML_R_handle, (int) ( DutyCycle*handle->PWM_fact ));
            else
                PWM_setDuty(handle->ML_R_handle, MotorDriver_PWMSCALAR_100 );
        }else{
            DutyCycle*=-1;
            PWM_setDuty(handle->ML_R_handle, MotorDriver_PWMOff);
            if(DutyCycle < 100)
                PWM_setDuty(handle->ML_F_handle, (int) ( DutyCycle*handle->PWM_fact ));
            else
                PWM_setDuty(handle->ML_F_handle, MotorDriver_PWMSCALAR_100 );
        }
    }

//  * @brief  Controlar el motor derecho
    void MotorDriver_Right(MotorDriver_Handle handle, MotorDriver_DutyCycle DutyCycle){
        if(DutyCycle<0){
            DutyCycle*=-1;
            PWM_setDuty(handle->MR_F_handle, MotorDriver_PWMOff);
            if(DutyCycle < 100)
                PWM_setDuty(handle->MR_R_handle, (int) ( DutyCycle*handle->PWM_fact ));
            else
                PWM_setDuty(handle->MR_R_handle, MotorDriver_PWMSCALAR_100);
        }else{
            PWM_setDuty(handle->MR_R_handle, MotorDriver_PWMOff);
            if(DutyCycle < 100)
                PWM_setDuty(handle->MR_F_handle, (int) ( DutyCycle*handle->PWM_fact ));
            else
                PWM_setDuty(handle->MR_F_handle, MotorDriver_PWMSCALAR_100);
        }
    }
