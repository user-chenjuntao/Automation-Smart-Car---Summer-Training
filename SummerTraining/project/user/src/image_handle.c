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

  
