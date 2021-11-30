#include "warningpatternwoven.h"
#include "ui_warningpatternwoven.h"
#include <QMessageBox>
#include <QDateTime>
#include "mymessagebox.h"
#include "myheader.h"
#include "Windows/WIFI/xrwifi.h"

extern XRWiFi *myWiFi;
extern int warningShow_mark;
int warning_times;
extern QString machineID;

WarningPatternwoven::WarningPatternwoven(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WarningPatternwoven)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2,(deskdop->height()-this->height())/2);
    timer_warningpv=new QTimer(this);
    timer_warningpv->start(100);
    ui->plainTextEdit->setStyleSheet("QPlainTextEdit{font-size:26px;color:red;background-color: rgb(255, 255, 255)}");
    connect(timer_warningpv, SIGNAL(timeout()),this,SLOT(showWarningMessage_pv()));
    connect(ui->back_lb, SIGNAL(clicked()),this,SLOT(back_slot()));
    warning_times=0;


    for(int i=0;i<8;i++)
    {
         //0x370 1-8号密度电机报警
        alarmName[0][i]=QString(tr("%1号密度电机异常！")).arg(i+1);
        alarmwifi[0].code[i]=1;
        alarmwifi[0].value[i]=i+1;
        //0x376 9-16号密度电机报警
        alarmName[6][i]=QString(tr("%1号密度电机异常！")).arg(i+9);
        alarmwifi[6].code[i]=1;
        alarmwifi[6].value[i]=i+9;
        //0x372 1-8号探针报警
        alarmName[2][i]=QString(tr("%1号探针异常！")).arg(i+1);
        alarmwifi[2].code[i]=2;
        alarmwifi[2].value[i]=i+1;

        //0x371
        alarmwifi[1].code[i]=3+i;
        alarmwifi[1].value[i]=1;
        //0x373
        alarmwifi[3].code[i]=11+i;
        alarmwifi[3].value[i]=1;
        //0x374
        alarmwifi[4].code[i]=19+i;
        alarmwifi[4].value[i]=1;
        //0x375
        alarmwifi[3].code[i]=27+i;
        alarmwifi[3].value[i]=1;
        //0x377
        alarmwifi[7].code[i]=0;
        alarmwifi[7].value[i]=0;
    }
    //0x371
    alarmName[1][0]=tr("风门电机异常！");
    alarmName[1][1]=tr("哈夫同步异常！");
    alarmName[1][2]=tr("针筒零位编码异常！");
    alarmName[1][3]=tr("零位传感器异常！");
    alarmName[1][4]=tr("");
    alarmName[1][5]=tr("");
    alarmName[1][6]=tr("");
    alarmName[1][7]=tr("织物未通过！");


    //0x373
    alarmName[3][0]=tr("2号断针！");
    alarmName[3][1]=tr("6号断针！");
    alarmName[3][2]=tr("机头上限位！");
    alarmName[3][3]=tr("机头下限位！");
    alarmName[3][4]=tr("BTSR停机！");
    alarmName[3][5]=tr("上下机头导通！");
    alarmName[3][6]=tr("风门电机零位电压异常！");
    alarmName[3][7]=tr("风门电机过流！");
    //0x374
    alarmName[4][0]=tr("低气压！");
    alarmName[4][1]=tr("低油压1！");
    alarmName[4][2]=tr("低油位！");
    alarmName[4][3]=tr("出衣口报警！");
    alarmName[4][4]=tr("KTF1停机！");
    alarmName[4][5]=tr("储纬器报警！");
    alarmName[4][6]=tr("KTF2停机！");
    alarmName[4][7]=tr("储纬器断纱报警！");
    //0x375
    alarmName[5][0]=tr("伺服报警！");
    alarmName[5][1]=tr("机头异常报警！");
    alarmName[5][2]=tr("出哈夫针状态！");
    alarmName[5][3]=tr("产量编织完成！");
    alarmName[5][4]=tr("系统异常请联系厂家！");
    alarmName[5][5]=tr("机器超速！");
    alarmName[5][6]=tr("系统无花型，请尽快下载！");
    alarmName[5][7]=tr("低油压2！");
    //0x38C
    alarmName[28][0]=tr("废纱2-4");
    alarmName[28][1]=tr("废纱6-8");
    alarmName[28][2]=tr("工程参数异常！");
    alarmName[28][3]=tr("");
    alarmName[28][4]=tr("CAN1发送邮箱溢出");
    alarmName[28][5]=tr("CAN1接收邮箱溢出");
    alarmName[28][6]=tr("CAN2发送邮箱溢出");
    alarmName[28][7]=tr("CAN2接收邮箱溢出");

    //0x379-0x384
    for(int i=0;i<8;i++)
    {
        alarmName[7][i]=QString(tr("第%1路氨纶无纱线")).arg(i+1);
        alarmName[9][i]=QString(tr("电机板%1邮箱溢出！")).arg(i+1);
        alarmName[10][i]=QString(tr("纱嘴板%1邮箱溢出！")).arg(i+1);
        alarmName[11][i]=QString(tr("功能气阀板%1邮箱溢出！")).arg(i+1);
        alarmName[12][i]=QString(tr("纱嘴板%1过流报警！")).arg(i+1);
        alarmName[13][i]=QString(tr("功能气阀板%1过流报警！")).arg(i+1);
        alarmName[14][i]=QString(tr("纱嘴板%1离线！")).arg(i+1);
        alarmName[15][i]=QString(tr("功能气阀板%1离线！")).arg(i+1);
        alarmName[16][i]=QString(tr("电机板%1离线！")).arg(i+1);
        alarmName[17][i]=QString(tr("选针器板%1离线！")).arg(i+1);
        alarmName[18][i]=QString(tr("选针器板%1离线！")).arg(i+9);
        alarmName[19][i]=QString(tr("密度电机%1零位电压异常！")).arg(i+1);
        alarmName[20][i]=QString(tr("密度电机%1过流！")).arg(i+1);
        alarmName[21][i]=QString(tr("选针器%1过流报警！")).arg(i+1);
        alarmName[22][i]=QString(tr("选针器%1过流报警！")).arg(i+9);
        alarmName[23][i]=QString(tr("选针器%1断路报警！")).arg(i+1);
        alarmName[24][i]=QString(tr("选针器%1断路报警！")).arg(i+9);
        alarmName[25][i]=QString(tr("纱嘴板%1断路报警")).arg(i+1);
        alarmName[26][i]=QString(tr("功能气阀板%1断路报警")).arg(i+1);
        alarmName[27][i]=QString(tr("密度电机%1断路报警")).arg(i+1);
        for(int j=9;j<=27;j++)
        {
            if(j<18)
            {
                alarmwifi[j].code[i]=26+j;
                alarmwifi[j].value[i]=i+1;
            }
            else if(j==18)
            {
                alarmwifi[j].code[i]=26+j-1;
                alarmwifi[j].value[i]=i+9;
            }
            else if(j>18&&j<=20)
            {
                alarmwifi[j].code[i]=26+j-1;
                alarmwifi[j].value[i]=i+1;
            }
            else
            {
                alarmwifi[j].code[i]=0;
                alarmwifi[j].value[i]=0;
            }
        }
    }
    alarmName[9][3]=tr("伺服反转！");
    alarmName[9][4]=tr("点动超速！");
    alarmName[9][5]=tr("编织圈数过大！");
    alarmName[9][6]=tr("1-3-5-7路氨纶！");
    alarmName[9][7]=tr("2-4-6-8路氨纶！");
}

