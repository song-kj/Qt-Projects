#ifndef WARNINGPATTERNWOVEN_H
#define WARNINGPATTERNWOVEN_H

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QDesktopWidget>
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
#include "QCom/qextserialport.h"

namespace Ui {
class WarningPatternwoven;
}

struct AlarmWifi{
    int code[8];//报警编号
    int value[8];//报警值
};

class WarningPatternwoven : public QWidget
{
    Q_OBJECT

public:
    explicit WarningPatternwoven(QWidget *parent = 0);
    ~WarningPatternwoven();
    bool showtime;

signals:
    void sendAlarm(QString str,int code,int bite);
    void alarmclear();
public slots:
    void showWarningMessage_pv();
    void back_slot();
    void getStopMark(bool);
private:
    Ui::WarningPatternwoven *ui;
    QTimer *timer_warningpv;
    quint8 alarmAddr[29];//报警数据 地址370-384;
    QString alarmName[29][8];
    AlarmWifi alarmwifi[29];
};

#endif // WARNINGPATTERNWOVEN_H
