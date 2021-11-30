#include "runninginsetting.h"
#include "ui_runninginsetting.h"
#include "myheader.h"
#include <QSettings>

extern QByteArray temp;

RunningInSetting::RunningInSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunningInSetting)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    initialForm();
}

RunningInSetting::~RunningInSetting()
{
    delete ui;
}

void RunningInSetting::initialForm()
{
    ui->label_remainingtime->setText("");
    ui->tlb_heatflag->setText(machine_Pars.heatFalg==0?tr("关闭"):tr("打开"));
    ui->tlb_runindays->setText(QString::number(runningIn.days));
    ui->tlb_runinspeed->setText(QString::number(machine_Pars.runInLimitSpeed));
    ui->tlb_heatclothnum->setText(QString::number(machine_Pars.heatClothNum));
    ui->tlb_tempre->setText(QString::number((float)(machine_Pars.heatTemperature/10.0)));
    ui->tlb_speedlimit->setText(QString::number(machine_Pars.heatLimtspeed));
    ui->tlb_tempre_2->setText(QString::number((float)(machine_Pars.heatTemperature2/10.0)));
    ui->tlb_speedlimit_2->setText(QString::number(machine_Pars.heatLimtspeed2));

    connect(ui->tlb_runindays,SIGNAL(clicked(bool)),this,SLOT(toolbuttonclicked()));
    connect(ui->tlb_heatclothnum,SIGNAL(clicked(bool)),this,SLOT(toolbuttonclicked()));
    connect(ui->tlb_speedlimit,SIGNAL(clicked(bool)),this,SLOT(toolbuttonclicked()));
    connect(ui->tlb_tempre,SIGNAL(clicked(bool)),this,SLOT(toolbuttonclicked()));
    connect(ui->tlb_speedlimit_2,SIGNAL(clicked(bool)),this,SLOT(toolbuttonclicked()));
    connect(ui->tlb_tempre_2,SIGNAL(clicked(bool)),this,SLOT(toolbuttonclicked()));
    connect(ui->tlb_runinspeed,SIGNAL(clicked(bool)),this,SLOT(toolbuttonclicked()));
    curbutton=ui->tlb_runindays;
    curbutton->click();
    timer1=new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(timeout()));
    timer1->start(100);
}


//返回
void RunningInSetting::on_pushButton_back_clicked()
{
    this->close();
}

void RunningInSetting::timeout()
{
    if(runningIn.mark)
    {
        int secs=runningIn.startTime+86400*runningIn.days-QDateTime::currentDateTime().toTime_t();
        ui->label_remainingtime->setText(tr("磨合期剩余时间 %1天%2小时").arg(secs/60/60/24).arg(secs%(24*60*60)/3600));
    }
    if(this->isActiveWindow()&&temp.length()>0)
    {
        quint8 key=temp[0];
        temp.clear();
        if(key==12)//1
        {
            nunmberInput("1");
        }
        else if(key==13)//4
        {
            nunmberInput("4");
        }
        else if(key==14)//7
        {
            nunmberInput("7");
        }
        else if(key==17)//2
        {
            nunmberInput("2");
        }
        else if(key==18)//5
        {
            nunmberInput("5");
        }
        else if(key==19)//8
        {
            nunmberInput("8");
        }
        else if(key==20)//0
        {
            nunmberInput("0");
        }
        else if(key==22)//3
        {
            nunmberInput("3");
        }
        else if(key==23)//6
        {
            nunmberInput("6");
        }
        else if(key==24)//9
        {
            nunmberInput("9");
        }
        else if(key==25)//.
        {
            nunmberInput(".");
        }
        else if(key==27)//del
        {
            nunmberInput("del");
        }
        else if(key==28||key==10)//esc
        {
            ui->pushButton_back->click();
        }
        else if(key==34||key==9)//ok
        {
            ui->pushButton_save->click();
        }
        else if(key==8)
        {
            ui->pushButton_start->click();
        }
    }
}

//保存
void RunningInSetting::on_pushButton_save_clicked()
{
    machine_Pars.runInLimitSpeed=ui->tlb_runinspeed->text().toInt();
    machine_Pars.heatClothNum=ui->tlb_heatclothnum->text().toInt();
    machine_Pars.heatTemperature=ui->tlb_tempre->text().toFloat()*10;
    machine_Pars.heatLimtspeed=ui->tlb_speedlimit->text().toInt();
    machine_Pars.heatTemperature2=ui->tlb_tempre_2->text().toFloat()*10;
    machine_Pars.heatLimtspeed2=ui->tlb_speedlimit_2->text().toInt();
    runningIn.days=ui->tlb_runindays->text().toInt();
    para_operation.machine_par_save();
    //保存磨合天数
    QSettings *set=new QSettings(Path_machine,QSettings::IniFormat);
    set->setValue("RunningIn/Days",runningIn.days);
    delete set;
    system("sync");
    ui->label_tips->setText(tr("保存成功！"));
}


void RunningInSetting::nunmberInput(QString str)
{
    QString btntext=curbutton->text();
    if(str=="del")
    {
        btntext="0";
    }
    else if(str==".")
    {
        if(curbutton->toolTip()=="1")
        {
            if(btntext.length()<=3)
            {
                if(btntext.contains(".")==false)
                {
                    btntext+=".";
                }
            }
        }
    }
    else if(str=="0")
    {
        if(btntext!="0"&&btntext.length()<5)
            btntext+=str;
    }
    else
    {
        if(btntext!="0"&&btntext.length()<5)
            btntext+=str;
        if(btntext=="0")
            btntext=str;
    }
    curbutton->setText(btntext);
}

//开始磨合
void RunningInSetting::on_pushButton_start_clicked()
{
    if(runningIn.mark)
    {
        ui->label_tips->setText(tr("磨合已开始，请勿重新开始！"));
        return;
    }
    QSettings *set=new QSettings(Path_machine,QSettings::IniFormat);
    runningIn.mark=true;
    set->setValue("RunningIn/Mark","1");
    runningIn.days=ui->tlb_runindays->text().toInt();
    set->setValue("RunningIn/Days",runningIn.days);
    runningIn.startTime=QDateTime::currentDateTime().toTime_t();
    set->setValue("RunningIn/StartTime",runningIn.startTime);
    delete set;
    system("sync");
}

void RunningInSetting::toolbuttonclicked()
{
    curbutton=(QToolButton*)sender();
    ui->tlb_runindays->setChecked(false);
    ui->tlb_speedlimit->setChecked(false);
    ui->tlb_tempre->setChecked(false);
    ui->tlb_runinspeed->setChecked(false);
    ui->tlb_tempre_2->setChecked(false);
    ui->tlb_speedlimit_2->setChecked(false);
    ui->tlb_heatclothnum->setChecked(false);
    curbutton->setChecked(true);
}

void RunningInSetting::on_tlb_heatflag_clicked()
{
    machine_Pars.heatFalg++;
    if(machine_Pars.heatFalg>1)
        machine_Pars.heatFalg=0;
    ui->tlb_heatflag->setText(machine_Pars.heatFalg==0?tr("关闭"):tr("打开"));
}
