#include "opencvhelper.h"

uchar *pixelColor(Mat &img,int x,int y)
{
    if(x>img.rows||y>img.cols){
        cout<<"Invalid position!!!"<<endl;
        return NULL;
    }
    int channelNumber=img.channels();
    assert(channelNumber>=1);
    uchar *ret=(uchar *)malloc(sizeof(uchar)*channelNumber);
    uchar *poz=img.ptr<uchar>(x);
    for(int i=0;i<channelNumber;i++)
        ret[i]=poz[i];
    return ret;
}
