//#include "image_test.h"

////-------------------------------------------------------------------------------------------------------------------
////  简介:八邻域图像处理

////------------------------------------------------------------------------------------------------------------------


//uint8 while_flag = 0;


///*变量声明*/
//uint8 original_image[image_h][image_w];
//uint8 image_thereshold = 80;//图像分割阈值
//////------------------------------------------------------------------------------------------------------------------
//////  @brief      获得一副灰度图像
//////  @since      v1.0 
//////------------------------------------------------------------------------------------------------------------------
//void Get_image(uint8(*mt9v03x_image)[image_w])
//{
//	#define use_num		1	//1就是不压缩，2就是压缩一倍	
//	uint8 i = 0, j = 0, row = 0, line = 0;
//    for (i = 0; i < image_h; i += use_num)          //
//    {
//        for (j = 0; j <image_w; j += use_num)     //
//        {
//            original_image[row][line] = mt9v03x_image[i][j];//这里的参数填写你的摄像头采集到的图像
//			line++;
//        }
//        line = 0;
//        row++;
//    }
//}
////------------------------------------------------------------------------------------------------------------------
////  @brief     动态阈值
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
//    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
//    uint8 MinValue=0, MaxValue=0;
//    uint8 Threshold = 0;
//	
//	
//    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
//    {
//        //Y=Image_Height;
//        for (X = 0; X < Image_Width; X++)
//        {
//        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
//        }
//    }




//    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
//    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

//    if (MaxValue == MinValue)
//    {
//        return MaxValue;          // 图像中只有一个颜色
//    }
//    if (MinValue + 1 == MaxValue)
//    {
//        return MinValue;      // 图像中只有二个颜色
//    }

//    for (Y = MinValue; Y <= MaxValue; Y++)
//    {
//        Amount += HistGram[Y];        //  像素总数
//    }

//    PixelIntegral = 0;
//    for (Y = MinValue; Y <= MaxValue; Y++)
//    {
//        PixelIntegral += HistGram[Y] * Y;//灰度值总数
//    }
//    SigmaB = -1;
//    for (Y = MinValue; Y < MaxValue; Y++)
//    {
//          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
//          PixelFore = Amount - PixelBack;         //背景像素点数
//          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
//          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
//          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
//          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
//          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
//          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
//          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
//          if (Sigma > SigmaB)//遍历最大的类间方差g
//          {
//              SigmaB = Sigma;
//              Threshold = (uint8)Y;
//          }
//    }
//   return Threshold;
//}

//////-------------------------------------------------------------------------------------
//////使用大津法进行二值化处理
//////-------------------------------------------------------------------------------------
////uint8 otsuThreshold(uint8 *image, uint16 width, uint16 height)
////{
////    #define GrayScale 256
////    int pixel_count[GrayScale] = {0};//每个灰度值所占像素个数
////    float pixel_account[GrayScale] = {0};//每个灰度值所占总像素比例
////    int i,j;   
////    int pixel_sum = width * height;   //总像素点
////    uint8 threshold = 0;
////    uint8* pixel_data = image;  //指向像素数据的指针


////    //统计灰度级中每个像素在整幅图像中的个数  
////    for (i = 0; i < height; i++)
////    {
////        for (j = 0; j < width; j++)
////        {
////            pixel_count[(int)pixel_data[i * width + j]]++;  //将像素值作为计数数组的下标

////        }
////    }
////    float u = 0;  
////    for (i = 0; i < GrayScale; i++)
////    {
////        pixel_account[i] = (float)pixel_count[i] / pixel_sum;   //计算每个像素在整幅图像中的比例  
////        u += i * pixel_account[i];  //总平均灰度
////    }

////      
////    float variance_max=0.0;  //最大类间方差
////    float w0 = 0, avgValue  = 0;  //w0 前景比例 ，avgValue 前景平均灰度
////    for(int i = 0; i < 256; i++)     //每一次循环都是一次完整类间方差计算 (两个for叠加为1个)
////    {  
////        w0 += pixel_account[i];  //假设当前灰度i为阈值, 0~i 灰度像素所占整幅图像的比例即前景比例
////        avgValue  += i * pixel_account[i];        
////        
////        float variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //类间方差   
////        if(variance > variance_max) 
////        {  
////            variance_max = variance;  
////            threshold = i;  
////        }  
////    } 

