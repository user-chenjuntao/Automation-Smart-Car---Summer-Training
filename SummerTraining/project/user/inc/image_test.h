#ifndef _image_test_h_
#define _image_test_h_


#include "zf_common_typedef.h"
#include "zf_common_font.h"
#include "zf_device_mt9v03x.h"
#include "zf_device_ips200.h"
#include "math.h"


//��ɫ����
#define uesr_RED     RGB565_RED    //��ɫ
#define uesr_GREEN   RGB565_GREEN    //��ɫ
#define uesr_BLUE    RGB565_BLUE    //��ɫ


//�궨��
#define image_h	120//ͼ��߶�
#define image_w	188//ͼ����

#define white_pixel	255
#define black_pixel	0

#define bin_jump_num	1//�����ĵ���
#define border_max	image_w-2 //�߽����ֵ
#define border_min	1	//�߽���Сֵ	

extern void image_process(void); //ֱ�����жϻ�ѭ������ô˳���Ϳ���ѭ��ִ����

uint8 crossing_handle_l(void);
uint8 crossing_handle_r(void);
void least_squares(uint8 begin, uint8 end, uint8 *border, float *xielv, float *jieju);

void crossing_add(uint8 num_l, uint8 num_r);
int16 limit_a_b(int16 x, int a, int b);
int16 limit1(int16 x, int16 y);
#endif

