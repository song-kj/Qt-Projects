#include "dialogjiesuo.h"
#include "ui_dialogjiesuo.h"
#include <QFile>
#include "mymessagebox.h"
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include "Tool/KeyBoard/keybd.h"
#include "Tool/KeyBoard/softwear.h"
#include "Communication/noanswer.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDebug>
extern QByteArray temp;

DialogJiesuo::DialogJiesuo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogJiesuo)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    hire = new Hirepurchase;
    QStringList header;
    header<<tr("名称")<<tr("大小");
    ui->USB_tw->setColumnCount(2);
    ui->USB_tw->horizontalHeader()->resizeSection(0,400);
    ui->USB_tw->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->USB_tw->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->USB_tw->verticalHeader()->setDefaultSectionSize(35); //设置行高
//    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->USB_tw->setShowGrid(true); //设置不显示格子线
    ui->USB_tw->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->USB_tw->horizontalHeader()->setVisible(true);
    ui->USB_tw->setHorizontalHeaderLabels(header);
    ui->USB_tw->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->USB_tw->setSelectionMode(QAbstractItemView::SingleSelection);//设置单行选择
    ui->USB_tw->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->USB_tw->setAlternatingRowColors(true);//设置奇偶行颜色不同
    ui->USB_tw->horizontalHeader()->setStyleSheet("QHeaderView::section{color:black;background-color:#ffffff;"
                                                       "font-size:22px;border:1px solid gray;"
                                                       "border-width:1px 1px 1px 1px;}"); //设置表头背景色

    ui->USB_tw->setStyleSheet("QScrollBar{background:transparent; width: 28px;margin: 28px 0 28px 0;}"
                                   "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:0px;}"
                                   "QScrollBar::handle:hover{background:gray;}"
                                   "QScrollBar::handle:vertical{min-height:28px;}"
                                   "QScrollBar::sub-line{background:transparent;image: url(:/img/top-line.png);height:28px;subcontrol-position: top;subcontrol-origin: margin;}"
                                   "QScrollBar::sub-line:hover{image: url(:/img/top-line-hover.png);}"
                                   "QScrollBar::sub-line:pressed{image: url(:/img/top-line-pressed.png);}"
                                   "QScrollBar::add-line{background:transparent;image: url(:/img/bottom-line.png);height:28px;subcontrol-position: bottom;subcontrol-origin: margin;}"
                                   "QScrollBar::add-line:hover{image: url(:/img/bottom-line-hover.png);}"
                                   "QScrollBar::add-line:pressed{image: url(:/img/bottom-line-pressed.png);}"
                                   "QTableWidget{selection-background-color:#4d93fd;"
                                   "color:black;"
                                   "gridline-color: gray;"
                                   "selection-color:white;"
                                   "font-size:20px;"
                                   "background-color:#ffffff;"
                                   "alternate-background-color:#a8ddfd;"
                                   "border:1px solid gray;}");
    isfirstEnter=true;
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        ui->label_tips->show();
    }
    else
    {
        ui->label_tips->hide();
    }

//    ui->tabWidget->removeTab(0);
//    ui->autoJiesuo_btn->hide();
    hire->setData(sys_maintenance.CompanyCodeAsciiLength,sys_maintenance.CompanyCodeAscii);
    ui->lineEdit->setText(sys_maintenance.id);
    if(sys_maintenance.endtime.length()>=8&&sys_maintenance.setting_mark==true)
    {
        ui->lineEdit_2->setText(QString("%1-%2-%3").arg(sys_maintenance.endtime.left(4)).arg(sys_maintenance.endtime.mid(4,2))
                                .arg(sys_maintenance.endtime.right(2)));
        ui->lineEdit_3->setText(QString("%1-%2-%3").arg(sys_maintenance.endtime.left(4)).arg(sys_maintenance.endtime.mid(4,2))
                                .arg(sys_maintenance.endtime.right(2)));
    }
    pTimer = new QTimer(this);
    connect(pTimer,SIGNAL(timeout()),this,SLOT(update_slot()));
    pTimer->start(10);
    watcher = new QFileSystemWatcher(this);
    QStringList strList;
    strList<<"/media"<<"/media/myusb";
    watcher->addPaths(strList);
    connect(watcher,SIGNAL(directoryChanged(QString)),this,SLOT(getUSB(QString)));
    createRandCode();
}

DialogJiesuo::~DialogJiesuo()
{
    delete ui;
}

void DialogJiesuo::getUSB(QString str)
{
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        showinfo_USB();
    }
    else
    {
        ui->USB_tw->setRowCount(0);
    }
}

void DialogJiesuo::showinfo_USB()
{
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        dir.setFilter(QDir::Dirs|QDir::Files);
        QStringList filters;
        filters << "*.xuren";
        dir.setNameFilters(filters);
        QFileInfoList list = dir.entryInfoList();
        if(list.size()>0)
        {
            ui->USB_tw->setRowCount(list.size());
            for(int i = 0;i < list.size();i++)
            {
                QFileInfo file_info = list.at(i);
                ui->USB_tw->setItem(i,0,new QTableWidgetItem( file_info.fileName()));

                qint64 size = file_info.size();
                QTableWidgetItem *suffixAndsizeItem = new QTableWidgetItem();
                if(size<1024)
                    suffixAndsizeItem->setText(QString("%1 B").arg(size));
                else if(size>=1024&&size<0x100000)
                {
                    suffixAndsizeItem->setText(QString("%1 KB").arg(int((size + 1023) / 1024)));
                }
                else
                {
                    suffixAndsizeItem->setText(QString("%1 MB").arg(QString::number((float)(size/1048576.0),'f',1)));
                }
                suffixAndsizeItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->USB_tw->setItem(i,1,suffixAndsizeItem);
            }
            ui->USB_tw->setCurrentCell(0,0);
        }
        ui->label_tips->hide();
    }
}

