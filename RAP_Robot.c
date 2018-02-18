#include <Robot/Robot.h>
#include <Robot/Wheels.h>

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
    rad speed=0.0;
    Robot_EnableIMU();
    RobotLog("\nWrite the Left speed\n");
        speed = RobotLogReadFloat();
    RobotLogFloat(speed);
        Left_Wheel_Speed(speed);

    RobotLog("\nWrite the Right speed\n");
        speed = RobotLogReadFloat();
    RobotLogFloat(speed);
        Right_Wheel_Speed(speed);

    RobotLog(",\n");

    Wheels_start();
TASK0_LOOP:
        RobotLogFloat( Right_Wheel_readSpeed() );
        RobotLogFloat( Left_Wheel_readSpeed() );
        Task_sleep(100);
    goto TASK0_LOOP;
}
