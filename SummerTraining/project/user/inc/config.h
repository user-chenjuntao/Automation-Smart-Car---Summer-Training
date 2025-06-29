#ifndef __CONFIG_H_
#define __CONFIG_H_

#include "pid.h"

#define threshold 200
#define Basic_Speed 2200

extern tagPID_T Speedpid;
extern tagPID_T Anglepid;
extern PIDInitStruct SpeedPidInitStruct;
extern PIDInitStruct AnglePidInitStruct;



#endif
