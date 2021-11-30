#ifndef TIMESETTING_H
#define TIMESETTING_H

#include <QDialog>
#include <QPushButton>
#include "stdlib.h"
#include "time.h"
#include <QTimer>
#include "myheader.h"

namespace Ui {
class TimeSetting;
}

class TimeSetting : public QDialog
{
    Q_OBJECT
    
public:
    explicit TimeSetting(QWidget *parent = 0);
    ~TimeSetting();
    void curTime();
public slots:
    void listeningClicked();
    void number_answer(QString&);
    
private slots:
    void on_sure_btn_clicked();

    void on_cancle_btn_clicked();

    void getKeyBoardData(QString);

    void update_slot();

private:
    Ui::TimeSetting *ui;
    QPushButton *btn;
    QTimer *ptimer;
    int digital_mark;
    QString str;
    bool showBool;
};

#endif // TIMESETTING_H
