#include "timesetting.h"
#include "ui_timesetting.h"
#include <QDateTime>

extern QByteArray temp;
TimeSetting::TimeSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeSetting)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    digital_mark=0;
    btn=new QPushButton;
    btn=ui->year_btn;
    showBool = true;
    ptimer=new QTimer(this);
    connect(ptimer,SIGNAL(timeout()),this,SLOT(update_slot()));
    ptimer->start(10);
    ui->year_btn->setStyleSheet("QPushButton{border-width:0;border-style:outset;background: transparent;font-size:20px;color:red}");
    ui->month_btn->setStyleSheet("QPushButton{border-width:0;border-style:outset;background: transparent;font-size:20px;color:blue}");
    ui->day_btn->setStyleSheet("QPushButton{border-width:0;border-style:outset;background: transparent;font-size:20px;color:blue}");
    ui->hour_btn->setStyleSheet("QPushButton{border-width:0;border-style:outset;background: transparent;font-size:20px;color:blue}");
    ui->minute_btn->setStyleSheet("QPushButton{border-width:0;border-style:outset;background: transparent;font-size:20px;color:blue}");
    ui->second_btn->setStyleSheet("QPushButton{border-width:0;border-style:outset;background: transparent;font-size:20px;color:blue}");
    ui->sure_btn->setStyleSheet("QPushButton{border-radius:5;font-size:20px;color:black}");
    ui->cancle_btn->setStyleSheet("QPushButton{border-radius:5;font-size:20px;color:black}");
    connect(ui->year_btn,SIGNAL(clicked()),this,SLOT(listeningClicked()));
    connect(ui->month_btn,SIGNAL(clicked()),this,SLOT(listeningClicked()));
    connect(ui->day_btn,SIGNAL(clicked()),this,SLOT(listeningClicked()));
    connect(ui->hour_btn,SIGNAL(clicked()),this,SLOT(listeningClicked()));
    connect(ui->minute_btn,SIGNAL(clicked()),this,SLOT(listeningClicked()));
    connect(ui->second_btn,SIGNAL(clicked()),this,SLOT(listeningClicked()));
}

TimeSetting::~TimeSetting()
{
    delete ui;
}
void TimeSetting::listeningClicked()
{
    btn->setStyleSheet("QPushButton{border-width:0;border-style:outset;background: transparent;font-size:20px;color:blue}");
    btn=(QPushButton*)sender();
    btn->setStyleSheet("QPushButton{border-width:0;border-style:outset;background: transparent;font-size:20px;color:red}");
}

void TimeSetting::on_sure_btn_clicked()
{
    struct tm nowtime;
    time_t t;
    nowtime.tm_sec=ui->second_btn->text().toInt();/* Seconds.[0-60](1 leap second)*/
    nowtime.tm_min=ui->minute_btn->text().toInt();/* Minutes.[0-59] */
    nowtime.tm_hour=ui->hour_btn->text().toInt();/* Hours. [0-23] */
    nowtime.tm_mday=ui->day_btn->text().toInt();/*  Day.[1-31]  */
    nowtime.tm_mon=ui->month_btn->text().toInt()-1;/* Month.[0-11]*/
    nowtime.tm_year=ui->year_btn->text().toInt()-1900;/* Year- 1900.*/
    nowtime.tm_isdst=-1;/*DST.[-1/0/1]*/
    t=mktime(&nowtime);
    #ifdef Q_OS_WIN32
    #else
    stime(&t);
    #endif
    system("hwclock -w");
    close();
}

void TimeSetting::getKeyBoardData(QString str)
{
    if(btn->whatsThis().toInt()==1)
    {
        ui->year_btn->setText(str);
    }
    else if(btn->whatsThis().toInt()==2)
    {
        ui->month_btn->setText(str);
    }
    else if(btn->whatsThis().toInt()==3)
    {
        ui->day_btn->setText(str);
    }
    else if(btn->whatsThis().toInt()==4)
    {
        ui->hour_btn->setText(str);
    }
    else if(btn->whatsThis().toInt()==5)
    {
        ui->minute_btn->setText(str);
    }
    else if(btn->whatsThis().toInt()==6)
    {
        ui->second_btn->setText(str);
    }
}

