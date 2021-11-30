#ifndef NOANSWER_H
#define NOANSWER_H

#include <QDialog>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class Noanswer;
}

class Noanswer : public QDialog
{
    Q_OBJECT
    
public:
    explicit Noanswer(QWidget *parent = 0);
    ~Noanswer();
    
private slots:
    void on_sure_pb_clicked();
    void update_slot();

private:
    Ui::Noanswer *ui;
    QTimer *timer_noanswer;
};

#endif // NOANSWER_H
