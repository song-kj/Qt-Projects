#include "continueweave.h"
#include "ui_continueweave.h"

extern QByteArray temp;
ContinueWeave::ContinueWeave(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContinueWeave)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->label->setStyleSheet("font: 34px ;color: rgb(255, 0, 0);");
    ui->ok_btn->setStyleSheet("QPushButton{border-radius:5;background-color: rgb(100, 100, 100);font-size:22px;color:white}");
    ui->cancel_btn->setStyleSheet("QPushButton{border-radius:5;background-color: rgb(100, 100, 100);font-size:22px;color:white}");
    pTimer = new QTimer(this);
    connect(pTimer,SIGNAL(timeout()),this,SLOT(update_slot()));
    pTimer->start(10);
    count  = 0;
}

ContinueWeave::~ContinueWeave()
{
    delete ui;
}
void ContinueWeave::update_slot()
{
    if(!ui->ok_btn->isVisible())
    {
        count ++;
        if(count  == 220)
        {
            reject();
        }
    }
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)34)
        {
            temp[0]=255;
            on_ok_btn_clicked();
        }
        else if(temp[0]==(char)28)
        {
            temp[0]=255;
            on_cancel_btn_clicked();
        }
    }
}
void ContinueWeave::on_ok_btn_clicked()
{
    Ram.SendData[0]=0x20;
    Ram.SendData[1]=0xA3;
    Ram.SendData[2]=0x03;
    Ram.SendData[3]=0x01;//CANCEL
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    initSys();
}

void ContinueWeave::on_cancel_btn_clicked()
{
    Ram.SendData[0]=0x20;
    Ram.SendData[1]=0xA3;
    Ram.SendData[2]=0x03;
    Ram.SendData[3]=0x02;//CANCEL
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    initSys();
}

void ContinueWeave::initSys()
 {
    count=0;
     ui->ok_btn->setVisible(false);
     ui->cancel_btn->setVisible(false);
     ui->label->setAlignment(Qt::AlignCenter);
     ui->label->setGeometry(0,60,491,41);
     ui->label->setText("系统正在初始化，请稍后...");
 }
