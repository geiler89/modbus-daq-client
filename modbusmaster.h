#ifndef MODBUSMASTER_H
#define MODBUSMASTER_H

#include <QObject>
#include <QDebug>
#include <QModbusDevice>
#include <QModbusClient>
#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QModbusDataUnit>

#include <iostream>

class ModbusMaster : public QObject
{
    Q_OBJECT
public:
    explicit ModbusMaster(QObject *parent = nullptr);
    ~ModbusMaster();

signals:
    void sendMessage(QString message);
    void selectReadWriteOperation();

public slots:
    void setSerialConnectionParameters();
    void setTCPConnectionParameters(QString ip, uint port);
    void setParametersForOperations(quint8 modbusAddres, quint16 startRegistersAddress,
                                    quint16 numberRegistersRead, quint8 addressModbusSlave, QList<quint16> valuesToWrite);

private slots:
    void onErrorOcurred(QModbusDevice::Error error);
    void onStateChanged(QModbusDevice::State state);
    void onReadReady();

private:
    QModbusClient *modbusDevice = nullptr;

    void connectToSlaveDevice();
};

#endif // MODBUSMASTER_H
