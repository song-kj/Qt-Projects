#include "updateselect.h"
#include "ui_updateselect.h"

extern QByteArray temp;
updateSelect::updateSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::updateSelect)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    buttun=new QPushButton;
    timer_update=new QTimer(this);

    connect(timer_update, SIGNAL(timeout()),this,SLOT(update_timer()));
    connect(ui->Control_pb, SIGNAL(clicked()),this,SLOT(dealwithclick()));
    connect(ui->Actuator_pb, SIGNAL(clicked()),this,SLOT(dealwithclick()));
    connect(ui->Motor_pb, SIGNAL(clicked()),this,SLOT(dealwithclick()));
    connect(ui->Valve_pb, SIGNAL(clicked()),this,SLOT(dealwithclick()));
    connect(ui->UI_pb, SIGNAL(clicked()),this,SLOT(dealwithclick()));
    connect(ui->PYFI_pb,SIGNAL(clicked(bool)),this,SLOT(dealwithclick()));

    Ram.to_SystemMaintance();
    timer_update->start(100);
}

updateSelect::~updateSelect()
{
    delete ui;
}
void updateSelect::dealwithclick()
{
    buttun=(QPushButton*)sender();
    if(buttun->toolTip().toInt()==0)
    {
        controlUI *cui=new controlUI;
        cui->setProgramType(MainCtl);
        cui->exec();
    }
    if(buttun->toolTip().toInt()==1)
    {
        controlUI *cui=new controlUI;
        cui->setProgramType(AirValve);
        cui->exec();
    }
    if(buttun->toolTip().toInt()==2)
    {
         controlUI *cui=new controlUI;
         cui->setProgramType(StepMotor);
         cui->exec();
    }
    if(buttun->toolTip().toInt()==3)
    {
        controlUI *cui=new controlUI;
        cui->setProgramType(Actuator);
        cui->exec();
    }
    if(buttun->toolTip().toInt()==4)
    {
        controlUI *cui=new controlUI;
        cui->setProgramType(PYF);
        cui->exec();
    }
    if(buttun->toolTip().toInt()==5)
    {
        pUI=new UI;
        pUI->exec();
    }
}

void updateSelect::update_timer()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)12)
        {
            temp[0]=255;
            ui->Control_pb->click();
        }
        if(temp[0]==(char)17)
        {
            temp[0]=255;
            ui->Motor_pb->click();
        }
        if(temp[0]==(char)22)
        {
            temp[0]=255;
            ui->Valve_pb->click();
        }
        if(temp[0]==(char)13)
        {
            temp[0]=255;
            ui->Actuator_pb->click();
        }
        if(temp[0]==(char)18)
        {
            temp[0]=255;
            ui->UI_pb->click();
        }
        if(temp[0]==(char)28)
        {
            temp[0]=255;
            on_back_pb_clicked();
        }
    }
}

void updateSelect::on_back_pb_clicked()
{
    timer_update->stop();
    this->reject();
}

