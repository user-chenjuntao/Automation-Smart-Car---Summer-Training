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

  
