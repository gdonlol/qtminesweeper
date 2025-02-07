#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //basic setup
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.setFixedSize(1000, 533);
    mainWindow.setStyleSheet("background-color: #313338; color: #FFFFFF;");
    mainWindow.show();
    return app.exec();
}
