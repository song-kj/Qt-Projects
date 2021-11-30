#ifndef SYSTEMMAINTENCE_H
#define SYSTEMMAINTENCE_H

#include <QDialog>
#include <QTimer>
#include "Windows/SystemMaintenance/updateselect.h"
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
#include "Windows/SystemMaintenance/systemlog.h"
#include "Windows/SystemMaintenance/timesetting.h"
#include "myheader.h"

namespace Ui {
class SystemMaintence;
}

class SystemMaintence : public QDialog
{
    Q_OBJECT

public:
    explicit SystemMaintence(QWidget *parent = 0);
    ~SystemMaintence();
    void showColor();

private slots:
    void update_slot();
    void dealwithclick();

    void on_sysSetting_btn_clicked();

    void on_sysUnlock_btn_clicked();

private:
    Ui::SystemMaintence *ui;
    QTimer *timer_systemaintence;
    QPushButton *buttun;
    QPushButton *buttun_pre;
    bool visionShow;
    bool fenqiShowOrHide;
};

#endif // SYSTEMMAINTENCE_H
