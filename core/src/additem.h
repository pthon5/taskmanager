#include <QString>
#include <string>
#include <QObject>
#ifndef ADDITEM_H
#define ADDITEM_H


class addItem
{
public:
    addItem(QString name,const int type);
private:
    void createEthernet(const QString name);
    void createGpu(QString name);
    void createDrive(const QString name);
};

#endif // ADDITEM_H
