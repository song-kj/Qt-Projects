#ifndef STEPPERMOTORTESTING_H
#define STEPPERMOTORTESTING_H

#include <QDialog>
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
#include "QPushButton"
#include <QLabel>
#include <QDebug>
#include <QString>
#include <QLineEdit>
#include <QTimer>
#include <QMessageBox>
#include "myheader.h"

namespace Ui {
class StepperMotorTesting;
}

class StepperMotorTesting : public QDialog
{
    Q_OBJECT

public:
    explicit StepperMotorTesting(QWidget *parent = 0);
    ~StepperMotorTesting();
    void number_answer(QString &);

public slots:
    void deal_steppermotor();
    void RreceiveData(QString);

private slots:
    void on_speedup_Stp_pb_clicked();

    void on_slowdown_Stp_pb_clicked();

    void on_fullrun_Stp_pb_clicked();

    void on_fullzero_Stp_pb_clicked();

    void on_unitrun_Stp_pb_clicked();

    void on_singlezero_Stp_pb_clicked();

    void on_back_Stp_pb_clicked();

    void update_slot();

    void on_speedsetting_pb_clicked();

    void on_revwesal_Stp_pb_clicked();

private:
    Ui::StepperMotorTesting *ui;
    QLabel *motor_edit[9];
    int y;
    QPushButton *steppermotor_1_lb[9];
    int mark;
    QPushButton *label;
    QPushButton *display_lb;
    int editstate;
    int display;
    int sum_up;
    int mark_up;
    QPushButton *dislabel_up;
    bool trigger;
    int sum_down;
    int mark_down;
    QPushButton *dislabel_down;
    int sum_whole;
    QTimer *timer_stepper;
    int speed_interval;
    bool revwesal;
    int digital_mark;

};

#endif // STEPPERMOTORTESTING_H
