#include "controlui.h"
#include "ui_controlui.h"
#include <QTextStream>
#include <QCryptographicHash>
#include "xrfile.h"
#include <QDebug>
#include "upgradefeedback.h"

extern QByteArray temp;
extern QString global_MainControl_Version;//主控程序版本

controlUI::controlUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::controlUI)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉qt头
    this->setAttribute(Qt::WA_DeleteOnClose);
}

controlUI::~controlUI()
{
    delete ui;
}

//设置升级升级程序类型
void controlUI::setProgramType(ProgramType type)
{
    proType=type;
    maxfeeder=1;
    ui->vision_pb->hide();
    filetransfercomplete=false;
    if(proType==MainCtl)
    {
        filters<<"MainCTL*"<<"EFMainCTL*";
        proTypeNum=0;
        feeder=0xff;
        ui->groupBox->setTitle(tr("主控程序升级"));
        //主控显示当前版本
//        ui->vision_pb->show();
        ui->info_pb->setText(global_MainControl_Version);
        //主控升级隐藏路数
        ui->pushbutton_add->hide();
        ui->pushbutton_sub->hide();
        ui->label_feeder->hide();
    }
    else if(proType==AirValve)
    {
        ui->groupBox->setTitle(tr("气阀程序升级"));
        filters<<"VlvDRV*"<<"EFVlvDRV*";
        proTypeNum=2;
        feeder=1;
        maxfeeder=18;
    }
    else if(proType==StepMotor)
    {
        ui->groupBox->setTitle(tr("电机程序升级"));
        filters<<"SmtrDRV*"<<"EFSmtrDRV*";
        proTypeNum=1;
        feeder=1;
        maxfeeder=3;
    }
    else if(proType==Actuator)
    {
        ui->groupBox->setTitle(tr("选针器程序升级"));
        filters<<"ActaDRV*"<<"EFActaDRV*";
        proTypeNum=3;
        feeder=1;
        maxfeeder=16;
    }
    else if(proType==PYF)
    {
        ui->groupBox->setTitle(tr("裸氨功能板程序升级"));
        //裸氨升级隐藏路数
        ui->pushbutton_add->hide();
        ui->pushbutton_sub->hide();
        ui->label_feeder->hide();
        filters<<"PyfDRV*"<<"EFPyfDRV*";
        feeder=0xff;
        proTypeNum=4;
    }
    initialForm();
}

void controlUI::getUSB(QString str)
{
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        showinfo_USB();
    }
    else
    {
        ui->USB_twd->setRowCount(0);
    }
}

