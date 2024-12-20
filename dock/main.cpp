#include "Widget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.resize(1080 * 2, 720 * 2);
    w.show();
    return a.exec();
}
