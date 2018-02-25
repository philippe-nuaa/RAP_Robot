/** ============================================================================
 *  @file       IMU.h
 *
 *  @brief      IMU sensor driver interface
 *
 *  Created on: 20/01/2018
 *      Author: Josepablo Cruz Baas
 *
 *  El encabezado debe ser incluido en una aplicacion como sigue:
 *  @code
 *  #include "IMU.h"
 *  @endcode
 *
 *  # Operacion #
 *  El driver IMU permite operar al sensor de inercia de multiples ejes MPU 9250,
 *  conectado a un periferico I2C en transferencia a bloques. Lo que significa
 *  que durante la transferencia de datos, la ejecución de la tarea es bloqueada
 *  hasta la finalizacion de la transferencia.
 *  Las APIs en este driver sirven como una interfaz para una aplicacion tipica
 *  TI-RTOS.
 *  Se requiere del fichero IMU_hw_memmap.h
 *
 *  ## Abriendo el dirver #
 *  @code
 *  IMU_State      state;
 *  IMU_Handle     handle;
 *  IMU_Params     params;
 *
 *  IMU_Params_init(&params);
 *  params.I2C_index = Board_I2C;
 *  handle = IMU_open(&state,&params);
 *  if(!handle){
 *      System_printf("IMU no ha sido abierto");
 *  }
 *  @endcode
 *
 *  ## Configurando los parametros del sensor #
 *  @code
 *  IMU_Sensor_Configuration        sensor_config;
 *  IMU_Sensor_Configuration_init(&sensor_config);
 *  ret = IMU_Configure(handle,&sensor_config)
 *
 *  if(!ret){
 *      System_printf("IMU no ha sido configurado");
 *  }
 *  @endcode
 *
 *  ## Lectura de acelerómetro #
 *  @code
 *  IMU_axis    Axis;
 *  Axis = IMU_ReadAccelerometer(imu_handle);
 *  if(Axis.isValid){
 *      System_printf("Error de lectura de ejes");
 *  }
 *  @endcode
 *
 *  ## Lectura de giroscópio #
 *  @code
 *  IMU_axis    Axis;
 *  Axis = IMU_ReadGyroscope(imu_handle);
 *  if(Axis.isValid){
 *      System_printf("Error de lectura de ejes");
 *  }
 *  @endcode
 *
 *  ## Lectura de magnetómetro #
 *  @code
 *  IMU_axis    Axis;
 *  Axis = IMU_ReadMagnetometer(imu_handle);
 *  if(Axis.isValid){
 *      System_printf("Error de lectura de ejes");
 *  }
 *  @endcode
 *
 *  ## Lectura de temperatura #
 *  @code
 *  IMU_temp    Temperature;
 *  Temperature = IMU_ReadTemperature(imu_handle);
 *  if(Temperature == IMU_READ_FAIL){
 *      System_printf("Error de lectura de temperatura");
 *  }
 *  @endcode
 *
 *  ## Asignacion de eventos #
 *  @code
 *  IMU_setCallback(imu_handle,SensorEvent,BOARD_IMU_INT_PIN);
 *  IMU_EnableEvents(imu_handle);
 *  @endcode
 *
 *  ## Configuracion de Magnetometro #
 *  @code
 *  if(!IMU_Sensor_Init_Magnetometer(imu_handle)){
 *          System_printf("Error al inicializar el magnetometro");
 *  }else{
 *      System_printf("Magnetometro activo!");
 *      configuration.Address       =   REGISTER_ADDRESS;
 *      configuration.Configuration =   REGISTER_BYTE_OR_WORD;
 *      configuration.Type          =   ByteConfiguration; -OR- WordConfiguration;
 *      if(IMU_Sensor_Configure_Magnetometer(imu,&configuration)){
 *          System_printf("Exitoso");
 *      }else{
 *          System_printf("Error al configurar el magnetometro");
 *      }
 *
 *  }
 *  @endcode
 *
 *  En este fichero de encabezado se definen:
 *      # Constantes para la manipulacion del sensor por medio de un periferico I2C.
 *      # Constantes para la manipulacion de parametros y configuraciones de esta API.
 *      # Funciones API genericas para el manejo del sensor.
 *      # Funciones API especificas para el manejo del sensor.
 *
 *  ============================================================================
 *  CONSTANTES PARA EL DRIVER
 *  En este fichero se han definido 3 tipos de constantes.
 *      1.- Comandos I2C para el sensor.
 *      2.- Direcciones de registros de memoria.
 *      3.- Configuraciones tipicas para los registros de configuración.
 *  Tanto los comandos del sensor como las direcciones de registros
 *  son constantes privadas que corresponden al manejo del driver I2C.
 *  Las configuraciones tipicas para los registros del sensor, son valores
 *  binarios que reflejan un comportamiento del sensor. El programador utiliza
 *  estas constantes como parametros de configuracion del dispositivo ya
 *  abierto y funcionando.
 *  No todas las configuraciones son posibles, leer la hoja tecnica para
 *  mayor informacion.
 *  ============================================================================
 *  ESTRUCTURAS GENERICAS DEL DRIVER
 *      #IMU_Handle        Manejador del sensor IMU.
 *      #IMU_Params        Parametros de inicializacion del hardware.
 *      #IMU_State         Buffer de estados del manejador.
 *  ============================================================================
 *  ESTRUCTURAS ESPECIFICAS DEL DRIVER
 *      #IMU_Sensor_Configuration   Parametros de configuracion del
 *                                      comportamiento del sensor IMU.
 *      #IMU_axis                   Información en flotante de cada eje.
 *      #IMU_temp                   Temperatura en flotante reconocida por el sensor.
 *      #IMU_REG_ADDRESS            Registro de 8 bits renombrado
 *      #IMU_REG_8b                 Registro de 8 bits renombrado
 *      #IMU_REG_16b                Registro de 16 bits renombrado
 *      #IMU_Temp                   Flotante renombrado
 *      #*IMU_CallbackEvent         Formato/Plantilla de funcion compatible como evento CallBack
 *      #ConfigurationType          Bandera/Selector de tipo de configuracion para el sensor, define el tamaño del registro a escribir
 *      #IMU_Sensor_Configuration   Parametros de configuracion del comportamiento del sensor compatible con el magnetometro
 *      #IMU_axis                   Valores en formato flotante de los ejes leidos por el sensor
 *      #g_fIMUAccelFactors         Tabla de factores de conversion para el acelerometro
 *      #g_fIMUGyroFactors          Tabla de factores de conversion para el giroscopio
 *  ============================================================================
 *  FUNCIONES GENERICAS DEL DRIVER
 *      #IMU_Params_init   Configura los parametros por defecto del hardware.
 *      #IMU_init          Macro, Inicializa el driver sin manipular algun
 *                              registro del hardware.
 *      #IMU_open          Inicializa el hardware.
 *      #IMU_close         Cierra el hardware.
 *  ============================================================================
 *  FUNCIONES ESPECIFICAS DEL DRIVER
 *      #IMU_Sensor_Configuration_init      Configura los parametros por defecto del sensor.
 *      #IMU_Sensor_Configure               Configura el sensor segun los parametros asignados.
 *      #IMU_Sensor_Configure_Magnetometer  Configura el magnetometro dentro del sensor IMU.
 *      #IMU_ReadRegister                   Realiza una lectura del registro especificado.
 *      #IMU_WriteRegister                  Realiza una escritura en el registro especificado.
 *      #IMU_ReadRegister_Magnetometer      Realiza una lectura de registros del magnetometro.
 *      #IMU_WriteRegister_Magnetometer     Realiza una escritura de registros del magnetometro.
 *      #IMU_ReadAccelerometer              Realiza una lectura del acelerometro.
 *      #IMU_ReadGyroscope                  Realiza una lectura del giroscópio.
 *      #IMU_ReadMagnetometer               Realiza una lectura del magnetómetro.
 *      #IMU_ReadTemperature                Realiza una lectura de temperatura.
 *      #IMU_Reset                          Realiza un reset del sistema del sensor.
 *      #IMU_Sleep                          Fuerza el modo de bajo consumo del sensor (IMU)
 *      #IMU_setCallback                    Asigna un evento de interrupcion al pin INT del IMU.
 *      #IMU_EnableEvents                   Activa los eventos de interrupcion
 *      #IMU_DisableEvents                  Desactiva los eventos de interrupcion
 *  ============================================================================
 *  MACROS DEL DRIVER
 *      #IMU_init          Inicializa el driver sin manipular algun registro
 *                              del hardware.
 *  ============================================================================
 */

