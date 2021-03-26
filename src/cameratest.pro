QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    axistag.cpp \
    calibrate.cpp \
    calibrationgridparameters.cpp \
    cameracalibrationimageprocessor.cpp \
    characteristics.cpp \
    checkerboardtest.cpp \
    customlable.cpp \
    fpschart.cpp \
    lmsolver.cpp \
    main.cpp \
    mainwindow.cpp \
    optimizationresult.cpp \
    udp.cpp \
    qcustomplot.cpp \
    lmmin.cpp

HEADERS += \
    axistag.h \
    calibrate.h \
    calibrationgridparameters.h \
    cameracalibrationimageprocessor.h \
    characteristics.h \
    checkerboardtest.h \
    customlable.h \
    fpschart.h \
    lmsolver.h \
    mainwindow.h \
    optimizationresult.h \
    solverstatus.h \
    udp.h \
    qcustomplot.h \
    lmmin.h \
    lmstruct.h

FORMS += \
    fpschart.ui \
    mainwindow.ui

INCLUDEPATH += D:\Software\OpenCV\opencv\build\include
INCLUDEPATH += D:\repos\Qt_Camera_Test\include

LIBS += D:\Software\OpenCV\opencv\build\bin\libopencv_core451.dll
LIBS += D:\Software\OpenCV\opencv\build\bin\libopencv_highgui451.dll
LIBS += D:\Software\OpenCV\opencv\build\bin\libopencv_imgcodecs451.dll
LIBS += D:\Software\OpenCV\opencv\build\bin\libopencv_imgproc451.dll
LIBS += D:\Software\OpenCV\opencv\build\bin\libopencv_features2d451.dll
LIBS += D:\Software\OpenCV\opencv\build\bin\libopencv_calib3d451.dll
#LIBS += D:\Vives\Stage\lmfit\lmfit32.dll

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
