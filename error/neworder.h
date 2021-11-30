#ifndef NEWORDER_H
#define NEWORDER_H

#include <QDialog>
#include <QFile>
#include <QTimer>
#include "error/errorcode.h"

namespace Ui {
class NewOrder;
}

class NewOrder : public QDialog
{
    Q_OBJECT

public:
    explicit NewOrder(QWidget *parent = 0);
    ~NewOrder();

    void ShowInfo(const QString &str);

private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

    void step_Slot();
signals:
    void single_stop_signal();
private:
    Ui::NewOrder *ui;
    QTimer *step_Timer;//选择开始新任务需要等到织完当前件时才能停车，次定时器用于判定何时织完当前件。
    ErrorCode *m_Info;
};

#endif // NEWORDER_H
