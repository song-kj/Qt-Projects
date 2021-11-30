#include "faultdetection.h"
#include "ui_faultdetection.h"

extern QByteArray temp;

FaultDetection::FaultDetection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FaultDetection)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    initialForm();
}

FaultDetection::~FaultDetection()
{
    delete ui;
}

void FaultDetection::initialForm()
{
    //进入测试界面
    Ram.to_TestingMain_slot();
    feeder=1;
    points=0;
    timecount=0;
    detecting=false;
    ui->label_feeder->setText(QString::number(feeder));
    ui->label_tips->hide();
    for(int i=0;i<16;i++)
    {
        tlb[i]=new QToolButton(this);
        tlb[i]->setGeometry(170+(i%8)*60,260+(i/8)*80,41,41);
        tlb[i]->setCheckable(true);
        tlb[i]->setEnabled(false);
        tlb[i]->setText(QString::number(i+1));
    }
    timer1=new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(timeout()));
    timer1->start(100);

}

void FaultDetection::refreshForm()
{
    for(int i=0;i<16;i++)
    {
        if(((points>>i)&1)==1)
        {
            tlb[i]->setChecked(true);
        }
        else
            tlb[i]->setChecked(false);
    }
}

void FaultDetection::on_pushButton_back_clicked()
{
    //发送气阀状态回归指令
    Ram.SendData[0]=0x11;
    Ram.SendData[1]=0xA4;
    Ram.SendData[2]=0x03;
    Ram.SendData[3]=feeder;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    this->close();
}

void FaultDetection::timeout()
{
    if(detecting)
    {
        timecount++;
        if(timecount>20)
        {
            ui->label_tips->setText(tr("检测超时！"));
            detecting=false;
        }
    }
    if(Ram.Receive(0x300)==0x55)
    {
        if(Ram.Receive(0x200)==0x11)
        {
            if(Ram.Receive(0x201)==0xA4)
            {
                if(Ram.Receive(0x202)==0x02)
                {
                    if(Ram.Receive(0x203)==feeder)
                    {
                        Ram.Send(0xAA,0x300);
                        //故障点位
                        points=Ram.Receive(0x204)+Ram.Receive(0x205)*256;
                        refreshForm();
                        ui->label_tips->hide();
                        detecting=false;
                    }
                }
            }
        }
    }
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        quint8 key=temp[0];
        temp.clear();
        if(key==10||key==28)
        {
            ui->pushButton_back->click();
        }
        else if(key==34)//ok
        {
            ui->pushButton_detection->click();
        }
        else if(key==33)//++
        {
            ui->pushButton_add->click();
        }
        else if(key==35)//--
        {
            ui->pushButton_sub->click();
        }
    }
}

//检测
void FaultDetection::on_pushButton_detection_clicked()
{
    feeder=ui->label_feeder->text().toInt();
    Ram.SendData[0]=0x11;
    Ram.SendData[1]=0xA4;
    Ram.SendData[2]=0x01;
    Ram.SendData[3]=feeder;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    timecount=0;
    detecting=true;
    ui->label_tips->show();
    ui->label_tips->setText(tr("检测中..."));
}

//++
void FaultDetection::on_pushButton_add_clicked()
{
    if(feeder<18)
    {
        feeder++;
        if(feeder==9)
            feeder=11;
        ui->label_feeder->setText(QString::number(feeder));
    }
}

//--
void FaultDetection::on_pushButton_sub_clicked()
{
    if(feeder>1)
    {
        feeder--;
        if(feeder==10)
            feeder=8;
        ui->label_feeder->setText(QString::number(feeder));
    }
}
