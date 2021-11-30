#include "machineconfigure.h"
#include "ui_machineconfigure.h"
#include "Communication/noanswer.h"
#include "mymessagebox.h"


MachineConfigure::MachineConfigure(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MachineConfigure)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    accepted=false;
    issetting=false;
    timer1=new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(timeout()));
    timer1->start(100);
}

MachineConfigure::~MachineConfigure()
{
    delete ui;
}

void MachineConfigure::setconfigureType(configureType type)
{
    confType=type;
    //UI模式下只能进行机号配置
    if(type==typeui)
    {
        ui->label_machineid->setText(sys_maintenance.id);
        ui->label_starttime->setText(tr("%1-%2-%3 %4:%5:%6").arg(sys_maintenance.starttime.mid(0,4)).arg(sys_maintenance.starttime.mid(4,2))
                                     .arg(sys_maintenance.starttime.mid(6,2)).arg(sys_maintenance.starttime.mid(8,2))
                                     .arg(sys_maintenance.starttime.mid(10,2)).arg(sys_maintenance.starttime.mid(12,2)));
        ui->label_endtime->setText(tr("%1-%2-%3").arg(sys_maintenance.endtime.mid(0,4)).arg(sys_maintenance.endtime.mid(4,2))
                                   .arg(sys_maintenance.endtime.mid(6,2)));
        ui->pushButton_read->hide();
        ui->pushButton_save->hide();
    }
    else if(type==typertc)
    {
        ui->pushButton_configure->hide();
    }
}

void MachineConfigure::setID(QString id)
{
    machineID=id;
}

//请求读取主板信息
void MachineConfigure::on_pushButton_read_clicked()
{
    Ram.SendData[0]=0xA0;
    Ram.SendData[1]=0x20;
    Ram.SendData[2]=0x0A;
    Ram.SendData[3]=0x02;
    Ram.SendData[4]=0x55;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    ui->label_tips->setText(tr("读取中"));
}

//配置
void MachineConfigure::on_pushButton_configure_clicked()
{
    ui->pushButton_configure->setEnabled(false);
    Ram.SendData[0]=0xA0;
    Ram.SendData[1]=0x20;
    Ram.SendData[2]=0x07;
    Ram.SendData[3]=sys_maintenance.idInt[0];
    Ram.SendData[4]=sys_maintenance.idInt[1];
    Ram.SendData[5]=sys_maintenance.idInt[2];
    Ram.SendData[6]=sys_maintenance.idInt[3];
    Ram.SendData[7]=sys_maintenance.idInt[4];
    Ram.SendData[8]=sys_maintenance.idInt[5];
    Ram.SendData[9]=sys_maintenance.idInt[6];
    Ram.Send(sys_maintenance.idInt[7],0x10a);
    Ram.Send(sys_maintenance.idInt[8],0x10b);
    Ram.Send(sys_maintenance.idInt[9],0x10c);
    Ram.UI_to_RTC();
    timecount=0;
    issetting=true;
}

//保存
void MachineConfigure::on_pushButton_save_clicked()
{
    if(readed)
    {
        QFile fileId(tr("/xrfile/").append("id.txt"));
        if(fileId.exists())
            fileId.remove();
        if(fileId.open(QIODevice::ReadWrite))
        {
            QTextStream in(&fileId);
            in<<ui->label_machineid->text();
            fileId.close();
            sys_maintenance.id=ui->label_machineid->text();
        }
        QFile fileSt(tr("/xrfile/").append("startTime.txt"));
        if(fileSt.exists())
            fileSt.remove();
        if(fileSt.open(QIODevice::ReadWrite))
        {
            QTextStream in(&fileSt);
            in<<startTime;
            fileSt.close();
            sys_maintenance.starttime=startTime;
        }
        QFile fileEt(tr("/xrfile/").append("endingTime.txt"));
        if(fileEt.exists())
            fileEt.remove();
        if(fileEt.open(QIODevice::ReadWrite))
        {
            QTextStream in(&fileEt);
            fileEt.resize(0);
            in<<endTime;
            fileEt.close();
            sys_maintenance.endtime=endTime;
        }
        QFile filecompanyCodeAscii(tr("/xrfile/").append("companyCodeAscii.txt"));
        if(filecompanyCodeAscii.exists())
            filecompanyCodeAscii.remove();
        if(filecompanyCodeAscii.open(QIODevice::ReadWrite))
        {
            QTextStream in(&filecompanyCodeAscii);
            in<<companyCodeAscii;
            filecompanyCodeAscii.close();
            sys_maintenance.CompanyCodeAscii=companyCodeAscii;
        }
        QFile filecompanyCodeAsciiLength(tr("/xrfile/").append("companyCodeAsciiLength.txt"));
        if(filecompanyCodeAsciiLength.exists())
            filecompanyCodeAsciiLength.remove();
        if(filecompanyCodeAsciiLength.open(QIODevice::ReadWrite))
        {
            QTextStream in(&filecompanyCodeAsciiLength);
            in<<companyCodeAsciiLength;
            filecompanyCodeAsciiLength.close();
            sys_maintenance.CompanyCodeAsciiLength=companyCodeAsciiLength;
        }
        QFile filecompanyCode(tr("/xrfile/").append("companyCode.txt"));
        if(filecompanyCode.exists())
            filecompanyCode.remove();
        if(filecompanyCode.open(QIODevice::ReadWrite))
        {
            QTextStream in(&filecompanyCode);
            in<<companyCode;
            filecompanyCode.close();
            sys_maintenance.CompanyCode=companyCode;
        }

        QFile fileSettingMark(tr("/xrfile/").append("SettingMark.txt"));
        if(fileSettingMark.exists())
            fileSettingMark.remove();
        if(fileSettingMark.open(QIODevice::ReadWrite))
        {
            QTextStream in(&fileSettingMark);
            in<<"1";
            fileSettingMark.close();
            sys_maintenance.setting_mark=true;
        }
        system("sync");
        ui->label_tips->setText(tr("保存成功！"));
        accepted=true;
    }
    else
        ui->label_tips->setText(tr("信息不能为空！"));
}

