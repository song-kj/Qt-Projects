#include "dialogsyssetiing.h"
#include "ui_dialogsyssetiing.h"
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include "mymessagebox.h"
#include "Communication/noanswer.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Tool/KeyBoard/keybd.h"

extern QByteArray temp;
extern bool sysset_enable;
DialogSysSetiing::DialogSysSetiing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSysSetiing)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    initialForm();
    hire = new Hirepurchase;
    ui->lineEdit->setText(sys_maintenance.id);
    showStartTime(sys_maintenance.starttime);
    showEndingTime(sys_maintenance.endtime);
    pTimer = new QTimer(this);
    connect(pTimer,SIGNAL(timeout()),this,SLOT(update_slot()));
    pTimer->start(100);
    watcher = new QFileSystemWatcher(this);
    QStringList strList;
    strList<<"/media"<<"/media/myusb";
    watcher->addPaths(strList);
    connect(watcher,SIGNAL(directoryChanged(QString)),this,SLOT(getUSB(QString)));

}
DialogSysSetiing::~DialogSysSetiing()
{
    delete ui;
}

void DialogSysSetiing::initialForm()
{
    clickCount=0;
    isfirstEnter=true;
    QStringList header;
    header<<tr("名称")<<tr("大小");
    ui->USB_tw->setColumnCount(2);
    ui->USB_tw->horizontalHeader()->resizeSection(0,300);
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
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        ui->label_tips->show();
    }
    else
    {
        ui->label_tips->hide();
    }
}


void DialogSysSetiing::getUSB(QString str)
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

void DialogSysSetiing::showinfo_USB()
{
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        dir.setFilter(QDir::Dirs|QDir::Files);
        QStringList filters;
        filters << "*.xr";
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

/*配置*/
void DialogSysSetiing::on_pushButton_clicked()
{
    if(ui->USB_tw->currentRow()>-1)
    {
        if(sysset_enable==false)
        {
            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("当前状态不能配置！"),0);
            box->exec();
            return;
        }
        if(sys_maintenance.setting_mark==true)
        {
            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("不能重复配置！"),0);
            box->exec();
            return;
        }
        int row = ui->USB_tw->currentRow();
        QString fileName=Path_myusb+ui->USB_tw->item(row,0)->text();
        QFile file(fileName);
        if(file.exists())
        {
            if(file.size()!=0)
            {
                if(hire->sysetemSetting(fileName))
                {
                    if(hire->sysStartTime=="")
                    {
                        MyMessageBox *box = new MyMessageBox(tr("提示"),tr("配置文件出错！"),0);
                        box->exec();
                    }
                    else
                    {
                        QFile fileId(tr("/xrfile/").append("id.txt"));
                        if(fileId.exists())
                            fileId.remove();
                        if(fileId.open(QIODevice::ReadWrite))
                        {
                            QTextStream in(&fileId);
                            in<<hire->sysID;
                            fileId.close();
                            system("sync");
                            ui->lineEdit->setText(hire->sysID);
                            sys_maintenance.id=hire->sysID;
                        }
                        QFile fileSt(tr("/xrfile/").append("startTime.txt"));
                        if(fileSt.exists())
                            fileSt.remove();
                        if(fileSt.open(QIODevice::ReadWrite))
                        {
                            QTextStream in(&fileSt);
                            in<<hire->sysStartTime;
                            fileSt.close();
                            system("sync");
                            showStartTime(hire->sysStartTime);
                            sys_maintenance.starttime=hire->sysStartTime;
                        }
                        QFile fileEt(tr("/xrfile/").append("endingTime.txt"));
                        if(fileEt.exists())
                            fileEt.remove();
                        if(fileEt.open(QIODevice::ReadWrite))
                        {
                            QTextStream in(&fileEt);
                            fileEt.resize(0);
                            in<<hire->sysDeadlineTime;
                            fileEt.close();
                            system("sync");
                            showEndingTime(hire->sysDeadlineTime);
                            sys_maintenance.endtime=hire->sysDeadlineTime;
                        }
                        QFile filecompanyCodeAscii(tr("/xrfile/").append("companyCodeAscii.txt"));
                        if(filecompanyCodeAscii.exists())
                            filecompanyCodeAscii.remove();
                        if(filecompanyCodeAscii.open(QIODevice::ReadWrite))
                        {
                            QTextStream in(&filecompanyCodeAscii);
                            in<<hire->companyCodeAscii;
                            filecompanyCodeAscii.close();
                            system("sync");
                            sys_maintenance.CompanyCodeAscii=hire->companyCodeAscii;
                        }
                        QFile filecompanyCodeAsciiLength(tr("/xrfile/").append("companyCodeAsciiLength.txt"));
                        if(filecompanyCodeAsciiLength.exists())
                            filecompanyCodeAsciiLength.remove();
                        if(filecompanyCodeAsciiLength.open(QIODevice::ReadWrite))
                        {
                            QTextStream in(&filecompanyCodeAsciiLength);
                            in<<hire->companyCodeAsciiLength;
                            filecompanyCodeAsciiLength.close();
                            system("sync");
                            sys_maintenance.CompanyCodeAsciiLength=hire->companyCodeAsciiLength;
                        }
                        QFile filecompanyCode(tr("/xrfile/").append("companyCode.txt"));
                        if(filecompanyCode.exists())
                            filecompanyCode.remove();
                        if(filecompanyCode.open(QIODevice::ReadWrite))
                        {
                            QTextStream in(&filecompanyCode);
                            in<<hire->companyCode;
                            filecompanyCode.close();
                            system("sync");
                            sys_maintenance.CompanyCode=hire->companyCode;
                        }

                        QFile fileSettingMark(tr("/xrfile/").append("SettingMark.txt"));
                        if(fileSettingMark.exists())
                            fileSettingMark.remove();
                        if(fileSettingMark.open(QIODevice::ReadWrite))
                        {
                            QTextStream in(&fileSettingMark);
                            in<<"1";
                            fileSettingMark.close();
                            system("sync");
                            sys_maintenance.setting_mark=true;
                        }
                        QString id = hire->sysID;
                        for(int i=0;i<10;i++)
                        {
                            sys_maintenance.idInt[i] = id.left(1).toInt();
                            id = id.remove(0,1);
                        }
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
                }
            }
            else
            {
                MyMessageBox *box = new MyMessageBox(tr("提示"),tr("配置文件出错！"),0);
                box->exec();
            }
        }
    }
}

