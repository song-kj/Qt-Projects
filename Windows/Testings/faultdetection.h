#ifndef FAULTDETECTION_H
#define FAULTDETECTION_H

#include <QDialog>
#include <QToolButton>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class FaultDetection;
}

class FaultDetection : public QDialog
{
    Q_OBJECT

public:
    explicit FaultDetection(QWidget *parent = 0);
    ~FaultDetection();

private slots:
    void on_pushButton_back_clicked();
    void timeout();

    void on_pushButton_detection_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_sub_clicked();

private:
    Ui::FaultDetection *ui;
    QToolButton *tlb[16];
    quint8 feeder;//驱动板号
    quint16 points;//故障点位
    bool detecting;
    int timecount;
    QTimer *timer1;
    void initialForm();
    void refreshForm();
};

#endif // FAULTDETECTION_H
