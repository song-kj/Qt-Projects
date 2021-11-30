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
    void alarmClear();//报警消除
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
    DynamicData dynData;//动态数据
    quint32 time_t_can;

    bool resetBool;//复位
    bool savelog;
    bool Switch_ratelimit;//限速计数
    bool Switch_economy;//节约
    bool Switch_singlestop;//单停
    bool Switch_F4stop;    //F4停车
    bool Switch_fastforward;//快进
    bool Switch_blowfabric;//吹织
    bool Switch_alarm;     //报警
    bool Switch_stopchain;//停链
    bool Switch_debug;//平布调试
    bool Switch_outNeed;//全出针
    bool Switch_inNeed;//全埋针
    bool Switch_oil;//加油
    bool HardwareError;//硬件错误
    bool sendAllSystemParameter;
    bool IsFirstEnterMain;
    bool motorsetting;
    bool m_bFirst;
    bool bCurrent_Outp_change;
    bool yieldUpdated;//产量更新
    bool shuttlestate[3];
    bool simualted;

    int sendingTimeCount;
    int page_id;
    int resetOrNot;
    int timeCheck;
    int cntProgress;//wifi连接状态，为4表示连接成功
    void initialForm();
    void setStyleSheets();
    void refreshButton();
    void deleteWarningHistory();//删除一个月前的报警历史文件
    void sendAllPara();//开机时发送所有参数
    void toPatternWoven();
    void statusFeedBack();//状态反馈
    void readrealstate();//读取双口RAM实时状态数据
    void shuttlechanged(int num);
    void initialDynamicData();//初始化动态数据
    void refreshDynamicData();//刷新动态数据
    void setButtonState(QPushButton *btn, bool checked);
    void Simulation();

private slots:
    void single_stop_slot();
    void on_luoan_pb_clicked();
    void on_luoanD_pb_clicked();
    void on_BTSR_pb_clicked();
};

#endif // PATTERNWOVEN_H
