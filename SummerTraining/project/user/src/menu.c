#include "menu.h"

uint32 straight_speed,turn_speed,init_servo;
uint32 speed_limit,left_limit,right_limit;
uint8 threshold_image = 0;
uint32 encoder_1,encoder_2,angle;
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
	.content = {"SKp","SKi","SKd","SMax_Iout","SMax_out"},
	.number = 5,
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
	.content = {"SKp","SKi","SKd","SMax_Iout","SMax_out"},
	.number = 5,
};

static uint16 cursor = 1;
uint8 currentIndex = 1;
uint8 pastIndex = 0;
menu_item *currentMenu=&main_Menu;

void moveup(void)
{
    if (cursor>1)
    cursor--;
    else
    cursor=currentMenu->number;
}

void movedown(void)
{
    if (cursor<currentMenu->number)
    cursor++;
    else
    cursor=1;
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
				currentMenu = &DataParameter_Menu;
				break;
			case PRAMETERSERVO:
				currentMenu = &DataParameter_Menu;
				break;
			case PRAMETERPID:
				currentMenu = &DataParameter_Menu;
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
    }
}

void getout(void)
{
	if (cursor > 10)
	{
		cursor = cursor % 10;
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
				currentMenu = &Pa_Speed_Menu;
				break;
			case PRAMETERSERVO:
				currentMenu = &Pa_Servo_Menu;
				break;
			case PRAMETERPID:
				currentMenu = &Pa_Pid_Menu;
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
    ips200_init(IPS200_TYPE);
}

void menu_display(void)
{
	ips200_clear();
	currentIndex = cursor % 10;
	pastIndex = cursor / 10;
	ips200_show_string(0,0,currentMenu->name);
	for (uint8 i = 0; i < currentMenu->number; i++)
	{
		if (currentIndex == (i+1))
		{
			ips200_show_string(1+i, 0, "->");
			ips200_show_string(1+i, 3, currentMenu->content[i]);
		}
		else
		{
			ips200_show_string(1+i, 3, currentMenu->content[i]);
		}
	}
	switch (pastIndex)
	{
		case CARGO:
			ips200_show_string(1, 17, "start");
			ips200_show_string(2, 19, "end");
			break;
		case PRAMETERSPEED:
			ips200_show_uint(1, 18, straight_speed, 4);
			ips200_show_uint(2, 18, turn_speed, 4);
			ips200_show_uint(3, 18, speed_limit, 4);
			break;
		case PRAMETERSERVO:
			ips200_show_uint(1, 18, init_servo, 4);
			ips200_show_uint(2, 18, left_limit, 4);
			ips200_show_uint(3, 18, right_limit, 4);
			break;
		case PRAMETERPID:
			ips200_show_float(1, 16, SpeedPidInitStruct.fKp, 3, 2);
			ips200_show_float(2, 16, SpeedPidInitStruct.fKi, 3, 2);
			ips200_show_float(3, 16, SpeedPidInitStruct.fKd, 3, 2);
			ips200_show_float(4, 16, SpeedPidInitStruct.fMax_Iout, 3, 2);
			ips200_show_float(5, 16, SpeedPidInitStruct.fMax_Out, 3, 2);
			break;
		case STATUSSPEED:
			ips200_show_uint(1, 18, straight_speed, 4);
			ips200_show_uint(2, 18, turn_speed, 4);
			ips200_show_uint(3, 18, speed_limit, 4);
			break;
		case STATUSSERVO:
			ips200_show_uint(1, 18, init_servo, 4);
			ips200_show_uint(2, 18, left_limit, 4);
			ips200_show_uint(3, 18, right_limit, 4);
			break;
		case STATUSPID:
			ips200_show_float(1, 16, SpeedPidInitStruct.fKp, 3, 2);
			ips200_show_float(2, 16, SpeedPidInitStruct.fKi, 3, 2);
			ips200_show_float(3, 16, SpeedPidInitStruct.fKd, 3, 2);
			ips200_show_float(4, 16, SpeedPidInitStruct.fMax_Iout, 3, 2);
			ips200_show_float(5, 16, SpeedPidInitStruct.fMax_Out, 3, 2);
			break;
		case IMAGE:
			ips200_show_uint(1, 18, threshold_image, 4);
			ips200_show_gray_image(2, 0, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, threshold_image);
			ips200_show_string(12, 0, "angle");
			ips200_show_string(13, 0, "encoder_1");
			ips200_show_string(14, 0, "encoder_2");
		    ips200_show_uint(12, 18, angle, 4);
			ips200_show_uint(13, 18, encoder_1, 4);
			ips200_show_uint(14, 18, encoder_2, 4);
		default:
			break;
			
	}
}