#ifndef _image_handle_h_
#define _image_handle_h_

#include "zf_common_typedef.h"
#include "zf_device_mt9v03x.h"
#include "math.h"

#define GrayScale 256//����256���Ҷȼ�

void sobelAutoThreshold(const uint8 image_mt[MT9V03X_H/2][MT9V03X_W],uint8 target[MT9V03X_H/2][MT9V03X_W]);
uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row);

#endif
