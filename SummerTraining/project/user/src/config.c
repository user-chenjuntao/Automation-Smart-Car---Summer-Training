#include "config.h"

tagPID_T Speedpid;
tagPID_T SLpid;
tagPID_T SRpid;

PIDInitStruct SpeedPidInitStruct = 
{
	.fKp       = 1.30,     //.fKp     1.30     1.16
	.fKi       = 0,     //.fKi
	.fKd       = 23.0,     //.fKd    23.0     13.80
	.fMax_Iout = 100,      //.fMax_Iout
	.fMax_Out  = 150,       //.fMax_Out
	.alpha     = 0.8
};

PIDInitStruct SLPidInitStruct = 
{
	.fKp       = 11.7,     //.fKp     8.5       11.7
	.fKi       = 2.15,     //.fKi     1.5       2.15
	.fKd       = 0,     //.fKd
	.fMax_Iout = 4000,      //.fMax_Iout
	.fMax_Out  = 7000,       //.fMax_Out
	.alpha     = 0.9
};

PIDInitStruct SRPidInitStruct = 
{
	.fKp       = 11.6,     //.fKp          11.6
	.fKi       = 2.15,     //.fKi          2.15
	.fKd       = 0,     //.fKd
	.fMax_Iout = 4000,      //.fMax_Iout
	.fMax_Out  = 7000,       //.fMax_Out
	.alpha     = 0.9
};