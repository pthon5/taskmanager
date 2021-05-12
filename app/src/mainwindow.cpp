#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processproxymodel.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    // setup local variables
    this->curSelectedPID = 0;

    setWindowIcon(QIcon(":/icon.png"));

    ui->setupUi(this);
    // setup StatsCore
    this->core = StatsCore::createCore(1000, parent);

    QStringList staticInfo = core->staticInformation();

    // setup process table
    QAbstractItemModel *processModel = core->processModel();
    // setup a proxy model to add color and other UI-related elements to it
    ProcessProxyModel *proxyModel = new ProcessProxyModel(staticInfo.at(StatsCore::StaticSystemField::TotalMemory).toULongLong(), processModel);
    proxyModel->setSourceModel(processModel);
    ui->processView->setModel(proxyModel);
    ui->processView->setSelectionBehavior(QTableView::SelectRows);
    ui->processView->setSelectionMode(QTableView::SingleSelection);
    ui->processView->setColumnWidth(0,190);
    ui->processView->setColumnWidth(1,60);
    ui->processView->setColumnWidth(2,60);
    ui->processView->setColumnWidth(3,100);
    ui->processView->setColumnWidth(4,90);
    ui->processView->setColumnWidth(5,80);
    ui->processView->setSortingEnabled(true);
    ui->processView->setEditTriggers(QTableView::NoEditTriggers);

    // setup performance tab
    // first setup static information
    ui->cpuName->setText(staticInfo.at(StatsCore::StaticSystemField::CPUName));
    ui->maxSpeed->setText(staticInfo.at(StatsCore::StaticSystemField::MaxSpeed));
    ui->cores->setText(staticInfo.at(StatsCore::StaticSystemField::Cores));
    ui->memorySpeed->setText(staticInfo.at(StatsCore::StaticSystemField::MemorySpeed));
    ui->memorySockets->setText(staticInfo.at(StatsCore::StaticSystemField::MemorySockets));
    ui->logicalProcessors->setText(staticInfo.at(StatsCore::StaticSystemField::LogicalProcessors));
    QAbstractItemModel *systemModel = core->systemModel();
    connect(systemModel, &QAbstractItemModel::dataChanged, [=](const QModelIndex &topLeft, const QModelIndex &bottomRight) {
        for(int i = topLeft.row(); i <= bottomRight.row(); i ++)
        {
            QString data = systemModel->index(i, 0).data().toString();
            switch(i)
            {
            case StatsCore::DynamicSystemField::UpTime:
                ui->upTime->setText(data); break;
            case StatsCore::DynamicSystemField::Utilization:
            {
                double rawData = data.toDouble();
                ui->utilization->setText(QString::number(rawData, 'f', 1) + " %");
                ui->cpuUsagePLot->addData(rawData);
                proxyModel->setCPUUtilization(rawData);
                this->updateUsageOptionIcon();
                break;
            }
            case StatsCore::DynamicSystemField::CPUSpeed:
                ui->speed->setText(data); break;
            case StatsCore::DynamicSystemField::Processes:
                ui->processes->setText(data); break;
            case StatsCore::DynamicSystemField::UsedMemory:
            {
                quint64 usedMemory = data.toULongLong();
                if(usedMemory < 1024 * 1024)
                    ui->usedMemory->setText(QString::number(usedMemory / 1024.0, 'f', 2) + " KB");
                else if(usedMemory < 1024 * 1024 * 1024)
                    ui->usedMemory->setText(QString::number(usedMemory / (1024.0 * 1024.0), 'f', 2) + " MB");
                else
                    ui->usedMemory->setText(QString::number(usedMemory / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB");
                ui->memoryUsagePlot->addData(data.toDouble() / (1024 * 1024 * 1024));
                proxyModel->setMemoryUtilization(data.toDouble() * 100 / staticInfo.at(StatsCore::StaticSystemField::TotalMemory).toDouble());
                this->updateUsageOptionIcon();
                break;
            }
            case StatsCore::DynamicSystemField::AvailableMemory:
            {
                quint64 availableMemory = data.toULongLong();
                if(availableMemory < 1024 * 1024)
                    ui->availableMemory->setText(QString::number(availableMemory / 1024.0, 'f', 2) + " KB");
                else if(availableMemory < 1024 * 1024 * 1024)
                    ui->availableMemory->setText(QString::number(availableMemory / (1024.0 * 1024.0), 'f', 2) + " MB");
                else
                    ui->availableMemory->setText(QString::number(availableMemory / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB");
                break;
            }
            case StatsCore::DynamicSystemField::CachedMemory:
            {
                quint64 cachedMemory = data.toULongLong();
                if(cachedMemory < 1024 * 1024)
                    ui->cached->setText(QString::number(cachedMemory / 1024.0, 'f', 2) + " KB");
                else if(cachedMemory < 1024 * 1024 * 1024)
                    ui->cached->setText(QString::number(cachedMemory / (1024.0 * 1024.0), 'f', 2) + " MB");
                else
                    ui->cached->setText(QString::number(cachedMemory / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB");
                break;
            }
            case StatsCore::DynamicSystemField::Temperature:
                double temperature = data.toDouble();
                if(temperature > 50 && temperature <= 60)
                    ui->temperature->setStyleSheet("color: rgb(255, 140, 102);");
                else if(temperature > 60 && temperature <= 70)
                    ui->temperature->setStyleSheet("color: rgb(255, 64, 0);");
                else if(temperature > 70)
                    ui->temperature->setStyleSheet("color: red; font: bold;");
                ui->temperature->setText(data + " °C"); break;
            }
        }
    });
    // setup cpu usage plot
    ui->cpuUsagePLot->setPlotName("% Utilization");
    ui->cpuUsagePLot->setMaximumTime(60);
    ui->cpuUsagePLot->setMaximumUsage(100);
    ui->cpuUsagePLot->setUsageUnit("%");
    ui->cpuUsagePLot->setThemeColor(QColor(17, 125, 187));

    // setup memory usage plot
    ui->memoryUsagePlot->setPlotName("Memory Usage");
    ui->memoryUsagePlot->setMaximumTime(60);
    ui->memoryUsagePlot->setMaximumUsage(staticInfo.at(StatsCore::StaticSystemField::TotalMemory).toDouble() / (1024 * 1024 * 1024));
    ui->memoryUsagePlot->setUsageUnit("GB");
    ui->memoryUsagePlot->setThemeColor(QColor(139,18,174));

    this->updateUsageOptionIcon();

    // setup other widgets
    connect(ui->killProcessButton, &QPushButton::clicked,
            [=]{
        QModelIndex curIndex = ui->processView->currentIndex();
        if(curIndex.isValid())
        {
            unsigned int pid = curIndex.sibling(curIndex.row(), 1).data().toUInt();
            this->core->killProcess(pid);
        }
    });

    connect(processModel, &QAbstractItemModel::modelAboutToBeReset, this, [=] {
        // store the current selected PID
        QModelIndex index = ui->processView->selectionModel()->currentIndex();
        if (index.isValid())
            this->curSelectedPID = index.sibling(index.row(), 1).data().toULongLong();
    });

    connect(processModel, &QAbstractItemModel::modelReset, this, [=] {
        // restore the current selected PID
        if(this->curSelectedPID != 0)
        {
            int current = 0;
            while(ui->processView->model()->canFetchMore(QModelIndex()))
            {
                // the first fetch has been done by library
                if(current != 0)
                    ui->processView->model()->fetchMore(QModelIndex());

                int rowCount = ui->processView->model()->rowCount();
                for (; current < rowCount; current ++)
                {
                    const QModelIndex &index = ui->processView->model()->index(current, 1);
                    if(index.data().toULongLong() == this->curSelectedPID)
                    {
                        ui->processView->selectionModel()->setCurrentIndex(index,
                                                                           QItemSelectionModel::Rows | QItemSelectionModel::ClearAndSelect);
                        return;
                    }
                }
            };
            // if cannot find the current PID (which means it has been removed)
            this->curSelectedPID = 0;
        }
    });

    // connect list widget's signals to stack widget
    ui->usageOptionList->setIconSize(QSize(60,50));
    connect(ui->usageOptionList, &QListWidget::currentRowChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    ui->usageOptionList;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateUsageOptionIcon()
{
    QPixmap cpuPixmap = ui->cpuUsagePLot->toPixmap();
    cpuPixmap = cpuPixmap.copy(0, 18, cpuPixmap.width(), cpuPixmap.height() - 36);
    ui->usageOptionList->item(0)->setIcon(QIcon(cpuPixmap));
    QPixmap memoryPixmap = ui->memoryUsagePlot->toPixmap();
    memoryPixmap = memoryPixmap.copy(0, 18, memoryPixmap.width(), memoryPixmap.height() - 36);
    ui->usageOptionList->item(1)->setIcon(QIcon(memoryPixmap));
}

QListWidget* MainWindow::getQLW() {
    return ui->usageOptionList;
}
