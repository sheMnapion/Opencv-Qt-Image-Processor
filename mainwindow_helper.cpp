#include "mainwindow_helper.h"
#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include <QDateTime>
#include <opencvhelper.h>

// some help functions
// include mat-qimage transformation, html log helpers and etc

void MyMainWindow::setDisplayImage(Mat &img,bool newImage)
{
    if(img.cols>ui->label->size().height()&&img.rows>ui->label->size().width())
        cv::resize(img,img,Size(ui->label->size().width(),ui->label->size().height()));
    _presentSize=ui->label->size();
    if(newImage)
        addInProcessList(img);
    QImage *imgo=Mat2QImage(img);
    //store the picture
    ui->label->clear();
    ui->label->setPixmap(QPixmap::fromImage(*imgo));
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

// end of help functions