void TimeSetting::on_cancle_btn_clicked()
{
    ptimer->stop();/***/
    reject();
}
void TimeSetting::update_slot()
{
    if(showBool)
    {
         curTime();
    }

    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)39)
        {
            digital_mark=0;
            temp[0]=255;
            if(btn->whatsThis().toInt()==1)
            {
                ui->month_btn->click();
            }
            else if(btn->whatsThis().toInt()==2)
            {
                ui->day_btn->click();
            }
            else if(btn->whatsThis().toInt()==3)
            {
                ui->hour_btn->click();
            }
            else if(btn->whatsThis().toInt()==4)
            {
                ui->minute_btn->click();
            }
            else if(btn->whatsThis().toInt()==5)
            {
                ui->second_btn->click();
            }
            else if(btn->whatsThis().toInt()==6)
            {
                ui->year_btn->click();
            }
        }
        else if(temp[0]==(char)29)
        {
            digital_mark=0;
            temp[0]=255;
            if(btn->whatsThis().toInt()==6)
            {
                ui->minute_btn->click();
            }
            else if(btn->whatsThis().toInt()==5)
            {
                ui->hour_btn->click();
            }
            else if(btn->whatsThis().toInt()==4)
            {
                ui->day_btn->click();
            }
            else if(btn->whatsThis().toInt()==3)
            {
                ui->month_btn->click();
            }
            else if(btn->whatsThis().toInt()==2)
            {
                ui->year_btn->click();
            }
            else if(btn->whatsThis().toInt()==1)
            {
                ui->second_btn->click();
            }
        }
        else if(temp[0]==(char)33)
        {
            digital_mark=0;
            temp[0]=255;
            if(btn->whatsThis().toInt()==6)
            {
                ui->day_btn->click();
            }
            else if(btn->whatsThis().toInt()==3)
            {
                ui->minute_btn->click();
            }
            else if(btn->whatsThis().toInt()==5)
            {
                ui->month_btn->click();
            }
            else if(btn->whatsThis().toInt()==2)
            {
                ui->hour_btn->click();
            }
            else if(btn->whatsThis().toInt()==4)
            {
                ui->year_btn->click();
            }
            else if(btn->whatsThis().toInt()==1)
            {
                ui->second_btn->click();
            }
        }
        else if(temp[0]==(char)35)
        {
            digital_mark=0;
            temp[0]=255;
            if(btn->whatsThis().toInt()==1)
            {
                ui->hour_btn->click();
            }
            else if(btn->whatsThis().toInt()==4)
            {
                ui->month_btn->click();
            }
            else if(btn->whatsThis().toInt()==2)
            {
                ui->minute_btn->click();
            }
            else if(btn->whatsThis().toInt()==5)
            {
                ui->day_btn->click();
            }
            else if(btn->whatsThis().toInt()==3)
            {
                ui->second_btn->click();
            }
            else if(btn->whatsThis().toInt()==6)
            {
                ui->year_btn->click();
            }
        }
        else if(temp[0]==(char)28)
        {
            temp[0]=255;
            on_cancle_btn_clicked();
        }
        else if(temp[0]==(char)34)
        {
            temp[0]=255;
            on_sure_btn_clicked();

        }
        QString  number=btn->text();
        if(temp[0]==(char)27)
        {
            showBool = false;
            temp[0]=255;
            btn->setText("0");

        }
        if(temp[0]==(char)12)
        {
            showBool = false;
            temp[0]=255;
            number_answer(number);
              if(number=="0")
              {
                  btn->setText("1");
              }
              else
              {
                  btn->setText(number.append("1"));
              }
        }
        if(temp[0]==(char)13)
        {
            showBool = false;
            temp[0]=255;
             number_answer(number);
              if(number=="0")
              {
                  btn->setText("4");
              }
              else
              {
                  btn->setText(number.append("4"));
              }

        }
        if(temp[0]==(char)14)
        {
            showBool = false;
            temp[0]=255;
             number_answer(number);
              if(number=="0")
              {
                  btn->setText("7");
              }
              else
              {
                  btn->setText(number.append("7"));
              }
        }
        if(temp[0]==(char)17)
        {
            showBool = false;
            temp[0]=255;
              number_answer(number);
              if(number=="0")
              {
                  btn->setText("2");
              }
              else
              {
                  btn->setText(number.append("2"));
              }
        }
        if(temp[0]==(char)18)
        {
            showBool = false;
            temp[0]=255;
              number_answer(number);
              if(number=="0")
              {
                  btn->setText("5");
              }
              else
              {
                  btn->setText(number.append("5"));
              }
        }
        if(temp[0]==(char)19)
        {
            showBool = false;
            temp[0]=255;

             number_answer(number);
              if(number=="0")
              {
                  btn->setText("8");
              }
              else
              {
                  btn->setText(number.append("8"));
              }
        }
        if(temp[0]==(char)20)
        {
            showBool = false;
            temp[0]=255;
             number_answer(number);
              if(number=="0")
              {
                  btn->setText("0");
              }
              else
              {
                  btn->setText(number.append("0"));
              }
        }
        if(temp[0]==(char)22)
        {
            showBool = false;
            temp[0]=255;
             number_answer(number);
              if(number=="0")
              {
                  btn->setText("3");
              }
              else
              {
                  btn->setText(number.append("3"));
              }
        }
        if(temp[0]==(char)23)
        {
            showBool = false;
            temp[0]=255;
             number_answer(number);
              if(number=="0")
              {
                  btn->setText("6");
              }
              else
              {
                  btn->setText(number.append("6"));
              }
        }
        if(temp[0]==(char)24)
        {
            showBool = false;
            temp[0]=255;
             number_answer(number);
              if(number=="0")
              {
                  btn->setText("9");
              }
              else
              {
                  btn->setText(number.append("9"));
              }
        }
    }
}
void TimeSetting::number_answer(QString& n)
{
    if(digital_mark==0)
    {
        n="0";
        digital_mark=1;
    }
}
void TimeSetting::curTime()
{
    QDateTime time = QDateTime::currentDateTime();
    str = time.toString("yyyyMMddhhmmss");

    ui->year_btn->setText(str.left(4));
    str = str.remove(0,4);

    ui->month_btn->setText(str.left(2));

    str = str.remove(0,2);

    ui->day_btn->setText(str.left(2));

    str = str.remove(0,2);

    ui->hour_btn->setText(str.left(2));

    str = str.remove(0,2);

    ui->minute_btn->setText(str.left(2));

    str = str.remove(0,2);

    ui->second_btn->setText(str);

}
