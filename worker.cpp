#include "worker.h"
#include <QThread>
#include "mainwindow.h"
Worker::Worker(QObject *parent): QObject(parent)
{

}

//槽函数，通过主线程的信号发送
void Worker::longTimeWork()
{
    MainWindow mainwinObj;

    mainwinObj.selectTxt();
}

