#ifndef RUNNINGINSETTING_H
#define RUNNINGINSETTING_H

#include <QDialog>
#include <QTimer>
#include <QToolButton>
#include "myheader.h"

namespace Ui {
class RunningInSetting;
}

class RunningInSetting : public QDialog
{
    Q_OBJECT

public:
    explicit RunningInSetting(QWidget *parent = 0);
    ~RunningInSetting();

private slots:
    void on_pushButton_back_clicked();

    void timeout();
    void on_pushButton_save_clicked();

    void on_pushButton_start_clicked();

    void toolbuttonclicked();

    void on_tlb_heatflag_clicked();

private:
    Ui::RunningInSetting *ui;
    QTimer *timer1;
    QToolButton *curbutton;
    void initialForm();
    void nunmberInput(QString str);
};

#endif // RUNNINGINSETTING_H