void controlUI::initialForm()
{
    exchangeMark=0;
    firstEnter=true;
    isdowning=false;
    ui->label_feeder->setText(QString::number(feeder));
    ui->diskMesg_pb->setEnabled(false);
    ui->usbMesg_pb->setEnabled(false);
    ui->diskMesg_pb->setChecked(true);
    ui->USB_twd->setColumnCount(2);
    ui->USB_twd->horizontalHeader()->resizeSection(0,270);
    ui->USB_twd->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->USB_twd->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->USB_twd->verticalHeader()->setDefaultSectionSize(35); //设置行高
//    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->USB_twd->setShowGrid(true); //设置不显示格子线
    ui->USB_twd->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->USB_twd->horizontalHeader()->setVisible(false);
    ui->USB_twd->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->USB_twd->setSelectionMode(QAbstractItemView::SingleSelection);//设置单行选择
    ui->USB_twd->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->USB_twd->setAlternatingRowColors(true);//设置奇偶行颜色不同
    ui->USB_twd->horizontalHeader()->setStyleSheet("QHeaderView::section{color:white;background-color:#34495e;"
                                                       "font-size:22px;border:0px solid white;"
                                                       "border-width:0px 0px 0px 0px;}"); //设置表头背景色

    ui->USB_twd->setStyleSheet("QScrollBar{background:transparent; width: 28px;margin: 28px 0 28px 0;}"
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

    ui->DISK_twd->setColumnCount(2);
    ui->DISK_twd->horizontalHeader()->resizeSection(0,270);
    ui->DISK_twd->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->DISK_twd->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->DISK_twd->verticalHeader()->setDefaultSectionSize(35); //设置行高
//    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->DISK_twd->setShowGrid(true); //设置不显示格子线
    ui->DISK_twd->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->DISK_twd->horizontalHeader()->setVisible(false);
    ui->DISK_twd->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->DISK_twd->setSelectionMode(QAbstractItemView::SingleSelection);//设置单行选择
    ui->DISK_twd->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->DISK_twd->setAlternatingRowColors(true);//设置奇偶行颜色不同
    ui->DISK_twd->horizontalHeader()->setStyleSheet("QHeaderView::section{color:white;background-color:#34495e;"
                                                       "font-size:22px;border:0px solid white;"
                                                       "border-width:0px 0px 0px 0px;}"); //设置表头背景色

    ui->DISK_twd->setStyleSheet("QScrollBar{background:transparent; width: 28px;margin: 28px 0 28px 0;}"
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

    ui->progressBar->hide();//未下载之前隐藏进度条
    /**********初始化定时器**********/
    timer_control=new QTimer(this);
    timer_control->start(20);
    connect(timer_control,SIGNAL(timeout()),this,SLOT(update_slot()));
    /******设置点击Item时的链接槽，用于标识本地还是usb中的文件被点击，方便后续处理******/
    connect(ui->DISK_twd,SIGNAL(itemClicked (QTableWidgetItem* )),this,SLOT(GetQTableWidgetObject(QTableWidgetItem*)));
    connect(ui->USB_twd,SIGNAL(itemClicked ( QTableWidgetItem*  )),this,SLOT(GetQTableWidgetObject(QTableWidgetItem*)));

    /**********************/
    table=new QTableWidget();
    table=ui->DISK_twd;

    /**************隐藏列表头**************/
    ui->DISK_twd->verticalHeader()->setVisible(false);
    ui->DISK_twd->horizontalHeader()->setVisible(false);
    ui->USB_twd->verticalHeader()->setVisible(false);
    ui->USB_twd->horizontalHeader()->setVisible(false);

    /**********显示文件信息************/
    showinfo_DISK();
    //   showinfo_USB();
    watcher = new QFileSystemWatcher(this);
    QStringList strList;
    strList<<"/media"<<"/media/myusb";
    watcher->addPaths(strList);
    connect(watcher,SIGNAL(directoryChanged(QString)),this,SLOT(getUSB(QString)));
    QDir dir;
    if(dir.exists("media/myusb"))
    {
        ui->label_tips->setText("U盘文件加载中，请稍后！");
        ui->label_tips->show();
    }
    else
        ui->label_tips->hide();
}

/**********显示USB中文件信息*************/
void controlUI::showinfo_USB()
{
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        dir.setFilter(QDir::Dirs|QDir::Files);
        QStringList files = dir.entryList(filters,QDir::Files | QDir::NoSymLinks); //所有文件
        ui->USB_twd->clearContents();
        ui->USB_twd->setRowCount(0);
        if(files.size()>0)
        {
            //加载本地花型文件
            for (int i = 0; i < files.size(); i++)
            {
                QFile file(dir.absoluteFilePath(files[i]));
                QString fileName = QFileInfo(file).fileName();             //
                QTableWidgetItem *fileNameItem = new QTableWidgetItem(fileName);
                int row = ui->USB_twd->rowCount();
                ui->USB_twd->insertRow(row);
                ui->USB_twd->setItem(row, 0, fileNameItem);

                qint64 size = QFileInfo(file).size();
                QTableWidgetItem *suffixAndsizeItem = new QTableWidgetItem(tr("%1 KB")
                                                                           .arg(int((size + 1023) / 1024)));           //文件大小
                suffixAndsizeItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->USB_twd->setItem(row, 1, suffixAndsizeItem);
            }
            ui->USB_twd->setCurrentCell(0,0);
        }
    }
//    QString name;
//    QDir dir(USB);
//    if(dir.exists())
//    {
//        /*************过滤出.bin文件****************/
//        dir.setFilter(QDir::Dirs|QDir::Files);
//        QStringList filters;
//        filters << "MainCTL*";
//        dir.setNameFilters(filters);
//        /*********提取该目录下的所有文件************/
//        QFileInfoList list = dir.entryInfoList();

//         List_USB=list.size();
//         SUM_USB=0;//当前处于第0行状态
//         if(list.size()>0)
//         {

//            ui->USB_twd->setRowCount(list.size());
//            ui->USB_twd->setColumnCount(1);
//            ui->USB_twd->setToolTip(tr("1"));
//            ui->USB_twd->setEditTriggers(QAbstractItemView::NoEditTriggers);
//            for(int i = 0;i < list.size();i++)
//            {
//                QFileInfo file_info = list.at(i);
//                name = file_info.fileName();
//                ui->USB_twd->setItem(i,0,new QTableWidgetItem(name));
//                /******如果本地存在文件，则设置usb中第一行为白色，未选中状态，如果本地没有文件，则设置usb第一行被选择***/
//                if(List_disk>0)
//                {
//                    ui->USB_twd->item(i,0)->setBackgroundColor(QColor(255,255,255));
//                }
//                else
//                {
//                    ui->USB_twd->item(0,0)->setBackgroundColor(QColor(0,0,255));
//                    QColor color("white");
//                    ui->USB_twd->item(0,0)->setTextColor(color);
//                }
//            }
//           filename_usb=ui->USB_twd->item(0,0)->text();
//         }
//    }
}

//显示本地文件
void controlUI::showinfo_DISK()
{
    QDir dir = QDir(Path_loacl);
    if(dir.exists())
    {
        dir.setFilter(QDir::Dirs|QDir::Files);
        QStringList files = dir.entryList(filters,QDir::Files | QDir::NoSymLinks); //所有文件
        ui->DISK_twd->clearContents();
        ui->DISK_twd->setRowCount(0);
        if(files.size()>0)
        {
            //加载本地花型文件
            for (int i = 0; i < files.size(); i++)
            {
                QFile file(dir.absoluteFilePath(files[i]));
                QString fileName = QFileInfo(file).fileName();             //
                QTableWidgetItem *fileNameItem = new QTableWidgetItem(fileName);
                int row = ui->DISK_twd->rowCount();
                ui->DISK_twd->insertRow(row);
                ui->DISK_twd->setItem(row, 0, fileNameItem);

                qint64 size = QFileInfo(file).size();
                QTableWidgetItem *suffixAndsizeItem = new QTableWidgetItem(tr("%1 KB")
                                                                           .arg(int((size + 1023) / 1024)));           //文件大小
                suffixAndsizeItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->DISK_twd->setItem(row, 1, suffixAndsizeItem);
            }
            ui->DISK_twd->setCurrentCell(0,0);
        }
    }

}

/*************考入USB***************/
void controlUI::on_copyToUSB_pb_clicked()
{
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        if(ui->DISK_twd->currentIndex().row()>-1)
        {
            QString filename_usb=Path_myusb+ui->DISK_twd->item(ui->DISK_twd->currentIndex().row(),0)->text();
            QString filename_disk=Path_loacl+ui->DISK_twd->item(ui->DISK_twd->currentIndex().row(),0)->text();
            if(QFile::exists(filename_usb))
            {
                QFile::remove(filename_usb);
                QFile::copy(filename_disk,filename_usb);
                MyMessageBox *box = new MyMessageBox("提示","覆盖成功！",0);
                box->exec();
            }
            else
            {
                QFile::copy(filename_disk,filename_usb);
            }
            system("sync");
        }
    }
}

