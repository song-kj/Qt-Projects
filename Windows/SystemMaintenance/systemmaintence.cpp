#include "systemmaintence.h"
#include "ui_systemmaintence.h"
#include "dialogsyssetiing.h"
#include "dialogjiesuo.h"
#include "Tool/KeyBoard/keybd.h"
#include "mymessagebox.h"
#include "warninghistory.h"
#include "versionquery.h"

extern QByteArray temp;

SystemMaintence::SystemMaintence(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemMaintence)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    timer_systemaintence=new QTimer(this);
    connect(timer_systemaintence, SIGNAL(timeout()),this,SLOT(update_slot()));
    timer_systemaintence->start(100);

    ui->screencalibration_pb->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color:red}");
    ui->languageselection_pb->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color:white}");
    ui->programupdade_pb->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color:white}");
    ui->passwordsetting_pb->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color:white}");
    ui->paymentinstallment_pb->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color:white}");
    ui->systemlog_pb->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color:white}");
    ui->restorefactory_pb->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color:white}");
    ui->currentprogramversion_pb->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color:white}");
    ui->back_pb->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color:white}");

    buttun=new QPushButton();
    buttun_pre=new QPushButton();
    buttun_pre=buttun=ui->screencalibration_pb;
    visionShow=true;
    connect( ui->screencalibration_pb, SIGNAL(clicked()), this, SLOT(dealwithclick()));
    connect( ui->languageselection_pb, SIGNAL(clicked()), this, SLOT(dealwithclick()));
    connect( ui->programupdade_pb, SIGNAL(clicked()), this, SLOT(dealwithclick()));
    connect( ui->passwordsetting_pb, SIGNAL(clicked()), this, SLOT(dealwithclick()));
    connect( ui->paymentinstallment_pb, SIGNAL(clicked()), this, SLOT(dealwithclick()));
    connect( ui->systemlog_pb, SIGNAL(clicked()), this, SLOT(dealwithclick()));
    connect( ui->restorefactory_pb, SIGNAL(clicked()), this, SLOT(dealwithclick()));
    connect( ui->currentprogramversion_pb, SIGNAL(clicked()), this, SLOT(dealwithclick()));
    connect( ui->back_pb, SIGNAL(clicked()), this, SLOT(dealwithclick()));
    fenqiShowOrHide=true;
    ui->sysInfo_wd->hide();
}

SystemMaintence::~SystemMaintence()
{
    delete ui;
}

void  SystemMaintence::dealwithclick()
{
    buttun=(QPushButton*)sender();

    if(buttun->toolTip().toInt()==3)
    {
        updateSelect *select=new updateSelect;
        select->exec();
    }
    else if(buttun->toolTip().toInt()==9)
    {
        timer_systemaintence->stop();
        this->close();
    }
    else if(buttun->toolTip().toInt()==8)
    {
//        if(visionShow)
//        {
//             ui->visionMark_lb->setStyleSheet("QLabel{font-size:20px;color:white}");
//             ui->visionMark_lb->setText("RifaUWM_UI_HI_2006107");
//             visionShow=false;
//        }
//        else
//        {
//            ui->visionMark_lb->setText("");
//            visionShow=true;
//        }
        VersionQuery *version=new VersionQuery;
        version->exec();
    }
    else if(buttun->toolTip().toInt()==6)
    {
        SystemLog *systemlog=new SystemLog;
        systemlog->exec();
//        WarningHistory *history=new WarningHistory;
//        history->exec();
    }
    else if(buttun->toolTip().toInt()==2)
    {
        temp[0]=255;
        KeyBd *bd = new KeyBd;
        if(bd->exec()==QDialog::Accepted)
        {
            if(bd->inputstr == "1028331")
            {
                TimeSetting *setting= new TimeSetting;
                setting->exec();
            }
            else
            {
                MyMessageBox *box = new MyMessageBox(tr("提示"),tr("密码错误！"),0);
                box->exec();
            }
        }
        delete  bd;
    }
    else if(buttun->toolTip().toInt()==5)//fen qi fukuan
    {
        if(fenqiShowOrHide)
        {
            fenqiShowOrHide=false;
            ui->sysInfo_wd->show();
        }
        else
        {
            fenqiShowOrHide=true;
            ui->sysInfo_wd->hide();
        }
    }
    else if(buttun->toolTip().toInt()==1)
    {
        MyMessageBox *box = new MyMessageBox(tr("提示"),tr("校准屏幕需要重启系统，是否继续？"),1);
        if(box->exec()==QDialog::Accepted)
        {
            system("rm /etc/pointercal");
            system("sync");
        }
    }
}

