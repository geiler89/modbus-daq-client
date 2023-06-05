#include <QCoreApplication>
#include <QObject>

#include "climessage.h"
#include "modbusmaster.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CLIMessage *messages = new CLIMessage();
    ModbusMaster *master = new ModbusMaster();

    QObject::connect(master, &ModbusMaster::sendMessage,
                     messages, &CLIMessage::printMessage);
    QObject::connect(master, &ModbusMaster::selectReadWriteOperation,
                     messages, &CLIMessage::selectReadWriteOption);
    QObject::connect(messages, &CLIMessage::sendModbusTCPConnectionParameters,
                     master, &ModbusMaster::setTCPConnectionParameters);
    QObject::connect(messages, &CLIMessage::sendParametersForOperations,
                     master, &ModbusMaster::setParametersForOperations);

    messages->start();

    return a.exec();
}
