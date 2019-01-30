#include "monitor.h"
#include "ui_monitor.h"
#include "thread.h"
#include <QPainter>
#include <QPixmap>
#include <QMessageBox>
#include <QStackedWidget>
#include <QHostInfo>
#include <QHostAddress>
#include <QTimer>
#include <math.h>
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE

Monitor::Monitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Monitor)
{
    ui->setupUi(this);
    resize(1366,768);


//    绘制标题图片
    QPixmap *pixmap = new QPixmap(":/picture/image/capital.png");
    pixmap->scaled(ui->label_capital->size(), Qt::KeepAspectRatio);
    ui->label_capital->setScaledContents(true);
    ui->label_capital->setPixmap(*pixmap);

    myTimer = new QTimer(this);
    connect(myTimer,&QTimer::timeout,this,&Monitor::dealTimerout);

    myThread = new Thread(this);
    connect(myThread,&Thread::isDone,this,&Monitor::deal_isDone);//使用信号开启线程
    connect(this,&Monitor::destroyed,this,&Monitor::stop_Thread);//点击窗口关闭按钮时触发关闭信号

    Uart_init();
    creatChart();

}

Monitor::~Monitor()
{
    delete ui;
}


//绘制背景图片
void Monitor::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    //绘图操作
    //p.drawxxxx;
    //画背景图
    //p.drawPixmap(0,0,width(),height(),QPixmap("../image/wanshengjie.jpg"));
    //背景图片放到源文件文件夹同一父文件夹,与下一句相同作用
    p.drawPixmap(rect(),QPixmap("../picture/b_linux.png"));
}

//按键槽
void Monitor::on_pushButton_home_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void Monitor::on_pushButton_2_clicked()//WIFI按钮
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Monitor::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Monitor::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void Monitor::on_pushButton_clicked()//开始监听按钮
{
    QString IP = ui->lineEdit_ip->text();
    quint16 PORT = ui->lineEdit_port->text().toUInt();
    QHostAddress addr(IP);
    tcpSever->listen(addr,PORT);//开始监听
    ui->textBro_tcpreceive->append("**开始监听"+tcpSever->serverAddress().toString()+":"
                                   +QString::number(tcpSever->serverPort()));
    ui->pushButton->setEnabled(false);

}
void Monitor::on_pushButton_stop_clicked()//停止监听按钮
{
    ui->textBro_tcpreceive->append("**Socket关闭");
    ui->pushButton->setEnabled(true);
    tcpSocket->close();
    tcpSever->close();
}

void Monitor::on_pushButton_9_clicked()
{
    myThread->start();//启动线程
}
void Monitor::on_pushButton_8_clicked()//状态更新按钮
{
    TCPServe_init();
}
void Monitor::on_pushButton_searchuart_clicked()//搜索串口按钮
{
    ui->comboBox_port->clear();
    //通过QSerialPortInfo查找可用串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox_port->addItem(info.portName());
    }
}



//信号槽
//定时器槽函数
void Monitor::dealTimerout()
{

}

void Monitor::deal_isDone()
{
    QMessageBox::about(this,"提示","线程已开启");
}
void Monitor::stop_Thread()
{
    myThread->quit();//停止线程
    myThread->wait();//等待线程处理完手头工作
}

//WIFI页面
void Monitor::TCPServe_init()
{
    QString localIP=GetLocalIP();
    ui->lineEdit_ip->setText(localIP);

    tcpSever = new QTcpServer(this);
    connect(tcpSever,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
}

QString Monitor::GetLocalIP()//获取本机IP
{
    QString hostName = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    QString localIP = "";
    QList<QHostAddress> addlist = hostInfo.addresses();

    if(!addlist.isEmpty())//如果列表不为空
    {
        for (int i=0;i<addlist.count();i++)
        {
            QHostAddress aHost = addlist.at(i);
            if (QAbstractSocket::IPv4Protocol==aHost.protocol())
            {localIP = aHost.toString();break;}
        }

    }
    return localIP;
}
void Monitor::onNewConnection()
{
    tcpSocket = tcpSever->nextPendingConnection();//获取socket
    ui->textBro_tcpreceive->append("**"+tcpSocket->peerAddress().toString()+"连接成功");
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(onClientDisconnect()));//若断开连接
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(onSocketReadready()));//若接收到消息

}
void Monitor::onClientDisconnect()
{
    ui->textBro_tcpreceive->append("**Socket连接断开");
}
void Monitor::onSocketReadready()
{
    QByteArray array = tcpSocket->readAll();//从通信套接字中读取内容
    ui->textBro_tcpreceive->append(array);
    QString str = array;
    QString judge = str.section('#',1,1);
    qDebug()<<judge;
}


void Monitor::on_pushButton_tcpsend_clicked()
{
    if(tcpSocket==NULL)
       {
           return;
       }
       QString str = ui->textEdit_tcpsend->toPlainText();//获取编辑区内容
       tcpSocket->write(str.toUtf8().data());//给对方发送数据，使用套接字时tcpsocket
}
void Monitor::on_pushButton_tcpcle_clicked()
{
    ui->textBro_tcpreceive->clear();
}



