#ifndef MOTORSAVEORNOT_H
#define MOTORSAVEORNOT_H

#include <QDialog>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class MotorSaveOrNot;
}

class MotorSaveOrNot : public QDialog
{
    Q_OBJECT
    
public:
    explicit MotorSaveOrNot(QWidget *parent = 0);
    ~MotorSaveOrNot();
    void setLabelText(QString str);

signals:
    void whetherSave(bool);
private slots:
    void on_ok_Btn_clicked();

    void on_cancle_Btn_clicked();
    void update_slot();
private:
    Ui::MotorSaveOrNot *ui;
    QTimer *pTimer;
};

#endif // MOTORSAVEORNOT_H
