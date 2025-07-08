#include "image_my.h"

uint8 image_otsuThreshold_less;
uint8 PostProcessing_image[MT9V03X_H][MT9V03X_W];                                      //���ڴ�Ŷ�ֵ��������ͼ������
//uint8 left_point[2] = { 0 };//�������x��yֵ
//uint8 right_point_r[2] = { 0 };//�ұ�����x��yֵ


//uint8 original_image[MT9V03X_H][MT9V03X_W];
//////------------------------------------------------------------------------------------------------------------------
//////  @brief      ���һ���Ҷ�ͼ��
//////  @since      v1.0 
//////------------------------------------------------------------------------------------------------------------------
//void Get_image(uint8(*mt9v03x_image)[MT9V03X_W])
//{
//	#define use_num		1	//1���ǲ�ѹ����2����ѹ��һ��	
//	uint8 i = 0, j = 0, row = 0, line = 0;
//    for (i = 0; i < MT9V03X_H; i += use_num)          //
//    {
//        for (j = 0; j <MT9V03X_W; j += use_num)     //
//        {
//            original_image[row][line] = mt9v03x_image[i][j];//����Ĳ�����д�������ͷ�ɼ�����ͼ��
//			line++;
//        }
//        line = 0;
//        row++;
//    }
//}

//-------------------------------------------------------------------------------------
//ʹ�ô�򷨽��ж�ֵ������
//-------------------------------------------------------------------------------------
uint8 otsuThreshold_less(uint8 *image, uint16 width, uint16 height)
{
    #define GrayScale 256
    int pixel_count[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ���ظ���
    float pixel_account[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ�����ر���
    int i,j;   
    int pixel_sum = width * height;   //�����ص�
    uint8 threshold = 0;
    uint8* pixel_data = image;  //ָ���������ݵ�ָ��


    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixel_count[(int)pixel_data[i * width + j]]++;  //������ֵ��Ϊ����������±�

        }
    }
    float u = 0;  
    for (i = 0; i < GrayScale; i++)
    {
        pixel_account[i] = (float)pixel_count[i] / pixel_sum;   //����ÿ������������ͼ���еı���  
        u += i * pixel_account[i];  //��ƽ���Ҷ�
    }

      
    float variance_max=0.0;  //�����䷽��
	float variance;
    float w0 = 0, avgValue  = 0;  //w0 ǰ������ ��avgValue ǰ��ƽ���Ҷ�
    for(int i = 0; i < 256; i++)     //ÿһ��ѭ������һ��������䷽����� (����for����Ϊ1��)
    {  
        w0 += pixel_account[i];  //���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷ�������ռ����ͼ��ı�����ǰ������
        avgValue  += i * pixel_account[i];        
        if (w0 <= 0 || w0 >= 1)
			continue;
        variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //��䷽��   
        if(variance > variance_max) 
        {  
            variance_max = variance;  
            threshold = i;  
        }  
    } 

    return threshold;

}


void image_postprocess(void)
{
	uint8 i = 0, j = 0;
	image_otsuThreshold_less = otsuThreshold_less(*mt9v03x_image,MT9V03X_W,MT9V03X_H);
	for (i = 0; i < MT9V03X_H; i++)
	{
		for (j = 0; j < MT9V03X_W; j++)
		{
			PostProcessing_image[i][j] = (mt9v03x_image[i][j] > image_otsuThreshold_less) ? WHITE_PIXEL : BLACK_PIXEL;
//			if (mt9v03x_image[i][j] > image_otsuThreshold_less)
//			{
//				PostProcessing_image[i][j] = WHITE_PIXEL;
//			}
//			else
//			{
//				PostProcessing_image[i][j] = BLACK_PIXEL;
//			}
		}
	}
}

//Ŀ����Ϊ�˳�ȥһЩ��㣬��ֹ����Ԫ���жϳ�������
void image_filter(uint8(*image)[MT9V03X_W])//��̬ѧ�˲�������˵�������ͺ͸�ʴ��˼��
{
	uint16 i, j;
	uint32 num = 0;


	for (i = 1; i < MT9V03X_H - 1; i++)
	{
		for (j = 1; j < (MT9V03X_W - 1); j++)
		{
			//ͳ�ư˸����������ֵ
			num =
				image[i - 1][j - 1] + image[i - 1][j] + image[i - 1][j + 1]
				+ image[i][j - 1] + image[i][j + 1]
				+ image[i + 1][j - 1] + image[i + 1][j] + image[i + 1][j + 1];


			if (num >= THRESHOLD_MAX && image[i][j] == 0)
			{

				image[i][j] = WHITE_PIXEL;//��  ���Ը�ɺ궨�壬�������

			}
			if (num <= THRESHOLD_MIN && image[i][j] == 255)
			{

				image[i][j] = BLACK_PIXEL;//��

			}

		}
	}

}
 //&& PostProcessing_image[MT9V03X_H - 1][i - 2] == BLACK_PIXEL&& PostProcessing_image[MT9V03X_H - 1][i + 2] == BLACK_PIXEL
