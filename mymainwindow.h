#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <set>
#include <vector>

using namespace cv;
using namespace std;

namespace Ui {
class MyMainWindow;
}

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyMainWindow(QWidget *parent = 0);
    ~MyMainWindow();

private slots:
    void on_actionExit_E_triggered();

    void on_actionLoad_Picture_L_triggered();

    void on_actionGray_Scale_G_triggered();

    void on_actionRestore_R_triggered();

    void on_actionNext_N_triggered();

    void on_actionCancel_C_triggered();

    void on_actionSave_Picture_S_triggered();

    void on_actionClear_C_triggered();

private:
    Ui::MyMainWindow *ui;
    bool notGrayed;
    vector<Mat *> _processList;
    int _processPointer;
    void addInProcessList(Mat &);
    Mat* getPresentMatrix();
    void htmlLog(QString &color, QString &info, QString &font, bool addTime);
};

#endif // MYMAINWINDOW_H
