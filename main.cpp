#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("by 0x296f");
    Widget w;
    w.show();
    w.resize(600,720);
    return a.exec();
}
