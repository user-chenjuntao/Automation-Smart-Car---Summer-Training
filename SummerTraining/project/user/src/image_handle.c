//#include "image_my.h"

//uint8 image_otsuThreshold_less;
//uint8 PostProcessing_image[MT9V03X_H][MT9V03X_W];                                      //用于存放二值化处理后的图像数组
//uint8 Left_Lost_Time = 0;                                                              //记录左边界无效点
//uint8 Right_Lost_Time = 0;                                                             //记录右边界无效点
//uint8 sum_average_value = 0;                                                           //记录
//uint8 image_flag = 0;

///*********************************************************
//** //图像权重数组  //权重越靠上，车转弯越早
//*********************************************************/
//uint8 mid_weight_list[120] = 
//{
//    1,1,1,1,1,1,1,1,1,1,
//    1,1,1,1,1,1,1,1,1,1,
//    1,1,1,1,1,1,1,1,1,1,
//    1,1,1,1,1,1,1,1,1,1,
//    1,1,1,1,1,1,1,1,1,1,
//    6,6,6,6,6,6,6,6,6,6,
//    7,8,9,10,11,12,13,14,15,16,
//    17,18,19,20,20,20,20,19,18,17,
//    16,15,14,13,12,11,10,9,8,7,
//    6,6,6,6,6,6,6,6,6,6,
//    1,1,1,1,1,1,1,1,1,1,
//    1,1,1,1,1,1,1,1,1,1,
//};

//uint8 final_mid_line = MID_W;   // 最终输出的中线值
//uint8 last_mid_line = MID_W;    // 上次中线值

////-------------------------------------------------------------------------------------
////使用大津法进行二值化处理
////-------------------------------------------------------------------------------------
//uint8 otsuThreshold_less(uint8 *image, uint16 width, uint16 height)
//{
//    #define GrayScale 256
//    int pixel_count[GrayScale] = {0};//每个灰度值所占像素个数
//    float pixel_account[GrayScale] = {0};//每个灰度值所占总像素比例
//    int i,j;   
//    int pixel_sum = width * height;   //总像素点
//    uint8 threshold = 0;
//    uint8* pixel_data = image;  //指向像素数据的指针


//    //统计灰度级中每个像素在整幅图像中的个数  
//    for (i = 0; i < height; i++)
//    {
//        for (j = 0; j < width; j++)
//        {
//            pixel_count[(int)pixel_data[i * width + j]]++;  //将像素值作为计数数组的下标

//        }
//    }
//    float u = 0;  
//    for (i = 0; i < GrayScale; i++)
//    {
//        pixel_account[i] = (float)pixel_count[i] / pixel_sum;   //计算每个像素在整幅图像中的比例  
//        u += i * pixel_account[i];  //总平均灰度
//    }

//      
//    float variance_max=0.0;  //最大类间方差
//	float variance;
//    float w0 = 0, avgValue  = 0;  //w0 前景比例 ，avgValue 前景平均灰度
//    for(int i = 0; i < 256; i++)     //每一次循环都是一次完整类间方差计算 (两个for叠加为1个)
//    {  
//        w0 += pixel_account[i];  //假设当前灰度i为阈值, 0~i 灰度像素所占整幅图像的比例即前景比例
//        avgValue  += i * pixel_account[i];        
//        if (w0 <= 0 || w0 >= 1)
//			continue;
//        variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //类间方差   
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
//	for (i = 0; i < MT9V03X_H; i++)
//	{
//		for (j = 0; j < MT9V03X_W; j++)
//		{
//			PostProcessing_image[i][j] = (mt9v03x_image[i][j] > image_otsuThreshold_less) ? WHITE_PIXEL : BLACK_PIXEL;
//		}
//	}
//}

////目的是为了出去一些噪点，防止对于元素判断出现问题
//void image_filter(uint8(*image)[MT9V03X_W])//形态学滤波，简单来说就是膨胀和腐蚀的思想
//{
//	uint16 i, j;
//	uint32 num = 0;


