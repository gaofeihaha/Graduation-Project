#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>
#include <QTimer>
#include "thread.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QtCharts>



namespace Ui {
class Monitor;
}

class Monitor : public QWidget
{
    Q_OBJECT

public:
    explicit Monitor(QWidget *parent = 0);
    ~Monitor();

//    void dealTimerout();//定时器槽函数

    void deal_isDone();
    void stop_Thread();

    void TCPServe_init();//TCPserve初始化
    void Uart_init();//串口初始化
    QString GetLocalIP();

    QTimer *pTimer1;

    void creatChart();

    void Thread_start();

    void Home_page(int para);

protected:
    //重写绘图事件
    //如果在窗口绘图，必须在绘图事件里实现
    //绘图事件内部自动调用，窗口需要重绘的时候（状态改变）
    void paintEvent(QPaintEvent *event);
private slots:
    void on_pushButton_home_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_9_clicked();

    void onNewConnection();
    void onClientDisconnect();
    void onSocketReadready();

    void on_pushButton_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_tcpsend_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_searchuart_clicked();

    void on_pushButton_openuart_clicked();

    void on_pushButton_closeuart_clicked();

    void on_pushButton_uartsend_clicked();

    void on_pushButton_cleuartrec_clicked();

    void on_pushButton_tcpcle_clicked();
    void serialPort_readyRead();

    void on_pushButton_drawing_clicked();

    void on_pushButton_drawing_stop_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::Monitor *ui;
    QTimer *myTimer;//声明变量
    Thread *myThread;//线程对象

    QTcpServer *tcpSever;
    QTcpSocket *tcpSocket;
    QSerialPort serial;

    QList<QPointF> m_data;
    int m_count;
    qreal m_maxY;
    qreal m_minY;


    QChart *Chart_1;
    QLineSeries *series0;
signals:
        void Start_Drawing();
        void Stop_Drawing();
        void Data_Rec_Over();
};



#endif // MONITOR_H
