
/*
#include "work.h"
#include <QThread>
#include <QDebug>

Worker::Worker(QObject *parent): QObject(parent)
{

}

//槽函数，通过主线程的信号发送

void Worker::longTimeWork()
{

   //mw.searchDirFile();
   //p->searchDirFile();
    std::cout<<"WORK!!!!!!!!!!!!!!!!!!!"<<std::endl;
    emit longTimeWorkFinished();
}

*/
