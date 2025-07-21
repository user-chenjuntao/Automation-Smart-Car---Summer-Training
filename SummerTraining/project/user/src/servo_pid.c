//#include "servo_pid.h"

//int last_deltax = 0;
//int now_delax   = 0;
//uint8 straight_flag = 0;
//uint8 duty_pwm = 15;
//float Kp_base  = 0.7;
//float Kd_base  = 5;

//void servo_pid_test(void)
//{
//	last_deltax = now_delax;
//	now_delax = final_mid_line - MID_W;
//	if (abs(now_delax) <= 9)
//	{
//		straight_flag = 1;
//		Speedpid.fKp = 0.6;
//		Speedpid.fKd = 0.2;
//		duty_pwm = 18;
//	}
//	else
//	{
//		straight_flag = 0;
//		Speedpid.fKp = 1.4;
//		Speedpid.fKd = 3;
//		duty_pwm = 16;
//	}
////	Control=S3010_kp*deltax+Contorl_median;
//}

//void dynamic_pid_value_set(void)
//{
//    // 一次Kp（注释掉的版本）
//    // servo_pid.Kp=Kp_base+my_abs_float(MID_W-final_mid_line)*0.045;//最大值2.05
//    
//    // 二次Kp（当前使用的版本）
//    Speedpid.fKp=Kp_base+(MID_W-final_mid_line)*(MID_W-final_mid_line)*0.0012;//最大值2.05
//    Speedpid.fKi=0;
//    Speedpid.fKd=Kd_base;
//}