#ifndef RAP_DRIVERS_IMU_H_
#define RAP_DRIVERS_IMU_H_

#include "../../RAP_Robot_Board_Hw/Robot_Board.h"

#include <stdint.h>
#include <stdbool.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/GPIO.h>
#include <driverlib/gpio.h>
#include "IMU_hw_memmap.h"

/*
 *  ============================================================================
    COMANDOS I2C PARA EL SENSOR
 *  ============================================================================
*/
    #define IMU_I2C_Default_Address                 0x68
    #define IMU_I2C_MAGN_Default_Address            0x0C
    #define Board_IMU                               Board_I2C1
/*
 *  ============================================================================
    
    MACROS Y CONSTANTES DEL DRIVER
    
 *  ============================================================================
*/
    //*****************************************************************************
    //
    // Converting sensor data to tesla (0.15 uT per LSB 16 bit (0.6 for 14bit))
    //
    //*****************************************************************************
    #define CONVERT_TO_TESLA        0.00000015  // for 16 bit resolution
    #define IMU_Gy_Fact             0           //
    #define IMU_Ac_Fact             3           //
    // Inicializacion del driver, Generic API Macro.
    #define IMU_READ_FAIL          0x7FFF
    #define IMU_init()             Board_initI2C();

    #define IMU_INTERRUPT_EVENT     GPIO_LOW_LEVEL

    // Funcion sin parametros y sin retorno para evento de interrupcion.
