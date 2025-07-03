#include "image_handle.h"

//-------------------------------------------------------------------------------------
//��������������Ե����㷨���ж�ֵ���������Ǿ����3*3�򻯳�3*1��ͬʱֻ��x��y��������
//�ϵ��ݶȣ������㷨�����Ч��
//-------------------------------------------------------------------------------------
void sobelAutoThreshold(const uint8 image_mt[MT9V03X_H/2][MT9V03X_W],uint8 target[MT9V03X_H/2][MT9V03X_W])
{
    const int x_start = 1;
    const int x_end = MT9V03X_W - 1;
    const int y_start = 1;
    const int y_end = (MT9V03X_H/2) - 1;
    
    for (int y = y_start; y < y_end; y++) {
        for (int x = x_start; x < x_end; x++) {
            // ʹ��1x3����˼���ˮƽ�ݶ�
            int16 gx = -image_mt[y][x-1] + image_mt[y][x+1];
            
            // ʹ��1x3����˼��㴹ֱ�ݶ�
            int16 gy = -image_mt[y-1][x] + image_mt[y+1][x];
            
            // �����ݶȷ�ֵ���򻯰棬���⿪ƽ����
            int16 gradient = abs(gx) + abs(gy);
            
            // ����3x3�����
            uint16_t neighborhood_sum = 
                image_mt[y-1][x-1] + image_mt[y-1][x] + image_mt[y-1][x+1] +
                image_mt[y][x-1]   + image_mt[y][x]   + image_mt[y][x+1] +
                image_mt[y+1][x-1] + image_mt[y+1][x] + image_mt[y+1][x+1];
            
            // ����Ӧ��ֵ�Ƚ�
            if (gradient > (neighborhood_sum / 12)) {
                target[y][x] = 0xFF;
            } else {
                target[y][x] = 0x00;
            }
        }
    }
    
    // �����Ե����
    memset(target, 0, MT9V03X_W * y_start);
    memset(target + (MT9V03X_H/2 - 1), 0, MT9V03X_W);
    
    for (int y = y_start; y < y_end; y++) {
        target[y][0] = 0;
        target[y][MT9V03X_W-1] = 0;
    }
}





//-------------------------------------------------------------------------------------
//ʹ�ô�򷨽��ж�ֵ������
//-------------------------------------------------------------------------------------
uint8 otsuThreshold(uint8 *image, uint16 width, uint16 height)
{
    
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
    float w0 = 0, avgValue  = 0;  //w0 ǰ������ ��avgValue ǰ��ƽ���Ҷ�
    for(int i = 0; i < 256; i++)     //ÿһ��ѭ������һ��������䷽����� (����for����Ϊ1��)
    {  
        w0 += pixel_account[i];  //���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷ�������ռ����ͼ��ı�����ǰ������
        avgValue  += i * pixel_account[i];        
        
        float variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //��䷽��   
        if(variance > variance_max) 
        {  
            variance_max = variance;  
            threshold = i;  
        }  
    } 

    return threshold;

}

  
//---------------------------------------------------------------------------
//ʹ�ò�Ⱥ͵��㷨����Ѳ��,������ɵ�˼·
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
//Ѱ�Ҳο��׵�ĻҶ�ֵ�������ú����е�ƽ���Ҷ�ֵ��Ϊ�ο��׵�ĻҶ�ֵ
//---------------------------------------------------------------------------
void get_reference_point (const uint8 *image)
{
	uint8 *p = (uint8 *)&image[(MT9V03X_H - REFERENCEHEIGHT) * MT9V03X_W];    //�ڶ�ά�������ҵ�������лҶ�ֵ�����
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
	for (col = 0; col < MT9V03X_W; col += CONTRASTOFFSET)                  //�����forѭ�������˼���Σ��Ҳ��Ǻܷ�ʱ���
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
    uint8 *p = (uint8 *)image[0];                          // ͼ������ָ��
    uint8 row_max = MT9V03X_H - 1;                    // �����ֵ
    uint8 row_min = STOPROW;                               // ����Сֵ
    uint8 col_max = MT9V03X_W - CONTRASTOFFSET;       // �����ֵ 
    uint8 col_min = CONTRASTOFFSET;                        // ����Сֵ
    int16 leftstartcol  = reference_col;                   // ��������ʼ��
    int16 rightstartcol = reference_col;                   // ��������ʼ��
    int16 leftendcol    = 0;                               // ��������ֹ��
    int16 rightendcol   = MT9V03X_W - 1;              // ��������ֹ��
    uint8 search_time   = 0;                               // �������ߴ���
    uint8 temp1 = 0, temp2 = 0;                            // ��ʱ���� ���ڴ洢ͼ������
    int   temp3 = 0;                                        // ��ʱ���� ���ڴ洢�Աȶ�
    int   leftstop = 0, rightstop = 0, stoppoint = 0;      // ������������

    int col, row;

    for(row = row_max; row >= row_min; row --){
        left_edge_line[row]  = col_min - CONTRASTOFFSET;
        right_edge_line[row] = col_max + CONTRASTOFFSET;
    }

    for(row = row_max; row >= row_min; row --)
	{
        p = (uint8 *)&image[row * MT9V03X_W];         // ��ȡ�������λ��ָ��
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
					temp1 = *(p + col);  // ��ȡ��ǰ��Ҷ�ֵ
					temp2 = *(p + col - CONTRASTOFFSET);  // ��ȡ�Աȵ�Ҷ�ֵ

					
					if (temp1 < whitemax && col == leftstartcol && leftstartcol == reference_col) 
					{ 
						leftstop = 1;  // �����������������ڽ������������
						for (stoppoint = row; stoppoint >= 0; stoppoint--) {
							left_edge_line[stoppoint] = col_min;
						}
						search_time = 0;
						break;
					}
					if (temp1 < whitemin)  // �жϵ�ǰ���Ƿ�Ϊ�ڵ� ��Ϊ�ڵ��򲻽��жԱ�
					{
						left_edge_line[row] = (uint8)col;
						break;
					}

					if (temp2 > whitemax)  // �ж϶Աȵ��Ƿ�Ϊ�׵� ��Ϊ�׵���ֱ������
					{
						continue;
					}

					temp3 = (temp1 - temp2) * 200 / (temp1 + temp2);  // ����Աȶ�

					if (temp3 > reference_contrast || col == col_min)  // �ж϶Աȶ��Ƿ������ֵ
					{
						left_edge_line[row] = col - CONTRASTOFFSET;  // ���浱ǰ����߽�
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
