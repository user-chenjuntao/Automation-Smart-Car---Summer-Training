//#include "image_test.h"

////-------------------------------------------------------------------------------------------------------------------
////  ���:������ͼ����

////------------------------------------------------------------------------------------------------------------------


//uint8 while_flag = 0;


///*��������*/
//uint8 original_image[image_h][image_w];
//uint8 image_thereshold = 80;//ͼ��ָ���ֵ
//////------------------------------------------------------------------------------------------------------------------
//////  @brief      ���һ���Ҷ�ͼ��
//////  @since      v1.0 
//////------------------------------------------------------------------------------------------------------------------
//void Get_image(uint8(*mt9v03x_image)[image_w])
//{
//	#define use_num		1	//1���ǲ�ѹ����2����ѹ��һ��	
//	uint8 i = 0, j = 0, row = 0, line = 0;
//    for (i = 0; i < image_h; i += use_num)          //
//    {
//        for (j = 0; j <image_w; j += use_num)     //
//        {
//            original_image[row][line] = mt9v03x_image[i][j];//����Ĳ�����д�������ͷ�ɼ�����ͼ��
//			line++;
//        }
//        line = 0;
//        row++;
//    }
//}
////------------------------------------------------------------------------------------------------------------------
////  @brief     ��̬��ֵ
////  @since      v1.0 
////------------------------------------------------------------------------------------------------------------------
//uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)
//{
//#define GrayScale 256
//    uint16 Image_Width  = col;
//    uint16 Image_Height = row;
//    int X; uint16 Y;
//    uint8* data = image;
//    int HistGram[GrayScale] = {0};
//	
//	uint32 Amount = 0;
//    uint32 PixelBack = 0;
//    uint32 PixelIntegralBack = 0;
//    uint32 PixelIntegral = 0;
//    int32 PixelIntegralFore = 0;
//    int32 PixelFore = 0;
//    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // ��䷽��;
//    uint8 MinValue=0, MaxValue=0;
//    uint8 Threshold = 0;
//	
//	
//    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height��ΪY =Image_Height���Ա���� �ж�ֵ��
//    {
//        //Y=Image_Height;
//        for (X = 0; X < Image_Width; X++)
//        {
//        HistGram[(int)data[Y*Image_Width + X]]++; //ͳ��ÿ���Ҷ�ֵ�ĸ�����Ϣ
//        }
//    }




//    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
//    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ

//    if (MaxValue == MinValue)
//    {
//        return MaxValue;          // ͼ����ֻ��һ����ɫ
//    }
//    if (MinValue + 1 == MaxValue)
//    {
//        return MinValue;      // ͼ����ֻ�ж�����ɫ
//    }

//    for (Y = MinValue; Y <= MaxValue; Y++)
//    {
//        Amount += HistGram[Y];        //  ��������
//    }

//    PixelIntegral = 0;
//    for (Y = MinValue; Y <= MaxValue; Y++)
//    {
//        PixelIntegral += HistGram[Y] * Y;//�Ҷ�ֵ����
//    }
//    SigmaB = -1;
//    for (Y = MinValue; Y < MaxValue; Y++)
//    {
//          PixelBack = PixelBack + HistGram[Y];    //ǰ�����ص���
//          PixelFore = Amount - PixelBack;         //�������ص���
//          OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
//          OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
//          PixelIntegralBack += HistGram[Y] * Y;  //ǰ���Ҷ�ֵ
//          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
//          MicroBack = (double)PixelIntegralBack / PixelBack;//ǰ���ҶȰٷֱ�
//          MicroFore = (double)PixelIntegralFore / PixelFore;//�����ҶȰٷֱ�
//          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
//          if (Sigma > SigmaB)//����������䷽��g
//          {
//              SigmaB = Sigma;
//              Threshold = (uint8)Y;
//          }
//    }
//   return Threshold;
//}

//////-------------------------------------------------------------------------------------
//////ʹ�ô�򷨽��ж�ֵ������
//////-------------------------------------------------------------------------------------
////uint8 otsuThreshold(uint8 *image, uint16 width, uint16 height)
////{
////    #define GrayScale 256
////    int pixel_count[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ���ظ���
////    float pixel_account[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ�����ر���
////    int i,j;   
////    int pixel_sum = width * height;   //�����ص�
////    uint8 threshold = 0;
////    uint8* pixel_data = image;  //ָ���������ݵ�ָ��


