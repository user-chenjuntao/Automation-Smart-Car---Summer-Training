#include "menu.h"

uint32 straight_speed = 6000,turn_speed = 3000;
uint32 speed_limit = 9000,left_limit = 500,right_limit = 2500;
extern uint16 servo_pwm_value;
uint32 angle = 90;
float level[5] = {100,10,1,0.1,0.01};
static uint8 level_i = 0;
extern int16 encoder_data_1;
extern int16 encoder_data_2;
extern uint8 duty_pwm;
extern uint8 break_num_left;
extern uint8 break_num_right;
//uint32 pid[5]={}

//-----------------------------------------
//一级菜单（主菜单）
//-----------------------------------------
menu_item main_Menu = {
	.name = "MainMenu",
	.content = {"CarGo","DataParameter","DataStatus","Image"},
	.number = 4,
};

//--------------------------------------------------

//-----------------------------------------
//二级菜单——跑车程序（DataParameter）
//-----------------------------------------
menu_item CarGo_Menu = {
	.name = "CarGo",
	.content = {"Start","End"},
	.number = 2,
};
//-----------------------------------------
//二级菜单——参数调整（DataParameter）
//-----------------------------------------
menu_item DataParameter_Menu = {
	.name = "DataParameter",
	.content = {"Speed","Servo","PID"},
	.number = 3,
};
//-----------------------------------------
//二级菜单——参数状态（DataStatus）
//-----------------------------------------
menu_item DataStatusMenu = {
	.name = "DataStatus",
	.content = {"Speed","Servo","PID"},
	.number = 3,
};
//-----------------------------------------
//二级菜单——图像与中间量（Image）
//-----------------------------------------
menu_item ImageMenu = {
	.name = "Image",
	.content = {"BinaryImage","Variable"},
	.number = 2,
};

//------------------------------------------------------

//-----------------------------------------
//三级菜单——速度调整
//-----------------------------------------
menu_item Pa_Speed_Menu = {
	.name = "PaSpeed",
	.content = {"LineSpeed","TurnSpeed","Limit"},
	.number = 3,
};
//-----------------------------------------
//三级菜单——速度调整
//-----------------------------------------
menu_item Pa_Servo_Menu = {
	.name = "PaServo",
	.content = {"InitPwm","LeftLimit","RightLimit"},
	.number = 3,
};
//-----------------------------------------
//三级菜单——PID调整
//-----------------------------------------
menu_item Pa_Pid_Menu = {
	.name = "PaPid",
	.content = {"SKp","SKi","SKd","SMax_Iout","SMax_out","Alpha"},
	.number = 6,
};
//-----------------------------------------
//三级菜单——速度状态
//-----------------------------------------
menu_item St_Speed_Menu = {
	.name = "StSpeed",
	.content = {"LineSpeed","TurnSpeed","Limit"},
	.number = 3,
};
//-----------------------------------------
//三级菜单——速度状态
//-----------------------------------------
menu_item St_Servo_Menu = {
	.name = "StServo",
	.content = {"InitPwm","LeftLimit","RightLimit"},
	.number = 3,
};
//-----------------------------------------
//三级菜单——PID状态
//-----------------------------------------
menu_item St_Pid_Menu = {
	.name = "StPid",
	.content = {"SKp","SKi","SKd","SMax_Iout","SMax_out","Alpha"},
	.number = 6,
};

static uint16 cursor = 1;
static uint8 currentIndex = 1;
static uint8 pastIndex = 0;
menu_item *currentMenu=&main_Menu;

void moveup(void)
{
    if (cursor> 1 + (cursor - cursor % 10))
    cursor--;
    else
    cursor= (cursor - cursor % 10) + currentMenu->number;
}

void movedown(void)
{
    if (cursor < currentMenu->number + (cursor - cursor % 10))
    cursor++;
    else
    cursor=1 + (cursor - cursor % 10);
}

