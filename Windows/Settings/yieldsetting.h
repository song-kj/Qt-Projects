#ifndef YIELDSETTING_H
#define YIELDSETTING_H

#include <QDialog>
#include <QString>
#include <QTimer>
#include <QFile>
#include <QMessageBox>
#include <QDataStream>
#include <QToolButton>
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
#include "myheader.h"
#define ZERO "/xrfile/ZERO.er"
#define ParaLocation "/xrfile/Parameter.er"
namespace Ui {
class YieldSetting;
}

class YieldSetting : public QDialog
{
    Q_OBJECT

public:
    explicit YieldSetting(QWidget *parent = 0);
    ~YieldSetting();
     void number_answer(QString &);

private slots:
    void on_back_Yie_pb_clicked();
    void on_ensure_Yie_pb_clicked();
    void on_clearoutput_Yie_pb_clicked();
    void update_slot();
    void dealwithclick();

private:
    Ui::YieldSetting *ui;
    QTimer *timer_output;
    QToolButton *buttun;
    QToolButton *buttun_pre;
    int showData;
    int digital_mark;
};

#endif // YIELDSETTING_H
