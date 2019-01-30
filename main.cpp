#include "monitor.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w_login;
 //   w.show();
    w_login.show();

    return a.exec();
}
