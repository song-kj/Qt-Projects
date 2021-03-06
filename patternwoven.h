#ifndef PATTERNWOVEN_H
#define PATTERNWOVEN_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include<QDateTime>
#include<QTimer>
#include <QStyleFactory>
#include "Windows/Menu/menu.h"
#include  "Windows/Settings/yieldsetting.h"
#include "Communication/doubleportram.h"
#include "Windows/Settings/parametersetting.h"
#include "warningactuator.h"
#include "warningpatternwoven.h"
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
#include "Windows/Settings/zerolevelsetting.h"
#include "Windows/Settings/stitchadjust.h"
#include "outin.h"
#include "flatcloth.h"
#include "continueweave.h"
#include "Windows/WIFI/wifimode.h"
#include "myheader.h"
#define Device_Location "/dev/daul_ram"
#include "shuttlechoose.h"
#include "hirepurchase.h"
#include "error/neworder.h"
#include "Windows/WIFI/xrwifi.h"
#include "machineconfigure.h"
#include "Windows/Settings/parametersaveinfo.h"
#include "modifyco.h"
#define NeedleLocation "/xrfile/auxiliaryNeedle.er"

namespace Ui {
class PatternWoven;
}

class PatternWoven : public QMainWindow
{
    Q_OBJECT

public:
    explicit PatternWoven(QWidget *parent = 0);
    ~PatternWoven();
    void currentoutput_update();
    void showmessage(QString);
    void errorHardware();
    int readPreOffPowerState();
    void saveOffPowerState();

signals:
    void sendCancleMark(bool);

private slots:
    void on_output_lb_clicked();
    void on_reset_Pat_pb_clicked();
    void on_ratelimit_Pat_pb_clicked();
    void on_stopchain_Pat_lb_clicked();
    void on_economy_Pat_pb_clicked();
    void on_singlestop_Pat_pb_clicked();
    void on_fastforward_Pat_pb_clicked();
    void on_blowfabric_Pat_pb_clicked();
    void on_alarm_Pat_pb_clicked();
    void on_F4stop_Pat_pb_clicked();
    void on_liftshuttle_Pat_pb_clicked();
    void qtimeSlot();
    void updatepara_slot();

    void on_toMenu_pb_clicked();

    void on_PatternWoven_pb_clicked();
    void on_flatclothing_pb_clicked();
    void on_circulation_pb_clicked();
    void receiveWarning(QString str, int code, int bite);
    void on_pressNeedle_pb_clicked();

    void on_GoGo_pb_clicked();

    void update_keyboard();

    void on_previousPage_pb_clicked();

    void on_outNeedle_pb_clicked();

    void on_inNeedle_pb_clicked();

    void on_nextPage_pb_clicked();

    void on_Yield_pb_clicked();

    void getExpireReminder(bool);

    void getKeyComData();

    void update_MachineID();
    void alarmClear();//????????????
    void autoScroll();
private:
    Ui::PatternWoven *ui;
    WarningPatternwoven *pWarningPatternwoven;
    ParameterSaveInfo *taishazui;
    Hirepurchase *pHirepurchase;
    WIFIMode *wifi;
//    NewOrder *m_pOrder;
    QextSerialPort mycom;
    QDate lastDate;
    QTimer *timer;
    QTimer *timer_paraupdate;
    QDateTime  single;
    DynamicData dynData;//????????????
    quint32 time_t_can;

    bool resetBool;//??????
    bool savelog;
    bool Switch_ratelimit;//????????????
    bool Switch_economy;//??????
    bool Switch_singlestop;//??????
    bool Switch_F4stop;    //F4??????
    bool Switch_fastforward;//??????
    bool Switch_blowfabric;//??????
    bool Switch_alarm;     //??????
    bool Switch_stopchain;//??????
    bool Switch_debug;//????????????
    bool Switch_outNeed;//?????????
    bool Switch_inNeed;//?????????
    bool Switch_oil;//??????
    bool HardwareError;//????????????
    bool sendAllSystemParameter;
    bool IsFirstEnterMain;
    bool motorsetting;
    bool m_bFirst;
    bool bCurrent_Outp_change;
    bool yieldUpdated;//????????????
    bool shuttlestate[3];
    bool simualted;

    int sendingTimeCount;
    int page_id;
    int resetOrNot;
    int timeCheck;
    int cntProgress;//wifi??????????????????4??????????????????
    void initialForm();
    void setStyleSheets();
    void refreshButton();
    void deleteWarningHistory();//???????????????????????????????????????
    void sendAllPara();//???????????????????????????
    void toPatternWoven();
    void statusFeedBack();//????????????
    void readrealstate();//????????????RAM??????????????????
    void shuttlechanged(int num);
    void initialDynamicData();//?????????????????????
    void refreshDynamicData();//??????????????????
    void setButtonState(QPushButton *btn, bool checked);
    void Simulation();

private slots:
    void single_stop_slot();
    void on_luoan_pb_clicked();
    void on_luoanD_pb_clicked();
    void on_BTSR_pb_clicked();
};

#endif // PATTERNWOVEN_H
