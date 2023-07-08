#include "mainwindow.h"
#include <QApplication>
#include "server.h"
#include "gamescreen.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Server sv;
    sv.startServer();

    MainWindow w;
    w.show();

    //GameScreen g;
    //g.show();
    return a.exec();
}
