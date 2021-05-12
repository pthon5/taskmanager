#include "hardwareinfo.h"
#include "stdio.h"
#include "iostream"
#include "cstdio"
#include <QDebug>
#include <string>
#include <QStringListModel>
#include <QMap>
QMap<int, QMap<QString, QString>> gpu;
hardwareInfo::hardwareInfo()
{
    //Collecting GPU's information
    FILE *speed = popen("sudo lspci | grep 'VGA'", "r");
    if (speed == NULL) {
        qWarning("Can't get gpu info, run as sudo.");
    } else {
        char buff[20];
        QString str;
        while(fgets(buff, sizeof(buff), speed)!=NULL){
            str += buff;
        }
        str = str.trimmed();
        const QStringList &sl = str.split("\n");
        for (int i=0;i<sl.size();++i) {
            if (sl[i].contains("amd", Qt::CaseInsensitive))
                gpu[i]["vendor"] = "amd";
            if (sl[i].contains("intel", Qt::CaseInsensitive))
                gpu[i]["vendor"] = "intel";
            if (sl[i].contains("nvidia", Qt::CaseInsensitive))
                gpu[i]["vendor"] = "nvidia";
            gpu[i]["bus"] = sl[i].split(" ")[0];
        }

        for (int i=0;i<gpu.size();++i) {
            qWarning() << "Card №:" << i << " Vendor:" << gpu[i]["vendor"] << " Bus:" << gpu[i]["bus"];
        }
    }
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

QMap<QString, QString> hardwareInfo::getGpuLoad(int index) {
    try {
        if (gpu[index]["vendor"] == "amd")
            return getAmdGpuLoad(index);
        if (gpu[index]["vendor"] == "indel")
            return getIntelGpuLoad(index);
        if (gpu[index]["vendor"] == "nvidia")
            return getNvidiaGpuLoad(index);
    }  catch (...) {qWarning() << "Error at hardwareinfo [getGpuLoad]";}
}

QMap<QString, QString> hardwareInfo::getAmdGpuLoad(int index) {
    try {
        QString ss = "sudo radeontop --b "+gpu[index]["bus"]+" -l 1 -d -";
        FILE *speed = popen(ss.toStdString().c_str(), "r");
        if (speed == NULL) {
            qWarning("Can't get gpu load, run as sudo.");
        } else {
            char buff[400];
            QString str;
            while(fgets(buff, sizeof(buff), speed)!=NULL){
                str += buff;
            }
            QMap<QString, QString> output;
            str = str.trimmed();
            //get gpu load
            QStringList sl = str.split("\n");
            sl = sl[1].split(","); //[1] gpu 0.00%
            sl = sl[1].split(" "); //[1] 0.00%
            output["gpu_load"] = sl[2];
            //get gpu memory load
            sl = str.split("\n");
            sl = sl[1].split(","); //[1] vram x.xx% xxx.xmb
            sl = sl[12].split(" "); //[1] 0.00%
            output["mem_load"] = sl[2];
            output["mem_load_mb"] = sl[3];
            return output;
        }

    } catch(...) {qWarning() << "Error at hardwareinfo [getAmdGpuLoad]";}
}


QMap<QString, QString> hardwareInfo::getIntelGpuLoad(int index) {
    try {
        QString ss = "sudo intel_gpu_top -l -s 0 -o - -d sys:/sys/devices/pci0000:00/0000:"+gpu[index]["bus"]+" | head -n 3";
        FILE *speed = popen(ss.toStdString().c_str(), "r");
        if (speed == NULL) {
            qWarning("Can't get gpu load, run as sudo.");
        } else {
            char buff[400];
            QString str;
            while(fgets(buff, sizeof(buff), speed)!=NULL){
                str += buff;
            }
            QMap<QString, QString> output;
            str = str.trimmed();
            //get gpu load
            QStringList sl = str.split("\n");
            sl = sl[2].split("\t");
            output["gpu_load"] = sl[1];
            //get gpu memory load
            output["mem_load"] = "No";
            output["mem_load_mb"] = "No";
            return output;
    }  } catch (...) {qWarning() << "Error at hardwareinfo [getIntelGpuLoad]";}
}

QMap<QString, QString> hardwareInfo::getNvidiaGpuLoad(int index) {
    try {

    }  catch (...) {qWarning() << "Error at hardwareinfo [getNvidiaGpuLoad]";}
}
