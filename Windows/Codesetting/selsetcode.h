#ifndef SELSETCODE_H
#define SELSETCODE_H

#include <QDial>
#include <QMessageBox>
#include "mymessagebox.h"
#include "myheader.h"

namespace Ui {
class SelSetCode;
}

class SelSetCode : public QDialog
{
    Q_OBJECT

public:
    explicit SelSetCode(int t, QWidget *parent = 0);
    ~SelSetCode();

private slots:
    void on_pushButton_set_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_sub_clicked();

    void on_pushButton_test_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_back_clicked();

    void on_pushButton_new_clicked();

    void on_pushButton_testnum_clicked();

    void timeoutslot();
    void update_slot();


private:
    Ui::SelSetCode *ui;
    quint16 olddata;
    quint16 newdata,testdata;
    QTimer*time;
    QTimer *ptimer;
    int num;//用于计数
    bool flag;
    QString str;
    quint8 type;
    void setNumber(QString num);
    void initialForm();
};

#endif // SELSETCODE_H
