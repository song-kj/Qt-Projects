#ifndef OUTPUTTESTING_H
#define OUTPUTTESTING_H

#include <QDialog>
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
#include "myheader.h"
#include <QTimer>
namespace Ui {
class OutputTesting;
}

class OutputTesting : public QDialog
{
    Q_OBJECT

public:
    explicit OutputTesting(QWidget *parent = 0);
    ~OutputTesting();
    void number_answer(QString &);

private slots:
    void on_stop_Out_lb_clicked();

    void on_pointmove_Out_pb_clicked();
    void ReceiveData(QString);
    void on_plus_Out_pb_clicked();
    void on_minus_Out_lb_clicked();
    void switch_slot();
    void update_slot();   
    void dealwithclick();
    void xiangjingclicked();
    void on_pushButton_back_clicked();

private:
    Ui::OutputTesting *ui;
    QTimer *timer_port;
    QPushButton *click;
    int sum;
    int times;
    QPushButton *buttun;
    QPushButton *buttun_pre;
    int exchange;
    bool stop;
    bool pointMove;
    int digital_mark;
};

#endif // OUTPUTTESTING_H
