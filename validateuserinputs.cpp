#include "validateuserinputs.h"

bool validateIpAddress(const QString &ipAddress)
{
    QRegularExpression ipRegex("^(([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.){3}([01]?\\d\\d?|2[0-4]\\d|25[0-5])$");
    QRegularExpressionMatch match = ipRegex.match(ipAddress);
    return match.hasMatch();
}