/*
 *  ============================================================================

    ESTRUCTURAS ESPECIFICAS DEL DRIVER
    
 *  ============================================================================
*/
    typedef unsigned char   IMU_REG_ADDRESS;    //uint8_t
    typedef unsigned char   IMU_REG_8b;
    typedef unsigned short  IMU_REG_16b;
    typedef float           IMU_Temp;
    
    typedef void (*IMU_CallbackEvent)(unsigned int index);

    //Tipo de configuracion para el sensor, define el tamaño del registro
    typedef enum ConfigurationType{
        ByteConfiguration=0,
        WordConfiguration,
    }ConfigurationType;
    
    //Parametros de configuracion para el sensor
    typedef struct IMU_Sensor_Configuration{
        IMU_REG_ADDRESS         Address;
        IMU_REG_16b             Configuration;
        ConfigurationType       Type;
    }IMU_Sensor_Configuration;

    typedef struct IMU_axis{
        float   X_axis;
        float   Y_axis;
        float   Z_axis;
        bool    isValid;
    }IMU_axis;

    //*****************************************************************************
    //
    // The factors used to convert the acceleration readings from the MPU9250 into
    // floating point values in meters per second squared.
    //
    // Values are obtained by taking the g conversion factors from the data sheet
    // and multiplying by 9.81 (1 g = 9.81 m/s^2).
    //
    //*****************************************************************************
    static const float g_fIMUAccelFactors[] =
    {
        0.0005987548,                           // Range = +/- 2 g (16384 lsb/g)
        0.0011970964,                           // Range = +/- 4 g (8192 lsb/g)
        0.0023941928,                           // Range = +/- 8 g (4096 lsb/g)
        0.0047883855                            // Range = +/- 16 g (2048 lsb/g)
    };

    //*****************************************************************************
    //
    // The factors used to convert the acceleration readings from the MPU9150 into
    // floating point values in radians per second.
    //
    // Values are obtained by taking the degree per second conversion factors
    // from the data sheet and then converting to radians per sec (1 degree =
    // 0.0174532925 radians).
    //
    //*****************************************************************************
    static const float g_fIMUGyroFactors[] =
    {
        1.3323124e-4,                           // Range = +/- 250 dps (131.0)
        2.6646248e-4,                           // Range = +/- 500 dps (65.5)
        5.3211258e-4,                           // Range = +/- 1000 dps (32.8)
        0.0010642252                            // Range = +/- 2000 dps (16.4)
    };
/*
 *  ============================================================================
    
    ESTRUCTURAS GENERICAS DEL DRIVER
    
 *  ============================================================================
*/
    //Manejador del sensor
    typedef struct IMU_State{
        I2C_Handle      i2c_handle;
        unsigned int    i2c_index;
        unsigned int    Address;
        unsigned char   Address2;
        float           Magn_Calibration_fact[3];
        unsigned int    INTERRUPT_BASE_PORT;
        unsigned char   INTERRUPT_PIN;
    }IMU_State;
    typedef struct IMU_State   *IMU_Handle;

    //Parametros para configuracion de comunicacion del sensor
    typedef struct IMU_Params{

        unsigned char   Address;
        unsigned char   Address2;
        unsigned int    I2C_index;
    }IMU_Params;

