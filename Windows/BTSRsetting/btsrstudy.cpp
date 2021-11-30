#include "btsrstudy.h"
#include "ui_btsrstudy.h"
#include <QDebug>

extern QByteArray temp;

BTSRStudy::BTSRStudy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BTSRStudy)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->radioButton->setChecked(true);
    keyboard_timer = new QTimer(this);
    connect(keyboard_timer,SIGNAL(timeout()),this,SLOT(InputEvent()));
    keyboard_timer->start(10);
}

BTSRStudy::~BTSRStudy()
{
    delete ui;
}

//确定按钮槽函数 发送功能代码指令
void BTSRStudy::on_btn_ok_clicked()
{
    quint8 p1=0x00,p2=0x00;
    if(ui->radioButton->isChecked())
        p1=0x00;
    if(ui->radioButton_2->isChecked())
        p1=0x01;
    if(ui->radioButton_3->isChecked())
        p1=0x02;
    if(ui->checkBox->isChecked())
        p2=0x01;
    else
        p2=0x00;
    if(ui->checkBox_2->isChecked())
        p2|=0x02;

    Ram.SendData[0]=0x70;
    Ram.SendData[1]=0x01;
    Ram.SendData[2]=0x04;
    Ram.SendData[3]=p1;
    Ram.SendData[4]=p2;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
}

//返回
void BTSRStudy::on_return_btn_clicked()
{
    this->close();
}

void BTSRStudy::InputEvent()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==char(9))
        {
            temp[0]=255;
            ui->btn_ok->click();
        }
        else if(temp[0]==char(10))
        {
            temp[0]=255;
            ui->return_btn->click();
        }
    }
    if(Ram.Receive(0x300)==0x55)
    {
        if(Ram.Receive(0x200)==0x70)
        {
            if(Ram.Receive(0x201)==0x01)
            {
                if(Ram.Receive(0x202)==0x05)
                {
                    if(Ram.Receive(0x203)==0x55)
                    {
                        Ram.Send(0xaa,0x300);
                        ui->btn_ok->setText(tr("设/n定/n成/n功"));
                    }
                    else if(Ram.Receive(0x203)==0xaa)
                    {
                        Ram.Send(0xaa,0x300);
                        ui->btn_ok->setText(tr("设/n定/n失/n败"));
                    }
                    ui->btn_ok->setStyleSheet("border-radius:10;background-color: rgb(100, 100, 100);font-size:22px;color:rgb(255, 0, 0);");
                    QTimer::singleShot(1000,this,SLOT(ButtonVisible()));
                }
            }
        }
    }
}

void BTSRStudy::ButtonVisible()
{
    ui->btn_ok->setText(tr("确/n定"));
    ui->btn_ok->setStyleSheet("border-radius:10;background-color: rgb(100, 100, 100);font-size:22px;color:rgb(255, 255, 0);");
}
