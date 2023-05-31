#include "modbusmaster.h"

ModbusMaster::ModbusMaster(QObject *parent) : QObject(parent), cin(stdin), cout(stdout)
{

}

ModbusMaster::~ModbusMaster()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();

    delete modbusDevice;
}

void ModbusMaster::start(bool tcp)
{
    if (!tcp)
    {
        modbusDevice = new QModbusRtuSerialMaster(this);
        setSerialConnectionParameters();
    } else
    {
        modbusDevice = new QModbusTcpClient(this);
        setTCPConnectionParameters();
    }

    connect(modbusDevice, &QModbusClient::errorOccurred, this, &ModbusMaster::onErrorOcurred);
    connect(modbusDevice, &QModbusClient::stateChanged, this, &ModbusMaster::onStateChanged);

    modbusDevice->connectDevice();
}

void ModbusMaster::onErrorOcurred(QModbusDevice::Error error)
{
    Q_UNUSED(error)
    qInfo() << modbusDevice->errorString();
}

void ModbusMaster::onStateChanged(QModbusDevice::State state)
{
    switch (state)
    {
        case QModbusDevice::UnconnectedState:
            qInfo() << "The device is disconnected";
        break;
        case QModbusDevice::ConnectingState:
            qInfo() << "The device is being connected";
        break;
        case QModbusDevice::ConnectedState:
            qInfo() << "The device is connected to the Modbus network\n";
            selectReadWriteOption() == 1 ? readParameters() : writeParameters();
        break;
        case QModbusDevice::ClosingState:
            qInfo() << "The device is being closed";
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
                                     .arg(QString::number(unit.value(i),
                                          unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));
            qInfo() << entry;
        }
    } else if (reply->error() == QModbusDevice::ProtocolError) {

    } else {

    }

    reply->deleteLater();
}

void ModbusMaster::setSerialConnectionParameters()
{

}

void ModbusMaster::setTCPConnectionParameters()
{
    QString ip, port, option;

    qInfo() << "Enter IP address:";
    cin >> ip;
    qInfo() << "Enter port:";
    cin >> port;

    modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ip);
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, port.toUInt());
}

quint8 ModbusMaster::selectReadWriteOption()
{
    qInfo() << "Please, select 1 to read or 2 to write";
    quint8 readWrite = cliOption(1,2);

    return readWrite;
}

void ModbusMaster::readParameters()
{
    qInfo() << "Please, select the type of modbus addres to read:\n"
            << "1 - Discrete Inputs\n" << "2 - Coils\n"
            << "3 - Input Registers\n" << "4 - Holding Registers\n";
    quint8 modbusAddres = cliOption(1, 4);

    qInfo() << "Please, select the start address:\n";
    int startRegistersAddress = cliOption();

    qInfo() << "Please, select the number of entries to read:\n";
    quint16 numberRegistersRead;
    do {
        numberRegistersRead = cliOption();

        if(numberRegistersRead == 0)
            qInfo() << "Please, select a number greater than zero:\n";
    } while (numberRegistersRead == 0);

    qInfo() << "Please, select the address of modbus server:\n";
    quint8 addressModbusSlave;
    do {
        addressModbusSlave = cliOption();

        if(addressModbusSlave == 0)
            qInfo() << "Please, select a number greater than zero:\n";
    } while (addressModbusSlave == 0);

    QModbusReply *reply = modbusDevice->sendReadRequest(
                QModbusDataUnit(static_cast<QModbusDataUnit::RegisterType>(modbusAddres),
                                startRegistersAddress, numberRegistersRead),
                addressModbusSlave);

    if (!reply->isFinished())
        connect(reply, &QModbusReply::finished, this, &ModbusMaster::onReadReady);
    else
        delete reply; // broadcast replies return immediately
}

void ModbusMaster::writeParameters()
{
    QString option;
    bool validateConversion;
    quint8 selectedOption = 0;

    if (modbusDevice->connectDevice())
    {
        qInfo() << "*****Device connected*****";
        qInfo() << "Please, select 1 to read or 2 to write";

        do {
            cin >> option;
            option.toUInt(&validateConversion);

            if (!validateConversion)
            {
                qInfo() << "Please, insert a number";
            } else
            {
                 selectedOption = option.toUInt();
                 if (selectedOption > 2 || selectedOption < 1)
                 {
                     qInfo() << "Please, select a correct option number";
                 }
            }
        } while (!validateConversion || (selectedOption > 2 || selectedOption < 1));
    }
}

uint ModbusMaster::cliOption(uint begin, uint end)
{
    QString option;
    bool validateConversion;
    bool correctedOption = false;
    uint selectedOption;

    do {
        cin >> option;
        selectedOption = option.toUInt(&validateConversion);

        if (!validateConversion)
        {
            qInfo() << "Please, insert a number";
        } else
        {
            if (end == 0)
                break;

            if (selectedOption >= begin && selectedOption <=end)
                correctedOption = true;
            else
                qInfo() << "Please, select a correct option number";
        }

        option.clear();
    } while (!validateConversion || !correctedOption);

    return selectedOption;
}
