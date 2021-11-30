#include "warninghistory.h"
#include "ui_warninghistory.h"
#include "mymessagebox.h"
#include <QDebug>
#include <QFile>
#include <QTableWidgetItem>

extern QByteArray temp;
WarningHistory::WarningHistory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarningHistory)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    initialForm();
    ui->dateEdit->setStyleSheet("font-size:26px;");
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setLocale(QLocale::Chinese);
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
}

WarningHistory::~WarningHistory()
{
    delete ui;
}

void WarningHistory::timeout()
{
    if(this->isActiveWindow()&&temp.length()>0)
    {
        quint8 key=temp[0];
        temp.clear();
        if(key==6)
        {
            ui->btn_pageup->click();
        }
        else if(key==7)
            ui->btn_pagedown->click();
        else if(key==8)
            ui->deletethisday->click();
        else if(key==9)
            ui->deleteall->click();
        else if(key==10)
            ui->back_Mot_pb->click();
        else if(key==1)
            ui->btn_today->click();
        else if(key==2)
            ui->btn_lastday->click();
        else if(key==3)
            ui->btn_nextday->click();
    }
}

void WarningHistory::initialForm()
{
    maxRowCount=200;
    timer1=new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(timeout()));
    timer1->start(20);
    QStringList headerLabels;
    headerLabels<<"报警时间"<<"报警名称";
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
}