void getin(void)
{
    if (cursor < 100)
    {
//        currentIndex = cursor % 10;
        switch (cursor)
        {
            case CARGO:
                currentMenu = &CarGo_Menu;
                break;
            case DATAPARAMETER:
                currentMenu = &DataParameter_Menu;
				break;
			case DATASTATUS:
				currentMenu = &DataStatusMenu;
				break;
			case IMAGE:
				currentMenu = &ImageMenu;
			    break;
			
			case PRAMETERSPEED:
				currentMenu = &Pa_Speed_Menu;
				break;
			case PRAMETERSERVO:
				currentMenu = &Pa_Servo_Menu;
				break;
			case PRAMETERPID:
				currentMenu = &Pa_Pid_Menu;
//				SpeedPidInitStruct.fKp = flash_union_buffer[0].float_type;
//				SpeedPidInitStruct.fKi = flash_union_buffer[1].float_type;
//				SpeedPidInitStruct.fKd = flash_union_buffer[2].float_type;
//				SpeedPidInitStruct.fMax_Iout = flash_union_buffer[3].float_type;
//				SpeedPidInitStruct.fMax_Out = flash_union_buffer[4].float_type;
//				SpeedPidInitStruct.alpha = flash_union_buffer[5].float_type;
				break;
			case STATUSSPEED:
				currentMenu = &St_Speed_Menu;
				break;
			case STATUSSERVO:
				currentMenu = &St_Servo_Menu;
				break;
			case STATUSPID:
				currentMenu = &St_Pid_Menu;
				break;
				
            default:
                break;
        }
        cursor = cursor * 10 + 1;
		if (cursor > 1000)
		{
			cursor = (cursor - 1) / 10;
		}
    }
}

void getout(void)
{
	if (cursor > 10)
	{
		cursor = cursor / 10;
		switch (cursor)
        {
            case CARGO:
                currentMenu = &main_Menu;
                break;
            case DATAPARAMETER:
                currentMenu = &main_Menu;
				break;
			case DATASTATUS:
				currentMenu = &main_Menu;
				break;
			case IMAGE:
				currentMenu = &main_Menu;
			    break;
			
			case PRAMETERSPEED:
				currentMenu = &DataParameter_Menu;
				break;
			case PRAMETERSERVO:
				currentMenu = &DataParameter_Menu;
				break;
			case PRAMETERPID:
				currentMenu = &DataParameter_Menu;
				break;
			case STATUSSPEED:
				currentMenu = &DataStatusMenu;
				break;
			case STATUSSERVO:
				currentMenu = &DataStatusMenu;
				break;
			case STATUSPID:
				currentMenu = &DataStatusMenu;
				break;
				
            default:
                break;
        }
	}
}

void menu_init(void)
{
	ips200_set_dir(IPS200_PORTAIT);
    ips200_set_font(IPS200_8X16_FONT);
    ips200_set_color(RGB565_WHITE, RGB565_BLACK);
    ips200_init(IPS200_TYPE_SPI);
}

