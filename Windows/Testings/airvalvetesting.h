#ifndef AIRVALVETESTING_H
#define AIRVALVETESTING_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
#include "myheader.h"
namespace Ui {
class AirValveTesting;
}

enum AutoType{
    noneAuto,
    singleAuto,
    groupAuto,
    wholeAuto,
};

class AirValveTesting : public QDialog
{
    Q_OBJECT

public:
    explicit AirValveTesting(QWidget *parent = 0);
    ~AirValveTesting();

private slots:
    void deal();
    void timeoutslot();
    void on_singleauto_Val_pb_clicked();
    void on_singlemanual_Val_pb_clicked();
    void on_speedup_Val_pb_clicked();
    void on_slowdown_Val_pb_clicked();
    void on_back_Val_pb_clicked();
    void on_groupauto_Val_pb_clicked();
    void on_wholeauto_Val_pb_clicked();
    void valveSwitchClicked();
    void valveNameClicked();
    void on_shuttlegroup_Val_pb_clicked();

private:
    Ui::AirValveTesting *ui;

    QToolButton *click;
    QPushButton* buttun;
    QTimer *timer;
    AutoType autoType;
    int allStart;
    char readValve[31];
    bool readInit;
    quint8 initValve[11][24];
    int times;
    int testSpeed;
    quint8 ValveNum[11];//每组气阀个数
    QPushButton *btn_valve[11];//气阀路数按钮
    quint8 currentValveNum;//当前气阀路数
    quint8 currentValveIndex[11];//当前气阀索引
    QPushButton *valveName[24];
    QToolButton *valveSwitch[24];
    quint8 valveStatus[11][24];//气阀状态
    quint8 manualCount;
    quint8 lastValveIndex;
    void initialValve();//初始化气阀界面
    void refreshValve();//刷新气阀界面
    void chooseValveNum(int num);//
    void valveSwitchClicked(int lastIndex, int index);
};

#endif // AIRVALVETESTING_H
