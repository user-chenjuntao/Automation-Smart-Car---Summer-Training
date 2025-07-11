#include "image_my.h"

uint8 image_otsuThreshold_less;
uint8 PostProcessing_image[MT9V03X_H][MT9V03X_W];                                      //���ڴ�Ŷ�ֵ��������ͼ������
uint8 Left_Lost_Time = 0;                                                              //��¼��߽���Ч��
uint8 Right_Lost_Time = 0;                                                             //��¼�ұ߽���Ч��

/*********************************************************
** //ͼ��Ȩ������  //Ȩ��Խ���ϣ���ת��Խ��
*********************************************************/
uint8 mid_weight_list[120] = 
{
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
    6,6,6,6,6,6,6,6,6,6,
    7,8,9,10,11,12,13,14,15,16,
    17,18,19,20,20,20,20,19,18,17,
    16,15,14,13,12,11,10,9,8,7,
    6,6,6,6,6,6,6,6,6,6,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
};

uint8 final_mid_line = MID_W;   // �������������ֵ
uint8 last_mid_line = MID_W;    // �ϴ�����ֵ

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
    uint8 i, j;
    uint8 longest_length = 0;  // ��¼���ɫ�еĳ���
    
    left_point = get_left_start();
    right_point = get_right_start();
    
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
        
        // ���ҵ�ǰ�а�ɫ�������ʼ��
        for (j = MT9V03X_H - 1; j > 0; j--)
        {
            if (PostProcessing_image[j][i] == WHITE_PIXEL && 
                PostProcessing_image[j - 1][i] == BLACK_PIXEL)
            {
                start_row = j;
                current_length = MT9V03X_H - start_row;  // �����ɫ�еĳ���
                break;
            }
        }
        
        // ���û���ҵ��ڰױ߽磬�����ж��ǰ�ɫ
        if (j == 0 && PostProcessing_image[0][i] == WHITE_PIXEL)
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
				if (road_left[i] == 2)
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
int Find_Left_Up_Point(int start,int end)//�����½ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i,t;
    int left_up_line=0;
    if(Left_Lost_Time>=0.8*MT9V03X_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
       return left_up_line;
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
              (road_left[i]-road_left[i+2])>=5&&
              (road_left[i]-road_left[i+3])>=10&&
              (road_left[i]-road_left[i+4])>=10)
        {
            left_up_line=i;//��ȡ��������
            break;
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
int Find_Right_Up_Point(int start,int end)//�����½ǵ㣬����ֵ�ǽǵ����ڵ�����
{
    int i,t;
    int right_up_line=0;
    if(Right_Lost_Time>=0.8*MT9V03X_H)//�󲿷ֶ����ߣ�û�йյ��жϵ�����
       return right_up_line;
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
              (road_right[i+2]-road_right[i])>=5&&
              (road_right[i+3]-road_right[i])>=10&&
              (road_right[i+4]-road_right[i])>=10)
        {
            right_up_line=i;//��ȡ��������
            break;
        }
    }
    return right_up_line;
}


