#include "patternwoven.h"
#include "ui_patternwoven.h"
#include "mymessagebox.h"
#include <QLineEdit>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <QSettings>
#include <fcntl.h>
#include "stdlib.h"
#include "time.h"
#include "Windows/WIFI/xrwifi.h"
#include "Windows/Settings/qluoansetting.h"
#include "Windows/Settings/qluoanangle.h"
#include "Tool/KeyBoard/keybd.h"
#include "Windows/BTSRsetting/btsrmain.h"
#define IOM_PATH "/xrfile/iom.txt"


QByteArray temp;
int warningShow_mark;
int startToCompare;
extern int warning_times;
extern int downLoadPatn;
bool startNet;
QDateTime lastTime;
extern QString ui_name;
bool sysset_enable;

/*
 * -------------------全局变量初始化------------------
 * */
DoublePortRam Ram;
Real_timeParameter realtime_parameter;//实时参数
machine_Parameter machine_Pars;//机器参数
ParamterOperation para_operation;//参数文件操作类
system_Maintenance sys_maintenance;
KeyData serialPortKey;          //串口键盘键值
KeyData serialPortKeyTemp;      //
int startToTransmitPara;        //参数传递标志
int machineSize;
KnitState knitState;            //编织状态
RunningIn runningIn;            //机器磨合参数
QString projectCOName;//CO工程名称
bool zeroSetted=false;
bool updateUI=false;
bool b_sendPYF_Par=true;
QString b_headerUp_Down="";
/*-------------------------------------------------*/


PatternWoven::PatternWoven(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PatternWoven)
{
    ui->setupUi(this);
    if(ui_name=="RifaUWM_UI")
    {
        ui->toMenu_pb->setGeometry(60,5,221,45);
        ui->toMenu_pb->setStyleSheet("QPushButton{ border-image: url(:/img/logo.png); }QPushButton::disabled{background-color:transparent; }");
    }
    else if(ui_name=="ShunYiUWM_UI")
    {
        ui->toMenu_pb->setGeometry(60,5,151,45);
        ui->toMenu_pb->setStyleSheet("QPushButton{ border-image: url(:/img/jingwei.png); }QPushButton::disabled{background-color:transparent; }");
    }
    this->setWindowFlags(Qt::FramelessWindowHint);
    /*打开双口RAM*/
    Ram.DeviceSwitch=::open(Device_Location,O_RDWR);
//    if(Ram.DeviceSwitch==-1)
//    {
//        QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("驱动安装失败！请重试"),QMessageBox::Yes);
//    }
    /*打开键盘串口*/
    mycom.setPortName("ttyO1");
    mycom.setBaudRate(BAUD38400);
    if(!mycom.open(QIODevice::ReadWrite))
    {
//        QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("Port open failed"),QMessageBox::Yes);
    }
    connect(&mycom,SIGNAL(readyRead()),this,SLOT(getKeyComData()));

    //开机读取参数文件
    para_operation.machine_par_open();

    //样式表设置
    setStyleSheets();

    lastDate=QDate::currentDate();
    timer = new QTimer(this);//系统时间
    timer_paraupdate=new QTimer(this);//数据刷新
    connect(timer, SIGNAL(timeout()),this,SLOT(qtimeSlot()));
    connect(timer_paraupdate, SIGNAL(timeout()),this,SLOT(updatepara_slot()));

    //显示产量信息
    ui->output_lb->setText(QString("%1/%2").arg(machine_Pars.yield_current).arg(machine_Pars.yield_setted));

    /*定义报警窗口*/
    pWarningPatternwoven=new WarningPatternwoven(this);
    pWarningPatternwoven->setGeometry((800-pWarningPatternwoven->width())/2,(480-pWarningPatternwoven->height())/2,pWarningPatternwoven->width(),pWarningPatternwoven->height());
    pWarningPatternwoven->hide();
    connect(pWarningPatternwoven, SIGNAL(sendAlarm(QString,int,int)),
                this,SLOT(receiveWarning(QString,int,int)));
    connect(this, SIGNAL(sendCancleMark(bool)),pWarningPatternwoven,
            SLOT(getStopMark(bool)));
    connect(pWarningPatternwoven,SIGNAL(alarmclear()),this,SLOT(alarmClear()));

    connect(ui->textBrowser,SIGNAL(cursorPositionChanged()),this,SLOT(autoScroll()));
    ui->textBrowser->document()->setMaximumBlockCount(200);

    //界面初始
    initialForm();

    ui->label_warning->setVisible(false);
    ui->diswarn->setVisible(false);
    ui->label_temp->hide();
    ui->label_tempear->hide();

    //删除30天前报警历史
    deleteWarningHistory();
    //刷两侧功能按钮显示
    refreshButton();

    //读设备编号
    QSettings *set=new QSettings(IOM_PATH,QSettings::IniFormat);
    ui->systemID_lb->setText(set->value("WiFiParameter/MachineID","").toString());
    delete set;

    wifi = new WIFIMode;
    //更新设备编号
    connect(wifi,SIGNAL(update_MachineID()),this,SLOT(update_MachineID()));
    wifi->setModal(false);

//    m_pOrder = new NewOrder;
//    connect(m_pOrder,SIGNAL(single_stop_signal()),this,SLOT(single_stop_slot()));
    m_bFirst = true;
    single = QDateTime::currentDateTime();
    bCurrent_Outp_change = false;
    yieldUpdated=false;
    initialDynamicData();
#ifdef Q_OS_LINUX
    ui->widget_left->setVisible(true);
    ui->widget_right->setVisible(true);
#else
    ui->widget_left->setVisible(false);
    ui->widget_right->setVisible(false);
#endif
    ui->disnotouch->show();
    timer->start(10);
    timer_paraupdate->start(20);
}

void PatternWoven::initialForm()
{
    if(QFile::exists("/media/myusb/Helloworld"))
    {
        QFile file("/bin/Helloworld");
        if(file.open(QIODevice::ReadOnly))
        {
            QDataStream out(&file);
            out.device()->seek(12);
            quint32 version;
            out>>version;
            file.close();
            if(version!=1609232158)
            {
                file.setFileName("/media/myusb/Helloworld");
                if(file.open(QIODevice::ReadOnly))
                {
                    QDataStream out(&file);
                    out.device()->seek(12);
                    quint32 version;
                    out>>version;
                    file.close();
                    if(version==1609232158)
                    {
                        if(QFile::remove("/bin/Helloworld"))
                        {
                            if(QFile::copy("/media/myusb/Helloworld","/bin/Helloworld"))
                            {
                                system("sync");
                            }
                        }
                    }
                }
            }
        }
    }
    sysset_enable=false;
    knitState=knitNULL;
    //实时状态数据初始化
    realtime_parameter.curpulse=0;
    realtime_parameter.curneedle=0;
    realtime_parameter.isRunnging=false;
    simualted=false;
    resetBool=true;
    /*---------------------------------------*/

    warningShow_mark=1;
    startToCompare=1;
    downLoadPatn=1;
    startToTransmitPara=0;
    zeroSetted=false;
    cntProgress=-1;//连接状态（信号强度），4为连接成功
    savelog=true;
    HardwareError=false;
    motorsetting=false;
    IsFirstEnterMain=true;
    page_id=1;
    resetOrNot=0;
    timeCheck=0;
    shuttlestate[0]=true;
    shuttlestate[1]=true;
    shuttlestate[2]=true;
    lastTime=QDateTime::currentDateTime();
    time_t_can=QDateTime::currentDateTime().toTime_t();

    /*辅助判断变量*/
    Switch_ratelimit=true;//限速
    Switch_economy=true;//节约
    Switch_singlestop=true;//单停
    Switch_F4stop=true;    //F4停车
    Switch_fastforward=true;//快进
    Switch_blowfabric=true;//吹织
    Switch_alarm=true;     //报警
    Switch_stopchain=true;//停链
    Switch_outNeed = true;
    Switch_inNeed = true;
    Switch_oil=true;

    //开机时，读取单件时间
    QFile file_("/xrfile/SingleTime.txt");
    if(file_.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file_);
        in>>realtime_parameter.singletime;
        file_.close();
        if(realtime_parameter.singletime.size()>7)
        {
            ui->singletime_lb->setText(tr("1分0秒"));
        }
        else
        {
            ui->singletime_lb->setText(realtime_parameter.singletime);
        }
    }

    QFile fileSt(tr("/xrfile/").append("startTime.txt"));
    if(fileSt.open(QIODevice::ReadWrite))
    {
        QTextStream out(&fileSt);
        out>>sys_maintenance.starttime;
        fileSt.close();
    }

    /*分期付款，读取系统到期时间*/
    QFile fileEt(tr("/xrfile/").append("endingTime.txt"));
    if(fileEt.open(QIODevice::ReadWrite))
    {
        QTextStream out(&fileEt);
        out>>sys_maintenance.endtime;
        fileEt.close();
    }

    QFile filecompanyCode(tr("/xrfile/").append("companyCode.txt"));
    if(filecompanyCode.open(QIODevice::ReadWrite))
    {
        QTextStream out(&filecompanyCode);
        out>>sys_maintenance.CompanyCode;
        filecompanyCode.close();
    }

    QFile filecompanyCodeA(tr("/xrfile/").append("companyCodeAscii.txt"));
    if(filecompanyCodeA.open(QIODevice::ReadWrite))
    {
        QTextStream out(&filecompanyCodeA);
        out>>sys_maintenance.CompanyCodeAscii;
        filecompanyCodeA.close();
    }

    QFile filecompanyCodeAsciiLength(tr("/xrfile/").append("companyCodeAsciiLength.txt"));
    if(filecompanyCodeAsciiLength.open(QIODevice::ReadWrite))
    {
        QTextStream out(&filecompanyCodeAsciiLength);
        out>>sys_maintenance.CompanyCodeAsciiLength;
        filecompanyCodeAsciiLength.close();
    }

    //读取分期付款开启标志
    QFile fileSettingMark(tr("/xrfile/").append("SettingMark.txt"));
    if(fileSettingMark.open(QIODevice::ReadWrite))
    {
        QString str;
        QTextStream out(&fileSettingMark);
        out>>str;
        if(str=="1")
            sys_maintenance.setting_mark=true;
        else
            sys_maintenance.setting_mark=false;
        fileSettingMark.close();
    }

    /*读取机号，和主控发来的比对*/
    QFile fileId(tr("/xrfile/").append("id.txt"));
    if(fileId.open(QIODevice::ReadWrite))
    {
        QString str;
        QTextStream out(&fileId);
        out>>str;
        fileId.close();
        sys_maintenance.id=str;
        if(str=="")
        {
            for(int i=0;i<10;i++)
            {
                sys_maintenance.idInt[i]=0;
            }
        }
        else
        {
            for(int i=0;i<10;i++)
            {
                sys_maintenance.idInt[i] = str.left(1).toInt();
                str = str.remove(0,1);
            }
        }
    }



    //机型号显示
    realtime_parameter.machineNeedleType="12/1056";
    if(machine_Pars.needletype==0)
    {
        realtime_parameter.machineNeedleType="12/1056";
    }
    else if(machine_Pars.needletype==1)
    {
        realtime_parameter.machineNeedleType="13/1152";
    }
    else if(machine_Pars.needletype==2)
    {
       realtime_parameter.machineNeedleType="14/1248";
    }
    else if(machine_Pars.needletype==3)
    {
        realtime_parameter.machineNeedleType="14/1440";
    }
    else if(machine_Pars.needletype==4)
    {
        realtime_parameter.machineNeedleType="15/1344";
    }
    else if(machine_Pars.needletype==5)
    {
        realtime_parameter.machineNeedleType="16/1440";
    }
    else if(machine_Pars.needletype==6)
    {
        realtime_parameter.machineNeedleType="17/1536";
    }
    else if(machine_Pars.needletype==7)
    {
        realtime_parameter.machineNeedleType="15/1536";
    }
    else if(machine_Pars.needletype==8)
    {
        realtime_parameter.machineNeedleType="18/1632";
    }
    else if(machine_Pars.needletype==9)
    {
        realtime_parameter.machineNeedleType="13/1344";
    }
    else if(machine_Pars.needletype==10)
    {
        realtime_parameter.machineNeedleType="16/1632";
    }
    else if(machine_Pars.needletype==11)
    {
        realtime_parameter.machineNeedleType="17/1728";
    }
    else if(machine_Pars.needletype==12)
    {
        realtime_parameter.machineNeedleType="19/1728";
    }
    else if(machine_Pars.needletype==13)
    {
        realtime_parameter.machineNeedleType="14/1344";
    }
    else if(machine_Pars.needletype==14)
    {
        realtime_parameter.machineNeedleType="14/1152";
    }
    else if(machine_Pars.needletype==15)
    {
        realtime_parameter.machineNeedleType="13/1056";
    }
    else if(machine_Pars.needletype==16)
    {
        realtime_parameter.machineNeedleType="12/960";
    }
    else if(machine_Pars.needletype==17)
    {
        realtime_parameter.machineNeedleType="20/1824";
    }
    //机器寸数
    machineSize=realtime_parameter.machineNeedleType.left(2).toInt();
    ui->machinetype_pb->setText(realtime_parameter.machineNeedleType);

    //机器磨合
    QSettings *set=new QSettings(Path_machine,QSettings::IniFormat);
    runningIn.mark=set->value("RunningIn/Mark","0")=="1"?true:false;
    runningIn.days=set->value("RunningIn/Days","0").toInt();
    runningIn.startTime=set->value("RunningIn/StartTime","0").toUInt();
    //开机读取当前状态是否为升完级后的
    QString updated=set->value("UI/Updated","0").toString();
    if(updated=="1")
    {
        set->setValue("UI/Updated","0");
        showmessage(tr("人机程序升级成功！"));
        readPreOffPowerState();
        //拷贝最新程序到XRUI
        if(ui_name!="XRUI")
        {
            set->setValue("UI/Copyed","1");
            system("sync");
            QString filename=QString("/bin/%1").arg(ui_name);
            QString newname="/bin/XRUI";
            if(QFile::exists(filename))
            {
                if(QFile::exists(newname))
                    QFile::remove(newname);
                QFile::copy(filename,newname);
                system("sync");
            }
        }
    }
    else
    {
        updated=set->value("UI/Copyed","0").toString();
        if(updated=="0")
        {
            set->setValue("UI/Copyed","1");
            system("sync");
            QString filename=QString("/bin/%1").arg(ui_name);
            QString newname="/bin/XRUI";
            if(QFile::exists(filename))
            {
                if(QFile::exists(newname))
                    QFile::remove(newname);
                QFile::copy(filename,newname);
                system("sync");
            }
        }
        showmessage(tr("系统上电！"));
    }

    delete set;
    system("sync");

    /*定义分期付款对象*/
    pHirepurchase = new Hirepurchase;
    connect(pHirepurchase,SIGNAL(setExpireReminder(bool)),this,SLOT(getExpireReminder(bool)));
    taishazui=new ParameterSaveInfo(this);
    taishazui->hide();
    taishazui->setGeometry((800-taishazui->width())/2,(480-taishazui->height())/2,taishazui->width(),taishazui->height());
    taishazui->setShowText(tr("抬沙嘴状态..."));
}

