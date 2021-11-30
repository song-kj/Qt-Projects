#include "mymessagebox.h"
#include "ui_mymessagebox.h"

extern QByteArray temp;
MyMessageBox::MyMessageBox(QString title,QString str,int type,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyMessageBox)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->title->setText(title);
    ui->label->setText(str);
    if(type==0)
    {
        ui->pushButton_2->hide();
    }
    pTimer = new QTimer(this);
    connect(pTimer,SIGNAL(timeout()),this,SLOT(update_slot()));
    pTimer->start(10);
}

MyMessageBox::~MyMessageBox()
{
    delete ui;
}

void MyMessageBox::on_pushButton_clicked()
{
    this->accept();
}

void MyMessageBox::on_pushButton_2_clicked()
{
    this->close();
}

void MyMessageBox::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0] == (char)28)
        {
            temp[0]=255;
            on_pushButton_2_clicked();
        }
        else if(temp[0] ==(char)34)
        {
            temp[0]=255;
            on_pushButton_clicked();
        }
    }
}