////    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
////    for (i = 0; i < height; i++)
////    {
////        for (j = 0; j < width; j++)
////        {
////            pixel_count[(int)pixel_data[i * width + j]]++;  //������ֵ��Ϊ����������±�

////        }
////    }
////    float u = 0;  
////    for (i = 0; i < GrayScale; i++)
////    {
////        pixel_account[i] = (float)pixel_count[i] / pixel_sum;   //����ÿ������������ͼ���еı���  
////        u += i * pixel_account[i];  //��ƽ���Ҷ�
////    }

////      
////    float variance_max=0.0;  //�����䷽��
////    float w0 = 0, avgValue  = 0;  //w0 ǰ������ ��avgValue ǰ��ƽ���Ҷ�
////    for(int i = 0; i < 256; i++)     //ÿһ��ѭ������һ��������䷽����� (����for����Ϊ1��)
////    {  
////        w0 += pixel_account[i];  //���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷ�������ռ����ͼ��ı�����ǰ������
////        avgValue  += i * pixel_account[i];        
////        
////        float variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //��䷽��   
////        if(variance > variance_max) 
////        {  
////            variance_max = variance;  
////            threshold = i;  
////        }  
////    } 

////    return threshold;

////}
////------------------------------------------------------------------------------------------------------------------
////  @brief      ͼ���ֵ���������õ��Ǵ�򷨶�ֵ��
////  @since      v1.0 
////------------------------------------------------------------------------------------------------------------------
//uint8 bin_image[image_h][image_w];//ͼ������
//void turn_to_bin(void)
//{
//  uint8 i,j;
//	while_flag++;
////	if (while_flag == 5)
////	{
//		image_thereshold = otsuThreshold(original_image[0], image_w, image_h);
////		while_flag = 0;
////	}
//  
//  for(i = 0;i<image_h;i++)
//  {
//      for(j = 0;j<image_w;j++)
//      {
//          if(original_image[i][j]>image_thereshold)bin_image[i][j] = white_pixel;
//          else bin_image[i][j] = black_pixel;
//      }
//  }
//}




////------------------------------------------------------------------------------------------------------------------
////�������ƣ�void get_start_point(uint8 start_row)
////����˵����Ѱ�������߽�ı߽����Ϊ������ѭ������ʼ��
////����˵����������������
////------------------------------------------------------------------------------------------------------------------
//uint8 start_point_l[2] = { 0 };//�������x��yֵ
//uint8 start_point_r[2] = { 0 };//�ұ�����x��yֵ
//uint8 get_start_point(uint8 start_row)
//{
//	uint8 i = 0,l_found = 0,r_found = 0;
//	//����
//	start_point_l[0] = 0;//x
//	start_point_l[1] = 0;//y

//	start_point_r[0] = 0;//x
//	start_point_r[1] = 0;//y

//		//���м�����ߣ��������
//	for (i = image_w / 2; i > border_min; i--)
//	{
//		start_point_l[0] = i;//x
//		start_point_l[1] = start_row;//y
//		if (bin_image[start_row][i] == 255 && bin_image[start_row][i - 1] == 0)
//		{
//			l_found = 1;
//			break;
//		}
//	}

//	for (i = image_w / 2; i < border_max; i++)
//	{
//		start_point_r[0] = i;//x
//		start_point_r[1] = start_row;//y
//		if (bin_image[start_row][i] == 255 && bin_image[start_row][i + 1] == 0)
//		{
//			//printf("�ҵ��ұ����image[%d][%d]\n",start_row, i);
//			r_found = 1;
//			break;
//		}
//	}

//	if(l_found&&r_found)return 1;
//	else {
//		//printf("δ�ҵ����\n");
//		return 0;
//	} 
//}

//#define USE_num	image_h*3	//�����ҵ�������Ա��������˵300�����ܷ��£�������Щ�������ȷʵ�Ѷ����ඨ����һ��

// //��ŵ��x��y����
//uint16 points_l[(uint16)USE_num][2] = { {  0 } };//����
//uint16 points_r[(uint16)USE_num][2] = { {  0 } };//����
//uint16 dir_r[(uint16)USE_num] = { 0 };//�����洢�ұ���������
//uint16 dir_l[(uint16)USE_num] = { 0 };//�����洢�����������
//uint16 data_stastics_l = 0;//ͳ������ҵ���ĸ���
//uint16 data_stastics_r = 0;//ͳ���ұ��ҵ���ĸ���
//uint8 hightest = 0;//��ߵ�
//void search_l_r(uint16 break_flag, uint8(*image)[image_w], uint16 *l_stastic, uint16 *r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint8*hightest)
//{

