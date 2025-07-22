#include "image_my.h"

uint8 image_otsuThreshold_less;
uint8 PostProcessing_image[MT9V03X_H][MT9V03X_W];                                      //���ڴ�Ŷ�ֵ��������ͼ������
uint8 Left_Lost_Time = 0;                                                              //��¼��߽���Ч��
uint8 Right_Lost_Time = 0;                                                             //��¼�ұ߽���Ч��
uint8 Zebra_stop_flag =0;
float line_error = 0;
uint8 low_high_choose = 0;
uint8 huandao_num_flag = 0;
uint8 crossing_flag_help = 0;


/*********************************************************
** //ͼ��Ȩ������  //Ȩ��Խ���ϣ���ת��Խ��
*********************************************************/
uint8 mid_weight_list[120] = 
{
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,6,6,
    6,6,6,6,8,8,8,9,9,9,
    9,10,10,10,11,12,13,14,15,16,
    17,18,20,24,20,20,19,19,18,17,
    16,15,14,13,12,11,10,9,8,7,
    6,6,6,6,6,6,6,6,6,6,
    6,5,4,3,2,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
};

uint8 low_weight_list[120] = 
{
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    1,1,1,2,2,2,2,3,3,3,
    4,4,5,6,7,8,9,10,11,12,
    13,14,15,16,17,18,17,16,15,14,
    13,12,11,10,9,8,7,6,6,6,
    6,6,5,5,5,4,4,3,2,1,
    1,1,1,1,1,1,1,1,1,1,
};

float final_mid_line = MID_W;   // �������������ֵ
float last_mid_line = MID_W;    // �ϴ�����ֵ

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
		}
	}
	image_filter(PostProcessing_image);
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

uint8 car_stop_flag = 0;

void car_stop(void)
{
	uint8 i, j;
	uint16 black_num = 0;
	
	for (i = MT9V03X_H - 1; i > MT9V03X_H - 6; i--)
	{
		for (j = 1; j < MT9V03X_W; j++)
		{
			if (PostProcessing_image[i][j] == BLACK_PIXEL)
				black_num++;
		}
	}
	if (black_num > 750)
	{
		car_stop_flag = 1;
	}
	else
	{
		car_stop_flag = 0;
	}
}

uint8 research_start_point(void)
{
	if (PostProcessing_image[MT9V03X_H - 1][MT9V03X_W/2] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][MT9V03X_W/2-1] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][MT9V03X_W/2+1] == WHITE_PIXEL)
	{
		return 2;
	}
	else if (PostProcessing_image[MT9V03X_H - 1][MT9V03X_W/4] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][MT9V03X_W/4-1] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][MT9V03X_W/4+1] == WHITE_PIXEL)
	{
		return 1;
	}
	else if (PostProcessing_image[MT9V03X_H - 1][MT9V03X_W/4*3] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][MT9V03X_W/4*3-1] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][MT9V03X_W/4*3+1] == WHITE_PIXEL)
	{
		return 3;
	}
	else
	{
		return 2;
	}
}
	
	

 //&& PostProcessing_image[MT9V03X_H - 1][i - 2] == BLACK_PIXEL&& PostProcessing_image[MT9V03X_H - 1][i + 2] == BLACK_PIXEL
uint8 get_left_start(uint8 num)
{
	uint8 i = 0;
	for(i = MT9V03X_W*(num/4.0f); i > 1; i--)
	{
		if ((PostProcessing_image[MT9V03X_H - 1][i] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][i - 1] == BLACK_PIXEL) || i == 2)
		{
			return i;
		}
	}
	return 2;
}


uint8 left_point,right_point;
uint8 get_right_start(uint8 num)
{
	uint8 i = 0;
	for(i = MT9V03X_W*(num/4.0f); i < MT9V03X_W - 2; i++)
	{
		if ((PostProcessing_image[MT9V03X_H - 1][i] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][i + 1] == BLACK_PIXEL ) || i == 185)
		{
			return i;
		}
	}
	return 185;
}





uint8 longest_line_number;           //��¼����е���λ�ã�x������
uint8 line;                          //��¼����еĳ��ȣ�0�������y������

