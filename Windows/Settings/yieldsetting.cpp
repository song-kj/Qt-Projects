#include "yieldsetting.h"
#include "ui_yieldsetting.h"
#include <QTime>

extern QByteArray temp;

YieldSetting::YieldSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::YieldSetting)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    showData=1;
    digital_mark=0;
    buttun=new QToolButton;
    buttun_pre=new QToolButton;
    buttun_pre=buttun=ui->alloutput_lb;
    timer_output=new QTimer(this);
    connect(timer_output,SIGNAL(timeout()),this,SLOT(update_slot()));
    connect(ui->alloutput_lb,SIGNAL(clicked()),this,SLOT(dealwithclick()));
    connect(ui->completedoutput_lb,SIGNAL(clicked()),this,SLOT(dealwithclick()));
    timer_output->start(100);
}
YieldSetting::~YieldSetting()
{
    delete ui;
}

void YieldSetting::dealwithclick()
{
    buttun= (QToolButton *)sender();
    if(buttun_pre==buttun)
    {
        buttun->setChecked(true);
    }
    else
    {
        buttun_pre->setChecked(false);
        buttun->setChecked(true);
    }
    buttun_pre=buttun;
}

void YieldSetting::on_back_Yie_pb_clicked()
{
    timer_output->stop();
    this->close();//直接退出该界面，不发送数据到编织界面
}

/*确定*/
void YieldSetting::on_ensure_Yie_pb_clicked()
{
    //设定当前产量和总产量并保存到参数文件
    machine_Pars.yield_current=(quint16)ui->completedoutput_lb->text().toInt();
    machine_Pars.yield_setted=(quint16)ui->alloutput_lb->text().toInt();
    para_operation.machine_par_save();
    timer_output->stop();
    Ram.SendData[0]=0x20;
    Ram.SendData[1]=0xA2;
    Ram.SendData[2]=0x06;//产量设置
    Ram.SendData[3]=machine_Pars.yield_setted%256;
    Ram.SendData[4]=machine_Pars.yield_setted/256;
    Ram.SendData[5]=machine_Pars.yield_current%256;
    Ram.SendData[6]=machine_Pars.yield_current/256;
    Ram.SendData[7]=0;
    Ram.SendData[8]=0;
    Ram.SendData[9]=0;
    Ram.UI_to_RTC();
    this->close();
}

/*产量清零*/
void YieldSetting::on_clearoutput_Yie_pb_clicked()
{
    showData=0;
    if(buttun->objectName().contains("completedoutput_lb"))
    {
        ui->completedoutput_lb->setText("0");
    }
    else if(buttun->objectName().contains("alloutput_lb"))
    {
        ui->alloutput_lb->setText("0");
    }

}

void YieldSetting::update_slot()
{
    if(showData==1)
    {
        ui->completedoutput_lb->setText(QString::number(machine_Pars.yield_current));
        ui->alloutput_lb->setText(QString::number(machine_Pars.yield_setted));
    }
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)37)
        {
        }
        if(temp[0]==(char)2||temp[0] == (char)34)
        {
            temp[0]=255;
            on_ensure_Yie_pb_clicked();
        }
        if(temp[0] == (char)3||temp[0] == (char)27)
        {
            temp[0]=255;
            on_clearoutput_Yie_pb_clicked();
        }
        if(temp[0]==(char)4)
        {
            temp[0]=255;
            on_back_Yie_pb_clicked();
        }
        if(temp[0]==(char)28)
        {
            temp[0]=255;
            on_back_Yie_pb_clicked();
        }
        if(temp[0]==(char)35)
        {
            temp[0]=255;
            ui->alloutput_lb->click();
            buttun=ui->alloutput_lb;
        }
        if(temp[0]==(char)33)
        {
            temp[0]=255;
            ui->completedoutput_lb->click();
            buttun=ui->completedoutput_lb;
        }

        QString  number=buttun->text();
        if(temp[0]==(char)27)
        {
            temp[0]=255;
            showData=0;
            buttun->setText("0");
        }
        if(temp[0]==(char)12)
        {
            temp[0]=255;
            showData=0;
            number_answer(number);
            if(number=="0")
            {
                buttun->setText("1");
            }
            else
            {
                buttun->setText(number.append("1"));
            }
        }
        if(temp[0]==(char)13)
        {
            temp[0]=255;
            showData=0;
            number_answer(number);
            if(number=="0")
            {
                buttun->setText("4");
            }
            else
            {
                buttun->setText(number.append("4"));
            }
        }
        if(temp[0]==(char)14)
        {
            temp[0]=255;
            showData=0;
            number_answer(number);
            if(number=="0")
            {
                buttun->setText("7");
            }
            else
            {
                buttun->setText(number.append("7"));
            }
        }
        if(temp[0]==(char)17)
        {
            temp[0]=255;
            showData=0;
            number_answer(number);
            if(number=="0")
            {
                buttun->setText("2");
            }
            else
            {
                buttun->setText(number.append("2"));
            }
        }
        if(temp[0]==(char)18)
        {
            temp[0]=255;
            showData=0;
            number_answer(number);
            if(number=="0")
            {
                buttun->setText("5");
            }
            else
            {
                buttun->setText(number.append("5"));
            }
        }
        if(temp[0]==(char)19)
        {
            temp[0]=255;
            showData=0;
            number_answer(number);
            if(number=="0")
            {
                buttun->setText("8");

            }
            else
            {
                buttun->setText(number.append("8"));
            }
        }
        if(temp[0]==(char)20)
        {
            temp[0]=255;
            showData=0;
            number_answer(number);
            if(number=="0")
            {
                buttun->setText("0");

            }
            else
            {
                buttun->setText(number.append("0"));
            }
        }
        if(temp[0]==(char)22)
        {
            temp[0]=255;
            showData=0;
            number_answer(number);
            if(number=="0")
            {
                buttun->setText("3");
            }
            else
            {
                buttun->setText(number.append("3"));
            }
        }
        if(temp[0]==(char)23)
        {
            temp[0]=255;
            showData=0;
            number_answer(number);
            if(number=="0")
            {
                buttun->setText("6");

            }
            else
            {
                buttun->setText(number.append("6"));
            }
        }
        if(temp[0]==(char)24)
        {
            temp[0]=255;
            showData=0;
            number_answer(number);
            if(number=="0")
            {
                buttun->setText("9");
            }
            else
            {
                buttun->setText(number.append("9"));
            }
        }
    }
}


void YieldSetting::number_answer(QString& n)
{
    if(digital_mark==0)
    {
        n="0";
        digital_mark=1;
    }
}