////    return threshold;

////}
////------------------------------------------------------------------------------------------------------------------
////  @brief      图像二值化，这里用的是大津法二值化
////  @since      v1.0 
////------------------------------------------------------------------------------------------------------------------
//uint8 bin_image[image_h][image_w];//图像数组
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
////函数名称：void get_start_point(uint8 start_row)
////功能说明：寻找两个边界的边界点作为八邻域循环的起始点
////参数说明：输入任意行数
////------------------------------------------------------------------------------------------------------------------
//uint8 start_point_l[2] = { 0 };//左边起点的x，y值
//uint8 start_point_r[2] = { 0 };//右边起点的x，y值
//uint8 get_start_point(uint8 start_row)
//{
//	uint8 i = 0,l_found = 0,r_found = 0;
//	//清零
//	start_point_l[0] = 0;//x
//	start_point_l[1] = 0;//y

//	start_point_r[0] = 0;//x
//	start_point_r[1] = 0;//y

//		//从中间往左边，先找起点
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
//			//printf("找到右边起点image[%d][%d]\n",start_row, i);
//			r_found = 1;
//			break;
//		}
//	}

//	if(l_found&&r_found)return 1;
//	else {
//		//printf("未找到起点\n");
//		return 0;
//	} 
//}

//#define USE_num	image_h*3	//定义找点的数组成员个数按理说300个点能放下，但是有些特殊情况确实难顶，多定义了一点

// //存放点的x，y坐标
//uint16 points_l[(uint16)USE_num][2] = { {  0 } };//左线
//uint16 points_r[(uint16)USE_num][2] = { {  0 } };//右线
//uint16 dir_r[(uint16)USE_num] = { 0 };//用来存储右边生长方向
//uint16 dir_l[(uint16)USE_num] = { 0 };//用来存储左边生长方向
//uint16 data_stastics_l = 0;//统计左边找到点的个数
//uint16 data_stastics_r = 0;//统计右边找到点的个数
//uint8 hightest = 0;//最高点
//void search_l_r(uint16 break_flag, uint8(*image)[image_w], uint16 *l_stastic, uint16 *r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint8*hightest)
//{

//	uint8 i = 0, j = 0;

//	//左边变量
//	uint8 search_filds_l[8][2] = { {  0 } };
//	uint8 index_l = 0;
//	uint8 temp_l[8][2] = { {  0 } };
//	uint8 center_point_l[2] = {  0 };
//	uint16 l_data_statics;//统计左边
//	//定义八个邻域
//	static int8 seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
//	//{-1,-1},{0,-1},{+1,-1},
//	//{-1, 0},	     {+1, 0},
//	//{-1,+1},{0,+1},{+1,+1},
//	//这个是顺时针

//	//右边变量
//	uint8 search_filds_r[8][2] = { {  0 } };
//	uint8 center_point_r[2] = { 0 };//中心坐标点
//	uint8 index_r = 0;//索引下标
//	uint8 temp_r[8][2] = { {  0 } };
//	uint16 r_data_statics;//统计右边
//	//定义八个邻域
//	static int8 seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
//	//{-1,-1},{0,-1},{+1,-1},
//	//{-1, 0},	     {+1, 0},
//	//{-1,+1},{0,+1},{+1,+1},
//	//这个是逆时针

//	l_data_statics = *l_stastic;//统计找到了多少个点，方便后续把点全部画出来
//	r_data_statics = *r_stastic;//统计找到了多少个点，方便后续把点全部画出来

//	//第一次更新坐标点  将找到的起点值传进来
//	center_point_l[0] = l_start_x;//x
//	center_point_l[1] = l_start_y;//y
//	center_point_r[0] = r_start_x;//x
//	center_point_r[1] = r_start_y;//y

//		//开启邻域循环
//	while (break_flag--)
//	{

//		//左边
//		for (i = 0; i < 8; i++)//传递8F坐标
//		{
//			search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
//			search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
//		}
//		//中心坐标点填充到已经找到的点内
//		points_l[l_data_statics][0] = center_point_l[0];//x
//		points_l[l_data_statics][1] = center_point_l[1];//y
//		l_data_statics++;//索引加一

