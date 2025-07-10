//#include "image_handle.h"

////-------------------------------------------------------------------------------------
////运用了索贝尔边缘检测算法进行二值化处理，但是卷积从3*3简化成3*1，同时只算x，y两个方向
////上的梯度，简化了算法，提高效率
////-------------------------------------------------------------------------------------
//void sobelAutoThreshold(const uint8 image_mt[MT9V03X_H/2][MT9V03X_W],uint8 target[MT9V03X_H/2][MT9V03X_W])
//{
//    const int x_start = 1;
//    const int x_end = MT9V03X_W - 1;
//    const int y_start = 1;
//    const int y_end = (MT9V03X_H/2) - 1;
//    
//    for (int y = y_start; y < y_end; y++) {
//        for (int x = x_start; x < x_end; x++) {
//            // 使用1x3卷积核计算水平梯度
//            int16 gx = -image_mt[y][x-1] + image_mt[y][x+1];
//            
//            // 使用1x3卷积核计算垂直梯度
//            int16 gy = -image_mt[y-1][x] + image_mt[y+1][x];
//            
//            // 计算梯度幅值（简化版，避免开平方）
//            int16 gradient = abs(gx) + abs(gy);
//            
//            // 计算3x3邻域和
//            uint16_t neighborhood_sum = 
//                image_mt[y-1][x-1] + image_mt[y-1][x] + image_mt[y-1][x+1] +
//                image_mt[y][x-1]   + image_mt[y][x]   + image_mt[y][x+1] +
//                image_mt[y+1][x-1] + image_mt[y+1][x] + image_mt[y+1][x+1];
//            
//            // 自适应阈值比较
//            if (gradient > (neighborhood_sum / 12)) {
//                target[y][x] = 0xFF;
//            } else {
//                target[y][x] = 0x00;
//            }
//        }
//    }
//    
//    // 处理边缘区域
//    memset(target, 0, MT9V03X_W * y_start);
//    memset(target + (MT9V03X_H/2 - 1), 0, MT9V03X_W);
//    
//    for (int y = y_start; y < y_end; y++) {
//        target[y][0] = 0;
//        target[y][MT9V03X_W-1] = 0;
//    }
//}






//  
////---------------------------------------------------------------------------
////使用差比和的算法进行巡线,按照逐飞的思路
////---------------------------------------------------------------------------

//uint8 reference_point = 0,reference_col = 255;
//uint8 whitemax = 0;
//uint8 whitemin = 0;
//uint8 remote_distance[MT9V03X_W];
//uint8 reference_col_line[MT9V03X_H];
//static uint8 reference_contrast = 20;
//uint8 left_edge_line[MT9V03X_H];
//uint8 right_edge_line[MT9V03X_H];


////int32 func_limit (int32 num1, int32 num2, int32 limit)
////{
////	if (num1 <= num2)
////	{
////		return 0;
////	}
////	else if (num1 > num2 && num1 <= limit)
////	{
////		return num1;
////	}
////	else
////	{
////		return  limit;
////	}
////	
////}

////---------------------------------------------------------------------------
////寻找参考白点的灰度值，这里用后四行的平均灰度值最为参考白点的灰度值
////---------------------------------------------------------------------------
//void get_reference_point (const uint8 *image)
//{
//	uint8 *p = (uint8 *)&image[(MT9V03X_H - REFERENCEHEIGHT) * MT9V03X_W];    //在二维数组上找到求后四行灰度值的起点
//	uint16 temp = 0;                                                          //
//	uint32 temp1 = 0;
//	temp = REFERENCEHEIGHT * MT9V03X_W;
//	for (uint8 i = 0; i < temp; i++)
//	{
//		temp1 += *(image + i);
//	}
//	reference_point = temp1 / temp;
//	whitemax = reference_point * WHITEMAX /10;
//	whitemin = reference_point * WHITEMIN /10;
//	if (whitemax >= 255)
//	{
//		whitemax = 255;
//	}
//	
//}

