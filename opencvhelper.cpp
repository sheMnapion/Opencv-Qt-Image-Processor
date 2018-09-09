#include "opencvhelper.h"
#include <QDebug>

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
void setFeatureMatrix(Mat &image,Mat &featureImage,int threshold,QString &method)
{
    if(image.empty())
        return;//do nothing for empty image
    qDebug()<<method<<" with threshold "<<threshold;
    Mat tempImage;
    cvtColor(image, tempImage, COLOR_BGR2GRAY);
    //create a keypoint vectors
    vector<KeyPoint> keypoints;
    //use method FAST for now
    if(method=="Fast Detector"){
        FastFeatureDetector fastDetector(threshold);
        //Compute keypoints in in_img with detector1 and detector2
        fastDetector.detect(tempImage, keypoints);
        drawKeypoints(image, keypoints, featureImage, Scalar::all(-1));
    }
    else if(method=="Surf Detector"){
        SurfFeatureDetector surfDetector(3500,threshold,2,false,false);
        surfDetector.detect(image,keypoints);
        drawKeypoints(image,keypoints,featureImage,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    }
    else if(method=="Orb Detector"){
        OrbFeatureDetector orbDetector(threshold,1.1f,2,31,0,2,ORB::HARRIS_SCORE,31);
        orbDetector.detect(image,keypoints);
        drawKeypoints(image,keypoints,featureImage);
    }
    else{
        qDebug()<<"TODO";
    }
    qDebug()<<"Size: "<<keypoints.size();
}
