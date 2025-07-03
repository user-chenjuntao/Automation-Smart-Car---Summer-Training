#include "image_handle.h"

//-------------------------------------------------------------------------------------
//运用了索贝尔边缘检测算法进行二值化处理，但是卷积从3*3简化成3*1，同时只算x，y两个方向
//上的梯度，简化了算法，提高效率
//-------------------------------------------------------------------------------------
void sobelAutoThreshold(const uint8 image_mt[MT9V03X_H/2][MT9V03X_W],uint8 target[MT9V03X_H/2][MT9V03X_W])
{
    const int x_start = 1;
    const int x_end = MT9V03X_W - 1;
    const int y_start = 1;
    const int y_end = (MT9V03X_H/2) - 1;
    
    for (int y = y_start; y < y_end; y++) {
        for (int x = x_start; x < x_end; x++) {
            // 使用1x3卷积核计算水平梯度
            int16 gx = -image_mt[y][x-1] + image_mt[y][x+1];
            
            // 使用1x3卷积核计算垂直梯度
            int16 gy = -image_mt[y-1][x] + image_mt[y+1][x];
            
            // 计算梯度幅值（简化版，避免开平方）
            int16 gradient = abs(gx) + abs(gy);
            
            // 计算3x3邻域和
            uint16_t neighborhood_sum = 
                image_mt[y-1][x-1] + image_mt[y-1][x] + image_mt[y-1][x+1] +
                image_mt[y][x-1]   + image_mt[y][x]   + image_mt[y][x+1] +
                image_mt[y+1][x-1] + image_mt[y+1][x] + image_mt[y+1][x+1];
            
            // 自适应阈值比较
            if (gradient > (neighborhood_sum / 12)) {
                target[y][x] = 0xFF;
            } else {
                target[y][x] = 0x00;
            }
        }
    }
    
    // 处理边缘区域
    memset(target, 0, MT9V03X_W * y_start);
    memset(target + (MT9V03X_H/2 - 1), 0, MT9V03X_W);
    
    for (int y = y_start; y < y_end; y++) {
        target[y][0] = 0;
        target[y][MT9V03X_W-1] = 0;
    }
}





//-------------------------------------------------------------------------------------
//使用大津法进行二值化处理
//-------------------------------------------------------------------------------------
uint8 otsuThreshold(uint8 *image, uint16 width, uint16 height)
{
    
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
    float w0 = 0, avgValue  = 0;  //w0 前景比例 ，avgValue 前景平均灰度
    for(int i = 0; i < 256; i++)     //每一次循环都是一次完整类间方差计算 (两个for叠加为1个)
    {  
        w0 += pixel_account[i];  //假设当前灰度i为阈值, 0~i 灰度像素所占整幅图像的比例即前景比例
        avgValue  += i * pixel_account[i];        
        
        float variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //类间方差   
        if(variance > variance_max) 
        {  
            variance_max = variance;  
            threshold = i;  
        }  
    } 

    return threshold;

}

  
//---------------------------------------------------------------------------
//使用差比和的算法进行巡线,按照逐飞的思路
//---------------------------------------------------------------------------

uint8 reference_point = 0,reference_col = 255;
uint8 whitemax = 0;
uint8 whitemin = 0;
uint8 remote_distance[MT9V03X_W];
uint8 reference_col_line[MT9V03X_H];
static uint8 reference_contrast = 20;
uint8 left_edge_line[MT9V03X_H];
uint8 right_edge_line[MT9V03X_H];


//int32 func_limit (int32 num1, int32 num2, int32 limit)
//{
//	if (num1 <= num2)
//	{
//		return 0;
//	}
//	else if (num1 > num2 && num1 <= limit)
//	{
//		return num1;
//	}
//	else
//	{
//		return  limit;
//	}
//	
//}

//---------------------------------------------------------------------------
//寻找参考白点的灰度值，这里用后四行的平均灰度值最为参考白点的灰度值
//---------------------------------------------------------------------------
void get_reference_point (const uint8 *image)
{
	uint8 *p = (uint8 *)&image[(MT9V03X_H - REFERENCEHEIGHT) * MT9V03X_W];    //在二维数组上找到求后四行灰度值的起点
	uint16 temp = 0;                                                          //
	uint32 temp1 = 0;
	temp = REFERENCEHEIGHT * MT9V03X_W;
	for (uint8 i = 0; i < temp; i++)
	{
		temp1 += *(image + i);
	}
	reference_point = temp1 / temp;
	whitemax = reference_point * WHITEMAX /10;
	whitemin = reference_point * WHITEMIN /10;
	if (whitemax >= 255)
	{
		whitemax = 255;
	}
	
}

