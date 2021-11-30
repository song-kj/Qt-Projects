#include "btsrsetid.h"
#include "ui_btsrsetid.h"
#include "Tool/KeyBoard/dialogcalculator.h"
#include <QFile>
#include <QDataStream>

extern QByteArray temp;
BtsrSetID::BtsrSetID(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BtsrSetID)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QFile file("/xrfile/btsrParameter.txt");
    quint8 btsr_num=0;
    if(file.open(QIODevice::ReadWrite))
    {
        QDataStream out(&file);
        out>>btsr_num;//读取电子眼个数
        file.close();
    }
    ui->toolButton_setnum->setText(QString::number(btsr_num));
    ui->toolButton_start->setText(QString::number(0));
    ui->label_3->setText("");
    keyboard_timer = new QTimer(this);
    connect(keyboard_timer,SIGNAL(timeout()),this,SLOT(InputEvent()));
    keyboard_timer->start(10);
}

BtsrSetID::~BtsrSetID()
{
    delete ui;
}

//输入感应器个数
void BtsrSetID::on_toolButton_setnum_clicked()
{
    DialogCalculator *sr=new DialogCalculator;
    sr->maxValue=100;
    sr->minValue=0;
    if(sr->exec()==QDialog::Accepted)
    {
        ui->toolButton_setnum->setText(sr->str);
    }
    delete sr;
}

//开始编号并把感应器个数写入文件
void BtsrSetID::on_pushButton_setid_clicked()
{
    ui->label_3->setText("");
    quint8 s=ui->toolButton_start->text().toUInt();
    quint8 sum=ui->toolButton_setnum->text().toUInt();
    Ram.SendData[0]=0xA6;
    Ram.SendData[1]=0x01;
    Ram.SendData[2]=0x01;
    Ram.SendData[3]=0X01;//开始编号
    Ram.SendData[4]=s;//起始编号
    Ram.SendData[5]=sum;//总个数
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    QFile file("/xrfile/btsrParameter.txt");
    if(file.open(QIODevice::ReadWrite))
    {
        QDataStream in(&file);
        in<<sum;
        file.close();
        system("sync");
    }
}

void BtsrSetID::on_pushButton_setid_stop_clicked()
{
    ui->label_3->setText("");
    quint8 sum=ui->toolButton_setnum->text().toUInt();
    Ram.SendData[0]=0xA6;
    Ram.SendData[1]=0x01;
    Ram.SendData[2]=0x01;
    Ram.SendData[3]=0X02;//停止编号
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=sum;//总个数
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    QFile file("/xrfile/btsrParameter.txt");
    if(file.open(QIODevice::ReadWrite))
    {
        QDataStream in(&file);
        in<<sum;
        file.close();
        system("sync");
    }
}

//返回
void BtsrSetID::on_return_btn_clicked()
{
    this->close();
}

//定时器槽函数 键盘操作和编号反馈信息
void BtsrSetID::InputEvent()
{
    if(temp.length()>=1&&this->isActiveWindow())
    {
        if(temp[0]==char(10))
        {
            temp[0]=255;
            ui->return_btn->click();
        }
        else if(temp[0]==char(34))//ok
        {
            temp[0]=255;
            ui->pushButton_setid->click();
        }
    }
    if(Ram.Receive(0x300)==0x55)
    {
        if(Ram.Receive(0x200)==0xA6)
        {
            if(Ram.Receive(0x201)==0x01)
            {
                if(Ram.Receive(0x202)==0x02)//信息反馈
                {
                    Ram.Send(0xaa,0x300);
                    if(Ram.Receive(0x203)==0x01)//结果信息
                    {
                        quint8 a=Ram.Receive(0x204);
                        if(a==0x01)
                            ui->label_3->setText(tr("提示：号码范围错误"));
                        else if(a==0x02)
                            ui->label_3->setText(tr("提示：CRC错误"));
                    }
                    else if(Ram.Receive(0x203)==0x02)//设定成功
                    {
                        quint8 num=Ram.Receive(0x204);
                        ui->label_3->setText(tr("提示：%1号编号成功！").arg(num));
                    }
                }
            }
        }
    }
}

void BtsrSetID::on_toolButton_start_clicked()
{
    DialogCalculator *sr=new DialogCalculator;
    sr->maxValue=100;
    sr->minValue=0;
    if(sr->exec()==QDialog::Accepted)
    {
        ui->toolButton_start->setText(sr->str);
    }
    delete sr;
}