//	for (i = 1; i < MT9V03X_H - 1; i++)
//	{
//		for (j = 1; j < (MT9V03X_W - 1); j++)
//		{
//			//统计八个方向的像素值
//			num =
//				image[i - 1][j - 1] + image[i - 1][j] + image[i - 1][j + 1]
//				+ image[i][j - 1] + image[i][j + 1]
//				+ image[i + 1][j - 1] + image[i + 1][j] + image[i + 1][j + 1];


//			if (num >= THRESHOLD_MAX && image[i][j] == 0)
//			{

//				image[i][j] = WHITE_PIXEL;//白  可以搞成宏定义，方便更改

//			}
//			if (num <= THRESHOLD_MIN && image[i][j] == 255)
//			{

//				image[i][j] = BLACK_PIXEL;//黑

//			}

//		}
//	}

//}
// //&& PostProcessing_image[MT9V03X_H - 1][i - 2] == BLACK_PIXEL&& PostProcessing_image[MT9V03X_H - 1][i + 2] == BLACK_PIXEL
//uint8 get_left_start(void)
//{
//	uint8 i = 0;
//	for(i = MT9V03X_W/2; i > 1; i--)
//	{
//		if ((PostProcessing_image[MT9V03X_H - 1][i] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][i - 1] == BLACK_PIXEL) || i == 2)
//		{
//			return i;
//		}
//	}
//	return 2;
//}

//uint8 get_right_start(void)
//{
//	uint8 i = 0;
//	for(i = MT9V03X_W/2; i < MT9V03X_W - 2; i++)
//	{
//		if ((PostProcessing_image[MT9V03X_H - 1][i] == WHITE_PIXEL && PostProcessing_image[MT9V03X_H - 1][i + 1] == BLACK_PIXEL ) || i == 185)
//		{
//			return i;
//		}
//	}
//	return 185;
//}

//uint8 left_point,right_point;
//uint8 longest_line_number;           //记录最长白列的列位置，x方向上
//uint8 line;                          //记录最长白列的长度，0代表最长，y方向，行

//void research_longest_line(void)
//{
//    uint8 i, j;
//    uint8 longest_length = 0;  // 记录最长白色列的长度
//    
//    left_point = get_left_start();
//    right_point = get_right_start();
//    
//    longest_line_number = MT9V03X_W/2;  // 默认值
//    line = 119;  // 默认值（119是一个任意选择，代表较长的长度）
//    
////    // 确保左右边界合理
////    if (left_point >= right_point)
////	{
////		// 设置安全默认值或添加错误处理
////		longest_line_number = MT9V03X_W / 2;
////		line = 60;
////		return;
////	}
//    
//    for (i = left_point; i < right_point; i++)
//    {
//        uint8 current_length = 0;  // 当前列的白色长度
//        uint8 start_row = 0;       // 当前列白色区域的起始行
//        
//        // 查找当前列白色区域的起始行
//        for (j = MT9V03X_H - 1; j > 0; j--)
//        {
//            if (PostProcessing_image[j][i] == WHITE_PIXEL && 
//                PostProcessing_image[j - 1][i] == BLACK_PIXEL)
//            {
//                start_row = j;
//                current_length = MT9V03X_H - start_row;  // 计算白色列的长度
//                break;
//            }
//        }
//        
//        // 如果没有找到黑白边界，但整列都是白色
//        if (j == 0 && PostProcessing_image[0][i] == WHITE_PIXEL)
//        {
//            start_row = 0;
//            current_length = MT9V03X_H;  // 整列都是白色
//        }
//        
//        // 更新最长白色列信息
//        if (current_length > longest_length)
//        {
//            longest_length = current_length;
//            longest_line_number = i;
//            line = start_row;  // 记录白色区域的起始行
//        }
//    }
//}
//uint8 road_left[120] = {0};         //记录x，或列数
//uint8 break_point_l[6][2] ={{0}};
//uint8 break_num_left;
//uint8 break_point_r[6][2] = {{0}};
//uint8 break_num_right;
//uint8 road_right[120] = {0};       
//uint8 left_flag_break;
//uint8 right_flag_break;
//void research_road(void)
//{
//	uint8 i,j;
//	left_flag_break = 0;
//	right_flag_break = 0;
//	break_num_left = 0;
//	break_num_right = 0; ////
//	for (i = MT9V03X_H - 1; i >= line; i--)
//	{
//		for (j = longest_line_number; j > 1; j--)       //列
//		{
//			if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j-1] == BLACK_PIXEL && PostProcessing_image[i][j-2] == BLACK_PIXEL) || j == 2)
//			{
//				
//				road_left[i] = j;
//				if (road_left[i] == 2)
//				{
//					Left_Lost_Time++;
//				}
//				left_flag_break++;
//				if (left_flag_break >1)
//					left_flag_break = 1;
//				break;
//			}
//		}
//		for (j = longest_line_number; j < MT9V03X_W - 2; j++)
//		{
//			if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j+1] == BLACK_PIXEL && PostProcessing_image[i][j+2] == BLACK_PIXEL) || j == 185)
//			{
//				road_right[i] = j;
//				if (road_right[i] == 185)
//				{
//					Right_Lost_Time++;
//				}
//				right_flag_break++;
//				if (right_flag_break >1)
//					right_flag_break = 1;
//				break;
//			}
//		}
//	}
//}





