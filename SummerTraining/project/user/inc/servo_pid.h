#ifndef _servo_pid_h_
#define _servo_pid_h_

#include "math.h"
#include "image_my.h"
#include "zf_common_typedef.h"

#include "config.h"
#include "zf_device_imu963ra.h"

//#define MID_W             93
#define CONTROL_MEDIAN    598


extern uint8 straight_flag;
extern float Kp_value;
extern float Kp_base;

void servo_pid_test(void);
void dynamic_pid_value_set(void);


#endif
