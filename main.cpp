#include "connectionform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectionForm w;
    w.show();
    return a.exec();
}
