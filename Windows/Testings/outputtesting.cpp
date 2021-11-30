#include "outputtesting.h"
#include "ui_outputtesting.h"
#include <QLabel>
#include "mymessagebox.h"
#include <QDebug>

extern QByteArray temp;
OutputTesting::OutputTesting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OutputTesting)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->mesg_lb->setStyleSheet("QLabel{font-size:16px;color: red}");
    timer_port=new QTimer(this);
    connect(timer_port, SIGNAL(timeout()), this, SLOT(update_slot()));
    timer_port->start(10);
    click=new QPushButton();
    buttun=new QPushButton();
    buttun_pre=new QPushButton();
    buttun=ui->speed_lb;
    click=ui->redlight_pb;
    exchange=0;
    times=0;
    digital_mark=0;
    stop=true;
    pointMove=true;
    ui->groupBox->setStyleSheet("QGroupBox{border:1px solid red;font-size:22px;color:green}");
    ui->groupBox_2->setStyleSheet("QGroupBox{border:1px solid white;font-size:22px;color:green}");
    ui->groupBox_3->setStyleSheet("QGroupBox{border:1px solid white;font-size:22px;color:green}"
                                  "QPushButton{background-color:transparent; color:white;font-size:20px;border:none;font:bold;}");

    ui->speed_lb->setStyleSheet("QPushButton{border-style:outset;background: transparent;font:bold;font-size:20px;color:red}");
    ui->plusandminus_lb->setStyleSheet("QPushButton{border-style:outset;background: transparent;font:bold;font-size:20px;color:blue}");
    ui->plus_Out_pb->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font:bold;font-size:18px;color:white}");
    ui->minus_Out_lb->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font:bold;font-size:18px;color:white}");
    ui->pointmove_Out_pb->setStyleSheet("QPushButton{border-radius:10;font-size:18px;color:rgb(255, 255, 0);;background-color: rgb(100, 100, 100)}");
    ui->stop_Out_lb->setStyleSheet("QPushButton{border-radius:10;font-size:18px;color:rgb(255, 255, 0);;background-color: rgb(100, 100, 100)}");
    ui->needle_lb->setStyleSheet("QPushButton{background-color: rgb(160, 160, 160);font:bold;font-size:18px;color:white;}");
    ui->plusandminus_lb->setText("0");

    ui->redlight_pb->setText(QString::fromUtf8("关"));
    ui->greenlight_pb->setText(QString::fromUtf8("关"));
    ui->yellowlight_pb->setText(QString::fromUtf8("关"));
    ui->needlelight_pb->setText(QString::fromUtf8("关"));

    ui->redlight_pb->setStyleSheet("QPushButton{border:2px solid green;font:bold;font-size:18px;color:red;background-color: rgb(255, 255, 127)}");
    ui->greenlight_pb->setStyleSheet("QPushButton{border:2px solid green;font:bold;font-size:18px;color:red;background-color: rgb(255, 255, 127)}");
    ui->yellowlight_pb->setStyleSheet("QPushButton{border:2px solid green;font:bold;font-size:18px;color:red;background-color: rgb(255, 255, 127)}");
    ui->needlelight_pb->setStyleSheet("QPushButton{border:2px solid green;font:bold;font-size:18px;color:red;background-color: rgb(255, 255, 127)}");

    ui->red_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
    ui->green_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
    ui->yellow_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
    ui->needle_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
    connect(ui->redlight_pb, SIGNAL(clicked()), this, SLOT(switch_slot()));
    connect(ui->greenlight_pb, SIGNAL(clicked()), this, SLOT(switch_slot()));
    connect(ui->yellowlight_pb, SIGNAL(clicked()), this, SLOT(switch_slot()));
    connect(ui->needlelight_pb, SIGNAL(clicked()), this, SLOT(switch_slot()));
    connect(ui->xiangjing_1,SIGNAL(clicked(bool)),this,SLOT(xiangjingclicked()));
    connect(ui->xiangjing_2,SIGNAL(clicked(bool)),this,SLOT(xiangjingclicked()));
    connect(ui->xiangjing_3,SIGNAL(clicked(bool)),this,SLOT(xiangjingclicked()));
    connect(ui->xiangjing_4,SIGNAL(clicked(bool)),this,SLOT(xiangjingclicked()));
    connect(ui->xiangjing_5,SIGNAL(clicked(bool)),this,SLOT(xiangjingclicked()));
    connect(ui->xiangjing_6,SIGNAL(clicked(bool)),this,SLOT(xiangjingclicked()));
    connect(ui->xiangjing_7,SIGNAL(clicked(bool)),this,SLOT(xiangjingclicked()));
    connect(ui->xiangjing_8,SIGNAL(clicked(bool)),this,SLOT(xiangjingclicked()));
    sum=0;
}

