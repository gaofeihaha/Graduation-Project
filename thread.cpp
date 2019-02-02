#include "thread.h"
#include <QTcpServer>

//extern Data_Init Data_Rec;

Thread::Thread(QObject *parent) : QThread(parent)
{

}

QStringList Data_Rec_list;
void Thread::run()
{

    //复杂运算
    Data_Rec_list=Str_Rec.split("#");

    emit isDone();  //链接槽函数：deal_isDone（）

}



