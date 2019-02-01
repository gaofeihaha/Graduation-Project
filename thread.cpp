#include "thread.h"
#include <QTcpServer>

//extern Data_Init Data_Rec;

Thread::Thread(QObject *parent) : QThread(parent)
{

}


void Thread::run()
{
    //复杂运算

 Data_Rec[2]=3;

    emit isDone();

}