//		//右边
//		for (i = 0; i < 8; i++)//传递8F坐标
//		{
//			search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
//			search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
//		}
//		//中心坐标点填充到已经找到的点内
//		points_r[r_data_statics][0] = center_point_r[0];//x
//		points_r[r_data_statics][1] = center_point_r[1];//y

//		index_l = 0;//先清零，后使用
//		for (i = 0; i < 8; i++)
//		{
//			temp_l[i][0] = 0;//先清零，后使用
//			temp_l[i][1] = 0;//先清零，后使用
//		}

//		//左边判断
//		for (i = 0; i < 8; i++)
//		{
//			if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
//				&& image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
//			{
//				temp_l[index_l][0] = search_filds_l[(i)][0];
//				temp_l[index_l][1] = search_filds_l[(i)][1];
//				index_l++;
//				dir_l[l_data_statics - 1] = (i);//记录生长方向
//			}

//			if (index_l)
//			{
//				//更新坐标点
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
//			//printf("三次进入同一个点，退出\n");
//			break;
//		}
//		if (abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
//			&& abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2)
//			)
//		{
//			//printf("\n左右相遇退出\n");	
//			*hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//取出最高点
//			//printf("\n在y=%d处退出\n",*hightest);
//			break;
//		}
//		if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
//		{
//			printf("\n if the left is higher than the right, the left wait the right\n");	
//			continue;//如果左边比右边高了，左边等待右边
//		}
//		if (dir_l[l_data_statics - 1] == 7
//			&& (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//左边比右边高且已经向下生长了
//		{
//			//printf("\n左边开始向下了，等待右边，等待中... \n");
//			center_point_l[0] = points_l[l_data_statics - 1][0];//x
//			center_point_l[1] = points_l[l_data_statics - 1][1];//y
//			l_data_statics--;
//		}
//		r_data_statics++;//索引加一

//		index_r = 0;//先清零，后使用
//		for (i = 0; i < 8; i++)
//		{
//			temp_r[i][0] = 0;//先清零，后使用
//			temp_r[i][1] = 0;//先清零，后使用
//		}

//		//右边判断
//		for (i = 0; i < 8; i++)
//		{
//			if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
//				&& image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
//			{
//				temp_r[index_r][0] = search_filds_r[(i)][0];
//				temp_r[index_r][1] = search_filds_r[(i)][1];
//				index_r++;//索引加一
//				dir_r[r_data_statics - 1] = (i);//记录生长方向
//				//printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
//			}
//			if (index_r)
//			{

//				//更新坐标点
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


//	//取出循环次数
//	*l_stastic = l_data_statics;
//	*r_stastic = r_data_statics;

//}
////------------------------------------------------------------
////函数名称：void get_left(uint16 total_L)
////功能说明：从八邻域边界里提取需要的边线
////参数说明：total_L	：找到的点的总数
////函数返回：Null
////------------------------------------------------------------

//uint8 l_border[image_h];//左线数组
//uint8 r_border[image_h];//右线数组
//uint8 center_line[image_h];//中线数组
//void get_left(uint16 total_L)
//{
//	uint8 i = 0;
//	uint16 j = 0;
//	uint8 h = 0;
//	//初始化
//	for (i = 0;i<image_h;i++)
//	{
//		l_border[i] = border_min;
//	}
//	h = image_h - 2;
//	//左边
//	for (j = 0; j < total_L; j++)
//	{
//		//printf("%d\n", j);
//		if (points_l[j][1] == h)
//		{
//			l_border[h] = points_l[j][0]+1;
//		}
//		else continue; //每行只取一个点，没到下一行就不记录
//		h--;
//		if (h == 0) 
//		{
//			break;//到最后一行退出
//		}
//	}
//}
////------------------------------------------------------------
////函数名称：void get_right(uint16 total_R)
////功能说明：从八邻域边界里提取需要的边线
////参数说明：total_R  ：找到的点的总数
////函数返回：Null
// //------------------------------------------------------------
//void get_right(uint16 total_R)
//{
//	uint8 i = 0;
//	uint16 j = 0;
//	uint8 h = 0;
//	for (i = 0; i < image_h; i++)
//	{
//		r_border[i] = border_max;//右边线初始化放到最右边，左边线放到最左边，这样八邻域闭合区域外的中线就会在中间，不会干扰得到的数据
//	}
//	h = image_h - 2;
//	//右边
//	for (j = 0; j < total_R; j++)
//	{
//		if (points_r[j][1] == h)
//		{
//			r_border[h] = points_r[j][0] - 1;
//		}
//		else continue;//每行只取一个点，没到下一行就不记录
//		h--;
//		if (h == 0)break;//到最后一行退出
//	}
//}

