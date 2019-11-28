#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "work.h"


MainWindow::MainWindow(QWidget *parent) :

    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);  //进度条初


    connect(ui->pushButton_openFile, SIGNAL(clicked()), this, SLOT(onPushButton()));
    connect(ui->pushButton_openDir, SIGNAL(clicked()), this, SLOT(onPushButton()));

    //创建自定义线程类对象
     Worker*worker_Thread = new Worker();
    //创建子线程
    m_objThread = new QThread(this);

    //Worker worker;
    connect(this, &MainWindow::wantToEmitDirPath, this, &MainWindow::getWantToEmitDirPath);

    connect(this, SIGNAL(emitDirPath(QString)), worker_Thread, SLOT(getDirPath(QString)));

    connect(worker_Thread, SIGNAL(convertTxt(QStringList, int)), this, SLOT(printDirTxtToWindow(QStringList,int)));


   //当耗时函数执行完毕，发出完成信号时，删除worker实例
   connect(worker_Thread, SIGNAL(longTimeWorkFinished()), worker_Thread, SLOT(deleteLater()));
   //当worker对象实例销毁时，退出线程
   connect(worker_Thread, SIGNAL(destroyed(QObject*)), m_objThread, SLOT(quit()));
   //当线程结束时，销毁线程对象实例
   connect(m_objThread, SIGNAL(finished()), m_objThread, SLOT(deleteLater()));
   //移动worker_thread对象实例到线程中，
   worker_Thread->moveToThread(m_objThread);
    //启动
   m_objThread->start();

   //m_objThread->quit();
   //m_objThread->wait();

}

MainWindow::~MainWindow()
{

    delete ui;
    m_objThread->quit();
    m_objThread->wait(1000);

    delete m_objThread;
    //delete worker_Thread;
}





void MainWindow::getWantToEmitDirPath()
{
    selectDir();
    emit emitDirPath(dirPath);
}



//选择文件夹函数
void MainWindow::selectDir()
{
    if(false == openSelectDir())
        return;
    if(dirPath == "")
        return;
    ui->lineEdit->setText(dirPath);
    //emit emitDirPath(dirPath);
}


