#include <string>
#include <QStringListModel>
#include <QMap>
#ifndef HARDWAREINFO_H
#define HARDWAREINFO_H


class hardwareInfo
{
public:
    hardwareInfo();
    QString getMemSpeed();
    QString getMemSockets();
    QMap<QString, QString> getGpuLoad(int index);
private:
    QMap<int, QMap<QString, QString>> gpu;
    QMap<QString, QString> getAmdGpuLoad(int index);
    QMap<QString, QString> getIntelGpuLoad(int index) ;
    QMap<QString, QString> getNvidiaGpuLoad(int index);
};

#endif // HARDWAREINFO_H
