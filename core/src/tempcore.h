#include <QString>
#include <QMap>
#ifndef TEMPCORE_H
#define TEMPCORE_H


class tempCore
{
private:
    QMap<QString, QMap<int, QString>> data;
    void writer(const QString &str);
public:
    tempCore();
    QString getCpuTemp();
    QString getGpuTemp();
};

#endif // TEMPCORE_H
