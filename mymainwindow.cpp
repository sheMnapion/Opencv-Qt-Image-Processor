#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include <QFileDialog>
#include <QString>
#include <QObject>
#include <QTextCodec>
#include <QMessageBox>
#include <QErrorMessage>
#include <QDebug>
#include <QIcon>
#include <QActionGroup>
#include <QAction>
#include <QTextFrame>
#include <QDateTime>

Mat image,grayImage,featureImage;
MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("My OpenCV Image Processor"));
    _processList.clear();
    _processPointer=-1;
    ui->textEdit->clear();
    ui->textEdit->insertHtml(tr("<h3>Welcome to Lac's Image Processor!<br>Version 2.0</h3>"));
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}
// some help functions
// include mat-qimage transformation, html log helpers and etc
QImage Mat2QImage(cv::Mat &imago)
{
    QImage img;

    if(imago.channels()==3)
    {
        //cvt Mat BGR 2 QImage RGB
        cout<<"Normal picture"<<endl;
        cv::cvtColor(imago,imago,CV_BGR2RGB);
        img =QImage((const unsigned char*)(imago.data),
                    imago.cols,imago.rows,
                    imago.cols*imago.channels(),
                    QImage::Format_RGB888);
    }
    else if(imago.type()==CV_8UC1){
        cout<<"Gray picture!"<<endl;
        img =QImage((const unsigned char*)(imago.data),
                    imago.cols,imago.rows,
                    imago.cols*imago.channels(),
                    QImage::Format_Grayscale8);
    }
    else
    {
        img =QImage((const unsigned char*)(imago.data),
                    imago.cols,imago.rows,
                    imago.cols*imago.channels(),
                    QImage::Format_RGB888);
    }

    return img;
}
void MyMainWindow::htmlLog(QString &color,QString &info,QString &font,bool addTime=true)
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

// end of help functions
void MyMainWindow::on_actionExit_E_triggered()
{
    this->close();
}
void MyMainWindow::addInProcessList(Mat &img)
{
    Mat *localCopy=new Mat();
    img.copyTo(*localCopy);
    if(_processList.size()==_processPointer+1){
        _processList.push_back(localCopy);
        _processPointer++;
    }
    else{
        _processList[_processPointer++]=localCopy;
    }
}

void MyMainWindow::on_actionLoad_Picture_L_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),"",tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    string name = code->fromUnicode(filename).data();
    cout<<"Opened "<<name<<endl;
    qDebug()<<ui->label->size();
    if(name.empty()){
        //do nothing
    }
    else{
        image=imread(name);
        if(!image.data){
            QMessageBox msgBox;
            msgBox.setText(tr("No such image"));
            msgBox.exec();
            ui->label->clear();
        }
        else{
            notGrayed=true;
            if(image.cols>ui->label->size().height()&&image.rows>ui->label->size().width())
                cv::resize(image,image,Size(ui->label->size().width(),ui->label->size().height()));
            addInProcessList(image);
            QImage img=Mat2QImage(image);
            //store the picture
            ui->label->clear();
            ui->label->setPixmap(QPixmap::fromImage(img));
            QString logInfo=filename+" opened.";
            htmlLog(tr("maroon"),logInfo,tr("times"));
        }
    }
}

void MyMainWindow::on_actionGray_Scale_G_triggered()
{
    if(!image.data){
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    }
    else if(!notGrayed){
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is already gray!"));
        msgBox.exec();
    }
    else{
        cvtColor(image,grayImage,COLOR_BGR2GRAY);
        notGrayed=false;
        addInProcessList(grayImage);
        QImage img=Mat2QImage(grayImage);
        ui->label->clear();
        ui->label->setPixmap(QPixmap::fromImage(img));
    }
}

void MyMainWindow::on_actionRestore_R_triggered()
{
    if(!image.data){
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    }
    else if(notGrayed){
        //do nothing
    }
    else{
        notGrayed=true;
        addInProcessList(image);
        QImage img=Mat2QImage(image);
        cvtColor(image,image,COLOR_BGR2RGB);
        ui->label->clear();
        ui->label->setPixmap(QPixmap::fromImage(img));
    }
}

void MyMainWindow::on_actionNext_N_triggered()
{
    qDebug()<<"Next: "<<_processPointer;
    if(_processPointer+1==_processList.size()){//already full list
        QErrorMessage *errorDialog=new QErrorMessage(this);
        errorDialog->setWindowTitle(tr("EndOfProcessList Error"));
        errorDialog->showMessage(tr("Reached end of processing history!"));
    }
    else{
        Mat* nextImageToShow=_processList[++_processPointer];
        ui->label->clear();
        ui->label->setPixmap(QPixmap::fromImage(Mat2QImage(*nextImageToShow)));
    }
}

void MyMainWindow::on_actionCancel_C_triggered()
{
    qDebug()<<"Last: "<<_processPointer;
    if(_processPointer<=0){
        QErrorMessage *errorDialog=new QErrorMessage(this);
        errorDialog->setWindowTitle(tr("NoFormerImage Error"));
        errorDialog->showMessage(tr("No former processing history found!"));
    }
    else{
        Mat* lastImageToShow=_processList[--_processPointer];
        ui->label->clear();
        ui->label->setPixmap(QPixmap::fromImage(Mat2QImage(*lastImageToShow)));
    }
}

Mat* MyMainWindow::getPresentMatrix()
{
    if(_processPointer<0)
        return NULL;
    return _processList[_processPointer];
}

void MyMainWindow::on_actionSave_Picture_S_triggered()
{
    Mat *temp=getPresentMatrix();
    if(temp==NULL){
        QErrorMessage *errorDialog=new QErrorMessage(this);
        errorDialog->setWindowTitle(tr("NoImage Error"));
        errorDialog->showMessage(tr("No available image to save!"));
    }
    else{
        QString saveFileName=QFileDialog::getSaveFileName(this,tr("Save Processed Image"),"",
                                                          tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
        QTextCodec *code = QTextCodec::codecForName("gb18030");
        string name = code->fromUnicode(saveFileName).data();
        cout<<"File name:["<<name<<"]"<<endl;
        if(name.empty())
            return;//not saved
        imwrite(name,*temp);
    }
}

void MyMainWindow::on_actionClear_C_triggered()
{
    Mat *presentImage=getPresentMatrix();
    if(presentImage==NULL)
        return;
    _processList.clear();
    _processPointer=-1;
    addInProcessList(*presentImage);
    htmlLog(tr("CadetBlue"),tr("Process list cleared!"),tr("Times New Roman"),false);
}

//git supported now
//trial line, to delete later