void research_longest_line(void)
{
    uint8 i, j;
    uint8 longest_length = 0;  // ��¼���ɫ�еĳ���
    uint8 pl = research_start_point();
    left_point = get_left_start(pl);
    right_point = get_right_start(pl);
    
    longest_line_number = MT9V03X_W/2;  // Ĭ��ֵ
    line = 119;  // Ĭ��ֵ��119��һ������ѡ�񣬴���ϳ��ĳ��ȣ�
    
    // ȷ�����ұ߽����
    if (left_point >= right_point)
	{
		// ���ð�ȫĬ��ֵ����Ӵ�����
		longest_line_number = MT9V03X_W / 2;
		line = 60;
		return;
	}
		
    
    for (i = left_point; i < right_point; i++)
    {
        uint8 current_length = 0;  // ��ǰ�еİ�ɫ����
        uint8 start_row = 0;       // ��ǰ�а�ɫ�������ʼ��
//        uint8 bottom_continuous_length = 0;
		
		
        // ���ҵ�ǰ�а�ɫ�������ʼ��
        for (j = MT9V03X_H - 1; j > 0; j--)
        {
//			if (PostProcessing_image[j][i] == WHITE_PIXEL)
//			{
//				bottom_continuous_length++;
//			}
            if (PostProcessing_image[(MT9V03X_H-1 + j)/2][i] == WHITE_PIXEL && 
				PostProcessing_image[MT9V03X_H - 1][i] == WHITE_PIXEL && 
				PostProcessing_image[j][i] == WHITE_PIXEL && 
                PostProcessing_image[j - 1][i] == BLACK_PIXEL)
            {
				
//				if (0.8*(MT9V03X_H - start_row) <= bottom_continuous_length)
//				{
					start_row = j;
					current_length = MT9V03X_H - start_row;  // �����ɫ�еĳ���
					break;
//				}
                
            }
        }
        
        // ���û���ҵ��ڰױ߽磬�����ж��ǰ�ɫ && bottom_continuous_length>0.8*MT9V03X_H
        if (j == 0 && PostProcessing_image[0][i] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H/2][i] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H-2][i] == WHITE_PIXEL)
        {
            start_row = 2;
            current_length = MT9V03X_H - 2;  // ���ж��ǰ�ɫ
        }
        
        // �������ɫ����Ϣ
        if (current_length > longest_length)
        {
            longest_length = current_length;
            longest_line_number = i;
            line = start_row;  // ��¼��ɫ�������ʼ��
        }
    }
	if (longest_line_number >= RIGHT_MAX)
	{
		longest_line_number = RIGHT_MAX;
	}
	else if (longest_line_number <= LEFT_MAX)
	{
		longest_line_number = LEFT_MAX;
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
	left_flag_break = 0;
	right_flag_break = 0;
	break_num_left = 0;
	break_num_right = 0; ////
	for (i = MT9V03X_H - 1; i >= line; i--)
	{
		for (j = longest_line_number; j > 1; j--)       //��
		{
			if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j-1] == BLACK_PIXEL && PostProcessing_image[i][j-2] == BLACK_PIXEL) || j == 2)
			{
				
				road_left[i] = j;
				if (road_left[i] <= 2)
				{
					Left_Lost_Time++;
				}
//				if (left_flag_break)
//				{
//					if (i >= 2 && road_left[i] - road_left[i-1] >= 38 && road_left[i] - road_left[i-2] >= 38 && break_num_left < 6)
//					{
//						break_point_l[break_num_left][0] = i;   //��һ��
//						break_point_l[break_num_left][1] = road_left[i];   //��һ��
//						break_num_left++;
//					}
//					else if (i >= 2 && road_left[i] - road_left[i-1] <= -38 && road_left[i] - road_left[i-2] <= -38 && break_num_left < 6)
//					{
//						break_point_l[break_num_left][0] = i - 1;
//						break_point_l[break_num_left][1] = road_left[i -1];
//						break_num_left++;
//					}
//				}
				left_flag_break++;
				if (left_flag_break >1)
					left_flag_break = 1;
				break;
			}
		}
		for (j = longest_line_number; j < MT9V03X_W - 2; j++)
		{
			if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j+1] == BLACK_PIXEL && PostProcessing_image[i][j+2] == BLACK_PIXEL) || j == 185)
			{
				road_right[i] = j;
				if (road_right[i] >= 185)
				{
					Right_Lost_Time++;
				}
//				if (right_flag_break)
//				{
//					if (i >= 2 && road_right[i] - road_right[i-1] >= 8 && road_right[i] - road_right[i-2] >= 8 && break_num_right < 6)
//					{
//						break_point_r[break_num_right][0] = i - 1;
//						break_point_r[break_num_right][1] = road_right[i -1];
//						break_num_right++;
//					}
//					else if (i >= 2 && road_right[i] - road_right[i-1] <= -8 && road_right[i] - road_right[i-2] <= -8 && break_num_right < 6)
//					{
//						break_point_r[break_num_right][0] = i;
//						break_point_r[break_num_right][1] = road_right[i];
//						break_num_right++;
//					}
//				}
				right_flag_break++;
				if (right_flag_break >1)
					right_flag_break = 1;
				break;
			}
		}
	}
//	merge_close_breakpoints();
}



// �ϲ���յ���ڽ��յ�
void merge_left_breakpoints(void)
{
    if (break_num_left <= 1) return;  // ����2���յ㣬����ϲ�

    uint8 temp_l[6][2] = {{0}};  // ��ʱ�洢�ϲ������յ�
    uint8 temp_num = 0;          // �ϲ������յ�����

    // ������һ���յ���Ϊ��ʼֵ
    temp_l[temp_num][0] = break_point_l[0][0];
    temp_l[temp_num][1] = break_point_l[0][1];
    temp_num++;

    // ����ʣ��յ㣬�ж��Ƿ���Ҫ�ϲ�
    for (uint8 i = 1; i < break_num_left; i++)
    {
        // ���㵱ǰ�յ�����һ�������Ĺյ��������
        uint8 row_diff = break_point_l[i][0] - temp_l[temp_num-1][0];

        if (row_diff > BREAKPOINT_MERGE_THRESHOLD)
        {
            // ���������ֵ��������ǰ�յ�
            temp_l[temp_num][0] = break_point_l[i][0];
            temp_l[temp_num][1] = break_point_l[i][1];
            temp_num++;
            if (temp_num >= 6) break;  // �����������������
        }
        // �������������ֵ������Ϊ�ڽ��յ㣬���������ϲ�����һ����
    }

    // ������յ����������
    memcpy(break_point_l, temp_l, sizeof(temp_l));
    break_num_left = temp_num;
}

// �ϲ��ҹյ���ڽ��յ㣨�߼�����յ���ȫһ�£�
void merge_right_breakpoints(void)
{
    if (break_num_right <= 1) return;  // ����2���յ㣬����ϲ�

    uint8 temp_r[6][2] = {{0}};  // ��ʱ�洢�ϲ�����ҹյ�
    uint8 temp_num = 0;          // �ϲ�����ҹյ�����

    // ������һ���յ���Ϊ��ʼֵ
    temp_r[temp_num][0] = break_point_r[0][0];
    temp_r[temp_num][1] = break_point_r[0][1];
    temp_num++;

    // ����ʣ��յ㣬�ж��Ƿ���Ҫ�ϲ�
    for (uint8 i = 1; i < break_num_right; i++)
    {
        // ���㵱ǰ�յ�����һ�������Ĺյ��������
        uint8 row_diff = break_point_r[i][0] - temp_r[temp_num-1][0];

        if (row_diff > BREAKPOINT_MERGE_THRESHOLD)
        {
            // ���������ֵ��������ǰ�յ�
            temp_r[temp_num][0] = break_point_r[i][0];
            temp_r[temp_num][1] = break_point_r[i][1];
            temp_num++;
            if (temp_num >= 6) break;  // �����������������
        }
        // �������������ֵ������Ϊ�ڽ��յ㣬���������ϲ�����һ����
    }

    // �����ҹյ����������
    memcpy(break_point_r, temp_r, sizeof(temp_r));
    break_num_right = temp_num;
}

