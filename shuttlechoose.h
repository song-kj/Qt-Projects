#ifndef SHUTTLECHOOSE_H
#define SHUTTLECHOOSE_H

#include <QDialog>
#include <QPushButton>
#include <QStyleFactory>
#include <QTimer>
#include <QLabel>
#include <QDesktopWidget>
#include "myheader.h"

namespace Ui {
class ShuttleChoose;
}

class ShuttleChoose : public QDialog
{
    Q_OBJECT
    
public:
    explicit ShuttleChoose(QWidget *parent = 0);
    ~ShuttleChoose();
    void setShuttleState(int num);
    void shuttleState(int);
    void changeColor(int);
public slots:
    void update_slot();
    void shuttle_slot();

private slots:
    void on_OK_btn_clicked();
signals:
    void closeShift();
private:
    Ui::ShuttleChoose *ui;
    QPushButton * buttun;
    QTimer *timer_shuttle;
};

#endif // SHUTTLECHOOSE_H
