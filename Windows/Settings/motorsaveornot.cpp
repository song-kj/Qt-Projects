#include "motorsaveornot.h"
#include "ui_motorsaveornot.h"

extern QByteArray temp;
MotorSaveOrNot::MotorSaveOrNot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MotorSaveOrNot)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    pTimer=new QTimer(this);
    connect(pTimer, SIGNAL(timeout()),this,SLOT(update_slot()));
    pTimer->start(100);
}

MotorSaveOrNot::~MotorSaveOrNot()
{
    delete ui;
}

void MotorSaveOrNot::setLabelText(QString str)
{
    ui->label->setText(str);
}

/*保存电机参数*/
void MotorSaveOrNot::on_ok_Btn_clicked()
{
    this->accept();
}

/*不保存*/
void MotorSaveOrNot::on_cancle_Btn_clicked()
{
    this->reject();
}

/*键盘定时器*/
void MotorSaveOrNot::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>0)
    {
        if(temp[0]==(char)28)
        {
            temp[0]=255;
            ui->cancle_Btn->click();
        }
        else if(temp[0]==(char)34)
        {
            temp[0]=255;
            ui->ok_Btn->click();
        }
    }
}
