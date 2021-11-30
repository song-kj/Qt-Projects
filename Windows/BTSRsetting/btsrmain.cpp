#include "btsrmain.h"
#include "ui_btsrmain.h"
#include "Windows/BTSRsetting/btsrsetid.h"
#include "Windows/BTSRsetting/btsrstudy.h"
#include "Windows/BTSRsetting/btsrparameter.h"
#include "Windows/BTSRsetting/btsrstartstudy.h"

extern QByteArray temp;

BTSRMain::BTSRMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BTSRMain)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    Ram.to_SettingMain_slot();
    keyboard_timer = new QTimer(this);
    connect(keyboard_timer,SIGNAL(timeout()),this,SLOT(InputEvent()));
    keyboard_timer->start(10);
}

BTSRMain::~BTSRMain()
{
    delete ui;
}

//编号
void BTSRMain::on_toolButton_setid_clicked()
{
    BtsrSetID *id=new BtsrSetID;
    id->exec();
}

//纱线参数
void BTSRMain::on_toolButton_yarn_parameter_clicked()
{
    BTSRParameter *par=new BTSRParameter(0);
    par->exec();
}

//学习
void BTSRMain::on_toolButton__study_clicked()
{
    BTSRStartStudy *stu=new BTSRStartStudy;
    stu->exec();
}

//感应器参数
void BTSRMain::on_toolButton_sensor_parameter_clicked()
{
    BTSRParameter *par=new BTSRParameter(1);
    par->exec();
}

//功能设置
void BTSRMain::on_toolButton_function_set_clicked()
{
    BTSRStudy *stu=new BTSRStudy;
    stu->exec();
}

//感应器禁用
void BTSRMain::on_toolButton_sensor_disable_clicked()
{
    BTSRParameter *par=new BTSRParameter(2);
    par->exec();
}

//返回
void BTSRMain::on_return_btn_clicked()
{
    Ram.to_PatternWoven_slot();
    this->close();
}

void BTSRMain::InputEvent()
{
    if(temp.length()>=1&&this->isActiveWindow())
    {
        if(temp[0]==(char)10||temp[0]==(char)28)
        {
            temp[0]=255;
            ui->return_btn->click();
        }
        else if(temp[0]==(char)1)
        {
            temp[0]=255;
            ui->toolButton_setid->click();
        }
        else if(temp[0]==(char)2)
        {
            temp[0]=255;
            ui->toolButton__study->click();
        }
        else if(temp[0]==(char)3)
        {
            temp[0]=255;
            ui->toolButton_function_set->click();
        }
        else if(temp[0]==(char)6)
        {
            temp[0]=255;
            ui->toolButton_yarn_parameter->click();
        }
        else if(temp[0]==(char)7)
        {
            temp[0]=255;
            ui->toolButton_sensor_parameter->click();
        }
        else if(temp[0]==(char)8)
        {
            temp[0]=255;
            ui->toolButton_sensor_disable->click();
        }
    }

}
