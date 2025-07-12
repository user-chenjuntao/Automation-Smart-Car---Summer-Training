#include "servo_pid.h"

int last_deltax = 0;
int now_delax   = 0;
uint16 Control  = CONTROL_MEDIAN;
float SU400_kp;

void servo_pid_test(void)
{
//	last_deltax = now_delax;
//	now_delax = final_mid_line - MID_W;
//	Control=S3010_kp*deltax+Contorl_median;
}