//样式表
void PatternWoven::setStyleSheets()
{
    setButtonState(ui->reset_Pat_pb,false);
    setButtonState(ui->stopchain_Pat_lb,false);
    setButtonState(ui->economy_Pat_pb,false);
    setButtonState(ui->singlestop_Pat_pb,false);
    setButtonState(ui->liftshuttle_Pat_pb,false);

    setButtonState(ui->ratelimit_Pat_pb,false);
    setButtonState(ui->fastforward_Pat_pb,false);
    setButtonState(ui->blowfabric_Pat_pb,false);
    setButtonState(ui->alarm_Pat_pb,false);
    setButtonState(ui->nextPage_pb,false);

    setButtonState(ui->pressNeedle_pb,false);
    setButtonState(ui->circulation_pb,false);
    setButtonState(ui->PatternWoven_pb,false);//转速设置
    setButtonState(ui->flatclothing_pb,false);
    setButtonState(ui->Yield_pb,false);

    setButtonState(ui->F4stop_Pat_pb,false);
    setButtonState(ui->outNeedle_pb,false);
    setButtonState(ui->inNeedle_pb,false);
    setButtonState(ui->previousPage_pb,false);

    setButtonState(ui->luoan_pb,false);
    setButtonState(ui->GoGo_pb,false);
    setButtonState(ui->BTSR_pb,false);

    //设置两侧按钮大小和位置
    ui->pressNeedle_pb->setGeometry(0,5,55,91);//1
    ui->circulation_pb->setGeometry(0,195,55,91);//3
    ui->flatclothing_pb->setGeometry(0,290,55,91);//4
    ui->PatternWoven_pb->setGeometry(0,100,55,91);//2
    ui->Yield_pb->setGeometry(0,385,55,91);//5
    ui->luoan_pb->setGeometry(745,100,55,91);
    ui->luoanD_pb->setGeometry(745,5,55,91);
    ui->F4stop_Pat_pb->setGeometry(745,5,55,91);//6
    ui->previousPage_pb->setGeometry(745,290,55,91);
    ui->GoGo_pb->setGeometry(745,100,55,91);//8
    ui->outNeedle_pb->setGeometry(745,100,55,91);//9
    ui->inNeedle_pb->setGeometry(745,195,55,91);//10
}

/*析构函数*/
PatternWoven::~PatternWoven()
{
    system("sync");
    delete ui;
}

/*打开产量设置*/
void PatternWoven::on_output_lb_clicked()
{
    if(checkInterval()==false)
        return;
    YieldSetting *pYieldSetting=new YieldSetting;
    pYieldSetting->exec();
}

