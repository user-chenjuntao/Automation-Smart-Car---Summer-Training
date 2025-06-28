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