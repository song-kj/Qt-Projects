#include "upgradefeedback.h"
#include "ui_upgradefeedback.h"
#include <QTableWidgetItem>
UpgradeFeedback::UpgradeFeedback(int type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpgradeFeedback)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setModal(true);
    if(type==1)
    {
        ui->label->setText("电机程序升级");
        proName=tr("%1号电机板");
        maxfeeder=3;
    }
    else if(type==2)
    {
        ui->label->setText(tr("气阀程序升级"));
        proName=tr("%1号气阀板");
        maxfeeder=16;
    }
    else if(type==3)
    {
        ui->label->setText("选针器程序升级");
        proName=tr("%1号选针器");
        maxfeeder=16;
    }
    //设置表头内容
    QStringList header;
    header<<tr("名称")<<tr("状态");
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setClickable(false); //设置表头不可点击（默认点击后进行排序）
    ui->tableWidget->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
        ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//设置表头列等宽
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40); //设置行高
//    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget->setShowGrid(false); //设置不显示格子线
    ui->tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
//    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//设置单行选择
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;font-size:18px;border:1px solid white;}"); //设置表头背景色

    ui->tableWidget->setStyleSheet("QScrollBar{background:transparent; width: 28px;margin: 28px 0 28px 0;}"
                                   "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:0px;}"
                                   "QScrollBar::handle:hover{background:gray;}"
                                   "QScrollBar::handle:vertical{min-height:28px;}"
                                   "QScrollBar::sub-line{background:transparent;image: url(:/img/top-line.png);height:28px;subcontrol-position: top;subcontrol-origin: margin;}"
                                   "QScrollBar::sub-line:hover{image: url(:/img/top-line-hover.png);}"
                                   "QScrollBar::sub-line:pressed{image: url(:/img/top-line-pressed.png);}"
                                   "QScrollBar::add-line{background:transparent;image: url(:/img/bottom-line.png);height:28px;subcontrol-position: bottom;subcontrol-origin: margin;}"
                                   "QScrollBar::add-line:hover{image: url(:/img/bottom-line-hover.png);}"
                                   "QScrollBar::add-line:pressed{image: url(:/img/bottom-line-pressed.png);}"
                                   "color:black;"
                                   "gridline-color: gray;"
                                   "selection-color:white;"
                                   "font-size:20px;"
                                   "background-color:#ffffff;"
                                   "alternate-background-color:#e8f5fd;"
                                   "border:1px solid gray;}"
                                   "QTableWidget{selection-background-color:#4d93fd;"
                                   "background-color:#f0f0f0;"
                                   "color:black;"
                                   "gridline-color: gray;"
                                   "selection-color:white;"
                                   "font-size:22px;}");
    ui->progressBar->setMaximum(maxfeeder);
    ui->progressBar->setValue(0);
    ui->tableWidget->setRowCount(maxfeeder);
    count=0;
    for(int i=0;i<maxfeeder;i++)
    {
        QTableWidgetItem *item1=new QTableWidgetItem;
        item1->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *item2=new QTableWidgetItem;
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,item1);
        ui->tableWidget->setItem(i,1,item2);
    }
}

UpgradeFeedback::~UpgradeFeedback()
{
    delete ui;
}

void UpgradeFeedback::on_pushButton_clicked()
{
    this->close();
}

void UpgradeFeedback::feedback(int index, int b)
{
    if(count<maxfeeder)
    {

        if(count>=0)
        {
            ui->tableWidget->item(count,0)->setText(proName.arg(index));
            if(b==0)
            {
                ui->tableWidget->item(count,1)->setText(tr("成功"));
            }
            else if(b==1)
            {
                ui->tableWidget->item(count,1)->setText(tr("地址错误"));
            }
            else if(b==2)
            {
                ui->tableWidget->item(count,1)->setText(tr("包号错误"));
            }
            else if(b==3)
            {
                ui->tableWidget->item(count,1)->setText(tr("驱动无响应"));
            }
            else if(b==0x04)
            {
                ui->tableWidget->item(count,1)->setText(tr("芯片类型校验错误！"));
            }
            else if(b==0x05)
                ui->tableWidget->item(count,1)->setText(tr("板类检测不匹配！"));
            else if(b==0x06)
                ui->tableWidget->item(count,1)->setText(tr("校验超时！"));
            ui->tableWidget->setCurrentCell(count,0);
        }
        count++;
        ui->progressBar->setValue(count);
    }
}