//	uint8 i = 0, j = 0;

//	//��߱���
//	uint8 search_filds_l[8][2] = { {  0 } };
//	uint8 index_l = 0;
//	uint8 temp_l[8][2] = { {  0 } };
//	uint8 center_point_l[2] = {  0 };
//	uint16 l_data_statics;//ͳ�����
//	//����˸�����
//	static int8 seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
//	//{-1,-1},{0,-1},{+1,-1},
//	//{-1, 0},	     {+1, 0},
//	//{-1,+1},{0,+1},{+1,+1},
//	//�����˳ʱ��

//	//�ұ߱���
//	uint8 search_filds_r[8][2] = { {  0 } };
//	uint8 center_point_r[2] = { 0 };//���������
//	uint8 index_r = 0;//�����±�
//	uint8 temp_r[8][2] = { {  0 } };
//	uint16 r_data_statics;//ͳ���ұ�
//	//����˸�����
//	static int8 seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
//	//{-1,-1},{0,-1},{+1,-1},
//	//{-1, 0},	     {+1, 0},
//	//{-1,+1},{0,+1},{+1,+1},
//	//�������ʱ��

//	l_data_statics = *l_stastic;//ͳ���ҵ��˶��ٸ��㣬��������ѵ�ȫ��������
//	r_data_statics = *r_stastic;//ͳ���ҵ��˶��ٸ��㣬��������ѵ�ȫ��������

//	//��һ�θ��������  ���ҵ������ֵ������
//	center_point_l[0] = l_start_x;//x
//	center_point_l[1] = l_start_y;//y
//	center_point_r[0] = r_start_x;//x
//	center_point_r[1] = r_start_y;//y

//		//��������ѭ��
//	while (break_flag--)
//	{

//		//���
//		for (i = 0; i < 8; i++)//����8F����
//		{
//			search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
//			search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
//		}
//		//�����������䵽�Ѿ��ҵ��ĵ���
//		points_l[l_data_statics][0] = center_point_l[0];//x
//		points_l[l_data_statics][1] = center_point_l[1];//y
//		l_data_statics++;//������һ

//		//�ұ�
//		for (i = 0; i < 8; i++)//����8F����
//		{
//			search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
//			search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
//		}
//		//�����������䵽�Ѿ��ҵ��ĵ���
//		points_r[r_data_statics][0] = center_point_r[0];//x
//		points_r[r_data_statics][1] = center_point_r[1];//y

//		index_l = 0;//�����㣬��ʹ��
//		for (i = 0; i < 8; i++)
//		{
//			temp_l[i][0] = 0;//�����㣬��ʹ��
//			temp_l[i][1] = 0;//�����㣬��ʹ��
//		}

//		//����ж�
//		for (i = 0; i < 8; i++)
//		{
//			if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
//				&& image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
//			{
//				temp_l[index_l][0] = search_filds_l[(i)][0];
//				temp_l[index_l][1] = search_filds_l[(i)][1];
//				index_l++;
//				dir_l[l_data_statics - 1] = (i);//��¼��������
//			}

//			if (index_l)
//			{
//				//���������
//				center_point_l[0] = temp_l[0][0];//x
//				center_point_l[1] = temp_l[0][1];//y
//				for (j = 0; j < index_l; j++)
//				{
//					if (center_point_l[1] > temp_l[j][1])
//					{
//						center_point_l[0] = temp_l[j][0];//x
//						center_point_l[1] = temp_l[j][1];//y
//					}
//				}
//			}

//		}
//		if ((points_r[r_data_statics][0]== points_r[r_data_statics-1][0]&& points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
//			&& points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
//			||(points_l[l_data_statics-1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics-1][0] == points_l[l_data_statics - 3][0]
//				&& points_l[l_data_statics-1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics-1][1] == points_l[l_data_statics - 3][1]))
//		{
//			//printf("���ν���ͬһ���㣬�˳�\n");
//			break;
//		}
//		if (abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
//			&& abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2)
//			)
//		{
//			//printf("\n���������˳�\n");	
//			*hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//ȡ����ߵ�
//			//printf("\n��y=%d���˳�\n",*hightest);
//			break;
//		}
//		if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
//		{
//			printf("\n if the left is higher than the right, the left wait the right\n");	
//			continue;//�����߱��ұ߸��ˣ���ߵȴ��ұ�
//		}
//		if (dir_l[l_data_statics - 1] == 7
//			&& (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//��߱��ұ߸����Ѿ�����������
//		{
//			//printf("\n��߿�ʼ�����ˣ��ȴ��ұߣ��ȴ���... \n");
//			center_point_l[0] = points_l[l_data_statics - 1][0];//x
//			center_point_l[1] = points_l[l_data_statics - 1][1];//y
//			l_data_statics--;
//		}
//		r_data_statics++;//������һ

