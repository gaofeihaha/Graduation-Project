#include "thread.h"
#include <QTcpServer>

Thread::Thread(QObject *parent) : QThread(parent)
{

}


void Thread::run()
{
    //复杂运算



    emit isDone();

}