///*-------------------------------------------------------------------------------------------------------------------
//  @brief     左下角点检测
//  @param     起始行，终止行
//  @return    返回角点所在的行数，找不到返回0
//  Sample     left_down_guai[0]=Find_Left_Down_Point(MT9V03X_H-1,20);
//  @note      角点检测阈值可根据实际值更改
//-------------------------------------------------------------------------------------------------------------------*/
//int Find_Left_Down_Point(int start,int end)//找左下角点，返回值是角点所在的行数
//{
//    int i,t;
//    int left_down_line=0;
//    if(Left_Lost_Time>=0.8*MT9V03X_H)//大部分都丢线，没有拐点判断的意义
//       return left_down_line;
//    if(start<end)//--访问，要保证start>end
//    {
//        t=start;
//        start=end;
//        end=t;
//    }
//    if(start>=MT9V03X_H-1-5)//下面5行上面5行数据不稳定，不能作为边界点来判断，舍弃
//        start=MT9V03X_H-1-5;//另一方面，当判断第i行时，会访问到i+3和i-4行，防止越界
//    if(end<=line)
//        end=line;
//    if(end<=5)
//       end=5;
//    for(i=start;i>=end;i--)
//    {
//        if(left_down_line==0&&//只找第一个符合条件的点
//           abs(road_left[i]-road_left[i+1])<=5&&//角点的阈值可以更改
//           abs(road_left[i+1]-road_left[i+2])<=5&&  
//           abs(road_left[i+2]-road_left[i+3])<=5&&
//              (road_left[i]-road_left[i-2])>=5&&
//              (road_left[i]-road_left[i-3])>=10&&
//              (road_left[i]-road_left[i-4])>=10)
//        {
//            left_down_line=i;//获取行数即可
//            break;
//        }
//    }
//    return left_down_line;
//}

///*-------------------------------------------------------------------------------------------------------------------
//  @brief     左上角点检测
//  @param     起始行，终止行
//  @return    返回角点所在的行数，找不到返回0
//  Sample     left_down_guai[0]=Find_Left_Down_Point(MT9V03X_H-1,20);
//  @note      角点检测阈值可根据实际值更改
//-------------------------------------------------------------------------------------------------------------------*/
//int Find_Left_Up_Point(int start,int end)//找左下角点，返回值是角点所在的行数
//{
//    int i,t;
//    int left_up_line=0;
//    if(Left_Lost_Time>=0.8*MT9V03X_H)//大部分都丢线，没有拐点判断的意义
//       return left_up_line;
//    if(start<end)//--访问，要保证start>end
//    {
//        t=start;
//        start=end;
//        end=t;
//    }
//    if(start>=MT9V03X_H-1-5)//下面5行上面5行数据不稳定，不能作为边界点来判断，舍弃
//        start=MT9V03X_H-1-5;//另一方面，当判断第i行时，会访问到i+3和i-4行，防止越界
//    if(end<=line)
//        end=line;
//    if(end<=5)
//       end=5;
//    for(i=start;i>=end;i--)
//    {
//        if(left_up_line==0&&//只找第一个符合条件的点
//           abs(road_left[i]-road_left[i-1])<=5&&//角点的阈值可以更改
//           abs(road_left[i-1]-road_left[i-2])<=5&&  
//           abs(road_left[i-2]-road_left[i-3])<=5&&
//              (road_left[i]-road_left[i+2])>=5&&
//              (road_left[i]-road_left[i+3])>=10&&
//              (road_left[i]-road_left[i+4])>=10)
//        {
//            left_up_line=i;//获取行数即可
//            break;
//        }
//    }
//    return left_up_line;
//}