/*********标识本地或usb的某一行被选中**********/
void controlUI::GetQTableWidgetObject(QTableWidgetItem* itemm)
{
    table=itemm->tableWidget();
    if(table->toolTip().toInt()==1)
    {
         exchangeMark=0;
         ui->diskMesg_pb->setChecked(true);
         ui->usbMesg_pb->setChecked(false);
         ui->programUpdate_pb->setEnabled(true);
    }
    else if(table->toolTip().toInt()==2)
    {
         exchangeMark=1;
         ui->usbMesg_pb->setChecked(true);
         ui->diskMesg_pb->setChecked(false);
         ui->programUpdate_pb->setEnabled(false);
    }
}

/******考入本地*******/
void controlUI::on_copyToDISK_pb_clicked()
{
    QDir dir(Path_loacl);
    if(dir.exists())
    {
        if(ui->USB_twd->currentIndex().row()>-1)
        {
            QString filename_usb=Path_myusb+ui->USB_twd->item(ui->USB_twd->currentIndex().row(),0)->text();
            QString filename_disk=Path_loacl+ui->USB_twd->item(ui->USB_twd->currentIndex().row(),0)->text();
            if(QFile::exists(filename_disk))
            {
                QFile::remove(filename_disk);
                QFile::copy(filename_usb,filename_disk);
                showinfo_DISK();
                MyMessageBox *box = new MyMessageBox("提示","覆盖成功！",0);
                box->exec();
            }
            else
            {
                QFile::copy(filename_usb,filename_disk);
                showinfo_DISK();
            }
            system("sync");
        }
    }
}