// ͳһ�������ҹյ�ϲ�����
void merge_close_breakpoints(void)
{
    merge_left_breakpoints();
    merge_right_breakpoints();
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���½ǵ���
  @param     ��ʼ�У���ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     left_down_guai[0]=Find_Left_Down_Point(MT9V03X_H-1,20);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Down_Point(int start,int end)//�����½ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i,t;
    int left_down_line=0;
    if(Left_Lost_Time>=0.8*MT9V03X_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
       return left_down_line;
    if(start<end)//--���ʣ�Ҫ��֤start>end
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=MT9V03X_H-1-5)//����5������5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
        start=MT9V03X_H-1-5;//��һ���棬���жϵ�i��ʱ������ʵ�i+3��i-4�У���ֹԽ��
    if(end<=line)
        end=line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(left_down_line==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(road_left[i]-road_left[i+1])<=5&&//�ǵ����ֵ���Ը���
           abs(road_left[i+1]-road_left[i+2])<=5&&  
           abs(road_left[i+2]-road_left[i+3])<=5&&
              (road_left[i]-road_left[i-2])>=5&&
              (road_left[i]-road_left[i-3])>=10&&
              (road_left[i]-road_left[i-4])>=10)
        {
            left_down_line=i;//��ȡ��������
            break;
        }
    }
    return left_down_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���Ͻǵ���
  @param     ��ʼ�У���ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     left_down_guai[0]=Find_Left_Down_Point(MT9V03X_H-1,20);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Up_Point(int start,int end,uint8 mode)//�����½ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i,t;
    int left_up_line=0;
    if(Left_Lost_Time>=0.8*MT9V03X_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
       return left_up_line;
	if (mode == 0)
	{
		if(start<end)//--���ʣ�Ҫ��֤start>end
		{
			t=start;
			start=end;
			end=t;
		}
		if(start>=MT9V03X_H-1-5)//����5������5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
			start=MT9V03X_H-1-5;//��һ���棬���жϵ�i��ʱ������ʵ�i+3��i-4�У���ֹԽ��
		if(end<=line)
			end=line;
		if(end<=5)
		   end=5;
		for(i=start;i>=end;i--)
		{
			if(left_up_line==0&&//ֻ�ҵ�һ�����������ĵ�
			   abs(road_left[i]-road_left[i-1])<=5&&//�ǵ����ֵ���Ը���
			   abs(road_left[i-1]-road_left[i-2])<=5&&  
			   abs(road_left[i-2]-road_left[i-3])<=5&&
				  (road_left[i]-road_left[i+2])>=8&&
				  (road_left[i]-road_left[i+3])>=15&&
				  (road_left[i]-road_left[i+4])>=15)
			{
				left_up_line=i;//��ȡ��������
				break;
			}
		}
		
	}
	else if (mode == 1)
	{
		if(start>end)//--���ʣ�Ҫ��֤start>end
		{
			t=start;
			start=end;
			end=t;
		}
		if(end>=MT9V03X_H-1-5)//����5������5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
			end=MT9V03X_H-1-5;//��һ���棬���жϵ�i��ʱ������ʵ�i+3��i-4�У���ֹԽ��
		if(start<=line)
			start=line;
		if(start<=5)
		   start=5;
		for(i=start;i<=end;i++)
		{
			if(left_up_line==0&&//ֻ�ҵ�һ�����������ĵ�
			   abs(road_left[i]-road_left[i-1])<=5&&//�ǵ����ֵ���Ը���
			   abs(road_left[i-1]-road_left[i-2])<=5&&  
			   abs(road_left[i-2]-road_left[i-3])<=5&&
				  (road_left[i]-road_left[i+2])>=8&&
				  (road_left[i]-road_left[i+3])>=15&&
				  (road_left[i]-road_left[i+4])>=15)
			{
				left_up_line=i;//��ȡ��������
				break;
			}
		}

	}
    return left_up_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���½ǵ���
  @param     ��ʼ�У���ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     left_down_guai[0]=Find_Left_Down_Point(MT9V03X_H-1,20);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Down_Point(int start,int end)//�����½ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i,t;
    int right_down_line=0;
    if(Right_Lost_Time>=0.8*MT9V03X_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
       return right_down_line;
    if(start<end)//--���ʣ�Ҫ��֤start>end
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=MT9V03X_H-1-5)//����5������5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
        start=MT9V03X_H-1-5;//��һ���棬���жϵ�i��ʱ������ʵ�i+3��i-4�У���ֹԽ��
    if(end<=line)
        end=line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(right_down_line==0&&//ֻ�ҵ�һ�����������ĵ�
           abs(road_right[i]-road_right[i+1])<=5&&//�ǵ����ֵ���Ը���
           abs(road_right[i+1]-road_right[i+2])<=5&&  
           abs(road_right[i+2]-road_right[i+3])<=5&&
              (road_right[i-2]-road_right[i])>=5&&
              (road_right[i-3]-road_right[i])>=10&&
              (road_right[i-4]-road_right[i])>=10)
        {
            right_down_line=i;//��ȡ��������
            break;
        }
    }
    return right_down_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     ���Ͻǵ���
  @param     ��ʼ�У���ֹ��
  @return    ���ؽǵ����ڵ��������Ҳ�������0
  Sample     left_down_guai[0]=Find_Left_Down_Point(MT9V03X_H-1,20);
  @note      �ǵ�����ֵ�ɸ���ʵ��ֵ����
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Up_Point(int start,int end,uint8 mode)//�����½ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i,t;
    int right_up_line=0;
    if(Right_Lost_Time>=0.8*MT9V03X_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
       return right_up_line;
	if (mode == 0)
	{
		if(start<end)//--���ʣ�Ҫ��֤start>end
		{
			t=start;
			start=end;
			end=t;
		}
		if(start>=MT9V03X_H-1-5)//����5������5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
			start=MT9V03X_H-1-5;//��һ���棬���жϵ�i��ʱ������ʵ�i+3��i-4�У���ֹԽ��
		if(end<=line)
			end=line;
		if(end<=5)
		   end=5;
		for(i=start;i>=end;i--)
		{
			if(right_up_line==0&&//ֻ�ҵ�һ�����������ĵ�
			   abs(road_right[i]-road_right[i-1])<=5&&//�ǵ����ֵ���Ը���
			   abs(road_right[i-1]-road_right[i-2])<=5&&  
			   abs(road_right[i-2]-road_right[i-3])<=5&&
				  (road_right[i+2]-road_right[i])>=8&&
				  (road_right[i+3]-road_right[i])>=15&&
				  (road_right[i+4]-road_right[i])>=15)
			{
				right_up_line=i;//��ȡ��������
				break;
			}
		}
	}
	else if (mode == 1)
	{
		if(start>end)//--���ʣ�Ҫ��֤start>end
		{
			t=start;
			start=end;
			end=t;
		}
		if(end>=MT9V03X_H-1-5)//����5������5�����ݲ��ȶ���������Ϊ�߽�����жϣ�����
			end=MT9V03X_H-1-5;//��һ���棬���жϵ�i��ʱ������ʵ�i+3��i-4�У���ֹԽ��
		if(start<=line)
			start=line;
		if(start<=5)
		   start=5;
		for(i=start;i<=end;i++)
		{
			if(right_up_line==0&&//ֻ�ҵ�һ�����������ĵ�
			   abs(road_right[i]-road_right[i-1])<=5&&//�ǵ����ֵ���Ը���
			   abs(road_right[i-1]-road_right[i-2])<=5&&  
			   abs(road_right[i-2]-road_right[i-3])<=5&&
				  (road_right[i+2]-road_right[i])>=8&&
				  (road_right[i+3]-road_right[i])>=15&&
				  (road_right[i+4]-road_right[i])>=15)
			{
				right_up_line=i;//��ȡ��������
				break;
			}
		}
	}
    return right_up_line;
}




