#include "selsetcode.h"
#include "ui_selsetcode.h"
#include "Tool/KeyBoard/dialogcalculator.h"

extern QByteArray temp;

SelSetCode::SelSetCode(int t, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelSetCode)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    type=t;
    initialForm();
}

SelSetCode::~SelSetCode()
{
    delete ui;
}

void SelSetCode::on_pushButton_set_clicked()
{
    Ram.SendData[0]=0X20;
    Ram.SendData[1]=0XA5;
    Ram.SendData[2]=type;
    Ram.SendData[3]=0X01;
    Ram.SendData[4]=(quint8)olddata;
    Ram.SendData[5]=(quint8)newdata;
    Ram.UI_to_RTC();
    ui->pushButton_set->setDisabled(true);
    ui->pushButton_cancel->setDisabled(true);
    ui->pushButton_back->setDisabled(true);
    ui->pushButton_test->setDisabled(true);
    ui->pushButton_stop->setDisabled(true);
    num=0;
    flag=true;
}

void SelSetCode::on_pushButton_cancel_clicked()
{
    time->stop();
    this->close();
}

void SelSetCode::on_pushButton_add_clicked()
{
    if(ui->pushButton_new->isChecked())
    {
    if(newdata<32)
            newdata++;
         ui->pushButton_new->setText(QString::number(newdata));
    }
    else
    {
        if(testdata<32)
                testdata++;
             ui->pushButton_testnum->setText(QString::number(testdata));
    }
}

void SelSetCode::on_pushButton_sub_clicked()
{
    if(ui->pushButton_new->isChecked())
    {
    if(newdata>0)
            newdata--;
         ui->pushButton_new->setText(QString::number(newdata));
    }
    else
    {
        if(testdata>0)
                testdata--;
             ui->pushButton_testnum->setText(QString::number(testdata));
    }
}

//老化测试
void SelSetCode::on_pushButton_test_clicked()
{
    ui->pushButton_test->setChecked(true);
    ui->pushButton_stop->setChecked(false);
    Ram.SendData[0]=0X20;
    Ram.SendData[1]=0XA5;
    Ram.SendData[2]=type;
    Ram.SendData[3]=0X03;
    Ram.SendData[4]=0X01;
    Ram.SendData[5]=(quint8)testdata;
    Ram.UI_to_RTC();
}

//停止测试
void SelSetCode::on_pushButton_stop_clicked()
{
    ui->pushButton_test->setChecked(false);
    ui->pushButton_stop->setChecked(true);
    Ram.SendData[0]=0X20;
    Ram.SendData[1]=0XA5;
    Ram.SendData[2]=type;
    Ram.SendData[3]=0X03;
    Ram.SendData[4]=0X02;
    Ram.SendData[5]=(quint8)testdata;
    Ram.UI_to_RTC();
}

//设置驱动板新号
void SelSetCode::on_pushButton_new_clicked()
{
    ui->pushButton_new->setChecked(true);
    ui->pushButton_testnum->setChecked(false);
    DialogCalculator *sr=new DialogCalculator;
    sr->maxValue=100;
    sr->minValue=1;
    if(sr->exec()==QDialog::Accepted)
    {
        ui->pushButton_new->setText(sr->str);
        newdata=sr->str.toInt();
    }
    delete sr;
}

//设置测试号
void SelSetCode::on_pushButton_testnum_clicked()
{
    ui->pushButton_new->setChecked(false);
    ui->pushButton_testnum->setChecked(true);
    DialogCalculator *sr=new DialogCalculator;
    sr->maxValue=100;
    sr->minValue=1;
    if(sr->exec()==QDialog::Accepted)
    {
        ui->pushButton_testnum->setText(sr->str);
        testdata=sr->str.toInt();
    }
    delete sr;
}

