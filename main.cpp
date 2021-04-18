#include "timed_relax.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    timed_relax* w = new timed_relax;
    w->show();
    return a.exec();
}
