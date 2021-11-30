#ifndef BTSRSETID_H
#define BTSRSETID_H

#include <QDialog>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class BtsrSetID;
}

class BtsrSetID : public QDialog
{
    Q_OBJECT

public:
    explicit BtsrSetID(QWidget *parent = 0);
    ~BtsrSetID();

private slots:
    void on_toolButton_setnum_clicked();

    void on_pushButton_setid_clicked();

    void on_return_btn_clicked();

    void InputEvent();//处理键盘的输入

    void on_toolButton_start_clicked();

    void on_pushButton_setid_stop_clicked();

private:
    Ui::BtsrSetID *ui;
    QTimer *keyboard_timer;
};

#endif // BTSRSETID_H
