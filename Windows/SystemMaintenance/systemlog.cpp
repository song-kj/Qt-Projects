#include "systemlog.h"
#include "ui_systemlog.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSettings>
#include "warninghistory.h"
#include "mymessagebox.h"

extern QByteArray temp;
SystemLog::SystemLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemLog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    pTimer=new QTimer(this);
    connect(pTimer,SIGNAL(timeout()),this,SLOT(update_slot()));
    initialForm();
}

SystemLog::~SystemLog()
{
    delete ui;
}

void SystemLog::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>0)
    {
        if(temp[0]==(char)10)
        {
            temp[0]=255;
            ui->back_Mot_pb->click();
        }
        else if(temp[0]==(char)9)
        {
            temp[0]=255;
            ui->back_Mot_pb_2->click();
        }
    }
}

void SystemLog::initialForm()
{
    ui->tableWidget->setColumnCount(1);
//    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//设置表头列等宽
    ui->tableWidget->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(45); //设置行高
    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget->setShowGrid(true); //设置不显示格子线
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//设置单行选择
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{color:white;background-color:#34495e;"
                                                       "font-size:22px;border:0px solid white;"
                                                       "border-width:0px 0px 0px 0px;}"); //设置表头背景色

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
                                   "QTableWidget{selection-background-color:#4d93fd;"
                                   "color:black;"
                                   "gridline-color: gray;"
                                   "selection-color:white;"
                                   "font-size:20px;"
                                   "background-color:#ffffff;"
                                   "alternate-background-color:#a8ddfd;"
                                   "border:1px solid gray;}");

    readFromFile("/xrfile/system.log");
}

void SystemLog::readFromFile(QString filename)
{
    if(QFile::exists(filename))
    {
        QStringList list;
        QSettings *set=new QSettings(filename,QSettings::IniFormat);
        int curIndex=set->value("Log/Index").toInt();
        for(int i=0;i<500;i++)
        {
            curIndex++;
            if(curIndex>500)
                curIndex=1;
            QString msg=set->value(QString("Log/Msg_%1").arg(curIndex)).toString();
            if(msg!="")
                list.append(msg);
        }
        delete set;

        ui->tableWidget->setRowCount(list.length());
        for(int i=0;i<list.length();i++)
        {
            QTableWidgetItem *item=new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignVCenter);
            item->setText(list[i]);
            ui->tableWidget->setItem(i,0,item);
        }
    }
}

void SystemLog::on_back_Mot_pb_clicked()
{
    this->close();
}

void SystemLog::on_back_Mot_pb_2_clicked()
{
    WarningHistory *warnhistory=new WarningHistory;
    warnhistory->exec();
}

void SystemLog::on_back_copy_clicked()
{
    QString pathusb=Path_myusb;
    QDir dir;
    if(dir.exists(pathusb))
    {
        QString tofilename=pathusb+"system.log";
        if(QFile::exists(tofilename))
            QFile::remove(tofilename);
        if(QFile::copy("/xrfile/system.log",tofilename))
        {
            system("sync");
            MyMessageBox *box=new MyMessageBox(tr("提示"),tr("拷出成功！"),0);
            box->exec();
        }
        else
        {
            MyMessageBox *box=new MyMessageBox(tr("提示"),tr("拷出失败！"),0);
            box->exec();
        }
    }
    else
    {
        MyMessageBox *box=new MyMessageBox(tr("提示"),tr("请插入U盘！"),0);
        box->exec();
    }
}
