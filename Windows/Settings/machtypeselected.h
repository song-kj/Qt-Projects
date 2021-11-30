#ifndef MACHTYPESELECTED_H
#define MACHTYPESELECTED_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class MachTypeSelected;
}

class MachTypeSelected : public QDialog
{
    Q_OBJECT
    
public:
    explicit MachTypeSelected(QWidget *parent = 0);
    ~MachTypeSelected();

public slots:
    void dealwithclick();
    void update_slot();

signals:
    void sendNo(int, QString);
private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::MachTypeSelected *ui;
    int curIndex;
    int totalMachs;
    QPushButton *machTypeButton[18];
    QTimer *timer_type;
    void initialForm();

};

#endif // MACHTYPESELECTED_H
