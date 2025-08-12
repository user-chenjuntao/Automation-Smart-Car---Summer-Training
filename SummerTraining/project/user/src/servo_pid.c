#include "servo_pid.h"

int last_deltax = 0;
int now_delax   = 0;
uint8 straight_flag = 0;
float Kp_base  = 0.6;
float Kp_value  = 0.00070;

void servo_pid_test(void)
{
	last_deltax = now_delax;
	now_delax = final_mid_line - MID_W;
	if (abs(now_delax) <= 9)
	{
		straight_flag = 1;
		Speedpid.fKp = 0.6;
		Speedpid.fKd = 0.2;

	}
	else
	{
		straight_flag = 0;
		Speedpid.fKp = 1.4;
		Speedpid.fKd = 3;

	}
//	Control=S3010_kp*deltax+Contorl_median;
}

void dynamic_pid_value_set(void)
{
    // 一次Kp（注释掉的版本）
    // servo_pid.Kp=Kp_base+my_abs_float(MID_W-final_mid_line)*0.045;//最大值2.05
    if (line_error > 40)
	{
		line_error = 40;
	}
	else if (line_error < -40)
	{
		line_error = -40;
	}
    // 二次Kp（当前使用的版本）
    Speedpid.fKp=Kp_base+line_error*line_error*Kp_value;//最大值2.05
	if (Speedpid.fKp > 1.4)
	{
		Speedpid.fKp = 1.4;
	}
//    Speedpid.fKd=imu963ra_gyro_z*Kd_value;
}
