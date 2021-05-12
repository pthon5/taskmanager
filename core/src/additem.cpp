#include "additem.h"
#include <QString>
#include <thread>
#include <string>
#include <unistd.h>
#include <QDebug>
#include "hardwareinfo.h"
#include "tempcore.h"
hardwareInfo hwinfo1;

addItem::addItem(QString name,const int type)
{
    /*
     types:
         1=ethernet
         2=drive
         3=gpu
*/
    try {
        switch (type) {
        case 1: {
            std::thread eth(&addItem::createEthernet, this, name);
            eth.detach();
            break;
        }
        case 2: {
            std::thread drive(&addItem::createDrive, this, name);
            drive.detach();
            break;
        }
        case 3: {
            std::thread gpu(&addItem::createGpu, this, name);
            gpu.detach();
            break;
            }
        }
    }  catch (...) {

    }

}

void addItem::createEthernet(const QString name) {
    while (true) {
        qWarning() << "Hi from eth thread" << rand() << "NAME: " << name;
        sleep(1);
    }
}



void addItem::createGpu(QString name) {

    tempCore tc;
    QMap<QString, QString> gpu_info;
    while (true) {
        gpu_info = hwinfo1.getGpuLoad(1);
        if (gpu_info.size() == 0)
            return;
        qWarning() << "GPU temp:" << tc.getGpuTemp() << " GPU load:" << gpu_info["gpu_load"] << " Mem load:" << gpu_info["mem_load"] << " Mem load MB:"<<gpu_info["mem_load_mb"];
        sleep(1);
    }


}

void addItem::createDrive(const QString name) {

}
