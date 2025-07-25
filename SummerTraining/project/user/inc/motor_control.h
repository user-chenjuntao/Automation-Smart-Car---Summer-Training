#ifndef _motor_control_h_
#define _motor_control_h_

#include "zf_common_typedef.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pwm.h"
#include "pid.h"
#include "config.h"
#include "image_my.h"
#include "zf_device_key.h"
#include "menu.h"
#include "servo_pid.h"


#define MAX_DUTY            (70 )                                               // 最大 MAX_DUTY% 占空比
#define DIR_R               (A0 )                                               // 左轮的接口
#define PWM_R               (TIM5_PWM_CH2_A1)                                   // 左轮的pwm输出通道

#define DIR_L               (A2 )                                               // 右轮的接口
#define PWM_L               (TIM5_PWM_CH4_A3)                                   // 右轮的pwm输出通道


#define SERVO_MOTOR_PWM             (TIM2_PWM_CH1_A15)                          // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (50 )                                       // 定义主板上舵机频率  请务必注意范围 50-300

#define SERVO_MOTOR_L_MAX           (75 )                                       // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_R_MAX           (105)                                       // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_INIT            (598)
#define SERVO_MOTOR_LEFT_LIMIT      (515)
#define SERVO_MOTOR_RIGHT_LIMIT     (685)

//#define LEFT_PWM                    20
//#define RIGHT_PWM                   20
#define LIMIT_PWM_MIN              -7000
#define LIMIT_PWM_MAX               7000



extern uint16 servo_pwm_value;
extern int motorL_pwm_value;
extern int motorR_pwm_value;


void motor_init(void);
void servo_init(void);
void motor_pwm(int duty_L, int duty_R);
int Limit_int(int left_limit, int target_num, int right_limit);
void Servo_control(void);
void motor_control(int Encoder_L, int Encoder_R);
void Servo_stop(void);
void Motor_stop(void);
void All_stop(void);
void final_motor_control(int speed, float k, int limit);
void total_stop(void);
void car_start(void);
//void servo_pwm(uint32 num);
//void move_control();
#endif