uint8 l_d_num = 0;
uint8 l_u_num = 0;
uint8 r_d_num = 0;
uint8 r_u_num = 0;
uint8 center_line[120];
uint16 center_value = 93;
//uint8 (*image_progress)[MT9V03X_W]
void image_process(void)
{
	uint16 sum;
	uint16 value_count;
	
	image_postprocess();
	research_longest_line();
	research_road();
	l_d_num = Find_Left_Down_Point(MT9V03X_H-1,line);
	l_u_num = Find_Left_Up_Point(MT9V03X_H-1,line);
	r_d_num = Find_Right_Down_Point(MT9V03X_H-1,line);
	r_u_num = Find_Right_Up_Point(MT9V03X_H-1,line);
	if (l_d_num > 0 && l_d_num < l_u_num)
	{
		l_d_num = 0;
	}
	if (r_d_num > 0 && r_d_num < r_u_num)
	{
		r_d_num = 0;
	}
	crossing_add(l_d_num,l_u_num,r_d_num,r_u_num);
	
	ips200_show_gray_image(0, 50, PostProcessing_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
	for (uint8 i = 0; i < MT9V03X_H; i++)
	{
		ips200_draw_point(longest_line_number, i+50,RGB565_YELLOW);
	}
	for (uint8 i = line; i < MT9V03X_H; i++)
	{
		ips200_draw_point(road_left[i], i+50,RGB565_GREEN);
		ips200_draw_point(road_right[i], i+50,RGB565_BLUE);
		center_line[i] = (road_left[i] + road_right[i]) >> 1;//������
		ips200_draw_point(center_line[i], i+50,RGB565_RED);
	}
//	for (uint8 i = 0; i < break_num_left; i++)
//	{
		ips200_draw_point(road_left[l_d_num],l_d_num+50,RGB565_RED);
	ips200_draw_point(road_left[l_u_num],l_u_num+50,RGB565_RED);
	ips200_draw_point(road_right[r_d_num],r_d_num+50,RGB565_RED);
	ips200_draw_point(road_right[r_u_num],r_u_num+50,RGB565_RED);
//	}
//	for (uint8 i = 0; i < break_num_right; i++)
//	{
//		ips200_draw_point(break_point_r[i][1],break_point_r[i][0]+50,RGB565_RED);
//	}
	ips200_show_uint(0,208,l_d_num,3);
	ips200_show_uint(30,208,l_u_num,3);
	ips200_show_uint(60,208,r_d_num,3);
	ips200_show_uint(90,208,r_u_num,3);
	sum = 0;
	
//	for (uint8 i = MT9V03X_H-31; i < MT9V03X_H - 1; i++)
//	{
//		if (center_line[i])
//			sum += center_line[i];
//	}
//	center_value = sum / 30;

	final_mid_line = find_mid_line_weight();
	ips200_show_uint(120,208,final_mid_line,3);
	break_num_left = 0;
	break_num_right = 0;
	Left_Lost_Time = 0;
	Right_Lost_Time = 0;
//	memset(break_point_l, 0, sizeof(break_point_l));
//	memset(break_point_r, 0, sizeof(break_point_r));
	
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
void xieji(uint8 begin, uint8 end, float *xielv, float *jieju, uint8 y_begin, uint8 y_end)
{
    if (end == begin)  // ����������
    {
        *xielv = 0;
        *jieju = y_begin;
        return;
    }
    
    // ����б�ʺͽؾ࣬ʹ�ø���������
    *xielv = (float)(y_end - y_begin) / (end - begin);
    *jieju = y_begin - (*xielv) * begin;
}


void crossing_add(uint8 num_d_l, uint8 num_u_l, uint8 num_d_r, uint8 num_u_r)
{
    float xielv_l, jieju_l;
    float xielv_r, jieju_r;
    uint16 i;
	int temp;
    
//	if (num_d_l == 0 && num_r >= 1)
//	{
//		if (line <= 3 && longest_line_number >= 63 && longest_line_number <= 126)
//		{
//			for (uint8 i = line; i < MT9V03X_H; i++)
//			{
//				temp = road_right[i] - 40;
//				if (temp <= 2)
//				{
//					temp = 2;
//				}
//			}
//		}
//	}
    // ������߽粹��
    if (num_d_l&& num_u_l&&num_d_r&&num_u_r)
    {
//        if (num_l == 1)
//        {
//            // ȷ�����㷶Χ��Ч
//            uint16 start = break_point_l[0][0] - 15;
//            uint16 end = break_point_l[0][0] - 5;
//            
//            if (start < 0) start = 0;
//            if (end <= start) return;
//            
//            least_squares(start, end, road_left, &xielv_l, &jieju_l);
//            
//            // ���߲����Ʊ߽�
//            for (i = break_point_l[0][0] - 5; i < MT9V03X_H - 1; i++)
//            {
//                road_left[i] = xielv_l * i + jieju_l;
//                if (road_left[i] <= 1)
//                    road_left[i] = 1;
//                else if (road_left[i] >= MT9V03X_W - 2)
//                    road_left[i] = MT9V03X_W - 2;
//            }
//        }
        if (abs(num_d_l-num_u_l) >= 15 && abs(num_d_r - num_u_r) >= 15)// num_l >= 2
        {
            // ʹ����������ϵ�������Բ�ֵ
            xieji(num_u_l, num_d_l, &xielv_l, &jieju_l, 
                  road_left[num_u_l], road_left[num_d_l]);
            
            for (i = num_u_l; i < num_d_l; i++)
            {
                road_left[i] = xielv_l * i + jieju_l;
                if (road_left[i] <= 1)
                    road_left[i] = 1;
                else if (road_left[i] >= MT9V03X_W - 2)
                    road_left[i] = MT9V03X_W - 2;
            }
			xieji(num_u_r, num_d_r, &xielv_r, &jieju_r, 
                  road_right[num_u_r], road_right[num_d_r]);
            
            for (i = num_u_r; i < num_d_r; i++)
            {
                road_right[i] = xielv_r * i + jieju_r;
                if (road_right[i] <= 1)
                    road_right[i] = 1;
                else if (road_right[i] >= MT9V03X_W - 2)
                    road_right[i] = MT9V03X_W - 2;
            }
        }
    
//		// �����ұ߽粹�ߣ��߼�����߽����ƣ�
//        if (num_r == 1)
//        {
//            uint16 start = break_point_r[0][0] - 15;
//            uint16 end = break_point_r[0][0] - 5;
//            
//            if (start < 0) start = 0;
//            if (end <= start) return;
//            
//            least_squares(start, end, road_right, &xielv_r, &jieju_r);
//            
//            for (i = break_point_r[0][0] - 5; i < MT9V03X_H - 1; i++)
//            {
//                road_right[i] = xielv_r * i + jieju_r;
//                if (road_right[i] <= 1)
//                    road_right[i] = 1;
//                else if (road_right[i] >= MT9V03X_W - 2)
//                    road_right[i] = MT9V03X_W - 2;
//            }
//        }
//        else if (num_r >= 2) // num_r >= 2
//        {
//            
//        }
    }
	if (num_d_l == 0 && num_d_r == 0 && num_u_l >= 0 && num_u_r >= 0)
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
uint8 find_mid_line_weight(void)
{
    uint8 mid_line_value = MID_W;       // �����������ֵ
    uint8 mid_line = MID_W;             // ��������ֵ
    uint32 weight_midline_sum = 0;      // ��Ȩ�����ۼ�ֵ
    uint32 weight_sum = 0;              // Ȩ���ۼ�ֵ

    for(uint8 i = MT9V03X_H - 1; i > line; i--)
    {
        weight_midline_sum += center_line[i] * mid_weight_list[i];
        weight_sum += mid_weight_list[i];
    }

    mid_line = (uint8)(weight_midline_sum / weight_sum);
    mid_line_value = last_mid_line * 0.2 + mid_line * 0.8; // �����˲�
    last_mid_line = mid_line_value;
    return mid_line_value;
}