OutputTesting::~OutputTesting()
{
    delete ui;
}
/*停止*/
void OutputTesting::on_stop_Out_lb_clicked()
{
    if(stop)
    {
        ui->stop_Out_lb->setStyleSheet("QPushButton{border-radius:10;font-size:18px;color:red;background-color: rgb(100, 100, 100)}");
        stop=false;
        Ram.SendData[0]=0x16;
        Ram.SendData[1]=0xA2;
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
    else
    {
        ui->stop_Out_lb->setStyleSheet("QPushButton{border-radius:10;font-size:18px;color:rgb(255, 255, 0);;background-color: rgb(100, 100, 100)}");
        stop=true;
    }


}
/*点动*/
void OutputTesting::on_pointmove_Out_pb_clicked()
{
    if(pointMove)
    {
        ui->pointmove_Out_pb->setStyleSheet("QPushButton{border-radius:10;font-size:18px;color:red;background-color: rgb(100, 100, 100)}");
        pointMove=false;
        Ram.SendData[0]=0x16;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x01;
        unsigned char speed =ui->speed_lb->text().toInt();
        Ram.SendData[3]=speed;//0-100R/min
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else
    {
        ui->pointmove_Out_pb->setStyleSheet("QPushButton{border-radius:10;font-size:18px;color:rgb(255, 255, 0);;background-color: rgb(100, 100, 100)}");
        pointMove=true;
    }

}
void  OutputTesting::dealwithclick()
{
    buttun=(QPushButton*)sender();
    if(buttun_pre==buttun)
    {
        buttun->setFocus();
        buttun->setStyleSheet("QPushButton{border-style:outset;background: transparent;font:bold;font-size:20px;color:red}");
    }
    else
    {
        buttun->setFocus();
        buttun_pre->setStyleSheet("QPushButton{border-style:outset;background: transparent;font:bold;font-size:18px;color:blue}");
        buttun->setStyleSheet("QPushButton{border-style:outset;background: transparent;font:bold;font-size:20px;color:red}");
    }
    buttun_pre=buttun;
}

void OutputTesting::xiangjingclicked()
{
    QPushButton *btn=(QPushButton*)sender();
    int index=btn->objectName().mid(10,1).toInt();
    if(btn->isChecked())
        btn->setText("开");
    else
        btn->setText("关");
    Ram.SendData[0]=0x16;
    Ram.SendData[1]=0xa1;
    Ram.SendData[2]=quint8(index+4);
    Ram.SendData[3]=btn->isChecked()?0x55:0xaa;
    Ram.UI_to_RTC();
    qDebug()<<btn->text()<<Ram.SendData[0]<<Ram.SendData[1]<<Ram.SendData[2]<<Ram.SendData[3];
}

/*显示接收来的数据*/
void OutputTesting::ReceiveData(QString num)
{
    if(buttun->toolTip().toInt()==6)
    {
        ui->plusandminus_lb->setText(num);
    }
    if(buttun->toolTip().toInt()==5)
    {
        if(num.toInt()<101)
        {
            ui->speed_lb->setText(num);
        }
    }
}
/*加*/
void OutputTesting::on_plus_Out_pb_clicked()
{
    int plus=ui->speed_lb->text().toInt();
    plus=plus+ui->plusandminus_lb->text().toInt();
    if(plus<101)
    {
        ui->speed_lb->setText(QString::number(plus));
    }
    else
    {
        plus=100;
        ui->speed_lb->setText(QString::number(plus));
    }
}
/*减*/
void OutputTesting::on_minus_Out_lb_clicked()
{
    int minus=ui->speed_lb->text().toInt();
    minus=minus-ui->plusandminus_lb->text().toInt();
    if(minus>0)
    {
        ui->speed_lb->setText(QString::number(minus));
    }
    else
    {
        minus=0;
        ui->speed_lb->setText(QString::number(minus));
    }
}
/*处理点击开关槽函数*/
void OutputTesting::switch_slot()
{
    click=(QPushButton*)sender();
    if(click->toolTip().toInt()==1)
    {
        Ram.SendData[2]=0x01;
        if(click->text()==QString::fromUtf8("关"))
        {
            Ram.SendData[3]=0x55;
            click->setText(QString::fromUtf8("开"));
        }

        else
        {
            Ram.SendData[3]=0xAA;
            click->setText(QString::fromUtf8("关"));
        }
        ui->n_lb->setText("");
        ui->r_lb->setText("| ");
        ui->h_lb->setText(" ");
        ui->g_lb->setText(" ");
        click=ui->redlight_pb;

        click->setStyleSheet("QPushButton{border:2px solid green;font:bold;font-size:18px;color:red;background-color: rgb(255, 255, 127)}");
        ui->red_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: red;border:noborder;}");
        ui->green_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
        ui->yellow_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
        ui->needle_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");

    }
    if(click->toolTip().toInt()==2)
    {
        Ram.SendData[2]=0x02;
        if(click->text()==QString::fromUtf8("关"))
        {
            Ram.SendData[3]=0x55;
            click->setText(QString::fromUtf8("开"));
        }

        else
        {
            Ram.SendData[3]=0xAA;
            click->setText(QString::fromUtf8("关"));
        }
        ui->g_lb->setText("|");
        ui->r_lb->setText(" ");
        ui->h_lb->setText(" ");
        ui->n_lb->setText(" ");
        click=ui->greenlight_pb;
        click->setStyleSheet("QPushButton{border:2px solid green;font:bold;font-size:18px;color:red;background-color: rgb(255, 255, 127)}");
        ui->green_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: red;border:noborder;}");
        ui->red_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
        ui->yellow_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
        ui->needle_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
    }
    if(click->toolTip().toInt()==3)
    {
        Ram.SendData[2]=0x03;
        if(click->text()==QString::fromUtf8("关"))
        {
            Ram.SendData[3]=0x55;
            click->setText(QString::fromUtf8("开"));
        }

        else
        {
            Ram.SendData[3]=0xAA;
            click->setText(QString::fromUtf8("关"));
        }
        ui->h_lb->setText("|");
        ui->r_lb->setText(" ");
        ui->g_lb->setText(" ");
        ui->n_lb->setText(" ");
        click=ui->yellowlight_pb;
        click->setStyleSheet("QPushButton{border:2px solid green;font:bold;font-size:18px;color:red;background-color: rgb(255, 255, 127)}");
        ui->yellow_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: red;border:noborder;}");
        ui->red_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
        ui->green_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
        ui->needle_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
    }
    if(click->toolTip().toInt()==4)
    {
        Ram.SendData[2]=0x04;
        if(click->text()==QString::fromUtf8("关"))
        {
            Ram.SendData[3]=0x55;
            click->setText(QString::fromUtf8("开"));
        }

        else
        {
            Ram.SendData[3]=0xAA;
            click->setText(QString::fromUtf8("关"));
        }
        ui->n_lb->setText("|");
        ui->r_lb->setText(" ");
        ui->h_lb->setText(" ");
        ui->g_lb->setText(" ");
        click=ui->needlelight_pb;
        click->setStyleSheet("QPushButton{border:2px solid green;font:bold;font-size:18px;color:red;background-color: rgb(255, 255, 127)}");
        ui->needle_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: red;border:noborder;}");
        ui->yellow_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
        ui->red_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
        ui->green_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
    }
    Ram.SendData[0]=0x16;
    Ram.SendData[1]=0xA1;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
}

