#ifndef PARAOUTIN_H
#define PARAOUTIN_H

#include <QDialog>
#include <QTimer>
#include <QScrollBar>
#include <QTableWidgetItem>
#include "myheader.h"

namespace Ui {
class ParaOutIn;
}

class ParaOutIn : public QDialog
{
    Q_OBJECT
    
public:
    explicit ParaOutIn(QWidget *parent = 0);
    ~ParaOutIn();
    
private slots:
    void on_Out_pb_clicked();

    void on_In_pb_clicked();

    void update_slot();

    void on_back_pb_clicked();

    void on_delede_pb_clicked();

private:
    Ui::ParaOutIn *ui;
    QTimer *timer_paraOutIn;
    QTableWidget *table;
    void  showUSB();
    bool isfirstEnter;
    void initialForm();
};

#endif // PARAOUTIN_H
