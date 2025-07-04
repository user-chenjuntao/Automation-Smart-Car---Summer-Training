#ifndef _image_test_h_
#define _image_test_h_


#include "zf_common_typedef.h"
#include "zf_common_font.h"
#include "zf_device_mt9v03x.h"
#include "zf_device_ips200.h"


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
extern uint8 original_image[image_h][image_w];
extern uint8 bin_image[image_h][image_w];//ͼ������

extern void image_process(void); //ֱ�����жϻ�ѭ������ô˳���Ϳ���ѭ��ִ����

#endif

