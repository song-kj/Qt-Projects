#ifndef SUMNEEDLEERROR_H
#define SUMNEEDLEERROR_H

#include <QDialog>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class SumNeedleError;
}

class SumNeedleError : public QDialog
{
    Q_OBJECT
    
public:
    explicit SumNeedleError(QWidget *parent = 0);
    ~SumNeedleError();
    void settext(QString str);
    
private slots:
    void on_ok_pb_clicked();
    void update_slot();

private:
    Ui::SumNeedleError *ui;
    QTimer *pQTimer;
};

#endif // SUMNEEDLEERROR_H
