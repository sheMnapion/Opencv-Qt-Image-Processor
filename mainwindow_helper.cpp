#include "mainwindow_helper.h"
#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include <QDateTime>
#include <opencvhelper.h>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QDebug>

// some help functions
// include process operations, html log helpers and etc

int MyMainWindow::getNextWindowNumber()
{
    bool occupied[10];
    memset(occupied,0,sizeof(occupied));
    QList<QMdiSubWindow* > allWindows=ui->multiImageArea->subWindowList();
    foreach (QMdiSubWindow* temp,allWindows){
        QString name=temp->windowTitle();
        string tempName=name.toStdString();
        char c=tempName[6];
        int windowNumber=c-'0';
        occupied[windowNumber]=true;
    }
    int ret=-1;
    for(int i=0;i<10;i++){
        if(!occupied[i]){
            ret=i;
            break;
        }
    }
    return ret;
}
int MyMainWindow::getPresentWindowNumber()
{
    if(_presentMode==SINGLE_IMAGE_EDIT_MODE)
        return -1;
    QWidget *focus=ui->multiImageArea->focusWidget();
    if(focus==NULL)
        return -1;
    QString temp=focus->windowTitle();
    string tempString=temp.toStdString();
    return tempString[6]-'0';
}

void MyMainWindow::addInProcessList(Mat &img,int arrayNumber)
{
    Mat *localCopy=new Mat();
    img.copyTo(*localCopy);
    vector<Mat*> *processor;
    int *processPointer=NULL;
    if(_presentMode==SINGLE_IMAGE_EDIT_MODE){
        processor=&_processList;
        processPointer=&_processPointer;
    }
    else if(_presentMode==MULTIPLE_IMAGE_EDIT_MODE){
        processor=&_multiProcessList[arrayNumber];
        processPointer=&_multiProcessPointer[arrayNumber];
    }
    else
        TODO();
    int size=*processPointer;
    qDebug()<<"Before size: "<<size;
    if(processor->size()==(size+1)){
        processor->push_back(localCopy);
        *processPointer=size+1;
    }
    else{
        while(processor->size()>size+1)
            processor->pop_back();
        processor->push_back(localCopy);
        *processPointer=size+1;
    }
    qDebug()<<"Process pointer: "<<*processPointer<<"\t"<<_processPointer;
}

void MyMainWindow::clearProcessList(int arrayNumber)
{
    if(arrayNumber<0){
        _processList.clear();
        _processPointer=-1;
    }
    else{
        _multiProcessList[arrayNumber].clear();
        _multiProcessPointer[arrayNumber]=-1;
    }
}

void MyMainWindow::on_SubWindowClosed()
{
    cout<<"CLOSED!"<<endl;
}

void MyMainWindow::setDisplayImage(Mat &img,bool newImage,bool newWindow,int arrayNumber)
{
    _presentSize=ui->label->size();
    assert(img.empty()==false);
    Mat tempImage=img.clone();
    if(_presentMode==SINGLE_IMAGE_EDIT_MODE){
        if(newImage){
            qDebug()<<"Add in.";
            addInProcessList(img); //store the picture
        }
        // display adjustments
        if(img.cols>ui->label->size().height()||img.rows>ui->label->size().width())
            cv::resize(tempImage,tempImage,Size(ui->label->size().width(),ui->label->size().height()));
        QImage *imgo=Mat2QImage(tempImage);
        ui->label->clear();
        ui->label->setPixmap(QPixmap::fromImage(*imgo));
    }
    else{
        if(img.cols>300||img.rows>300)
            cv::resize(tempImage,tempImage,Size(300,300));
        QImage *imgo=Mat2QImage(tempImage);
        int windowNumber;
        if(newWindow){//buggy version, don't consider situ vn num(pic)>10
            windowNumber=getNextWindowNumber();
            cout<<"Vacant number: "<<windowNumber<<endl;
        }
        else{
            windowNumber=getPresentWindowNumber();
            QWidget *presentFocus=ui->multiImageArea->focusWidget();
            presentFocus->destroyed(this);
        }
        if(newImage)
            addInProcessList(tempImage,windowNumber);
        QLabel *subImage=new QLabel(this);
        subImage->resize(300,300);
        subImage->setPixmap(QPixmap::fromImage(*imgo));
        subImage->setScaledContents(true);
        QMdiSubWindow *child=ui->multiImageArea->addSubWindow(subImage);

        QString windowTitle=QString("Image ")+QString::number(windowNumber,10);
        child->setWindowTitle(windowTitle);
        child->show();
    }
}

void MyMainWindow::setDetectionDisplay(bool enabled)
{
    ui->label_2->setVisible(enabled);
    ui->label_3->setVisible(enabled);
    ui->detectSelectionBox->setVisible(enabled);
    ui->detectThresholdBox->setVisible(enabled);
    ui->startDetectButton->setVisible(enabled);
    ui->endDetectButton->setVisible(enabled);
}

void MyMainWindow::setContraBrightDisplay(bool enabled)
{
    ui->brightLabel->setVisible(enabled);
    ui->brightSlider->setVisible(enabled);
    ui->contrastLabel->setVisible(enabled);
    ui->contrastSlider->setVisible(enabled);
    ui->brightContrastEndButton->setVisible(enabled);
}

void MyMainWindow::htmlLog(QString &color,QString &info,QString &font,bool addTime)
{
    QString insertMessage="<br><p style=' font-family : ";
    insertMessage+=font;
    insertMessage+="; color : ";
    insertMessage+=color;
    insertMessage+=" '>";
    insertMessage+=info;
    insertMessage+="<br>";
    if(addTime)
        insertMessage+=QDateTime::currentDateTime().toString();
    ui->textEdit->insertHtml(insertMessage);
}

void MyMainWindow::switchMode()
{
    //dumb implementation here, modify later for future use
    if(_presentMode==SINGLE_IMAGE_EDIT_MODE){
        ui->label->setVisible(false);
        ui->multiImageArea->setVisible(true);
    }
    else if(_presentMode==MULTIPLE_IMAGE_EDIT_MODE){
        ui->label->setVisible(true);
        ui->multiImageArea->setVisible(false);
    }
    _presentMode=!_presentMode;
}

// end of help functions