/*
 *  ============================================================================
    
    FUNCIONES GENERICAS DEL DRIVER
    
 *  ============================================================================
*/
    //Inicializacion de parametros por defecto para abrir el hardware.
    /*!
         * @brief  Inicializacion de parametros por defecto para abrir el hardware
         *
         *  Funcion para crear parametros de inicializacion de manejador
         *  como parametros por defecto.
         *
         *  @param  IMU_Params*      Apuntador a los parametros de inicializacion.
         *
        */
    extern void         IMU_Params_init(IMU_Params*);
    /*!
         * @brief  Configura y solicita el hardware para crear un manejador.
         *
         *  Funcion para configurar el hardware y solicitar el manejador del mismo.
         *
         *  @param  IMU_State*       Apuntador al registro estado del hardware.
         *
         *  @param  IMU_Params*      Apuntador a parametros de inicializacion.
         *
         *  @return IMU_Handle si ha sido exitosa la operacion, NULL si ha
         *  habido algun error o ya ha sido solicitado el hardware.
         *
        */
    extern IMU_Handle   IMU_open(IMU_State*,IMU_Params*);
    /*!
         * @brief  Cierra el driver y libera el hardware
         *
         *  Funcion para cerrar el driver y liberar el hardware.
         *
         *  @param  IMU_Handle       Manejador del sensor.
        */
    extern void         IMU_close(IMU_Handle);
