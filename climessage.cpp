#include "climessage.h"

CLIMessage::CLIMessage(QObject *parent) : QObject(parent), in(stdin), out(stdout)
{

}

void CLIMessage::start()
{
    printMessage("For connecting to a Modbus device, select 1 for a Modbus RTU or 2 for a Modbus TCP:");

    switch (cliOption(1,2))
    {
        case 1:
            setModbusRTUConnectionParameters();
            break;
        case 2:
            setModbusTCPConnectionParameters();
            break;
    }
}

void CLIMessage::printMessage(QString message)
{
    out << message << endl;
}

uint CLIMessage::cliOption(uint begin, uint end)
{
    QString option;
    bool validateConversion;
    bool correctedOption = false;
    uint selectedOption;

    do {
        in >> option;
        selectedOption = option.toUInt(&validateConversion);

        if (!validateConversion)
        {
            printMessage("Please, insert a number");
        } else
        {
            if (end == 0)
                break;

            if (selectedOption >= begin && selectedOption <=end)
                correctedOption = true;
            else
                printMessage("Please, select a correct option number");
        }

        option.clear();
    } while (!validateConversion || !correctedOption);

    return selectedOption;
}

void CLIMessage::setModbusRTUConnectionParameters()
{

}

void CLIMessage::setModbusTCPConnectionParameters()
{
    QString ip, port;

    printMessage("Enter IP address:");
    in >> ip;
    printMessage("Enter port:");
    in >> port;

    emit sendModbusTCPConnectionParameters(ip, port.toUInt());
}

void CLIMessage::setReadParameters()
{
    printMessage("Please, select the type of modbus addres to read:\n"
                 "1 - Discrete Inputs\n2 - Coils\n"
                 "3 - Input Registers\n4 - Holding Registers");
    quint8 modbusAddres = cliOption(1, 4);

    printMessage("Please, select the start address:");
    qint16 startRegistersAddress = cliOption();

    printMessage("Please, select the number of entries to read:");
    quint16 numberRegistersRead;
    do {
        numberRegistersRead = cliOption();

        if(numberRegistersRead == 0)
            printMessage("Please, select a number greater than zero:");
    } while (numberRegistersRead == 0);

    printMessage("Please, select the address of modbus server:");
    quint8 addressModbusSlave;
    do {
        addressModbusSlave = cliOption();

        if(addressModbusSlave == 0)
            printMessage("Please, select a number greater than zero:");
    } while (addressModbusSlave == 0);

    emit sendParametersForReading(modbusAddres, startRegistersAddress,
                                  numberRegistersRead, addressModbusSlave);
}

void CLIMessage::selectReadWriteOption()
{
    printMessage("Please, select 1 to read or 2 to write");
    quint8 readWrite = cliOption(1,2);

    switch (readWrite)
    {
        case 1:
        setReadParameters();
            break;
        case 2:
            break;
    }
}
