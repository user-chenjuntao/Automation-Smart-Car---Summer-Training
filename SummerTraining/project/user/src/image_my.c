#include "image_my.h"

uint8 image_otsuThreshold_less;
uint8 PostProcessing_image[MT9V03X_H][MT9V03X_W];                                      //用于存放二值化处理后的图像数组
//uint8 left_point[2] = { 0 };//左边起点的x，y值
//uint8 right_point_r[2] = { 0 };//右边起点的x，y值


//uint8 original_image[MT9V03X_H][MT9V03X_W];
//////------------------------------------------------------------------------------------------------------------------
//////  @brief      获得一副灰度图像
//////  @since      v1.0 
//////------------------------------------------------------------------------------------------------------------------
//void Get_image(uint8(*mt9v03x_image)[MT9V03X_W])
//{
//	#define use_num		1	//1就是不压缩，2就是压缩一倍	
//	uint8 i = 0, j = 0, row = 0, line = 0;
//    for (i = 0; i < MT9V03X_H; i += use_num)          //
//    {
//        for (j = 0; j <MT9V03X_W; j += use_num)     //
//        {
//            original_image[row][line] = mt9v03x_image[i][j];//这里的参数填写你的摄像头采集到的图像
//			line++;
//        }
//        line = 0;
//        row++;
//    }
//}