void MachineConfigure::timeout()
{
    if(Ram.Receive(0x300)==0x55)
    {
        if(Ram.Receive(0x200)==0xA0)
        {
            if(Ram.Receive(0x201)==0x20)
            {
                if(Ram.Receive(0x202)==0x0A)
                {
                    if(Ram.Receive(0x203)==0x01)
                    {
                        if(Ram.Receive(0x204)==0x55)
                        {
                            Ram.Send(0xAA,0x300);
                            //读取0x500处40个字节信息
                            quint8 array[40];
                            for(int i=0;i<40;i++)
                            {
                                array[i]=Ram.Receive(0x500+i);
                            }
                            //获取起始时间和到期时间
                            quint32 start=0;
                            quint32 end=0;
                            for(int i=0;i<4;i++)
                            {
                                start<<=8;
                                start+=array[i];
                                end<<=8;
                                end+=array[i+4];
                            }
                            //获取companycodeascii长度
                            quint8 len=array[8];
                            if(len==2)
                                len=4;
                            QByteArray da;
                            for(int i=0;i<len;i++)
                            {
                                da.append(array[9+i]);
                            }
                            if(machineID=="0000000000")
                            {
                                ui->label_tips->setText(tr("信息错误！"));
                            }
                            else
                            {
                                ui->label_machineid->setText(machineID);
                                QDateTime time=QDateTime::fromTime_t(start);
                                ui->label_starttime->setText(time.toString("yyyy-MM-dd HH:mm:ss"));
                                startTime=time.toString("yyyyMMddHHmmss");
                                time=QDateTime::fromTime_t(end);
                                ui->label_endtime->setText(time.toString("yyyy-MM-dd"));
                                endTime=time.toString("yyyyMMdd");
                                companyCodeAsciiLength=QString::number(len*2);
                                companyCodeAscii=da.toHex();
                                companyCode=da;
                                readed=true;
                                ui->label_tips->setText("");
                            }
                        }
                    }
                }
                else if(Ram.Receive(0x202) == 0x07)
                {
                    Ram.Send(0xAA,0x300);
                    if(Ram.Receive(0x203) == 0xAA)
                    {
                        QString date=QString("%1-%2-%3 23:59:59").arg(sys_maintenance.endtime.left(4))
                                .arg(sys_maintenance.endtime.mid(4,2))
                                .arg(sys_maintenance.endtime.right(2));
                        QDateTime time=QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss");
                        quint32 end=time.toTime_t();

                        date=QString("%1-%2-%3 %4:%5:%6").arg(sys_maintenance.starttime.left(4))
                                .arg(sys_maintenance.starttime.mid(4,2))
                                .arg(sys_maintenance.starttime.mid(6,2))
                                .arg(sys_maintenance.starttime.mid(8,2))
                                .arg(sys_maintenance.starttime.mid(10,2))
                                .arg(sys_maintenance.starttime.mid(12,2));
                        time=QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss");
                        quint32 start=time.toTime_t();

                        quint8 len=sys_maintenance.CompanyCodeAsciiLength.toInt();

                        QByteArray ba;
                        ba.append((quint8)(start>>24));
                        ba.append((quint8)(start>>16));
                        ba.append((quint8)(start>>8));
                        ba.append((quint8)(start));

                        ba.append((quint8)(end>>24));
                        ba.append((quint8)(end>>16));
                        ba.append((quint8)(end>>8));
                        ba.append((quint8)(end));
                        ba.append((len/2));
                        bool ok;
                        for(int i=0;i<sys_maintenance.CompanyCodeAscii.length()/2;i++)
                        {
                            ba.append(sys_maintenance.CompanyCodeAscii.mid(i*2,2).toUInt(&ok,16));
                        }
                        for(int i=0;i<ba.length();i++)
                        {
                            Ram.Send((quint8)(ba[i]),0x500+i);
                        }
                        //信息上传到RTC
                        Ram.SendData[0]=0xA0;
                        Ram.SendData[1]=0x20;
                        Ram.SendData[2]=0x0A;
                        Ram.SendData[3]=0x01;
                        Ram.SendData[4]=0x55;
                        Ram.SendData[5]=0x00;
                        Ram.SendData[6]=0x00;
                        Ram.SendData[7]=0x00;
                        Ram.SendData[8]=0x00;
                        Ram.SendData[9]=0x00;
                        Ram.UI_to_RTC();
                        accepted=true;
                        ui->label_tips->setText(tr("配置成功！"));
                        issetting=false;
                        ui->pushButton_configure->setEnabled(true);
                    }
                    else if(Ram.Receive(0x203) == 0x55)
                    {
                        ui->label_tips->setText(tr("配置失败！"));
                        issetting=false;
                        ui->pushButton_configure->setEnabled(true);
                    }
                }
            }
        }
    }
    if(issetting)
    {
        timecount++;
        if(timecount>20)
            ui->pushButton_configure->setEnabled(true);
    }
}

//返回
void MachineConfigure::on_pushButton_back_clicked()
{
    if(accepted)
        this->accept();
    else
    {
        sys_maintenance.setting_mark=true;
        if(confType==typeui)
        {
            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("信息未配置，退出后系统无法正常使用！"),0);
            box->exec();
        }
        else if(confType==typertc)
        {
            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("信息未保存，退出后系统无法正常使用！"),0);
            box->exec();
        }
        this->reject();
    }
}