WarningPatternwoven::~WarningPatternwoven()
{
    delete ui;
}

void WarningPatternwoven::getStopMark(bool stop)
{
    if(stop)
    {
        if(timer_warningpv->isActive())
        {
            timer_warningpv->stop();
            ui->plainTextEdit->clear();
        }
        warning_times=0;
    }
    else
    {
        if(!timer_warningpv->isActive())
        {
            timer_warningpv->start(100);
            warning_times=0;
        }
    }
}

void WarningPatternwoven::showWarningMessage_pv()
{
   if(warningShow_mark==1)
   {
       if(this->isHidden())
       {
           char require=Ram.Receive(0x303);
           if(require==0x55)
           {
               QString filename="/xrfile/WarningHistory/"+QDate::currentDate().toString("yyyyMMdd")+".warn";
               qint32 time=QDateTime::currentDateTime().toTime_t();
               ui->plainTextEdit->clear();
               QFile file(filename);
               file.open(QIODevice::ReadWrite);
               QDataStream in(&file);
               in.device()->seek(file.size());
               quint16 addr=0x370;
               for(int i=0;i<29;i++,addr++)
               {
                   alarmAddr[i]=Ram.Receive(addr);
                   for(int j=0;j<8;j++)
                   {
                       time=QDateTime::currentDateTime().toTime_t();
                       if((alarmAddr[i]>>j&0x01)==1)
                       {
                           in<<time<<addr<<quint8(j+1);
                           emit sendAlarm(alarmName[i][j],alarmwifi[i].code[j],alarmwifi[i].value[j]);
                           ui->plainTextEdit->insertPlainText(alarmName[i][j]+"\n");
                       }
                   }
               }
               file.close();
           }
           ui->plainTextEdit->show();
       }
       //报警消除
       char cancle=Ram.Receive(0x305);
       if(cancle==0x55)
       {
           emit alarmclear();
           warning_times=0;
           Ram.Send(0xAA,0x305);
           Ram.Send(0xAA,0x303);
           this->hide();
       }
   }
}

void WarningPatternwoven::back_slot()
{
    this->close();
}
