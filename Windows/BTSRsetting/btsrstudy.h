#ifndef BTSRSTUDY_H
#define BTSRSTUDY_H

#include <QDialog>
#include <QTimer>
#include <QButtonGroup>
#include "myheader.h"

struct Eye_Sensor//电子眼
{
    quint8 number;
    quint8 version;
    quint8 sensitivity;
    quint8 delay;
    quint8 frequency_start;
    quint8 frequency_end;
    quint16 broke;
    quint16 uncut;
    quint8 hspeed;
    quint8 prx;
    quint8 device_type;
};

namespace Ui {
class BTSRStudy;
}

class BTSRStudy : public QDialog
{
    Q_OBJECT

public:
    explicit BTSRStudy(QWidget *parent = 0);
    ~BTSRStudy();

private slots:
    void on_btn_ok_clicked();

    void on_return_btn_clicked();

    void InputEvent();//处理键盘的输入

    void ButtonVisible();
private:
    Ui::BTSRStudy *ui;
    Eye_Sensor sysSensor;
    QTimer *keyboard_timer;
};

#endif // BTSRSTUDY_H
