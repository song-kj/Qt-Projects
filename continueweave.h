#ifndef CONTINUEWEAVE_H
#define CONTINUEWEAVE_H

#include <QDialog>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class ContinueWeave;
}

class ContinueWeave : public QDialog
{
    Q_OBJECT
    
public:
    explicit ContinueWeave(QWidget *parent = 0);
    ~ContinueWeave();
     bool flag;
     int count;
     void initSys();
    
private slots:
    void on_ok_btn_clicked();

    void on_cancel_btn_clicked();

   void update_slot();

private:
    Ui::ContinueWeave *ui;
    QTimer *pTimer;
};

#endif // CONTINUEWEAVE_H
