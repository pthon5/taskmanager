#include "tempcore.h"
#include <QStringListModel>
#include <QFile>
#include <QMap>
#include <QtDebug>
#include <iostream>
#include <QDir>
QMap<QString, QString> data;
tempCore::tempCore()
{   
    int i = 0;
    do {
        std::cout << "Folder: /sys/class/hwmon"+std::to_string(i);
    } while (QDir("/sys/class/hwmon"+QString::number(i)).exists());
}

