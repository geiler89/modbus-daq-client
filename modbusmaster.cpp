#include "modbusmaster.h"

#include <iostream>

ModbusMaster::ModbusMaster(QObject *parent) : QObject(parent)
{

}

ModbusMaster::~ModbusMaster()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();

    delete modbusDevice;
}

void ModbusMaster::connectToSlaveDevice()
{
    connect(modbusDevice, &QModbusClient::errorOccurred, this, &ModbusMaster::onErrorOcurred);
    connect(modbusDevice, &QModbusClient::stateChanged, this, &ModbusMaster::onStateChanged);

    modbusDevice->connectDevice();
}

void ModbusMaster::onErrorOcurred(QModbusDevice::Error error)
{
    Q_UNUSED(error)
    emit sendMessage(modbusDevice->errorString());
}

void ModbusMaster::onStateChanged(QModbusDevice::State state)
{
    switch (state)
    {
        case QModbusDevice::UnconnectedState:
            emit sendMessage("The device is disconnected");
            modbusDevice->disconnectDevice();
        break;
        case QModbusDevice::ConnectingState:
            emit sendMessage("The device is being connected");
        break;
        case QModbusDevice::ConnectedState:
            emit sendMessage("The device is connected to the Modbus network");
            emit selectReadWriteOperation();
        break;
        case QModbusDevice::ClosingState:
            emit sendMessage("The device is being closed");
        break;
    }
}

void ModbusMaster::onReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
                                     .arg(QString::number(unit.value(i)/*,
                                          unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16*/));
            qInfo() << entry;
        }
    } else if (reply->error() == QModbusDevice::ProtocolError) {

    } else {

    }

    reply->deleteLater();
}

void ModbusMaster::setSerialConnectionParameters()
{
    modbusDevice = new QModbusRtuSerialMaster(this);
    connectToSlaveDevice();
}

void ModbusMaster::setTCPConnectionParameters(QString ip, uint port)
{
    modbusDevice = new QModbusTcpClient(this);
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ip);
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, QString::number(port));

    connectToSlaveDevice();
}

void ModbusMaster::setParametersForOperations(quint8 modbusAddres, quint16 startRegistersAddress, quint16 numberRegistersRead,
                                              quint8 addressModbusSlave, QList<quint16> valuesToWrite)
{
    QModbusDataUnit block(static_cast<QModbusDataUnit::RegisterType>(modbusAddres),
                          startRegistersAddress, numberRegistersRead);
    QModbusReply *reply = nullptr;

    if (valuesToWrite.isEmpty())
    {
        reply = modbusDevice->sendReadRequest(block, addressModbusSlave);
    } else
    {
        for (int i = 0; i < valuesToWrite.size(); i++)
        {
            block.setValue(i, valuesToWrite.at(i));
        }

        reply = modbusDevice->sendWriteRequest(block, addressModbusSlave);
    }

    if (!reply->isFinished())
        connect(reply, &QModbusReply::finished, this, &ModbusMaster::onReadReady);
    else
        delete reply; // broadcast replies return immediately
}