void DialogSysSetiing::showStartTime(QString str)
{
    QString year = str.left(4);
    str = str.remove(0,4);
    QString  month = str.left(2);
    str = str.remove(0,2);
    QString day = str.left(2);
    str = str.remove(0,2);
    QString hour = str.left(2);
    str = str.remove(0,2);
    QString minute = str.left(2);
    QString second = str.right(2);
    ui->startTime_le->setText(year.append("/").append(month).append("/").append(day).append(" ").append(hour).append(":").append(minute).append(":").append(second));
}

void DialogSysSetiing::showEndingTime(QString str)
{
    QString year = str.left(4);
    str = str.remove(0,4);
    QString month = str.left(2);
    QString day = str.right(2);
    ui->deadline_le->setText(year.append(tr("/")).append(month).append(tr("/")).append(day));
}

void DialogSysSetiing::on_pushButton_2_clicked()
{
    pTimer->stop();/***/
    reject();
}

void DialogSysSetiing::update_slot()
{
    if(isfirstEnter)
    {
        isfirstEnter=false;
        showinfo_USB();
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
                        MyMessageBox *box = new MyMessageBox(tr("提示"),tr("机号配置成功！"),0);
                        box->exec();
                    }
                    else if(Ram.Receive(0x203) == 0x55)
                    {
                        MyMessageBox *box = new MyMessageBox(tr("提示"),tr("机号配置失败！"),0);
                        box->exec();
                    }
                }
            }
        }
    }
    if(this->isActiveWindow()&&temp.length()>0)
    {
        if(temp[0] == (char)28)
        {

            temp[0]=255;
            on_pushButton_2_clicked();
        }
        else if(temp[0] ==(char)34)
        {
            temp[0]=255;
            on_pushButton_clicked();
        }
    }
}

//清除分期付款信息
void DialogSysSetiing::on_pushButton_clear_clicked()
{
    clickCount++;
    if(clickFirstTime.isNull())
    {
        clickFirstTime=QDateTime::currentDateTime();
    }
    if(clickFirstTime.addSecs(10).operator <=(QDateTime::currentDateTime()))
    {
        clickFirstTime=QDateTime::currentDateTime();
        clickCount=1;
    }
    if(clickCount>=8)
    {
        clickCount=0;
        clickFirstTime=QDateTime::currentDateTime();
        temp[0]=255;
        KeyBd *bd = new KeyBd;
        if(bd->exec()==QDialog::Accepted)
        {
            if(bd->inputstr == "16540418")
            {
                Ram.SendData[0]=0xA0;
                Ram.SendData[1]=0x20;
                Ram.SendData[2]=0x0A;
                Ram.SendData[3]=0x03;
                Ram.SendData[4]=0xAA;
                Ram.SendData[5]=0x00;
                Ram.SendData[6]=0x00;
                Ram.SendData[7]=0x00;
                Ram.SendData[8]=0x00;
                Ram.SendData[9]=0x00;
                if(Ram.UI_to_RTC())
                {
                    QFile file("/xrfile/id.txt");
                    if(file.exists())
                        file.remove();
                    file.setFileName("/xrfile/startTime.txt");
                    if(file.exists())
                        file.remove();
                    file.setFileName("/xrfile/endingTime.txt");
                    if(file.exists())
                        file.remove();
                    file.setFileName("/xrfile/companyCodeAscii.txt");
                    if(file.exists())
                        file.remove();
                    file.setFileName("/xrfile/companyCodeAsciiLength.txt");
                    if(file.exists())
                        file.remove();
                    file.setFileName("/xrfile/companyCode.txt");
                    if(file.exists())
                        file.remove();
                    file.setFileName("/xrfile/SettingMark.txt");
                    if(file.exists())
                        file.remove();
                    system("sync");
                    MyMessageBox *box = new MyMessageBox(tr("提示"),tr("清除成功！"),0);
                    box->exec();
                }
            }
            else
            {
                MyMessageBox *box = new MyMessageBox(tr("提示"),tr("密码错误！"),0);
                box->exec();
            }
        }
        delete  bd;
    }
}