/*
 *  ============================================================================
    
    FUNCIONES ESPECIFICAS DEL DRIVER
    
 *  ============================================================================
*/
    /*!
         * @brief  Lectura de registro del sensor
         *
         *  Funcion para recuperar un registro de 16 bits del sensor
         *  especificado por la direccion.
         *
         *  @param  IMU_Handle       Manejador del sensor.
         *
         *  @param  IMU_REG_ADDRESS  Comando de acceso con direccion.
         *
         *  @param  ConfigurationType Tipo o tamanio de buffer, 8 o 16 bits.
         *
         *  @return IMU_REG, registro de 16 bits diferente de 0 si ha sido exitoso,
         *                        0 si hubo error.
         *
         */
    extern IMU_REG_16b  IMU_ReadRegister(IMU_Handle,IMU_REG_ADDRESS,ConfigurationType);
    /*!
         * @brief  Escritura de registro del sensor
         *
         *  Funcion para realizar una escritura en algun registro del sensor
         *
         *  @param  IMU_Handle       Manejador del sensor.
         *
         *  @param  IMU_REG*         Buffer de hasta 3 bytes que enviar, donde el
         *                              primer byte es la direccion del registro del sensor
         *
         *  @param  ConfigurationType Tipo o tamanio de buffer, 8 o 16 bits.
         *
         *  @return True si ha sido exitoso, False si no.
         *
        */
    extern bool         IMU_WriteRegister(IMU_Handle,IMU_REG_8b*,ConfigurationType);
    /*!
         * @brief  Inicializar los parametros de comportamiento del sensor.
         *
         *  Funcion para configurar los parametros de comportamiento del sensora por defecto.
         *
         *  @param  IMU_Sensor_Configuration       Apuntador al registro de configuracion.
         *
         */
    extern void         IMU_Sensor_Configuration_init(IMU_Sensor_Configuration*);
    /*!
         * @brief  Configura el comportamiento del sensor segun parametros.
         *
         *  Funcion para configurar el sensor con los parametros solicitados.
         *
         *  @param  IMU_Handle                 Manejador del sensor.
         *
         *  @param  IMU_Sensor_Configuration*  Apuntador al registro de configuracion.
         *
         *  @return True si la operacion ha sido exitosa, False, si ha habido un error de transferencia.
         *
        */
    extern bool         IMU_Sensor_Configure(IMU_Handle,IMU_Sensor_Configuration*);
    /*!
         * @brief  Inicializar el magnetometro
         *
         *  Funcion para activar y configurar el magnetometro
         *
         *  @param  IMU_Handle         Manejador del sensor.
         *
         */
    extern bool         IMU_Sensor_Init_Magnetometer(IMU_Handle);
    /*!
         * @brief  Lectura de registro del sensor magnetometro
         *
         *  Funcion para recuperar un registro de 16 bits del sensor
         *  especificado por la direccion.
         *
         *  @param  IMU_Handle       Manejador del sensor.
         *
         *  @param  IMU_REG_ADDRESS  Comando de acceso con direccion.
         *
         *  @param  ConfigurationType Tipo o tamanio de buffer, 8 o 16 bits.
         *
         *  @return IMU_REG, registro de 16 bits diferente de 0 si ha sido exitoso,
         *                        0 si hubo error.
         *
         */
    extern IMU_REG_16b  IMU_ReadRegister_Magnetometer(IMU_Handle,IMU_REG_ADDRESS,ConfigurationType);
    /*!
         * @brief  Escritura de registro del sensor magnetometro
         *
         *  Funcion para realizar una escritura en algun registro del sensor magnetometro
         *
         *  @param  IMU_Handle       Manejador del sensor.
         *
         *  @param  IMU_REG*         Buffer de hasta 3 bytes que enviar, donde el
         *                              primer byte es la direccion del registro del sensor
         *
         *  @param  ConfigurationType Tipo o tamanio de buffer, 8 o 16 bits.
         *
         *  @return True si ha sido exitoso, False si no.
         *
         */
    extern bool         IMU_WriteRegister_Magnetometer(IMU_Handle,IMU_REG_8b*,ConfigurationType);
    /*!
         * @brief  Configura el comportamiento del sensor magnetometro segun parametros.
         *
         *  Funcion para configurar el magnetometro con los parametros solicitados.
         *
         *  @param  IMU_Handle                 Manejador del sensor.
         *
         *  @param  IMU_Sensor_Configuration*  Apuntador al registro de configuracion.
         *
         *  @return True si la operacion ha sido exitosa, False, si ha habido un error de transferencia.
         *
        */
    extern bool         IMU_Sensor_Configure_Magnetometer(IMU_Handle,IMU_Sensor_Configuration*);
    /*!
         * @brief  Lectura de unidades del acelerometro
         *
         *  Funcion para recuperar el registro de temperatura especificado.
         *
         *  @param  IMU_Handle         Manejador del sensor.
         *
         *  @return IMU_axis           Unidades de cada eje en formato flotante.
         *
        */
    extern  IMU_axis    IMU_ReadAccelerometer(IMU_Handle);
    /*!
         * @brief  Lectura de unidades del giroscópio
         *
         *  Funcion para recuperar el registro de temperatura especificado.
         *
         *  @param  IMU_Handle         Manejador del sensor.
         *
         *  @return IMU_axis           Unidades de cada eje en formato flotante.
         *
        */
    extern  IMU_axis    IMU_ReadGyroscope(IMU_Handle);
    /*!
         * @brief  Lectura de unidades del magnetometro
         *
         *  Funcion para recuperar el registro de temperatura especificado.
         *
         *  @param  IMU_Handle         Manejador del sensor.
         *
         *  @return IMU_axis           Unidades de cada eje en formato flotante.
         *
        */
    extern  IMU_axis    IMU_ReadMagnetometer(IMU_Handle);
    /*!
         * @brief  Lectura de temperatura
         *
         *  Funcion para recuperar el registro de temperatura.
         *
         *  @param  IMU_Handle         Manejador del sensor.
         *
         *  @return IMU_Temp           Temperatura formato flotante, 0xFFFFFFFF si ha habido algun error.
         *
        */
    extern IMU_Temp     IMU_ReadTemperature(IMU_Handle);
    /*!
         * @brief  Forzar al sensor entrar en modo de bajo consumo
         *
         *  Funcion para forzar al sensor a entrar en modo de bajo consumo.
         *  Segun la hoja de datos, el sensor operando en modo I2C se encuentra en
         *  modo bajo consumo siempre y cuando no haya alguna operacion pendiente
         *  de escritura o lectura o transferencia de datos hacia el i2c maestro.
         *
         *  @param  IMU_Handle       Manejador del sensor.
         *
        */
    extern void         IMU_Sleep(IMU_Handle);
    /*!
             * @brief  Registrar evento de interrupción
             *
             *  Funcion para registrar un evento de interrupcion, evento que debe
             *  ejecutarse cuando un pin alcance un estado definido en las
             *  constantes de este driver.
             *  El periferico correspondiente al puerto de interrupcion debe
             *  estar activado ya antes de realizar la configuracion
             *  Para activar los eventos se debe hace un llamado unico a IMU_EnableEvents.
             *
             *  @param  IMU_Handle          Manejador del sensor.
             *  @param  IMU_CallbackEvent   Funcion de evento.
             *  @param  PORT_BASE           Direccion al puerto base del pin de eventos.
             *  @param  PIN                 Bit del puerto de eventos.
             *
            */
    extern void         IMU_setCallback(IMU_Handle,IMU_CallbackEvent,uint32_t PIN);
    /*!
             * @brief  Activar los eventos
             *
             *  Funcion para para activar los eventos, a partir de esta funcion,
             *  se comenzaran a generar eventos a nivel de interrupcion de hardware.
             *
             *  @param  IMU_Handle          Manejador del sensor.
             *
            */
    extern void         IMU_EnableEvents(IMU_Handle);
    /*!
           * @brief  Activar los eventos
           *
           *  Funcion para para desactivar los eventos, a partir de esta funcion,
           *  se dejaran de generar eventos.
           *
           *  @param  IMU_Handle          Manejador del sensor.
           *
           */
    extern void         IMU_DisableEvents(IMU_Handle);

#endif /* RAP_DRIVERS_IMU_H_ */
