QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ImageWidget.cpp \
    KNTree.cpp \
    PointEdge.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ImageWidget.h \
    KNTree.h \
    PointEdge.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

#opencv
INCLUDEPATH+=D:\opencv\build\include

LIBS += D:\opencv\openbuild\bin\libopencv_core440.dll
LIBS += D:\opencv\openbuild\bin\libopencv_highgui440.dll
LIBS += D:\opencv\openbuild\bin\libopencv_imgcodecs440.dll
LIBS += D:/opencv/openbuild/bin/libopencv_imgproc440.dll
LIBS += D:/opencv/openbuild/bin/libopencv_features2d440.dll
LIBS += D:\opencv\openbuild\bin\libopencv_calib3d440.dll

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
