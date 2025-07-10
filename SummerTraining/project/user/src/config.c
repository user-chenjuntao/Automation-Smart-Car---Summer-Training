#include "config.h"

tagPID_T Speedpid;
tagPID_T Anglepid;

PIDInitStruct SpeedPidInitStruct = 
{
	.fKp       = 2.4,     //.fKp
	.fKi       = 0,     //.fKi
	.fKd       = 1.2,     //.fKd
	.fMax_Iout = 1,      //.fMax_Iout
	.fMax_Out  = 150,       //.fMax_Out
	.alpha     = 1.0
};

PIDInitStruct AnglePidInitStruct = 
{
	.fKp       = 50,     //.fKp
	.fKi       = 0,     //.fKi
	.fKd       = 30,     //.fKd
	.fMax_Iout = 10,      //.fMax_Iout
	.fMax_Out  = 10,       //.fMax_Out
	.alpha     = 1.0
};
