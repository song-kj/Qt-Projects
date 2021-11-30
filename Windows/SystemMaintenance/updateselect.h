#ifndef UPDATESELECT_H
#define UPDATESELECT_H

#include <QDialog>
#include <QTimer>
#include <QPushButton>
#include "Windows/SystemMaintenance/controlui.h"
#include "Windows/SystemMaintenance/ui.h"
#include "myheader.h"

namespace Ui {
class updateSelect;
}

class updateSelect : public QDialog
{
    Q_OBJECT 
public:
    explicit updateSelect(QWidget *parent = 0);
    ~updateSelect();
    
private slots:
    void on_back_pb_clicked();
    void dealwithclick();
    void update_timer();

private:
    Ui::updateSelect *ui;
    QPushButton *buttun;
    QTimer *timer_update;
    UI *pUI;
};

#endif // UPDATESELECT_H