QString WarningHistory::getWarningName(quint16 addr, quint8 bite)
{
    QString str;
    switch (addr) {
    case 0x370:
        str=QString(tr("%1号密度电机报警")).arg(bite);
        break;
    case 0x376:
        str=QString(tr("%1号密度电机报警")).arg(bite+8);
        break;
    case 0x371:
        if(bite==1)
            str=tr("风门电机");
        else if(bite==2)
            str=tr("哈夫同步异常");
        else if(bite==3)
            str=tr("针筒零位编码异常");
        else if(bite==4)
            str=tr("纱嘴通讯异常");
        else if(bite==5)
            str=tr("气阀通讯异常");
        else if(bite==6)
            str=tr("选针器通讯异常");
        else if(bite==7)
            str=tr("电机通讯异常");
        else if(bite==8)
            str=tr("织物未通过");
        break;
    case 0x372:
        str=QString(tr("%1号探针异常")).arg(bite);
        break;
    case 0x373:
        if(bite==1)
            str=tr("2号断针");
        else if(bite==2)
            str=tr("6号断针");
        else if(bite==3)
            str=tr("机头上限位");
        else if(bite==4)
            str=tr("机头下限位");
        else if(bite==5)
            str=tr("BTSR停机");
        else if(bite==6)
            str=tr("上下机头导通");
        else if(bite==7)
            str=tr("风门电机零位异常");
        else if(bite==8)
            str=tr("风门电机过流");
        break;
    case 0x374:
        if(bite==1)
            str=tr("低气压");
        else if(bite==2)
            str=tr("低油压");
        else if(bite==3)
            str=tr("低油位");
        else if(bite==4)
            str=tr("吸风感应");
        else if(bite==5)
            str=tr("KTF1报警");
        else if(bite==6)
            str=tr("储纬器报警");
        else if(bite==7)
            str=tr("KTF3报警");
        else if(bite==8)
            str=tr("LGL报警");
        break;
    case 0x375:
        if(bite==1)
            str=tr("伺服报警");
        else if(bite==2)
            str=tr("机头异常报警");
        else if(bite==3)
            str=tr("出哈夫针状态");
        else if(bite==4)
            str=tr("产量编织完成");
        else if(bite==5)
            str=tr("系统异常请联系厂家");
        else if(bite==6)
            str=tr("机器超速");
        else if(bite==7)
            str=tr("系统无花型");
        else if(bite==8)
            str=tr("低油压2");
        break;
    case 0x377:
        str=QString(tr("第%1路氨纶无纱线")).arg(bite);
        break;
    case 0x378:
        str=QString(tr("%1号探针")).arg(bite);
        break;
    case 0x379:
        if(bite<=3)
            str=QString(tr("电机板%1邮箱溢出")).arg(bite);
        else if(bite==5)
            str="点动超速";
        else if(bite==6)
            str="编织圈数过大";
        else if(bite==7)
            str="1-3-5-7路氨纶报警";
        else if(bite==8)
            str="2-4-6-8路氨纶报警";
        break;
    case 0x37a:
        str=QString(tr("纱嘴板%1邮箱溢出")).arg(bite);
        break;
    case 0x37b:
        str=QString(tr("功能气阀板%1邮箱溢出")).arg(bite);
        break;
    case 0x37c:
        str=QString(tr("纱嘴板%1过流报警")).arg(bite);
        break;
    case 0x37d:
        str=QString(tr("功能气阀板%1过流报警")).arg(bite);
        break;
    case 0x37e:
        str=QString(tr("纱嘴板%1离线报警")).arg(bite);
        break;
    case 0x37f:
        str=QString(tr("功能气阀板%1离线报警")).arg(bite);
        break;
    case 0x380:
        str=QString(tr("电机板%1离线报警")).arg(bite);
        break;
    case 0x381:
        str=QString(tr("选针器板%1离线报警")).arg(bite);
        break;
    case 0x382:
        str=QString(tr("选针器板%1离线报警")).arg(bite+8);
        break;
    case 0x383:
        str=QString(tr("密度电机%1零位电压异常")).arg(bite);
        break;
    case 0x384:
        str=QString(tr("密度电机%1过流报警")).arg(bite);
        break;
    case 0x385:
        str=QString(tr("选针器%1过流报警")).arg(bite);
        break;
    case 0x386:
        str=QString(tr("选针器%1过流报警")).arg(bite+8);
        break;
    case 0x387:
        str=QString(tr("选针器%1断路报警")).arg(bite);
        break;
    case 0x388:
        str=QString(tr("选针器%1断路报警")).arg(bite+8);
        break;
    case 0x389:
        str=QString(tr("纱嘴板%1断路报警")).arg(bite);
        break;
    case 0x38a:
        str=QString(tr("功能气阀板%1断路报警")).arg(bite);
        break;
    case 0x38b:
        str=QString(tr("密度电机%1断路报警")).arg(bite);
        break;
    case 0x38C:
        if(bite==1)
            str=tr("废纱2-4");
        else if(bite==2)
            str=tr("废纱6-8");
        else if(bite==3)
            str=tr("工程参数异常！");
        else if(bite==4)
            str=tr("");
        else if(bite==5)
            str=tr("CAN1发送邮箱溢出");
        else if(bite==6)
            str=tr("CAN1接收邮箱溢出");
        else if(bite==7)
            str=tr("CAN2发送邮箱溢出");
        else if(bite==8)
            str=tr("CAN2接收邮箱溢出");
        break;
    case 0x400:
        if(bite==1)
            str=tr("CAN1异常：EWG");
        else if(bite==2)
            str=tr("CAN1异常：EPV");
        else if(bite==3)
            str=tr("CAN1异常：BOF");
        else if(bite==4)
            str=tr("CAN1异常：LEC");
        else if(bite==5)
            str=tr("CAN1异常：ERR");
        else if(bite==6)
            str=tr("CAN1异常：发送邮箱空");
        else if(bite==7)
            str=tr("CAN1异常：发送标志未使能");
        break;
    case 0x401:
        if(bite==1)
            str=tr("CAN2异常：EWG");
        else if(bite==2)
            str=tr("CAN2异常：EPV");
        else if(bite==3)
            str=tr("CAN2异常：BOF");
        else if(bite==4)
            str=tr("CAN2异常：LEC");
        else if(bite==5)
            str=tr("CAN2异常：ERR");
        else if(bite==6)
            str=tr("CAN2异常：发送邮箱空");
        else if(bite==7)
            str=tr("CAN2异常：发送标志未使能");
        break;
    default:
        break;
    }
    return str;
}