void search_reference_col (const uint8 *image)
{
	uint8 col,row;
	uint16 temp1 = 0,temp2 = 0,temp3 = 0;
	
	for (col = 0; col < MT9V03X_W; col++)
	{
		remote_distance[col] = MT9V03X_H - 1;
	}
	for (col = 0; col < MT9V03X_W; col += CONTRASTOFFSET)                  //这里的for循环经历了几万次，我猜是很费时间的
	{
		for (row = MT9V03X_H - 1; row > STOPROW; row -= CONTRASTOFFSET)
		{
			temp1 = *(image + row * MT9V03X_W + col);
			
			temp2 = *(image + (row - CONTRASTOFFSET) * MT9V03X_W + col);
			
			
			if (temp2 > whitemax)
			{
				continue;
			}
			if (temp1 < whitemin)
			{
				remote_distance[col] = row;
				break;
			}
			
			temp3 = (temp1 - temp2) * 128 /(temp1 + temp2);
			
			if (temp3 >= reference_contrast || row == STOPROW)
			{
				remote_distance[col] = row;
				break;
			}
			
		}
	}
	reference_col = remote_distance[0];
	for (uint8 i = 0; i < MT9V03X_W; i++)
	{
		if (reference_col < remote_distance[i])
		{
			reference_col = remote_distance[i];
		}
	}
	
	//
	for (uint8 i = 0; i < MT9V03X_H; i++)
	{
		reference_col_line[i] = reference_col;
	}
}


void search_line(const uint8 *image)
{
    uint8 *p = (uint8 *)image[0];                          // 图像数组指针
    uint8 row_max = MT9V03X_H - 1;                    // 行最大值
    uint8 row_min = STOPROW;                               // 行最小值
    uint8 col_max = MT9V03X_W - CONTRASTOFFSET;       // 列最大值 
    uint8 col_min = CONTRASTOFFSET;                        // 列最小值
    int16 leftstartcol  = reference_col;                   // 搜线左起始列
    int16 rightstartcol = reference_col;                   // 搜线右起始列
    int16 leftendcol    = 0;                               // 搜线左终止列
    int16 rightendcol   = MT9V03X_W - 1;              // 搜线右终止列
    uint8 search_time   = 0;                               // 单点搜线次数
    uint8 temp1 = 0, temp2 = 0;                            // 临时变量 用于存储图像数据
    int   temp3 = 0;                                        // 临时变量 用于存储对比度
    int   leftstop = 0, rightstop = 0, stoppoint = 0;      // 搜线自锁变量

    int col, row;

    for(row = row_max; row >= row_min; row --){
        left_edge_line[row]  = col_min - CONTRASTOFFSET;
        right_edge_line[row] = col_max + CONTRASTOFFSET;
    }

    for(row = row_max; row >= row_min; row --)
	{
        p = (uint8 *)&image[row * MT9V03X_W];         // 获取本行起点位置指针
        if (!leftstop) 
		{
			search_time = 2;
			do {
				if (search_time == 1) {
					leftstartcol = reference_col;
					leftendcol = col_min;
				}
				search_time--;
				for (col = leftstartcol; col >= leftendcol; col--) {
					temp1 = *(p + col);  // 获取当前点灰度值
					temp2 = *(p + col - CONTRASTOFFSET);  // 获取对比点灰度值

					
					if (temp1 < whitemax && col == leftstartcol && leftstartcol == reference_col) 
					{ 
						leftstop = 1;  // 搜线索引自锁，不在进行左边线搜索
						for (stoppoint = row; stoppoint >= 0; stoppoint--) {
							left_edge_line[stoppoint] = col_min;
						}
						search_time = 0;
						break;
					}
					if (temp1 < whitemin)  // 判断当前点是否为黑点 若为黑点则不进行对比
					{
						left_edge_line[row] = (uint8)col;
						break;
					}

					if (temp2 > whitemax)  // 判断对比点是否为白点 若为白点则直接跳过
					{
						continue;
					}

					temp3 = (temp1 - temp2) * 200 / (temp1 + temp2);  // 计算对比度

					if (temp3 > reference_contrast || col == col_min)  // 判断对比度是否大于阈值
					{
						left_edge_line[row] = col - CONTRASTOFFSET;  // 保存当前行左边界
						leftstartcol = (uint8)col + SEARCHRANGE;
						if (leftstartcol > col_max)
							leftstartcol = col_max;
						leftendcol = (uint8)col - SEARCHRANGE;
						search_time = 0;
						break;
					}
				}
			} while (search_time);
		}	
    }
}