void menu_display(void)
{
//	ips200_clear();
	currentIndex = cursor % 10;
	pastIndex = cursor / 10;
	ips200_show_string(0,0,currentMenu->name);
	for (uint8 i = 0; i < currentMenu->number; i++)
	{
		if (currentIndex == (i+1))
		{
			ips200_show_string(0, 16+i*16, "->");
			ips200_show_string(24, 16+i*16, currentMenu->content[i]);
		}
		else
		{
			ips200_show_string(0, 16+i*16, "  ");
			ips200_show_string(24, 16+i*16, currentMenu->content[i]);
		}
	}
	
	ips200_show_string(0, 224, "cursor");
	ips200_show_uint(200, 224, cursor, 3);
	if (cursor <100)
	{
		ips200_show_string(0, 300, "E5:OUT|E4:IN|E3:DOWN|E2:UP");
	}
	switch (pastIndex)
	{
		case MAIN:
//			ips200_show_string(0, 300, "E5:OUT/E4:IN/E3:DOWN/E2:UP");
			break;
		case CARGO:
			ips200_show_string(192, 16, "start");
			ips200_show_string(208, 32, "end");
//			ips200_show_string(0, 300, "E5:OUT/E4:IN/E3:DOWN/E2:UP");
			break;
		case PRAMETERSPEED:
			ips200_show_int(200, 16, duty_pwm, 4);
			ips200_show_uint(200, 32, turn_speed, 4);
			ips200_show_uint(200, 48, speed_limit, 4);
			ips200_show_string(0, 208, "level");
			ips200_show_float(184, 208, level[level_i], 3, 2);
			ips200_show_string(0, 300, "E5:LEVEL|E4:-|E3:+|E2:UP/OUT");
			break;
		case PRAMETERSERVO:
			ips200_show_uint(184, 16, servo_pwm_value, 3);
			ips200_show_uint(200, 32, left_limit, 4);
			ips200_show_uint(200, 48, right_limit, 4);
			ips200_show_string(0, 208, "level");
			ips200_show_float(184, 208, level[level_i], 3, 2);
			ips200_show_string(0, 300, "E5:LEVEL|E4:-|E3:+|E2:UP/OUT");
			break;
		case PRAMETERPID:
			ips200_show_float(184, 16, Speedpid.fKp, 3, 2);
			ips200_show_float(184, 32, Speedpid.fKi, 3, 2);
			ips200_show_float(184, 48, Speedpid.fKd, 3, 2);
			ips200_show_float(184, 64, Speedpid.fMax_Iout, 3, 2);
			ips200_show_float(184, 80, Speedpid.fMax_Out, 3, 2);
			ips200_show_float(184, 96, Speedpid.alpha, 1, 1);
			ips200_show_string(0, 208, "level");
			ips200_show_float(184, 208, level[level_i], 3, 2);
			ips200_show_string(0, 300, "E5:LEVEL|E4:-|E3:+|E2:UP/OUT");
			break;
		case STATUSSPEED:
			ips200_show_int(200, 16, duty_pwm, 4);
			ips200_show_uint(200, 32, turn_speed, 4);
			ips200_show_uint(200, 48, speed_limit, 4);
			ips200_show_string(0, 208, "level");
			ips200_show_float(184, 208, level[level_i], 3, 2);
			ips200_show_string(0, 300, "E5:LEVEL|E4:-|E3:+|E2:UP/OUT");
			break;
		case STATUSSERVO:
			ips200_show_uint(184, 16, servo_pwm_value, 3);
			ips200_show_uint(200, 32, left_limit, 4);
			ips200_show_uint(200, 48, right_limit, 4);
			ips200_show_string(0, 208, "level");
			ips200_show_float(184, 208, level[level_i], 3, 2);
			ips200_show_string(0, 300, "E5:LEVEL|E4:-|E3:+|E2:UP/OUT");
			break;
		case STATUSPID:
			ips200_show_float(184, 16, Speedpid.fKp, 3, 2);
			ips200_show_float(184, 32, Speedpid.fKi, 3, 2);
			ips200_show_float(184, 48, Speedpid.fKd, 3, 2);
			ips200_show_float(184, 64, Speedpid.fMax_Iout, 3, 2);
			ips200_show_float(184, 80, Speedpid.fMax_Out, 3, 2);
			ips200_show_float(184, 96, Speedpid.alpha, 1, 1);
			ips200_show_string(0, 208, "level");
			ips200_show_float(184, 208, level[level_i], 3, 2);
			ips200_show_string(0, 300, "E5:LEVEL|E4:-|E3:+|E2:UP/OUT");
			break;
		case IMAGE:
//			ips200_show_uint(200, 16, image_threshold, 4);
			image_show();
			
			ips200_show_string(0, 240, "angle");
			ips200_show_string(0, 256, "encoder_1");
			ips200_show_string(0, 272, "encoder_2");
		    ips200_show_uint(200, 240, servo_pwm_value, 4);
			ips200_show_int(200, 256, encoder_data_1, 4);
			ips200_show_int(200, 272, encoder_data_2, 4);
//			ips200_show_string(0, 300, "E5:LEVEL|E4:-|E3:+|E2:UP/OUT");
		default:
			break;
			
	}
}



