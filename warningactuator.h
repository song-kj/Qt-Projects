#ifndef WARNINGACTUATOR_H
#define WARNINGACTUATOR_H

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include "Communication/doubleportram.h"
#include <QDesktopWidget>
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
namespace Ui {
class WarningActuator;
}

class WarningActuator : public QWidget
{
    Q_OBJECT

public:
    explicit WarningActuator(QWidget *parent = 0);
    ~WarningActuator();
signals:
    void sendActuatorWarning(bool);
public slots:
    void warning_act_slot();
    void back_slot();
private:
    Ui::WarningActuator *ui;
    QTimer *timer_actuator;
    DoublePortRam *Ram_warning_act;
};

#endif // WARNINGACTUATOR_H
