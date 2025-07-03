#ifndef _image_handle_h_
#define _image_handle_h_

#include "zf_common_typedef.h"
#include "zf_device_mt9v03x.h"
#include "math.h"

#define GrayScale 256//定义256个灰度级
#define REFERENCEHEIGHT   5
#define WHITEMAX          13
#define WHITEMIN          7
#define CONTRASTOFFSET    3
#define STOPROW           3
#define SEARCHRANGE       10

void sobelAutoThreshold(const uint8 image_mt[MT9V03X_H/2][MT9V03X_W],uint8 target[MT9V03X_H/2][MT9V03X_W]);
uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row);
//int32 func_limit (int32 num1, int32 num2, int32 limit);

#endif