//UART部分
void Monitor::Uart_init()
{
    //uart部分
    ui->comboBox_bode->addItem("115200");
    ui->comboBox_bode->addItem("9600");
    ui->comboBox_check->addItem("1");
    ui->comboBox_check->addItem("2");
    ui->comboBox_datanum->addItem("8");
    ui->comboBox_datanum->addItem("9");
    ui->comboBox_stopbit->addItem("0");
    ui->pushButton_uartsend->setEnabled(false);

    //连接信号和槽
    QObject::connect(&serial, &QSerialPort::readyRead, this, &Monitor::serialPort_readyRead);

}

void Monitor::on_pushButton_openuart_clicked()//打开串口
{
    //设置串口名
    serial.setPortName(ui->comboBox_port->currentText());
    //设置波特率
    serial.setBaudRate(ui->comboBox_bode->currentText().toInt());
    //设置数据位数
    switch(ui->comboBox_datanum->currentIndex())
    {
    case 8: serial.setDataBits(QSerialPort::Data8); break;
    default: break;
    }
    //设置奇偶校验
    switch(ui->comboBox_check->currentIndex())
    {
    case 0: serial.setParity(QSerialPort::NoParity); break;
    default: break;
    }
    //设置停止位
    switch(ui->comboBox_stopbit->currentIndex())
    {
    case 1: serial.setStopBits(QSerialPort::OneStop); break;
    case 2: serial.setStopBits(QSerialPort::TwoStop); break;
    default: break;
    }
    //设置流控制
    serial.setFlowControl(QSerialPort::NoFlowControl);

    //打开串口
    if(!serial.open(QIODevice::ReadWrite))
    {
        QMessageBox::about(NULL, "提示", "无法打开串口！");
        return;
    }
    ui->comboBox_port->setEnabled(false);
    ui->comboBox_datanum->setEnabled(false);
    ui->comboBox_bode->setEnabled(false);
    ui->comboBox_check->setEnabled(false);
    ui->comboBox_stopbit->setEnabled(false);

    //发送按键使能
    ui->pushButton_uartsend->setEnabled(true);

}

void Monitor::on_pushButton_closeuart_clicked()
{
    //关闭串口
    serial.close();
    ui->comboBox_port->setEnabled(true);
    ui->comboBox_datanum->setEnabled(true);
    ui->comboBox_bode->setEnabled(true);
    ui->comboBox_check->setEnabled(true);
    ui->comboBox_stopbit->setEnabled(true);

    //发送按键使能
    ui->pushButton_uartsend->setEnabled(false);
}

void Monitor::serialPort_readyRead()
{
    //从接收缓冲区中读取数据
    QByteArray buffer = serial.readAll();
    //从界面中读取以前收到的数据
    ui->textBro_uartreceive->append(QString(buffer).toUtf8());

}

void Monitor::on_pushButton_uartsend_clicked()
{
    QByteArray data = ui->textEdit_uartsend->toPlainText().toUtf8();
    serial.write(data);
}

void Monitor::on_pushButton_cleuartrec_clicked()
{
    ui->textBro_uartreceive->clear();
}


//第三页 图表部分
void Monitor::creatChart()  //创建波形图
{
    QTimer *pTimer1 = new QTimer(this);//using in drawing
    QLineSeries *series_vol = new QLineSeries();
    QLineSeries *series_cur = new QLineSeries();
    series_vol->setName("电压波形");
    series_cur->setName("电流波形");

    QChart *Chart_vol = new QChart();
    Chart_vol->addSeries(series_vol);
    Chart_vol->createDefaultAxes();
    Chart_vol->setTheme(QChart::ChartThemeDark);
    ui->ChartView->setChart(Chart_vol);

    QChart *Chart_cur = new QChart();
    Chart_cur->addSeries(series_cur);
    Chart_cur->createDefaultAxes();
    Chart_cur->setTheme(QChart::ChartThemeDark);
    ui->ChartView_1->setChart(Chart_cur);


    connect(this,&Monitor::Start_Drawing,
            [=]()
            {
                series_vol->clear();
                m_data.clear();
                m_count = 0;
                Chart_vol->axisX()->setRange(0,720);
                Chart_vol->axisY()->setRange(-1,1);


                for(int i=0;i<720;i++)
                {
                    qreal x = i;
                    m_data.append(QPointF(i,qSin(2.0*3.141592*x/360)));
                }
                series_vol->append(m_data);

                pTimer1->start(1);      //定时器，绘制波形图
            }
            );
    connect(this,&Monitor::Stop_Drawing,
            [=]()
            {
                pTimer1->stop();
            }
            );
    connect(pTimer1,&QTimer::timeout,
            [=]()
            {
                qreal x= m_count;
                for (int i=0; i<m_data.size(); ++i)
                     m_data[i].setX(m_data.at(i).x() - 1);
                m_data.append(QPointF(720, qSin(2.0 * 3.141592 * x / 360.0)));
                m_data.removeFirst();
                series_vol->replace(m_data);

                ++ m_count;
                if (m_count > 360)
                    m_count = 0;
            }
            );
}


void Monitor::on_pushButton_drawing_clicked()
{
    emit Start_Drawing();
}

void Monitor::on_pushButton_drawing_stop_clicked()
{
    emit Stop_Drawing();
}
