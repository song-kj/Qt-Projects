#ifndef FLATCLOTH_H
#define FLATCLOTH_H

#include <QDialog>
#include<QPushButton>
#include <QLabel>
#include <QTimer>
#include <QStyleFactory>
#include "Communication/doubleportram.h"
#include "myheader.h"

namespace Ui {
class FlatCloth;
}

class FlatCloth : public QDialog
{
    Q_OBJECT
    
public:
    explicit FlatCloth(QWidget *parent = 0);
    ~FlatCloth();
signals:
    void sendID(int);
private slots:
    void on_ok_pb_clicked();
    void dealwithClick();
    void update_slot();

private:
    Ui::FlatCloth *ui;
    QPushButton *buttun;
    int flat[6];
    int ID;
    bool flar_bool[6];
    QTimer *timer_flatcloth;
};

#endif // FLATCLOTH_H
