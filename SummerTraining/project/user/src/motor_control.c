#include "motor_control.h"

void motor_init(void)
{
	
	// 此处编写用户代码 例如外设初始化代码等
    gpio_init(DIR_L, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(DIR_R, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_R, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0
	
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
	if(0 <= duty)                                                           // 正转
    {
        gpio_set_level(DIR_L, GPIO_HIGH);                                   // DIR输出高电平
        pwm_set_duty(PWM_L, duty * (PWM_DUTY_MAX / 100));                   // 计算占空比

        gpio_set_level(DIR_R, GPIO_HIGH);                                   // DIR输出高电平
        pwm_set_duty(PWM_R, duty * (PWM_DUTY_MAX / 100));                   // 计算占空比
     }
     else                                                                    // 反转
     {
         gpio_set_level(DIR_L, GPIO_LOW);                                    // DIR输出低电平
         pwm_set_duty(PWM_L, (-duty) * (PWM_DUTY_MAX / 100));                // 计算占空比

         gpio_set_level(DIR_R, GPIO_LOW);                                    // DIR输出低电平
         pwm_set_duty(PWM_R, (-duty) * (PWM_DUTY_MAX / 100));                // 计算占空比
     }
	 

}