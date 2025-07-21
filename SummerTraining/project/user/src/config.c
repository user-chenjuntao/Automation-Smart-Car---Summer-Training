#include "config.h"

tagPID_T Speedpid;
tagPID_T SLpid;
tagPID_T SRpid;

PIDInitStruct SpeedPidInitStruct = 
{
	.fKp       = 1.41,     //.fKp
	.fKi       = 0,     //.fKi
	.fKd       = 8,     //.fKd8.2
	.fMax_Iout = 1,      //.fMax_Iout
	.fMax_Out  = 150,       //.fMax_Out
	.alpha     = 0.8
};

PIDInitStruct SLPidInitStruct = 
{
	.fKp       = 8.5,     //.fKp     8.5
	.fKi       = 1.5,     //.fKi     1.5
	.fKd       = 0,     //.fKd
	.fMax_Iout = 4000,      //.fMax_Iout
	.fMax_Out  = 7000,       //.fMax_Out
	.alpha     = 0.9
};

PIDInitStruct SRPidInitStruct = 
{
	.fKp       = 8.5,     //.fKp
	.fKi       = 1.5,     //.fKi
	.fKd       = 0,     //.fKd
	.fMax_Iout = 4000,      //.fMax_Iout
	.fMax_Out  = 7000,       //.fMax_Out
	.alpha     = 0.9
};