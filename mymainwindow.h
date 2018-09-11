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

#define PIC_SHOW(PIC,WIN_NAME,WINDOW_TYPE) namedWindow(WIN_NAME,WINDOW_TYPE); \
    imshow(WIN_NAME,PIC)
#define AUTO_SHOW(PIC,WIN_NAME) PIC_SHOW(PIC,WIN_NAME,WINDOW_AUTOSIZE)
#define NORMAL_SHOW(PIC,WIN_NAME) PIC_SHOW(PIC,WIN_NAME,WINDOW_NORMAL)

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

    void on_actionDetect_triggered();

    void on_startDetectButton_clicked();

    void on_endDetectButton_clicked();

    void on_detectSelectionBox_currentTextChanged(const QString &arg1);

    void on_actionContrast_Adjust_A_triggered();

    void on_brightSlider_sliderMoved(int position);

    void on_contrastSlider_sliderMoved(int position);

    void on_brightContrastEndButton_clicked();

    void on_actionHistogram_Equalization_E_triggered();

    void on_actionRetina_Model_R_triggered();

private:
    Ui::MyMainWindow *ui;
    vector<Mat *> _processList;
    int _processPointer;
    QSize _presentSize;
    void addInProcessList(Mat &);
    Mat* getPresentMatrix();
    void htmlLog(QString &color, QString &info, QString &font, bool addTime=true);
    void setDisplayImage(Mat &img,bool newImage=true);
    void setDetectionDisplay(bool);
    void setContraBrightDisplay(bool);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MYMAINWINDOW_H
