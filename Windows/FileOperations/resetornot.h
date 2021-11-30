#ifndef RESETORNOT_H
#define RESETORNOT_H

#include <QDialog>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class ResetOrNot;
}

class ResetOrNot : public QDialog
{
    Q_OBJECT
    
public:
    explicit ResetOrNot(QWidget *parent = 0);
    ~ResetOrNot();
    
private slots:
    void on_ok_pb_clicked();

    void update_slot();

private:
    Ui::ResetOrNot *ui;
    QTimer *pTimer;
};

#endif // RESETORNOT_H