///*-------------------------------------------------------------------------------------------------------------------
//  @brief     右下角点检测
//  @param     起始行，终止行
//  @return    返回角点所在的行数，找不到返回0
//  Sample     left_down_guai[0]=Find_Left_Down_Point(MT9V03X_H-1,20);
//  @note      角点检测阈值可根据实际值更改
//-------------------------------------------------------------------------------------------------------------------*/
//int Find_Right_Down_Point(int start,int end)//找左下角点，返回值是角点所在的行数
//{
//    int i,t;
//    int right_down_line=0;
//    if(Right_Lost_Time>=0.8*MT9V03X_H)//大部分都丢线，没有拐点判断的意义
//       return right_down_line;
//    if(start<end)//--访问，要保证start>end
//    {
//        t=start;
//        start=end;
//        end=t;
//    }
//    if(start>=MT9V03X_H-1-5)//下面5行上面5行数据不稳定，不能作为边界点来判断，舍弃
//        start=MT9V03X_H-1-5;//另一方面，当判断第i行时，会访问到i+3和i-4行，防止越界
//    if(end<=line)
//        end=line;
//    if(end<=5)
//       end=5;
//    for(i=start;i>=end;i--)
//    {
//        if(right_down_line==0&&//只找第一个符合条件的点
//           abs(road_right[i]-road_right[i+1])<=5&&//角点的阈值可以更改
//           abs(road_right[i+1]-road_right[i+2])<=5&&  
//           abs(road_right[i+2]-road_right[i+3])<=5&&
//              (road_right[i-2]-road_right[i])>=5&&
//              (road_right[i-3]-road_right[i])>=10&&
//              (road_right[i-4]-road_right[i])>=10)
//        {
//            right_down_line=i;//获取行数即可
//            break;
//        }
//    }
//    return right_down_line;
//}

///*-------------------------------------------------------------------------------------------------------------------
//  @brief     右上角点检测
//  @param     起始行，终止行
//  @return    返回角点所在的行数，找不到返回0
//  Sample     left_down_guai[0]=Find_Left_Down_Point(MT9V03X_H-1,20);
//  @note      角点检测阈值可根据实际值更改
//-------------------------------------------------------------------------------------------------------------------*/
//int Find_Right_Up_Point(int start,int end)//找左下角点，返回值是角点所在的行数
//{
//    int i,t;
//    int right_up_line=0;
//    if(Right_Lost_Time>=0.8*MT9V03X_H)//大部分都丢线，没有拐点判断的意义
//       return right_up_line;
//    if(start<end)//--访问，要保证start>end
//    {
//        t=start;
//        start=end;
//        end=t;
//    }
//    if(start>=MT9V03X_H-1-5)//下面5行上面5行数据不稳定，不能作为边界点来判断，舍弃
//        start=MT9V03X_H-1-5;//另一方面，当判断第i行时，会访问到i+3和i-4行，防止越界
//    if(end<=line)
//        end=line;
//    if(end<=5)
//       end=5;
//    for(i=start;i>=end;i--)
//    {
//        if(right_up_line==0&&//只找第一个符合条件的点
//           abs(road_right[i]-road_right[i-1])<=5&&//角点的阈值可以更改
//           abs(road_right[i-1]-road_right[i-2])<=5&&  
//           abs(road_right[i-2]-road_right[i-3])<=5&&
//              (road_right[i+2]-road_right[i])>=5&&
//              (road_right[i+3]-road_right[i])>=10&&
//              (road_right[i+4]-road_right[i])>=10)
//        {
//            right_up_line=i;//获取行数即可
//            break;
//        }
//    }
//    return right_up_line;
//}


