//#include "image_handle.h"



//uint8 PostProcessing_image[MT9V03X_H][MT9V03X_W];                                      //用于存放二值化处理后的图像数组
//uint8 Threshold = 0;                                                                   //大津法处理后阈值




///*-------------------------------------------------------------------------------------------------------------------
//  @brief     普通大津求阈值
//  @param     image       图像数组
//             width       列 ，宽度
//             height      行，长度
//  @return    threshold   返回int类型的的阈值
//  Sample     threshold=my_adapt_threshold(mt9v03x_image[0],MT9V03X_W, MT9V03X_H);//普通大津
//  @note      据说没有山威大津快，我感觉两个区别不大
//-------------------------------------------------------------------------------------------------------------------*/
//int My_Adapt_Threshold(uint8*image,uint16 width, uint16 height)   //大津算法，注意计算阈值的一定要是原图像
//{
//    #define GrayScale 256
//    int pixelCount[GrayScale];
//    float pixelPro[GrayScale];
//    int i, j;
//    int pixelSum = width * height/4;
//    int  threshold = 0;
//    uint8* data = image;  //指向像素数据的指针
//    for (i = 0; i < GrayScale; i++)
//    {
//        pixelCount[i] = 0;
//        pixelPro[i] = 0;
//    }
//    uint32 gray_sum=0;
//    for (i = 0; i < height; i+=2)//统计灰度级中每个像素在整幅图像中的个数
//    {
//        for (j = 0; j <width; j+=2)
//        {
//            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
//            gray_sum+=(int)data[i * width + j];       //灰度值总和
//        }
//    }
//    for (i = 0; i < GrayScale; i++) //计算每个像素值的点在整幅图像中的比例
//    {
//        pixelPro[i] = (float)pixelCount[i] / pixelSum;
//    }
//    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
//    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
//    for (j = 0; j < GrayScale; j++)//遍历灰度级[0,255]
//    {
//        w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
//        u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值
//        w1=1-w0;
//        u1tmp=gray_sum/pixelSum-u0tmp;
//        u0 = u0tmp / w0;              //背景平均灰度
//        u1 = u1tmp / w1;              //前景平均灰度
//        u = u0tmp + u1tmp;            //全局平均灰度
//        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);//平方
//        if (deltaTmp > deltaMax)
//        {
//            deltaMax = deltaTmp;//最大类间方差法
//            threshold = j;
//        }
//        if (deltaTmp < deltaMax)
//        {
//            break;
//        }
//    }
//    if(threshold>255)
//        threshold=255;
//    if(threshold<0)
//        threshold=0;
//  return threshold;
//}


///*-------------------------------------------------------------------------------------------------------------------
//  @brief     图像二值化处理函数
//  @param     二值化阈值
//  @return    NULL
//  Sample     Image_Binarization(Threshold);//图像二值化
//  @note      二值化后直接访问image_two_value[i][j]这个数组即可
//-------------------------------------------------------------------------------------------------------------------*/
//void Image_Binarization(int threshold)//图像二值化
//{
//    uint16 i,j;
//    for(i=0;i<MT9V03X_H;i++)
//    {
//        for(j=0;j<MT9V03X_W;j++)//灰度图的数据只做判断，不进行更改，二值化图像放在了新数组中
//        {
//            if(mt9v03x_image[i][j]>=threshold)
//                PostProcessing_image[i][j]=WHITE_PIXEL;//白
//            else
//                PostProcessing_image[i][j]=BLACK_PIXEL;//黑
//        }
//    }
//}


//void image_process(void)
//{

//    Threshold=My_Adapt_Threshold(mt9v03x_image[0],MT9V03X_W, MT9V03X_H);//大津计算阈值
//    Image_Binarization(Threshold);//图像二值化

//}