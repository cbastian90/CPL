#-------------------------------------------------
#
# Project created by QtCreator 2013-11-28T09:48:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlateDetection
TEMPLATE = app

unix{
    CONFIG += link_pkgconfig
    PKGCONFIG +=opencv
}
win32{
INCLUDEPATH +=  C:\OpenCV248bin\install\include \
                C:\OpenCV248bin\install\include\opencv \
                C:\OpenCV248bin\install\include\opencv2

LIBS += -LC:\OpenCV248bin\install\x64\mingw\lib \
        -lopencv_calib3d248.dll \
        -lopencv_contrib248.dll \
        -lopencv_core248.dll \
        -lopencv_features2d248.dll \
        -lopencv_flann248.dll \
        -lopencv_gpu248.dll \
        -lopencv_highgui248.dll \
        -lopencv_imgproc248.dll \
        -lopencv_legacy248.dll \
        -lopencv_ml248.dll \
        -lopencv_nonfree248.dll \
        -lopencv_objdetect248.dll \
        -lopencv_ocl248.dll \
        -lopencv_photo248.dll \
        -lopencv_stitching248.dll \
        -lopencv_superres248.dll \
        -lopencv_ts248 \
        -lopencv_video248.dll \
        -lopencv_videostab248.dll
}

SOURCES += main.cpp\
        platedetection.cpp \
    segmentation.cpp \
    hog.cpp

HEADERS  += platedetection.h \
    segmentation.h \
    hog.h

FORMS    += platedetection.ui
