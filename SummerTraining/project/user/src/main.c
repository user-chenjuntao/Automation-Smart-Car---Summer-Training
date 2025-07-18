/*********************************************************************************************************************
* MM32F327X-G8P Opensourec Library 即（MM32F327X-G8P 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MM32F327X-G8P 开源库的一部分
* 
* MM32F327X-G8P 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 8.32.4 or MDK 5.37
* 适用平台          MM32F327X_G8P
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-08-10        Teternal            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

//#define FLASH_SECTION_INDEX       (127)                                         // 存储数据用的扇区 倒数第一个扇区
//#define FLASH_PAGE_INDEX          (3)                                           // 存储数据用的页码 倒数第一个页码


#define PIT_MENU                    (TIM6_PIT )                                     // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT_PRIORITY                (TIM6_IRQn)                                     // 对应周期中断的中断编号 在 mm32f3277gx.h 头文件中查看 IRQn_Type 枚举体
#define PIT_ENCODER                 (TIM7_PIT) 
//#define PIT_SERVO                   (TIM1_PIT) 
#define ENCODER_1                   (TIM3_ENCODER)
#define ENCODER_1_A                 (TIM3_ENCODER_CH1_B4)
#define ENCODER_1_B                 (TIM3_ENCODER_CH2_B5)

#define ENCODER_2                   (TIM4_ENCODER)
#define ENCODER_2_A                 (TIM4_ENCODER_CH1_B6)
#define ENCODER_2_B                 (TIM4_ENCODER_CH2_B7)


int encoder_data_1 = 0;
int encoder_data_2 = 0;
int v1= 0;
int v2= 0;


//uint8 image_threshold = 0;
//extern uint8 reference_point;


// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库移植用空工程

// **************************** 代码区域 ****************************
int main(void)
{
	
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                                                               // 初始化默认 Debug UART

	// 此处编写用户代码 例如外设初始化代码等
//    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // 判断是否有数据
//    {
//        flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);                // 擦除这一页
//    }
    // 此处编写用户代码 例如外设初始化代码等
	encoder_quad_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);                     // 初始化编码器模块与引脚 正交解码编码器模式
    encoder_quad_init(ENCODER_2, ENCODER_2_A, ENCODER_2_B);                     // 初始化编码器模块与引脚 正交解码编码器模式
	motor_init();
	menu_init();
	servo_init();
	key_init(50);
    pit_ms_init(PIT_MENU, 50);                                                     // 初始化 PIT 为周期中断 50ms 周期
	pit_ms_init(PIT_ENCODER, 20);
//	pit_ms_init(PIT_SERVO, 20);
    interrupt_set_priority(PIT_PRIORITY, 4);                                    // 设置 PIT 对周期中断的中断优先级为 4
//	flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);           // 将数据从 flash 读取到缓冲区
//	SpeedPidInitStruct.fKp = flash_union_buffer[0].float_type;
//	SpeedPidInitStruct.fKi = flash_union_buffer[1].float_type;
//	SpeedPidInitStruct.fKd = flash_union_buffer[2].float_type;
//	SpeedPidInitStruct.fMax_Iout = flash_union_buffer[3].float_type;
//	SpeedPidInitStruct.fMax_Out = flash_union_buffer[4].float_type;
//	SpeedPidInitStruct.alpha = flash_union_buffer[5].float_type;
//	flash_union_buffer[0].float_type = SpeedPidInitStruct.fKp;
//	flash_union_buffer[1].float_type = SpeedPidInitStruct.fKi;
//	flash_union_buffer[2].float_type = SpeedPidInitStruct.fKd;
//	flash_union_buffer[3].float_type = SpeedPidInitStruct.fMax_Iout;
//	flash_union_buffer[4].float_type = SpeedPidInitStruct.fMax_Out;
//	flash_union_buffer[5].float_type = SpeedPidInitStruct.alpha;
    
	PID_Init(&Speedpid, &SpeedPidInitStruct);
	PID_Init(&SLpid, &SLPidInitStruct);
	PID_Init(&SRpid, &SRPidInitStruct);
	
	while(1)
    {
        if(mt9v03x_init())
        {
            ips200_show_string(0, 16, "mt9v03x reinit.");
        }
        else
        {
            break;
        }
        system_delay_ms(500);                                                   // 短延时快速闪灯表示异常
    }
    ips200_show_string(0, 16, "init success.");
	system_delay_ms(1000);  
	ips200_clear();
	// 此处编写用户代码 例如外设初始化代码等
    // 此处编写用户代码 例如外设初始化代码等

    while(1)
    {
        // 此处编写需要循环执行的代码
//		image_threshold = otsuThreshold_less(*mt9v03x_image,MT9V03X_W,MT9V03X_H);
		if(mt9v03x_finish_flag)
		{
			image_process();
			mt9v03x_finish_flag=0;
		}
//		servo_pid_test();
//		dynamic_pid_value_set();
        menu_switch();
		menu_display();
		image_data_clear();
		
		
		if (car_go_flag)
		{
			if (Zebra_stop_flag == 1)
			{
				total_stop();
				
			}
			if (car_stop_flag == 1)
			{
				total_stop();
			}
			else
			{
				
				final_motor_control(280, 1.5, 100);
//				motor_pwm(2000,0);
			}
		}
		else
		{
			Motor_stop();
		}
		
		
        // 此处编写需要循环执行的代码
    }
//	while(1)
//	{
////		if (key_get_state(KEY_4 == 1))
////		{
////			v1 = 600;
////			v2 = 600;
////		}
////		if (key_get_state(KEY_1 == 1))
////		{
////			v1 = 0;
////			v2 = 0;
////		}
//		motor_control(600,0);
//		printf("%d,%d,%d,%d\n",encoder_data_1,encoder_data_2,v1,v2);
////		system_delay_ms(10);
//	}
}
// **************************** 代码区域 ****************************

void pit_encoder_handler (void)
{
    encoder_data_1 = encoder_get_count(ENCODER_1);                              // 获取编码器计数
    encoder_clear_count(ENCODER_1);                                             // 清空编码器计数

    encoder_data_2 = -encoder_get_count(ENCODER_2);                              // 获取编码器计数
    encoder_clear_count(ENCODER_2);                                             // 清空编码器计数
}

void pit_key_handler (void)
{
    key_scanner();                                                              // 周期中断触发 标志位置位
	
}

void pit_servo_handler (void)
{
	if (1)
	{
		Servo_control();
	}
	
}
