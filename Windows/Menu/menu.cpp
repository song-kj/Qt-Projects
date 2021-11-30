#include "menu.h"
#include "ui_menu.h"
#include "Windows/Settings/settingmain.h"
#include "Windows/Testings/testingmain.h"
#include "Windows/FileOperations/fileoperations.h"
#include "Windows/SystemMaintenance/systemmaintence.h"
#include "Windows/Codesetting/codesettingmain.h"
#include "Windows/Testings/faultdetection.h"

extern QByteArray temp;
extern int warningShow_mark;
Menu::Menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
//    warningShow_mark = 0;
    timer_menu=new QTimer(this);
    connect(timer_menu, SIGNAL(timeout()),this,SLOT(update_slot()));
    timer_menu->start(100);
}

Menu::~Menu()
{
    delete ui;
}

/*判断键盘*/
void Menu::update_slot()
{
    if(this->isActiveWindow())
    {
        quint8 key=temp[0];
        temp.clear();
        if(key==12)
        {
            ui->setting_pb->click();
        }
        else if(key==13)
        {
            ui->systemmantence_pb->click();
        }
        else if(key==17)
        {
            ui->testing_pb->click();
        }
        else if(key==18)
        {
            ui->codesetting_pb->click();
        }
        else if(key==28||key==14)
        {
            ui->patternwoven_pb->click();
        }
        else if(key==22)
        {
            ui->fileoperation_pb->click();
        }
        else if(key==23)
        {
            ui->faultdetection_pb->click();
        }
    }

}

/*参数设置*/
void Menu::on_setting_pb_clicked()
{
    SettingMain *pSettingMain=new SettingMain;
    pSettingMain->exec();
}
/*机器测试*/
void Menu::on_testing_pb_clicked()
{
    TestingMain *pTestingMain=new TestingMain;
    pTestingMain->exec();
}
/*文件操作*/
void Menu::on_fileoperation_pb_clicked()
{
     FileOperations *pFileOperations=new FileOperations;
     pFileOperations->exec();
}
/*系统维护*/
void Menu::on_systemmantence_pb_clicked()
{
    SystemMaintence *pSystemMaintence=new SystemMaintence;
    pSystemMaintence->exec();
}
/*返回主界面*/
void Menu::on_patternwoven_pb_clicked()
{
//     warningShow_mark=1;
     timer_menu->stop();
     this->close();
}

//拨码设置
void Menu::on_codesetting_pb_clicked()
{
    CodesettingMain *code=new CodesettingMain;
    code->exec();
}

//故障检测
void Menu::on_faultdetection_pb_clicked()
{
    FaultDetection *falut=new FaultDetection;
    falut->exec();
}
