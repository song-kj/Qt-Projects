#ifndef PARAMETERSETTING_H
#define PARAMETERSETTING_H

#include <QDialog>
#include <QString>
#include <QDataStream>
#include <QPushButton>
#include <QFile>
#include <QLabel>
#include <QTimer>
#include <QProgressBar>
#include <QTextStream>
#include <QMessageBox>
#include "Tool/KeyBoard/keybd.h"
#include "Windows/Settings/paraoutin.h"
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
#include "Windows/Settings/machtypeselected.h"
#include "error/errorcode.h"
#include "myheader.h"

namespace Ui {
class ParameterSetting;
}

class ParameterSetting : public QDialog
{
    Q_OBJECT

public:
    explicit ParameterSetting(QWidget *parent = 0);
    ~ParameterSetting();
    void ReadFromFile();
    void HideParaSetting();
    void showdigital();
    void number_answer(QString &);

signals:

private slots:
    void on_back_Pra_pb_clicked();
    void update_slot();
    void dealwithclicked();
    void dealwithKey();
    void on_copy_pb_clicked();
    void on_save_pb_clicked();
    void on_btn_runinsetting_clicked();

    void on_FixedNeedle_pb_clicked();

private:
    Ui::ParameterSetting *ui;
    QTimer *timer_parasetting;
    QPushButton *button;
    QPushButton *button_pre;
    QString password;
    quint16 degree_12[16];
    quint16 degree_13[16];
    quint16 degree_14[16];
    quint16 degree_15[16];
    quint16 degree_16[16];
    quint16 degree_17[16];
    quint16 degree_18[16];
    quint16 degree_19[16];
    quint16 degree_20[16];
    int digital_mark;
    void initialForm();
    void dealActuatorPosition();
};

#endif // PARAMETERSETTING_H
