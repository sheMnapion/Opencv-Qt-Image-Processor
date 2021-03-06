#include "opencvhelper.h"
#include "mymainwindow.h"
#include <QDebug>
#include <QString>

uchar *pixelColor(Mat &img,int x,int y)
{
    if(x>img.rows||y>img.cols||x<0||y<0){
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
int setFeatureMatrix(Mat &image,Mat &featureImage,int threshold,QString &method)
{
    if(image.empty())
        return EMPTY_INPUT;//do nothing for empty image
    if(image.channels()!=3&&image.channels()!=4)
        return NOT_ENOUGH_CHANNELS;
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
    return 0;
}

QImage *Mat2QImage(const Mat &imago)
{
    QImage *img;

    if(imago.channels()==3)
    {
        //cvt Mat BGR 2 QImage RGB
        cout<<"Normal picture"<<endl;
        Mat *temp=new Mat();
        *temp=imago.clone();
        cv::cvtColor(*temp,*temp,CV_BGR2RGB);
        img =new QImage((const unsigned char*)(temp->data),
                    temp->cols,temp->rows,
                    temp->cols*temp->channels(),
                    QImage::Format_RGB888);
    }
    else if(imago.type()==CV_8UC1){
        cout<<"Gray picture!"<<endl;
        img =new QImage((const unsigned char*)(imago.data),
                    imago.cols,imago.rows,
                    imago.cols*imago.channels(),
                    QImage::Format_Grayscale8);
    }
    else
    {
        img =new QImage((const unsigned char*)(imago.data),
                    imago.cols,imago.rows,
                    imago.cols*imago.channels(),
                    QImage::Format_RGB888);
    }

    return img;
}

void setContraBrightMatrix(Mat &img, Mat &dst, int contra, int bright)
{
    cvtColor(img,img,COLOR_BGR2GRAY);
    int deltaContra=contra-100;
    int deltaBright=bright-100;

    double a,b;
    if(deltaContra>0){
        double delta=127.*deltaContra/100;
        a=255./(255.-2*delta);
        b=a*(deltaBright-delta);
    }
    else{
        double delta=-128.*deltaContra/100;
        a=(256.-delta*2)/255.;
        b=a*deltaBright+delta;
    }
    img.convertTo(dst,CV_8U,a,b);
}

void setEqualizedMatrix(Mat &img, Mat &dst)
{
    if(img.empty()==true){
        qDebug()<<"Empty image to equalize!";
        return;
    }
    vector<Mat> bgrPlanes;
    split(img,bgrPlanes);
    int size=bgrPlanes.size();
    for(int i=0;i<size;i++)
        equalizeHist(bgrPlanes[i],bgrPlanes[i]);
    merge(bgrPlanes,dst);
}

void getRetinaMatrix(Mat &img, Mat &dst)
{
    Retina myRetina(img.size());
    //save default retina parameters file
    // myRetina->write("RetinaDefaultParameters.xml");
    //The parameters may be reloaded using method setup
    myRetina.clearBuffers();
    myRetina.run(img);
    //retrieve and display retina output
    myRetina.getParvo(dst);
}

QString cvhelperError(int errorNumber)
{
    QString ret;
    switch(errorNumber){
        case SUCCESS:
            ret=QString("Success");
            break;
        case NOT_ENOUGH_CHANNELS:
            ret=QString("The input image doesn't contain enough channel for desired operation");
            break;
        case EMPTY_INPUT:
            ret=QString("The input image is empty");
            break;
        default:
            ret=QString("Unexpected input!!!");
            break;
    }
    return ret;
}