/*******升级********/
void controlUI::on_programUpdate_pb_clicked()
{
    if(exchangeMark==0)//只能升级本地中的文件
    {
        if(ui->DISK_twd->currentIndex().row()>-1)
        {
            ui->label_tips->hide();
            filename=ui->DISK_twd->item(ui->DISK_twd->currentIndex().row(),0)->text();
            QString bfname=QString("/home/root/%1").arg(filename);
            if(QFileInfo(bfname).size()==0)
            {
                ui->label_tips->show();
                ui->label_tips->setText(QString::fromUtf8("无效的升级文件！"));
                return;
            }
            transname="/home/root/updatefile.bin";
            if(QFile::exists(transname))
                QFile::remove(transname);
            //文件解密
            XRFILE *xrfile=new XRFILE(bfname.toAscii().data(),transname.toAscii().data());
            //返回值为0表示文件未加密 1表示文件加密
            if(xrfile->decryptFile()==0)
            {
                transname=QString("home/root/%1").arg(filename);
            }
            else if(xrfile->decryptFile()==1)
            {
                transname="/home/root/updatefile.bin";
                QString md51,md52;
                //文件加密的需要验证md5码
                QFile file(bfname);
                if(file.open(QIODevice::ReadWrite))
                {
                    file.seek(16);
                    QByteArray ba=file.read(16);
                    md51=ba.toHex();
                    file.close();

                    QFile file1(transname);
                    if(file1.open(QIODevice::ReadWrite))
                    {
                        ba=QCryptographicHash::hash(file1.readAll(),QCryptographicHash::Md5);
                        md52=ba.toHex();
                    }
                }
                if(md51!=md52)
                {
                    MyMessageBox *box = new MyMessageBox("提示","升级文件错误！",0);
                    box->exec();
                    return;
                }
                quint8 MCU_type=xrfile->MCU_type;
                qDebug()<<"MCU: "<<MCU_type;
                Ram.SendData[0]=0x80;
                Ram.SendData[1]=0x40;
                Ram.SendData[2]=proTypeNum;//升级程序类型
                Ram.SendData[3]=MCU_type;
                Ram.SendData[4]=0x00;
                Ram.SendData[5]=0x00;
                Ram.SendData[6]=0x00;
                Ram.SendData[7]=0x00;
                Ram.SendData[8]=0x00;
                Ram.SendData[9]=0x00;
                Ram.UI_to_RTC();
            }

            QFile file(transname);
            if(file.open(QIODevice::ReadWrite))
            {
                totalBytes=file.size();//文件字节数
                totalPacks=(totalBytes+2559)/2560;

                /********申请传递文件*********/
                Ram.Send(0xAA,0x300);
                Ram.SendData[0]=0x80;
                Ram.SendData[1]=0x50;
                Ram.SendData[2]=proTypeNum;//升级程序类型
                Ram.SendData[3]=totalPacks%256;
                Ram.SendData[4]=totalPacks/256;
                Ram.SendData[5]=(totalBytes%256)%256;
                Ram.SendData[6]=(totalBytes/256)%256;
                Ram.SendData[7]=(totalBytes%256)/256;
                Ram.SendData[8]=(totalBytes/256)/256;
                Ram.SendData[9]=feeder;
                if(Ram.UI_to_RTC())
                {
                    ui->programUpdate_pb->setEnabled(false);
                    ui->progressBar->setValue(0);
                    ui->progressBar->setRange(0,totalPacks);//设置进度条范围
                    ui->progressBar->show();
                    isdowning=true;
                    timecount=0;
                }
            }
        }
    }
}