//		index_r = 0;//�����㣬��ʹ��
//		for (i = 0; i < 8; i++)
//		{
//			temp_r[i][0] = 0;//�����㣬��ʹ��
//			temp_r[i][1] = 0;//�����㣬��ʹ��
//		}

//		//�ұ��ж�
//		for (i = 0; i < 8; i++)
//		{
//			if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
//				&& image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
//			{
//				temp_r[index_r][0] = search_filds_r[(i)][0];
//				temp_r[index_r][1] = search_filds_r[(i)][1];
//				index_r++;//������һ
//				dir_r[r_data_statics - 1] = (i);//��¼��������
//				//printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
//			}
//			if (index_r)
//			{

//				//���������
//				center_point_r[0] = temp_r[0][0];//x
//				center_point_r[1] = temp_r[0][1];//y
//				for (j = 0; j < index_r; j++)
//				{
//					if (center_point_r[1] > temp_r[j][1])
//					{
//						center_point_r[0] = temp_r[j][0];//x
//						center_point_r[1] = temp_r[j][1];//y
//					}
//				}

//			}
//		}


//	}


//	//ȡ��ѭ������
//	*l_stastic = l_data_statics;
//	*r_stastic = r_data_statics;

//}
////------------------------------------------------------------
////�������ƣ�void get_left(uint16 total_L)
////����˵�����Ӱ�����߽�����ȡ��Ҫ�ı���
////����˵����total_L	���ҵ��ĵ������
////�������أ�Null
////------------------------------------------------------------

//uint8 l_border[image_h];//��������
//uint8 r_border[image_h];//��������
//uint8 center_line[image_h];//��������
//void get_left(uint16 total_L)
//{
//	uint8 i = 0;
//	uint16 j = 0;
//	uint8 h = 0;
//	//��ʼ��
//	for (i = 0;i<image_h;i++)
//	{
//		l_border[i] = border_min;
//	}
//	h = image_h - 2;
//	//���
//	for (j = 0; j < total_L; j++)
//	{
//		//printf("%d\n", j);
//		if (points_l[j][1] == h)
//		{
//			l_border[h] = points_l[j][0]+1;
//		}
//		else continue; //ÿ��ֻȡһ���㣬û����һ�оͲ���¼
//		h--;
//		if (h == 0) 
//		{
//			break;//�����һ���˳�
//		}
//	}
//}
////------------------------------------------------------------
////�������ƣ�void get_right(uint16 total_R)
////����˵�����Ӱ�����߽�����ȡ��Ҫ�ı���
////����˵����total_R  ���ҵ��ĵ������
////�������أ�Null
// //------------------------------------------------------------
//void get_right(uint16 total_R)
//{
//	uint8 i = 0;
//	uint16 j = 0;
//	uint8 h = 0;
//	for (i = 0; i < image_h; i++)
//	{
//		r_border[i] = border_max;//�ұ��߳�ʼ���ŵ����ұߣ�����߷ŵ�����ߣ�����������պ�����������߾ͻ����м䣬������ŵõ�������
//	}
//	h = image_h - 2;
//	//�ұ�
//	for (j = 0; j < total_R; j++)
//	{
//		if (points_r[j][1] == h)
//		{
//			r_border[h] = points_r[j][0] - 1;
//		}
//		else continue;//ÿ��ֻȡһ���㣬û����һ�оͲ���¼
//		h--;
//		if (h == 0)break;//�����һ���˳�
//	}
//}

////�������ͺ͸�ʴ����ֵ����
//#define threshold_max	255*5//�˲����ɸ����Լ����������
//#define threshold_min	255*2//�˲����ɸ����Լ����������
//void image_filter(uint8(*bin_image)[image_w])//��̬ѧ�˲�������˵�������ͺ͸�ʴ��˼��
//{
//	uint16 i, j;
//	uint32 num = 0;


