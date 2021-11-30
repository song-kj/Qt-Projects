#ifndef ZEROLEVELSETTING_H
#define ZEROLEVELSETTING_H

#include <QDialog>
#include<QTimer>
#include<QFile>
#include <QMessageBox>
#include "myheader.h"
#define ZERO "/xrfile/ZERO.er"
#define ParaLocation "/xrfile/Parameter.er"
namespace Ui {
class ZeroLevelSetting;
}

class ZeroLevelSetting : public QDialog
{
    Q_OBJECT

public:
    explicit ZeroLevelSetting(QWidget *parent = 0);
    ~ZeroLevelSetting();

signals:

private slots:
    void on_back_Zero_pb_clicked();
    void zero_slot();
    void on_auto_Zero_pb_clicked();

    void on_save_pb_clicked();

    void on_sumPluseSetting_btn_clicked();

    void on_zeroNeedleSetting_btn_clicked();

    void on_pushbutton_set_clicked();

    void on_auto_Zero_pb_2_clicked();

    void on_btn_fixZeroOffset_clicked();

private:
    Ui::ZeroLevelSetting *ui;
    QTimer *timer_zero;
};

#endif // ZEROLEVELSETTING_H
