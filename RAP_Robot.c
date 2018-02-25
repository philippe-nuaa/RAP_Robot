#include <Robot/Robot.h>
//#include <Robot/IMUSensor.h>
#include <Robot/Control/Balance.h>
//#include <Robot/Wheels.h>

#define TASKSTACKSIZE   512

Task_Struct     task0Struct;
Char task0Stack[TASKSTACKSIZE];

Void Task0(UArg, UArg);

int main(void){
    Task_Params         taskParams;

    Robot_init();
    //
    //    Crear una tarea comun
    //
    Task_Params_init(&taskParams);
        taskParams.arg0 = 50;
        taskParams.stackSize = TASKSTACKSIZE;
        taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)Task0, &taskParams, NULL);

    BIOS_start();
    return (0);
}

Void Task0(UArg arg0,UArg arg1){
//    unsigned char i=0;
//    Wheels_init();
//    Wheels_start();
//    Left_Wheel_Speed(2.5);
//    Right_Wheel_Speed(2.5);
//
//TASK0_LOOP:
//        i++;
//        RobotLogFloat(Left_Wheel_readSpeed());
//        RobotLog(" , ");
//        RobotLogFloat(Right_Wheel_readSpeed());
//        RobotLogCh('\n');
//    Task_sleep(100);
//    goto TASK0_LOOP;


//    Robot_EnableIMU();
//    IMUSensor_init();
//    IMUSensor_Enable();
//TASK0_LOOP:
//        RobotLogHex(IMU_ReadRegister(Robot_IMU,IMU_ACCEL_CONFIG,WordConfiguration));
//        RobotLog(" , ");
//        RobotLogFloat(IMUSensor_getRoll());
//        RobotLog("\n");
//        Task_sleep(100);
//    goto TASK0_LOOP;

    float buffer = 0.0;
    char cmd=' ';
    Balance_Init();
    Balance_calibrate();
    Balance_start();
TASK0_LOOP:
    RobotLog("\nEsperando comando: ");
    cmd = RobotLogReadChar();
    switch(cmd){
        case 'k':
            RobotLog("\nCalibrando equilibrio\n");
            Balance_calibrate();
            break;
        case 'g':
            RobotLog("\nComenzando control de equilibrio\n");
            Balance_start();
            break;
        case 's':
            RobotLog("\nDeteniendo control de equilibrio\n");
            Balance_stop();
            break;
        case 't':
            RobotLog("\nBalance Kp : ");
                buffer = RobotLogReadFloat();
                Balance_setKp(buffer);

            RobotLog("\nBalance Ki : ");
                buffer = RobotLogReadFloat();
                Balance_setKi(buffer);

            RobotLog("\nBalance Kd : ");
                buffer = RobotLogReadFloat();
                Balance_setKd(buffer);
            break;
        default:
            RobotLog("\nComando desconocido!\n");
    }
        Task_sleep(100);
    goto TASK0_LOOP;
}
