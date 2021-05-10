#include <QString>
#include <QMap>
#ifndef TEMPCORE_H
#define TEMPCORE_H


class tempCore
{
public:
    tempCore();
    //static QString init();
private:
    QMap<QString, QString> data;
};

#endif // TEMPCORE_H
