#include "machtypeselected.h"
#include "ui_machtypeselected.h"

extern QByteArray temp;
MachTypeSelected::MachTypeSelected(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MachTypeSelected)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    initialForm();
    timer_type=new QTimer(this);
    connect(timer_type,SIGNAL(timeout()),this,SLOT(update_slot()));
    timer_type->start(100);
}

MachTypeSelected::~MachTypeSelected()
{
    delete ui;
}

/*连接槽*/
void MachTypeSelected::dealwithclick()
{
    QPushButton *btn= (QPushButton*)sender();
    curIndex=btn->whatsThis().toInt();
    for(int i=0;i<totalMachs;i++)
    {
        machTypeButton[i]->setChecked(false);
    }
    btn->setChecked(true);
}

/*键盘定时器，选择机型*/
void MachTypeSelected::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>0)
    {
        if(temp[0]==(char)34)//ok
        {
            ui->btn_ok->click();
        }
        else if(temp[0]==(char)28)//esc
        {
            ui->btn_cancel->click();
        }
        //right
        else if(temp[0]==(char)39)
        {
            curIndex++;
            if(curIndex>totalMachs-1)
                curIndex=0;
            machTypeButton[curIndex]->click();
        }
        //left
        else if(temp[0]==(char)29)
        {
            curIndex--;
            if(curIndex<0)
                curIndex=totalMachs-1;
            machTypeButton[curIndex]->click();
        }
        //up
        else if(temp[0]==(char)33)
        {
            curIndex-=3;
            if(curIndex<0)
                curIndex+=totalMachs-1;
            machTypeButton[curIndex]->click();
        }
        //down
        else  if(temp[0]==(char)35)
        {
            curIndex+=3;
            if(curIndex>totalMachs-1)
                curIndex-=totalMachs-1;
            machTypeButton[curIndex]->click();
        }
        temp[0]=255;
    }
}

//确定
void MachTypeSelected::on_btn_ok_clicked()
{
    machine_Pars.needletype=machTypeButton[curIndex]->toolTip().toInt();
    timer_type->stop();
    this->accept();
}

//取消
void MachTypeSelected::on_btn_cancel_clicked()
{
    timer_type->stop();
    this->reject();
}

//界面按钮初始
void MachTypeSelected::initialForm()
{
    QStringList needletype;
    needletype<<"12/960"<<"12/1056"<<"13/1056"<<"13/1152"<<"13/1344"<<"14/1152"<<"14/1248"<<"14/1344"<<"14/1440"<<"15/1344"
             <<"15/1536"<<"16/1440"<<"16/1632"<<"17/1536"<<"17/1728"<<"18/1632"
            <<"19/1728"<<"20/1824";
    totalMachs=needletype.length();
    QStringList typenum;
    typenum<<"16"<<"0"<<"15"<<"1"<<"9"<<"14"<<"2"<<"13"<<"3"<<"4"<<"7"<<"5"<<"10"<<"6"<<"11"<<"8"<<"12"<<"17";
    for(int i=0;i<totalMachs;i++)
    {
        machTypeButton[i]=new QPushButton(this);
        machTypeButton[i]->setGeometry(50+(i%3)*140,20+(i/3)*50,101,41);
        machTypeButton[i]->setText(needletype[i]);
        machTypeButton[i]->setToolTip(typenum[i]);
        machTypeButton[i]->setWhatsThis(QString::number(i));
        machTypeButton[i]->setCheckable(true);
        connect(machTypeButton[i],SIGNAL(clicked(bool)),this,SLOT(dealwithclick()));
        if(machine_Pars.needletype==typenum[i].toInt())
        {
            curIndex=i;
            machTypeButton[i]->setChecked(true);
        }
        else
            machTypeButton[i]->setChecked(false);
    }
}
