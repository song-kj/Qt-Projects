#include "ui.h"
#include "ui_ui.h"
#include <QTime>
#include <QFile>
#include <QSettings>
#include <QDirIterator>
#include <QCopChannel>
#include "mymessagebox.h"

extern QByteArray temp;
extern QString ui_name;

UI::UI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UI)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    firstEnter=true;
    timer_ui=new QTimer(this);
    timer_ui->start(100);
    connect(timer_ui, SIGNAL(timeout()),this,SLOT(update_slot()));

    QSettings *set=new QSettings("/xrfile/Machine.ini",QSettings::IniFormat);
    QString appname= set->value("UI/AppName").toString();
    delete set;
    ui->vision_pb->setText(QString(tr("当前版本:%1")).arg(appname));
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->horizontalHeader()->resizeSection(0,310);
    ui->tableWidget->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(35); //设置行高
//    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget->setShowGrid(true); //设置不显示格子线
    ui->tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//设置单行选择
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setAlternatingRowColors(true);//设置奇偶行颜色不同
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
                                   "alternate-background-color:#e8f5fd;"
                                   "border:1px solid gray;}");
    watcher = new QFileSystemWatcher(this);
    Path_usb="/media/myusb/";
    QStringList strList;
    strList<<"/media/"<<"/media/myusb/";
    watcher->addPaths(strList);
    connect(watcher,SIGNAL(directoryChanged(QString)),this,SLOT(getUSB(QString)));
    QDir dir;
    if(dir.exists("media/myusb"))
    {
        ui->label_tips->show();
    }
    else
        ui->label_tips->hide();
}

UI::~UI()
{
    delete ui;
}

void UI::getUSB(QString)
{
    QDir dir = QDir(Path_usb);
    if(dir.exists())
    {
        dir.setFilter(QDir::Dirs|QDir::Files);
        QStringList filter;filter<<"RifaUWM_UI*"<<"ShunYiUWM_UI*";
        QStringList files = dir.entryList(filter,QDir::Files | QDir::NoSymLinks); //所有文件
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        if(files.size()>0)
        {
            //加载本地花型文件
            for (int i = 0; i < files.size(); i++)
            {
                QFile file(dir.absoluteFilePath(files[i]));
                QString fileName = QFileInfo(file).fileName();             //
                QTableWidgetItem *fileNameItem = new QTableWidgetItem(fileName);
                int row = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(row);
                ui->tableWidget->setItem(row, 0, fileNameItem);

                qint64 size = QFileInfo(file).size();
                QTableWidgetItem *suffixAndsizeItem = new QTableWidgetItem(tr("%1 KB")
                                                                           .arg(int((size + 1023) / 1024)));           //文件大小
                suffixAndsizeItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->tableWidget->setItem(row, 1, suffixAndsizeItem);
            }
            ui->tableWidget->setCurrentCell(0,0);
        }
    }
}

void UI::myseleep(unsigned int msec)
{
    QTime t=QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime()<t) {
        QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
    }
}

void UI::update_slot()
{
    if(firstEnter)
    {
        firstEnter=false;
        getUSB("");
        ui->label_tips->hide();
    }
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)37)
        {
        }
        if(temp[0]==(char)34)
        {
            temp[0]=255;
            ui->OK_pb->click();
        }
        else if(temp[0]==(char)28)
        {
            temp[0]=255;
            ui->ESC_pb->click();
        }
        if(temp[0]==(char)33)//上
        {
            temp[0]=255;
            int row=ui->tableWidget->currentRow();
            if(row>0&&ui->tableWidget->rowCount()>0)
            {
                row--;
                ui->tableWidget->setCurrentCell(row,0);
            }
        }
        if(temp[0]==(char)35)//下
        {
            temp[0]=255;
            int row=ui->tableWidget->currentRow();
            if(row>-1&&row<ui->tableWidget->rowCount()-1)
            {
                row++;
                ui->tableWidget->setCurrentCell(row,0);
            }
        }
    }
}

void UI::on_OK_pb_clicked()
{
    if(ui->tableWidget->currentIndex().row()>-1)
    {
        QString filename=ui->tableWidget->item(ui->tableWidget->currentIndex().row(),0)->text();
        QFile file(Path_usb+filename);
        if(file.open(QIODevice::ReadOnly))
        {
            QDataStream out(&file);
            out.device()->seek(12);
            quint32 version;
            out>>version;
            file.close();
            quint32 curVersion=1626538035;
            bool rst=false;
            if(version>=curVersion)
            {
                rst=true;
            }
            else
            {
                MyMessageBox *box=new MyMessageBox(tr("提示"),tr("所选程序版本号低于当前版本号，是否继续升级？"),1);
                if(box->exec()==QDialog::Accepted)
                {
                    selectName=filename;
                    KeyBd *bd = new KeyBd;
                    if(bd->exec()==QDialog::Accepted)
                    {
                        if(bd->inputstr=="625000")
                        {
                            rst=true;
                        }
                        else
                        {
                            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("密码错误！"),0);
                            box->exec();
                        }
                    }
                    delete bd;
                }
            }
            if(rst)
            {
                //判断路径是否存在
                QDir dir("/bin/");
                if(dir.exists())
                {
                    //获取所选文件类型过滤器
                    QStringList filters;
                    //    文件过滤
                    filters<<"*";
                    //定义迭代器并设置过滤器
                    QDirIterator dir_iterator("/bin/",
                                              filters,
                                              QDir::Files | QDir::NoSymLinks,
                                              QDirIterator::Subdirectories);
                    //QStringList string_list;
                    while(dir_iterator.hasNext())
                    {
                        dir_iterator.next();
                        QFileInfo file_info = dir_iterator.fileInfo();
                        QString absolute_file_path = file_info.absoluteFilePath();
                        if(absolute_file_path.contains("RifaUWM_UI")&&file_info.fileName()!=ui_name)
                        {
                            QFile::remove(absolute_file_path);
                        }
                    }
                    system("sync");
                }

                QFile fileout;
                if(fileout.open(stdout, QIODevice::ReadWrite))
                {
                    QString str="update";
                    fileout.write(str.toAscii().constData(), 6);
                    fileout.close();
                    QSettings *set=new QSettings(Path_machine,QSettings::IniFormat);
                    set->setValue("UI/AppName",filename);
                    set->setValue("UI/Updated","1");
                    delete set;
                    system("sync");
                    updateUI=true;
                    int count=0;
                    while (updateUI) {
                        myseleep(100);
                        count++;
                        if(count>30)
                            break;
                    }
                    qApp->quit();
                }
//#ifdef Q_OS_LINUX
//                QByteArray value;
//                QDataStream out(&value,QIODevice::ReadWrite);
//                out<<"update";
//                if(QCopChannel::send("Helloworld","update",value))
//                {
//                    QSettings *set=new QSettings(Path_machine,QSettings::IniFormat);
//                    set->setValue("UI/AppName",filename);
//                    set->setValue("UI/Updated","1");
//                    delete set;
//                    system("sync");
//                    updateUI=true;
//                    int count=0;
//                    while (updateUI) {
//                        myseleep(100);
//                        count++;
//                        if(count>30)
//                            break;
//                    }
//                    qApp->quit();
//                }
//#endif
            }
        }
    }
}

void UI::on_ESC_pb_clicked()
{
    timer_ui->stop();/****/
    this->reject();
}
