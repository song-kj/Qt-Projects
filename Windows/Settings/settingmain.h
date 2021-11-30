#ifndef SETTINGMAIN_H
#define SETTINGMAIN_H

#include <QDialog>
#include <QTimer>
#include "Windows/Settings/parametersetting.h"
#include "Windows/Settings/yieldsetting.h"
#include "Windows/Settings/zerolevelsetting.h"
#include "Windows/Settings/parametersaveinfo.h"
#include "myheader.h"

namespace Ui {
class SettingMain;
}

class SettingMain : public QDialog
{
    Q_OBJECT

public:
    explicit SettingMain(QWidget *parent = 0);
    ~SettingMain();

private slots:
    void on_parametersetting_lb_clicked();
    void on_motorsetting_lb_clicked();
    void on_zerolevelsetting_lb_clicked();
    void on_back_lb_clicked();
    void update_slot();

    void on_warnsetting_lb_clicked();

    void on_netsetting_lb_clicked();

signals:
        void sendProgressData(int);
        void setGotoMark(int);
        void sendStartToZero();
private:
    Ui::SettingMain *ui;
    bool sendpar;
    ParameterSaveInfo *saveinfo;
    QTimer *timer_settingmain;
};

#endif // SETTINGMAIN_H
