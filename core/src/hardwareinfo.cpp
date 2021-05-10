#include "hardwareinfo.h"
#include "stdio.h"
#include "iostream"
#include "cstdio"
#include <QDebug>
#include <string>
#include <QStringListModel>
hardwareInfo::hardwareInfo()
{

}
QString hardwareInfo::getMemSpeed() {
    FILE *speed = popen("sudo dmidecode --type memory | grep 'Speed' | head -n 1", "r");
    if (speed == NULL) {
        qWarning("Can't get memory speed, run as sudo.");
        return 0;
    } else {
        char buff[20];
        QString str;
        while(fgets(buff, sizeof(buff), speed)!=NULL){
            str += buff;
        }
        str = str.trimmed();
        const QStringList &sl = str.split(" ");
        return sl[1];
    }
    return 0;
}

QString hardwareInfo::getMemSockets() {
    FILE *speed = popen("sudo dmidecode -t 16 | grep 'Number Of Devices'", "r");
    if (speed == NULL) {
        qWarning("Can't get memory sockets, run as sudo.");
        return 0;
    } else {
        char buff[20];
        QString str;
        while(fgets(buff, sizeof(buff), speed)!=NULL){
            str += buff;
        }
        str = str.trimmed();
        const QStringList &sl = str.split(" ");
        return sl[3];
    }
    return 0;
}