uint8 huandao_flag = 0;

uint8 l_d_num = 0;
uint8 l_u_num = 0;
uint8 r_d_num = 0;
uint8 r_u_num = 0;
uint8 center_line[120];
int num_123;
uint8 see_flag = 0;
void image_process(void)
{

	uint16 value_count;
	
	image_postprocess();
	car_stop();
	Zebra_crossing_handle();
	research_longest_line();
	research_road();
	l_d_num = Find_Left_Down_Point(MT9V03X_H-1,line);
	l_u_num = Find_Left_Up_Point(MT9V03X_H-1,line,0);
	r_d_num = Find_Right_Down_Point(MT9V03X_H-1,line);
	r_u_num = Find_Right_Up_Point(MT9V03X_H-1,line,0);
	if (l_d_num > 0 && l_d_num < l_u_num)
	{
		l_d_num = 0;
	}
	if (r_d_num > 0 && r_d_num < r_u_num)
	{
		r_d_num = 0;
	}
	
//	num_123 = Continuity_Change_Left(MT9V03X_H, line - 2);
	
	
	
	
	crossing_add(l_d_num,l_u_num,r_d_num,r_u_num);
	
	if (crossing_flag_help == 0)
	{
		yuanhuan_in_handle();
	}
	
	
	
	
	
	
//	ips200_show_gray_image(0, 50, PostProcessing_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
//	for (uint8 i = 0; i < MT9V03X_H; i++)
//	{
//		ips200_draw_point(longest_line_number, i+50,RGB565_YELLOW);
//	}
	for (uint8 i = line; i < MT9V03X_H; i++)
	{
		center_line[i] = (road_left[i] + road_right[i]) >> 1;//������
	}
//	for (uint8 i = line; i < MT9V03X_H; i++)
//	{
//		ips200_draw_point(road_left[i], i+50,RGB565_GREEN);
//		ips200_draw_point(road_right[i], i+50,RGB565_BLUE);
//		center_line[i] = (road_left[i] + road_right[i]) >> 1;//������
//		ips200_draw_point(center_line[i], i+50,RGB565_RED);
//	}

//	ips200_draw_point(road_left[l_d_num],l_d_num+50,RGB565_RED);
//	ips200_draw_point(road_left[l_u_num],l_u_num+50,RGB565_RED);
//	ips200_draw_point(road_right[r_d_num],r_d_num+50,RGB565_RED);
//	ips200_draw_point(road_right[r_u_num],r_u_num+50,RGB565_RED);

//	ips200_show_uint(0,208,l_d_num,3);
//	ips200_show_uint(30,208,l_u_num,3);
//	ips200_show_uint(60,208,r_d_num,3);
//	ips200_show_uint(90,208,r_u_num,3);


	final_mid_line = find_mid_line_weight();
	line_error = final_mid_line - MID_W;
	if (fabs(line_error) <= 4)
	{
		line_error = 0.5*line_error;
	}
	else if (fabs(line_error) > 4 && fabs(line_error) < 10)
	{
		line_error = line_error;
	}
	else
	{
		line_error = line_error*1.2;
	}
//	else if (fabs(line_error) > 9)
//	{
//		if (line_error >0)
//		{
//			line_error = line_error*line_error/10;
//		}
//		else
//		{
//			line_error = -line_error*line_error/10;
//		}
//	}
//	ips200_show_uint(120,208,final_mid_line,3);

	



//	break_num_left = 0;
//	break_num_right = 0;
//	Left_Lost_Time = 0;
//	Right_Lost_Time = 0;

//	
//	memset(road_left, 0, sizeof(road_left));
//	memset(road_right, 0, sizeof(road_right));
//	
//	memset(center_line, 0, sizeof(center_line));

		
}

uint8 left_test = 0;
uint8 right_test = 0;

