#ifndef _motor_control_h_
#define _motor_control_h_

#include "zf_common_typedef.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pwm.h"
#include "pid.h"
#include "config.h"


#define MAX_DUTY            (70 )                                               // ��� MAX_DUTY% ռ�ձ�
#define DIR_L               (A0 )                                               // ���ֵĽӿ�
#define PWM_L               (TIM5_PWM_CH2_A1)                                   // ���ֵ�pwm���ͨ��

#define DIR_R               (A2 )                                               // ���ֵĽӿ�
#define PWM_R               (TIM5_PWM_CH4_A3)                                   // ���ֵ�pwm���ͨ��

#define SERVO_MOTOR_PWM             (TIM2_PWM_CH1_A15)                          // ���������϶����Ӧ����
#define SERVO_MOTOR_FREQ            (50 )                                       // ���������϶��Ƶ��  �����ע�ⷶΧ 50-300

#define SERVO_MOTOR_L_MAX           (75 )                                       // ���������϶�����Χ �Ƕ�
#define SERVO_MOTOR_R_MAX           (105)                                       // ���������϶�����Χ �Ƕ�
#define SERVO_MOTOR_INIT            (598)

void motor_init(void);
void servo_init(void);
void motor_pwm(int8 duty);
//void servo_pwm(uint32 num);
//void move_control();
#endif
