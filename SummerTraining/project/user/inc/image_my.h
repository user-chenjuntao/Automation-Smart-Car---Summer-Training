#ifndef _image_my_h_
#define _image_my_h_


#include "zf_common_typedef.h"
#include "zf_common_font.h"
#include "zf_device_mt9v03x.h"
#include "zf_device_ips200.h"
#include "math.h"


#define WHITE_PIXEL                255
#define BLACK_PIXEL                0
//�������ͺ͸�ʴ����ֵ����
#define THRESHOLD_MAX              255*5
#define THRESHOLD_MIN              255*2
//#define BORDERLEFT                 1
//#define BORDERRIGHT                254
#define MID_W                      93
// �����ڽ��յ����ֵ��������ܴ�ֵ��ϲ����ɸ���ʵ�ʵ�����
#define BREAKPOINT_MERGE_THRESHOLD 6


uint8 otsuThreshold_less(uint8 *image, uint16 col, uint16 row);
void image_postprocess(void);
void image_filter(uint8(*image)[MT9V03X_W]);
uint8 get_left_start(void);
uint8 get_right_start(void);
void research_longest_line(void);
void research_road(void);
void image_process(void);
void xieji(uint8 begin, uint8 end, float *xielv, float *jieju,uint8 y_begin,uint8 y_end);
void least_squares(uint8 begin, uint8 end, uint8 *border, float *xielv, float *jieju);
void crossing_add(uint8 num_d_l, uint8 num_u_l, uint8 num_d_r, uint8 num_u_r);
uint8 my_limit_min(uint8 num1, uint8 num2);
void straight_line_handle(void);
uint8 find_mid_line_weight(void);

extern uint8 final_mid_line;


#endif

