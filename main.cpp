#include <QCoreApplication>

#include "modbusmaster.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ModbusMaster *master = new ModbusMaster();
    master->start();

    return a.exec();
}