//	for (i = 1; i < image_h - 1; i++)
//	{
//		for (j = 1; j < (image_w - 1); j++)
//		{
//			//ͳ�ư˸����������ֵ
//			num =
//				bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
//				+ bin_image[i][j - 1] + bin_image[i][j + 1]
//				+ bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];


//			if (num >= threshold_max && bin_image[i][j] == 0)
//			{

//				bin_image[i][j] = 255;//��  ���Ը�ɺ궨�壬�������

//			}
//			if (num <= threshold_min && bin_image[i][j] == 255)
//			{

//				bin_image[i][j] = 0;//��

//			}

//		}
//	}

//}

////------------------------------------------------------------------------------------------------------------------
////�������ƣ�void image_draw_rectan(uint8(*image)[image_w])
////����˵������ͼ��һ���ڿ�
////����˵����uint8(*image)[image_w]	ͼ���׵�ַ
////�������أ���
////------------------------------------------------------------------------------------------------------------------
//void image_draw_rectan(uint8(*image)[image_w])
//{

//	uint8 i = 0;
//	for (i = 0; i < image_h; i++)
//	{
//		image[i][0] = 0;
//		image[i][1] = 0;
//		image[i][image_w - 1] = 0;
//		image[i][image_w - 2] = 0;

//	}
//	for (i = 0; i < image_w; i++)
//	{
//		image[0][i] = 0;
//		image[1][i] = 0;
//		//image[image_h-1][i] = 0;

//	}
//}

//uint8 break_point_l[8];
//uint8 break_point_r[8];
//uint8 center_value = 94;
////------------------------------------------------------------------------------------------------------------------
////�������ƣ�void image_process(void)
////����˵�������մ�����
////����˵������
////�������أ���
////------------------------------------------------------------------------------------------------------------------
//void image_process(void)
//{
//	uint16 i;
//	center_value = 0;
//	uint16 sum;
//	uint8 hightest = 0;//����һ�������
//	/*�������ߵ����õ�*/
//	Get_image(mt9v03x_image);
//	turn_to_bin();
//	/*��ȡ�����߽�*/
//	image_filter(bin_image);//�˲�
//	image_draw_rectan(bin_image);//Ԥ����
//	//����
//	data_stastics_l = 0;
//	data_stastics_r = 0;
//	sum = 0;
//	if (get_start_point(image_h - 2))//�ҵ�����ˣ���ִ�а�����û�ҵ���һֱ��
//	{
//		printf("Eight Areas Start\n");
//		search_l_r((uint16)USE_num, bin_image, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &hightest);
//		printf("Eight Areas End\n");
//		// ����ȡ�ı߽�������ȡ���� �� ��������������õı���
//		
//		get_left(data_stastics_l);
//		get_right(data_stastics_r);
//		crossing_add(crossing_handle_l(), crossing_handle_r());
////		cross_fill(bin_image,r_border,l_border,data_stastics_l,data_stastics_r,dir_l,dir_r,points_l,points_r);
//		
//	}


//	//��ʾͼ��
//	ips200_show_gray_image(0, 50, bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);

//		//��������ѭ�����������߽��
//		for (i = 0; i < data_stastics_l; i++)
//		{
//			ips200_draw_point(points_l[i][0]+2, points_l[i][1] + 50, uesr_BLUE);//��ʾ���
//		}
//		for (i = 0; i < data_stastics_r; i++)
//		{
//			ips200_draw_point(points_r[i][0]-2, points_r[i][1] + 50, uesr_RED);//��ʾ���
//		}

//		for (i = hightest; i < image_h-1; i++)
//		{
//			center_line[i] = (l_border[i] + r_border[i]) >> 1;//������
//			//�������������󣬲����ǲ��߻�����״̬����ȫ�����ʹ��һ����ߣ����������������ܸ����������
//			//��ȻҲ�ж�����ߵ��ҷ������Ǹ��˸о��ܷ�����������
//			ips200_draw_point(center_line[i], i + 50, uesr_RED);//��ʾ��� ��ʾ����	
//			ips200_draw_point(l_border[i], i + 50, uesr_GREEN);//��ʾ��� ��ʾ�����
//			ips200_draw_point(r_border[i], i + 50, uesr_BLUE);//��ʾ��� ��ʾ�ұ���
//		}
//		for (i = image_h - 17; i < image_h - 1; i++)
//		{
//			if (center_line[i])
//				sum += center_line[i];
//		}
//		center_value = sum / 16;
//		memset(break_point_l, 0, sizeof(break_point_l));
//		memset(break_point_r, 0, sizeof(break_point_r));
//		
//}