void OutputTesting::update_slot()
{
    QString number;
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        quint8 key=temp[0];
        temp.clear();
        if(sum==0)
        {
            buttun->click();
            sum=1;
        }
        if(key==6)
        {
            if(exchange==0)
            {
                if(times==0)
                {
                    click->click();
                    times=1;
                }
                exchange=1;
                ui->groupBox_2->setStyleSheet("QGroupBox{border:1px solid red;font-size:22px;color:green}");
                ui->groupBox->setStyleSheet("QGroupBox{border:1px solid white;font-size:22px;color:green}");
            }
            else
            {
                exchange=0;
                ui->groupBox->setStyleSheet("QGroupBox{border:1px solid red;font-size:22px;color:green}");
                ui->groupBox_2->setStyleSheet("QGroupBox{border:1px solid white;font-size:22px;color:green}");
            }
        }

        if(key==29)
        {
            on_plus_Out_pb_clicked();
        }
        if(key==39)
        {
            on_minus_Out_lb_clicked();
        }
        if(key==7)
        {
            on_pointmove_Out_pb_clicked();
        }
        if(key==8)
        {
            on_stop_Out_lb_clicked();
        }
        number=buttun->text();
        if(key==27)
        {
            buttun->setText("0");
        }
        if(key==12)
        {
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
        if(key==13)
        {
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
        if(key==14)
        {
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
        if(key==17)
        {
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
        if(key==18)
        {
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
        if(key==19)
        {
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
        if(key==20)
        {
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
        if(key==22)
        {
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
        if(key==23)
        {
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
        if(key==24)
        {
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
        if(key==28)
        {
            ui->pushButton_back->click();
        }
        if(key==34)
        {
            click->click();
        }
        if(key==33)
        {
            digital_mark=0;
            if(exchange==1)
            {
                if(click->toolTip().toInt()>1)
                {
                    int curr=click->toolTip().toInt()-1;
                    switch(curr)
                    {
                    case 2:
                    {
                        ui->g_lb->setText("|");
                        ui->r_lb->setText(" ");
                        ui->h_lb->setText(" ");
                        ui->n_lb->setText(" ");
                        click=ui->greenlight_pb;
                        ui->red_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:18px;color: white;border:noborder;}");
                        ui->green_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: red;border:noborder;}");
                        ui->yellow_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                        ui->needle_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                    }
                        break;
                    case 3:
                    {
                        ui->h_lb->setText("|");
                        ui->r_lb->setText(" ");
                        ui->g_lb->setText(" ");
                        ui->n_lb->setText(" ");
                        click=ui->yellowlight_pb;
                        ui->red_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:18px;color: white;border:noborder;}");
                        ui->green_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                        ui->yellow_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: red;border:noborder;}");
                        ui->needle_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                    }
                        break;
                    case 1:
                    {
                        ui->n_lb->setText("");
                        ui->r_lb->setText("| ");
                        ui->h_lb->setText(" ");
                        ui->g_lb->setText(" ");
                        click=ui->redlight_pb;
                        ui->red_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: red;border:noborder;}");
                        ui->green_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                        ui->yellow_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                        ui->needle_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                    }
                        break;

                    }
                }

            }
            else if(exchange==0)
            {
                ui->speed_lb->click();
            }
        }
        if(key==35)
        {
            digital_mark=0;
            if(exchange==1)
            {
                if(click->toolTip().toInt()<4)
                {
                    int cur=click->toolTip().toInt()+1;
                    switch(cur)
                    {
                    case 2:
                    {
                        ui->g_lb->setText("|");
                        ui->r_lb->setText(" ");
                        ui->h_lb->setText(" ");
                        ui->n_lb->setText(" ");
                        click=ui->greenlight_pb;
                        ui->red_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                        ui->green_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: red;border:noborder;}");
                        ui->yellow_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                        ui->needle_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                    }
                        break;
                    case 3:
                    {
                        ui->h_lb->setText("|");
                        ui->r_lb->setText(" ");
                        ui->g_lb->setText(" ");
                        ui->n_lb->setText(" ");
                        click=ui->yellowlight_pb;
                        ui->red_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                        ui->green_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                        ui->yellow_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: red;border:noborder;}");
                        ui->needle_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                    }
                        break;
                    case 4:
                    {
                        ui->n_lb->setText("|");
                        ui->r_lb->setText(" ");
                        ui->h_lb->setText(" ");
                        ui->g_lb->setText(" ");
                        click=ui->needlelight_pb;
                        ui->red_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                        ui->green_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                        ui->yellow_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: white;border:noborder;}");
                        ui->needle_lb->setStyleSheet("QPushButton{background: transparent;font:bold;font-size:20px;color: red;border:noborder;}");
                    }
                        break;
                    }
                }

            }
            else if(exchange==0)
            {
                ui->plusandminus_lb->click();
            }
        }
    }
    //}
}


void OutputTesting::number_answer(QString& n)
{
    if(digital_mark==0)
    {
        n="0";
        digital_mark=1;
    }
}

void OutputTesting::on_pushButton_back_clicked()
{
    timer_port->stop();
    Ram.to_TestingMain_slot();
    this->close();
}