void DialogJiesuo::createRandCode()
{
    QSettings *set=new QSettings(Path_hirepurchase,QSettings::IniFormat);
    uint creat_t=set->value("Rand/time","0").toUInt();
    uint cur_t=QDateTime::currentDateTime().toTime_t();
    QString str=set->value("Rand/code","").toString();

    if(cur_t-creat_t>86400||str.length()!=8)
    {
        ui->pushButton->setText(hire->createRandomCode());
        set->setValue("Rand/code",ui->pushButton->text());
        set->setValue("Rand/time",QString::number(cur_t));
        system("sync");
    }
    else
    {
        ui->pushButton->setText(str);
    }
    delete set;
}

void DialogJiesuo::on_pushButton_2_clicked()
{
    Softwear bd;
    connect(&bd,SIGNAL(setNumber(QString)),this,SLOT(getString(QString)));
    bd.exec();
}

void DialogJiesuo::getString(QString num)
{
    QString str=hire->decryption(num);
    if(str.mid(0,8)==ui->pushButton->text())
    {
        bool ok;
        uint time_t=str.right(8).toUInt(&ok,16);
        QDateTime t=QDateTime::fromTime_t(time_t);
        str=t.toString("yyyyMMdd");
        hire->sysDeadlineTimeEnding=t.toString("yyyyMMdd");
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
    }
    else
    {
        MyMessageBox *box  = new MyMessageBox(tr("提示"),tr("密钥错误！"),0);
        box->exec();
    }
}

/*产生随机码*/
void DialogJiesuo::on_pushButton_clicked()
{
    createRandCode();
}

/*自动解锁*/
void DialogJiesuo::on_autoJiesuo_btn_clicked()
{
    if(ui->USB_tw->currentRow()>-1)
    {
        QString filename=Path_myusb+ui->USB_tw->item(ui->USB_tw->currentRow(),0)->text();
        QFile fileU(filename);
        if(fileU.exists())
        {
            if(fileU.size()!=0)
            {
                int rst=hire->unlockedAuto(sys_maintenance.id,sys_maintenance.CompanyCode,filename);
                if(rst==1)
                {
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
                }
                else
                {
                    QString str="";
                    if(rst==2)
                        str=tr("设备编号错误，解锁失败！");
                    else if(rst==3)
                        str=tr("厂家名称错误，解锁失败！");
                    else if(rst==4)
                        str=tr("解锁时间小于当前系统时间，解锁失败！");
                    else if(rst==5)
                        str=tr("设备识别码错误，解锁失败！");
                    else
                        str=tr("解锁失败！");
                    MyMessageBox *box = new MyMessageBox(tr("提示"),str,0);
                    box->exec();
                }
            }
        }
    }
}

void DialogJiesuo::update_slot()
{
    if(isfirstEnter)
    {
        isfirstEnter=false;
        showinfo_USB();
    }
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0] == (char)1)
        {
            temp[0]=255;
            on_pushButton_clicked();
        }
        else if(temp[0] == (char)2)
        {
            temp[0]=255;

        }
        else if(temp[0] ==(char)28||temp[0]==(char)10)
        {
            temp[0]=255;
            ui->btn_back->click();
        }
        else if(temp[0] ==(char)34)
        {
            temp[0]=255;
            on_autoJiesuo_btn_clicked();
        }
    }
    if(Ram.Receive(0x300)==0x55)
    {
        if((unsigned char)Ram.Receive(0x200) == 0xA0)
        {
            if(Ram.Receive(0x201) == 0x20)
            {
                if(Ram.Receive(0x202) == 0x07)
                {
                    Ram.Send(0xAA,0x300);
                    if((unsigned char)Ram.Receive(0x203) == 0xAA)
                    {
                        QFile fileEt(tr("/xrfile/").append("endingTime.txt"));
                        if(fileEt.open(QIODevice::ReadWrite))
                        {
                            QTextStream in(&fileEt);
                            fileEt.resize(0);
                            in<<hire->sysDeadlineTimeEnding;
                            fileEt.close();
                            sys_maintenance.endtime=hire->sysDeadlineTimeEnding;
                            system("sync");
                            ui->lineEdit_2->setText(QString("%1-%2-%3").arg(sys_maintenance.endtime.left(4)).arg(sys_maintenance.endtime.mid(4,2))
                                                    .arg(sys_maintenance.endtime.right(2)));
                            ui->lineEdit_3->setText(QString("%1-%2-%3").arg(sys_maintenance.endtime.left(4)).arg(sys_maintenance.endtime.mid(4,2))
                                                    .arg(sys_maintenance.endtime.right(2)));
                        }

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
                        MyMessageBox *box = new MyMessageBox(tr("提示"),tr("解锁成功！"),0);
                        box->exec();
                    }
                    else if(Ram.Receive(0x203) == 0x55)
                    {
                        MyMessageBox *box = new MyMessageBox(tr("提示"),tr("解锁失败！"),0);
                        box->exec();
                    }
                }
            }
        }
    }
}

//返回
void DialogJiesuo::on_btn_back_clicked()
{
    pTimer->stop();/*****/
    reject();
}

void DialogJiesuo::on_tabWidget_currentChanged(int index)
{
    if(index==0)
        ui->autoJiesuo_btn->show();
    else
        ui->autoJiesuo_btn->hide();
}
