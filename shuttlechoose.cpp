#include "shuttlechoose.h"
#include "ui_shuttlechoose.h"

extern QByteArray temp;
ShuttleChoose::ShuttleChoose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShuttleChoose)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setGeometry(120,240,381,181);
    timer_shuttle=new QTimer(this);
    timer_shuttle->start(100);
    connect(timer_shuttle,SIGNAL(timeout()),this,SLOT(update_slot()));
    connect(ui->one,SIGNAL(clicked()),this,SLOT(shuttle_slot()));
    connect(ui->two,SIGNAL(clicked()),this,SLOT(shuttle_slot()));
    connect(ui->three,SIGNAL(clicked()),this,SLOT(shuttle_slot()));

    buttun=new QPushButton;
}

ShuttleChoose::~ShuttleChoose()
{
    delete ui;
}

void ShuttleChoose::setShuttleState(int num)
{
    if(num==1)
        ui->one->setChecked(true);
    else if(num==2)
        ui->two->setChecked(true);
    else
        ui->three->setChecked(true);
}

void ShuttleChoose::shuttle_slot()
{
    buttun=(QPushButton*)sender();
    shuttleState(buttun->toolTip().toInt()-1);
}

void ShuttleChoose::shuttleState(int state)
{
    if(checkInterval()==false)
        return;
    QString msg=tr("纱嘴");
    if(state==0)
        msg+="1、2、3";
    else if(state==1)
        msg+="4、5、6";
    else
        msg+="7、8";
    if(buttun->isChecked())
    {
        msg+=tr("抬起");
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x0A;
        Ram.SendData[3]=state+1;//梭子路数1-8
        Ram.SendData[4]=0x55;//抬起
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else
    {
        msg+=tr("放下");
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x0A;
        Ram.SendData[3]=state+1;//梭子路数1-8
        Ram.SendData[4]=0xAA;//放下
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    para_operation.saveSystemLog(msg,QDateTime::currentDateTime());
}

void ShuttleChoose::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)37)
        {

        }
        else if(temp[0]==(char)12||temp[0]==(char)74)
        {
            temp[0]=255;
            ui->one->click();
        }
        else if(temp[0]==(char)17||temp[0]==(char)79)
        {
            temp[0]=255;
            ui->two->click();
        }
        else if(temp[0]==(char)22||temp[0]==(char)84)
        {
            ui->three->click();
            temp[0]=255;
        }
        else if(temp[0]==(char)34)
        {
            temp[0]=255;
            ui->OK_btn->click();
        }
        else if(temp[0] ==(char)34)
        {
            temp[0] =255;
            buttun->click();
        }
    }
}

void ShuttleChoose::changeColor(int tip)
{
    switch (tip) {
        case 1:ui->one->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:22px;color:red;border:noborder}");
               ui->two->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:22px;color:rgb(255, 255, 0);border:noborder}");
               ui->three->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:22px;color:rgb(255, 255, 0);border:noborder}");
            break;
        case 2:ui->one->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:22px;color:rgb(255, 255, 0);border:noborder}");
               ui->two->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:22px;color:red;border:noborder}");
               ui->three->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:22px;color:rgb(255, 255, 0);border:noborder}");
            break;
        case 3:ui->one->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:22px;color:rgb(255, 255, 0);border:noborder}");
               ui->two->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:22px;color:rgb(255, 255, 0);border:noborder}");
               ui->three->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:22px;color:red;border:noborder}");
            break;
        default:
            break;
        }
}

void ShuttleChoose::on_OK_btn_clicked()
{
    if(checkInterval()==false)
                return;
    Ram.SendData[0]=0x20;
    Ram.SendData[1]=0xA1;
    Ram.SendData[2]=0x0A;
    Ram.SendData[3]=0xAA;//状态回归
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    emit closeShift();
    close();
}