/////** 
////* @brief ��С���˷�
////* @param uint8 begin				�������
////* @param uint8 end					�����յ�
////* @param uint8 *border				������Ҫ����б�ʵı߽��׵�ַ
////*  @see CTest		Slope_Calculate(start, end, border);//б��
////* @return ����˵��
////*     -<em>false</em> fail
////*     -<em>true</em> succeed
////*/
////float Slope_Calculate(uint8 begin, uint8 end, uint8 *border)
////{
////	float xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
////	int16 i = 0;
////	float result = 0;
////	static float resultlast;

////	for (i = begin; i < end; i++)
////	{
////		xsum += i;
////		ysum += border[i];
////		xysum += i * (border[i]);
////		x2sum += i * i;

////	}
////	if ((end - begin)*x2sum - xsum * xsum) //�жϳ����Ƿ�Ϊ��
////	{
////		result = ((end - begin)*xysum - xsum * ysum) / ((end - begin)*x2sum - xsum * xsum);
////		resultlast = result;
////	}
////	else
////	{
////		result = resultlast;
////	}
////	return result;
////}

/////** 
////* @brief ����б�ʽؾ�
////* @param uint8 start				�������
////* @param uint8 end					�����յ�
////* @param uint8 *border				������Ҫ����б�ʵı߽�
////* @param float *slope_rate			����б�ʵ�ַ
////* @param float *intercept			����ؾ��ַ
////*  @see CTest		calculate_s_i(start, end, r_border, &slope_l_rate, &intercept_l);
////* @return ����˵��
////*     -<em>false</em> fail
////*     -<em>true</em> succeed
////*/
////void calculate_s_i(uint8 start, uint8 end, uint8 *border, float *slope_rate, float *intercept)
////{
////	uint16 i, num = 0;
////	uint16 xsum = 0, ysum = 0;
////	float y_average, x_average;

////	num = 0;
////	xsum = 0;
////	ysum = 0;
////	y_average = 0;
////	x_average = 0;
////	for (i = start; i < end; i++)
////	{
////		xsum += i;
////		ysum += border[i];
////		num++;
////	}

////	//�������ƽ����
////	if (num)
////	{
////		x_average = (float)(xsum / num);
////		y_average = (float)(ysum / num);

////	}

////	/*����б��*/
////	*slope_rate = Slope_Calculate(start, end, border);//б��
////	*intercept = y_average - (*slope_rate)*x_average;//�ؾ�
////}

/////** 
////* @brief ʮ�ֲ��ߺ���
////* @param uint8(*image)[image_w]		�����ֵͼ��
////* @param uint8 *l_border			������߽��׵�ַ
////* @param uint8 *r_border			�����ұ߽��׵�ַ
////* @param uint16 total_num_l			�������ѭ���ܴ���
////* @param uint16 total_num_r			�����ұ�ѭ���ܴ���
////* @param uint16 *dir_l				����������������׵�ַ
////* @param uint16 *dir_r				�����ұ����������׵�ַ
////* @param uint16(*points_l)[2]		������������׵�ַ
////* @param uint16(*points_r)[2]		�����ұ������׵�ַ
////*  @see CTest		cross_fill(image,l_border, r_border, data_statics_l, data_statics_r, dir_l, dir_r, points_l, points_r);
////* @return ����˵��
////*     -<em>false</em> fail
////*     -<em>true</em> succeed
//// */
////void cross_fill(uint8(*image)[image_w], uint8 *l_border, uint8 *r_border, uint16 total_num_l, uint16 total_num_r,
////										 uint16 *dir_l, uint16 *dir_r, uint16(*points_l)[2], uint16(*points_r)[2])
////{
////	uint8 i;
////	uint8 break_num_l = 0;
////	uint8 break_num_r = 0;
////	uint8 start, end;
////	float slope_l_rate = 0, intercept_l = 0;
////	//��ʮ��
////	for (i = 1; i < total_num_l; i++)
////	{
////		if (dir_l[i - 1] == 4 && dir_l[i] == 4 && dir_l[i + 3] == 6 && dir_l[i + 5] == 6 && dir_l[i + 7] == 6)
////		{
////			break_num_l = points_l[i][1];//����y����
////			printf("brea_knum-L:%d\n", break_num_l);
////			printf("I:%d\n", i);
//////			printf("ʮ�ֱ�־λ��1\n");
////			break;
////		}
////	}
////	for (i = 1; i < total_num_r; i++)
////	{
////		if (dir_r[i - 1] == 4 && dir_r[i] == 4 && dir_r[i + 3] == 6 && dir_r[i + 5] == 6 && dir_r[i + 7] == 6)
////		{
////			break_num_r = points_r[i][1];//����y����
////			printf("brea_knum-R:%d\n", break_num_r);
////			printf("I:%d\n", i);
//////			printf("ʮ�ֱ�־λ��1\n");
////			break;
////		}
////	}
////	if (break_num_l&&break_num_r&&image[image_h - 1][4] && image[image_h - 1][image_w - 4])//�����������򶼷�������
////	{
////		//����б��
////		start = break_num_l - 15;
////		start = limit_a_b(start, 0, image_h);
////		end = break_num_l - 5;
////		calculate_s_i(start, end, l_border, &slope_l_rate, &intercept_l);
////		//printf("slope_l_rate:%d\nintercept_l:%d\n", slope_l_rate, intercept_l);
////		for (i = break_num_l - 5; i < image_h - 1; i++)
////		{
////			l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
////			l_border[i] = limit_a_b(l_border[i], border_min, border_max);//�޷�
////		}

