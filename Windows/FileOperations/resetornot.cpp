#include "resetornot.h"
#include "ui_resetornot.h"

extern QByteArray temp;
ResetOrNot::ResetOrNot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResetOrNot)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->ok_pb->setStyleSheet("QPushButton{border-radius:5;background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0);}");
    pTimer = new QTimer(this);
    connect(pTimer,SIGNAL(timeout()),this,SLOT(update_slot()));
    pTimer->start(10);
}

ResetOrNot::~ResetOrNot()
{
    delete ui;
}

void ResetOrNot::on_ok_pb_clicked()
{
    reject();
}
void ResetOrNot::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)34)
        {
            temp[0]=255;
            on_ok_pb_clicked();
        }
    }
}
