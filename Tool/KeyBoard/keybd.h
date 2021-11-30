#ifndef KEYBD_H
#define KEYBD_H
#include <QDesktopWidget>
#include <QDialog>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class KeyBd;
}

class KeyBd : public QDialog
{
    Q_OBJECT
    
public:
    explicit KeyBd(QWidget *parent = 0);
    QString inputstr;
    ~KeyBd();
signals:
      void  sendData(QString);
private slots:
    void on_pb_ensure_clicked();

    void on_pb_close_clicked();
    void update_slot();

private:
    Ui::KeyBd *ui;
    QString string;
    int number;
    QString str;
    QTimer *ptime;
    QPushButton *digital;
};

#endif // KEYBD_H