void SystemMaintence::showColor()
{
     if(buttun_pre==buttun)
     {

         buttun->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color:red}");
     }
     else
     {

         buttun_pre->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color:white}");
         buttun->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color:red}");
     }
     buttun_pre=buttun;
}

void SystemMaintence::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)34)
        {
             temp[0]=255;
            buttun->click();
        }
        if(temp[0]==(char)12)//1
        {
            temp[0]=255;

        }
        if(temp[0]==(char)1)
        {
             temp[0]=255;
             on_sysSetting_btn_clicked();
        }
        else if(temp[0]==(char)2)
        {
             temp[0]=255;
             on_sysUnlock_btn_clicked();
        }
        if(temp[0]==(char)17)//2
        {
            temp[0] = 255;
            buttun = ui->languageselection_pb;
            showColor();

        }
        if(temp[0]==(char)22)//3
        {
            temp[0] = 255;
            buttun = ui->programupdade_pb;
            showColor();
        }
        if(temp[0]==(char)13)//4
        {
            temp[0]=255;

        }
        if(temp[0]==(char)18)//5
        {
            temp[0] = 255;
            buttun = ui->paymentinstallment_pb;
            showColor();

        }
        if(temp[0]==(char)23)//6
        {
            temp[0]=255;

        }
        if(temp[0]==(char)14)//7
        {
            temp[0]=255;

        }
        if(temp[0]==(char)19)//8
        {
            temp[0]=255;
            buttun = ui->currentprogramversion_pb;
            showColor();

        }
        if(temp[0]==(char)24)//9
        {
            temp[0]=255;
            buttun = ui->back_pb;
            showColor();
        }
        if(temp[0]==(char)37)
        {
            temp[0]=255;

        }
        if(temp[0]==(char)28)
        {
           temp[0]=255;
           ui->back_pb->click();
        }
        if(temp[0]==(char)35)
        {
            temp[0]=255;
            if(buttun->toolTip().toInt()>0)
            {
                int current =buttun->toolTip().toInt()+1;
                switch(current)
                {
                case 2:
                {
                    buttun=ui->languageselection_pb;
                    showColor();
                    break;
                }
                case 3:
                {
                    buttun=ui->programupdade_pb;
                    showColor();
                    break;
                }
                case 4:
                {
                    buttun=ui->passwordsetting_pb;
                    showColor();
                    break;
                }
                case 5:
                {
                    buttun=ui->paymentinstallment_pb;
                    showColor();
                    break;
                }
                case 6:
                {
                    buttun=ui->systemlog_pb;
                     showColor();
                    break;
                }
                case 7:
                {
                    buttun=ui->restorefactory_pb;
                     showColor();
                    break;
                }
                case 8:
                {
                    buttun=ui->currentprogramversion_pb;
                    showColor();
                    break;
                }
                case 9:
                {
                    buttun=ui->back_pb;
                    showColor();
                    break;
                }

                }
            }
        }
        if(temp[0]==(char)33)
        {
            temp[0]=255;
            if(buttun->toolTip().toInt()<=9)
            {
                int current =buttun->toolTip().toInt()-1;
                switch(current)
                {
                case 1:
                     {
                        buttun=ui->screencalibration_pb;
                        showColor();
                         break;
                     }
                case 2:
                     {
                        buttun=ui->languageselection_pb;
                         showColor();
                        break;
                     }
                case 3:
                      {
                        buttun=ui->programupdade_pb;
                         showColor();
                        break;
                      }
                case 4:
                       {
                         buttun=ui->passwordsetting_pb;
                          showColor();
                         break;
                       }
                case 5:
                       {
                         buttun=ui->paymentinstallment_pb;
                          showColor();
                         break;
                       }
                case 6:
                       {
                         buttun=ui->systemlog_pb;
                          showColor();
                         break;
                       }
                case 7:
                      {
                         buttun=ui->restorefactory_pb;
                          showColor();
                         break;
                      }
                case 8:
                      {
                         buttun=ui->currentprogramversion_pb;
                          showColor();
                         break;
                      }
                }
            }
        }
    }
}

/*pei zhi xi tong*/
void SystemMaintence::on_sysSetting_btn_clicked()
{
    temp[0]=255;
    KeyBd *bd = new KeyBd;
    if(bd->exec()==QDialog::Accepted)
    {
        if(bd->inputstr=="888888")
        {
            DialogSysSetiing *set=new DialogSysSetiing;
            set->exec();
        }
        else
        {
            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("密码错误！"),0);
            box->exec();
        }
    }
    delete bd;
}

/*jie suo */
void SystemMaintence::on_sysUnlock_btn_clicked()
{
    DialogJiesuo jiesuo;
    jiesuo.exec();
}