/*****关闭窗口********/
void controlUI::on_pushButton_4_clicked()
{
    timer_control->stop();
    this->reject();
}

/*****删除********/
void controlUI::on_delete_pb_clicked()
{
    if(exchangeMark==1)
    {
        if(ui->USB_twd->currentIndex().row()>-1)
        {
            QString filename=Path_myusb+ui->USB_twd->item(ui->USB_twd->currentIndex().row(),0)->text();
            if(QFile::exists(filename))
            {
                QFile::remove(filename);
                system("sync");
            }
        }
    }
    else if(exchangeMark==0)
    {
        if(ui->DISK_twd->currentIndex().row()>-1)
        {
            QString filename=Path_loacl+ui->DISK_twd->item(ui->DISK_twd->currentIndex().row(),0)->text();
            if(QFile::exists(filename))
            {
                QFile::remove(filename);
                system("sync");
                showinfo_DISK();
            }
        }
    }
}

void controlUI::update_slot()
{
    if(firstEnter)
    {
        firstEnter=false;
        getUSB("");
        ui->label_tips->hide();
    }
    if(isdowning)
    {
        timecount++;
        if(timecount>100)
        {
            ui->progressBar->hide();
            ui->label_tips->show();
            ui->label_tips->setText(tr("升级失败,请重启！"));
            ui->programUpdate_pb->setEnabled(true);
            isdowning=false;
            return;
        }
        if(Ram.Receive(0x300)==0x55)
        {
            if(Ram.Receive(0x200)==0x80)
            {
                if(Ram.Receive(0x201)==0x52)
                {
                    if(Ram.Receive(0x202)==proTypeNum)
                    {
                       Ram.Send(0xAA,0x300);
                       timecount=0;
                       int baoshu=Ram.Receive(0x203)+Ram.Receive(0x204)*256;//主控请求的包次，从第0包开始计算。。。
                       int lastpack=2560;
                       if(baoshu==totalPacks-1)//判断是否是最后一包，如果是，则需要计算最后一包的字节数
                       {
                          lastpack= totalBytes-(totalPacks-1)*2560;
                       }
                       QFile file(transname);
                       if(file.open(QIODevice::ReadWrite))
                       {
                           QDataStream out(&file);
                           out.device()->seek(baoshu*2560);//定位到从某一位置提取字节
                           quint8 s=0;
                           for(int j=0;j<lastpack;j++)
                           {
                               out>>s;
                               Ram.Send(s,0x500+j);//发送字节给主控
                           }
                           ui->progressBar->setValue(baoshu);
                           file.close();
                           /******发送完一包后通知主控*****/
                           Ram.SendData[0]=0x80;
                           Ram.SendData[1]=0x51;
                           Ram.SendData[2]=proTypeNum;
                           Ram.SendData[3]=baoshu%256;//总包数L
                           Ram.SendData[4]=baoshu/256;//总包数H
                           Ram.SendData[5]=lastpack%256;
                           Ram.SendData[6]=lastpack/256;
                           Ram.SendData[7]=0x00;
                           Ram.SendData[8]=0x00;
                           Ram.SendData[9]=0x00;
                           Ram.UI_to_RTC();
                       }
                       else
                       {
                           ui->progressBar->hide();
                           ui->label_tips->show();
                           ui->label_tips->setText(tr("升级失败,请重启！"));
                           ui->programUpdate_pb->setEnabled(true);
                           isdowning=false;
                       }
                    }
                }
            }
        }
        else if(Ram.Receive(0x300)==0xcc)//升级完成
        {
             Ram.Send(0xAA,0x300);
             ui->progressBar->hide();
             isdowning=false;
             if(proType==MainCtl)
             {
                 ui->label_tips->show();
                 ui->label_tips->setText(tr("升级完成,请重启！"));
                 para_operation.saveSystemLog(tr("主控程序(%1)升级完成！").arg(filename),QDateTime::currentDateTime());
                 ui->programUpdate_pb->setEnabled(true);
                 //******把升级文件存入文件，用于进入该窗口时的读取
                 QFile file(Vision_ID);
                 if(file.open(QIODevice::ReadWrite))
                 {
                     file.resize(0);
                     QTextStream out(&file);
                     out<<filename;
                     file.close();
                     system("sync");
                 }
             }
             else
             {
                 timecount=0;
                 filetransfercomplete=true;
                 if(feeder==0xff)
                 {
                     UpgradeFeedback *f=new UpgradeFeedback(proTypeNum);
                     connect(this,SIGNAL(sendfeedback(int,int)),f,SLOT(feedback(int,int)));
                     f->show();
                 }
             }
        }
        else if(Ram.Receive(0x300)==0xEE)
        {
            Ram.Send(0xAA,0x300);
            ui->progressBar->hide();
            ui->label_tips->show();
            ui->label_tips->setText(tr("升级失败！"));
            ui->programUpdate_pb->setEnabled(true);
            isdowning=false;
        }
    }

    if(filetransfercomplete)
    {
        timecount++;
        if(timecount>100)
        {
            ui->label_tips->show();
            ui->label_tips->setText(tr("升级超时！"));
            ui->programUpdate_pb->setEnabled(true);
            filetransfercomplete=false;
            return;
        }
        if(Ram.Receive(0x300)==0x55)
        {
            if(Ram.Receive(0x200)==0x80)
            {
                if(Ram.Receive(0x201)==0x53)//升级路数反馈
                {
                    if(Ram.Receive(0x202)==proTypeNum)
                    {
                        timecount=0;
                        //升级完成
                        if(Ram.Receive(0x203)==0xCC)
                        {
                            Ram.Send(0xAA,0x300);
                            timecount=0;
                            //读取完成路数
                            quint8 Rd=Ram.Receive(0x204);
                            ui->progressBar->hide();
                            ui->label_tips->show();
                            if(proType==PYF)
                            {
                                ui->label_tips->setText(tr("升级完成！"));
                                ui->programUpdate_pb->setEnabled(true);
                                filetransfercomplete=false;
                            }
                            else
                            {
                                if(Rd<0xff)
                                {
                                    ui->label_tips->setText(tr("%1路升级完成！").arg(Rd));
                                }
                                else if(Rd==0xff)
                                {
                                    filetransfercomplete=false;
                                    ui->label_tips->setText(tr("全部升级完成,请重启！"));
                                    ui->programUpdate_pb->setEnabled(true);
                                }
                                if(feeder==0xff)
                                {
                                    if(Rd==0xff)
                                        Rd=maxfeeder;
                                    emit sendfeedback(Rd,0);
                                }
                                else
                                {
                                    filetransfercomplete=false;
                                    ui->programUpdate_pb->setEnabled(true);
                                }
                            }
                        }
                        else if(Ram.Receive(0x203)==0xEE)//升级错误
                        {
                            Ram.Send(0xAA,0x300);
                            timecount=0;
                            quint8 RD=Ram.Receive(0x204);
                            quint8 type=Ram.Receive(0x205);
                            ui->progressBar->hide();
                            ui->label_tips->show();
                            if(proType==PYF)
                            {
                                if(type==0x01)
                                {
                                    ui->label_tips->setText(tr("升级地址错误!"));
                                }
                                else if(type==0x02)
                                {
                                    ui->label_tips->setText(tr("升级包号错误！"));
                                }
                                else if(type==0x03)
                                {
                                    ui->label_tips->setText(tr("驱动无响应！"));
                                }
                                else if(type==0x04)
                                {
                                    ui->label_tips->setText(tr("%1号板芯片类型校验错误！").arg(RD));
                                }
                                else if(type==0x05)
                                {
                                    ui->label_tips->setText(tr("%1号板板类检测不匹配！").arg(RD));
                                }
                                else if(type==0x06)
                                {
                                    ui->label_tips->setText(tr("%1号板校验超时！").arg(RD));
                                }
                                ui->programUpdate_pb->setEnabled(true);
                                filetransfercomplete=false;
                            }
                            else
                            {
                                if(RD==0xff)
                                    RD=maxfeeder;
                                if(type==0x01)
                                {
                                    ui->label_tips->setText(QString::fromUtf8("%1号板升级地址错误!").arg(RD));
                                    if(feeder==0xff)
                                    {
                                        emit sendfeedback(RD,1);
                                    }
                                }
                                else if(type==0x02)
                                {
                                    ui->label_tips->setText(QString::fromUtf8("%1号板升级包号错误！").arg(RD));
                                    if(feeder==0xff)
                                    {
                                        emit sendfeedback(RD,2);
                                    }
                                }
                                else if(type==0x03)
                                {
                                    ui->label_tips->setText(QString::fromUtf8("%1号板驱动无响应！").arg(RD));
                                    if(feeder==0xff)
                                    {
                                        emit sendfeedback(RD,3);
                                    }
                                }
                                else if(type==0x04)
                                {
                                    ui->label_tips->setText(tr("%1号板芯片类型校验错误！").arg(RD));
                                    if(feeder==0xff)
                                    {
                                        emit sendfeedback(RD,4);
                                    }
                                }
                                else if(type==0x05)
                                {
                                    ui->label_tips->setText(tr("%1号板板类检测不匹配！").arg(RD));
                                    if(feeder==0xff)
                                    {
                                        emit sendfeedback(RD,5);
                                    }
                                }
                                else if(type==0x06)
                                {
                                    ui->label_tips->setText(tr("%1号板校验超时！").arg(RD));
                                    if(feeder==0xff)
                                    {
                                        emit sendfeedback(RD,6);
                                    }
                                }
                                if(feeder==0xff)
                                {
                                    if(RD==0xff)
                                    {
                                        filetransfercomplete=false;
                                        ui->programUpdate_pb->setEnabled(true);
                                    }
                                }
                                else
                                {
                                    ui->programUpdate_pb->setEnabled(true);
                                    filetransfercomplete=false;
                                }
                            }
                        }

                    }
                }
            }
        }
    }

    //按键操作
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)1)
        {
            temp[0]=255;
            on_copyToUSB_pb_clicked();
        }
        if(temp[0]==(char)2)
        {
            temp[0]=255;
        }
        if(temp[0]==(char)10)
        {
             temp[0]=255;

        }
        if(temp[0]==(char)34)
        {
            temp[0]=255;
            if(exchangeMark==0)
            {
                 on_programUpdate_pb_clicked();
            }
            else if(exchangeMark==1)
            {
                on_copyToDISK_pb_clicked();
            }
        }
        if(temp[0]==(char)28)
        {
            temp[0]=255;
            on_pushButton_4_clicked();
        }
        if(temp[0]==(char)27)
        {
            temp[0]=255;
            on_delete_pb_clicked();
        }
        if(temp[0]==(char)29)
        {
            temp[0]=255;
            exchangeMark=1;
            ui->usbMesg_pb->setChecked(true);
            ui->diskMesg_pb->setChecked(false);
            ui->programUpdate_pb->setEnabled(false);
        }
        if(temp[0]==(char)39)
        {
            temp[0]=255;
            exchangeMark=0;
            ui->diskMesg_pb->setChecked(true);
            ui->usbMesg_pb->setChecked(false);
            ui->programUpdate_pb->setEnabled(true);
        }
        if(temp[0]==(char)33)
        {
            temp[0]=255;
            if(exchangeMark==0)
            {
                int row=ui->DISK_twd->currentRow();
                if(row>0&&ui->DISK_twd->rowCount()>0)
                {
                    row--;
                    ui->DISK_twd->setCurrentCell(row,0);
                }
            }
            else if(exchangeMark==1)
            {
                int row=ui->USB_twd->currentRow();
                if(row>0&&ui->USB_twd->rowCount()>0)
                {
                    row--;
                    ui->USB_twd->setCurrentCell(row,0);
                }
            }
        }
        if(temp[0]==(char)35)
        {
            temp[0]=255;
            if(exchangeMark==0)
            {
                int row=ui->DISK_twd->currentRow();
                if(row>-1&&row<ui->DISK_twd->rowCount()-1)
                {
                    row++;
                    ui->DISK_twd->setCurrentCell(row,0);
                }
            }
            else if(exchangeMark==1)
            {
                int row=ui->USB_twd->currentRow();
                if(row>-1&&row<ui->USB_twd->rowCount()-1)
                {
                    row++;
                    ui->USB_twd->setCurrentCell(row,0);
                }
            }
        }
    }
}

