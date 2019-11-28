#ifndef WORKER_H
#define WORKER_H
#include <QObject>
#include "mainwindow.h"

class Work:public QObject
{
    Q_OBJECT
public:
    explicit Work(QObject* parent = 0);

public slots:
    void longTimeWork(); //耗时操作，槽函数

};
#endif // WORKER_H
