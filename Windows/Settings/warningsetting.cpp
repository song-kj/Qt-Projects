#include "warningsetting.h"
#include "ui_warningsetting.h"
#include <QDebug>
#include "mymessagebox.h"
#include "myheader.h"

extern int startToCompare;
extern QByteArray temp;

WarningSetting::WarningSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarningSetting)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    startsend=false;
    allset=false;
    timecount=0;
    timer1=new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(timeout()));
    timer1->start(20);

    for(int i=0;i<5;i++)
    {
        warnstate[i*2]=machine_Pars.alarm_setted[i]>>8;
        warnstate[i*2+1]=machine_Pars.alarm_setted[i];
    }

    header<<tr("报警名称")<<tr("报警屏蔽")<<tr("报警名称")<<tr("报警屏蔽");
    ui->tableWidget->setColumnCount(header.length());
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//设置表头列等宽
    ui->tableWidget->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(45); //设置行高
    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget->setShowGrid(true); //设置不显示格子线
    ui->tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;font-size:18px;border:1px solid white;}"); //设置表头背景色

    ui->tableWidget->setStyleSheet("QScrollBar{background:white;width: 28px;margin: 28px 0 28px 0;}"
                                   "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:0px;}"
                                   "QScrollBar::handle:hover{background:gray;}"
                                   "QScrollBar::handle:vertical{min-height:28px;}"
                                   "QScrollBar::sub-line{background:white;image: url(:/img/top-line.png);height:28px;subcontrol-position: top;subcontrol-origin: margin;}"
                                   "QScrollBar::sub-line:hover{image: url(:/img/top-line-hover.png);}"
                                   "QScrollBar::sub-line:pressed{image: url(:/img/top-line-pressed.png);}"
                                   "QScrollBar::add-line{background:white;image: url(:/img/bottom-line.png);height:28px;subcontrol-position: bottom;subcontrol-origin: margin;}"
                                   "QScrollBar::add-line:hover{image: url(:/img/bottom-line-hover.png);}"
                                   "QScrollBar::add-line:pressed{image: url(:/img/bottom-line-pressed.png);}"
                                   "QTableWidget{selection-background-color:white;"
                                   "color:black;"
                                   "background-color:white;"
                                   "gridline-color: lightgray;"
                                   "selection-color:white;"
                                   "font-size:18px;}");

    header.clear();
    for(int i=0;i<8;i++)
    {
        header<<QString(tr("密度电机%1").arg(i+1));
    }
    for(int i=0;i<8;i++)
    {
        header<<QString(tr("%1号探针").arg(i+1));
    }
    for(int i=0;i<8;i++)
    {
        header<<QString(tr("%1路纱嘴板离线").arg(i+1));
    }
    for(int i=0;i<8;i++)
    {
        header<<QString(tr("%1路功能气阀板").arg(i+1));
    }
    for(int i=0;i<16;i++)
    {
        header<<QString(tr("%1路选针器板离线").arg(i+1));
    }
    header<<tr("电机板1离线")<<tr("电机板2离线")<<tr("电机板3离线")<<tr("BTSR停机")<<tr("伺服报警")<<tr("出哈夫针状态")<<tr("产量编织完成")<<tr("系统异常请联系厂家")
            <<tr("低气压")<<tr("低油压")<<tr("低油位")<<tr("吸风感应")<<tr("KTF1报警")<<tr("储纬器报警")<<tr("KTF2报警")<<tr("LGL报警")
           <<tr("风门电机")<<tr("哈夫同步异常")<<tr("针筒零位编码异常")<<tr("织物未通过")<<tr("2号断针")<<tr("6号断针")<<tr("上下机头导通")<<tr("机头异常报警")
          <<tr("下低油压")<<tr("废纱2-4")<<tr("废纱6-8")<<tr("零位传感器异常");
    rowCount=(header.length()+1)/2;
    ui->tableWidget->setRowCount(rowCount);
    for(int i=0;i<header.length();i++)
    {
        QTableWidgetItem *item=new QTableWidgetItem;
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        item->setText(header[i]);
        item->setFlags(Qt::NoItemFlags);

        QPushButton *btn=new QPushButton;
        btn->setCheckable(true);
        btn->setText("");
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setStyleSheet("QPushButton{background-color:transparent;border:noborder;image: url(:/img/unchecked.png);}QPushButton::checked{image: url(:/img/checked_.png);}");
        if(((warnstate[i/8]>>(i%8))&0x01)==1)
            btn->setChecked(true);
        ui->tableWidget->setItem(i%rowCount,(i/rowCount)*2,item);
        ui->tableWidget->setCellWidget(i%rowCount,(i/rowCount)*2+1,btn);
    }
}

WarningSetting::~WarningSetting()
{
    delete ui;
}

void WarningSetting::on_btn_back_clicked()
{
    this->close();
}

void WarningSetting::on_btn_ok_clicked()
{
    QPushButton *btn;
    for(int i=0;i<header.length();i++)
    {
        btn=(QPushButton*)ui->tableWidget->cellWidget(i%rowCount,(i/rowCount)*2+1);
        if(btn->isChecked())
            warnstate[i/8]|=(1<<(i%8));
        else
            warnstate[i/8]&=~(1<<(i%8));
    }
    //保存参数
    for(int i=0;i<5;i++)
    {
        machine_Pars.alarm_setted[i]=0;
        machine_Pars.alarm_setted[i]=warnstate[i*2+1]|(warnstate[i*2]<<8);
    }
    para_operation.machine_par_save();
    startToCompare=1;
    //主界面传递系统参数
    startToTransmitPara=0;
    startsend=true;
    ui->btn_ok->setEnabled(false);
    ui->label_tips->setText("");
}

void WarningSetting::timeout()
{
    //开始传递参数
    if(startsend)
    {
       //等待主界面参数传递状态
       if(startToTransmitPara>=4)
       {
           startsend=false;
           if(startToTransmitPara==4)
               ui->label_tips->setText(tr("保存成功！"));
           else
               ui->label_tips->setText(tr("保存失败！"));
           ui->btn_ok->setEnabled(true);
       }
    }
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)8)
        {
            temp[0]=255;
            ui->btn_all->click();
        }
        else if(temp[0]==(char)9)
        {
            temp[0]=255;
            ui->btn_ok->click();
        }
        else if(temp[0]==(char)10||temp[0]==(char)28)
        {
            temp[0]=255;
            ui->btn_back->click();
        }
    }
}

void WarningSetting::on_btn_all_clicked()
{
    if(allset)
    {
        ui->btn_all->setText(tr("全\n关"));
        allset=false;
        for(int i=0;i<10;i++)
        {
            warnstate[i]=0;
        }
    }
    else
    {
        ui->btn_all->setText(tr("全\n开"));
        allset=true;
        for(int i=0;i<10;i++)
        {
            warnstate[i]=0xff;
        }
    }

    QPushButton *btn;
    for(int i=0;i<header.length();i++)
    {
        btn=(QPushButton*)ui->tableWidget->cellWidget(i%rowCount,(i/rowCount)*2+1);
        btn->setChecked(allset);
    }
}
