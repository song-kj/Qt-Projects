#include "settingmain.h"
#include "ui_settingmain.h"
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
#include "QCom/qextserialport.h"
#include "Windows/Settings/warningsetting.h"
#include "mymessagebox.h"
#include "stitchadjust.h"

extern QByteArray temp;
extern bool startNet;
SettingMain::SettingMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingMain)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    sendpar=false;
    saveinfo=new ParameterSaveInfo(this);
    saveinfo->hide();
    Ram.to_SettingMain_slot();
    timer_settingmain=new QTimer(this);
    connect(timer_settingmain, SIGNAL(timeout()),this,SLOT(update_slot()));
    timer_settingmain->start(100);
}

SettingMain::~SettingMain()
{
    delete ui;
}

void SettingMain::update_slot()
{
    if(sendpar)
    {
        //等待参数传递完成退出界面
        if(startToTransmitPara>=4)
        {
            timer_settingmain->stop();
            saveinfo->close();
            this->close();
        }
    }
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        quint8 key=temp[0];
        temp.clear();
        if(key==12)
        {
            ui->parametersetting_lb->click();
        }
        else if(key==17)
        {
            ui->zerolevelsetting_lb->click();
        }
        else if(key==22)
        {
            ui->motorsetting_lb->click();
        }
        else if(key==13)
        {
            ui->netsetting_lb->click();
        }
        else if(key==18)
        {
            ui->warnsetting_lb->click();
        }
        else if(key==28||key==23)
        {
            ui->back_lb->click();
        }
    }
}

//参数设置
void SettingMain::on_parametersetting_lb_clicked()
{
    ParameterSetting *pParameterSetting=new ParameterSetting;
    pParameterSetting->exec();
}

//压针微调设置
void SettingMain::on_motorsetting_lb_clicked()
{
    StitchAdjust *stitch=new StitchAdjust;
    stitch->setType(1);
    stitch->exec();
}

//零位设置
void SettingMain::on_zerolevelsetting_lb_clicked()
{
    ZeroLevelSetting *pZeroLevelSetting=new ZeroLevelSetting;
    pZeroLevelSetting->exec();
}

//返回 传递参数
void SettingMain::on_back_lb_clicked()
{
    if(!sendpar)
    {
        startToTransmitPara=0;
        sendpar=true;
        saveinfo->show();
    }
}

//报警屏蔽设置
void SettingMain::on_warnsetting_lb_clicked()
{
    KeyBd *pKeyBd=new KeyBd;
    if(pKeyBd->exec()==QDialog::Accepted)
    {
        if(pKeyBd->inputstr=="888888")
        {
            WarningSetting *warn=new WarningSetting;
            warn->exec();
        }
        else
        {
            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("密码错误！"),0);
            box->exec();
        }
    }
    delete pKeyBd;
}

//网络设置
void SettingMain::on_netsetting_lb_clicked()
{
    startNet=true;
}
