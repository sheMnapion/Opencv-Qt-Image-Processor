#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include "opencvhelper.h"
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
#include <QEvent>
#include <QMimeData>
#include <QResizeEvent>

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
    setAcceptDrops(true);
    ui->label->setScaledContents(true);
    setDetectionDisplay(false);
    //detection box initialization
    ui->detectSelectionBox->addItem("Fast Detector");
    ui->detectSelectionBox->addItem("Surf Detector");
    ui->detectSelectionBox->addItem("Orb Detector");
    ui->detectThresholdBox->setMinimum(0);
    ui->detectThresholdBox->setMaximum(255);
    ui->detectThresholdBox->setValue(70);
    //contrast box initialization
    setContraBrightDisplay(false);
    ui->contrastSlider->setMinimum(0);
    ui->contrastSlider->setMaximum(200);
    ui->contrastSlider->setValue(100);
    ui->brightSlider->setMinimum(0);
    ui->brightSlider->setMaximum(200);
    ui->brightSlider->setValue(100);
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}

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
            msgBox.setText(tr("No such image!"));
            msgBox.exec();
            ui->label->clear();
        }
        else{
            setDisplayImage(image);
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
    else if(image.type()==CV_8UC1){
        QMessageBox msgBox;
        msgBox.setText(tr("Image is already in gray scale!"));
        msgBox.exec();
    }
    else{
        cvtColor(image,grayImage,COLOR_BGR2GRAY);
        setDisplayImage(grayImage);
    }
}

void MyMainWindow::on_actionRestore_R_triggered()
{
    if(!image.data){
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    }
    else{
        setDisplayImage(image);
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
        ui->label->setPixmap(QPixmap::fromImage(*Mat2QImage(*nextImageToShow)));
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
        ui->label->setPixmap(QPixmap::fromImage(*Mat2QImage(*lastImageToShow)));
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

//event dealers
void MyMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()){
        qDebug()<<event->mimeData()->urls();
        event->acceptProposedAction();
    }
    else
        event->ignore();
}

void MyMainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData=event->mimeData();
    if(mimeData->hasUrls()){
        QList<QUrl> urlList=mimeData->urls();
        //let the first url to be the local path
        QString fileName=urlList.at(0).toLocalFile();
        qDebug()<<"fileName:"<<fileName;
        if(!fileName.isEmpty()){
            QTextCodec *code = QTextCodec::codecForName("gb18030");
            string name = code->fromUnicode(fileName).data();
            Mat tempImage=imread(name);
            if(tempImage.empty()){
                cout<<"Invalid image!"<<endl;
                return;
            }
            tempImage.copyTo(image);
            setDisplayImage(image);
            QString logInfo=fileName+" opened.";
            htmlLog(tr("maroon"),logInfo,tr("times"));
        }
    }
}

void MyMainWindow::resizeEvent(QResizeEvent *)
{
    qDebug()<<this->frameGeometry();
}

void MyMainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        QCursor cursor;
        cursor.setShape(Qt::ClosedHandCursor);
        Mat *preImage=getPresentMatrix();
        if(preImage==NULL)
            return;
        QApplication::setOverrideCursor(cursor);
        QPoint clickPoint=event->pos()-ui->label->pos()-ui->centralWidget->pos();
        qDebug()<<"Click at "<<clickPoint;
        uchar *pixels=pixelColor(*preImage,clickPoint.ry(),clickPoint.rx());
        if(pixels==NULL){
            ui->textBrowser->clear();
            ui->textBrowser->setTextColor(Qt::red);
            //ui->textBrowser->setText(tr("Out of image range"));
            return;
        }
        QString message="Pos:(";
        message+=QString::number(clickPoint.rx())+" "+QString::number(clickPoint.ry());
        message+=") Color:(";
        qDebug()<<message;
        for(int i=0;i<preImage->channels();i++){
            message+=QString::number(pixels[i]);
            if(i<preImage->channels()-1)
                message+=" ";
        }
        message+=")";
        qDebug()<<message;
        ui->textBrowser->clear();
        ui->textBrowser->setTextColor(Qt::black);
        ui->textBrowser->setText(message);
        free(pixels);
    }
    else if(event->button()==Qt::RightButton){
        QCursor cursor(QPixmap("image/stock_print.jpg"));
        QApplication::setOverrideCursor(cursor);
    }
}

void MyMainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QApplication::restoreOverrideCursor();
}


void MyMainWindow::on_actionDetect_triggered()
{
    setDetectionDisplay(true);
}

void MyMainWindow::on_startDetectButton_clicked()
{
    Mat *presentImage=getPresentMatrix();
    if(presentImage==NULL){
        QMessageBox msgBox;
        msgBox.setText(tr("image data is null"));
        msgBox.exec();
        return;
    }
    Mat img=presentImage->clone();
    Mat featureImage;
    int threshold=ui->detectThresholdBox->value();
    QString method=ui->detectSelectionBox->currentText();
    setFeatureMatrix(img,featureImage,threshold,method);
    setDisplayImage(featureImage,false);
}

void MyMainWindow::on_endDetectButton_clicked()
{
    setDetectionDisplay(false);
}

void MyMainWindow::on_detectSelectionBox_currentTextChanged(const QString &arg1)
{
    qDebug()<<"Current Text: "<<arg1;
    if(arg1=="Surf Detector"){
        ui->detectThresholdBox->setMinimum(2);
        ui->detectThresholdBox->setMaximum(9);
    }
    else if(arg1=="Fast Detector"){
        ui->detectThresholdBox->setMinimum(0);
        ui->detectThresholdBox->setMaximum(255);
    }
    else if(arg1=="Orb Detector"){//adjust nFeature
        ui->detectThresholdBox->setMinimum(10);
        ui->detectThresholdBox->setMaximum(500);
        ui->detectThresholdBox->setSingleStep(10);
    }
    else
        assert(0);
}

void MyMainWindow::on_actionContrast_Adjust_A_triggered()
{
    setContraBrightDisplay(true);
}

void MyMainWindow::on_brightSlider_sliderMoved(int position)
{
    Mat *presentImage=getPresentMatrix();
    if(presentImage==NULL){
        return;
    }
    Mat img=presentImage->clone();
    Mat dst;
    int contra=ui->contrastSlider->value();
    setContraBrightMatrix(img,dst,contra,position);
    setDisplayImage(dst,false);
}


void MyMainWindow::on_contrastSlider_sliderMoved(int position)
{
    Mat *presentImage=getPresentMatrix();
    if(presentImage==NULL){
        return;
    }
    Mat img=presentImage->clone();
    Mat dst;
    int bright=ui->brightSlider->value();
    setContraBrightMatrix(img,dst,position,bright);
    setDisplayImage(dst,false);
}

void MyMainWindow::on_brightContrastEndButton_clicked()
{
    setContraBrightDisplay(false);
}

void MyMainWindow::on_actionHistogram_Equalization_E_triggered()
{
    Mat *preImage=getPresentMatrix();
    if(preImage==NULL){
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    }
    else{
        Mat tempImage=preImage->clone();
        Mat *dst=new Mat();
        setEqualizedMatrix(tempImage,*dst);
        addInProcessList(*dst);
        setDisplayImage(*dst);
    }
}
