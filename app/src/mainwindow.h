#pragma once
#include "stable.h"
#include "statscore.h"
#include "QtWidgets"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    StatsCore *core;
    quint64 curSelectedPID;
    void setupUsagePlots();
private slots:
    void updateUsageOptionIcon();
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateWidget(const QVariantList & property);
    QListWidget* getQLW();
private:
    Ui::MainWindow *ui;
};
