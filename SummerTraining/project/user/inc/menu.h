#ifndef _menu_h_
#define _menu_h_

#include "zf_common_typedef.h"
#include "zf_device_ips200.h"

#include "zf_common_font.h"

#define IPS200_TYPE     (IPS200_TYPE_PARALLEL8)                                 // 双排排针 并口两寸屏 这里宏定义填写 IPS200_TYPE_PARALLEL8

#define CARGO           1
#define DATAPARAMETER   2
#define DATASTATUS      3
#define IMAGE           4
#define CARGOSTART      11
#define CARGOEND        12
#define PRAMETERSPEED   21
#define PRAMETERSERVO   22
#define PRAMETERPID     23
#define STATUSSPEED     31
#define STATUSSERVO     32
#define STATUSPID       33
#define IMAGERAW        41
#define IMAGEYZ         42
#define VARIABLE        43
#define PASPEED         211
#define PASERVO         221
#define PAPID           231
#define STSPEED         311
#define STSERVO         321
#define STPID           331


typedef struct{
	
	char name[20];
	char *content[100];
	int number;
	
}menu_item;

void moveup(void);
void movedown(void);
void getin(void);
void getout(void);
void menu_init(void);
void menu_display(void);

#endif
