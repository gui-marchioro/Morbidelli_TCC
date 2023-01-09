#ifndef MillChanger_h
#define MillChanger_h

#define MILL_TOOL_SLOT_MIN 1
#define MILL_TOOL_SLOT_MAX 10
#define TOOL_PRESENT_INPUT 1049
#define PISTON_TOOL_ON_INPUT 1048
#define PISTON_TOOL_OFF_INPUT 1047
#define PISTON_TOOL_OUTPUT 1132
#define SPHERE_GRIPPER_OUTPUT 1129
#define PANTOGRAPH_OFF_SENSOR_INPUT 1042 //Pantografo recuado/alto
#define PANTOGRAPH_MOVING_SENSOR_INPUT 1043 //Pantografo baixo
#define PANTOGRAPH_ON_SENSOR_INPUT 1044 //Pantografo baixo

#define OFFSET_X 20
#define OFFSET_Y -20
#define OFFSET_Z -7.5

#define FAST_SPEED 300 // mm/sec -> G0 velocity

#endif /* MillChanger_h */