void SelSetCode::timeoutslot()
{
    //RTC反馈
    if(Ram.Receive(0x300)==0x55)
    {
        if(Ram.Receive(0x200)==0x20)
        {
            if(Ram.Receive(0x201)==0xA5)
            {
                if(Ram.Receive(0x202)==type)
                {
                    if(Ram.Receive(0x203)==0x02)
                    {
                        //驱动板旧号反馈
                        if(Ram.Receive(0x205)==0X55)
                        {
                            //读取驱动板旧号
                            olddata=Ram.Receive(0x204);
                            ui->pushButton_old->setText(QString::number(olddata));
                            num=0;
                        }
                        else if(Ram.Receive(0x205)==0XAA)//设定成功
                        {
                            if(Ram.Receive(0x204)==newdata){
                                olddata=newdata;
                                testdata=newdata;
                                ui->pushButton_old->setText(QString::number(olddata));
                                ui->pushButton_testnum->setText(QString::number(testdata));
                                num=0;
                                flag=false;
                                MyMessageBox *box = new MyMessageBox(tr("提示"),tr("设定成功"),0);
                                box->exec();

                            }else{
                                num=0;
                                flag=false;
                                MyMessageBox *box = new MyMessageBox(tr("报警"),tr("设定新号有误!"),0);
                                box->exec();

                            }
                            ui->pushButton_set->setDisabled(false);
                            ui->pushButton_cancel->setDisabled(false);
                            ui->pushButton_back->setDisabled(false);
                            ui->pushButton_test->setDisabled(false);
                            ui->pushButton_stop->setDisabled(false);
                        }
                        else if(Ram.Receive(0x205)==0XEE)//设定失败
                        {
                            flag=false;
                            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("设定失败"),0);
                            box->exec();
                            num=0;
                            ui->pushButton_set->setDisabled(false);
                            ui->pushButton_cancel->setDisabled(false);
                            ui->pushButton_back->setDisabled(false);
                            ui->pushButton_test->setDisabled(false);
                            ui->pushButton_stop->setDisabled(false);
                        }
                        Ram.Send(0xaa,0x300);
                    }
                }
            }
        }
    }
    if(flag)
    {
        num++;
        if(num>=20){
            MyMessageBox *box = new MyMessageBox(tr("报警"),tr("设定超时!"),0);
            box->exec();
            ui->pushButton_set->setDisabled(false);
            ui->pushButton_cancel->setDisabled(false);
            ui->pushButton_back->setDisabled(false);
            ui->pushButton_test->setDisabled(false);
            ui->pushButton_stop->setDisabled(false);
            num=0;
            flag=false;
        }
    }
}

void SelSetCode::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)33)//上
        {
            temp[0]=255;
            if(ui->pushButton_new->isChecked())
                on_pushButton_testnum_clicked();
            else
                on_pushButton_new_clicked();
        }
        else if(temp[0]==(char)35)//下
        {
            temp[0]=255;
            if(ui->pushButton_new->isChecked())
                on_pushButton_testnum_clicked();
            else
                on_pushButton_new_clicked();
        }
        else if(temp[0]==(char)28)
        {
            temp[0]=255;
            on_pushButton_cancel_clicked();
        }
        else if(temp[0]==(char)34)
        {
            temp[0]=255;
            on_pushButton_set_clicked();
        }
        if(temp[0]==(char)27)
        {
            temp[0]=255;
            str="0";
            setNumber(str);
        }
        if(temp[0]==(char)12)
        {
            temp[0]=255;
            str+="1";
            setNumber(str);
        }
        if(temp[0]==(char)13)
        {
            temp[0]=255;
            str+="4";
            setNumber(str);
        }
        if(temp[0]==(char)14)
        {
            temp[0]=255;
            str+="7";
            setNumber(str);
        }
        if(temp[0]==(char)17)
        {
            temp[0]=255;
            str+="2";
            setNumber(str);
        }
        if(temp[0]==(char)18)
        {
            temp[0]=255;
            str+="5";
            setNumber(str);
        }
        if(temp[0]==(char)19)
        {
            temp[0]=255;
            str+="8";
            setNumber(str);
        }
        if(temp[0]==(char)20)
        {
            temp[0]=255;
            str+="0";
            setNumber(str);
        }
        if(temp[0]==(char)22)
        {
            temp[0]=255;
            str+="3";
            setNumber(str);
        }
        if(temp[0]==(char)23)
        {
            temp[0]=255;
            str+="6";
            setNumber(str);
        }
        if(temp[0]==(char)24)
        {
            temp[0]=255;
            str+="9";
            setNumber(str);
        }
    }
}

void SelSetCode::setNumber(QString num)
{
    int n=num.toInt();
        if(n>32)
            str.remove(str.length()-1,1);
        if(ui->pushButton_new->isChecked())
        {
            ui->pushButton_new->setText(str);
            newdata=str.toInt();
        }
        else
        {
            ui->pushButton_testnum->setText(str);
            testdata=str.toInt();
        }
}

void SelSetCode::initialForm()
{
    switch (type) {
    case 0x01:
        ui->label_title->setText(tr("选针器拨码设置"));
        ui->pushButton_test->setText(tr("打上"));
        ui->pushButton_stop->setText(tr("打下"));
        ui->widget_test->show();
        break;
    case 0x02:
        ui->label_title->setText(tr("气阀板拨码设置"));
        ui->widget_test->hide();
        break;
    case 0x03:
        ui->label_title->setText(tr("电机板拨码设置"));
        ui->widget_test->hide();
        break;
    default:
        break;
    }
    olddata=0;
    newdata=0;
    testdata=0;
    ui->pushButton_old->setText(QString::number(olddata));
    ui->pushButton_new->setText(QString::number(olddata));
    ui->pushButton_new->setChecked(true);
    time=new QTimer(this);
    connect(time,SIGNAL(timeout()),this,SLOT(timeoutslot()));
    time->start(100);
    ptimer=new QTimer(this);
    connect(ptimer,SIGNAL(timeout()),this,SLOT(update_slot()));
    ptimer->start(10);
    num=0;
    flag=false;
}

void SelSetCode::on_pushButton_back_clicked()
{
    time->stop();
    this->close();
}