////		//����б��
////		start = break_num_r - 15;//���
////		start = limit_a_b(start, 0, image_h);//�޷�
////		end = break_num_r - 5;//�յ�
////		calculate_s_i(start, end, r_border, &slope_l_rate, &intercept_l);
////		//printf("slope_l_rate:%d\nintercept_l:%d\n", slope_l_rate, intercept_l);
////		for (i = break_num_r - 5; i < image_h - 1; i++)
////		{
////			r_border[i] = slope_l_rate * (i)+intercept_l;
////			r_border[i] = limit_a_b(r_border[i], border_min, border_max);
////		}


////	}

////}


////------------------------------------------------------------------------------------------------------------------
////�������ƣ�uint8 crossing_handle(void)
////����˵����ʮ�ִ�����
////����˵������
////�������أ�
////------------------------------------------------------------------------------------------------------------------

//uint8 crossing_handle_l(void)
//{
//	uint8 i = 0;
//	uint8 break_num_l = 0;
//	for (i = 0; i < image_h-2; i++)
//	{
//		if (l_border[i] != 0)
//		{
//			if (l_border[i] - l_border[i+1] >= 5)
//			{
//				break_point_l[break_num_l] = i;
//				break_num_l++;
//			}
//			else if (l_border[i] - l_border[i+1] <= -5)
//			{
//				break_point_l[break_num_l] = i + 1;
//				break_num_l++;
//			}
//		}
//		else
//		{
//			break;
//		}
//	}
//	if (break_num_l)
//	{
//		return break_num_l;
//	}
//	else
//	{
//		return 0;
//	}
//}

//uint8 crossing_handle_r(void)
//{
//	uint8 i = 0;
//	uint8 break_num_r = 0;
//	for (i = 0; i < image_h-2; i++)
//	{
//		if (r_border[i] != 0)
//		{
//			if (r_border[i] - r_border[i+1] >= 5)
//			{
//				break_point_r[break_num_r] = i + 1;
//				break_num_r++;
//			}
//			else if (r_border[i] - r_border[i+1] <= -5)
//			{
//				break_point_r[break_num_r] = i;
//				break_num_r++;
//			}
//		}
//		else
//		{
//			break;
//		}
//	}
//	if (break_num_r)
//	{
//		return break_num_r;
//	}
//	else
//	{
//		return 0;
//	}
//}

////------------------------------------------------------------------------------------------------------------------
////�������ƣ�uint8 crossing_handle(void)
////����˵������С���˷�����
////����˵������
////�������أ�
////------------------------------------------------------------------------------------------------------------------

//void least_squares(uint8 begin, uint8 end, uint8 *border, float *xielv, float *jieju)
//{
//	float sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
//	int16 i = 0;
//	*xielv = 0;
//	*jieju = 0;

//	for (i = begin; i < end; i++)
//	{
//		sum_x += i;
//		sum_y += border[i];
//		sum_xy += i * (border[i]);
//		sum_x2 += i * i;