////定义膨胀和腐蚀的阈值区间
//#define threshold_max	255*5//此参数可根据自己的需求调节
//#define threshold_min	255*2//此参数可根据自己的需求调节
//void image_filter(uint8(*bin_image)[image_w])//形态学滤波，简单来说就是膨胀和腐蚀的思想
//{
//	uint16 i, j;
//	uint32 num = 0;


//	for (i = 1; i < image_h - 1; i++)
//	{
//		for (j = 1; j < (image_w - 1); j++)
//		{
//			//统计八个方向的像素值
//			num =
//				bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
//				+ bin_image[i][j - 1] + bin_image[i][j + 1]
//				+ bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];


//			if (num >= threshold_max && bin_image[i][j] == 0)
//			{

//				bin_image[i][j] = 255;//白  可以搞成宏定义，方便更改

//			}
//			if (num <= threshold_min && bin_image[i][j] == 255)
//			{

//				bin_image[i][j] = 0;//黑

//			}

//		}
//	}

//}

////------------------------------------------------------------------------------------------------------------------
////函数名称：void image_draw_rectan(uint8(*image)[image_w])
////功能说明：给图像画一个黑框
////参数说明：uint8(*image)[image_w]	图像首地址
////函数返回：无
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
////函数名称：void image_process(void)
////功能说明：最终处理函数
////参数说明：无
////函数返回：无
////------------------------------------------------------------------------------------------------------------------
//void image_process(void)
//{
//	uint16 i;
//	center_value = 0;
//	uint16 sum;
//	uint8 hightest = 0;//定义一个最高行
//	/*这是离线调试用的*/
//	Get_image(mt9v03x_image);
//	turn_to_bin();
//	/*提取赛道边界*/
//	image_filter(bin_image);//滤波
//	image_draw_rectan(bin_image);//预处理
//	//清零
//	data_stastics_l = 0;
//	data_stastics_r = 0;
//	sum = 0;
//	if (get_start_point(image_h - 2))//找到起点了，再执行八领域，没找到就一直找
//	{
//		printf("Eight Areas Start\n");
//		search_l_r((uint16)USE_num, bin_image, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &hightest);
//		printf("Eight Areas End\n");
//		// 从爬取的边界线内提取边线 ， 这个才是最终有用的边线
//		
//		get_left(data_stastics_l);
//		get_right(data_stastics_r);
//		crossing_add(crossing_handle_l(), crossing_handle_r());
////		cross_fill(bin_image,r_border,l_border,data_stastics_l,data_stastics_r,dir_l,dir_r,points_l,points_r);
//		
//	}


//	//显示图像
//	ips200_show_gray_image(0, 50, bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);

//		//根据最终循环次数画出边界点
//		for (i = 0; i < data_stastics_l; i++)
//		{
//			ips200_draw_point(points_l[i][0]+2, points_l[i][1] + 50, uesr_BLUE);//显示起点
//		}
//		for (i = 0; i < data_stastics_r; i++)
//		{
//			ips200_draw_point(points_r[i][0]-2, points_r[i][1] + 50, uesr_RED);//显示起点
//		}

//		for (i = hightest; i < image_h-1; i++)
//		{
//			center_line[i] = (l_border[i] + r_border[i]) >> 1;//求中线
//			//求中线最好最后求，不管是补线还是做状态机，全程最好使用一组边线，中线最后求出，不能干扰最后的输出
//			//当然也有多组边线的找法，但是个人感觉很繁琐，不建议
//			ips200_draw_point(center_line[i], i + 50, uesr_RED);//显示起点 显示中线	
//			ips200_draw_point(l_border[i], i + 50, uesr_GREEN);//显示起点 显示左边线
//			ips200_draw_point(r_border[i], i + 50, uesr_BLUE);//显示起点 显示右边线
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
////* @brief 最小二乘法
////* @param uint8 begin				输入起点
////* @param uint8 end					输入终点
////* @param uint8 *border				输入需要计算斜率的边界首地址
////*  @see CTest		Slope_Calculate(start, end, border);//斜率
////* @return 返回说明
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
////	if ((end - begin)*x2sum - xsum * xsum) //判断除数是否为零
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
////* @brief 计算斜率截距
////* @param uint8 start				输入起点
////* @param uint8 end					输入终点
////* @param uint8 *border				输入需要计算斜率的边界
////* @param float *slope_rate			输入斜率地址
////* @param float *intercept			输入截距地址
////*  @see CTest		calculate_s_i(start, end, r_border, &slope_l_rate, &intercept_l);
////* @return 返回说明
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

