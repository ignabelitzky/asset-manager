QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/core/AppController.cpp \
    src/core/DBMigrationService.cpp \
    src/core/DatabaseManager.cpp \
    src/core/MainWindow.cpp

HEADERS += \
    src/core/AppController.h \
    src/core/DBMigrationService.h \
    src/core/DatabaseManager.h \
    src/core/MainWindow.h

FORMS += \
    src/ui/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
