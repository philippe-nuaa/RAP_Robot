/*
 * IMU.c
 *  Requiere IMU_hw_memmap.h
 *  Created on: 20/01/2018
 *      Author: josepablocb
 */

#include "IMU.h"

/*
 *  ============================================================================
    PROTOTIPOS DE FUNCIONES PRIVADAS
 *  ============================================================================
*/

//     * @brief  Permutar los bytes dentro del word
void _IMU_UTILS_permut_(short*);


/*
 *  ============================================================================
    DECLARACION DE FUNCIONES
 *  ============================================================================
*/

//     * @brief  Inicializacion de parametros por defecto para abrir el hardware
void IMU_Params_init(IMU_Params* imu_params){
    imu_params->Address                   = IMU_I2C_Default_Address;
    imu_params->Address2                  = IMU_I2C_MAGN_Default_Address;
    imu_params->I2C_index                 = Board_IMU;
}
//     * @brief  Configura y solicita el hardware para crear un manejador.
IMU_Handle IMU_open(IMU_State* imu_state,IMU_Params* imu_params){
        I2C_Params      i2c_params;
        I2C_Handle      i2c_handle;
        I2C_Transaction i2cTransaction;
        unsigned char Write[]={0x00};

        //Configuracion del I2C
        I2C_Params_init(&i2c_params);
        i2c_params.bitRate      = I2C_400kHz;
        i2c_params.transferMode = I2C_MODE_BLOCKING;
        i2c_handle = I2C_open(imu_params->I2C_index,&i2c_params);

        //Verificar el estado del hardware
        if(!i2c_handle){
            return NULL;
        }

        //Verificar el estado del sensor en el bus
        i2cTransaction.readBuf          = NULL;
        i2cTransaction.readCount        = NULL;
        i2cTransaction.slaveAddress     = imu_params->Address;
        i2cTransaction.writeBuf         = Write;
        i2cTransaction.writeCount       = sizeof(Write);
        if(!I2C_transfer(i2c_handle,&i2cTransaction)){
            I2C_close(i2c_handle);
            return NULL;
        }
        imu_state->Address                  = imu_params->Address;
        imu_state->Address2                 = imu_params->Address2;
        imu_state->i2c_handle               = i2c_handle;
        imu_state->i2c_index                = imu_params->I2C_index;
        imu_state->INTERRUPT_BASE_PORT      = NULL;
        imu_state->INTERRUPT_PIN            = NULL;
        imu_state->Magn_Calibration_fact[0] = NULL;
        imu_state->Magn_Calibration_fact[1] = NULL;
        imu_state->Magn_Calibration_fact[2] = NULL;
        return imu_state;
}
//         * @brief  Cierra el driver y libera el hardware
void IMU_close(IMU_Handle imu_handle){
    I2C_close(imu_handle->i2c_handle);
}
/*
 *  ============================================================================

    FUNCIONES ESPECIFICAS DEL DRIVER

 *  ============================================================================
*/
//     * @brief  Lectura de registro del sensor
IMU_REG_16b IMU_ReadRegister(IMU_Handle imu_handle,IMU_REG_ADDRESS imu_reg_address,ConfigurationType type){
    I2C_Transaction i2cTransaction;
    unsigned char Reg8b[2];
    i2cTransaction.readBuf      = Reg8b;
    i2cTransaction.readCount    = (type)?2:1;    //Si se recibe un solo byte o si se recibe un word
    i2cTransaction.writeBuf     = &imu_reg_address;
    i2cTransaction.writeCount   = 1;
    i2cTransaction.slaveAddress = imu_handle->Address;
    if(!I2C_transfer(imu_handle->i2c_handle,&i2cTransaction)){
        return IMU_READ_FAIL;//It means error!
    }
                                                //Si se recibe un solo byte o si se recibe un word
    return (type)?((Reg8b[0]<<8)+Reg8b[1]):(0x0000|Reg8b[0]);
}
//         * @brief  Escritura de registro del sensor
bool IMU_WriteRegister(IMU_Handle imu_handle,IMU_REG_8b *imu_reg_data,ConfigurationType type){
    I2C_Transaction i2cTransaction;
    i2cTransaction.readBuf      = NULL;
    i2cTransaction.readCount    = 0;
    i2cTransaction.writeBuf     = imu_reg_data;
    i2cTransaction.writeCount   = (type)?3:2;    //Si se recibe un solo byte o si se recibe un word
    i2cTransaction.slaveAddress = imu_handle->Address;
    if(!I2C_transfer(imu_handle->i2c_handle,&i2cTransaction)){
        return false;
    }
    return true;
}
//         * @brief  Inicializar los parametros de comportamiento del sensor.
void IMU_Sensor_Configuration_init(IMU_Sensor_Configuration* imu_configuration){
    imu_configuration->Address = IMU_USER_CTRL;
    imu_configuration->Configuration = 0x0000;
    imu_configuration->Type    = ByteConfiguration;
}
//         * @brief  Configura el comportamiento del sensor segun parametros.
bool IMU_Sensor_Configure(IMU_Handle imu_handle,IMU_Sensor_Configuration* imu_configuration){
    unsigned char Reg8b[3];
        Reg8b[0] = imu_configuration->Address;//Direccion del registro a configurar
    if(imu_configuration->Type){
        Reg8b[1] = imu_configuration->Configuration>>8;
        Reg8b[2] = imu_configuration->Configuration&0xFF;
    }else{
        Reg8b[1] = imu_configuration->Configuration&0xFF;
    }
    if(!IMU_WriteRegister(imu_handle,Reg8b,imu_configuration->Type)){
        return false;
    }
    return true;
}
//     * @brief  Inicializa el magnetometro
bool IMU_Sensor_Init_Magnetometer(IMU_Handle imu_handle){
    unsigned char   AddressBuffer;
    unsigned char   Reg8b[3];
    unsigned short  Reg_buffer;

    //Configurar Bypass Mode
    Reg8b[0] = IMU_INT_PIN_CFG;
    Reg8b[1] = 0xB2;//INT Enabled, Active Low, Push-Pull, I2C Bypass enabled
    if(!IMU_WriteRegister(imu_handle,Reg8b,ByteConfiguration)){goto ERROR;}

    //Configurar el magnetometro; Cambiamos i2c Slave
    AddressBuffer = imu_handle->Address;
    imu_handle->Address = imu_handle->Address2;
    //Enter Fuse ROM access mode
        Reg8b[0] = IMU_MAGN_CNTL;
        Reg8b[1] = 0x1F;
    //Enviar configuracion
    if(!IMU_WriteRegister(imu_handle,Reg8b,ByteConfiguration)){
        goto ERROR;
    }

    // Recuperar los datos de calibracion y validarlos
                //
                // Convertir los datos de bits a factores de calibracion
                //
    Reg_buffer = IMU_ReadRegister(imu_handle,IMU_MAGN_ASAX,ByteConfiguration);
    if(Reg_buffer == IMU_READ_FAIL){goto ERROR;}
                //
                imu_handle->Magn_Calibration_fact[0] = ((float)Reg_buffer - 128)/256. + 1.;
    Reg_buffer = IMU_ReadRegister(imu_handle,IMU_MAGN_ASAX,ByteConfiguration);
    if(Reg_buffer == IMU_READ_FAIL){goto ERROR;}
                //
                imu_handle->Magn_Calibration_fact[1] = ((float)Reg_buffer - 128)/256. + 1.;
    Reg_buffer = IMU_ReadRegister(imu_handle,IMU_MAGN_ASAX,ByteConfiguration);
    if(Reg_buffer == IMU_READ_FAIL){goto ERROR;}
                //
                imu_handle->Magn_Calibration_fact[2] = ((float)Reg_buffer - 128)/256. + 1.;


    //Regresar la direccion i2c a la normalidad
    imu_handle->Address = AddressBuffer;
    return true;
ERROR:
    imu_handle->Magn_Calibration_fact[0] = imu_handle->Magn_Calibration_fact[1] \
        = imu_handle->Magn_Calibration_fact[2] = 0;
    imu_handle->Address = AddressBuffer;
    return false;
}
//     * @brief  Lectura de registro del sensor
IMU_REG_16b IMU_ReadRegister_Magnetometer(IMU_Handle imu_handle,IMU_REG_ADDRESS imu_reg_address,ConfigurationType type){
    unsigned short RegBuffer;
    unsigned short AddressBuffer;
    //Almacenar temporalmente la direccion del acelerometro
    AddressBuffer = imu_handle->Address;
    //Recuperar la direccion del i2c del magnetometro
    imu_handle->Address = imu_handle->Address2;

    //Reutilizar  la funcion de lectura de registro
    RegBuffer = IMU_ReadRegister(imu_handle,imu_reg_address,type);

    //Regresar la direccion i2c a la normalidad
    imu_handle->Address = AddressBuffer;

    return RegBuffer;
}
//         * @brief  Escritura de registro del sensor
bool IMU_WriteRegister_Magnetometer(IMU_Handle imu_handle,IMU_REG_8b* imu_reg_data,ConfigurationType type){
        unsigned char   AddressBuffer;
        //Almacenar temporalmente la direccion del acelerometro
        AddressBuffer = imu_handle->Address;
        //Recuperar la direccion i2c del magnetometro
        imu_handle->Address = imu_handle->Address2;

        //Reutilizar la funcion de escritura a registro
        if(IMU_WriteRegister(imu_handle,imu_reg_data,type)){goto ERROR;}

            //Regresar la direccion i2c a la normalidad
            imu_handle->Address = AddressBuffer;
        return true;//Si se alcanza, entonces ha habido una transmision exitosa
    ERROR:
            imu_handle->Address = AddressBuffer;
        return false;//Si se alcanza, entonces ha habido una transmision fallida
}
//     * @brief  Configura el comportamiento del sensor magnetometro segun parametros.
bool IMU_Sensor_Configure_Magnetometer(IMU_Handle imu_handle,IMU_Sensor_Configuration* imu_configuration){
    unsigned char AddressBuffer;
    //Almacenamos la direccion i2c del acelerometro
    AddressBuffer  = imu_handle->Address;
    //Configuramos la direccion del magnetometro
    imu_handle->Address = imu_handle->Address2;

    //Reutilizamos la funcion Configure
    if(IMU_Sensor_Configure(imu_handle,imu_configuration)){
        goto DONE;
    }

    //Recuperamos la direccion del acelerometro
    imu_handle->Address = AddressBuffer;
    return false;//Si se alcanza es porque hubo un error
DONE:
    //Recuperamos la direccion del acelerometro
    imu_handle->Address = AddressBuffer;
    return true;//Si se alcanza => proceso exitoso
}
//         * @brief  Lectura de unidades del acelerometro
IMU_axis IMU_ReadAccelerometer(IMU_Handle imu_handle){
    IMU_axis axis;
    axis.isValid = true;
    short Buffer;
    //
    //  Factor de conversion 2g
    //
    float fact = g_fIMUAccelFactors[IMU_Ac_Fact];
    //Recuperar el valor desde el sensor
        //Validar el valor antes de tratarlo
    Buffer = IMU_ReadRegister(imu_handle,IMU_ACCEL_XOUT_H,WordConfiguration);
    if(Buffer==IMU_READ_FAIL){  axis.isValid=false; }
        axis.X_axis = (float)Buffer;
        axis.X_axis *= fact;
    Buffer = IMU_ReadRegister(imu_handle,IMU_ACCEL_YOUT_H,WordConfiguration);
    if(Buffer==IMU_READ_FAIL){  axis.isValid=false; }
        axis.Y_axis = (float)Buffer;
        axis.Y_axis *= fact;
    Buffer = IMU_ReadRegister(imu_handle,IMU_ACCEL_ZOUT_H,WordConfiguration);
    if(Buffer==IMU_READ_FAIL){  axis.isValid=false;   }
        axis.Z_axis = (float)Buffer;
        axis.Z_axis *= fact;
    return axis;
}
//         * @brief  Lectura de unidades del giroscópio
IMU_axis IMU_ReadGyroscope(IMU_Handle imu_handle){
    IMU_axis axis;
    axis.isValid = true;
    short Buffer;
    float fact = g_fIMUGyroFactors[IMU_Gy_Fact];
    Buffer = IMU_ReadRegister(imu_handle,IMU_GYRO_XOUT_H,WordConfiguration);
    if(Buffer==IMU_READ_FAIL){  axis.isValid=false; }
        axis.X_axis = (float)Buffer;
        axis.X_axis *= fact;
    Buffer = IMU_ReadRegister(imu_handle,IMU_GYRO_XOUT_H,WordConfiguration);
    if(Buffer==IMU_READ_FAIL){  axis.isValid=false; }
        axis.Y_axis = (float)Buffer;
        axis.Y_axis *= fact;
    Buffer = IMU_ReadRegister(imu_handle,IMU_GYRO_XOUT_H,WordConfiguration);
    if(Buffer==IMU_READ_FAIL){  axis.isValid=false;   }
        axis.Z_axis = (float)Buffer;
        axis.Z_axis *= fact;
    return axis;
}
//         * @brief  Lectura de unidades del magnetometro
IMU_axis IMU_ReadMagnetometer(IMU_Handle imu_handle){
        IMU_axis axis;
        axis.isValid = true;
        unsigned char AddressBuffer;
        short Buffer;

        //Configurar el magnetometro
        //Cambiamos la direccion i2c Slave
        AddressBuffer = imu_handle->Address;
        imu_handle->Address = imu_handle->Address2;
        //Solicitamos los registros
        Buffer = IMU_ReadRegister(imu_handle,IMU_MAGN_HXL,WordConfiguration);
            if(Buffer == IMU_READ_FAIL){axis.isValid=false;}
                _IMU_UTILS_permut_(&Buffer);
                axis.X_axis = ((float)Buffer);
                axis.X_axis *= CONVERT_TO_TESLA;
                axis.X_axis *= imu_handle->Magn_Calibration_fact[0];
        Buffer = IMU_ReadRegister(imu_handle,IMU_MAGN_HYL,WordConfiguration);
            if(Buffer == IMU_READ_FAIL){axis.isValid=false;}
                _IMU_UTILS_permut_(&Buffer);
                axis.Y_axis = ((float)Buffer);
                axis.Y_axis *= CONVERT_TO_TESLA;
                axis.Y_axis *= imu_handle->Magn_Calibration_fact[1];
        Buffer = IMU_ReadRegister(imu_handle,IMU_MAGN_HZL,WordConfiguration);
            if(Buffer == IMU_READ_FAIL){axis.isValid=false;}
                _IMU_UTILS_permut_(&Buffer);
                axis.Z_axis = ((float)Buffer);
                axis.Z_axis *= CONVERT_TO_TESLA;
                axis.Z_axis *= imu_handle->Magn_Calibration_fact[2];
        //Regresar la direccion i2c a la normalidad
        imu_handle->Address = AddressBuffer;
        return axis;
}
//         * @brief  Lectura de temperatura
IMU_Temp IMU_ReadTemperature(IMU_Handle imu_handle){
    IMU_Temp temp;
    short Buffer;
    Buffer = IMU_ReadRegister(imu_handle,IMU_TEMP_OUT_H,WordConfiguration);
    if(Buffer == IMU_READ_FAIL){
        return Buffer;
    }
    //RoomTemp_Offset
    temp = (float)Buffer;
    //Temp_Sensitivity
    temp *= .003;
    //21 Deg.
    temp += 21;
    return temp;
}
//         * @brief  Forzar al sensor entrar en modo de bajo consumo
void IMU_Sleep(IMU_Handle imu_handle){
//    configuration.Address           = IMU_PWR_MGMT_1;
//    configuration.Type              = Configuration;
//    configuration.Configuration     = 0x20C0;//Cycle - 1
//    IMU_Sensor_Configure(imu,&configuration);
}
//         * @brief  Registrar evento de interrupción
void IMU_setCallback(IMU_Handle imu_handle,IMU_CallbackEvent CallbackEvent,uint32_t PIN){
    if(!imu_handle || !CallbackEvent){return;}
    imu_handle->INTERRUPT_PIN = PIN;
    GPIO_setCallback(imu_handle->INTERRUPT_PIN, CallbackEvent);
}
//       * @brief  Activar los eventos
void IMU_EnableEvents(IMU_Handle imu_handle){
    GPIO_enableInt(imu_handle->INTERRUPT_PIN);
}
//       * @brief  Activar los eventos
void IMU_DisableEvents(IMU_Handle imu_handle){
    GPIO_disableInt(imu_handle->INTERRUPT_PIN);

}
//       * @brief  Permutar los bytes dentro del word
void _IMU_UTILS_permut_(short* value){
    *value = ((*value>>8)&0xFF) + (0xFF&(*value<<8));
}
