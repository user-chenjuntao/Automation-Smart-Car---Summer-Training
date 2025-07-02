#include "motor_control.h"

void motor_init(void)
{
	
	// �˴���д�û����� ���������ʼ�������
    gpio_init(DIR_L, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO ��ʼ��Ϊ��� Ĭ�����������
    pwm_init(PWM_L, 17000, 0);                                                  // PWM ͨ����ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0

    gpio_init(DIR_R, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO ��ʼ��Ϊ��� Ĭ�����������
    pwm_init(PWM_R, 17000, 0);                                                  // PWM ͨ����ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0
	
}

void motor_pwm(int8 duty)
{
	
	if (duty >= 50)
	{
		duty = 50;
	}
	else if (duty <= -50)
	{
		duty = -50;
	}
	if(0 <= duty)                                                           // ��ת
    {
        gpio_set_level(DIR_L, GPIO_HIGH);                                   // DIR����ߵ�ƽ
        pwm_set_duty(PWM_L, duty * (PWM_DUTY_MAX / 100));                   // ����ռ�ձ�

        gpio_set_level(DIR_R, GPIO_HIGH);                                   // DIR����ߵ�ƽ
        pwm_set_duty(PWM_R, duty * (PWM_DUTY_MAX / 100));                   // ����ռ�ձ�
     }
     else                                                                    // ��ת
     {
         gpio_set_level(DIR_L, GPIO_LOW);                                    // DIR����͵�ƽ
         pwm_set_duty(PWM_L, (-duty) * (PWM_DUTY_MAX / 100));                // ����ռ�ձ�

         gpio_set_level(DIR_R, GPIO_LOW);                                    // DIR����͵�ƽ
         pwm_set_duty(PWM_R, (-duty) * (PWM_DUTY_MAX / 100));                // ����ռ�ձ�
     }
	 

}