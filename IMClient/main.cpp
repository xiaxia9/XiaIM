#include "src/login/login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CLogin w;
    w.show();
    return a.exec();
}
