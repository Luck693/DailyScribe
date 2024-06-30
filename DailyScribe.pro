    QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    diaries.cpp \
    diarydialog.cpp \
    form.cpp \
    global.cpp \
    inkwidget.cpp \
    jottings.cpp \
    main.cpp \
    page_login.cpp \
    recollections.cpp \
    regis.cpp \
    settings.cpp


HEADERS += \
    diaries.h \
    diarydialog.h \
    form.h \
    global.h \
    inkwidget.h \
    jottings.h \
    page_login.h \
    recollections.h \
    regis.h \
    settings.h


FORMS += \
    diaries.ui \
    diarydialog.ui \
    form.ui \
    inkwidget.ui \
    jottings.ui \
    page_login.ui \
    recollections.ui \
    regis.ui \
    settings.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc

DISTFILES += \
    ../../../background2.jpg \
    pic/background2.jpg \
    res/pic/background.jpg \
    res/pic/background2.jpg \
    res/pic/green.jpg \
    res/pic/icon_login.png \
    res/pic/pink.jpg \
    res/pic/purple.jpg \
    res/pic/yellow_green.jpg
