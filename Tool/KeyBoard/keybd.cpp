#include "keybd.h"
#include "ui_keybd.h"

extern QByteArray temp;
KeyBd::KeyBd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyBd)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
//    ui->lineEdit->setReadOnly(true);
    ui->lineEdit->setText(tr(""));
    ptime=new QTimer(this);
    ptime->start(100);
    connect(ptime,SIGNAL(timeout()),this,SLOT(update_slot()));
}

KeyBd::~KeyBd()
{
    delete ui;
}

//确定键
void KeyBd::on_pb_ensure_clicked()
{
    emit sendData(ui->lineEdit->text());
    inputstr=ui->lineEdit->text();
    this->accept();
}

void KeyBd::on_pb_close_clicked()
{
    this->reject();
}

void KeyBd::update_slot()
{
    QString str;
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        str= ui->lineEdit->text();
        if(temp[0]==(char)20)//0
        {
            temp[0]=255;
           if(str=="0")
           {
                ui->lineEdit->setText("0");
           }
           else
           {
                ui->lineEdit->setText(str.append("0"));
           }

        }
        if(temp[0]==(char)12)//1
        {
            temp[0]=255;
            if(str=="0")
            {
                 ui->lineEdit->setText("1");
            }
            else
            {
                 ui->lineEdit->setText(str.append("1"));
            }

        }
        if(temp[0]==(char)17)//2
        {
            temp[0]=255;
              if(str=="0")
              {
                   ui->lineEdit->setText("2");
              }
              else
              {
                   ui->lineEdit->setText(str.append("2"));
              }

        }
        if(temp[0]==(char)22)//3
        {
            temp[0]=255;
            if(str=="0")
            {
                 ui->lineEdit->setText("3");
            }
            else
            {
                 ui->lineEdit->setText(str.append("3"));
            }

        }
        if(temp[0]==(char)13)//4
        {
            temp[0]=255;
            if(str=="0")
            {
                 ui->lineEdit->setText("4");
            }
            else
            {
                 ui->lineEdit->setText(str.append("4"));
            }

        }
        if(temp[0]==(char)18)//5
        {
            temp[0]=255;
            if(str=="0")
            {
                 ui->lineEdit->setText("5");
            }
            else
            {
                 ui->lineEdit->setText(str.append("5"));
            }

        }
        if(temp[0]==(char)23)//6
        {
            temp[0]=255;
            if(str=="0")
            {
                 ui->lineEdit->setText("6");
            }
            else
            {
                 ui->lineEdit->setText(str.append("6"));
            }

        }
        if(temp[0]==(char)14)//7
        {
            temp[0]=255;
            if(str=="0")
            {
                 ui->lineEdit->setText("7");
            }
            else
            {
                 ui->lineEdit->setText(str.append("7"));
            }

        }
        if(temp[0]==(char)19)//8
        {
            temp[0]=255;
            if(str=="0")
            {
                 ui->lineEdit->setText("8");
            }
            else
            {
                 ui->lineEdit->setText(str.append("8"));
            }

        }
        if(temp[0]==(char)24)//9
        {
            temp[0]=255;
            if(str=="0")
            {
                 ui->lineEdit->setText("9");
            }
            else
            {
                 ui->lineEdit->setText(str.append("9"));
            }
        }
        if(temp[0]==(char)34)
        {
            temp[0]=255;
            on_pb_ensure_clicked();

        }
        if(temp[0]==(char)29)
        {
            temp[0]=255;

        }
        if(temp[0]==(char)28)
        {
            temp[0]=255;
            on_pb_close_clicked();

        }
        if(temp[0]==(char)27)
        {
            temp[0]=255;
            str="";
            ui->lineEdit->setText(str);
        }
    }
}