//uint8 l_d_num = 0;
//uint8 l_u_num = 0;
//uint8 r_d_num = 0;
//uint8 r_u_num = 0;
//uint8 center_line[120];
//uint8 straight_flag = 0;
//void image_process(void)
//{
//	uint16 value_count;
////	uint8 proof;
//	
//	image_postprocess();
//	research_longest_line();
//	research_road();
//	l_d_num = Find_Left_Down_Point(MT9V03X_H-1,line);
//	l_u_num = Find_Left_Up_Point(MT9V03X_H-1,line);
//	r_d_num = Find_Right_Down_Point(MT9V03X_H-1,line);
//	r_u_num = Find_Right_Up_Point(MT9V03X_H-1,line);
//	if (l_d_num > 0 && l_d_num < l_u_num)
//	{
//		l_d_num = 0;
//	}
//	if (r_d_num > 0 && r_d_num < r_u_num)
//	{
//		r_d_num = 0;
//	}
//	crossing_add(l_d_num,l_u_num,r_d_num,r_u_num);
//	straight_line_handle();
//	ips200_show_gray_image(0, 50, PostProcessing_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
//	for (uint8 i = 0; i < MT9V03X_H; i++)
//	{
//		ips200_draw_point(longest_line_number, i+50,RGB565_YELLOW);
//	}
//	for (uint8 i = line; i < MT9V03X_H; i++)
//	{
//		ips200_draw_point(road_left[i], i+50,RGB565_GREEN);
//		ips200_draw_point(road_right[i], i+50,RGB565_BLUE);
//		center_line[i] = (road_left[i] + road_right[i]) >> 1;//求中线
//		ips200_draw_point(center_line[i], i+50,RGB565_RED);
//	}
//	final_mid_line = find_mid_line_weight();
//	
//	ips200_draw_point(road_left[l_d_num],l_d_num+50,RGB565_RED);
//	ips200_draw_point(road_left[l_u_num],l_u_num+50,RGB565_RED);
//	ips200_draw_point(road_right[r_d_num],r_d_num+50,RGB565_RED);
//	ips200_draw_point(road_right[r_u_num],r_u_num+50,RGB565_RED);

//	ips200_show_uint(0,208,l_d_num,3);
//	ips200_show_uint(30,208,l_u_num,3);
//	ips200_show_uint(60,208,r_d_num,3);
//	ips200_show_uint(90,208,r_u_num,3);
//	ips200_show_uint(120,208,straight_flag,1);
//	ips200_show_uint(150,208,final_mid_line,3);
//	
//	
//	
//	break_num_left = 0;
//	break_num_right = 0;
//	Left_Lost_Time = 0;
//	Right_Lost_Time = 0;
//	
//	memset(road_left, 0, sizeof(road_left));
//	memset(road_right, 0, sizeof(road_right));
//	
//	memset(center_line, 0, sizeof(center_line));
//		
//}



////------------------------------------------------------------------------------------------------------------------
////函数名称：uint8 crossing_handle(void)
////功能说明：最小二乘法函数
////参数说明：无
////函数返回：
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
//	if ((end - begin)*sum_x2 - sum_x * sum_x) //判断除数是否为零
//	{
//		*xielv = ((end - begin)*sum_xy - sum_x * sum_y) / ((end - begin)*sum_x2 - sum_x * sum_x);

//	}
//	*jieju = (sum_y - (*xielv) * sum_x) / (end - begin);
//}
//void xieji(uint8 begin, uint8 end, float *xielv, float *jieju, uint8 y_begin, uint8 y_end)
//{
//    if (end == begin)  // 避免除零错误
//    {
//        *xielv = 0;
//        *jieju = y_begin;
//        return;
//    }
//    
//    // 计算斜率和截距，使用浮点数运算
//    *xielv = (float)(y_end - y_begin) / (end - begin);
//    *jieju = y_begin - (*xielv) * begin;
//}


//void crossing_add(uint8 num_d_l, uint8 num_u_l, uint8 num_d_r, uint8 num_u_r)
//{
//    float xielv_l, jieju_l;
//    float xielv_r, jieju_r;
//    uint16 i;
//	int temp;
//    

//    // 处理左边界补线
//    if (num_d_l&& num_u_l&&num_d_r&&num_u_r)
//    {

