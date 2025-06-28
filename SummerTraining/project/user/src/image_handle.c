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