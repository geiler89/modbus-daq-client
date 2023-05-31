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

    void start(bool tcp = true);

signals:

private slots:
    void onErrorOcurred(QModbusDevice::Error error);
    void onStateChanged(QModbusDevice::State state);
    void onReadReady();

private:
    QModbusClient *modbusDevice = nullptr;
    QTextStream cin, cout;
    bool isTCP;

    void setSerialConnectionParameters();
    void setTCPConnectionParameters();
    quint8 selectReadWriteOption();
    quint8 selectTypeModbusAddres();
    void readParameters();
    void writeParameters();
    uint cliOption(uint begin = 0, uint end = 0);
};

#endif // MODBUSMASTER_H