//--
void controlUI::on_pushbutton_sub_clicked()
{
    if(proType==StepMotor||proType==Actuator)
    {
        if(feeder==0xff)
        {
            feeder=maxfeeder;
            ui->label_feeder->setText(QString::number(feeder));
        }
        else
        {
            if(feeder==1)
            {
                feeder=0xff;
                ui->label_feeder->setText(tr("全路"));
            }
            else
            {
                if(feeder>1)
                {
                    feeder--;
                    ui->label_feeder->setText(QString::number(feeder));
                }
            }
        }
    }
    else if(proType==AirValve)
    {
        if(feeder==0xff)
        {
            feeder=maxfeeder;
            ui->label_feeder->setText(QString::number(feeder));
        }
        else
        {
            if(feeder==1)
            {
                feeder=0xff;
                ui->label_feeder->setText(tr("全路"));
            }
            else
            {
                if(feeder>1)
                {
                    if(feeder==11)
                        feeder-=3;
                    else
                        feeder--;
                    ui->label_feeder->setText(QString::number(feeder));
                }
            }
        }
    }
}

//++
void controlUI::on_pushbutton_add_clicked()
{
    if(proType==StepMotor||proType==Actuator)
    {
        if(feeder==0xff)
        {
            feeder=1;
            ui->label_feeder->setText(QString::number(feeder));
        }
        else
        {
            if(feeder==maxfeeder)
            {
                feeder=0xff;
                ui->label_feeder->setText(tr("全路"));
            }
            else
            {
                if(feeder<maxfeeder)
                {
                    feeder++;
                    ui->label_feeder->setText(QString::number(feeder));
                }
            }
        }
    }
    else if(proType==AirValve)
    {
        if(feeder==0xff)
        {
            feeder=1;
            ui->label_feeder->setText(QString::number(feeder));
        }
        else
        {
            if(feeder==maxfeeder)
            {
                feeder=0xff;
                ui->label_feeder->setText(tr("全路"));
            }
            else
            {
                if(feeder<maxfeeder)
                {
                    if(feeder==8)
                        feeder+=3;
                    else
                        feeder++;
                    ui->label_feeder->setText(QString::number(feeder));
                }
            }
        }
    }
}
