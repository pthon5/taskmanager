#include "tempcore.h"
#include <QStringListModel>
#include <QFile>
#include <QMap>
#include <QtDebug>
#include <iostream>
#include <QDir>
#include <QIODevice>
#include <string>

void tempCore::writer(const QString &str) {
    QFile nameFile(str+"/name");
    if (nameFile.open(QIODevice::ReadOnly)) {
        QTextStream ts(&nameFile);
        const QString &nameContent = ts.readLine();
        if (nameContent == "coretemp") {
            bool start = true;
            int i = 0;
            do {
                if (data["cpus"][i] == 0) {
                    data["cpus"][i] = str;
                    start = false;
                }
                i++;
            } while(start);
        }
        if (nameContent == "amdgpu") {
            bool start = true;
            int i = 0;
            do {
                if (data["gpus"][i] == 0) {
                    data["gpus"][i] = str;
                    start = false;
                }
                i++;
            } while(start);
        }
    }

}

tempCore::tempCore()
{   
    int i = 0;
    do {
        QString full_path = "/sys/class/hwmon/hwmon"+QString::number(i);
        writer(full_path);
        i++;
    } while (QDir("/sys/class/hwmon/hwmon"+QString::number(i)).exists());
}

QString tempCore::getCpuTemp() {
    QFile dataFile(data["cpus"][0]+"/temp1_input");
    if (dataFile.open(QIODevice::ReadOnly)) {
        QTextStream ts(&dataFile);
        return QString::number(ts.readLine().toInt()/1000);
    } else
        return "No data";
}

QString tempCore::getGpuTemp() {
    QFile dataFile(data["gpus"][0]+"/temp1_input");
    if (dataFile.open(QIODevice::ReadOnly)) {
        QTextStream ts(&dataFile);
        return QString::number(ts.readLine().toInt()/1000);
    } else
        return "No data";
}

