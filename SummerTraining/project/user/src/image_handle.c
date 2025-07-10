//#include "image_handle.h"

////-------------------------------------------------------------------------------------
////��������������Ե����㷨���ж�ֵ���������Ǿ����3*3�򻯳�3*1��ͬʱֻ��x��y��������
////�ϵ��ݶȣ������㷨�����Ч��
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
//            // ʹ��1x3����˼���ˮƽ�ݶ�
//            int16 gx = -image_mt[y][x-1] + image_mt[y][x+1];
//            
//            // ʹ��1x3����˼��㴹ֱ�ݶ�
//            int16 gy = -image_mt[y-1][x] + image_mt[y+1][x];
//            
//            // �����ݶȷ�ֵ���򻯰棬���⿪ƽ����
//            int16 gradient = abs(gx) + abs(gy);
//            
//            // ����3x3�����
//            uint16_t neighborhood_sum = 
//                image_mt[y-1][x-1] + image_mt[y-1][x] + image_mt[y-1][x+1] +
//                image_mt[y][x-1]   + image_mt[y][x]   + image_mt[y][x+1] +
//                image_mt[y+1][x-1] + image_mt[y+1][x] + image_mt[y+1][x+1];
//            
//            // ����Ӧ��ֵ�Ƚ�
//            if (gradient > (neighborhood_sum / 12)) {
//                target[y][x] = 0xFF;
//            } else {
//                target[y][x] = 0x00;
//            }
//        }
//    }
//    
//    // �����Ե����
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
////ʹ�ò�Ⱥ͵��㷨����Ѳ��,������ɵ�˼·
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
////Ѱ�Ҳο��׵�ĻҶ�ֵ�������ú����е�ƽ���Ҷ�ֵ��Ϊ�ο��׵�ĻҶ�ֵ
////---------------------------------------------------------------------------
//void get_reference_point (const uint8 *image)
//{
//	uint8 *p = (uint8 *)&image[(MT9V03X_H - REFERENCEHEIGHT) * MT9V03X_W];    //�ڶ�ά�������ҵ�������лҶ�ֵ�����
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
//	for (col = 0; col < MT9V03X_W; col += CONTRASTOFFSET)                  //�����forѭ�������˼���Σ��Ҳ��Ǻܷ�ʱ���
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
//    uint8 *p = (uint8 *)image[0];                          // ͼ������ָ��
//    uint8 row_max = MT9V03X_H - 1;                    // �����ֵ
//    uint8 row_min = STOPROW;                               // ����Сֵ
//    uint8 col_max = MT9V03X_W - CONTRASTOFFSET;       // �����ֵ 
//    uint8 col_min = CONTRASTOFFSET;                        // ����Сֵ
//    int16 leftstartcol  = reference_col;                   // ��������ʼ��
//    int16 rightstartcol = reference_col;                   // ��������ʼ��
//    int16 leftendcol    = 0;                               // ��������ֹ��
//    int16 rightendcol   = MT9V03X_W - 1;              // ��������ֹ��
//    uint8 search_time   = 0;                               // �������ߴ���
//    uint8 temp1 = 0, temp2 = 0;                            // ��ʱ���� ���ڴ洢ͼ������
//    int   temp3 = 0;                                        // ��ʱ���� ���ڴ洢�Աȶ�
//    int   leftstop = 0, rightstop = 0, stoppoint = 0;      // ������������

//    int col, row;

//    for(row = row_max; row >= row_min; row --){
//        left_edge_line[row]  = col_min - CONTRASTOFFSET;
//        right_edge_line[row] = col_max + CONTRASTOFFSET;
//    }

//    for(row = row_max; row >= row_min; row --)
//	{
//        p = (uint8 *)&image[row * MT9V03X_W];         // ��ȡ�������λ��ָ��
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
//					temp1 = *(p + col);  // ��ȡ��ǰ��Ҷ�ֵ
//					temp2 = *(p + col - CONTRASTOFFSET);  // ��ȡ�Աȵ�Ҷ�ֵ

//					
//					if (temp1 < whitemax && col == leftstartcol && leftstartcol == reference_col) 
//					{ 
//						leftstop = 1;  // �����������������ڽ������������
//						for (stoppoint = row; stoppoint >= 0; stoppoint--) {
//							left_edge_line[stoppoint] = col_min;
//						}
//						search_time = 0;
//						break;
//					}
//					if (temp1 < whitemin)  // �жϵ�ǰ���Ƿ�Ϊ�ڵ� ��Ϊ�ڵ��򲻽��жԱ�
//					{
//						left_edge_line[row] = (uint8)col;
//						break;
//					}

//					if (temp2 > whitemax)  // �ж϶Աȵ��Ƿ�Ϊ�׵� ��Ϊ�׵���ֱ������
//					{
//						continue;
//					}

//					temp3 = (temp1 - temp2) * 200 / (temp1 + temp2);  // ����Աȶ�

//					if (temp3 > reference_contrast || col == col_min)  // �ж϶Աȶ��Ƿ������ֵ
//					{
//						left_edge_line[row] = col - CONTRASTOFFSET;  // ���浱ǰ����߽�
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