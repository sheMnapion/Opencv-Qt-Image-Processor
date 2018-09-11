#ifndef OPENCVHELPER_H
#define OPENCVHELPER_H

#include "mymainwindow.h"
#include <ctype.h>
#include <stdint.h>
uchar *pixelColor(Mat &img,int x,int y);
QImage *Mat2QImage(const Mat &img);
void setFeatureMatrix(Mat &image, Mat &featureImage, int threshold,QString &method);
void setContraBrightMatrix(Mat &img,Mat &dst,int contra,int bright);
void setEqualizedMatrix(Mat &img,Mat &dst);
void getRetinaMatrix(Mat &img,Mat &dst);
#endif // OPENCVHELPER_H
