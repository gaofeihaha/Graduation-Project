#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "monitor.h"


namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();


protected:
    //重写绘图事件
    //如果在窗口绘图，必须在绘图事件里实现
    //绘图事件内部自动调用，窗口需要重绘的时候（状态改变）
    void paintEvent(QPaintEvent *event);
private slots:
    void on_pushButton_Login_clicked();

    void on_pushButton_Exit_clicked();

private:
    Ui::Login *ui;
    Monitor w;
};

#endif // LOGIN_H