//-------------------------------------------------------------------------------------
//使用大津法进行二值化处理
//-------------------------------------------------------------------------------------
uint8 otsuThreshold_less(uint8 *image, uint16 width, uint16 height)
{
    #define GrayScale 256
    int pixel_count[GrayScale] = {0};//每个灰度值所占像素个数
    float pixel_account[GrayScale] = {0};//每个灰度值所占总像素比例
    int i,j;   
    int pixel_sum = width * height;   //总像素点
    uint8 threshold = 0;
    uint8* pixel_data = image;  //指向像素数据的指针


    //统计灰度级中每个像素在整幅图像中的个数  
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixel_count[(int)pixel_data[i * width + j]]++;  //将像素值作为计数数组的下标

        }
    }
    float u = 0;  
    for (i = 0; i < GrayScale; i++)
    {
        pixel_account[i] = (float)pixel_count[i] / pixel_sum;   //计算每个像素在整幅图像中的比例  
        u += i * pixel_account[i];  //总平均灰度
    }

      
    float variance_max=0.0;  //最大类间方差
	float variance;
    float w0 = 0, avgValue  = 0;  //w0 前景比例 ，avgValue 前景平均灰度
    for(int i = 0; i < 256; i++)     //每一次循环都是一次完整类间方差计算 (两个for叠加为1个)
    {  
        w0 += pixel_account[i];  //假设当前灰度i为阈值, 0~i 灰度像素所占整幅图像的比例即前景比例
        avgValue  += i * pixel_account[i];        
        if (w0 <= 0 || w0 >= 1)
			continue;
        variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //类间方差   
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

//目的是为了出去一些噪点，防止对于元素判断出现问题
void image_filter(uint8(*image)[MT9V03X_W])//形态学滤波，简单来说就是膨胀和腐蚀的思想
{
	uint16 i, j;
	uint32 num = 0;


	for (i = 1; i < MT9V03X_H - 1; i++)
	{
		for (j = 1; j < (MT9V03X_W - 1); j++)
		{
			//统计八个方向的像素值
			num =
				image[i - 1][j - 1] + image[i - 1][j] + image[i - 1][j + 1]
				+ image[i][j - 1] + image[i][j + 1]
				+ image[i + 1][j - 1] + image[i + 1][j] + image[i + 1][j + 1];


			if (num >= THRESHOLD_MAX && image[i][j] == 0)
			{

				image[i][j] = WHITE_PIXEL;//白  可以搞成宏定义，方便更改

			}
			if (num <= THRESHOLD_MIN && image[i][j] == 255)
			{

				image[i][j] = BLACK_PIXEL;//黑

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
uint8 longest_line_number;           //记录最长白列的列位置，x方向上
uint8 line;                          //记录最长白列的长度，0代表最长，y方向，行
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
    uint8 longest_length = 0;  // 记录最长白色列的长度
    
    left_point = get_left_start();
    right_point = get_right_start();
    
    longest_line_number = MT9V03X_W/2;  // 默认值
    line = 119;  // 默认值（119是一个任意选择，代表较长的长度）
    
    // 确保左右边界合理
    if (left_point >= right_point) return;
    
    for (i = left_point; i < right_point; i++)
    {
        uint8 current_length = 0;  // 当前列的白色长度
        uint8 start_row = 0;       // 当前列白色区域的起始行
        
        // 查找当前列白色区域的起始行
        for (j = MT9V03X_H - 1; j > 0; j--)
        {
            if (PostProcessing_image[j][i] == WHITE_PIXEL && 
                PostProcessing_image[j - 1][i] == BLACK_PIXEL)
            {
                start_row = j;
                current_length = MT9V03X_H - start_row;  // 计算白色列的长度
                break;
            }
        }
        
        // 如果没有找到黑白边界，但整列都是白色
        if (j == 0 && PostProcessing_image[0][i] == WHITE_PIXEL)
        {
            start_row = 0;
            current_length = MT9V03X_H;  // 整列都是白色
        }
        
        // 更新最长白色列信息
        if (current_length > longest_length)
        {
            longest_length = current_length;
            longest_line_number = i;
            line = start_row;  // 记录白色区域的起始行
        }
    }
}
uint8 road_left[120] = {0};         //记录x，或列数
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
		for (j = longest_line_number; j > 1; j--)       //列
		{
//			left_num++;
			if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j-1] == BLACK_PIXEL) || j == 2)
			{
				road_left[i] = j;
				if (left_flag_break)
				{
					if (road_left[i] - road_left[i-1] >= 5 && break_num_left < 6)
					{
						break_point_l[break_num_left][0] = i;   //哪一行
						break_point_l[break_num_left][1] = road_left[i];   //哪一列
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

//uint8 road_left[120] = {255};         // 初始化为无效值255（超出188范围）
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
//        // 左边界搜索（保持原有逻辑不变）
//        for (j = longest_line_number; j > 1; j--)
//        {
//            if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j-1] == BLACK_PIXEL && PostProcessing_image[i][j-2] == BLACK_PIXEL) || j == 2)
//            {
//                road_left[i] = j;
//                break;
//            }
//        }

//        // 右边界搜索（保持原有逻辑不变）
//        for (j = longest_line_number; j < MT9V03X_W - 2; j++)
//        {
//            if ((PostProcessing_image[i][j] == WHITE_PIXEL && PostProcessing_image[i][j+1] == BLACK_PIXEL && PostProcessing_image[i][j+2] == BLACK_PIXEL) || j == 185)
//            {
//                road_right[i] = j;
//                break;
//            }
//        }

//        // -------------------------- 左边界拐点检测（十字特征）--------------------------
//        if (left_flag_break && i > line)  // 确保有上一行数据
//        {
//            int8 diff = road_left[i] - road_left[i-1];
//            
//            // 检测向右的突变（可能是十字的左侧边缘）
//            if (diff >= 5 && break_num_left < 6)
//            {
//                break_point_l[break_num_left][0] = i;      // 记录当前行（十字左侧边缘）
//                break_point_l[break_num_left][1] = road_left[i];
//                break_num_left++;
//            }
//            
//            // 检测向左的突变（可能是普通弯道）
//            if (diff <= -5 && break_num_left < 6)
//            {
//                break_point_l[break_num_left][0] = i-1;    // 记录上一行（普通弯道）
//                break_point_l[break_num_left][1] = road_left[i-1];
//                break_num_left++;
//            }
//        }
//        left_flag_break = 1;

//        // -------------------------- 右边界拐点检测（十字特征）--------------------------
//        if (right_flag_break && i > line)  // 确保有上一行数据
//        {
//            int8 diff = road_right[i] - road_right[i-1];
//            
//            // 检测向左的突变（可能是十字的右侧边缘）
//            if (diff <= -5 && break_num_right < 6)
//            {
//                break_point_r[break_num_right][0] = i;      // 记录当前行（十字右侧边缘）
//                break_point_r[break_num_right][1] = road_right[i];
//                break_num_right++;
//            }
//            
//            // 检测向右的突变（可能是普通弯道）
//            if (diff >= 5 && break_num_right < 6)
//            {
//                break_point_r[break_num_right][0] = i-1;    // 记录上一行（普通弯道）
//                break_point_r[break_num_right][1] = road_right[i-1];
//                break_num_right++;
//            }
//        }
//        right_flag_break = 1;
//    }
//    
//    // 后处理：合并邻近拐点（如果需要）
//    merge_close_breakpoints();
//}



// 合并左拐点的邻近拐点
void merge_left_breakpoints(void)
{
    if (break_num_left <= 1) return;  // 不足2个拐点，无需合并

    uint8 temp_l[6][2] = {{0}};  // 临时存储合并后的左拐点
    uint8 temp_num = 0;          // 合并后的左拐点数量

    // 保留第一个拐点作为初始值
    temp_l[temp_num][0] = break_point_l[0][0];
    temp_l[temp_num][1] = break_point_l[0][1];
    temp_num++;

    // 遍历剩余拐点，判断是否需要合并
    for (uint8 i = 1; i < break_num_left; i++)
    {
        // 计算当前拐点与上一个保留的拐点的行数差
        uint8 row_diff = break_point_l[i][0] - temp_l[temp_num-1][0];

        if (row_diff > BREAKPOINT_MERGE_THRESHOLD)
        {
            // 行数差超过阈值，保留当前拐点
            temp_l[temp_num][0] = break_point_l[i][0];
            temp_l[temp_num][1] = break_point_l[i][1];
            temp_num++;
            if (temp_num >= 6) break;  // 不超过数组最大容量
        }
        // 否则（行数差≤阈值），视为邻近拐点，不保留（合并到上一个）
    }

    // 更新左拐点数组和数量
    memcpy(break_point_l, temp_l, sizeof(temp_l));
    break_num_left = temp_num;
}

// 合并右拐点的邻近拐点（逻辑与左拐点完全一致）
void merge_right_breakpoints(void)
{
    if (break_num_right <= 1) return;  // 不足2个拐点，无需合并

    uint8 temp_r[6][2] = {{0}};  // 临时存储合并后的右拐点
    uint8 temp_num = 0;          // 合并后的右拐点数量

    // 保留第一个拐点作为初始值
    temp_r[temp_num][0] = break_point_r[0][0];
    temp_r[temp_num][1] = break_point_r[0][1];
    temp_num++;

    // 遍历剩余拐点，判断是否需要合并
    for (uint8 i = 1; i < break_num_right; i++)
    {
        // 计算当前拐点与上一个保留的拐点的行数差
        uint8 row_diff = break_point_r[i][0] - temp_r[temp_num-1][0];

        if (row_diff > BREAKPOINT_MERGE_THRESHOLD)
        {
            // 行数差超过阈值，保留当前拐点
            temp_r[temp_num][0] = break_point_r[i][0];
            temp_r[temp_num][1] = break_point_r[i][1];
            temp_num++;
            if (temp_num >= 6) break;  // 不超过数组最大容量
        }
        // 否则（行数差≤阈值），视为邻近拐点，不保留（合并到上一个）
    }

    // 更新右拐点数组和数量
    memcpy(break_point_r, temp_r, sizeof(temp_r));
    break_num_right = temp_num;
}

// 统一调用左、右拐点合并函数
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
		center_line[i] = (road_left[i] + road_right[i]) >> 1;//求中线
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



//uint8 center_line[MT9V03X_H];  // 适配高度宏定义
//uint16 center_value = MT9V03X_W / 2;  // 初始值设为图像中心（188/2=94，与原默认值一致）

//void image_process(void)
//{
//    uint16 sum = 0;
//    uint8 valid_count = 0;  // 记录有效中心线的行数

//    // 1. 图像处理核心流程（顺序不变，依赖宏定义）
//    image_postprocess();          // 二值化
//	image_filter(PostProcessing_image);
//    research_longest_line();      // 找最长白列
//    research_road();              // 找道路边界和拐点
////    merge_close_breakpoints();    // 合并邻近拐点
//    crossing_add(break_num_left, break_num_right);  // 补线

//    // 2. 显示二值化图像（使用MT9V03X_W/H宏定义）
//    ips200_show_gray_image(0, 50, PostProcessing_image[0], 
//                          MT9V03X_W, MT9V03X_H, 
//                          MT9V03X_W, MT9V03X_H, 0);

//    // 绘制最长白列（黄色线，循环范围为图像高度）
//    for (uint8 i = 0; i < MT9V03X_H; i++)
//    {
//        // 确保列数在有效范围内（0 ~ MT9V03X_W-1）
//        uint8 draw_col = longest_line_number;
//        if (draw_col >= MT9V03X_W) draw_col = MT9V03X_W - 1;
//        ips200_draw_point(draw_col, i + 50, RGB565_YELLOW);
//    }

//    // 3. 计算中心线并绘制
//    for (uint8 i = line; i < MT9V03X_H; i++)
//    {
//        // 左/右边界有效性检查（255为无效标记）
//        bool left_valid = (road_left[i] != 255) && (road_left[i] < MT9V03X_W);
//        bool right_valid = (road_right[i] != 255) && (road_right[i] < MT9V03X_W);

//        if (left_valid && right_valid)
//        {
//            // 有效边界：计算中心线（平均）
//            center_line[i] = (road_left[i] + road_right[i]) >> 1;
//        }
//        else if (left_valid)
//        {
//            // 仅左边界有效：中心线 = 左边界 + 预设半宽（根据实际道路调整）
//            center_line[i] = road_left[i] + 40;  // 40为示例，需匹配实际道路宽度
//        }
//        else if (right_valid)
//        {
//            // 仅右边界有效：中心线 = 右边界 - 预设半宽
//            center_line[i] = road_right[i] - 40;
//        }
//        else
//        {
//            // 均无效：沿用前一行的中心线（避免突变）
//            center_line[i] = (i > line) ? center_line[i-1] : center_value;
//        }

//        // 限制中心线在图像范围内（0 ~ MT9V03X_W-1）
//        if (center_line[i] >= MT9V03X_W) center_line[i] = MT9V03X_W - 1;
//        if (center_line[i] < 0) center_line[i] = 0;

//        // 绘制边界和中心线（检查绘制范围）
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
//        ips200_draw_point(center_line[i], i + 50, RGB565_RED);  // 中心线已确保在范围内
//    }

//    // 4. 计算中心值（动态平均，取底部16行）
//    sum = 0;
//    valid_count = 0;
//    uint8 start_row = MT9V03X_H - 16;  // 从底部第16行开始（120-16=104）
//    for (uint8 i = start_row; i < MT9V03X_H; i++)
//    {
//        // 仅累加有效中心线（0 ~ MT9V03X_W-1）
//        if (center_line[i] >= 0 && center_line[i] < MT9V03X_W)
//        {
//            sum += center_line[i];
//            valid_count++;
//        }
//    }
//    // 有效行数≥8时更新中心值（确保稳定性）
//    if (valid_count >= 8)
//    {
//        center_value = sum / valid_count;
//    }
//    // 有效行数不足时沿用上次值

//    // 5. 清空数据（为下一次处理准备）
//    break_num_left = 0;
//    break_num_right = 0;
//    memset(break_point_l, 0, sizeof(break_point_l));
//    memset(break_point_r, 0, sizeof(break_point_r));
//    memset(road_left, 255, sizeof(road_left));  // 重置为无效值（255）
//    memset(road_right, 255, sizeof(road_right));
//    memset(center_line, 0, sizeof(center_line));
//}



//------------------------------------------------------------------------------------------------------------------
//函数名称：uint8 crossing_handle(void)
//功能说明：最小二乘法函数
//参数说明：无
//函数返回：
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
	if ((end - begin)*sum_x2 - sum_x * sum_x) //判断除数是否为零
	{
		*xielv = ((end - begin)*sum_xy - sum_x * sum_y) / ((end - begin)*sum_x2 - sum_x * sum_x);

	}
	*jieju = (sum_y - (*xielv) * sum_x) / (end - begin);
}
void xieji(uint8 begin, uint8 end, float *xielv, float *jieju, uint8 y_begin, uint8 y_end)
{
    if (end == begin)  // 避免除零错误
    {
        *xielv = 0;
        *jieju = y_begin;
        return;
    }
    
    // 计算斜率和截距，使用浮点数运算
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
//		//认为左边线丢失
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
    
    // 处理左边界补线
    if (num_l >= 1)
    {
        if (num_l == 1)
        {
            // 确保计算范围有效
            uint16 start = break_point_l[0][0] - 15;
            uint16 end = break_point_l[0][0] - 5;
            
            if (start < 0) start = 0;
            if (end <= start) return;
            
            least_squares(start, end, road_left, &xielv_l, &jieju_l);
            
            // 补线并限制边界
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
            // 使用最后两个断点进行线性插值
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
    
    // 处理右边界补线（逻辑与左边界类似）
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