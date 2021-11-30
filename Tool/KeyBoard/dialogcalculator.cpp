#include "dialogcalculator.h"
#include "ui_dialogcalculator.h"
#include "mymessagebox.h"

DialogCalculator::DialogCalculator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCalculator)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    firstOpen=false;
    str="0";
    ui->lineEdit->setText(str);
    setFlag(false);//默认需显示正负号
    zhengfu=false;
}

DialogCalculator::~DialogCalculator()
{
    delete ui;
}

//设置正负号显示
void DialogCalculator::setFlag(bool b)
{
    if(b)
    {
        ui->btn_fuhao->show();
        ui->btn_Num0->setGeometry(1,320,141,81);
    }
    else
    {
        ui->btn_fuhao->hide();
        ui->btn_Num0->setGeometry(1,320,211,81);
    }
}

void DialogCalculator::on_btn_Back_clicked()
{
    this->close();
}

void DialogCalculator::on_btn_Num7_clicked()
{
    if(firstOpen)
    {
        str="7";
        firstOpen=false;
    }
    else
    {
        if(str=="0")
        {
            str="7";
        }
        else if(str=="-0")
        {
            str="-7";
        }
        else
        {
            str+="7";
        }
    }
    ui->lineEdit->setText(str);
}

void DialogCalculator::on_btn_Num8_clicked()
{
    if(firstOpen)
    {
        str="8";
        firstOpen=false;
    }
    else
    {
        if(str=="0")
        {
            str="8";
        }
        else if(str=="-0")
        {
            str="-8";
        }
        else
        {
            str+="8";
        }
    }
    ui->lineEdit->setText(str);
}

void DialogCalculator::on_btn_Num9_clicked()
{
    if(firstOpen)
    {
        str="9";
        firstOpen=false;
    }
    else
    {
        if(str=="0")
        {
            str="9";
        }
        else if(str=="-0")
        {
            str="-9";
        }
        else
        {
            str+="9";
        }
    }
    ui->lineEdit->setText(str);
}

void DialogCalculator::on_btn_Num4_clicked()
{
    if(firstOpen)
    {
        str="4";
        firstOpen=false;
    }
    else
    {
        if(str=="0")
        {
            str="4";
        }
        else if(str=="-0")
        {
            str="-4";
        }
        else
        {
            str+="4";
        }
    }
    ui->lineEdit->setText(str);
}

void DialogCalculator::on_btn_Num5_clicked()
{
    if(firstOpen)
    {
        str="5";
        firstOpen=false;
    }
    else
    {
        if(str=="0")
        {
            str="5";
        }
        else if(str=="-0")
        {
            str="-5";
        }
        else
        {
            str+="5";
        }
    }
    ui->lineEdit->setText(str);
}

void DialogCalculator::on_btn_Num6_clicked()
{
    if(firstOpen)
    {
        str="6";
        firstOpen=false;
    }
    else
    {
        if(str=="0")
        {
            str="6";
        }
        else if(str=="-0")
        {
            str="-6";
        }
        else
        {
            str+="6";
        }
    }
    ui->lineEdit->setText(str);
}

void DialogCalculator::on_btn_Num1_clicked()
{
    if(firstOpen)
    {
        str="1";
        firstOpen=false;
    }
    else
    {
        if(str=="0")
        {
            str="1";
        }
        else if(str=="-0")
        {
            str="-1";
        }
        else
        {
            str+="1";
        }
    }
    ui->lineEdit->setText(str);
}

void DialogCalculator::on_btn_Num2_clicked()
{
    if(firstOpen)
    {
        str="2";
        firstOpen=false;
    }
    else
    {
        if(str=="0")
        {
            str="2";
        }
        else if(str=="-0")
        {
            str="-2";
        }
        else
        {
            str+="2";
        }
    }
    ui->lineEdit->setText(str);
}

void DialogCalculator::on_btn_Num3_clicked()
{
    if(firstOpen)
    {
        str="3";
        firstOpen=false;
    }
    else
    {
        if(str=="0")
        {
            str="3";
        }
        else if(str=="-0")
        {
            str="-3";
        }
        else
        {
            str+="3";
        }
    }
    ui->lineEdit->setText(str);
}

void DialogCalculator::on_btn_Num0_clicked()
{
    if(firstOpen)
    {
        str="0";
        firstOpen=false;
    }
    else
    {
        if(str=="0")
        {
            str="0";
        }
        else if(str=="-0")
        {
            str="-0";
        }
        else
        {
            str+="0";
        }
    }
    ui->lineEdit->setText(str);
}

void DialogCalculator::on_btn_Backspace_clicked()
{
    str=str.mid(0,str.length()-1);
    if(str=="")
    {
        str="0";
    }
    else if(str=="-")
    {
        str="-0";
    }
    ui->lineEdit->setText(str);
}

void DialogCalculator::on_btn_Sure_clicked()
{
    if(ui->lineEdit->text().toInt()<minValue||ui->lineEdit->text().toInt()>maxValue)
    {
        MyMessageBox *mmb=new MyMessageBox(tr("提示"),tr("请输入正确的数！"),0);
        mmb->exec();
        str="0";
        zhengfu=false;
        ui->lineEdit->setText("0");
        return;
    }
    else
    {
        this->accept();
    }
}

//正负号
void DialogCalculator::on_btn_fuhao_clicked()
{
    if(zhengfu)
    {
        zhengfu=false;
        str.remove("-");
    }
    else
    {
        zhengfu=true;
        if(str=="")
            str="-0";
        else
            str="-"+str;
    }
    ui->lineEdit->setText(str);
}
