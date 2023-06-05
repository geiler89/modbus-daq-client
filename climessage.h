#ifndef CLIMESSAGE_H
#define CLIMESSAGE_H

#include <QObject>
#include <QTextStream>
#include <QDebug>

class CLIMessage : public QObject
{
    Q_OBJECT
public:
    explicit CLIMessage(QObject *parent = nullptr);

    void start();

signals:
    void sendModbusRTUConnectionParameters();
    void sendModbusTCPConnectionParameters(QString ip, uint port);
    void sendParametersForOperations(quint8 modbusAddres, quint16 startRegistersAddress,
                                     quint16 numberRegistersRead, quint8 addressModbusSlave, QList<quint16> valuesToWrite);

public slots:
    void printMessage(QString message);
    void selectReadWriteOption();

private:
    QTextStream in, out;

    uint cliOption(uint begin = 0, uint end = 0);
    void setModbusRTUConnectionParameters();
    void setModbusTCPConnectionParameters();
    void getTypeModbusAddres(quint8 &pModbusAddres);
    void getStartAddress(quint16 &pStartRegistersAddress);
    void getNumberEntries(quint16 &pNumberRegisters);
    void getAddressModbusServer(quint8 &pAddressModbusSlave);
    void setModbusServerParameters(quint8 &pModbusAddres, quint16 &pStartRegistersAddress,
                                   quint16 &pNumberRegisters, quint8 &pAddressModbusSlave);
    void setValuesToWrite(quint16 numberRegistersToWrite, QList<quint16> &pValuesToWrite);
};

#endif // CLIMESSAGE_H