/*复位*/
void PatternWoven::on_reset_Pat_pb_clicked()
{
    if(checkInterval()==false)
        return;
    if(resetBool)
    {
        showmessage(tr("复位..."));
        setButtonState(ui->reset_Pat_pb,true);
        setButtonState(ui->stopchain_Pat_lb,false);
        setButtonState(ui->economy_Pat_pb,false);
        setButtonState(ui->fastforward_Pat_pb,false);
        setButtonState(ui->singlestop_Pat_pb,true);
        setButtonState(ui->outNeedle_pb,false);
        setButtonState(ui->inNeedle_pb,false);
        setButtonState(ui->flatclothing_pb,false);
        resetBool=false;
        Switch_singlestop=false;
        Switch_economy=true;
        Switch_stopchain=true;
        Switch_inNeed=true;
        Switch_outNeed=true;
        Switch_fastforward=true;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x05;
        Ram.SendData[3]=0x55;//设定
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
}

/*限速*/
void PatternWoven::on_ratelimit_Pat_pb_clicked()
{
    if(checkInterval()==false)
        return;

    if(Switch_ratelimit)
    {
        showmessage(tr("限速..."));
        setButtonState(ui->ratelimit_Pat_pb,true);
        Switch_ratelimit=false;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x01;
        Ram.SendData[3]=0x55;//设定
        Ram.SendData[4]=(quint8)machine_Pars.speed_limt;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else
    {
        showmessage(tr("限速手动取消"));
        setButtonState(ui->ratelimit_Pat_pb,false);
        Switch_ratelimit=true;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x01;
        Ram.SendData[3]=0xAA;//取消
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
}

/*停链*/
void PatternWoven::on_stopchain_Pat_lb_clicked()
{
    if(checkInterval()==false)
        return;
    //复位状态下不能使用
    if(resetBool==false)
        return;
    //节约状态下不能按停链
    if(Switch_economy==false)
        return;
    //全出针或全埋针状态不能按停链
    if(Switch_inNeed==false||Switch_outNeed==false)
        return;
    if(Switch_stopchain)
    {
        //判断当前步骤是否有循环指令 有循环指令才能按停链
        bool b=false;
        for(int i=0;i<50;i++)
        {
            if(machine_Pars.eco_stepstart[i]==realtime_parameter.curstep)
            {
                if(machine_Pars.eco_value[i]>0)
                {
                    b=true;break;
                }
            }
        }
        if(!b)
            return;
        Switch_fastforward=true;
        Switch_economy=true;
        Switch_stopchain=false;
        yieldUpdated=true;
        showmessage(tr("停链..."));
        setButtonState(ui->stopchain_Pat_lb,true);
        setButtonState(ui->economy_Pat_pb,false);
        setButtonState(ui->fastforward_Pat_pb,false);
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x02;
        Ram.SendData[3]=0x55;//设定
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else
    {
        showmessage(tr("停链手动取消！"));
        setButtonState(ui->stopchain_Pat_lb,false);
        Switch_stopchain=true;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x02;
        Ram.SendData[3]=0xAA;//设定
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
}

/*节约*/
void PatternWoven::on_economy_Pat_pb_clicked()
{
    if(checkInterval()==false)
        return;
    //复位状态下不能使用
    if(resetBool==false)
        return;
    //全出针或全埋针状态不能按节约
    if(Switch_inNeed==false||Switch_outNeed==false)
        return;
    if(Switch_economy)
    {
        Switch_stopchain=true;
        Switch_fastforward=true;
        Switch_economy=false;
        yieldUpdated=true;
        showmessage(tr("节约..."));
        setButtonState(ui->economy_Pat_pb,true);
        setButtonState(ui->fastforward_Pat_pb,false);
        setButtonState(ui->stopchain_Pat_lb,false);
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x03;
        Ram.SendData[3]=0x55;//设定
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else
    {
        showmessage(tr("节约手动取消！"));
        setButtonState(ui->economy_Pat_pb,false);
        Switch_economy=true;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x03;
        Ram.SendData[3]=0xAA;//设定
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
}

/*单停*/
void PatternWoven::on_singlestop_Pat_pb_clicked()
{
    if(checkInterval()==false)
        return;
    if(Switch_singlestop)
    {
        showmessage(tr("单停..."));
        setButtonState(ui->singlestop_Pat_pb,true);
        Switch_singlestop=false;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x04;
        Ram.SendData[3]=0x55;//设定
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();

    }
    else
    {
        showmessage(tr("单停手动取消！"));
        setButtonState(ui->singlestop_Pat_pb,false);
        Switch_singlestop=true;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x04;
        Ram.SendData[3]=0xAA;//cancel
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
}

/*快进*/
void PatternWoven::on_fastforward_Pat_pb_clicked()
{
    if(checkInterval()==false)
        return;
    //复位状态下不能使用
    if(resetBool==false)
        return;
    if(Switch_fastforward)
    {
        Switch_stopchain=true;
        Switch_economy=true;
        Switch_fastforward=false;
        yieldUpdated=true;
        showmessage(tr("快进..."));
        setButtonState(ui->fastforward_Pat_pb,true);
        setButtonState(ui->stopchain_Pat_lb,false);
        setButtonState(ui->economy_Pat_pb,false);
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x06;
        Ram.SendData[3]=0x55;//设定
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else
    {
        showmessage(tr("快进手动取消！"));
        setButtonState(ui->fastforward_Pat_pb,false);
        Switch_fastforward=true;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x06;
        Ram.SendData[3]=0xAA;//取消
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
}

/*吹织*/
void PatternWoven::on_blowfabric_Pat_pb_clicked()
{
    if(checkInterval()==false)
        return;
    if(Switch_blowfabric)
    {
        showmessage(tr("吹织..."));
        setButtonState(ui->blowfabric_Pat_pb,true);
        Switch_blowfabric=false;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x07;
        Ram.SendData[3]=0x55;//设定
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else
    {
        showmessage(tr("吹织手动取消！"));
        setButtonState(ui->blowfabric_Pat_pb,false);
        Switch_blowfabric=true;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x07;
        Ram.SendData[3]=0xAA;//设定
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
}

/*全出针*/
void PatternWoven::on_outNeedle_pb_clicked()
{
    if(checkInterval()==false)
        return;
//    //复位状态下不能操作
//    if(resetBool==false)
//        return;
    if(Switch_outNeed)
    {
        Switch_inNeed = true;
        Switch_outNeed = false;
        Switch_stopchain=true;
        //全出针状态下消除节约
        Switch_economy=true;
        setButtonState(ui->economy_Pat_pb,false);
        setButtonState(ui->outNeedle_pb,true);
        setButtonState(ui->inNeedle_pb,false);
        setButtonState(ui->flatclothing_pb,false);
        setButtonState(ui->stopchain_Pat_lb,false);
        showmessage(tr("全出针..."));
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x07;
        Ram.SendData[3]=0x02;
        Ram.SendData[4]=0x55;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else
    {
        Switch_outNeed = true;
        Switch_stopchain=true;
        setButtonState(ui->outNeedle_pb,false);
        showmessage(tr("全出针手动取消！"));
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x07;
        Ram.SendData[3]=0x02;
        Ram.SendData[4]=0xAA;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }

}

/*全埋针*/
void PatternWoven::on_inNeedle_pb_clicked()
{
    if(checkInterval()==false)
        return;
//    //复位状态下不能操作
//    if(resetBool==false)
//        return;
    if(Switch_inNeed)
    {
        Switch_outNeed = true;
        Switch_inNeed = false;
        Switch_stopchain=true;
        //全埋针状态下消除节约
        Switch_economy=true;
        setButtonState(ui->economy_Pat_pb,false);
        setButtonState(ui->outNeedle_pb,false);
        setButtonState(ui->inNeedle_pb,true);
        setButtonState(ui->flatclothing_pb,false);
        showmessage(tr("全埋针..."));
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x07;
        Ram.SendData[3]=0x01;
        Ram.SendData[4]=0x55;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else
    {
        Switch_inNeed = true;
        setButtonState(ui->inNeedle_pb,false);
        showmessage(tr("全埋针手动取消！"));
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x07;
        Ram.SendData[3]=0x01;
        Ram.SendData[4]=0xAA;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
}

/*在提示栏显示报警*/
void PatternWoven::receiveWarning(QString str, int code, int bite)
{
    if(str!="")
    {
        if(warning_times==0)
        {
            savelog=false;
            showmessage(str);
            savelog=true;
            wifi->myWiFi->writeAlarm(code,bite);
            warning_times=1;
        }
        pWarningPatternwoven->show();
    }
}

/*报警*/
void PatternWoven::on_alarm_Pat_pb_clicked()
{
    if(checkInterval()==false)
        return;
    /*
     * 报警消除机制：首先在0x303上写0xAA，UI读到数据后往相应位置写0x55
     * */
    if(Switch_alarm)
    {
        emit sendCancleMark(true);
        pWarningPatternwoven->hide();
        Ram.Send(0xAA,0x303);
        showmessage(tr("报警屏蔽！"));
        setButtonState(ui->alarm_Pat_pb,true);
        Switch_alarm=false;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x08;
        Ram.SendData[3]=0x55;//报警无效
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else
    {
        showmessage(tr("报警打开！"));
        emit sendCancleMark(false);
        setButtonState(ui->alarm_Pat_pb,false);
        Switch_alarm=true;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x08;
        Ram.SendData[3]=0xAA;//报警有效
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
}

//F4停车
void PatternWoven::on_F4stop_Pat_pb_clicked()
{
    if(checkInterval()==false)
        return;
    if(Switch_F4stop)
    {
        showmessage(tr("F4停车..."));
        setButtonState(ui->F4stop_Pat_pb,true);
        Switch_F4stop=false;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x09;
        Ram.SendData[3]=0x55;//设定
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else
    {
        showmessage(tr("F4停车手动取消！"));
        setButtonState(ui->F4stop_Pat_pb,false);
        Switch_F4stop=true;
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x09;
        Ram.SendData[3]=0xAA;//取消
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
}

//抬纱嘴
void PatternWoven::on_liftshuttle_Pat_pb_clicked()
{
    if(checkInterval()==false)
        return;
    ShuttleChoose *pShuttleChoose=new ShuttleChoose;
    if(taishazui->isHidden()==false)
        taishazui->hide();
    bool b=false;
    if(shuttlestate[0]==false)
    {
        b=true;
        pShuttleChoose->setShuttleState(1);
    }
    if(shuttlestate[1]==false)
    {
        b=true;
        pShuttleChoose->setShuttleState(2);
    }
    if(shuttlestate[2]==false)
    {
        b=true;
        pShuttleChoose->setShuttleState(3);
    }
    if(!b)
        showmessage(tr("抬纱嘴..."));
    pShuttleChoose->exec();
    shuttlestate[0]=true;
    shuttlestate[1]=true;
    shuttlestate[2]=true;
    showmessage(tr("纱嘴状态回归..."));
}

/*键盘定时器*/
void PatternWoven::update_keyboard()
{
    if(this->isActiveWindow()&&temp.length()>0)
    {
        while(temp.length()>0)
        {
            quint8 key=temp[0];
            temp.remove(0,1);
            if(key==26)
            {
                ui->GoGo_pb->click();//加油单击事件
            }
            else if(key==71)
            {
                if(realtime_parameter.head_position==1)
                    showmessage(tr("机头已在上限位！"));
                else
                    showmessage(tr("升机头..."));
                Ram.SendData[0]=0x20;
                Ram.SendData[1]=0xA2;
                Ram.SendData[2]=0x01;
                Ram.SendData[3]=0x55;
                Ram.SendData[4]=0x00;
                Ram.SendData[5]=0x00;
                Ram.SendData[6]=0x00;
                Ram.SendData[7]=0x00;
                Ram.SendData[8]=0x00;
                Ram.SendData[9]=0x00;
                Ram.UI_to_RTC();
            }
            else if(key==11)
            {
                if(realtime_parameter.head_position!=1)
                    showmessage(tr("升机头取消！"));
                Ram.SendData[0]=0x20;
                Ram.SendData[1]=0xA2;
                Ram.SendData[2]=0x01;
                Ram.SendData[3]=0xaa;
                Ram.SendData[4]=0x00;
                Ram.SendData[5]=0x00;
                Ram.SendData[6]=0x00;
                Ram.SendData[7]=0x00;
                Ram.SendData[8]=0x00;
                Ram.SendData[9]=0x00;
                Ram.UI_to_RTC();
            }
            else if(key==76)
            {
                if(realtime_parameter.head_position==3)
                    showmessage(tr("机头已在下限位！"));
                else
                    showmessage(tr("降机头..."));
                Ram.SendData[0]=0x20;
                Ram.SendData[1]=0xA2;
                Ram.SendData[2]=0x02;
                Ram.SendData[3]=0x55;
                Ram.SendData[4]=0x00;
                Ram.SendData[5]=0x00;
                Ram.SendData[6]=0x00;
                Ram.SendData[7]=0x00;
                Ram.SendData[8]=0x00;
                Ram.SendData[9]=0x00;
                Ram.UI_to_RTC();
            }
            else if(key==16)
            {
                if(realtime_parameter.head_position!=3)
                    showmessage(tr("降机头取消！"));
                Ram.SendData[0]=0x20;
                Ram.SendData[1]=0xA2;
                Ram.SendData[2]=0x02;
                Ram.SendData[3]=0xaa;
                Ram.SendData[4]=0x00;
                Ram.SendData[5]=0x00;
                Ram.SendData[6]=0x00;
                Ram.SendData[7]=0x00;
                Ram.SendData[8]=0x00;
                Ram.SendData[9]=0x00;
                Ram.UI_to_RTC();
            }
            else if(key==10)
            {
                if(page_id==1||page_id==2)
                {
                    ui->nextPage_pb->click();//F9下一页
                }
                else if(page_id==3)
                {
                    ui->previousPage_pb->click();
                }
            }
            else if(key==21)
            {
                if(ui->widget_left->isVisible())
                {
                    ui->widget_left->setVisible(false);
                    ui->widget_right->setVisible(false);
                    ui->disnotouch->hide();
                }
                else
                {
                    ui->widget_left->setVisible(true);
                    ui->widget_right->setVisible(true);
                    ui->disnotouch->show();
                }
            }
            else if(key==34)
            {
                Ram.Send(0xAA,0x303);
                pWarningPatternwoven->hide();//OK键
            }
            else if(key==72)
            {
                ui->pressNeedle_pb->click();//组合键INSERT+1，压针微调
            }
            else if(key==77)
            {
                ui->PatternWoven_pb->click();//组合键INSERT+2，转速设置
            }
            else if(key==78)
            {
                ui->circulation_pb->click();//INSERT+5，节约调整
            }
            else if(key==82)
            {
                ui->inNeedle_pb->click();//INSERT+3，全埋针
            }
            else if(key==73)
            {
                ui->flatclothing_pb->click();
            }
            else if(key==83)
            {
                ui->output_lb->click();
            }
            else if(key==74)
            {
                shuttlechanged(0);
            }
            else if(key==79)
            {
                shuttlechanged(1);
            }
            else if(key==84)
            {
                shuttlechanged(2);
            }
            else if(key==1)
            {
                if(page_id==1)
                {
                    ui->reset_Pat_pb->click();
                }
                else if(page_id==2)
                {
                    ui->pressNeedle_pb->click();
                }

            }
            else if(key==2)
            {
                if(page_id==1)
                {
                    ui->stopchain_Pat_lb->click();
                }
                else if(page_id==2)
                {
                    ui->PatternWoven_pb->click();
                }

            }
            else if(key==3)
            {
                if(page_id==1)
                {
                    ui->economy_Pat_pb->click();
                }
                else if(page_id==2)
                {
                    ui->circulation_pb->click();
                }

            }
            else if(key==4)
            {
                if(page_id==1)
                {
                    ui->singlestop_Pat_pb->click();
                }
                else if(page_id==2)
                {
                    ui->flatclothing_pb->click();
                }
            }
            else if(key==5)
            {
                if(page_id==1)
                {
                    ui->liftshuttle_Pat_pb->click();
                }
                else if(page_id==2)
                {
                    ui->Yield_pb->click();
                }

            }
            else if(key==6)
            {
                if(page_id==1)
                {
                    ui->ratelimit_Pat_pb->click();
                }
                else if(page_id==2)
                {
                    ui->F4stop_Pat_pb->click();
                }
                else if(page_id==3)
                {
                    ui->luoanD_pb->click();
                }
            }
            else if(key==7)
            {
                if(page_id==1)
                {
                    ui->fastforward_Pat_pb->click();
                }
                else if(page_id==2)
                {
                    ui->outNeedle_pb->click();//全出针
                }
                else if(page_id==3)
                {
                    ui->luoan_pb->click();//裸氨
                }
            }
            else if(key==8)
            {
                if(page_id==1)
                {
                    ui->blowfabric_Pat_pb->click();
                }
                else if(page_id==2)
                {
                    ui->inNeedle_pb->click();//全埋针
                }
                else if(page_id==3)
                {
                    ui->GoGo_pb->click();//加油
                }
            }
            else if(key==9)
            {
                if(page_id==1)
                {
                    ui->alarm_Pat_pb->click();
                }
                else if(page_id==2)
                {
                    ui->previousPage_pb->click();//上一页
                }
                else if(page_id==3)
                {
                    ui->BTSR_pb->click();
                }
            }
            else  if(key==36)
            {
                ui->toMenu_pb->click();
            }
        }
    }
    else if(motorsetting&&warningShow_mark==1)
    {
        if(temp[0]==(char)74)
        {
            temp[0]=255;
            shuttlechanged(0);
        }
        else if(temp[0]==(char)79)
        {
            temp[0]=255;
            shuttlechanged(1);
        }
        else if(temp[0]==(char)84)
        {
            temp[0]=255;
            shuttlechanged(2);
        }
    }
}

/*系统时间槽函数和系统实时显示参数的刷新*/
//10ms执行一次
void PatternWoven::qtimeSlot()
{
    timeCheck++;
    //每5分钟同步一次系统时间并且备份系统参数文件
    if(timeCheck==30000)
    {
        timeCheck=0;
        system("hwclock --hctosys");
        if(QFile::exists(Path_parameterfile))
        {
            if(QFile::exists(Path_parameterfile_backup))
                QFile::remove(Path_parameterfile_backup);
            if(QFile::copy(Path_parameterfile,Path_parameterfile_backup))
            {
                system("sync");
            }
        }
    }

    //开机时下发系统参数请求
    if(startToTransmitPara==0)
    {
        if(IsFirstEnterMain)
        {
            if(Ram.Receive(0x300)==0x55)//UI接收中断
            {
                if(Ram.Receive(0x200)==0xA0)
                {
                    if(Ram.Receive(0x201)==0x20)
                    {
                        if(Ram.Receive(0x202)==0x00)//开机准备
                        {
                            if(Ram.Receive(0x203)==0x00)//无机号核对
                            {
                                //人机有机号
                                if(sys_maintenance.setting_mark)
                                {
                                    MachineConfigure *machine=new MachineConfigure;
                                    machine->setconfigureType(typeui);
                                    if(machine->exec()==QDialog::Accepted)
                                    {
                                        sendAllPara();
                                        HardwareError=false;
                                    }
                                    else
                                    {
                                        sendAllPara();
                                        HardwareError=true;
                                    }
                                }
                                else
                                {
                                    sendAllPara();
                                    HardwareError=false;
                                }
                            }
                            else if(Ram.Receive(0x203) ==0x55)//机号核对
                            {
                                if(Ram.Receive(0x204) ==sys_maintenance.idInt[0]
                                        &&Ram.Receive(0x205)==sys_maintenance.idInt[1]
                                        &&Ram.Receive(0x206)==sys_maintenance.idInt[2]
                                        &&Ram.Receive(0x207)==sys_maintenance.idInt[3]
                                        &&Ram.Receive(0x208)==sys_maintenance.idInt[4]
                                        &&Ram.Receive(0x209)==sys_maintenance.idInt[5]
                                        &&Ram.Receive(0x20a)==sys_maintenance.idInt[6]
                                        &&Ram.Receive(0x20b)==sys_maintenance.idInt[7]
                                        &&Ram.Receive(0x20c)==sys_maintenance.idInt[8]
                                        &&Ram.Receive(0x20d)==sys_maintenance.idInt[9])
                                {
                                    sendAllPara();
                                    HardwareError=false;
                                }
                                else
                                {
                                    MachineConfigure *machine=new MachineConfigure;
                                    machine->setconfigureType(typertc);
                                    QString id="";
                                    for(int i=0;i<10;i++)
                                    {
                                        id+=QString::number(Ram.Receive(0x204+i));
                                    }
                                    machine->setID(id);
                                    if(machine->exec()==QDialog::Accepted)
                                    {
                                        sendAllPara();
                                        HardwareError=false;
                                    }
                                    else
                                    {
                                        sendAllPara();
                                        HardwareError=true;
                                    }
                                }
                            }
                            sysset_enable=true;
                        }
                    }
                }
            }
        }
        else
            sendAllPara();
    }

    //下发系统参数
    if(startToTransmitPara==1)
    {
        if(sendAllSystemParameter)
            sendAllPara();
        else
        {
            sendingTimeCount++;
            //系统参数传递超时
            if(sendingTimeCount>100)
            {
                startToTransmitPara=5;
                toPatternWoven();
                return;
            }
            if(Ram.Receive(0x300)==0x55)//UI接收中断
            {
                if(Ram.Receive(0x200)==0x40)//参数传递指令
                {
                    if(Ram.Receive(0x201)==0x52)//传递请求
                    {
                        if(Ram.Receive(0x202)==0xFF)//所有参数
                        {
                            if(Ram.Receive(0x203)==0x00)
                            {
                                Ram.Send(0xAA,0x300);//UI数据接收完，向0x300处写0xAA,清除中断

                                QFile file_transmit(Path_parameterfile);
                                quint16 length=parameter_Count*2;
                                if(file_transmit.open(QIODevice::ReadOnly))
                                {
                                    QDataStream in(&file_transmit);
                                    long unsigned int addr=0x500;

                                    qint16 data_;qint8 DATA[length];
                                    for(int pos=0;pos<parameter_Count;pos++)
                                    {
                                        in>>data_;
                                        DATA[2*pos]=data_&0x00ff;//0 2  ...1606，低8位
                                        DATA[2*pos+1]=(data_&0xff00)>>8;//1 3   1607，高8位
                                    }
                                    file_transmit.close();
                                    for(int i=0;i<length;i++)
                                    {
                                        Ram.Send(DATA[i],addr);
                                        addr++;
                                    }
                                }
                                Ram.SendData[0]=0x40;
                                Ram.SendData[1]=0x51;
                                Ram.SendData[2]=0xff;
                                Ram.SendData[3]=0x00;
                                Ram.SendData[4]=0x01;//包数L
                                Ram.SendData[5]=0x00;//包数H
                                Ram.SendData[6]=length%256;//字节数L
                                Ram.SendData[7]=length/256;//字节数H
                                Ram.SendData[8]=0x00;
                                Ram.SendData[9]=0x00;
                                Ram.UI_to_RTC();
                            }
                        }
                    }
                }
            }
            else if(Ram.Receive(0x300)==0xcc)//传递成功
            {
                Ram.Send(0xAA,0x300);
                bool b=false;
//                ui->label->setText("编织主界面");
                QFile file(Path_parameterfile);
                if(file.open(QIODevice::ReadOnly))
                {
                    QDataStream in(&file);
                    quint16 length=parameter_Count*2;
                    quint16 data_;quint8 par[length];
                    for(int i=0;i<parameter_Count;i++)
                    {
                        in>>data_;
                        par[2*i]=data_&0x00ff;
                        par[2*i+1]=(data_&0xff00)>>8;
                    }
                    file.close();
                    int addr=0x500;
                    for(int i=0;i<parameter_Count;i++)
                    {
                        if(par[i]!=Ram.Receive(addr))
                        {
                            b=true;
                            break;
                        }
                        addr++;
                    }
                }
                if(b)
                    Ram.SendData[2]=0x01;
                else
                    Ram.SendData[2]=0x00;
                Ram.SendData[0]=0x40;
                Ram.SendData[1]=0x53;
                Ram.SendData[3]=0x00;
                Ram.SendData[4]=0x00;
                Ram.SendData[5]=0x00;
                Ram.SendData[6]=0x00;
                Ram.SendData[7]=0x00;
                Ram.SendData[8]=0x00;
                Ram.SendData[9]=0x00;
                Ram.UI_to_RTC();
                //开机时需要传递裸氨参数
                if(b_sendPYF_Par)
                {
                    b_sendPYF_Par=false;
                    //系统参数传递完成后下发裸氨参数
                    sendAllPara();
                }
                else
                {
                    startToTransmitPara=4;
                }
            }
            else if(Ram.Receive(0x300)==0xee)//传递失败
            {
                Ram.Send(0xAA,0x300);
                MyMessageBox *box=new MyMessageBox(tr("提示"),tr("传输失败！"),0);
                box->exec();
                startToTransmitPara=4;
            }
        }
    }
    else if(startToTransmitPara==2)//下发裸氨参数
    {
        if(sendAllSystemParameter)
        {
            if(IsFirstEnterMain)
                toPatternWoven();
            else
                startToTransmitPara=4;
        }
        else
        {
            sendingTimeCount++;
            if(sendingTimeCount>200)
            {
//                showmessage("裸氨参数传输超时！",mesgtime);
                if(IsFirstEnterMain)
                    toPatternWoven();
                else
                    startToTransmitPara=4;
                return;
            }
            if(Ram.Receive(0x300)==0x55)
            {
                if(Ram.Receive(0x200)==0x50)
                {
                    if(Ram.Receive(0x201)==0x52)
                    {
                        if(Ram.Receive(0x202)==0x00)
                        {
                            Ram.Send(0xAA,0x300);
                            sendingTimeCount=0;
                            for(int i=0;i<2000;i++)
                            {

                            }

                            QFile file_transmit("/xrfile/Parameter_Luoan.er");
                            file_transmit.open(QIODevice::ReadWrite);
                            int length=file_transmit.size();
                            int pack=length/2560;
                            if((length%2560)!=0)
                                pack=length/2560+1;
                            int baoshu=Ram.Receive(0x203)+Ram.Receive(0x204)*256;
                            int lastpack=2560;
                            if(baoshu==pack-1)
                            {
                                lastpack= length-(pack-1)*2560;
                            }
                            QDataStream in(&file_transmit);
                            in.device()->seek(baoshu*2560);
                            for(int j=0;j<lastpack;j++)
                            {
                                quint8 s;
                                in>>s;
                                Ram.Send(s,0x500+j);
                            }
                            file_transmit.close();
                            Ram.SendData[0]=0x50;
                            Ram.SendData[1]=0x51;
                            Ram.SendData[2]=0x00;
                            Ram.SendData[3]=baoshu%256;//总包数L
                            Ram.SendData[4]=baoshu/256;//总包数H
                            Ram.SendData[5]=lastpack%256;
                            Ram.SendData[6]=lastpack/256;
                            Ram.SendData[7]=0x00;
                            Ram.SendData[8]=0x00;
                            Ram.SendData[9]=0x00;
                            Ram.UI_to_RTC();
                        }
                    }
                }
            }
            else if(Ram.Receive(0x300)==0xcc)
            {
                Ram.Send(0xAA,0x300);
//                showmessage("裸氨参数传输成功！",mesgtime);
                if(IsFirstEnterMain)
                    toPatternWoven();
                else
                    startToTransmitPara=4;
            }
            else if(Ram.Receive(0x300)==0xEE)
            {
                Ram.Send(0xAA,0x300);
                if(IsFirstEnterMain)
                    toPatternWoven();
                else
                    startToTransmitPara=4;
                MyMessageBox *box=new MyMessageBox(tr("提示"),tr("传输失败！"),0);
                box->exec();
            }
        }
    }
}

//传递所有参数
void PatternWoven::sendAllPara()
{
    //系统参数传递
    if(startToTransmitPara==0)
    {
        Ram.Send(0xAA,0x300);
        //切换到机器设置界面
        Ram.SendData[0]=0xa0;
        Ram.SendData[1]=0x20;
        Ram.SendData[2]=0x04;//机器设置
        Ram.SendData[3]=0x00;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();

        usleep(50000);

        quint16 length=parameter_Count*2;
        Ram.SendData[0]=0x40;//参数传递指令
        Ram.SendData[1]=0x50;//功能代码，传递功能
        Ram.SendData[2]=0xFF;//传递所有参数
        Ram.SendData[3]=0x00;
        Ram.SendData[4]=0x01;//总包数L
        Ram.SendData[5]=0x00;//总包数H
        Ram.SendData[6]=length%256;//总字节数L
        Ram.SendData[7]=length/256;//总字节数H
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
        sendingTimeCount=0;
        sendAllSystemParameter=false;
    }
    else if(startToTransmitPara==1)//裸氨参数传递
    {
        QFile file_transmit("/xrfile/Parameter_Luoan.er");
        int length=file_transmit.size();
        if(length>0)
        {
            int pack=length/2560;
            if((length%2560)!=0)
                pack=length/2560+1;
            Ram.Send(0xAA,0x300);

            Ram.SendData[0]=0x50;
            Ram.SendData[1]=0x50;
            Ram.SendData[2]=0x00;
            Ram.SendData[3]=pack%256;//总包数L
            Ram.SendData[4]=pack/256;//总包数H
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
            sendingTimeCount=0;
            sendAllSystemParameter=false;
        }
        else
        {
            sendAllSystemParameter=true;
        }
    }
    startToTransmitPara++;
}

//切换到正常编织界面
void PatternWoven::toPatternWoven()
{
    startToTransmitPara++;
    if(IsFirstEnterMain)
    {
        IsFirstEnterMain=false;
        Ram.to_PatternWoven_slot();//切换到花型编织界面
        usleep(50000);
        //系统主要编织界面——正常编织，开机默认界面
        if(!HardwareError)
        {
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA1;
            Ram.SendData[2]=0x00;
            Ram.SendData[3]=0x00;
            Ram.SendData[4]=0x00;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            //开启磨合状态
            if(runningIn.mark)
            {
                //磨合期已到期
                if(QDateTime::currentDateTime().toTime_t()>runningIn.startTime+86400*runningIn.days)
                {
                    QSettings *set=new QSettings(Path_machine,QSettings::IniFormat);
                    runningIn.mark=false;
                    set->setValue("RunningIn/Mark","0");
                    delete set;
                    system("sync");
                }
                else
                {
                    Ram.SendData[4]=0x01;
                    Ram.SendData[5]=0x55;
                }
            }
            Ram.UI_to_RTC();
        }
        else
        {
            //硬件错误
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA1;
            Ram.SendData[2]=0x00;
            Ram.SendData[3]=0x55;
            Ram.SendData[4]=0x00;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
        }
    }
}

//状态反馈
void PatternWoven::statusFeedBack()
{
    if(Ram.Receive(0x300)==0x55)
    {
        if(Ram.Receive(0x200)==0x20)
        {
            if((Ram.Receive(0x201)==0xA3))
            {
                if(Ram.Receive(0x202)==0x01)//压针电机实时值
                {
                    Ram.Send(0xAA,0x300);
                    int addr=0x350;
                    for(int i=0;i<8;i++,addr+=2)
                    {
                        realtime_parameter.motorstate[i]=Ram.GetU16Data(addr);
                    }

                    ui->pressneedle_1_le->setText(QString::number(realtime_parameter.motorstate[0]));
                    ui->pressneedle_2_le->setText(QString::number(realtime_parameter.motorstate[1]));
                    ui->pressneedle_3_le->setText(QString::number(realtime_parameter.motorstate[2]));
                    ui->pressneedle_4_le->setText(QString::number(realtime_parameter.motorstate[3]));
                    ui->pressneedle_5_le->setText(QString::number(realtime_parameter.motorstate[4]));
                    ui->pressneedle_6_le->setText(QString::number(realtime_parameter.motorstate[5]));
                    ui->pressneedle_7_le->setText(QString::number(realtime_parameter.motorstate[6]));
                    ui->pressneedle_8_le->setText(QString::number(realtime_parameter.motorstate[7]));

                }
                else if(Ram.Receive(0x202)==0x02)//续织
                {
                    if(Ram.Receive(0x203)==0x55)
                    {
                        Ram.Send(0xaa,0x300);
                        if(readPreOffPowerState()==1)
                        {
                            showmessage(tr("掉电续织！"));
                        }
                        else
                        {
                            usleep(200000);
                            on_reset_Pat_pb_clicked();
                        }
                    }
                    else if((unsigned char)Ram.Receive(0x203)==0xAA)
                    {
                       Ram.Send(0xaa,0x300);
                       on_reset_Pat_pb_clicked();
                    }
                    pHirepurchase->startHirpurchase(sys_maintenance.endtime);
                }
            }
            else if(Ram.Receive(0x201)==0xA1)//编织状态反馈
            {
                if(Ram.Receive(0x202)==0x02)//停链状态反馈
                {
                    if(Ram.Receive(0x203)==0x00)
                    {
                        if(Ram.Receive(0x204)==0xAA)
                        {
                            Ram.Send(0xAA,0x300);
                            setButtonState(ui->stopchain_Pat_lb,false);
                            Switch_stopchain=true;
                        }
                    }
                }
                else if(Ram.Receive(0x202)==0x03)//节约状态反馈
                {
                    if(Ram.Receive(0x203)==0x00)
                    {
                        if(Ram.Receive(0x204)==0xAA)
                        {
                            Ram.Send(0xAA,0x300);
                            showmessage(("节约自动取消！"));
                            setButtonState(ui->economy_Pat_pb,false);
                            setButtonState(ui->singlestop_Pat_pb,false);
                            Switch_economy=true;
                            Switch_singlestop=true;
                        }
                    }
                }
                else if(Ram.Receive(0x202)==0x04)//单停状态反馈
                {
                    if(Ram.Receive(0x203)==0x00)
                    {
                        if(Ram.Receive(0x204)==0xAA)
                        {
                            Ram.Send(0xAA,0x300);
                            showmessage(tr("单停自动取消！"));
                            setButtonState(ui->singlestop_Pat_pb,false);
                            setButtonState(ui->economy_Pat_pb,false);
                            Switch_singlestop=true;
                            Switch_economy=true;
                        }
                    }
                }
                else if(Ram.Receive(0x202)==0x05)//复位状态反馈
                {
                    if(Ram.Receive(0x203)==0x00)
                    {
                        if(Ram.Receive(0x204)==0xAA)
                        {
                            Ram.Send(0xAA,0x300);
                            setButtonState(ui->reset_Pat_pb,false);
                            setButtonState(ui->singlestop_Pat_pb,false);
                            setButtonState(ui->blowfabric_Pat_pb,false);
                            Switch_singlestop=true;
                            Switch_blowfabric=true;
                            resetBool=true;
                            single = QDateTime::currentDateTime();
                            showmessage(tr("复位自动取消！"));
                        }
                    }
                }
                else if(Ram.Receive(0x202)==0x06)//快进状态反馈
                {
                    if(Ram.Receive(0x203)==0x00)
                    {
                        if(Ram.Receive(0x204)==0xAA)
                        {
                            Ram.Send(0xAA,0x300);
                            setButtonState(ui->fastforward_Pat_pb,false);
                            Switch_fastforward=true;
                            showmessage(tr("快进自动取消！"));
                        }
                    }
                }
                else if(Ram.Receive(0x202)==0x07)//吹织状态反馈
                {
                    if(Ram.Receive(0x203)==0x00)
                    {
                        if(Ram.Receive(0x204)==0xAA)
                        {
                            Ram.Send(0xAA,0x300);
                            setButtonState(ui->blowfabric_Pat_pb,false);
                            Switch_blowfabric=true;
                            showmessage(tr("吹织自动取消！"));
                        }
                    }
                }
                else if(Ram.Receive(0x202)==0x09)//F4停状态反馈
                {
                    if(Ram.Receive(0x203)==0x00)
                    {
                        if(Ram.Receive(0x204)==0xAA)
                        {
                            Ram.Send(0xAA,0x300);
                            setButtonState(ui->F4stop_Pat_pb,false);
                            Switch_F4stop=true;
                            showmessage(tr("F4停自动取消！"));
                        }
                    }
                }
            }
            else if(Ram.Receive(0x201)==0xA6)//报警反馈
            {
                if(Ram.Receive(0x202)==0x01)//CAN异常报警
                {
                    //写入异常报警
                    quint8 can[2];
                    can[0]=Ram.Receive(0x203);
                    can[1]=Ram.Receive(0x204);
                    QString filename="/xrfile/WarningHistory/"+QDate::currentDate().toString("yyyyMMdd")+".warn";
                    if(QFile::exists(filename))
                    {
                        if(QFile(filename).size()>100*1024)
                        {
                            QFile file(filename);
                            QByteArray ba;
                            if(file.open(QIODevice::ReadWrite))
                            {
                                file.seek(70000);
                                ba=file.readAll();
                                file.close();
                            }
                            QFile::remove(filename);
                            if(file.open(QIODevice::ReadWrite))
                            {
                                file.write(ba);
                                ba.clear();
                                file.close();
                                system("sync");
                            }
                        }
                    }
                    quint32 time=QDateTime::currentDateTime().toTime_t();
                    if(time-time_t_can>1)
                    {
                        time_t_can=time;
                        QFile file(filename);
                        if(file.open(QIODevice::ReadWrite))
                        {
                            QDataStream in(&file);
                            in.device()->seek(file.size());
                            quint16 addr=0x400;
                            for(int i=0;i<2;i++,addr++)
                            {
                                for(int j=0;j<8;j++)
                                {
                                    time=QDateTime::currentDateTime().toTime_t();
                                    if((can[i]>>j&0x01)==1)
                                    {
                                        in<<time<<addr<<quint8(j+1);
                                    }
                                }
                            }
                            file.close();
                        }
                    }
                    Ram.Send(0xAA,0x300);
                }
            }
        }
        else if(Ram.Receive(0x200)==0xa0)//掉电保护
        {
            if(Ram.Receive(0x201)==0x20)
            {
                if(Ram.Receive(0x202)==0x09)
                {
                    if(Ram.Receive(0x203)==0x55)
                    {
                         Ram.Send(0xAA,0x300);
                         wifi->myWiFi->machineStatusSwitch(Off);
                         saveOffPowerState();
                         showmessage(tr("系统掉电！！"));
                    }
                }
            }
        }
    }
}

//双口RAM实时状态数据读取
void PatternWoven::readrealstate()
{
    realtime_parameter.knitstate=Ram.Receive(0x424);
    realtime_parameter.head_position=Ram.Receive(0x425);
    if(realtime_parameter.knitstate==0)
        knitState=knitNULL;
    else if(realtime_parameter.knitstate==1)
        knitState=knitRest;
    else if(realtime_parameter.knitstate==2)
        knitState=knitting;
    else if(realtime_parameter.knitstate==3)
        knitState=knitFlat;

    //当前脉冲
    realtime_parameter.curpulse=Ram.GetU16Data(0x320);
    //    ckmRunInfo.cur_pulse=realtime_parameter.curpulse;
    //当前针数
    realtime_parameter.curneedle=Ram.GetU16Data(0x323);
    //角度
    realtime_parameter.curangle=Ram.GetU16Data(0x326);
    //当前圈
    realtime_parameter.curcircle=Ram.GetU16Data(0x329);

    //当前速度和设定速度
    realtime_parameter.curspeed=Ram.GetU16Data(0x32C);
    realtime_parameter.setspeed=Ram.GetU16Data(0x32E);

    //更新联网速度数据
    //    ckmRunInfo.cur_speed=realtime_parameter.curspeed;//当前速度
    //    ckmRunInfo.set_speed=realtime_parameter.setspeed;//设定速度
    //当前循环和总循环
    realtime_parameter.curcycle=Ram.GetU16Data(0x330);//当前循环
    realtime_parameter.totalcycle=Ram.GetU16Data(0x333);//总循环

    //更新联网循环圈数据
    //    ckmRunInfo.cur_circle=realtime_parameter.curcycle;//当前步循环
    //    ckmRunInfo.total_circle=realtime_parameter.totalcycle;//当前步总循环数
    //当前步
    realtime_parameter.curstep=Ram.GetU16Data(0x339);

    //零位基准
    realtime_parameter.zero=Ram.GetU16Data(0x420);

    ui->needle_lb->setText(QString::number(realtime_parameter.curneedle));
    ui->anglelb->setText(QString::number(realtime_parameter.curangle));
    if(realtime_parameter.totalcircle!=0)
        ui->roll_lb->setText(QString("%1/%2").arg(realtime_parameter.curcircle).arg(realtime_parameter.totalcircle));
    else
        ui->roll_lb->setText(QString::number(realtime_parameter.curcircle));
    ui->speed_lb->setText(QString("%1/%2").arg(realtime_parameter.curspeed).arg(realtime_parameter.setspeed));
    ui->circulation_lb->setText(QString("%1/%2").arg(realtime_parameter.curcycle).arg(realtime_parameter.totalcycle));
    ui->step_lb->setText(QString("%1/%2").arg(realtime_parameter.curstep).arg(realtime_parameter.totalstep));
    ui->debugswitch_lb->setText(QString::number((qint16)realtime_parameter.zero));

    //温度读取
    realtime_parameter.tempera=Ram.GetU16Data(0x422);//温度
    //温度为0是隐藏温度显示
    if(realtime_parameter.tempera==0)
    {
        if(ui->label_tempear->isHidden()==false)
        {
            ui->label_tempear->setText("");
            ui->label_temp->hide();
            ui->label_tempear->hide();
        }
    }
    else
    {
        if(ui->label_tempear->isHidden())
        {
            ui->label_temp->show();
            ui->label_tempear->show();
        }
        float tp=(float)realtime_parameter.tempera/10;
        ui->label_tempear->setText(QString::number(tp,'f',1));
    }

    //判断机器运行状态
    if(realtime_parameter.curneedleTemp==realtime_parameter.curneedle)//机器在运转过程中，不能复位。即针数有变化。
    {
        resetOrNot++;
        if(resetOrNot>=25)
        {
            realtime_parameter.isRunnging=false;
            ui->toMenu_pb->setEnabled(true);
            ui->reset_Pat_pb->setEnabled(true);
            ui->liftshuttle_Pat_pb->setEnabled(true);
            ui->outNeedle_pb->setEnabled(true);
            ui->inNeedle_pb->setEnabled(true);
            ui->output_lb->setEnabled(true);
            ui->circulation_pb->setEnabled(true);
            ui->flatclothing_pb->setEnabled(true);
            ui->PatternWoven_pb->setEnabled(true);
            ui->Yield_pb->setEnabled(true);
            ui->luoan_pb->setEnabled(true);
            ui->luoanD_pb->setEnabled(true);
            ui->F4stop_Pat_pb->setEnabled(true);
            ui->BTSR_pb->setEnabled(true);
            resetOrNot=0;
            //机器状态为运行的情况下 设为停车
            if(wifi->myWiFi->machineState==Run)
            {
                if(cntProgress==4)
                {
                    //停车指令
                    wifi->myWiFi->machineStatusSwitch(Stop);
                }
            }
        }
    }
    else
    {
        //机器状态设为运行
        if(cntProgress==4)
        {
            if(wifi->myWiFi->machineState!=Run)
            {
                //运行状态指令
                wifi->myWiFi->machineStatusSwitch(Run);
            }
        }
        if(resetOrNot!=0)
        {
            resetOrNot=0;
        }
        realtime_parameter.curneedleTemp=realtime_parameter.curneedle;
        realtime_parameter.isRunnging=true;
        //机器运行时复位、菜单和
        ui->toMenu_pb->setEnabled(false);
        ui->reset_Pat_pb->setEnabled(false);
        ui->liftshuttle_Pat_pb->setEnabled(false);
        ui->outNeedle_pb->setEnabled(false);
        ui->inNeedle_pb->setEnabled(false);
        ui->output_lb->setEnabled(false);
        ui->circulation_pb->setEnabled(false);
        ui->flatclothing_pb->setEnabled(false);
        ui->PatternWoven_pb->setEnabled(false);
        ui->Yield_pb->setEnabled(false);
        ui->luoan_pb->setEnabled(false);
        ui->luoanD_pb->setEnabled(false);
        ui->F4stop_Pat_pb->setEnabled(false);
        ui->BTSR_pb->setEnabled(false);
    }
}

void PatternWoven::shuttlechanged(int num)
{
    if(realtime_parameter.isRunnging)
        return;
    QString msg=tr("纱嘴");
    if(num==0)
        msg+="1、2、3";
    else if(num==1)
        msg+="4、5、6";
    else
        msg+="7、8";
    if(shuttlestate[num])
    {
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x0A;
        Ram.SendData[3]=num+1;//梭子路数1-8
        Ram.SendData[4]=0x55;//抬起
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
        shuttlestate[num]=false;
        msg+=tr("抬起");
        if(taishazui->isHidden())
            taishazui->show();
    }
    else
    {
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x0A;
        Ram.SendData[3]=num+1;//梭子路数1-8
        Ram.SendData[4]=0xAA;//放下
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
        shuttlestate[num]=true;
        msg+=tr("放下");
        usleep(5000);
        if(shuttlestate[0]==true&&shuttlestate[1]==true&&shuttlestate[2]==true)
        {
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA1;
            Ram.SendData[2]=0x0A;
            Ram.SendData[3]=0xAA;
            Ram.SendData[4]=0x00;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
            msg=tr("纱嘴状态回归...");
            taishazui->hide();
        }
    }
    showmessage(msg);
}

void PatternWoven::initialDynamicData()
{
    dynData.needles=realtime_parameter.curneedle;
    dynData.angle=realtime_parameter.curangle;
    dynData.speed=realtime_parameter.curspeed;
    dynData.selAdjust=0;
    dynData.fixNeedle=0;
    dynData.dynSpeed=0;
    dynData.limSpeed=0;
    dynData.maxSpeed=0;
    dynData.run_speed="00000000";
    dynData.injInterval=0;
    dynData.injNumbers=0;
    dynData.op_state="00000000";
    dynData.ele_switch="00000000";
    dynData.up_lamp="00000000";
    dynData.down_lamp="00000000";
    dynData.flu_lamp="00000000";
    dynData.fan="0";
    for(int i=0;i<8;i++)
    {
        dynData.fineTuning[i]=0;
    }
    dynData.cur_yield=machine_Pars.yield_current;
    dynData.set_yield=machine_Pars.yield_setted;
    dynData.total_yield=0;
    dynData.single_time=0;
    dynData.duty_id="00000000";
    dynData.cur_step=realtime_parameter.curstep;
    dynData.total_step=realtime_parameter.totalstep;
    dynData.cur_circle=realtime_parameter.curcircle;
    dynData.total_circle=realtime_parameter.totalcircle;

    dynData.selAdjust=machine_Pars.actuator_adjust;
    dynData.fixNeedle=machine_Pars.fixed_needle;
    dynData.dynSpeed=machine_Pars.jog_speed;
    dynData.limSpeed=machine_Pars.speed_limt;
    dynData.maxSpeed=machine_Pars.max_speed;
    dynData.injInterval=machine_Pars.oil_interval;
    dynData.injNumbers=machine_Pars.oil_times;
    for(int i=0;i<8;i++)
        dynData.fineTuning[i]=machine_Pars.motor_adjust[i];
    wifi->myWiFi->dynamicData=dynData;
}

void PatternWoven::refreshDynamicData()
{
    dynData.needles=realtime_parameter.curneedle;
    dynData.angle=realtime_parameter.curangle;
    dynData.speed=realtime_parameter.curspeed;
    dynData.cur_yield=machine_Pars.yield_current;
    dynData.set_yield=machine_Pars.yield_setted;
    dynData.cur_step=realtime_parameter.curstep;
    dynData.total_step=realtime_parameter.totalstep;
    dynData.cur_circle=realtime_parameter.curcircle;
    dynData.total_circle=realtime_parameter.totalcircle;
    wifi->myWiFi->dynamicData=dynData;
}

void PatternWoven::setButtonState(QPushButton *btn,bool checked)
{
    QString str=btn->text();
    if(checked)
    {
        if(str.length()>5)
        {
            btn->setStyleSheet("QPushButton{"
                               "border:noborder;"
                               "border-radius:10;"
                               "font-size:18px;"
                               "color:red;"
                               "background-color: rgb(80, 80, 80);"
                               "}"
                               "QPushButton::pressed{"
                               "background-color: rgb(120, 120, 120);"
                               "}");
        }
        else
        {
            btn->setStyleSheet("QPushButton{"
                               "border:noborder;"
                               "border-radius:10;"
                               "font-size:22px;"
                               "color:red;"
                               "background-color: rgb(80, 80, 80);"
                               "}"
                               "QPushButton::pressed{"
                               "background-color: rgb(120, 120, 120);"
                               "}");
        }
    }
    else
    {
        if(str.length()>5)
        {
            btn->setStyleSheet("QPushButton{"
                               "border:noborder;"
                               "border-radius:10;"
                               "font-size:18px;"
                               "color:yellow;"
                               "background-color: rgb(80, 80, 80);"
                               "}"
                               "QPushButton::pressed{"
                               "background-color: rgb(120, 120, 120);"
                               "}");
        }
        else
        {
            btn->setStyleSheet("QPushButton{"
                               "border:noborder;"
                               "border-radius:10;"
                               "font-size:22px;"
                               "color:yellow;"
                               "background-color: rgb(80, 80, 80);"
                               "}"
                               "QPushButton::pressed{"
                               "background-color: rgb(120, 120, 120);"
                               "}");
        }
    }
}

void PatternWoven::Simulation()
{
    if(simualted)
    {
        realtime_parameter.curangle+=1;

        //查表获得转速
        for(int j=0;j<70;j++)
        {
            if(machine_Pars.speed_step[j]>realtime_parameter.curstep)
                break;
            if(machine_Pars.speed_step[j]==realtime_parameter.curstep)
            {
                realtime_parameter.curspeed=machine_Pars.speed_value[j];
                break;
            }
        }
        //速度不能大于最大速度
        if(realtime_parameter.curspeed>machine_Pars.max_speed)
            realtime_parameter.setspeed=realtime_parameter.curspeed=machine_Pars.max_speed;
        else
            realtime_parameter.setspeed=realtime_parameter.curspeed;

        //根据实时转速计算脉冲累加值
        int interval=realtime_parameter.curspeed*machine_Pars.total_puls/(60*1000.0)*20;
        //每个计数周期脉冲累加
        realtime_parameter.curpulse+=interval;
        //圈数+1
        if(realtime_parameter.curpulse>machine_Pars.total_puls)
        {
            realtime_parameter.curpulse=0;
            realtime_parameter.curcycle++;
            realtime_parameter.curcircle++;

            {
                //循环数做完  步骤+1
                if(realtime_parameter.curcycle>realtime_parameter.totalcycle)
                {
                    realtime_parameter.curstep++;
                    //产量+1
                    if(realtime_parameter.curstep>realtime_parameter.totalstep)
                    {
                        realtime_parameter.curstep=0;
                        realtime_parameter.curcircle=0;
                    }
                    //查表获得循环数
                    for(int j=0;j<50;j++)
                    {
                        if(machine_Pars.eco_stepstart[j]>realtime_parameter.curstep)
                        {
                            realtime_parameter.curcycle=realtime_parameter.totalcycle=1;
                            break;
                        }
                        if(machine_Pars.eco_stepstart[j]==realtime_parameter.curstep)
                        {
                            realtime_parameter.curcycle=1;
                            realtime_parameter.totalcycle=machine_Pars.eco_value[j];
                            break;
                        }
                        if(machine_Pars.eco_stepstart[j]==0)
                        {
                            realtime_parameter.curcycle=realtime_parameter.totalcycle=1;
                            break;
                        }
                    }

                    for(int j=0;j<200;j++)
                    {
                        if(machine_Pars.motor_step[j]==realtime_parameter.curstep)
                        {
                            int num=machine_Pars.motor_number[j];
                            realtime_parameter.motorstate[num]=machine_Pars.motor_position[j];
                        }
                    }
                }
            }
        }
        realtime_parameter.curangle=(realtime_parameter.curpulse/(float)machine_Pars.total_puls)*360;
        realtime_parameter.curneedle=(realtime_parameter.curpulse/(float)machine_Pars.total_puls)*machine_Pars.totalneedles;
    }
}

/*实时刷新的参数，100ms执行一次*/
void PatternWoven::updatepara_slot()
{
    if(startNet)
    {
        startNet=false;
        wifi->exec();
    }
    if(wifi->myWiFi->connectProgress!=-1)
    {
        if(ui->wifiFlag_lb->isVisible()==false)
            ui->wifiFlag_lb->show();
        //显示连接状态（信号强度）
        if(cntProgress!=wifi->myWiFi->connectProgress)
        {
            cntProgress=wifi->myWiFi->connectProgress;
            ui->wifiFlag_lb->setStyleSheet(QString("image: url(:/img/success_%0.png);").arg(cntProgress));
        }
    }
//    else
//    {
//        if(ui->wifiFlag_lb->isVisible())
//            ui->wifiFlag_lb->hide();
//    }
    /*开机时，读取花型名称*/
    if(downLoadPatn==1)
    {
        downLoadPatn=0;
        QFile file_project("/xrfile/PatternName.txt");
        if(file_project.open(QIODevice::ReadWrite))
        {
            QString name;
            QTextStream in(&file_project);
            in>>name;
            file_project.close();
            ui->name_pb->setText(name);
            dynData.pro_name=name;
            projectCOName=name;
            if(!IsFirstEnterMain)
                showmessage(tr("已装载%1文件").arg(name));
        }
        else
        {
            para_operation.saveSystemLog(tr("花型装载失败！"),QDateTime::currentDateTime());
        }

        if(QFile::exists(Path_patternUCO))
        {
            QFile file(Path_patternUCO);
            if(file.open(QIODevice::ReadOnly))
            {
                file.seek(0x250);
                QByteArray ba=file.read(6);
                file.close();
                realtime_parameter.totalstep=(quint8)ba[0]+((quint8)ba[1])*256;
                realtime_parameter.totalcircle=(quint8)ba[2]+((quint8)ba[3])*256;
            }
        }
        else
        {
            //读取总步骤数
            QFile file_sumStep(NeedleLocation);
            if(file_sumStep.open(QIODevice::ReadWrite))
            {
                QDataStream in(&file_sumStep);
                in.device()->seek(900);
                in>>realtime_parameter.totalstep;
                realtime_parameter.totalcircle=0;
                file_sumStep.close();
            }
        }
    }

    /*开机时，读取机型*/
    if(startToCompare==1)
    {
        startToCompare=0;
        machineSize=realtime_parameter.machineNeedleType.left(2).toInt();
        ui->machinetype_pb->setText(realtime_parameter.machineNeedleType);
        //报警显示隐藏
        ui->diswarn->hide();
        for(int i=0;i<5;i++)
        {
            if(machine_Pars.alarm_setted[i]!=0)
            {
                ui->diswarn->show();
                break;
            }
        }
    }

    if(updateUI)
    {
        saveOffPowerState();
        updateUI=false;
    }

    if(b_headerUp_Down!="")
    {
        showmessage(b_headerUp_Down);
        b_headerUp_Down="";
    }
    ui->finetuning_1_le->setText(QString::number((qint16)machine_Pars.motor_adjust[0]));
    ui->finetuning_2_le->setText(QString::number((qint16)machine_Pars.motor_adjust[1]));
    ui->finetuning_3_le->setText(QString::number((qint16)machine_Pars.motor_adjust[2]));
    ui->finetuning_4_le->setText(QString::number((qint16)machine_Pars.motor_adjust[3]));
    ui->finetuning_5_le->setText(QString::number((qint16)machine_Pars.motor_adjust[4]));
    ui->finetuning_6_le->setText(QString::number((qint16)machine_Pars.motor_adjust[5]));
    ui->finetuning_7_le->setText(QString::number((qint16)machine_Pars.motor_adjust[6]));
    ui->finetuning_8_le->setText(QString::number((qint16)machine_Pars.motor_adjust[7]));

    //双口RAM实时状态读取
    readrealstate();
    //状态反馈
    statusFeedBack();
    //当前产量更新
    currentoutput_update();

    refreshDynamicData();
    //当前任务完成，弹出提示：是否继续编织
//    if(machine_Pars.yield_current==machine_Pars.yield_setted&&machine_Pars.yield_setted!=0)
//    {
//        if(m_bFirst)
//        {
//            m_bFirst = false;
//            m_pOrder->ShowInfo(tr("当前任务已完成，是否继续编织该任务？"));
//        }
//    }
//    else
//    {
//        m_bFirst = true;
//    }
    //系统到期剩余7天内提醒
    if(sys_maintenance.endtime!="")
    {
        QString date=QString("%1-%2-%3 23:59:59").arg(sys_maintenance.endtime.left(4)).arg(sys_maintenance.endtime.mid(4,2)).arg(sys_maintenance.endtime.right(2));
        QDateTime time=QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss");
        int secs=QDateTime::currentDateTime().secsTo(time);
        if(time.toTime_t()<QDateTime::currentDateTime().toTime_t())
        {
            ui->label_warning->setVisible(true);
            ui->label_warning->setShowText(tr("系统已到期，请联系厂家！"));
        }
        else
        {
            if(secs<=604800&&secs>=0)
            {
                ui->label_warning->setVisible(true);
                QString str="";
                if(secs<(24*3600)&&secs>=3600)
                {
                    str=QString("系统有效期剩余 %1小时%2分！").arg(secs/3600).arg((secs%3600)/60);
                }
                else if(secs<3600)
                {
                    str=QString("系统有效期剩余 %1分钟！").arg(secs/60);
                }
                else
                {
                    str=QString("系统有效期剩余 %1天%2小时！").arg(secs/60/60/24).arg(secs%(24*60*60)/3600);
                }
                ui->label_warning->setShowText(str);
//                ui->label_warning->setShowText(QString("系统有效期剩余 %1天%2小时").arg(secs/60/60/24).arg(secs%(24*60*60)/3600));
            }
            else
            {
                if(ui->label_warning->isVisible())
                    ui->label_warning->hide();
            }
        }
    }

    //主界面为活动窗口
    if(this->isActiveWindow())
    {
        //分期付款到期并且在主界面下才能发送到期指令
        int days= lastDate.daysTo(QDate::currentDate());
        if(days>0)
        {
            deleteWarningHistory();
            lastDate=QDate::currentDate();
            pHirepurchase->startHirpurchase(sys_maintenance.endtime);
        }

        //开启磨合状态
        if(runningIn.mark)
        {
            //磨合期已到期
            if(QDateTime::currentDateTime().toTime_t()>runningIn.startTime+86400*runningIn.days)
            {
                QSettings *set=new QSettings(Path_machine,QSettings::IniFormat);
                runningIn.mark=false;
                set->setValue("RunningIn/Mark","0");
                delete set;
                system("sync");
                Ram.SendData[0]=0x20;
                Ram.SendData[1]=0xA1;
                Ram.SendData[2]=0x00;
                Ram.SendData[3]=0x00;
                Ram.SendData[4]=0x01;
                Ram.SendData[5]=0xAA;
                Ram.SendData[6]=0x00;
                Ram.SendData[7]=0x00;
                Ram.SendData[8]=0x00;
                Ram.SendData[9]=0x00;
                Ram.UI_to_RTC();
            }
        }
    }
}

void PatternWoven::single_stop_slot()
{
    ui->singlestop_Pat_pb->click();
}

//当前产量更新 curOutP=currentoutput;
void PatternWoven::currentoutput_update()
{
    //当前步骤=总步骤数 并且总步骤数不为0 表示产量当前花型即将编织完成
    if(realtime_parameter.curstep==realtime_parameter.totalstep&&realtime_parameter.totalstep!=0)
    {
        bCurrent_Outp_change = true;
    }

    if(bCurrent_Outp_change)
    {
        //当前步骤号变为0 花型已编织完成
        if(realtime_parameter.curstep==0)
        {
            bCurrent_Outp_change = false;
            //允许更新产量
            if(yieldUpdated==false)
            {
                //当前产量加1
                machine_Pars.yield_current++;
                para_operation.machine_par_save();

                //更新单件时间
                QDateTime latestTime = QDateTime::currentDateTime();
                int sumTime=single.secsTo(latestTime);
                dynData.single_time=sumTime;
                QString min=QString::number(sumTime/60);
                QString sec=QString::number(sumTime%60);
                realtime_parameter.singletime=min.append(QString::fromUtf8("分")).append(sec).append("秒");
                ui->singletime_lb->setText(realtime_parameter.singletime);
                single = QDateTime::currentDateTime();
                QFile file_("/xrfile/SingleTime.txt");
                if(file_.open(QIODevice::WriteOnly))
                {
                    file_.resize(0);
                    QTextStream out(&file_);
                    out<<realtime_parameter.singletime;
                    file_.close();
                    system("sync");
                }
            }
            else
            {
                single = QDateTime::currentDateTime();
                yieldUpdated=false;
            }
        }
    }
    //产量显示
    ui->output_lb->setText(QString("%1/%2").arg(machine_Pars.yield_current).arg(machine_Pars.yield_setted));
}

/*提示栏信息打印*/
void PatternWoven::showmessage(QString mesg)
{
    if(savelog)
        para_operation.saveSystemLog(mesg,QDateTime::currentDateTime());
    if(ui->textBrowser->toPlainText()!="")
        ui->textBrowser->insertPlainText("\n");
    ui->textBrowser->insertPlainText(QString("[%1] - %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(mesg));
}

/*切换到菜单*/
void PatternWoven::on_toMenu_pb_clicked()
{
    if(checkInterval()==false)
        return;
    if(IsFirstEnterMain)
        IsFirstEnterMain=false;
    Menu *pMenu=new Menu;
    pMenu->exec();
    //切换到编织界面
    Ram.to_PatternWoven_slot();//切换到花型编织界面
    usleep(50000);
    Ram.SendData[0]=0x20;
    Ram.SendData[1]=0xA1;
    Ram.SendData[2]=0x00;
    Ram.SendData[3]=0x00;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    //开启磨合状态
    if(runningIn.mark)
    {
        //磨合期已到期
        if(QDateTime::currentDateTime().toTime_t()>runningIn.startTime+86400*runningIn.days)
        {

        }
        else
        {
            Ram.SendData[4]=0x01;
            Ram.SendData[5]=0x55;
        }
    }
    Ram.UI_to_RTC();
    warningShow_mark=1;

    if(zeroSetted)
    {
        usleep(5000);
        ui->reset_Pat_pb->click();
        zeroSetted=false;
    }
}

/*切换到转速设置*/
void PatternWoven::on_PatternWoven_pb_clicked()
{
    if(checkInterval()==false)
        return;
    ModifyCO *co=new ModifyCO;
    co->settype(1);
    co->exec();
}

/*平布调试*/
void PatternWoven::on_flatclothing_pb_clicked()
{
    if(checkInterval()==false)
        return;
    FlatCloth *pFlatCloth=new FlatCloth;
    resetBool=true;
    Switch_outNeed=true;
    Switch_inNeed=true;
    setButtonState(ui->reset_Pat_pb,false);
    setButtonState(ui->outNeedle_pb,false);
    setButtonState(ui->inNeedle_pb,false);
    pFlatCloth->exec();
}

/*节约调整*/
void PatternWoven::on_circulation_pb_clicked()
{
    if(checkInterval()==false)
        return;
    ModifyCO *co=new ModifyCO;
    co->settype(0);
    co->exec();
}
/*压针微调*/
void PatternWoven::on_pressNeedle_pb_clicked()
{
    if(checkInterval()==false)
        return;
    motorsetting=true;
    StitchAdjust *stitch=new StitchAdjust;
    stitch->exec();
    motorsetting=false;
}

/*加油*/
void PatternWoven::on_GoGo_pb_clicked()
{
    if(checkInterval()==false)
        return;
    if(Switch_oil)
    {
        setButtonState(ui->GoGo_pb,true);
        Switch_oil=false;
        showmessage(tr("加油..."));
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x0A;
        Ram.SendData[3]=0x55;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else
    {
        setButtonState(ui->GoGo_pb,false);
        Switch_oil=true;
        showmessage(tr("加油停止..."));
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x0A;
        Ram.SendData[3]=0xAA;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
}

/*上一页*/
void PatternWoven::on_previousPage_pb_clicked()
{
    if(checkInterval()==false)
        return;
    page_id--;
    refreshButton();
}

/*下一页*/
void PatternWoven::on_nextPage_pb_clicked()
{
    if(checkInterval()==false)
        return;
    page_id++;
    refreshButton();
}
/*产量设置*/
void PatternWoven::on_Yield_pb_clicked()
{
    on_output_lb_clicked();
}

/*判断系统到期的槽函数*/
void PatternWoven::getExpireReminder(bool mark)
{
    if(mark)//到期
    {
        /*指令，提示下位机到期*/
        Ram.SendData[0] = 0x20;
        Ram.SendData[1] = 0xA4;
        Ram.SendData[2] = 0x55;
        Ram.SendData[3] = 0x00;
        Ram.SendData[4] = 0x00;
        Ram.SendData[5] = 0x00;
        Ram.SendData[6] = 0x00;
        Ram.SendData[7] = 0x00;
        Ram.SendData[8] = 0x00;
        Ram.SendData[9] = 0x00;
        Ram.UI_to_RTC();
    }
}

/*硬件错误*/
void PatternWoven::errorHardware()
{
    resetBool=false;
    Ram.SendData[0]=0x20;
    Ram.SendData[1]=0xA1;
    Ram.SendData[2]=0x00;
    Ram.SendData[3]=0x55;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    MyMessageBox *box = new MyMessageBox(tr("提示"),tr("检测到硬件错误！"),0);
    box->exec();
}


/*接收键盘数据*/
void PatternWoven::getKeyComData()
{
    if (mycom.bytesAvailable() >= 1)
    {
        temp=mycom.readAll();
        update_keyboard();
    }
}

void PatternWoven::saveOffPowerState()
{
    QFile file("/xrfile/offState.er");
    if(file.open(QIODevice::ReadWrite))
    {
        QDataStream out(&file);
        file.resize(0);
        if(ui->stopchain_Pat_lb->styleSheet().contains("color:red")||ui->stopchain_Pat_lb->styleSheet().contains("color:(255,255,0)"))
        {
            out.device()->seek(0);
            out<<(quint8)1;
        }
        if(ui->economy_Pat_pb->styleSheet().contains("color:red")||ui->economy_Pat_pb->styleSheet().contains("color:(255,255,0)"))
        {
            out.device()->seek(1);
            out<<(quint8)1;
        }
        if(ui->singlestop_Pat_pb->styleSheet().contains("color:red")||ui->singlestop_Pat_pb->styleSheet().contains("color:(255,255,0)"))
        {
            out.device()->seek(2);
            out<<(quint8)1;
        }
        if(ui->F4stop_Pat_pb->styleSheet().contains("color:red")||ui->F4stop_Pat_pb->styleSheet().contains("color:(255,0,0)"))
        {
            out.device()->seek(3);
            out<<(quint8)1;
        }
        if(ui->ratelimit_Pat_pb->styleSheet().contains("color:red")||ui->ratelimit_Pat_pb->styleSheet().contains("color:(255,0,0)"))
        {
            out.device()->seek(4);
            out<<(quint8)1;
        }
        if(ui->fastforward_Pat_pb->styleSheet().contains("color:red")||ui->fastforward_Pat_pb->styleSheet().contains("color:(255,0,0)"))
        {
            out.device()->seek(5);
            out<<(quint8)1;
        }
        if(ui->blowfabric_Pat_pb->styleSheet().contains("color:red")||ui->blowfabric_Pat_pb->styleSheet().contains("color:(255,0,0)"))
        {
            out.device()->seek(6);
            out<<(quint8)1;
        }
        if(ui->alarm_Pat_pb->styleSheet().contains("color:red")||ui->alarm_Pat_pb->styleSheet().contains("color:(255,0,0)"))
        {
            out.device()->seek(7);
            out<<(quint8)1;
        }
        if(ui->GoGo_pb->styleSheet().contains("color:red")||ui->GoGo_pb->styleSheet().contains("color:(255,0,0)"))
        {
            out.device()->seek(8);
            out<<(quint8)1;
        }
        if(ui->outNeedle_pb->styleSheet().contains("color:red")||ui->outNeedle_pb->styleSheet().contains("color:(255,0,0)"))
        {
            out.device()->seek(9);
            out<<(quint8)1;
        }
        if(ui->inNeedle_pb->styleSheet().contains("color:red")||ui->inNeedle_pb->styleSheet().contains("color:(255,0,0)"))
        {
            out.device()->seek(10);
            out<<(quint8)1;
        }
        if(ui->reset_Pat_pb->styleSheet().contains("color:red")||ui->reset_Pat_pb->styleSheet().contains("color:(255,0,0)"))
        {
            out.device()->seek(11);
            out<<(quint8)1;
        }
        out.device()->seek(12);
        out<<(quint8)1;
        file.close();
        system("sync");
    }
}

/**开机时，读取关机前的状态**/
int PatternWoven::readPreOffPowerState()
{
    int rst=0;
    QFile file("/xrfile/offState.er");
    if(file.open(QIODevice::ReadWrite))
    {
        QDataStream in(&file);
        quint8 para[13];
        for(int i = 0;i<13;i++)
        {
            in>>para[i];
        }
        in.device()->seek(12);
        in<<quint8(0);
        file.close();
        system("sync");
        if(para[12]==1)//有掉电保存
        {
            if(para[0] == 1)
            {
                showmessage(tr("停链..."));
                setButtonState(ui->stopchain_Pat_lb,true);
                Switch_stopchain=false;
                yieldUpdated=true;//停链是不计产量
            }
            if(para[1] == 1)
            {
                showmessage(tr("节约..."));
                setButtonState(ui->economy_Pat_pb,true);
                Switch_economy=false;
                yieldUpdated=true;//节约是不计产量
            }
            if(para[2] == 1)
            {
                showmessage(tr("单停..."));
                setButtonState(ui->singlestop_Pat_pb,true);
                Switch_singlestop=false;
            }
            if(para[3] == 1)
            {
                showmessage(tr("F4停车..."));
                setButtonState(ui->F4stop_Pat_pb,true);
                Switch_F4stop=false;
            }
            if(para[4] == 1)
            {
                showmessage(tr("限速..."));
                setButtonState(ui->ratelimit_Pat_pb,true);
                Switch_ratelimit=false;
            }
            if(para[5] == 1)
            {
                showmessage(tr("快进..."));
                setButtonState(ui->fastforward_Pat_pb,true);
                Switch_fastforward=false;
                yieldUpdated=true;//快进是不计产量
            }
            if(para[6] == 1)
            {
                showmessage(tr("吹织..."));
                setButtonState(ui->blowfabric_Pat_pb,true);
                Switch_blowfabric=false;
            }
            if(para[7] == 1)
            {
                emit sendCancleMark(true);
                pWarningPatternwoven->hide();
                Ram.Send(0xAA,0x303);
                showmessage(tr("报警隐藏..."));
                setButtonState(ui->alarm_Pat_pb,true);
                Switch_alarm=false;
            }
            if(para[8] == 1)
            {
                setButtonState(ui->GoGo_pb,true);
                showmessage(tr("加油..."));
                Switch_oil=false;
            }
            if(para[9] == 1)
            {
                setButtonState(ui->outNeedle_pb,true);
                showmessage(tr("全出针..."));
                Switch_outNeed = false;
            }
            if(para[10] == 1)
            {
                setButtonState(ui->inNeedle_pb,true);
                showmessage(tr("全埋针..."));
                Switch_inNeed = false;
            }
            if(para[11] == 1)
            {
                resetBool=false;
                setButtonState(ui->reset_Pat_pb,true);
                showmessage(tr("复位"));
            }
            rst=1;
        }
    }
    return rst;
}

void PatternWoven::update_MachineID()
{
    ui->systemID_lb->setText(wifi->myWiFi->my_WiFi.machineID);
}

void PatternWoven::alarmClear()
{
    wifi->myWiFi->machineStatusSwitch(AlarmClear);
}

void PatternWoven::autoScroll()
{
    QTextCursor cursor =  ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);
}

void PatternWoven::refreshButton()
{
    if(page_id==1)
    {
        ui->reset_Pat_pb->show();
        ui->stopchain_Pat_lb->show();
        ui->nextPage_pb->show();
        ui->economy_Pat_pb->show();
        ui->singlestop_Pat_pb->show();
        ui->ratelimit_Pat_pb->show();
        ui->fastforward_Pat_pb->show();
        ui->blowfabric_Pat_pb->show();
        ui->alarm_Pat_pb->show();
        ui->liftshuttle_Pat_pb->show();
        ui->GoGo_pb->hide();
        ui->pressNeedle_pb->hide();
        ui->circulation_pb->hide();
        ui->flatclothing_pb->hide();
        ui->PatternWoven_pb->hide();
        ui->Yield_pb->hide();
        ui->luoan_pb->hide();
        ui->luoanD_pb->hide();
        ui->F4stop_Pat_pb->hide();
        ui->previousPage_pb->hide();
        //ui->GoGo_pb->hide();
        ui->outNeedle_pb->hide();
        ui->inNeedle_pb->hide();
        ui->BTSR_pb->hide();
    }
    else if(page_id==2)
    {
        ui->reset_Pat_pb->hide();
        ui->stopchain_Pat_lb->hide();
        //ui->nextPage_pb->hide();
        ui->economy_Pat_pb->hide();
        ui->singlestop_Pat_pb->hide();
        ui->ratelimit_Pat_pb->hide();
        ui->fastforward_Pat_pb->hide();
        ui->blowfabric_Pat_pb->hide();
        ui->alarm_Pat_pb->hide();
        ui->liftshuttle_Pat_pb->hide();
        ui->luoan_pb->hide();
        ui->luoanD_pb->hide();
        ui->nextPage_pb->show();
        ui->pressNeedle_pb->show();
        ui->circulation_pb->show();
        ui->flatclothing_pb->show();
        ui->PatternWoven_pb->show();
        ui->Yield_pb->show();
        ui->F4stop_Pat_pb->show();
        ui->previousPage_pb->show();
        ui->previousPage_pb->setGeometry(745,290,55,91);
        ui->GoGo_pb->hide();
        ui->outNeedle_pb->show();
        ui->inNeedle_pb->show();
        ui->BTSR_pb->hide();
    }
    else if(page_id==3)
    {
        ui->pressNeedle_pb->hide();
        ui->circulation_pb->hide();
        ui->flatclothing_pb->hide();
        ui->PatternWoven_pb->hide();
        ui->Yield_pb->hide();
        ui->F4stop_Pat_pb->hide();
        ui->outNeedle_pb->hide();
        ui->inNeedle_pb->hide();
        ui->nextPage_pb->hide();
        ui->previousPage_pb->show();
        ui->GoGo_pb->show();
        ui->luoan_pb->show();
        ui->previousPage_pb->setGeometry(745,385,55,91);
        ui->GoGo_pb->setGeometry(745,195,55,91);//8
        ui->BTSR_pb->show();
    }
}

void PatternWoven::deleteWarningHistory()
{
    QDir dir("/xrfile/WarningHistory");
    if(dir.exists()==false)
    {
        dir.mkdir("/xrfile/WarningHistory");
    }
    QStringList nameFilters;
    nameFilters<<"*.warn";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    QDate today=QDate::currentDate();
    QDate last;
    QFileInfo fi;
    int y,m,d,day=30;
    int totalsize=0;
    for(int i=0;i<files.length();i++)
    {
        totalsize+=fi.size();
    }
    if(totalsize>20971520)
        day=3;
    for(int i=0;i<files.length();i++)
    {
        fi=QFileInfo(files[i]);
        QString filename=fi.fileName();
        y=filename.mid(0,4).toInt();
        m=filename.mid(4,2).toInt();
        d=filename.mid(6,2).toInt();
        last=QDate(y,m,d);
        int days=last.daysTo(today);
        if(days>day)
        {
            filename="/xrfile/WarningHistory/"+filename;
            if(QFile::exists(filename))
                QFile::remove(filename);
        }
        system("sync");
    }
}

void PatternWoven::on_luoan_pb_clicked()
{
    KeyBd *pKeyBd=new KeyBd;
    if(pKeyBd->exec()==QDialog::Accepted)
    {
        if(pKeyBd->inputstr=="888888")
        {
            QLuoAnSetting *luoan = new QLuoAnSetting;
            luoan->ShowDialog();
        }
        else
        {
            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("密码错误！"),0);
            box->exec();
        }
    }
    delete pKeyBd;
}

void PatternWoven::on_luoanD_pb_clicked()
{
//    keyMark=61;
//    KeyBd *pKeyBd=new KeyBd;
//    pKeyBd->exec();
//    keyMark=1;
//    if(pKeyBd->inputstr=="")
//    {
//        keyMark=98;
//        QLuoAnAngle *luoan=new QLuoAnAngle;
//        luoan->show();
//    }
}

void PatternWoven::on_BTSR_pb_clicked()
{
    BTSRMain *btsr=new BTSRMain;
    btsr->exec();
}

bool checkInterval()
{
    int mes=lastTime.msecsTo(QDateTime::currentDateTime());
    lastTime=QDateTime::currentDateTime();
    if(mes>200)
        return true;
    else
        return false;
}
