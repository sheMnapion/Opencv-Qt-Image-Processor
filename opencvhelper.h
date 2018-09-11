#ifndef OPENCVHELPER_H
#define OPENCVHELPER_H

#include "mymainwindow.h"
#include <ctype.h>
#include <stdint.h>
#include <QString>

#define SUCCESS             0
#define NOT_ENOUGH_CHANNELS 0x1
#define EMPTY_INPUT         0x2

uchar *pixelColor(Mat &img,int x,int y);
QImage *Mat2QImage(const Mat &img);
int setFeatureMatrix(Mat &image, Mat &featureImage, int threshold,QString &method);
void setContraBrightMatrix(Mat &img,Mat &dst,int contra,int bright);
void setEqualizedMatrix(Mat &img,Mat &dst);
void getRetinaMatrix(Mat &img,Mat &dst);
QString cvhelperError(int errorNumber);

#endif // OPENCVHELPER_H