//	}
//	if ((end - begin)*sum_x2 - sum_x * sum_x) //�жϳ����Ƿ�Ϊ��
//	{
//		*xielv = ((end - begin)*sum_xy - sum_x * sum_y) / ((end - begin)*sum_x2 - sum_x * sum_x);

//	}
//	*jieju = (sum_y - (*xielv) * sum_x) / (end - begin);
//}
//void xieji(uint8 begin, uint8 end, float *xielv, float *jieju,uint8 flag)
//{
//	*xielv = 0;
//	*jieju = 0;
//	if (flag == 0)
//	{
//		*xielv = (l_border[end] - l_border[begin]) / end - begin;
//		*jieju = l_border[begin] - (*xielv) * begin;
//	}
//	else if (flag == 1)
//	{
//		*xielv = (r_border[end] - r_border[begin]) / end - begin;
//		*jieju = r_border[begin] - (*xielv) * begin;
//	}
//}

////void loss_line(uint16 total_num_l, uint16 total_num_r)
////{
////	uint8 i;
////	uint8 num_l,num_r;
////	num_l = 0;
////	num_r = 0;
////	for (i = 1; i < total_num_l; i++)
////	{
////		if (dir_l[i] == 4)
////		{
////			num_l++;
////		}
////	}
////	for (i = 0; i < total_num_r; i++)
////	{
////		if (dir_r[i] == 4)
////		{
////			num_r++;
////		}
////	}
////	if (4*num_l > 3*total_num_l)
////	{
////		
////		//��Ϊ����߶�ʧ
////	}
////}

//void crossing_add(uint8 num_l, uint8 num_r)
//{
//	float xielv_l,jieju_l;
//	float xielv_r,jieju_r;
//	uint16 i =0;
//	if (num_l == 0 && num_r ==0)
//	{
//		
//	}
//	if (num_l == 1)
//	{
//		least_squares(break_point_l[0]-15, break_point_l[0] - 5, l_border, &xielv_l, &jieju_l);
//		for (i = break_point_l[0] - 5; i < image_h - 1; i++)
//		{
//			l_border[i] = xielv_l*i + jieju_l;
//			if (l_border[i] <= 2)
//			{
//				l_border[i] = 2;
//			}
//		}
//	}
//	else if (num_l == 2)
//	{
//		xieji(break_point_l[1] , break_point_l[0], &xielv_l, &jieju_l, 0);
//		for (i = break_point_l[1]; i < break_point_l[0]; i++)
//		{
//			l_border[i] = xielv_l*i + jieju_l;
//			if (l_border[i] <= 2)
//			{
//				l_border[i] = 2;
//			}
//		}
//	}
//	if (num_r == 1)
//	{
//		least_squares(break_point_r[0]-15, break_point_r[0] - 5, r_border, &xielv_r, &jieju_r);
//		for (i = break_point_r[0] - 5; i < image_h - 1; i++)
//		{
//			r_border[i] = xielv_r*i + jieju_r;
//			if (r_border[i] >= 185)
//			{
//				r_border[i] = 185;
//			}
//		}
//	}
//	else if (num_r == 2)
//	{
//		xieji(break_point_r[1] , break_point_r[0], &xielv_r, &jieju_r, 0);
//		for (i = break_point_r[1]; i < break_point_r[0]; i++)
//		{
//			r_border[i] = xielv_r*i + jieju_r;
//			if (r_border[i] >= 185)
//			{
//				r_border[i] = 185;
//			}
//		}
//	}
//}

////------------------------------------------------------------------------------------------------------------------
////�������ƣ�int my_abs(int value)
////����˵���������ֵ
////------------------------------------------------------------------------------------------------------------------

////int my_abs(int value)
////{
////if(value>=0) return value;
////else return -value;
////}

//int16 limit_a_b(int16 x, int a, int b)
//{
//    if(x<a) x = a;
//    if(x>b) x = b;
//    return x;
//}

////------------------------------------------------------------------------------------------------------------------
////�������ƣ�int16 limit(int16 x, int16 y)
////����˵������x,y�е���Сֵ
////�������أ�������ֵ�е���Сֵ
////------------------------------------------------------------------------------------------------------------------
//int16 limit1(int16 x, int16 y)
//{
//	if (x > y)             return y;
//	else if (x < -y)       return -y;
//	else                return x;
//}
