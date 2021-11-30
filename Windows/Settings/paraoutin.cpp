#include "paraoutin.h"
#include "ui_paraoutin.h"
#include <QDir>
#include <QTime>

extern QByteArray temp;
ParaOutIn::ParaOutIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParaOutIn)
{
    ui->setupUi(this);
     setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    initialForm();
    timer_paraOutIn=new QTimer(this);
    connect(timer_paraOutIn, SIGNAL(timeout()), this, SLOT(update_slot()));
    timer_paraOutIn->start(100);
}

ParaOutIn::~ParaOutIn()
{
    delete ui;
}

void ParaOutIn::showUSB()
{
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        dir.setFilter(QDir::Dirs|QDir::Files);
        QStringList filters;
        filters << "*.er";
        dir.setNameFilters(filters);
        QFileInfoList list = dir.entryInfoList();
        ui->usb_twd->clearContents();
        ui->usb_twd->setRowCount(0);
        if(list.size()>0)
        {
            ui->usb_twd->setRowCount(list.size());
            for(int i = 0;i < list.size();i++)
            {
                QFileInfo file_info = list.at(i);
                ui->usb_twd->setItem(i,0,new QTableWidgetItem( file_info.fileName()));

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
                ui->usb_twd->setItem(i,1,suffixAndsizeItem);
            }
            ui->usb_twd->setCurrentCell(0,0);
        }
        if(isfirstEnter)
            ui->label_tips->setText(tr(""));
    }
}

//拷出
void ParaOutIn::on_Out_pb_clicked()//to usb
{
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        QString filepath_USB=tr(Path_myusb).append("Parameter.er");
        QString filepath_DISK=Path_parameterfile;
        if(QFile::exists(filepath_USB))
        {
            QFile::remove(filepath_USB);
        }
        if(QFile::copy(filepath_DISK,filepath_USB))
        {
            system("sync");
            showUSB();
            ui->label_tips->setText(tr("拷出成功！"));
        }
    }
    else
    {
        ui->label_tips->setText(tr("请先插入U盘！"));
    }
}

//拷入
void ParaOutIn::on_In_pb_clicked()
{
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        int row=ui->usb_twd->currentRow();
        if(row>-1)
        {
            QString filepath_USB=tr(Path_myusb).append(ui->usb_twd->item(row,0)->text());
            QString filepath_DISK=Path_parameterfile;

            if(QFile::exists(filepath_DISK))
            {
                QFile::remove(filepath_DISK);
            }
            if(QFile::copy(filepath_USB,filepath_DISK))
            {
                system("sync");
                showUSB();
                ui->label_tips->setText(tr("拷入成功！"));
            }
        }
    }
    else
    {
        ui->label_tips->setText(tr("请先插入U盘！"));
    }
}


void ParaOutIn::update_slot()
{
    if(isfirstEnter)
    {
        showUSB();
        isfirstEnter=false;
    }
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)2)
        {
            temp[0]=255;
            ui->Out_pb->click();
        }
        if(temp[0]==(char)3)
        {
            temp[0]=255;
            ui->In_pb->click();
        }
        if(temp[0]==(char)27)
        {
            temp[0]=255;
            ui->delede_pb->click();
        }
        if(temp[0]==(char)28)
        {
            temp[0]=255;
            ui->back_pb->click();
        }
        if(temp[0]==(char)33)//上
        {
            temp[0]=255;
            QDir dir(Path_myusb);
            if(dir.exists())
            {
                if(ui->usb_twd->rowCount()>0)
                {
                    int row=ui->usb_twd->currentRow();
                    if(row>0)
                    {
                        row--;
                        ui->usb_twd->setCurrentCell(row,0);
                    }
                }
            }
        }
        if(temp[0]==(char)35)//下
        {
            temp[0]=255;
            QDir dir(Path_myusb);
            if(dir.exists())
            {
                if(ui->usb_twd->rowCount()>0)
                {
                    int row=ui->usb_twd->currentRow();
                    if(row<ui->usb_twd->rowCount()-1)
                    {
                        row++;
                        ui->usb_twd->setCurrentCell(row,0);
                    }
                }
            }
        }
    }
}

//返回
void ParaOutIn::on_back_pb_clicked()
{
    timer_paraOutIn->stop();
    reject();
}

//删除
void ParaOutIn::on_delede_pb_clicked()
{
    if(ui->usb_twd->currentRow()>-1)
    {
        QString filepath_USB=tr(Path_myusb).append(ui->usb_twd->item(ui->usb_twd->currentRow(),0)->text());
        if(QFile::exists(filepath_USB))
        {
            QFile::remove(filepath_USB);
            system("sync");
            showUSB();
            ui->label_tips->setText(tr("删除成功！"));
        }
    }
}

void ParaOutIn::initialForm()
{
    isfirstEnter=true;
    QStringList header;
    header<<tr("名称")<<tr("大小");
    ui->usb_twd->setColumnCount(2);
    ui->usb_twd->horizontalHeader()->resizeSection(0,300);
    ui->usb_twd->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->usb_twd->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->usb_twd->verticalHeader()->setDefaultSectionSize(35); //设置行高
//    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->usb_twd->setShowGrid(true); //设置不显示格子线
    ui->usb_twd->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->usb_twd->horizontalHeader()->setVisible(true);
    ui->usb_twd->setHorizontalHeaderLabels(header);
    ui->usb_twd->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->usb_twd->setSelectionMode(QAbstractItemView::SingleSelection);//设置单行选择
    ui->usb_twd->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->usb_twd->setAlternatingRowColors(true);//设置奇偶行颜色不同
    ui->usb_twd->horizontalHeader()->setStyleSheet("QHeaderView::section{color:black;background-color:#ffffff;"
                                                       "font-size:22px;border:1px solid gray;"
                                                       "border-width:0px 1px 1px 1px;}"); //设置表头背景色

    ui->usb_twd->setStyleSheet("QScrollBar{background:transparent; width: 28px;margin: 28px 0 28px 0;}"
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
        ui->label_tips->setText(tr("U盘文件加载中，请稍后！"));
    }
    else
    {
        ui->label_tips->setText(tr(""));
    }
}