void image_show(void)
{
	ips200_show_gray_image(0, 50, PostProcessing_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
	for (uint8 i = 0; i < MT9V03X_H; i++)
	{
		ips200_draw_point(longest_line_number, i+50,RGB565_YELLOW);
	}
	for (uint8 i = line; i < MT9V03X_H; i++)
	{
		ips200_draw_point(road_left[i], i+50,RGB565_GREEN);
		ips200_draw_point(road_right[i], i+50,RGB565_BLUE);
		ips200_draw_point(center_line[i], i+50,RGB565_RED);
	}
	ips200_draw_point(road_left[l_d_num],l_d_num+50,RGB565_RED);
	ips200_draw_point(road_left[l_u_num],l_u_num+50,RGB565_RED);
	ips200_draw_point(road_right[r_d_num],r_d_num+50,RGB565_RED);
	ips200_draw_point(road_right[r_u_num],r_u_num+50,RGB565_RED);
	
	ips200_show_uint(0,208,l_d_num,3);
	ips200_show_uint(30,208,l_u_num,3);
	ips200_show_uint(60,208,r_d_num,3);
	ips200_show_uint(90,208,r_u_num,3);
	
	ips200_show_uint(120,208,final_mid_line,3);
//	ips200_show_uint(150,208,straight_flag,1);
	
//	ips200_show_int(160,208,num_123,3);
	ips200_show_uint(150,208,left_test,1);
	ips200_show_uint(170,208,right_test,1);
//	ips200_show_uint(170,208,qiebuxian,3);
	ips200_show_uint(200,208,huandao_flag,1);
}


void image_data_clear(void)
{
	break_num_left = 0;
	break_num_right = 0;
	Left_Lost_Time = 0;
	Right_Lost_Time = 0;
//	huandao_flag = 0;
	crossing_flag_help = 0;
	Zebra_stop_flag = 0;
	
	memset(road_left, 0, sizeof(road_left));
	memset(road_right, 0, sizeof(road_right));
	
	memset(center_line, 0, sizeof(center_line));
}


void huandao_clear(void)
{
	huandao_flag = 0;
	huandao_num_flag = 0;
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
void xieji(uint8 begin, uint8 end, uint8 y_begin, uint8 y_end,uint8 arr[])
{
	uint8 i = 0;
	float xielv;
	float jieju;
    if (end == begin)  // ����������
    {
        xielv = 0;
        jieju = y_begin;
		return;
    }
    
    // ����б�ʺͽؾ࣬ʹ�ø���������
    xielv = (float)(y_end - y_begin) / (float)(end - begin);
    jieju = y_begin - (xielv) * begin;
	
	for (i = begin; i < end; i++)
    {
		arr[i] = (uint8)(xielv * i + jieju + 0.5f);
        if (arr[i] <= 1)
			arr[i] = 1;
        else if (arr[i] >= MT9V03X_W - 2)
            arr[i] = MT9V03X_W - 2;
    }
}

//ʮ�ֲ���
void crossing_add(uint8 num_d_l, uint8 num_u_l, uint8 num_d_r, uint8 num_u_r)
{
    float xielv_l, jieju_l;
    float xielv_r, jieju_r;
    uint16 i;
	int temp;
    
	
	
    // ������߽粹��
    if (num_d_l&& num_u_l&&num_d_r&&num_u_r && huandao_flag == 0)
    {

        if (abs(num_u_l-num_u_r) <=10 && abs(num_d_l-num_d_r) <=10 && abs(num_d_l-num_u_l) >= 15 && abs(num_d_r - num_u_r) >= 15 && Continuity_Change_Left(MT9V03X_H-5, line + 10,line) != 0 && Continuity_Change_Right(MT9V03X_H-5, line + 10,line) != 0)// num_l >= 2
        {
            // ʹ����������ϵ�������Բ�ֵ
            xieji(num_u_l, num_d_l, road_left[num_u_l], road_left[num_d_l],road_left);
            
//            for (i = num_u_l; i < num_d_l; i++)
//            {
//                road_left[i] = xielv_l * i + jieju_l;
//                if (road_left[i] <= 1)
//                    road_left[i] = 1;
//                else if (road_left[i] >= MT9V03X_W - 2)
//                    road_left[i] = MT9V03X_W - 2;
//            }
			xieji(num_u_r, num_d_r, road_right[num_u_r], road_right[num_d_r],road_right);
            
//            for (i = num_u_r; i < num_d_r; i++)
//            {
//                road_right[i] = xielv_r * i + jieju_r;
//                if (road_right[i] <= 1)
//                    road_right[i] = 1;
//                else if (road_right[i] >= MT9V03X_W - 2)
//                    road_right[i] = MT9V03X_W - 2;
//            }
			huandao_clear();
			crossing_flag_help = 1;
        }
		
    
    }
	else if (num_d_l == 0 && num_d_r == 0 && num_u_l >= MT9V03X_H/2+5 && num_u_r >= MT9V03X_H/2+5 && abs(num_u_l-num_u_r) <=12 && huandao_flag == 0)
	{
		if (abs(num_u_l - num_u_r) < 12)
		{
			least_squares(num_u_l-15,num_u_l-5,road_left,&xielv_l,&jieju_l);
            // ���߲����Ʊ߽�
            for (i = num_u_l - 5; i < MT9V03X_H - 1; i++)
            {
                road_left[i] = xielv_l * i + jieju_l;
                if (road_left[i] <= 1)
                    road_left[i] = 1;
                else if (road_left[i] >= MT9V03X_W - 2)
                    road_left[i] = 1;
            }
			least_squares(num_u_r-15,num_u_r-5,road_right,&xielv_r,&jieju_r);
            for (i = num_u_r - 5; i < MT9V03X_H - 1; i++)
            {
                road_right[i] = xielv_r * i + jieju_r;
                if (road_right[i] <= 1)
                    road_right[i] = MT9V03X_W - 2;
                else if (road_right[i] >= MT9V03X_W - 2)
                    road_right[i] = MT9V03X_W - 2;
            }
//			huandao_clear();
			crossing_flag_help = 1;
		}
	}
}

uint8 my_max(uint8 num1, uint8 num2)
{
	if (num1 > num2)
		return num1;
	else
		return num2;
}

/*********************************************************
** ��ע��Ȩ�ؿ�ͨ��ͼ��Ȩ���������
*********************************************************/
float find_mid_line_weight(void)
{
    float mid_line_value = MID_W;       // �����������ֵ
    float mid_line = MID_W;             // ��������ֵ
    float weight_midline_sum = 0;      // ��Ȩ�����ۼ�ֵ
    float weight_sum = 0;              // Ȩ���ۼ�ֵ
//	if (low_high_choose == 0)
//	{
		for(uint8 i = MT9V03X_H - 1; i > line; i--)
		{
			weight_midline_sum += center_line[i] * mid_weight_list[i];
			weight_sum += mid_weight_list[i];
		}
//	}
//	else if (low_high_choose == 1)
//	{
//		for(uint8 i = MT9V03X_H - 1; i > line; i--)
//		{
//			weight_midline_sum += center_line[i] * low_weight_list[i];
//			weight_sum += low_weight_list[i];
//		}
//	}
    

    mid_line = (float)(weight_midline_sum / weight_sum);
    mid_line_value = last_mid_line * 0.2 + mid_line * 0.8; // �����˲�
    last_mid_line = mid_line_value;
    return mid_line_value;
}


void Zebra_crossing_handle(void)
{
	uint8 i ,j;
	uint8 Zebra_crossing_num = 0;
	for (i = 40; i <= 100; i+=10)
	{
		for (j = 20; j < 168; j+=2)
		{
			if ((PostProcessing_image[i][j] == BLACK_PIXEL && PostProcessing_image[i][j-1] == WHITE_PIXEL) || (PostProcessing_image[i][j] == BLACK_PIXEL && PostProcessing_image[i][j+1] == WHITE_PIXEL))
			{
				Zebra_crossing_num++;
			}
		}
		if (Zebra_crossing_num >= 10)
		{
			Zebra_stop_flag = 1;
			break;
		}
		else
		{
			Zebra_crossing_num = 0;
			
		}
	}

}

//// �Ż���������һ�����ټ��㣩
//#define ZEBRA_ROW_STEP 2         // �м��������ÿ2��ɨ1�У�
//#define ZEBRA_COL_STEP 2         // �м��������ÿ2��ɨ1�У�
//#define ZEBRA_ROW_RATIO_START 0.4f  // ����з�Χ���ƣ����۽����²���
//#define ZEBRA_ROW_RATIO_END   0.7f
//#define MIN_EDGES_PER_ROW 10      // ���ͱ�Ե������ֵ������������
//#define MIN_CONTINUOUS_ROWS 2
//#define MIN_EDGE_GAP 4           // ��������ֵ������������Ե��ϡ�裩

//uint8 Zebra_stop_flag = 0;

//void Zebra_crossing_handle(void)
//{
//    // 1. ��̬�����С�ļ�ⷶΧ
//    uint8 start_row = (uint8)(MT9V03X_H * ZEBRA_ROW_RATIO_START);
//    uint8 end_row = (uint8)(MT9V03X_H * ZEBRA_ROW_RATIO_END);
//    uint8 start_col = (uint8)(MT9V03X_W * 0.15f);  // ��һ����С�з�Χ
//    uint8 end_col = (uint8)(MT9V03X_W * 0.85f);

//    uint8 continuous_valid_rows = 0;

//    // 2. �м��������ÿZEBRA_ROW_STEP��ɨ1�У�����50%�б�����
//    for (uint8 i = start_row; i <= end_row; i += ZEBRA_ROW_STEP)
//    {
//        uint8 edge_count = 0;
//        uint8 last_edge_col = 0;

//        // 3. �м��������ÿZEBRA_COL_STEP��ɨ1�У�����50%�б�����
//        for (uint8 j = start_col; j < end_col - ZEBRA_COL_STEP; j += ZEBRA_COL_STEP)
//        {
//            // �򻯱�Ե�жϣ�ֻ�������������䣨��Чԭ�߼���������٣�
//            if (PostProcessing_image[i][j] != PostProcessing_image[i][j + ZEBRA_COL_STEP])
//            {
//                // ���˽���������������������Ե�����ֵ������
//                if (edge_count == 0 || (j - last_edge_col) >= MIN_EDGE_GAP)
//                {
//                    edge_count++;
//                    last_edge_col = j;
//                    // ��ǰ�˳�
//                    if (edge_count > MIN_EDGES_PER_ROW) break;
//                }
//            }
//        }

//        // 4. ���ж��߼�
//        if (edge_count >= MIN_EDGES_PER_ROW)
//        {
//            if (++continuous_valid_rows >= MIN_CONTINUOUS_ROWS)
//            {
//                Zebra_stop_flag = 1;
//                return;  // �ҵ��������˳������ٺ�������
//            }
//        }
//        else
//        {
//            continuous_valid_rows = 0;
//        }
//    }

//    Zebra_stop_flag = 0;
//}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     �����������Լ��
  @param     ��ʼ�㣬��ֹ��
  @return    ��������0�����������ض��߳�����
  Sample     continuity_change_flag=Continuity_Change_Left(int start,int end)
  @note      �����Ե���ֵ����Ϊ5���ɸ���
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Left(int start,int end, uint8 ref_line)
{
    int i;
    int t;
    int continuity_change_flag=0;
    if(Right_Lost_Time>=0.8*(MT9V03X_H - ref_line))//�󲿷ֶ����ߣ�û��Ҫ�ж���
       return 1;
    if(start>=MT9V03X_H-5)//����Խ�籣��
        start=MT9V03X_H-5;
    if(end<=15)
       end=15;
    if(start<end)//���Ǵ������ϼ���ģ����˾ͻ���һ��
    {
       t=start;
       start=end;
       end=t;
    }
 
    for(i=start;i>=end;i--)
    {
        if(abs(road_left[i]-road_left[i-1])>=5)//��������ֵ��5���ɸ���
       {
            continuity_change_flag=i;
            break;
       }
    }
    return continuity_change_flag;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     �����������Լ��
  @param     ��ʼ�㣬��ֹ��
  @return    ��������0�����������ض��߳�����
  Sample     continuity_change_flag=Continuity_Change_Right(int start,int end)
  @note      �����Ե���ֵ����Ϊ5���ɸ���
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Right(int start,int end, uint8 ref_line)
{
    int i;
    int t;
    int continuity_change_flag=0;
    if(Right_Lost_Time>=0.8*(MT9V03X_H - ref_line))//�󲿷ֶ����ߣ�û��Ҫ�ж���
       return 1;
    if(start>=MT9V03X_H-5)//����Խ�籣��
        start=MT9V03X_H-5;
    if(end<=15)
       end=15;
    if(start<end)//���Ǵ������ϼ���ģ����˾ͻ���һ��
    {
       t=start;
       start=end;
       end=t;
    }
 
    for(i=start;i>=end;i--)
    {
        if(abs(road_right[i]-road_right[i-1])>=5)//��������ֵ��5���ɸ���
       {
            continuity_change_flag=i;
            break;
       }
    }
    return continuity_change_flag;
}


int find_min(uint8 arr[], int start, int end) {
	//���������ҵ�
	int circle_num = 0;
	
    if (start < end || start < 0 || end < 0) {
        return -1; // ������
    }
	if (start >= MT9V03X_H - 8)
	{
		start = MT9V03X_H - 8;
	}

    for (int i = start - 1; i >= end; i--) {
        if (arr[i] <= arr[i-1] && 
			arr[i-1] <= arr[i-2] && 
			arr[i-2] < arr[i-3] &&
			arr[i-3] < arr[i-4] &&
			arr[i] <= arr[i+1] &&
			arr[i+1] <= arr[i+2] &&
			arr[i+2] < arr[i+3] &&
			arr[i+3] < arr[i+4])
		{
            circle_num = i;
			return circle_num;
        }
    }
    return circle_num;
}

int find_max(uint8 arr[], int start, int end) {
	//���������ҵ�
	int circle_num = 0;
	
    if (start < end || start < 0 || end < 0) {
        return -1; // ������
    }
	if (start >= MT9V03X_H - 5)
	{
		start = MT9V03X_H - 5;
	}

    for (int i = start - 1; i >= end; i--) {
        if (arr[i] >= arr[i-1] && 
			arr[i-1] >= arr[i-2] && 
			arr[i-2] > arr[i-3] &&
			arr[i-3] > arr[i-4] &&
			arr[i] >= arr[i+1] &&
			arr[i+1] >= arr[i+2] &&
			arr[i+2] > arr[i+3] &&
			arr[i+3] > arr[i+4])
		{
            circle_num = i;
			return circle_num;
        }
    }
    return circle_num;
}




uint8 research_20point(uint8 num,uint8 arr[])
{
	if (num >= MT9V03X_H - 5)
	{
		return 0;
	}
	uint8 i = 0;
	for (i = MT9V03X_H - 5; i > num; i--)
	{
		if (arr[i] < MT9V03X_W - 15)
		{
			return i;
			
		}
	}
	return 0;
}


int find_min_min(uint8 arr[], int start, int end) {
	//���������ҵ�
	int circle_num = 0;
	
    if (start < end || start < 0 || end < 0) {
        return -1; // ������
    }
	if (start >= MT9V03X_H - 8)
	{
		start = MT9V03X_H - 8;
	}

    for (int i = start - 1; i >= end; i--) {
        if (arr[i] <= arr[i-1] && 
			arr[i-1] <= arr[i-2] && 
			arr[i-2] <= arr[i-3] &&
			arr[i-3] <= arr[i-4] &&
			arr[i] <= arr[i+1] &&
			arr[i+1] <= arr[i+2] &&
			arr[i+2] <= arr[i+3] &&
			arr[i+3] <= arr[i+4 ]&&
			arr[i] < MT9V03X_W-6) 
		{
            circle_num = i;
			return circle_num;
        }
    }
    return circle_num;
}

uint8 find_max_limit(uint8 num1,uint8 num2)
{
	if (num1 > num2)
	{
		return num1;
	}
	else
	{
		return num2;	
	}
}

uint8 Continuity_reduce_left(uint8 arr[])
{
	uint8 i = 0;
	uint8 reduce_left = 0;
	uint8 wrong_num = 0;
	for (i = MT9V03X_H - 5; i > MT9V03X_H - 40; i--)
	{
//		if (arr[i] - arr[i-3] < -)
		if (arr[i] -arr[i-1] > 0)
		{
			wrong_num++;
		}
		if (wrong_num > 2)
		{
			reduce_left = 1;
			break;
		}
	}
	return reduce_left;
}

/**
 * @brief �����߽��Ƿ����Բ�����������ϴ����У�start_row > end_row��
 * @param arr       �߽����������飨�� road_left��
 * @param start_row ��ʼ�У������£��кŴ�
 * @param end_row   �����У������ϣ��к�С��
 * @return arr����߽磬arr_another���ұ߽�
 */
uint8 detect_left_arc(uint8 arr[],uint8 arr_another[], uint8 start_row, uint8 end_row) {
    // 1. �Ϸ��Լ�飺����������ϣ�start_row > end_row��
    if (start_row <= end_row) return 0; 

    int8 diff1[MT9V03X_H] = {0}; // һ�ײ�֣���ǰ�У��£� - ��һ�У��ϣ����б仯
    int8 diff2[MT9V03X_H] = {0}; // ���ײ�֣�һ�ײ�ֵı仯�ʣ���ӳ������
	uint8 num = 0;
	uint8 wrong_num = 0;
	uint8 sum = 0;

    // 2. ����һ�ײ�֣��������ϱ����У�i�ݼ���
    for (uint8 i = start_row; i > end_row; i--) {
        if (i == 0) break; // ��ֹ i-1 Խ�磨�к�0�������
        diff1[i] = arr[i] - arr[i-1]; // ��ǰ�У��£� - ��һ�У��ϣ����б仯
		if (diff1[i] > 0)
		{
			wrong_num++;
		}
		if (wrong_num > 2)
		{
			return 0;
		}
		if (arr_another[i] >= MT9V03X_W-3)
		{
			sum++;
		}
    }

    // 3. ������ײ�֣���ӳб�ʵı仯��i�ݼ�������
    for (uint8 i = start_row - 1; i > end_row; i--) {
        if (i + 1 >= MT9V03X_H) break; // ��ֹ i+1 Խ��
        diff2[i] = diff1[i] - diff1[i+1]; // ��ǰ��� - ��һ�У������£��Ĳ��
		if (abs(diff2[i]) >=1)
		{
			num++;
		}
    }
	if (num > 5 && sum >= (start_row-end_row-10))
	{
		return 1;
	}
	else
	{
		return 0;
	}
	

}

uint8 research_black_point(void)
{
	uint8 i = 0;
	for (i = MT9V03X_H-5; i > 5; i--)
	{
		if (PostProcessing_image[i][MT9V03X_W-4]==WHITE_PIXEL && PostProcessing_image[i-1][MT9V03X_W-4]==BLACK_PIXEL && PostProcessing_image[i-2][MT9V03X_W-4]==BLACK_PIXEL)
		{
			return i;
		}
	}
	return 0;
}


//uint8  yuanhuan_see_handle(void)
//{
//	uint8 yuanhuan_qiedian = 0;
//	uint8 see_huanyuan_flag = 0;
////	float xielv,jieju;
//	if (Continuity_Change_Left(MT9V03X_H, line + 2) == 0 && Continuity_Change_Right(MT9V03X_H, line + 2) != 0 && r_u_num > 0 && r_d_num > 0)
//	{
//		see_huanyuan_flag = 1;
//		yuanhuan_qiedian = find_min(road_right, r_u_num+5, 10);
//		huandao_flag = 1;
//		if (yuanhuan_qiedian && huandao_flag == 1)
//		{
//			xieji(yuanhuan_qiedian, r_d_num, road_right[yuanhuan_qiedian], road_right[r_d_num], road_right);
//            
////            for (uint8 i = yuanhuan_qiedian; i < r_d_num; i++)
////            {
////                road_right[i] = xielv * i + jieju;
////                if (road_right[i] <= 1)
////                    road_right[i] = 1;
////                else if (road_right[i] >= MT9V03X_W - 2)
////                    road_right[i] = MT9V03X_W - 2;
////            }
//		}
//		
//	}
//	return see_huanyuan_flag;
//}

uint8 going_flag = 0;
static uint8 exit_cnt_1 = 0;
static uint8 exit_cnt_2_1 = 0;
static uint8 exit_cnt_2_2 = 0;
static uint8 exit_cnt_3 = 0;
static uint8 exit_cnt_4 = 0;
static uint8 exit_cnt_0 = 0;

void yuanhuan_in_handle(void)
{
	float xielv,jieju;
	uint8 u_num = 0;
	
	u_num = Find_Right_Up_Point(MT9V03X_H-1,line,1);
	if (huandao_flag == 0 && huandao_num_flag == 0)
	{
		
		if (Continuity_Change_Left(MT9V03X_H-25, line + 2,line) == 0 && Continuity_Change_Right(MT9V03X_H, line + 2,line) != 0 && r_u_num > 0 && r_d_num > 0 && line < 5 && Left_Lost_Time <= MT9V03X_H/10 && Right_Lost_Time >= MT9V03X_H/3)
		{
			exit_cnt_1++;
			if (exit_cnt_1 >= 1)
			{
				huandao_flag = 1;
				exit_cnt_1 = 0;
				exit_cnt_2_1 = 0;
			}
			
			
		}
		else if (r_d_num == 0 && r_u_num != 0 && Continuity_Change_Left(MT9V03X_H-15, line + 10,line) == 0 && Continuity_Change_Right(MT9V03X_H, line + 2,line) != 0 && line < 5 && Left_Lost_Time <=MT9V03X_H/10 && Right_Lost_Time >= MT9V03X_H/3)
		{
			exit_cnt_2_1++;
			if (exit_cnt_2_1 >= 3)
			{
				huandao_flag = 2;
				exit_cnt_1 = 0;
				exit_cnt_2_1 = 0;
			}
			
		}
		else
		{
			exit_cnt_1 = 0;
			exit_cnt_2_1 = 0;
		}
	}
	else if (huandao_flag == 1)
	{// && Continuity_Change_Left(MT9V03X_H-15, line + 10,line) == 0
		uint8 yuanhuan_qiedian = 0;
		
		yuanhuan_qiedian = find_min(road_right, r_u_num+5, 10);
		if (r_d_num == 0 && r_u_num != 0)
		{
			exit_cnt_2_2++;
			if (exit_cnt_2_2 >= 1)
			{
				huandao_flag = 2;
				exit_cnt_2_2 = 0;
			}
			
			
		}
		else
		{
			exit_cnt_2_2 = 0;
		}
		if (yuanhuan_qiedian)
		{
			xieji(yuanhuan_qiedian, r_d_num, road_right[yuanhuan_qiedian], road_right[r_d_num], road_right);
            
		}
		
	}
	else if (huandao_flag == 2)
	{// && detect_left_arc(road_left,road_right,MT9V03X_H-3,MT9V03X_H-38)
		uint8 qiebuxian = 0;
		going_flag = 0;
		
		qiebuxian = find_min_min(road_right, MT9V03X_H-5, u_num - 5);
		
		if (r_d_num == 0 && r_u_num == 0)
		{
			exit_cnt_3++;
			if (exit_cnt_3 >= 1)
			{
				huandao_flag = 3;
				exit_cnt_3 = 0;
			}
			
		}
		else
		{
			exit_cnt_3 = 0;
		}
		
		if (qiebuxian > 0)
		{
			xieji(qiebuxian, MT9V03X_H-5,road_right[qiebuxian], MT9V03X_W-20, road_right);
		}
		if (u_num > 10 && r_d_num == 0 && abs(u_num - 45) <= 20)
		{
			xieji(u_num, MT9V03X_H-5, road_right[u_num], 20,road_left);
			xieji(10, u_num, MT9V03X_W-2, road_right[u_num],road_left);
			for (uint8 i = 10; i <= u_num; i++)
			{
				road_right[i]=MT9V03X_W-3;
			}
//			low_high_choose = 1;
		}
		
//		if (going_flag == 1 && Continuity_reduce_left(road_left))
//		{
//			huandao_flag = 2;
//		}
	}
	else if (huandao_flag == 3)
	{
		
		uint8 left_new_point = find_max(road_left, MT9V03X_H-5, 20);
		if (Continuity_Change_Left(MT9V03X_H-10, line + 10,line) == 0 && Continuity_Change_Right(MT9V03X_H, line + 2,line) != 0 && u_num > 0)
		{
			exit_cnt_4++;
			if (exit_cnt_4 >= 1)
			{
				huandao_flag = 4;
				exit_cnt_4 = 0;
			}
			
			
		}
		else
		{
			exit_cnt_4 = 0;
		}
//		uint8 right_add_point = research_black_point();
		if (left_new_point > 0)
		{
			xieji(20, left_new_point, MT9V03X_W-5, road_left[left_new_point],road_left);
			for (uint8 i = 20; i < MT9V03X_H-55; i++)
			{
				road_right[i] = MT9V03X_W-3;
			}
			going_flag = 1;
		}
		else if (left_new_point == 0 && going_flag == 1)
		{
			xieji(5, MT9V03X_H, MT9V03X_W-60, 3, road_left);
			for (uint8 i = 5; i < MT9V03X_H-35; i++)
			{
				road_right[i] = MT9V03X_W-3;
			}
		}
		
	}
	else if (huandao_flag == 4)
	{
		
		left_test = Continuity_Change_Left(MT9V03X_H-7, line+25, line);
		right_test = Continuity_Change_Right(MT9V03X_H-7, line+25, line);
		if (left_test == 0 && right_test == 0 && l_d_num == 0 && r_d_num == 0)
		{
			exit_cnt_0++;
			if (exit_cnt_0 >=1)
			{
				huandao_flag = 0;
				huandao_num_flag = 10;
				exit_cnt_0 = 0;
			}
			
		}
		else
		{
			exit_cnt_0 = 0;
		}
		if (huandao_flag != 0)
		{
			xieji(70, MT9V03X_H-5, MT9V03X_W-60, MT9V03X_W-45,road_right);// || u_num > 140
		}
		
	}
}


