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

void CLIMessage::getTypeModbusAddres(quint8 &pModbusAddres)
{
    printMessage("Please, select the type of modbus addres to read:\n"
                 "1 - Discrete Inputs\n2 - Coils\n"
                 "3 - Input Registers\n4 - Holding Registers");
    pModbusAddres = cliOption(1, 4);
}

void CLIMessage::getStartAddress(quint16 &pStartRegistersAddress)
{
    printMessage("Please, select the start address:");
    pStartRegistersAddress = cliOption();
}

void CLIMessage::getNumberEntries(quint16 &pNumberRegisters)
{
    printMessage("Please, select the number of entries to read:");
    do {
        pNumberRegisters = cliOption();

        if(pNumberRegisters == 0)
            printMessage("Please, select a number greater than zero:");
    } while (pNumberRegisters == 0);
}

void CLIMessage::getAddressModbusServer(quint8 &pAddressModbusSlave)
{
    printMessage("Please, select the address of modbus server:");
    do {
        pAddressModbusSlave = cliOption();

        if(pAddressModbusSlave == 0)
            printMessage("Please, select a number greater than zero:");
    } while (pAddressModbusSlave == 0);
}

void CLIMessage::setModbusServerParameters(quint8 &pModbusAddres, quint16 &pStartRegistersAddress,
                                           quint16 &pNumberRegisters, quint8 &pAddressModbusSlave)
{
    getTypeModbusAddres(pModbusAddres);
    getStartAddress(pStartRegistersAddress);
    getNumberEntries(pNumberRegisters);
    getAddressModbusServer(pAddressModbusSlave);
}

void CLIMessage::setValuesToWrite(quint16 numberRegistersToWrite, QList<quint16> &pValuesToWrite)
{
    printMessage(QString("Please, enter the %1 values to be written:").arg(numberRegistersToWrite));
    quint16 value;
    do {
        in.flush();
        in >> value;
        pValuesToWrite << value;
        numberRegistersToWrite--;
    } while (numberRegistersToWrite != 0);
}

void CLIMessage::selectReadWriteOption()
{
    quint8 modbusAddres;
    quint16 startRegistersAddress;
    quint16 numberRegisters;
    quint8 addressModbusSlave;
    QList<quint16> valuesToWrite;

    printMessage("Please, select 1 to read or 2 to write");
    quint8 readWrite = cliOption(1,2);

    setModbusServerParameters(modbusAddres, startRegistersAddress, numberRegisters, addressModbusSlave);

    if (readWrite == 2)
        setValuesToWrite(numberRegisters, valuesToWrite);

    emit sendParametersForOperations(modbusAddres, startRegistersAddress, numberRegisters, addressModbusSlave, valuesToWrite);
}
