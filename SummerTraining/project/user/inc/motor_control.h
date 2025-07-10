#ifndef _motor_control_h_
#define _motor_control_h_

#include "zf_common_typedef.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pwm.h"
#include "pid.h"
#include "config.h"


#define MAX_DUTY            (70 )                                               // 最大 MAX_DUTY% 占空比
#define DIR_L               (A0 )                                               // 左轮的接口
#define PWM_L               (TIM5_PWM_CH2_A1)                                   // 左轮的pwm输出通道

#define DIR_R               (A2 )                                               // 右轮的接口
#define PWM_R               (TIM5_PWM_CH4_A3)                                   // 右轮的pwm输出通道

#define SERVO_MOTOR_PWM             (TIM2_PWM_CH1_A15)                          // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (50 )                                       // 定义主板上舵机频率  请务必注意范围 50-300

#define SERVO_MOTOR_L_MAX           (75 )                                       // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_R_MAX           (105)                                       // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_INIT            (598)

void motor_init(void);
void servo_init(void);
void motor_pwm(int8 duty);
//void servo_pwm(uint32 num);
//void move_control();
#endif
