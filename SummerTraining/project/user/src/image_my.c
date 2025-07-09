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
//void research_longest_line(void)
//{
//	uint8 i,j;
//	uint8 stopflag = 0;
//	left_point = 0;
//	right_point = 0;
//	longest_line_number = MT9V03X_W/2;
//	line = 119;
//	left_point = get_left_start();
//	right_point = get_right_start();
//	stopflag = 0;
//	for (i = left_point; i < right_point; i++)
//	{
//		for (j = MT9V03X_H - 1; j > 0; j--)
//		{
//			
//			if ((PostProcessing_image[j][i] == WHITE_PIXEL && PostProcessing_image[j - 1][i] == BLACK_PIXEL))
//			{
////				dead_line[i] = j;
//				if (j < line)
//				{
//					line = j;
//					longest_line_number = i;
//					break;
//				}
//			}
//			else if (j == 1)
//			{
//				longest_line_number = i;
//				line = j;
//				stopflag = 1;
//				break;
//			}
//			
//		}
//		if (stopflag)
//		{
//			break;
//		}
//	}
//}
void research_longest_line(void)
{
    uint8 i, j;
    uint8 longest_length = 0;  // ��¼���ɫ�еĳ���
    
    left_point = get_left_start();
    right_point = get_right_start();
    
    longest_line_number = MT9V03X_W/2;  // Ĭ��ֵ
    line = 119;  // Ĭ��ֵ��119��һ������ѡ�񣬴���ϳ��ĳ��ȣ�
    
    // ȷ�����ұ߽����
    if (left_point >= right_point) return;
    
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
            start_row = 0;
            current_length = MT9V03X_H;  // ���ж��ǰ�ɫ
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
//	uint8 left_num = 0;
//	uint8 right_num = 0;
	left_flag_break = 0;
	right_flag_break = 0;
	break_num_left = 0;
	break_num_right = 0; //&& PostProcessing_image[i][j-2] == BLACK_PIXEL// && PostProcessing_image[i][j+2] == BLACK_PIXEL
	for (i = line; i < MT9V03X_H; i++)
	{
		for (j = longest_line_number; j > 1; j--)       //��
		{
//			left_num++;
			if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j-1] == BLACK_PIXEL) || j == 2)
			{
				road_left[i] = j;
				if (left_flag_break)
				{
					if (road_left[i] - road_left[i-1] >= 5 && break_num_left < 6)
					{
						break_point_l[break_num_left][0] = i;   //��һ��
						break_point_l[break_num_left][1] = road_left[i];   //��һ��
						break_num_left++;
					}
					else if (road_left[i] - road_left[i-1] <= -5 && break_num_left < 6)
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
		for (j = longest_line_number; j < MT9V03X_W - 2; j++)
		{
//			right_num++;
			if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j+1] == BLACK_PIXEL) || j == 185)
			{
				road_right[i] = j;
				if (right_flag_break)
				{
					if (road_right[i] - road_right[i-1] >= 5 && break_num_right < 6)
					{
						break_point_r[break_num_right][0] = i - 1;
						break_point_r[break_num_right][1] = road_right[i -1];
						break_num_right++;
					}
					else if (road_right[i] - road_right[i-1] <= -5 && break_num_right < 6)
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
	merge_close_breakpoints();
}

//uint8 road_left[120] = {255};         // ��ʼ��Ϊ��Чֵ255������188��Χ��
//uint8 break_point_l[6][2] = {{0}};
//uint8 break_num_left;
//uint8 break_point_r[6][2] = {{0}};
//uint8 break_num_right;
//uint8 road_right[120] = {255};       
//uint8 left_flag_break;
//uint8 right_flag_break;
//void research_road(void)
//{
//    uint8 i, j;
//    left_flag_break = 0;
//    right_flag_break = 0;
//    break_num_left = 0;
//    break_num_right = 0;

//    for (i = line; i < MT9V03X_H; i++)
//    {
//        // ��߽�����������ԭ���߼����䣩
//        for (j = longest_line_number; j > 1; j--)
//        {
//            if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j-1] == BLACK_PIXEL && PostProcessing_image[i][j-2] == BLACK_PIXEL) || j == 2)
//            {
//                road_left[i] = j;
//                break;
//            }
//        }

//        // �ұ߽�����������ԭ���߼����䣩
//        for (j = longest_line_number; j < MT9V03X_W - 2; j++)
//        {
//            if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j+1] == BLACK_PIXEL && PostProcessing_image[i][j+2] == BLACK_PIXEL) || j == 185)
//            {
//                road_right[i] = j;
//                break;
//            }
//        }

//        // -------------------------- ��߽�յ��⣨ʮ��������--------------------------
//        if (left_flag_break && i > line)  // ȷ������һ������
//        {
//            int8 diff = road_left[i] - road_left[i-1];
//            
//            // ������ҵ�ͻ�䣨������ʮ�ֵ�����Ե��
//            if (diff >= 5 && break_num_left < 6)
//            {
//                break_point_l[break_num_left][0] = i;      // ��¼��ǰ�У�ʮ������Ե��
//                break_point_l[break_num_left][1] = road_left[i];
//                break_num_left++;
//            }
//            
//            // ��������ͻ�䣨��������ͨ�����
//            if (diff <= -5 && break_num_left < 6)
//            {
//                break_point_l[break_num_left][0] = i-1;    // ��¼��һ�У���ͨ�����
//                break_point_l[break_num_left][1] = road_left[i-1];
//                break_num_left++;
//            }
//        }
//        left_flag_break = 1;

//        // -------------------------- �ұ߽�յ��⣨ʮ��������--------------------------
//        if (right_flag_break && i > line)  // ȷ������һ������
//        {
//            int8 diff = road_right[i] - road_right[i-1];
//            
//            // ��������ͻ�䣨������ʮ�ֵ��Ҳ��Ե��
//            if (diff <= -5 && break_num_right < 6)
//            {
//                break_point_r[break_num_right][0] = i;      // ��¼��ǰ�У�ʮ���Ҳ��Ե��
//                break_point_r[break_num_right][1] = road_right[i];
//                break_num_right++;
//            }
//            
//            // ������ҵ�ͻ�䣨��������ͨ�����
//            if (diff >= 5 && break_num_right < 6)
//            {
//                break_point_r[break_num_right][0] = i-1;    // ��¼��һ�У���ͨ�����
//                break_point_r[break_num_right][1] = road_right[i-1];
//                break_num_right++;
//            }
//        }
//        right_flag_break = 1;
//    }
//    
//    // �����ϲ��ڽ��յ㣨�����Ҫ��
//    merge_close_breakpoints();
//}



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








uint8 center_line[120];
uint16 center_value = 94;
//uint8 (*image_progress)[MT9V03X_W]
void image_process(void)
{
	uint16 sum;
	uint16 value_count;
	
	image_postprocess();
	research_longest_line();
	research_road();
//	crossing_add(break_num_left,break_num_right);
	
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
	
	sum = 0;
	
	for (uint8 i = MT9V03X_H - 25; i < MT9V03X_H - 1; i++)
	{
		if (center_line[i])
			sum += center_line[i];
	}
	center_value = sum / 24;
	break_num_left = 0;
	break_num_right = 0;
	memset(break_point_l, 0, sizeof(break_point_l));
	memset(break_point_r, 0, sizeof(break_point_r));
	
	memset(road_left, 0, sizeof(road_left));
	memset(road_right, 0, sizeof(road_right));
	
	memset(center_line, 0, sizeof(center_line));
		
}



//uint8 center_line[MT9V03X_H];  // ����߶Ⱥ궨��
//uint16 center_value = MT9V03X_W / 2;  // ��ʼֵ��Ϊͼ�����ģ�188/2=94����ԭĬ��ֵһ�£�

//void image_process(void)
//{
//    uint16 sum = 0;
//    uint8 valid_count = 0;  // ��¼��Ч�����ߵ�����

//    // 1. ͼ����������̣�˳�򲻱䣬�����궨�壩
//    image_postprocess();          // ��ֵ��
//	image_filter(PostProcessing_image);
//    research_longest_line();      // �������
//    research_road();              // �ҵ�·�߽�͹յ�
////    merge_close_breakpoints();    // �ϲ��ڽ��յ�
//    crossing_add(break_num_left, break_num_right);  // ����

//    // 2. ��ʾ��ֵ��ͼ��ʹ��MT9V03X_W/H�궨�壩
//    ips200_show_gray_image(0, 50, PostProcessing_image[0], 
//                          MT9V03X_W, MT9V03X_H, 
//                          MT9V03X_W, MT9V03X_H, 0);

//    // ��������У���ɫ�ߣ�ѭ����ΧΪͼ��߶ȣ�
//    for (uint8 i = 0; i < MT9V03X_H; i++)
//    {
//        // ȷ����������Ч��Χ�ڣ�0 ~ MT9V03X_W-1��
//        uint8 draw_col = longest_line_number;
//        if (draw_col >= MT9V03X_W) draw_col = MT9V03X_W - 1;
//        ips200_draw_point(draw_col, i + 50, RGB565_YELLOW);
//    }

//    // 3. ���������߲�����
//    for (uint8 i = line; i < MT9V03X_H; i++)
//    {
//        // ��/�ұ߽���Ч�Լ�飨255Ϊ��Ч��ǣ�
//        bool left_valid = (road_left[i] != 255) && (road_left[i] < MT9V03X_W);
//        bool right_valid = (road_right[i] != 255) && (road_right[i] < MT9V03X_W);

//        if (left_valid && right_valid)
//        {
//            // ��Ч�߽磺���������ߣ�ƽ����
//            center_line[i] = (road_left[i] + road_right[i]) >> 1;
//        }
//        else if (left_valid)
//        {
//            // ����߽���Ч�������� = ��߽� + Ԥ��������ʵ�ʵ�·������
//            center_line[i] = road_left[i] + 40;  // 40Ϊʾ������ƥ��ʵ�ʵ�·���
//        }
//        else if (right_valid)
//        {
//            // ���ұ߽���Ч�������� = �ұ߽� - Ԥ����
//            center_line[i] = road_right[i] - 40;
//        }
//        else
//        {
//            // ����Ч������ǰһ�е������ߣ�����ͻ�䣩
//            center_line[i] = (i > line) ? center_line[i-1] : center_value;
//        }

//        // ������������ͼ��Χ�ڣ�0 ~ MT9V03X_W-1��
//        if (center_line[i] >= MT9V03X_W) center_line[i] = MT9V03X_W - 1;
//        if (center_line[i] < 0) center_line[i] = 0;

//        // ���Ʊ߽�������ߣ������Ʒ�Χ��
//        if (left_valid)
//        {
//            uint8 left_col = road_left[i] + 2;
//            if (left_col < MT9V03X_W) ips200_draw_point(left_col, i + 50, RGB565_GREEN);
//        }
//        if (right_valid)
//        {
//            uint8 right_col = road_right[i] - 2;
//            if (right_col >= 0) ips200_draw_point(right_col, i + 50, RGB565_BLUE);
//        }
//        ips200_draw_point(center_line[i], i + 50, RGB565_RED);  // ��������ȷ���ڷ�Χ��
//    }

//    // 4. ��������ֵ����̬ƽ����ȡ�ײ�16�У�
//    sum = 0;
//    valid_count = 0;
//    uint8 start_row = MT9V03X_H - 16;  // �ӵײ���16�п�ʼ��120-16=104��
//    for (uint8 i = start_row; i < MT9V03X_H; i++)
//    {
//        // ���ۼ���Ч�����ߣ�0 ~ MT9V03X_W-1��
//        if (center_line[i] >= 0 && center_line[i] < MT9V03X_W)
//        {
//            sum += center_line[i];
//            valid_count++;
//        }
//    }
//    // ��Ч������8ʱ��������ֵ��ȷ���ȶ��ԣ�
//    if (valid_count >= 8)
//    {
//        center_value = sum / valid_count;
//    }
//    // ��Ч��������ʱ�����ϴ�ֵ

//    // 5. ������ݣ�Ϊ��һ�δ���׼����
//    break_num_left = 0;
//    break_num_right = 0;
//    memset(break_point_l, 0, sizeof(break_point_l));
//    memset(break_point_r, 0, sizeof(break_point_r));
//    memset(road_left, 255, sizeof(road_left));  // ����Ϊ��Чֵ��255��
//    memset(road_right, 255, sizeof(road_right));
//    memset(center_line, 0, sizeof(center_line));
//}



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

//void crossing_add(uint8 num_l, uint8 num_r)
//{
//	float xielv_l,jieju_l;
//	float xielv_r,jieju_r;
//	uint16 i =0;
////	if (num_l == 0 && num_r ==0)
////	{
////		
////	}
//	if (num_l == 1)
//	{
//		least_squares(break_point_l[0][0]-15, break_point_l[0][0] - 5, road_left, &xielv_l, &jieju_l);
//		for (i = break_point_l[0][0] - 5; i < MT9V03X_H - 1; i++)
//		{
//			road_left[i] = xielv_l*i + jieju_l;
//			if (road_left[i] <= 1)
//			{
//				road_left[i] = 1;
//			}
//		}
//	}
//	else if (num_l == 2)
//	{
//		xieji(break_point_l[1][0] , break_point_l[0][0], &xielv_l, &jieju_l, break_point_l[1][1], break_point_l[0][1]);
//		for (i = break_point_l[1][0]; i < break_point_l[0][0]; i++)
//		{
//			road_left[i] = xielv_l*i + jieju_l;
//			if (road_left[i] <= 1)
//			{
//				road_left[i] = 1;
//			}
//		}
//	}
//	if (num_r == 1)
//	{
//		least_squares(break_point_r[0][0]-15, break_point_r[0][0] - 5, road_right, &xielv_r, &jieju_r);
//		for (i = break_point_r[0][0] - 5; i < MT9V03X_H - 1; i++)
//		{
//			road_right[i] = xielv_r*i + jieju_r;
//			if (road_right[i] <= 1)
//			{
//				road_right[i] = 1;
//			}
//		}
//	}
//	else if (num_r == 2)
//	{
//		xieji(break_point_r[1][0] , break_point_r[0][0], &xielv_r, &jieju_r, break_point_r[1][1],break_point_r[0][1]);
//		for (i = break_point_r[1][0]; i < break_point_r[0][0]; i++)
//		{
//			road_right[i] = xielv_r*i + jieju_r;
//			if (road_right[i] <= 1)
//			{
//				road_right[i] = 1;
//			}
//		}
//	}
//}
void crossing_add(uint8 num_l, uint8 num_r)
{
    float xielv_l, jieju_l;
    float xielv_r, jieju_r;
    uint16 i;
    
    // ������߽粹��
    if (num_l >= 1)
    {
        if (num_l == 1)
        {
            // ȷ�����㷶Χ��Ч
            uint16 start = break_point_l[0][0] - 15;
            uint16 end = break_point_l[0][0] - 5;
            
            if (start < 0) start = 0;
            if (end <= start) return;
            
            least_squares(start, end, road_left, &xielv_l, &jieju_l);
            
            // ���߲����Ʊ߽�
            for (i = break_point_l[0][0] - 5; i < MT9V03X_H - 1; i++)
            {
                road_left[i] = xielv_l * i + jieju_l;
                if (road_left[i] <= 1)
                    road_left[i] = 1;
                else if (road_left[i] >= MT9V03X_W - 2)
                    road_left[i] = MT9V03X_W - 2;
            }
        }
        else if (num_l >= 2)// num_l >= 2
        {
            // ʹ����������ϵ�������Բ�ֵ
            xieji(break_point_l[num_l-2][0], break_point_l[num_l-1][0], &xielv_l, &jieju_l, 
                  break_point_l[num_l-2][1], break_point_l[num_l-1][1]);
            
            for (i = break_point_l[num_l-2][0]; i < break_point_l[num_l-1][0]; i++)
            {
                road_left[i] = xielv_l * i + jieju_l;
                if (road_left[i] <= 1)
                    road_left[i] = 1;
                else if (road_left[i] >= MT9V03X_W - 2)
                    road_left[i] = MT9V03X_W - 2;
            }
        }
    }
    
    // �����ұ߽粹�ߣ��߼�����߽����ƣ�
    if (num_r >= 1)
    {
        if (num_r == 1)
        {
            uint16 start = break_point_r[0][0] - 15;
            uint16 end = break_point_r[0][0] - 5;
            
            if (start < 0) start = 0;
            if (end <= start) return;
            
            least_squares(start, end, road_right, &xielv_r, &jieju_r);
            
            for (i = break_point_r[0][0] - 5; i < MT9V03X_H - 1; i++)
            {
                road_right[i] = xielv_r * i + jieju_r;
                if (road_right[i] <= 1)
                    road_right[i] = 1;
                else if (road_right[i] >= MT9V03X_W - 2)
                    road_right[i] = MT9V03X_W - 2;
            }
        }
        else if (num_r >= 2) // num_r >= 2
        {
            xieji(break_point_r[num_r-2][0], break_point_r[num_r-1][0], &xielv_r, &jieju_r, 
                  break_point_r[num_r-2][1], break_point_r[num_r-1][1]);
            
            for (i = break_point_r[num_r-2][0]; i < break_point_r[num_r-1][0]; i++)
            {
                road_right[i] = xielv_r * i + jieju_r;
                if (road_right[i] <= 1)
                    road_right[i] = 1;
                else if (road_right[i] >= MT9V03X_W - 2)
                    road_right[i] = MT9V03X_W - 2;
            }
        }
    }
}