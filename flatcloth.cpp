#include "flatcloth.h"
#include "ui_flatcloth.h"
#include <QTime>

extern QByteArray temp;
FlatCloth::FlatCloth(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FlatCloth)
{
     ui->setupUi(this);
     setAttribute(Qt::WA_DeleteOnClose);
     this->setWindowFlags(Qt::FramelessWindowHint);
     this->setGeometry(219,168,362,144);
     buttun=new QPushButton;
     for(int i=0;i<6;i++)
     {
         flat[i]=0;
         flar_bool[i]=true;
     }
     ID=0;
     timer_flatcloth=new QTimer(this);
     ui->one_pb->setCheckable(true);
     ui->two_pb->setCheckable(true);
     ui->three_pb->setCheckable(true);
     ui->four_pb->setCheckable(true);
     ui->five_pb->setCheckable(true);
     ui->six_pb->setCheckable(true);

     ui->one_pb->setStyle(QStyleFactory::create("Windows"));
     ui->two_pb->setStyle(QStyleFactory::create("Windows"));
     ui->three_pb->setStyle(QStyleFactory::create("Windows"));
     ui->four_pb->setStyle(QStyleFactory::create("Windows"));
     ui->five_pb->setStyle(QStyleFactory::create("Windows"));
     ui->six_pb->setStyle(QStyleFactory::create("Windows"));

     ui->one_pb->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");
     ui->two_pb->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");
     ui->three_pb->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");
     ui->four_pb->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");
     ui->five_pb->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");
     ui->six_pb->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");
     ui->ok_pb->setStyleSheet("QPushButton{border-radius:10;background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");

     connect(timer_flatcloth, SIGNAL(timeout()),this,SLOT(update_slot()));
     timer_flatcloth->start(100);
     connect(ui->one_pb, SIGNAL(clicked()),this,SLOT(dealwithClick()));
     connect(ui->two_pb, SIGNAL(clicked()),this,SLOT(dealwithClick()));
     connect(ui->three_pb, SIGNAL(clicked()),this,SLOT(dealwithClick()));
     connect(ui->four_pb, SIGNAL(clicked()),this,SLOT(dealwithClick()));
     connect(ui->five_pb, SIGNAL(clicked()),this,SLOT(dealwithClick()));
     connect(ui->six_pb, SIGNAL(clicked()),this,SLOT(dealwithClick()));
}

FlatCloth::~FlatCloth()
{
    delete ui;
}
void FlatCloth::dealwithClick()
{
    buttun=(QPushButton*)sender();
    int tip=buttun->toolTip().toInt();
    if(tip==1)
    {
        if(flar_bool[0])
        {
            flat[0]=1;
            flar_bool[0]=false;
            buttun->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:red}");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x08;
            Ram.SendData[3]=0x55;
            Ram.SendData[4]=0x01;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
        }
        else
        {
            flat[0]=0;
            flar_bool[0]=true;
            buttun->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x08;
            Ram.SendData[3]=0xAA;
            Ram.SendData[4]=0x01;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();

        }

    }
    else if(tip==2)
    {
        if(flar_bool[1])
        {
            flar_bool[1]=false;
            flat[1]=1;
            buttun->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:red}");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x08;
            Ram.SendData[3]=0x55;
            Ram.SendData[4]=0x02;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();

        }
        else
        {
            flar_bool[1]=true;
            flat[1]=0;
            buttun->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x08;
            Ram.SendData[3]=0xAA;
            Ram.SendData[4]=0x02;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();

        }

    }
    else if(tip==3)
    {
        if(flar_bool[2])
        {
            flar_bool[2]=false;
            flat[2]=1;
            buttun->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:red}");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x08;
            Ram.SendData[3]=0x55;
            Ram.SendData[4]=0x03;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();

        }
        else
        {
            flar_bool[2]=true;
            flat[2]=0;
            buttun->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x08;
            Ram.SendData[3]=0xAA;
            Ram.SendData[4]=0x03;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();

        }

    }
    else if(tip==4)
    {
        if(flar_bool[3])
        {
            flar_bool[3]=false;
            flat[3]=1;
            buttun->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:red}");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x08;
            Ram.SendData[3]=0x55;
            Ram.SendData[4]=0x04;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();


        }
        else
        {
            flar_bool[3]=true;
            flat[3]=0;
            buttun->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x08;
            Ram.SendData[3]=0xAA;
            Ram.SendData[4]=0x04;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();

        }

    }
    else if(tip==5)
    {
        if(flar_bool[4])
        {
            flar_bool[4]=false;
            flat[4]=1;
            buttun->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:red}");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x08;
            Ram.SendData[3]=0x55;
            Ram.SendData[4]=0x05;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();

        }
        else
        {
            flar_bool[4]=true;
            flat[4]=0;
            buttun->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x08;
            Ram.SendData[3]=0xAA;
            Ram.SendData[4]=0x05;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();

        }
    }
    else if(tip==6)
    {
        if(flar_bool[5])
        {
            flar_bool[5]=false;
            flat[5]=1;
            buttun->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:red}");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x08;
            Ram.SendData[3]=0x55;
            Ram.SendData[4]=0x06;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
        }
        else
        {
            flar_bool[5]=true;
            flat[5]=0;
            buttun->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100);font-size:20px;color:rgb(255, 255, 0)}");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x08;
            Ram.SendData[3]=0xAA;
            Ram.SendData[4]=0x06;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();

        }
    }
//    ID=flat[0]+2*flat[1]+4*flat[2]+8*flat[3]+16*flat[4]+32*flat[5];
//    Ram.SendData[0]=0x20;
//    Ram.SendData[1]=0xA2;
//    Ram.SendData[2]=0x08;
//    Ram.SendData[3]=0x55;
//    Ram.SendData[4]=ID;
//    Ram.SendData[5]=0x00;
//    Ram.SendData[6]=0x00;
//    Ram.SendData[7]=0x00;
//    Ram.SendData[8]=0x00;
//    Ram.SendData[9]=0x00;
//    Ram.UI_to_RTC();
   // emit sendID(ID);
}
void FlatCloth::on_ok_pb_clicked()
{
    timer_flatcloth->stop();
    this->reject();
}
void FlatCloth::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)37)
        {
        }
        if(temp[0]==(char)12)
        {
            temp[0]=255;
            ui->one_pb->click();
        }
        else if(temp[0]==(char)17)
        {
            temp[0]=255;
            ui->two_pb->click();
        }
        else if(temp[0]==(char)22)
        {
            temp[0]=255;
            ui->three_pb->click();
        }
        else if(temp[0]==(char)13)
        {
            temp[0]=255;
            ui->four_pb->click();
        }
        else if(temp[0]==(char)18)
        {
            temp[0]=255;
            ui->five_pb->click();
        }
        else if(temp[0]==(char)23)
        {
            temp[0]=255;
            ui->six_pb->click();
        }
        else if(temp[0]==(char)34)
        {
            temp[0]=255;
            on_ok_pb_clicked();
        }
    }
}