uint8 get_left_start(void)
{
	uint8 i = 0;
	for(i = MT9V03X_W/2; i > 1; i--)
	{
		if ((PostProcessing_image[MT9V03X_H - 1][i] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][i - 1] == BLACK_PIXEL) || i == 2)
		{
			return i;
		}
	}
	return 2;
}

uint8 get_right_start(void)
{
	uint8 i = 0;
	for(i = MT9V03X_W/2; i < MT9V03X_W - 2; i++)
	{
		if ((PostProcessing_image[MT9V03X_H - 1][i] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][i + 1] == BLACK_PIXEL ) || i == 185)
		{
			return i;
		}
	}
	return 185;
}

uint8 left_point,right_point;
uint8 longest_line_number;           //��¼����е���λ�ã�x������
uint8 line;                          //��¼����еĳ��ȣ�0�������y������
void research_longest_line(void)
{
	uint8 i,j;
	uint8 stopflag = 0;
	uint8 left_point = 0;
	uint8 right_point = 0;
	longest_line_number = MT9V03X_W/2;
	line = 119;
	left_point = get_left_start();
	right_point = get_right_start();
	stopflag = 0;
	for (i = left_point; i < right_point; i++)
	{
		for (j = MT9V03X_H - 1; j > 0; j--)
		{
			if (j == 1)
			{
				longest_line_number = i;
				line = j;
				stopflag = 1;
//				break;
			}
			else if ((PostProcessing_image[j][i] == WHITE_PIXEL && PostProcessing_image[j - 1][i] == BLACK_PIXEL))
			{
//				dead_line[i] = j;
				if (j < line)
				{
					line = j;
					longest_line_number = i;
//					break;
				}
			}
			
		}
//		if (stopflag)
//		{
//			break;
//		}
	}
}
uint8 road_left[120] = {0};         //��¼x��������
uint8 break_point_l[6][2] ={{0}};
uint8 break_num_left;
uint8 break_point_r[6][2] = {{0}};
uint8 break_num_right;
uint8 road_right[120] = {0};       
uint8 left_flag_break;
uint8 right_flag_break;
void research_road(void)
{
	uint8 i,j;
//	uint8 left_num = 0;
//	uint8 right_num = 0;
	left_flag_break = 0;
	right_flag_break = 0;
	break_num_left = 0;
	break_num_right = 0; //&& PostProcessing_image[i][j-2] == BLACK_PIXEL// && PostProcessing_image[i][j+2] == BLACK_PIXEL
	for (i = line; i < MT9V03X_H; i++)
	{
		for (j = 60; j > 1; j--)       //��
		{
//			left_num++;
			if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j-1] == BLACK_PIXEL) || j == 2)
			{
				road_left[i] = j;
				if (left_flag_break)
				{
					if (road_left[i] - road_left[i-1] >= 5)
					{
						break_point_l[break_num_left][0] = i;   //��һ��
						break_point_l[break_num_left][1] = road_left[i];   //��һ��
						break_num_left++;
					}
					else if (road_left[i] - road_left[i-1] <= -5)
					{
						break_point_l[break_num_left][0] = i - 1;
						break_point_l[break_num_left][1] = road_left[i -1];
						break_num_left++;
					}
				}
				left_flag_break++;
				if (left_flag_break >1)
					left_flag_break = 1;
				break;
			}
		}
		for (j = 60; j < MT9V03X_W - 2; j++)
		{
//			right_num++;
			if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j+1] == BLACK_PIXEL) || j == 185)
			{
				road_right[i] = j;
				if (right_flag_break)
				{
					if (road_right[i] - road_right[i-1] >= 5)
					{
						break_point_r[break_num_right][0] = i - 1;
						break_point_r[break_num_right][1] = road_right[i -1];
						break_num_right++;
					}
					else if (road_right[i] - road_right[i-1] <= -5)
					{
						break_point_r[break_num_right][0] = i;
						break_point_r[break_num_right][1] = road_right[i];
						break_num_right++;
					}
				}
				right_flag_break++;
				if (right_flag_break >1)
					right_flag_break = 1;
				break;
			}
		}
	}
}










