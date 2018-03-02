#include <Robot/Robot.h>// Incluye todos los drivers del hardware

//#include <Robot/IMUSensor.h>
//#include <Robot/Control/Balance.h>
//#include <Robot/Wheels.h>
#include  <stdbool.h>

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
//    float buffer=0.0;
//    //
//    //  Wheels Test
//    //
//    Wheels_init();
//    Wheels_start();
//    Left_Wheel_Speed(0);
//    Right_Wheel_Speed(0);
//
//TASK0_LOOP:
//    switch(RobotLogReadChar()){
//    case 's'://Stop
//        flag = false;
//        RobotLog("\nData Configured:\n");
//        RobotLog("\n\t\tLeft Wheel : P-I-D\n");
//            RobotLogFloat(Left_getKp());RobotLogEndl;
//            RobotLogFloat(Left_getKi());RobotLogEndl;
//            RobotLogFloat(Left_getKd());RobotLogEndl;
//        RobotLog("\n\t\tRight Wheel : P-I-D\n");
//            RobotLogFloat(Right_getKp());RobotLogEndl;
//            RobotLogFloat(Right_getKi());RobotLogEndl;
//            RobotLogFloat(Right_getKd());RobotLogEndl;
//        Wheels_stop();
//        Left_Wheel_Speed(0);
//        Right_Wheel_Speed(0);
//        break;
//    case 'p':
//        RobotLog("\nCalibrate Proportional : ");
//        buffer = RobotLogReadFloat();
//        RobotLog("\n[l]-Left or [r]-Right\n");
//        if('l'==RobotLogReadChar()){
//            Left_setKp(buffer);
//        }else{
//            Right_setKp(buffer);
//        }
//        break;
//    case 'i':
//        RobotLog("\nCalibrate Integral : ");
//        buffer = RobotLogReadFloat();
//        RobotLog("\n[l]-Left or [r]-Right\n");
//        if('l'==RobotLogReadChar()){
//            Left_setKi(buffer);
//        }else{
//            Right_setKi(buffer);
//        }
//        break;
//    case 'd':
//        RobotLog("\nCalibrate Derivative : ");
//        buffer = RobotLogReadFloat();
//        RobotLog("\n[l]-Left or [r]-Right\n");
//        if('l'==RobotLogReadChar()){
//            Left_setKd(buffer);
//        }else{
//            Right_setKd(buffer);
//        }
//        break;
//    case 'r':
//        RobotLog("\nLeft Speed : ");
//        Left_Wheel_Speed( RobotLogReadFloat() );
//        RobotLog("\nRight Speed : ");
//        Right_Wheel_Speed( RobotLogReadFloat() );
//        Wheels_start();
//        flag = true;
//        break;
//    default:
//        RobotLog("Unkown command!\n");
//    }
//    RobotLogEndl;
//    Task_sleep(100);
//    goto TASK0_LOOP;


//    //
//    //  IMU TEST
//    //
//    Robot_EnableIMU();
//    IMUSensor_init();
//    IMUSensor_Enable();
//TASK0_LOOP:
//        RobotLogFloat(IMUSensor_getXRotation());
//        RobotLog(" , ");
//        RobotLogFloat(IMUSensor_getYRotation());
//        RobotLog(" , ");
//        RobotLogFloat(IMUSensor_getZRotation());
//        RobotLog("\n");
//        Task_sleep(100);
//    goto TASK0_LOOP;

//    float buffer = 0.0;
//    char cmd=' ';
//    Balance_Init();
//    Balance_calibrate();
//    Balance_start();
//TASK0_LOOP:
//    RobotLog("\nEsperando comando: ");
//    cmd = RobotLogReadChar();
//    switch(cmd){
//        case 'k':
//            RobotLog("\nCalibrando equilibrio\n");
//            Balance_calibrate();
//            break;
//        case 'g':
//            RobotLog("\nComenzando control de equilibrio\n");
//            Balance_start();
//            break;
//        case 's':
//            RobotLog("\nDeteniendo control de equilibrio\n");
//            Balance_stop();
//            break;
//        case 't':
//            RobotLog("\nBalance Acc Kp : ");
//                buffer = RobotLogReadFloat();
//                Balance_setAccKp(buffer);
//            RobotLog("\nBalance Gyr Kp : ");
//                buffer = RobotLogReadFloat();
//                Balance_setGyrKp(buffer);
//            break;
//        default:
//            RobotLog("\nComando desconocido!\n");
//    }
//        Task_sleep(100);
//    goto TASK0_LOOP;

float buffer=0.0;
char cmd=' ';
    MotorDriver_Start(Robot_Motor);
TASK0_LOOP:
    RobotLog("\nEsperando comando: ");
    cmd = RobotLogReadChar();
    switch(cmd){
        case 's':
            MotorDriver_Stop(Robot_Motor);
            break;
        case 'g':
            MotorDriver_Start(Robot_Motor);
            break;
        case 't':
            RobotLog("\nLeft Speed : ");
                buffer = RobotLogReadFloat();
                MotorDriver_Left(Robot_Motor,buffer);
            RobotLog("\nRight Speed : ");
                buffer = RobotLogReadFloat();
                MotorDriver_Right(Robot_Motor,buffer);
            break;
        default:
            RobotLog("\nComando desconocido!\n");
    }
    Task_sleep(100);
    goto TASK0_LOOP;
}