void WarningHistory::readFromFile(QString filename)
{
    if(QFile::exists(filename))
    {
        QFile file(filename);
        if(file.open(QIODevice::ReadWrite))
        {
            QDataStream out(&file);
            int rows=file.size()/7;
            totalPages=(rows+maxRowCount-1)/maxRowCount;
            if(curPage<totalPages-1)
                rows=maxRowCount;
            else
                rows%=maxRowCount;
            out.device()->seek(curPage*maxRowCount*7);
            ui->label->setText(QString("%1/%2").arg(curPage+1).arg(totalPages));
            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(rows);
            qint32 time;quint16 code;
            quint8 bite;
            for(int i=0;i<rows;i++)
            {
                out>>time>>code>>bite;
                QTableWidgetItem *item1=new QTableWidgetItem;
                item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item1->setText(QDateTime::fromTime_t(time).toString("yyyy-MM-dd H:mm:ss"));
                item1->setFlags(Qt::NoItemFlags);

                QTableWidgetItem *item2=new QTableWidgetItem;
                item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item2->setText(getWarningName(code,bite));
                item2->setFlags(Qt::NoItemFlags);

                ui->tableWidget->setItem(i,0,item1);
                ui->tableWidget->setItem(i,1,item2);
            }
            file.close();
        }
    }
    else {
        ui->tableWidget->setRowCount(0);
        ui->label->setText("0/0");
    }

}

void WarningHistory::on_back_Mot_pb_clicked()
{
    close();
}

void WarningHistory::on_dateEdit_dateChanged(const QDate &date)
{
    QString filename="/xrfile/WarningHistory/"+date.toString("yyyyMMdd")+".warn";
    curPage=0;
    totalPages=1;
    readFromFile(filename);
}

void WarningHistory::on_btn_lastday_clicked()
{
    QDate date=ui->dateEdit->date();
    date=date.addDays(-1);
    ui->dateEdit->setDate(date);
}

void WarningHistory::on_btn_nextday_clicked()
{
    QDate date=ui->dateEdit->date();
    date=date.addDays(1);
    ui->dateEdit->setDate(date);
}

//删除当日
void WarningHistory::on_deletethisday_clicked()
{
    QString filename="/xrfile/WarningHistory/"+ui->dateEdit->date().toString("yyyyMMdd")+".warn";
    if(QFile::exists(filename))
    {
        MyMessageBox *box = new MyMessageBox("提示","是否删除当日报警日志？",1);
        if(box->exec()==QDialog::Accepted)
        {
            if(QFile::remove(filename))
            {
                system("sync");
                curPage=0;
                totalPages=1;
                readFromFile(filename);
                MyMessageBox *box = new MyMessageBox(tr("提示"),tr("删除成功！"),0);
                box->exec();
            }
        }
    }
    else
    {
        MyMessageBox *box = new MyMessageBox(tr("提示"),tr("没有当日历史信息！"),0);
        box->exec();
    }
}

//删除所有
void WarningHistory::on_deleteall_clicked()
{
    QDir dir("/xrfile/WarningHistory/");
    if(dir.exists())
    {
        dir.setFilter(QDir::Dirs|QDir::Files);
        QStringList filters;
        filters << "*.warn";
        dir.setNameFilters(filters);
        QFileInfoList list = dir.entryInfoList();
        if(list.length()>0)
        {
            MyMessageBox *box = new MyMessageBox("提示","是否删除所有报警日志？",1);
            if(box->exec()==QDialog::Accepted)
            {
                for(int i=0;i<list.length();i++)
                {
                    if(QFile::exists(list[i].filePath()))
                    {
                        QFile::remove(list[i].filePath());
                    }
                }
                system("sync");
                QString filename="/xrfile/WarningHistory/"+ui->dateEdit->date().toString("yyyyMMdd")+".warn";
                curPage=0;
                totalPages=1;
                readFromFile(filename);
                MyMessageBox *box = new MyMessageBox(tr("提示"),tr("删除成功！"),0);
                box->exec();
            }
        }
        else
        {
            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("无历史报警信息！"),0);
            box->exec();
        }
    }
}

//今日
void WarningHistory::on_btn_today_clicked()
{
    ui->dateEdit->setDate(QDate::currentDate());
}

void WarningHistory::on_btn_pageup_clicked()
{
    if(curPage>0)
    {
        curPage--;
        QString filename="/xrfile/WarningHistory/"+ui->dateEdit->date().toString("yyyyMMdd")+".warn";
        readFromFile(filename);
    }
}

void WarningHistory::on_btn_pagedown_clicked()
{
    if(curPage<totalPages-1)
    {
        curPage++;
        QString filename="/xrfile/WarningHistory/"+ui->dateEdit->date().toString("yyyyMMdd")+".warn";
        readFromFile(filename);
    }
}
