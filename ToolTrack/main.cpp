#include "src/core/MainWindow.h"
#include "src/core/AppController.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppController controller;

    if (!controller.initialize())
        return -1;

    /*
    MainWindow w;
    w.show();
    */

    int exitCode = a.exec();

    controller.shutdown();
    return exitCode;
}
