#include "login.h"
#include "ui_login.h"
#include <QPainter>
#include <QPen>
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    resize(800,500);

    ui->lineEdit_passward->setEchoMode(QLineEdit::Password);//密码隐藏显示模式
    ui->lineEdit_account->setStyleSheet("background:transparent;border-style:outset");//设置文本框透明；无框
    ui->lineEdit_passward->setStyleSheet("background:transparent;border-style:outset");
    ui->pushButton_Login->setStyleSheet("border-radius:5px;background-color: rgb(121,83,63)");//设置按钮弧度、颜色
    ui->pushButton_Exit->setStyleSheet("border-radius:5px;background-color: rgb(121,83,63)");
}

Login::~Login()
{
    delete ui;
}


void Login::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    //绘图操作
    //p.drawxxxx;
    //画背景图
    //p.drawPixmap(0,0,width(),height(),QPixmap("../image/wanshengjie.jpg"));
    //背景图片放到源文件文件夹同一父文件夹,与下一句相同作用
    p.drawPixmap(rect(),QPixmap("../picture/Linux.png"));
}

void Login::on_pushButton_Login_clicked()
{
    QString Account = ui->lineEdit_account->text();
    QString Passward = ui->lineEdit_passward->text();

    if (Account=="gaofei"&&Passward=="123456")
    {
        this->hide();
        w.setWindowTitle("交互界面");
        w.show();
    }
    else  {QMessageBox::about(this,"警告！","账号或密码错误！");}

}

void Login::on_pushButton_Exit_clicked()
{
    this->close();
}
