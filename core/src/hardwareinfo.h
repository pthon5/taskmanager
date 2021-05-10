#include <string>
#include <QStringListModel>
#ifndef HARDWAREINFO_H
#define HARDWAREINFO_H


class hardwareInfo
{
public:
    hardwareInfo();
    static QString getMemSpeed();
    static QString getMemSockets();
};

#endif // HARDWAREINFO_H
