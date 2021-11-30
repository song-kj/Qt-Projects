#include "btsrstartstudy.h"
#include "ui_btsrstartstudy.h"
#include <QDebug>

extern QByteArray temp;
BTSRStartStudy::BTSRStartStudy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BTSRStartStudy)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->label_3->setText("");
    ui->label_4->setText("");
    autoTest=false;
    autoTestCount=0;
    count=0;
    keyboard_timer = new QTimer(this);
    connect(keyboard_timer,SIGNAL(timeout()),this,SLOT(InputEvent()));
    keyboard_timer->start(10);
}

BTSRStartStudy::~BTSRStartStudy()
{
    delete ui;
}

void BTSRStartStudy::on_pushButton_study_clicked()
{
    ui->label_3->setText("提示：开始学习");
    Ram.SendData[0]=0x70;
    Ram.SendData[1]=0x01;
    Ram.SendData[2]=0x02;
    Ram.SendData[3]=0x00;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
}

void BTSRStartStudy::on_return_btn_clicked()
{
    this->close();
}

void BTSRStartStudy::InputEvent()
{
    if(temp.length()>=1&&this->isActiveWindow())
    {
        if(temp[0]==char(10))
            ui->return_btn->click();
        else if(temp[0]==char(34))//ok
            ui->pushButton_study->click();
        temp[0]=255;
    }
    if(Ram.Receive(0x300)==0x55)
    {
        if(Ram.Receive(0x200)==0x70)
        {
            if(Ram.Receive(0x201)==0x03)
            {
                if(Ram.Receive(0x202)==0x55)
                {
                    Ram.Send(0xaa,0x300);
                    ui->label_3->setText(tr("提示：学习完成"));
                }
                else if(Ram.Receive(0x202)==0xaa)
                {
                    Ram.Send(0xaa,0x300);
                    ui->label_3->setText(tr("提示：学习失败"));
                }
            }
        }
    }
    if(autoTest)
    {
        count++;
        if(count>=50)
        {
            count=0;
            autoTestCount++;
            if(autoTestCount==1)
            {
                on_pushButton_startplustest_clicked();
            }
            else
            {
                on_pushButton_plusstudytest_clicked();
                if(autoTestCount>=11)
                    autoTestCount=0;
            }

        }
    }
}

void BTSRStartStudy::on_pushButton_startplustest_clicked()
{
    ui->label_4->setText("状态：开始脉冲测试");
    Ram.SendData[0]=0x70;
    Ram.SendData[1]=0x01;
    Ram.SendData[2]=0x06;
    Ram.SendData[3]=0x00;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();

}

void BTSRStartStudy::on_pushButton_plusstudytest_clicked()
{
    ui->label_4->setText("状态：脉冲学习测试");
    Ram.SendData[0]=0x70;
    Ram.SendData[1]=0x01;
    Ram.SendData[2]=0x06;
    Ram.SendData[3]=0x01;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
}

void BTSRStartStudy::on_pushButton_stopplustest_clicked()
{
    ui->label_4->setText("状态：结束脉冲测试");
    Ram.SendData[0]=0x70;
    Ram.SendData[1]=0x01;
    Ram.SendData[2]=0x06;
    Ram.SendData[3]=0x02;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
}

void BTSRStartStudy::on_return_autotest_clicked()
{
    autoTest=ui->return_autotest->isChecked();
    autoTestCount=0;
    count=0;
    ui->label_4->setText("");
    if(autoTest)
    {
        ui->groupBox_2->setEnabled(false);
    }
    else
    {
        ui->groupBox_2->setEnabled(true);
    }
}
