#ifndef THREAD_H
#define THREAD_H

#include <QThread>

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = nullptr);



signals:
    void isDone();
protected:
    void run();//QThread的线程处理函数，不能直接调用，要用start间接调用

public slots:

};


extern int Data_Rec[5];


#endif // THREAD_H


//创建object基类，随后对头文件，public 以及CPP文件里面的父类进行修改即可
