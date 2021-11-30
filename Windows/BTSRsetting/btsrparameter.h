#ifndef BTSRPARAMETER_H
#define BTSRPARAMETER_H

#include <QDialog>
#include <QStringList>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class BTSRParameter;
}

class BTSRParameter : public QDialog
{
    Q_OBJECT

public:
    explicit BTSRParameter(int func,QWidget *parent = 0);
    ~BTSRParameter();

private slots:
    void cellclick(int row,int col);

    void on_btn_ok_clicked();

    void on_btn_back_clicked();

    void on_btn_disable_clicked();

    void ButtonVisible();

    void InputEvent();//处理键盘的输入

private:
    Ui::BTSRParameter *ui;
    int function;
    int position;
    int columns;
    int count;
    bool sendflag;
    QTimer *keyboard_timer;
    quint8 btsr_num;
    quint16 btsr_data[100][4];
    quint16 btsr_state[100];
    QString path;
    void initialForm();
    void readFromFile();
    void sendReceive(QString str);
};

#endif // BTSRPARAMETER_H
