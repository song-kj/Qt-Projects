#include "versionquery.h"
#include "ui_versionquery.h"
#include <QListView>
#include "myheader.h"

extern QByteArray temp;
VersionQuery::VersionQuery(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionQuery)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    initialForm();
    Ram.to_TestingMain_slot();
}

VersionQuery::~VersionQuery()
{
    delete ui;
}

//查询
void VersionQuery::on_btn_query_clicked()
{
    if(!querying&&version_type>0)
    {
        ui->tableWidget->setRowCount(0);
        Ram.SendData[0]=0x17;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=(quint8)version_type;
        Ram.SendData[3]=0x55;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        if(Ram.UI_to_RTC())
        {
            timecount=0;
            querying=true;
            ui->tableWidget->setRowCount(0);
            ui->label_tips->setText(tr("版本查询中，请稍后..."));
            ui->label_tips->show();
        }
//        readVersions();
    }
}

//返回
void VersionQuery::on_btn_back_clicked()
{
    this->close();
}

void VersionQuery::timeout()
{
    if(querying)
    {
        if(timecount>100)
        {
            querying=false;
            ui->label_tips->setText(tr("查询超时！"));
        }
        timecount++;
        if(Ram.Receive(0x300)==0x55)
        {
            if(Ram.Receive(0x200)==0x17)
            {
                if((Ram.Receive(0x201)==0xA2))
                {
                    if(Ram.Receive(0x202)==(quint8)version_type)//驱动板类型
                    {
                        if(Ram.Receive(0x203)==0x55)
                        {
                            Ram.Send(0xAA,0x300);
                            readVersions();
                            querying=false;
                            ui->label_tips->hide();
                        }
                    }
                }
            }
        }
    }
    //按键操作
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        quint8 key=temp[0];
        temp.clear();
        if(key==34)
            ui->btn_query->click();
        else if(key==10||key==28)
            ui->btn_back->click();
    }
}


void VersionQuery::initialForm()
{
    ui->comboBox->setView(new QListView());
    ui->label_tips->hide();
    QStringList headerLabels;
    headerLabels<<"名称"<<"版本号";
    ui->tableWidget->setColumnCount(headerLabels.length());
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//设置表头列等宽
    ui->tableWidget->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(45); //设置行高
    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget->setShowGrid(true); //设置不显示格子线
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//设置单行选择
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{color:white;background-color:#34495e;"
                                                       "font-size:22px;border:0px solid white;"
                                                       "border-width:0px 0px 0px 0px;}"); //设置表头背景色

    ui->tableWidget->setStyleSheet("QScrollBar{background:white; width: 28px;margin: 28px 0 28px 0;}"
                                   "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:0px;}"
                                   "QScrollBar::handle:hover{background:gray;}"
                                   "QScrollBar::handle:vertical{min-height:28px;}"
                                   "QScrollBar::sub-line{background:white;image: url(:/img/top-line.png);height:28px;subcontrol-position: top;subcontrol-origin: margin;}"
                                   "QScrollBar::sub-line:hover{image: url(:/img/top-line-hover.png);}"
                                   "QScrollBar::sub-line:pressed{image: url(:/img/top-line-pressed.png);}"
                                   "QScrollBar::add-line{background:white;image: url(:/img/bottom-line.png);height:28px;subcontrol-position: bottom;subcontrol-origin: margin;}"
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

    querying=false;
    version_type=0;
    versionShow();
//    borderNum=16;
//    for(int i=0;i<borderNum;i++)
//    {
//        borderNames<<tr("选针器板%1").arg(i+1);
//    }
    timer1=new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(timeout()));
    timer1->start(20);
}

void VersionQuery::readVersions()
{
    int addr=0x500;
    for(int i=0;i<borderNum;i++)
    {
        versions[i].year=Ram.Receive(addr);addr++;
        versions[i].month=Ram.Receive(addr);addr++;
        versions[i].day=Ram.Receive(addr);addr++;
        versions[i].num=Ram.Receive(addr);addr++;
    }
    ui->tableWidget->setRowCount(borderNum);
    for(int i=0;i<borderNum;i++)
    {
        QTableWidgetItem *item1=new QTableWidgetItem;
        item1->setTextAlignment(Qt::AlignCenter);
        item1->setFlags(Qt::NoItemFlags);
        item1->setText(borderNames[i]);

        QTableWidgetItem *item2=new QTableWidgetItem;
        item2->setTextAlignment(Qt::AlignCenter);
        item2->setFlags(Qt::NoItemFlags);
        item2->setText(tr("%1.%2.%3.%4").arg(versions[i].year,2,10,QChar('0')).arg(versions[i].month,2,10,QChar('0'))
                       .arg(versions[i].day,2,10,QChar('0')).arg(versions[i].num,2,10,QChar('0')));

        ui->tableWidget->setItem(i,0,item1);
        ui->tableWidget->setItem(i,1,item2);
    }
}

void VersionQuery::versionShow()
{
    borderNames.clear();
    ui->tableWidget->setRowCount(0);
    if(version_type==0)
    {
        borderNum=1;
        borderNames<<tr("人机程序版本");
        ui->tableWidget->setRowCount(borderNum);
        for(int i=0;i<borderNum;i++)
        {
            QTableWidgetItem *item1=new QTableWidgetItem;
            item1->setTextAlignment(Qt::AlignCenter);
            item1->setFlags(Qt::NoItemFlags);
            item1->setText(borderNames[i]);

            QTableWidgetItem *item2=new QTableWidgetItem;
            item2->setTextAlignment(Qt::AlignCenter);
            item2->setFlags(Qt::NoItemFlags);
            item2->setText(tr("RifaUWM_UI_201028R13"));
            ui->tableWidget->setItem(i,0,item1);
            ui->tableWidget->setItem(i,1,item2);
        }
    }
    else if(version_type==1)
    {
        borderNum=16;
        for(int i=0;i<borderNum;i++)
        {
            borderNames<<tr("选针器板%1").arg(i+1);
        }
    }
    else if(version_type==2)
    {
        borderNum=16;
        for(int i=0;i<8;i++)
        {
            borderNames<<tr("纱嘴板%1").arg(i+1);
        }
        for(int i=0;i<8;i++)
        {
            borderNames<<tr("功能气阀板%1").arg(i+1);
        }
    }
    else if(version_type==3)
    {
        borderNum=3;
        for(int i=0;i<borderNum;i++)
        {
            borderNames<<tr("电机板%1").arg(i+1);
        }
    }
    else if(version_type==4)
    {
        borderNum=1;
        borderNames<<tr("主控板");
    }
}

void VersionQuery::on_comboBox_currentIndexChanged(int index)
{
    ui->label_tips->hide();
    version_type=index;
    versionShow();
}
