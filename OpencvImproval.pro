#-------------------------------------------------
#
# Project created by QtCreator 2018-09-05T20:30:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpencvImproval
TEMPLATE = app


SOURCES += main.cpp\
        mymainwindow.cpp \
    opencvhelper.cpp

HEADERS  += mymainwindow.h \
    opencvhelper.h

FORMS    += mymainwindow.ui
INCLUDEPATH += F:/OpenCV/opencv/build/include

CONFIG(debug, debug|release): {
LIBS += -LF:/OpenCV/opencv/build/x86/vc12/lib \
-lopencv_core2413d \
-lopencv_imgproc2413d \
-lopencv_highgui2413d \
-lopencv_ml2413d \
-lopencv_video2413d \
-lopencv_features2d2413d \
-lopencv_calib3d2413d \
-lopencv_objdetect2413d \
-lopencv_nonfree2413d \
-lopencv_contrib2413d \
-lopencv_legacy2413d \
-lopencv_flann2413d
} else:CONFIG(release, debug|release): {
LIBS += -LF:/OpenCV/opencv/build/x86/vc12/lib \
-lopencv_core2413 \
-lopencv_imgproc2413 \
-lopencv_highgui2413 \
-lopencv_ml2413 \
-lopencv_video2413 \
-lopencv_nonfree2413 \
-lopencv_features2d2413 \
-lopencv_calib3d2413 \
-lopencv_objdetect2413 \
-lopencv_contrib2413 \
-lopencv_legacy2413 \
-lopencv_flann2413
}

RC_ICONS = trial.ico

RESOURCES += \
    myimages.qrc