void MainWindow:: printDirTxtToWindow(QStringList txt,  int tstep) //将文件夹里的txt内容打印出来
{

    QStringList txtcontentList =txt;
    curStep = 0;
    tolStep = tstep;
    ui->progressBar->setRange(0, tolStep); //进度条范围
    ui->progressBar->setMaximum(tolStep); //进度条最大值
    ui->progressBar->setMinimum(0); //进度条最小值
    ui->progressBar->setOrientation(Qt::Horizontal); //进度条方向
    //ui->progressBar->setValue(50);  //进度条初始
    std::cout<<"setInit!!!"<<std::endl;
    for(QStringList::Iterator it = txtcontentList.begin(); it != txtcontentList.end(); ++it)
    {
        ui->textEdit->append(*it);
        curStep++;
        onProcess();
        QEventLoop eventloop;
        QTimer::singleShot(500, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
}


//进度条
void MainWindow::onProcess()
{
   ui->progressBar->setValue(curStep);
   std::cout<<"progressBar"<<std::endl;
}

//不同按钮不同功能
void MainWindow::onPushButton()
{
    QPushButton *btn=qobject_cast<QPushButton*>(sender()); //得到当前操作的按钮
    if(btn == ui->pushButton_openFile)
    {
        selectFile();
    }
    if(btn == ui->pushButton_openDir)
    {
        emit wantToEmitDirPath();
    }
}




//选择文件函数
void MainWindow::selectFile()
{
    if(false == openSelectFile())
        return;
    if(filePath == "")
        return;
    ui->lineEdit->setText(filePath);
    showTxtToWindow();
}



//弹出选择文件夹的对话框
bool MainWindow::openSelectDir()
{
    dirPath = QFileDialog::getExistingDirectory(NULL,QString("Please select DIR"),"",QFileDialog::ShowDirsOnly);
    //QString thDirPath;
    if(dirPath == "")
    {
        QMessageBox::information(this,QString("Warning"),QString("quit selete!"),"Ok");
        return false;
    }
     return true;

}

//弹出选择文件的对话框
bool MainWindow::openSelectFile()
{
    //getOpenFileName(), 参数一是默认，参数二是对话框名字，参数四是
     filePath = QFileDialog::getOpenFileName(NULL, QString("Please select a file"),"",QObject::tr("txt(*.txt)"));
     if(filePath == "")
     {
         QMessageBox::information(this,QString("Warning"),QString("quit selete!"),"Ok");
         return false;
     }
      return true;
}


//显示文件内容
void MainWindow::showTxtToWindow()
{

    QString filename = filePath;
     if(false == filename.isEmpty() )
     {
         //true -->if string is null
         QFile *file = new QFile;

        file->setFileName(filename);
         bool is_OK = file->open(QIODevice::ReadOnly);
         if(is_OK == true)
         {
             QTextStream displayFile(file);
             Q_UNUSED(displayFile);
             displayFile.setCodec("UTF-8");  //设置解码器为 UTF-8解码方式
             ui->textEdit->setText(displayFile.readAll());
             file->close();
             tolStep = 1;
             curStep = 1;
             ui->progressBar->setRange(0, tolStep); //进度条范围
             onProcess();
             delete file;
         }
         else
         {
             QMessageBox::information(this, "错误信息","打开文件:", file->errorString());
             return;
         }
     }
     return;
}



 Worker::Worker(QObject *parent): QObject(parent)
 {
 }


 //获取路径
 void Worker::getDirPath(QString _dirPath)
 {
     dirPath = _dirPath;
     searchDirFile();

 }


QFileInfoList Worker:: getFileList(QString dir_path) //遍历文件夹
{
    QDir dir(dir_path);
    //列出dir(dir_path)目录下所有文件信息，存储到file_list容器里
    QFileInfoList file_list = dir.entryInfoList(QDir::Files);

    return file_list;
}

void  Worker::selectTxt(QFileInfoList file_list) //筛选Dir txt
{
     //这个fileList 用来存储文档的名字："1.txt"->"2.txt"->"3.txt"
    //遍历文件夹获取文件信息，存到容器file_info_list;
    //QFileInfoList file_info_list = getFileList(dirPath);
    //用foreach遍历这个容器，并进行过滤筛选，将结果存入fileList容器里
    //foreach(var, container),遍历变量container,将结果存在var里
    //QFileInfo 提供与系统无关的文件信息


    QString filter; //后缀名字
    tolStep = 0;
    foreach(QFileInfo fileInfo, file_list)
    {
        //suffix  == 后缀, 获取后缀
        filter = fileInfo.suffix();
        if(filter != "txt")
            continue;
        fileList.append(fileInfo.fileName());
    }
   tolStep = fileList.size();
}

void Worker:: openReadTxt() //打开并读取文件夹下所有txt
{
    QString dirPATH = dirPath;
    if(false == dirPATH.isEmpty())
    {
        for(QStringList::Iterator it = fileList.begin(); it != fileList.end(); ++it)
        {
            QFile* filePtr = new QFile;
            QDir::setCurrent(dirPath);
            filePtr->setFileName(*it);
            //打开一个大概只耗时0.001s左右
            bool is_OK = filePtr->open(QIODevice::ReadOnly);

            if(true == is_OK)
            {
                QTextStream showFile(filePtr);
                showFile.setCodec("UTF-8");  //设置解码器为 UTF-8解码方式

                //读取一个大概也是0.001s甚至更少
                QString fileText = showFile.readAll();
                txtcontentList.append(fileText);
                filePtr->close();
                delete filePtr;
             }
        }
    }
   emit convertTxt(txtcontentList,  tolStep);
}

void  Worker::searchDirFile()
{


    //先遍历文件夹
    QFileInfoList file_list = getFileList(dirPath);
    //再筛选txt
   selectTxt(file_list);
   //读取并存取内容
   openReadTxt();

}





