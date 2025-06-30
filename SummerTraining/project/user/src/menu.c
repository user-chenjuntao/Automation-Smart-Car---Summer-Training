#include "menu.h"

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
	.content = {"ImageRaw","ImageYZ","Variable"},
	.number = 3,
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
	
}