void menu_switch(void)
{

	if (cursor < 100)
	{
		if (key_get_state(KEY_1) == 1)
		{
			moveup();
			key_clear_state(KEY_1);
		}
		else if (key_get_state(KEY_2) == 1)
		{
			movedown();
			key_clear_state(KEY_2);
		}
		if (key_get_state(KEY_3) == 1)
		{
			getin();
			key_clear_state(KEY_3);
			ips200_clear();
		}
		else if (key_get_state(KEY_4) == 1)
		{
			getout();
			key_clear_state(KEY_4);
			ips200_clear();
		}
	}
	else if (cursor > 100)
	{
		if (key_get_state(KEY_1) == 1)
		{
			moveup();
			key_clear_state(KEY_1);
		}
		else if (key_get_state(KEY_1) == 2)
		{
			getout();
//			flash_union_buffer[0].float_type = SpeedPidInitStruct.fKp;
//			flash_union_buffer[1].float_type = SpeedPidInitStruct.fKi;
//			flash_union_buffer[2].float_type = SpeedPidInitStruct.fKd;
//			flash_union_buffer[3].float_type = SpeedPidInitStruct.fMax_Iout;
//			flash_union_buffer[4].float_type = SpeedPidInitStruct.fMax_Out;
//			flash_union_buffer[5].float_type = SpeedPidInitStruct.alpha;
			key_clear_state(KEY_1);
			ips200_clear();
		}
		if (key_get_state(KEY_2) == 1)       //数值加
		{
			switch (cursor)
			{
				case 211:
					duty_pwm += (uint32)level[level_i];
					break;
				case 212:
					turn_speed += (uint32)level[level_i];
					break;
				case 213:
					speed_limit += (uint32)level[level_i];
					break;
				case 221:
					servo_pwm_value += (uint32)level[level_i];
					break;
				case 222:
					left_limit += (uint32)level[level_i];
					break;
				case 223:
					right_limit += (uint32)level[level_i];
					break;
				case 231:
					Speedpid.fKp += level[level_i];
					break;
				case 232:
					Speedpid.fKi += level[level_i];
					break;
				case 233:
					Speedpid.fKd += level[level_i];
					break;
				case 234:
					Speedpid.fMax_Iout += level[level_i];
					break;
				case 235:
					Speedpid.fMax_Out += level[level_i];
					break;
				case 236:
					Speedpid.alpha += level[level_i];
					break;
				default:
					break;
			}
			key_clear_state(KEY_2);
		}
		else if (key_get_state(KEY_3) == 1)  //数值减
		{
			switch (cursor)
			{
				case 211:
					duty_pwm -= (uint32)level[level_i];
					break;
				case 212:
					turn_speed -= (uint32)level[level_i];
					break;
				case 213:
					speed_limit -= (uint32)level[level_i];
					break;
				case 221:
					servo_pwm_value -= (uint32)level[level_i];
					break;
				case 222:
					left_limit -= (uint32)level[level_i];
					break;
				case 223:
					right_limit -= (uint32)level[level_i];
					break;
				case 231:
					Speedpid.fKp -=level[level_i];
					break;
				case 232:
					Speedpid.fKi -=level[level_i];
					break;
				case 233:
					Speedpid.fKd -=level[level_i];
					break;
				case 234:
					Speedpid.fMax_Iout -=level[level_i];
					break;
				case 235:
					Speedpid.fMax_Out -=level[level_i];
					break;
				case 236:
					Speedpid.alpha -= level[level_i];
					break;
				default:
					break;
			}
			key_clear_state(KEY_3);
		}
		if (key_get_state(KEY_4) == 1)
		{
			level_i++;
			if (level_i > 4)
			{
				level_i = 0;
			}
			key_clear_state(KEY_4);
		}
	}
}