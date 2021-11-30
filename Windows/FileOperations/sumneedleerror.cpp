#include "sumneedleerror.h"
#include "ui_sumneedleerror.h"

extern QByteArray temp;
SumNeedleError::SumNeedleError(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SumNeedleError)
{
    ui->setupUi(this);
     this->setWindowFlags(Qt::FramelessWindowHint);
    ui->ok_pb->setStyleSheet("QPushButton{border-radius:5;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 255, 0);}");
    pQTimer=new QTimer(this);
    pQTimer->start(10);
    connect(pQTimer, SIGNAL(timeout()),this,SLOT(update_slot()));
}

SumNeedleError::~SumNeedleError()
{
    delete ui;
}

void SumNeedleError::settext(QString str)
{
    ui->label->setText(str);
}

void SumNeedleError::on_ok_pb_clicked()
{
    reject();
}
void SumNeedleError::update_slot()
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