uint8 center_line[120];
uint16 center_value = 94;
//uint8 (*image_progress)[MT9V03X_W]
void image_process(void)
{
	uint16 sum;
	sum = 0;
	image_postprocess();
	research_longest_line();
	research_road();
	crossing_add(break_num_left,break_num_right);
	
	ips200_show_gray_image(0, 50, PostProcessing_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
	for (uint8 i = 0; i < MT9V03X_H; i++)
	{
		ips200_draw_point(longest_line_number, i+50,RGB565_YELLOW);
	}
	for (uint8 i = line; i < MT9V03X_H; i++)
	{
		ips200_draw_point(road_left[i]+2, i+50,RGB565_GREEN);
		ips200_draw_point(road_right[i]-2, i+50,RGB565_BLUE);
		center_line[i] = (road_left[i] + road_right[i]) >> 1;//������
		ips200_draw_point(center_line[i]-2, i+50,RGB565_RED);
	}
	for (uint8 i = MT9V03X_H - 17; i < MT9V03X_H - 1; i++)
	{
		if (center_line[i])
			sum += center_line[i];
	}
	center_value = sum / 16;
	break_num_left = 0;
	break_num_right = 0;
	memset(break_point_l, 0, sizeof(break_point_l));
	memset(break_point_r, 0, sizeof(break_point_r));
	
	memset(road_left, 0, sizeof(road_left));
	memset(road_right, 0, sizeof(road_right));
	
	memset(center_line, 0, sizeof(center_line));
		
}







//------------------------------------------------------------------------------------------------------------------
//�������ƣ�uint8 crossing_handle(void)
//����˵������С���˷�����
//����˵������
//�������أ�
//------------------------------------------------------------------------------------------------------------------

void least_squares(uint8 begin, uint8 end, uint8 *border, float *xielv, float *jieju)
{
	float sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
	int16 i = 0;
	*xielv = 0;
	*jieju = 0;

	for (i = begin; i < end; i++)
	{
		sum_x += i;
		sum_y += border[i];
		sum_xy += i * (border[i]);
		sum_x2 += i * i;

	}
	if ((end - begin)*sum_x2 - sum_x * sum_x) //�жϳ����Ƿ�Ϊ��
	{
		*xielv = ((end - begin)*sum_xy - sum_x * sum_y) / ((end - begin)*sum_x2 - sum_x * sum_x);

	}
	*jieju = (sum_y - (*xielv) * sum_x) / (end - begin);
}
void xieji(uint8 begin, uint8 end, float *xielv, float *jieju,uint8 y_begin,uint8 y_end)
{
	*xielv = 0;
	*jieju = 0;

		*xielv = (y_end - y_begin) / end - begin;
		*jieju = y_begin - (*xielv) * begin;
}

//void loss_line(uint16 total_num_l, uint16 total_num_r)
//{
//	uint8 i;
//	uint8 num_l,num_r;
//	num_l = 0;
//	num_r = 0;
//	for (i = 1; i < total_num_l; i++)
//	{
//		if (dir_l[i] == 4)
//		{
//			num_l++;
//		}
//	}
//	for (i = 0; i < total_num_r; i++)
//	{
//		if (dir_r[i] == 4)
//		{
//			num_r++;
//		}
//	}
//	if (4*num_l > 3*total_num_l)
//	{
//		
//		//��Ϊ����߶�ʧ
//	}
//}

void crossing_add(uint8 num_l, uint8 num_r)
{
	float xielv_l,jieju_l;
	float xielv_r,jieju_r;
	uint16 i =0;
//	if (num_l == 0 && num_r ==0)
//	{
//		
//	}
	if (num_l == 1)
	{
		least_squares(break_point_l[0][0]-15, break_point_l[0][0] - 5, road_left, &xielv_l, &jieju_l);
		for (i = break_point_l[0][0] - 5; i < MT9V03X_H - 1; i++)
		{
			road_left[i] = xielv_l*i + jieju_l;
			if (road_left[i] <= 1)
			{
				road_left[i] = 1;
			}
		}
	}
	else if (num_l == 2)
	{
		xieji(break_point_l[1][0] , break_point_l[0][0], &xielv_l, &jieju_l, break_point_l[1][1], break_point_l[0][1]);
		for (i = break_point_l[1][0]; i < break_point_l[0][0]; i++)
		{
			road_left[i] = xielv_l*i + jieju_l;
			if (road_left[i] <= 1)
			{
				road_left[i] = 1;
			}
		}
	}
	if (num_r == 1)
	{
		least_squares(break_point_r[0][0]-15, break_point_r[0][0] - 5, road_right, &xielv_r, &jieju_r);
		for (i = break_point_r[0][0] - 5; i < MT9V03X_H - 1; i++)
		{
			road_right[i] = xielv_r*i + jieju_r;
			if (road_right[i] <= 1)
			{
				road_right[i] = 1;
			}
		}
	}
	else if (num_r == 2)
	{
		xieji(break_point_r[1][0] , break_point_r[0][0], &xielv_r, &jieju_r, break_point_r[1][1],break_point_r[0][1]);
		for (i = break_point_r[1][0]; i < break_point_r[0][0]; i++)
		{
			road_right[i] = xielv_r*i + jieju_r;
			if (road_right[i] <= 1)
			{
				road_right[i] = 1;
			}
		}
	}
}
