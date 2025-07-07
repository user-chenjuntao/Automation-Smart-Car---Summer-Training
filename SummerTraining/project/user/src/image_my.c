//#include "image_my.h"

//uint8 image_otsuThreshold_less;
//uint8 PostProcessing_image[MT9V03X_H][MT9V03X_W];                                      //���ڴ�Ŷ�ֵ��������ͼ������
////uint8 left_point[2] = { 0 };//�������x��yֵ
////uint8 right_point_r[2] = { 0 };//�ұ�����x��yֵ


////-------------------------------------------------------------------------------------
////ʹ�ô�򷨽��ж�ֵ������
////-------------------------------------------------------------------------------------
//uint8 otsuThreshold_less(uint8 *image, uint16 width, uint16 height)
//{
//    #define GrayScale 256
//    int pixel_count[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ���ظ���
//    float pixel_account[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ�����ر���
//    int i,j;   
//    int pixel_sum = width * height;   //�����ص�
//    uint8 threshold = 0;
//    uint8* pixel_data = image;  //ָ���������ݵ�ָ��


//    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
//    for (i = 0; i < height; i++)
//    {
//        for (j = 0; j < width; j++)
//        {
//            pixel_count[(int)pixel_data[i * width + j]]++;  //������ֵ��Ϊ����������±�

//        }
//    }
//    float u = 0;  
//    for (i = 0; i < GrayScale; i++)
//    {
//        pixel_account[i] = (float)pixel_count[i] / pixel_sum;   //����ÿ������������ͼ���еı���  
//        u += i * pixel_account[i];  //��ƽ���Ҷ�
//    }

//      
//    float variance_max=0.0;  //�����䷽��
//    float w0 = 0, avgValue  = 0;  //w0 ǰ������ ��avgValue ǰ��ƽ���Ҷ�
//    for(int i = 0; i < 256; i++)     //ÿһ��ѭ������һ��������䷽����� (����for����Ϊ1��)
//    {  
//        w0 += pixel_account[i];  //���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷ�������ռ����ͼ��ı�����ǰ������
//        avgValue  += i * pixel_account[i];        
//        
//        float variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //��䷽��   
//        if(variance > variance_max) 
//        {  
//            variance_max = variance;  
//            threshold = i;  
//        }  
//    } 

//    return threshold;

//}


//void image_postprocess(void)
//{
//	uint8 i = 0, j = 0;
//	image_otsuThreshold_less = otsuThreshold_less(*mt9v03x_image,MT9V03X_W,MT9V03X_H);
//	for (i = 0; i < MT9V03X_H; i--)
//	{
//		for (j = 0; j < MT9V03X_W; j--)
//		{
//			if (PostProcessing_image[i][j] > image_otsuThreshold_less)
//			{
//				PostProcessing_image[i][j] = WHITE_PIXEL;
//			}
//			else
//			{
//				PostProcessing_image[i][j] = BLACK_PIXEL;
//			}
//		}
//	}
//}

////Ŀ����Ϊ�˳�ȥһЩ��㣬��ֹ����Ԫ���жϳ�������
//void image_filter(uint8(*image)[MT9V03X_W])//��̬ѧ�˲�������˵�������ͺ͸�ʴ��˼��
//{
//	uint16 i, j;
//	uint32 num = 0;


//	for (i = 1; i < MT9V03X_H - 1; i++)
//	{
//		for (j = 1; j < (MT9V03X_W - 1); j++)
//		{
//			//ͳ�ư˸����������ֵ
//			num =
//				image[i - 1][j - 1] + image[i - 1][j] + image[i - 1][j + 1]
//				+ image[i][j - 1] + image[i][j + 1]
//				+ image[i + 1][j - 1] + image[i + 1][j] + image[i + 1][j + 1];


//			if (num >= THRESHOLD_MAX && image[i][j] == 0)
//			{

//				image[i][j] = WHITE_PIXEL;//��  ���Ը�ɺ궨�壬�������

//			}
//			if (num <= THRESHOLD_MIN && image[i][j] == 255)
//			{

//				image[i][j] = BLACK_PIXEL;//��

//			}

//		}
//	}

//}

//uint8 get_left_start(void)
//{
//	uint8 i = 0;
//	for(i = MT9V03X_W/2; i > 0; i--)
//	{
//		if ((PostProcessing_image[MT9V03X_H - 1][i] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][i - 1] == BLACK_PIXEL) || i == 1)
//		{
//			return i;
//		}
//	}
//	return 0;
//}

//uint8 get_right_start(void)
//{
//	uint8 i = 0;
//	for(i = MT9V03X_W/2; i < MT9V03X_W - 1; i++)
//	{
//		if ((PostProcessing_image[MT9V03X_H - 1][i] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][i + 1] == BLACK_PIXEL) || i == 186)
//		{
//			return i;
//		}
//	}
//	return 0;
//}

//uint8 left_point;
//uint8 longest_line_number;
//uint8 line;
//void research_longest_line()
//{
//	uint8 i,j;
//	uint8 stopflag = 0;
//	uint8 left_point = 0;
//	uint8 right_point = 0;
//	uint8 longest_line_number = MT9V03X_W/2;
//	uint8 line = 119;
//	left_point = get_left_start();
//	right_point = get_right_start();
//	stopflag = 0;
//	for (i = left_point; i < right_point; i++)
//	{
//		for (j = MT9V03X_H - 1; j < 0; j--)
//		{
//			if (j == 1)
//			{
//				longest_line_number = i;
//				line = j;
//				stopflag = 1;
//				break;
//			}
//			else if ((PostProcessing_image[j][i] == WHITE_PIXEL && PostProcessing_image[j - 1][i] == BLACK_PIXEL))
//			{
////				dead_line[i] = j;
//				if (j < line)
//				{
//					line = j;
//					longest_line_number = i;
//					break;
//				}
//			}
//			
//		}
//		if (stopflag)
//		{
//			break;
//		}
//	}
//}
//uint8 road_left[120];
//uint8 road_right[120];
//void research_road(void)
//{
//	uint8 i,j;
//	uint8 left_num = 0;
//	uint8 right_num = 0;
//	for (i = line; i < MT9V03X_H; i++)
//	{
//		for (j = longest_line_number; j > 0; j--)
//		{
//			left_num++;
//			if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j-1] == BLACK_PIXEL) || j == 1)
//			{
//				road_left[left_num - 1] = j;
//				break;
//			}
//		}
//		for (j = longest_line_number; j < MT9V03X_W - 1; j++)
//		{
//			right_num++;
//			if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j+1] == BLACK_PIXEL) || j == 186)
//			{
//				road_right[right_num - 1] = j;
//				break;
//			}
//		}
//	}
//}

//void image_progress(uint8 (*image_progress)[MT9V03X_W])
//{
//	image_postprocess();
//	research_longest_line();
//	research_road();
//}
