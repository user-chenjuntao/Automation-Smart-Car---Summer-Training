#ifndef _servo_pid_h_
#define _servo_pid_h_

#include "zf_common_typedef.h"
#include "image_my.h"
#include "math.h"
#include "config.h"

//#define MID_W             93
#define CONTROL_MEDIAN    598


extern uint8 duty_pwm;
extern uint8 straight_flag;

void servo_pid_test(void);


#endif
