#include "config.h"

tagPID_T Speedpid;
tagPID_T Anglepid;

PIDInitStruct SpeedPidInitStruct = 
{
	.fKp       = 50,     //.fKp
	.fKi       = 0,     //.fKi
	.fKd       = 30,     //.fKd
	.fMax_Iout = 10,      //.fMax_Iout
	.fMax_Out  = 10       //.fMax_Out
};

PIDInitStruct AnglePidInitStruct = 
{
	.fKp       = 50,     //.fKp
	.fKi       = 0,     //.fKi
	.fKd       = 30,     //.fKd
	.fMax_Iout = 10,      //.fMax_Iout
	.fMax_Out  = 10       //.fMax_Out
};
