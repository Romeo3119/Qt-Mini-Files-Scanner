
#pragma execution_character_set("utf-8")

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QDir>
#include <QDebug>
#include <vector>
#include <string>
#include <iostream>
#include <QThread>
#include <QTimer>
#include <Windows.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  QString  dirPath; //文件夹路径

   bool openSelectDir(); //弹出选择文件夹对话框


signals:
   void emitDirPath(QString dirPath);
   void toSearchDirFile();
   void wantToEmitDirPath();

    //声明槽函数
public slots:

   void getWantToEmitDirPath();


    void onPushButton();
    void showTxtToWindow();//显示一个txt文件内容
    void printDirTxtToWindow(QStringList txt, int tstep); //将文件夹里的txt内容打印出来
    void selectFile();  //选择文件
    void selectDir();  //选择文件夹
    void onProcess(); //进度条


private:
    Ui::MainWindow *ui;
    QThread* m_objThread;
   // Worker::Worker* worker_Thread;
    QString filePath; //文件路径

    int fileCount; //文件夹文件数量

    bool openSelectFile(); //弹出选择文件的对话框

    int  curStep;
    int  tolStep;



};



class Worker:public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject* parent = 0);
    QFileInfoList  getFileList(QString dir_path); //遍历文件夹
    void  selectTxt(QFileInfoList file_list); //筛选Dir txt
    void openReadTxt(); //打开并读取文件夹下所有txt
    void searchDirFile();


    QStringList  fileList; //存储目录里txt的名字的list
    QStringList txtcontentList; //目录下txt内容的list

private:
    QString dirPath;
     //int  curStep;
     int  tolStep;



signals:

    //发送信号

    void convertTxt(QStringList txt, int tstep);


public slots:

    void getDirPath(QString _dirPath);


};

#endif // MAINWINDOW_H