//void search_reference_col (const uint8 *image)
//{
//	uint8 col,row;
//	uint16 temp1 = 0,temp2 = 0,temp3 = 0;
//	
//	for (col = 0; col < MT9V03X_W; col++)
//	{
//		remote_distance[col] = MT9V03X_H - 1;
//	}
//	for (col = 0; col < MT9V03X_W; col += CONTRASTOFFSET)                  //这里的for循环经历了几万次，我猜是很费时间的
//	{
//		for (row = MT9V03X_H - 1; row > STOPROW; row -= CONTRASTOFFSET)
//		{
//			temp1 = *(image + row * MT9V03X_W + col);
//			
//			temp2 = *(image + (row - CONTRASTOFFSET) * MT9V03X_W + col);
//			
//			
//			if (temp2 > whitemax)
//			{
//				continue;
//			}
//			if (temp1 < whitemin)
//			{
//				remote_distance[col] = row;
//				break;
//			}
//			
//			temp3 = (temp1 - temp2) * 128 /(temp1 + temp2);
//			
//			if (temp3 >= reference_contrast || row == STOPROW)
//			{
//				remote_distance[col] = row;
//				break;
//			}
//			
//		}
//	}
//	reference_col = remote_distance[0];
//	for (uint8 i = 0; i < MT9V03X_W; i++)
//	{
//		if (reference_col < remote_distance[i])
//		{
//			reference_col = remote_distance[i];
//		}
//	}
//	
//	//
//	for (uint8 i = 0; i < MT9V03X_H; i++)
//	{
//		reference_col_line[i] = reference_col;
//	}
//}


//void search_line(const uint8 *image)
//{
//    uint8 *p = (uint8 *)image[0];                          // 图像数组指针
//    uint8 row_max = MT9V03X_H - 1;                    // 行最大值
//    uint8 row_min = STOPROW;                               // 行最小值
//    uint8 col_max = MT9V03X_W - CONTRASTOFFSET;       // 列最大值 
//    uint8 col_min = CONTRASTOFFSET;                        // 列最小值
//    int16 leftstartcol  = reference_col;                   // 搜线左起始列
//    int16 rightstartcol = reference_col;                   // 搜线右起始列
//    int16 leftendcol    = 0;                               // 搜线左终止列
//    int16 rightendcol   = MT9V03X_W - 1;              // 搜线右终止列
//    uint8 search_time   = 0;                               // 单点搜线次数
//    uint8 temp1 = 0, temp2 = 0;                            // 临时变量 用于存储图像数据
//    int   temp3 = 0;                                        // 临时变量 用于存储对比度
//    int   leftstop = 0, rightstop = 0, stoppoint = 0;      // 搜线自锁变量

//    int col, row;

//    for(row = row_max; row >= row_min; row --){
//        left_edge_line[row]  = col_min - CONTRASTOFFSET;
//        right_edge_line[row] = col_max + CONTRASTOFFSET;
//    }

//    for(row = row_max; row >= row_min; row --)
//	{
//        p = (uint8 *)&image[row * MT9V03X_W];         // 获取本行起点位置指针
//        if (!leftstop) 
//		{
//			search_time = 2;
//			do {
//				if (search_time == 1) {
//					leftstartcol = reference_col;
//					leftendcol = col_min;
//				}
//				search_time--;
//				for (col = leftstartcol; col >= leftendcol; col--) {
//					temp1 = *(p + col);  // 获取当前点灰度值
//					temp2 = *(p + col - CONTRASTOFFSET);  // 获取对比点灰度值

//					
//					if (temp1 < whitemax && col == leftstartcol && leftstartcol == reference_col) 
//					{ 
//						leftstop = 1;  // 搜线索引自锁，不在进行左边线搜索
//						for (stoppoint = row; stoppoint >= 0; stoppoint--) {
//							left_edge_line[stoppoint] = col_min;
//						}
//						search_time = 0;
//						break;
//					}
//					if (temp1 < whitemin)  // 判断当前点是否为黑点 若为黑点则不进行对比
//					{
//						left_edge_line[row] = (uint8)col;
//						break;
//					}

//					if (temp2 > whitemax)  // 判断对比点是否为白点 若为白点则直接跳过
//					{
//						continue;
//					}

//					temp3 = (temp1 - temp2) * 200 / (temp1 + temp2);  // 计算对比度

//					if (temp3 > reference_contrast || col == col_min)  // 判断对比度是否大于阈值
//					{
//						left_edge_line[row] = col - CONTRASTOFFSET;  // 保存当前行左边界
//						leftstartcol = (uint8)col + SEARCHRANGE;
//						if (leftstartcol > col_max)
//							leftstartcol = col_max;
//						leftendcol = (uint8)col - SEARCHRANGE;
//						search_time = 0;
//						break;
//					}
//				}
//			} while (search_time);
//		}	
//    }
//}


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