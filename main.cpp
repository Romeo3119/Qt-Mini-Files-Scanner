#include "mainwindow.h"
//#include "work.h"
#include <QApplication>
#include <QTextCodec>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle(QObject::tr("Mini FileTool"));
    w.show();

    return a.exec();
}
