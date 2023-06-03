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
    void sendParametersForReading(quint8 modbusAddres, qint16 startRegistersAddress,
                                  quint16 numberRegistersRead, quint8 addressModbusSlave);
    void initDataReading();

public slots:
    void printMessage(QString message);
    void selectReadWriteOption();

private:
    QTextStream in, out;

    uint cliOption(uint begin = 0, uint end = 0);
    void setModbusRTUConnectionParameters();
    void setModbusTCPConnectionParameters();
    void setReadParameters();
};

#endif // CLIMESSAGE_H