////	//计算各个平均数
////	if (num)
////	{
////		x_average = (float)(xsum / num);
////		y_average = (float)(ysum / num);

////	}

////	/*计算斜率*/
////	*slope_rate = Slope_Calculate(start, end, border);//斜率
////	*intercept = y_average - (*slope_rate)*x_average;//截距
////}

/////** 
////* @brief 十字补线函数
////* @param uint8(*image)[image_w]		输入二值图像
////* @param uint8 *l_border			输入左边界首地址
////* @param uint8 *r_border			输入右边界首地址
////* @param uint16 total_num_l			输入左边循环总次数
////* @param uint16 total_num_r			输入右边循环总次数
////* @param uint16 *dir_l				输入左边生长方向首地址
////* @param uint16 *dir_r				输入右边生长方向首地址
////* @param uint16(*points_l)[2]		输入左边轮廓首地址
////* @param uint16(*points_r)[2]		输入右边轮廓首地址
////*  @see CTest		cross_fill(image,l_border, r_border, data_statics_l, data_statics_r, dir_l, dir_r, points_l, points_r);
////* @return 返回说明
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
////	//出十字
////	for (i = 1; i < total_num_l; i++)
////	{
////		if (dir_l[i - 1] == 4 && dir_l[i] == 4 && dir_l[i + 3] == 6 && dir_l[i + 5] == 6 && dir_l[i + 7] == 6)
////		{
////			break_num_l = points_l[i][1];//传递y坐标
////			printf("brea_knum-L:%d\n", break_num_l);
////			printf("I:%d\n", i);
//////			printf("十字标志位：1\n");
////			break;
////		}
////	}
////	for (i = 1; i < total_num_r; i++)
////	{
////		if (dir_r[i - 1] == 4 && dir_r[i] == 4 && dir_r[i + 3] == 6 && dir_r[i + 5] == 6 && dir_r[i + 7] == 6)
////		{
////			break_num_r = points_r[i][1];//传递y坐标
////			printf("brea_knum-R:%d\n", break_num_r);
////			printf("I:%d\n", i);
//////			printf("十字标志位：1\n");
////			break;
////		}
////	}
////	if (break_num_l&&break_num_r&&image[image_h - 1][4] && image[image_h - 1][image_w - 4])//两边生长方向都符合条件
////	{
////		//计算斜率
////		start = break_num_l - 15;
////		start = limit_a_b(start, 0, image_h);
////		end = break_num_l - 5;
////		calculate_s_i(start, end, l_border, &slope_l_rate, &intercept_l);
////		//printf("slope_l_rate:%d\nintercept_l:%d\n", slope_l_rate, intercept_l);
////		for (i = break_num_l - 5; i < image_h - 1; i++)
////		{
////			l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
////			l_border[i] = limit_a_b(l_border[i], border_min, border_max);//限幅
////		}

////		//计算斜率
////		start = break_num_r - 15;//起点
////		start = limit_a_b(start, 0, image_h);//限幅
////		end = break_num_r - 5;//终点
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
////函数名称：uint8 crossing_handle(void)
////功能说明：十字处理函数
////参数说明：无
////函数返回：
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
////		//认为左边线丢失
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
////函数名称：int my_abs(int value)
////功能说明：求绝对值
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
////函数名称：int16 limit(int16 x, int16 y)
////功能说明：求x,y中的最小值
////函数返回：返回两值中的最小值
////------------------------------------------------------------------------------------------------------------------
//int16 limit1(int16 x, int16 y)
//{
//	if (x > y)             return y;
//	else if (x < -y)       return -y;
//	else                return x;
//}
