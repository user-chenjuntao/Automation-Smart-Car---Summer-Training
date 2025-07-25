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


#define MAX_DUTY            (70 )                                               // ��� MAX_DUTY% ռ�ձ�
#define DIR_R               (A0 )                                               // ���ֵĽӿ�
#define PWM_R               (TIM5_PWM_CH2_A1)                                   // ���ֵ�pwm���ͨ��

#define DIR_L               (A2 )                                               // ���ֵĽӿ�
#define PWM_L               (TIM5_PWM_CH4_A3)                                   // ���ֵ�pwm���ͨ��


#define SERVO_MOTOR_PWM             (TIM2_PWM_CH1_A15)                          // ���������϶����Ӧ����
#define SERVO_MOTOR_FREQ            (50 )                                       // ���������϶��Ƶ��  �����ע�ⷶΧ 50-300

#define SERVO_MOTOR_L_MAX           (75 )                                       // ���������϶�����Χ �Ƕ�
#define SERVO_MOTOR_R_MAX           (105)                                       // ���������϶�����Χ �Ƕ�
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