//        if (abs(num_d_l-num_u_l) >= 15 && abs(num_d_r - num_u_r) >= 15)// num_l >= 2
//        {
//            // 使用最后两个断点进行线性插值
//            xieji(num_u_l, num_d_l, &xielv_l, &jieju_l, 
//                  road_left[num_u_l], road_left[num_d_l]);
//            
//            for (i = num_u_l; i < num_d_l; i++)
//            {
//                road_left[i] = xielv_l * i + jieju_l;
//                if (road_left[i] <= 1)
//                    road_left[i] = 1;
//                else if (road_left[i] >= MT9V03X_W - 2)
//                    road_left[i] = MT9V03X_W - 2;
//            }
//			xieji(num_u_r, num_d_r, &xielv_r, &jieju_r, 
//                  road_right[num_u_r], road_right[num_d_r]);
//            
//            for (i = num_u_r; i < num_d_r; i++)
//            {
//                road_right[i] = xielv_r * i + jieju_r;
//                if (road_right[i] <= 1)
//                    road_right[i] = 1;
//                else if (road_right[i] >= MT9V03X_W - 2)
//                    road_right[i] = MT9V03X_W - 2;
//            }
//        }
//    

//    }
//	if (num_d_l == 0 && num_d_r == 0 && num_u_l >= 0 && num_u_r >= 0)
//	{
//		if (abs(num_u_l - num_u_r) < 12)
//		{
//			least_squares(num_u_l-15,num_u_l-5,road_left,&xielv_l,&jieju_l);
//            // 补线并限制边界
//            for (i = num_u_l - 5; i < MT9V03X_H - 1; i++)
//            {
//                road_left[i] = xielv_l * i + jieju_l;
//                if (road_left[i] <= 1)
//                    road_left[i] = 1;
//                else if (road_left[i] >= MT9V03X_W - 2)
//                    road_left[i] = 1;
//            }
//			least_squares(num_u_r-15,num_u_r-5,road_right,&xielv_r,&jieju_r);
//            for (i = num_u_r - 5; i < MT9V03X_H - 1; i++)
//            {
//                road_right[i] = xielv_r * i + jieju_r;
//                if (road_right[i] <= 1)
//                    road_right[i] = MT9V03X_W - 2;
//                else if (road_right[i] >= MT9V03X_W - 2)
//                    road_right[i] = MT9V03X_W - 2;
//            }
//		}
//	}
//}

//uint8 my_limit_min(uint8 num1, uint8 num2)
//{
//	if (num1 > num2)
//	{
//		return num2;
//	}
//	else
//	{
//		return num1;
//	}
//}

//void straight_line_handle(void)
//{
//	int proof;
//	int sum = 0;
//	uint8 limit = 0;//
//	if (line < 10 && Left_Lost_Time <= 20 && Right_Lost_Time <= 20)
//	{
//		limit = my_limit_min(line,10);
//		for (uint8 i = limit; i < 100; i++)
//		{
//			sum += ((road_left[i] + road_right[i]) >> 1);
//		}
//		proof = (int)(sum / (100 - limit));
//		if (abs(proof - 94) <= 10)
//		{
//			straight_flag = 1;
//		}
//		else
//		{
//			straight_flag = 0;
//		}
//			
//	}
//	else
//	{
//		straight_flag = 0;
//	}
//}


///*********************************************************
//** 备注：权重可通过图像权重数组调节
//*********************************************************/
//uint8 find_mid_line_weight(void)
//{
//    uint8 mid_line_value = MID_W;       // 最终中线输出值
//    uint8 mid_line = MID_W;             // 本次中线值
//    uint32 weight_midline_sum = 0;      // 加权中线累加值
//    uint32 weight_sum = 0;              // 权重累加值

//    for(uint8 i = MT9V03X_H - 1; i > line; i--)
//    {
//        weight_midline_sum += center_line[i] * mid_weight_list[i];
//        weight_sum += mid_weight_list[i];
//    }

//    mid_line = (uint8)(weight_midline_sum / weight_sum);
//    mid_line_value = last_mid_line * 0.2 + mid_line * 0.8; // 互补滤波
//    last_mid_line = mid_line_value;
//    return mid_line_value;
//}

