/*
#ifndef WORK_H
#define WORK_H
#include <QObject>
#include "mainwindow.h"

class Worker:public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject* parent = 0);
    //MainWindow* p;

signals:
    //子线程处理完毕后要发给主线程的信号
    void longTimeWorkFinished();


public slots:
    void longTimeWork(); //耗时操作，槽函数



};
#endif // WORK_H
*/
