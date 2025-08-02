#include "motor_control.h"
uint16 servo_pwm_value;
int motorL_pwm_value = 0;
int motorR_pwm_value = 0;

extern int encoder_data_1;
extern int encoder_data_2;
extern int v1;
extern int v2;
void motor_init(void)
{
	
	// 此处编写用户代码 例如外设初始化代码等
    gpio_init(DIR_L, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(DIR_R, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_R, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0
	
}

void servo_init(void)
{
	pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);
	pwm_set_duty(SERVO_MOTOR_PWM, SERVO_MOTOR_INIT);
}

void motor_pwm(int duty_L, int duty_R)
{
	
//	if (duty_L >= 7000 || duty_R >= 7000)
//	{
//		duty_L = 0;
//		duty_R = 0;
//	}
//	else if (duty_L <= -7000 || duty_R <= -7000)
//	{
//		duty_L = 0;
//		duty_R = 0;
//	}
	if(0 <= duty_L)                                                           // 正转
    {
        gpio_set_level(DIR_L, GPIO_HIGH);                                   // DIR输出高电平
        pwm_set_duty(PWM_L, duty_L);                   // 计算占空比
     }
     else                                                                    // 反转
     {
         gpio_set_level(DIR_L, GPIO_LOW);                                    // DIR输出低电平
         pwm_set_duty(PWM_L, (-duty_L));                // 计算占空比
     }
	 if (0 <= duty_R)
	 {
		 gpio_set_level(DIR_R, GPIO_HIGH);                                   // DIR输出高电平
         pwm_set_duty(PWM_R, duty_R);                   // 计算占空比
	 }
	 else
	 {
		 gpio_set_level(DIR_R, GPIO_LOW);                                    // DIR输出低电平
         pwm_set_duty(PWM_R, (-duty_R));                // 计算占空比
	 }
	 
	 
	         

}

int Limit_int(int left_limit, int target_num, int right_limit)
{
	if (left_limit > right_limit )
	{
		int temp = left_limit;
		left_limit = right_limit;
		right_limit  =temp;
	}
	if (target_num < left_limit)
	{
		return left_limit;
	}
	else if (target_num > right_limit)
	{
		return right_limit;
	}
	else 
	{
		return target_num;
	}
}


void Servo_control(void)
{
	int16 servo_num = 0;
//	servo_num = PID_Location_Calculate(&Speedpid, line_error, 0);
//	dynamic_pid_value_set();
	servo_num = PID_Location_Calculate(&Speedpid, line_error, 0);
	servo_pwm_value = SERVO_MOTOR_INIT + servo_num;
	servo_pwm_value = Limit_int(SERVO_MOTOR_LEFT_LIMIT, servo_pwm_value, SERVO_MOTOR_RIGHT_LIMIT);
	pwm_set_duty(SERVO_MOTOR_PWM, servo_pwm_value);
}

void motor_control(int TargetEncoder_L, int TargetEncoder_R)
{
	int Speed_L = TargetEncoder_L;
	int Speed_R = TargetEncoder_R;
	motorL_pwm_value = Limit_int(LIMIT_PWM_MIN, PID_Add_Calculate(&SLpid, encoder_data_1, Speed_L), LIMIT_PWM_MAX);
	motorR_pwm_value = Limit_int(LIMIT_PWM_MIN, PID_Add_Calculate(&SRpid, encoder_data_2, Speed_R), LIMIT_PWM_MAX);
	motor_pwm(motorL_pwm_value, motorR_pwm_value);
}

void final_motor_control(int speed, float k, int limit)
{
	motor_control(Limit_int(speed-limit, speed+k*line_error, speed+limit), Limit_int(speed-limit, speed-k*line_error, speed+limit));
}

void Servo_stop(void)
{
	pwm_set_duty(SERVO_MOTOR_PWM, SERVO_MOTOR_INIT);
}

void Motor_stop(void)
{
	motor_control(0, 0);
//	motorL_pwm_value = 0;
//	motorR_pwm_value = 0;
//	encoder_data_1 = 0;
//	encoder_data_2 = 0;
//	PID_Clear(&SLpid);
//	PID_Clear(&SRpid);
}

void All_stop(void)
{
	Servo_stop();
	motor_pwm(0,0);
	while (1)
	{
		if (key_get_state(KEY_4 == 1))
		{
			Zebra_stop_flag = 0;
			car_stop_flag = 0;
//			huandao_clear();
			break;
		}
	}
}

void total_stop(void)
{
	All_stop();
	while (1)
	{
		if (key_get_state(KEY_4 == 1))
		{
			Zebra_stop_flag = 0;
			car_stop_flag = 0;
//			huandao_clear();
			break;
		}
	}
}
uint8 start1 = 0;

void car_start(void)
{
    // 判断 KEY_1 是否短按（触发启动/停止切换）
    if(key_get_state(KEY_1) == KEY_SHORT_PRESS)  
    {
        start1++;  // 切换状态计数器自增
    }
    
    // 若处于“启动”状态（start1≠0）
    if(start1 != 0)                             
    {
        // 奇数状态：电机以 80 占空比运行
        if(start1 % 2 == 1)                     
        {
			v1 = 300;
			v2 = 300;
            motor_control(300, 300);              // 直接控制电机（示例：双轮80%占空比）
            // Final_Motor_Control(80, 0.3, 30); // 注释：闭环控制（若需启用可取消注释）
        }
        // 偶数状态：电机停止
        else                                    
        {
			v1 = 0;
			v2 = 0;
            motor_control(0, 0);                // 电机占空比设为0，停止运行
        }
    }
}