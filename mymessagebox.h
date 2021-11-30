#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H

#include <QDialog>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class MyMessageBox;
}

class MyMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit MyMessageBox(QString title,QString str,int type,QWidget *parent = 0);
    ~MyMessageBox();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void update_slot();

private:
    Ui::MyMessageBox *ui;
    QTimer *pTimer;
};

#endif // MYMESSAGEBOX_H
