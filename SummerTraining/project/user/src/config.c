#include "config.h"

tagPID_T Speedpid;
tagPID_T SLpid;
tagPID_T SRpid;

PIDInitStruct SpeedPidInitStruct = 
{
	.fKp       = 1.4,     //.fKp
	.fKi       = 0,     //.fKi
	.fKd       = 4,     //.fKd
	.fMax_Iout = 1,      //.fMax_Iout
	.fMax_Out  = 150,       //.fMax_Out
	.alpha     = 0.8
};

PIDInitStruct SLPidInitStruct = 
{
	.fKp       = 8,     //.fKp
	.fKi       = 3,     //.fKi
	.fKd       = 0,     //.fKd
	.fMax_Iout = 100,      //.fMax_Iout
	.fMax_Out  = 4000,       //.fMax_Out
	.alpha     = 0.9
};

PIDInitStruct SRPidInitStruct = 
{
	.fKp       = 8,     //.fKp
	.fKi       = 3,     //.fKi
	.fKd       = 0,     //.fKd
	.fMax_Iout = 100,      //.fMax_Iout
	.fMax_Out  = 4000,       //.fMax_Out
	.alpha     = 0.9
};