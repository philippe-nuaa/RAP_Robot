#include <Robot/Robot.h>
#include <Robot/Wheels.h>
#include <Robot/IMUSensor.h>


#define TASKSTACKSIZE   512

Task_Struct     task0Struct;
Char task0Stack[TASKSTACKSIZE];

Void Task0(UArg, UArg);

int main(void){
    Task_Params         taskParams;

    Robot_init();
    Wheels_init();

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
//    rad         speed=0.0;
    RobotLog("Intentando activar el IMU\n");
    Robot_EnableIMU();
    IMUSensor_init();
    IMUSensor_Enable();

    Left_Wheel_Speed(0.0);
    Right_Wheel_Speed(0.0);
    Wheels_start();
TASK0_LOOP:
        RobotLogFloat(IMUSensor_getRoll());
        RobotLog(" , ");
        RobotLogFloat(IMUSensor_getPitch());
        RobotLog(" , ");
        RobotLogFloat(IMUSensor_getYaw());
        RobotLogCh('\n');
        Task_sleep(100);
    goto TASK0_LOOP;
}
