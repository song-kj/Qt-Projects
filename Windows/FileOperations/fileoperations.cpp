#include "fileoperations.h"
#include "ui_fileoperations.h"
#include "mymessagebox.h"
#include "modifyco.h"

extern QByteArray temp;
int downLoadPatn;

FileOperations::FileOperations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileOperations)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    downLoadFlag = false;
    patterndownloaded=false;
    patterndowning=false;
    firstEnter=true;
    Ram.to_Fileoperations_slot();
    Ram.Send(0xAA,0x300);

    initialForm();

    timer_fileoperations = new QTimer(this);
    ui->pushButton->hide();
    ui->label_tips->hide();
    connect(timer_fileoperations, SIGNAL(timeout()),this,SLOT(update_slot()));
    timer_fileoperations->start(20);

    ui->progressBar->hide();
    //文件后缀名过滤器
    filters << "*.CO"<<"*.co"/*<<"*.uco"*/;
    watcher = new QFileSystemWatcher(this);
    QStringList strList;
    strList<<"/media"<<"/media/myusb";
    watcher->addPaths(strList);
    connect(watcher,SIGNAL(directoryChanged(QString)),this,SLOT(showinfo_USB()));
    m_pMsg = new ErrorCode;
    showinfo_DISK();//读取本地磁盘文件信息
    QDir dir;
    if(dir.exists("media/myusb"))
    {
        ui->label_tips->setText("U盘文件加载中，请稍后！");
        ui->label_tips->show();
    }
    else
        ui->label_tips->hide();
}
/*析构函数*/
FileOperations::~FileOperations()
{
    delete ui;
}

/*关闭该界面*/
void FileOperations::on_back_pb_clicked()
{
    timer_fileoperations->stop();
    this->close();
}

/*显示USB文件*/
void FileOperations::showinfo_USB()
{
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        dir.setFilter(QDir::Dirs|QDir::Files);
        dir.setNameFilters(filters);
        QFileInfoList list = dir.entryInfoList();
        ui->USBdevice_twd->clearContents();
        ui->USBdevice_twd->setRowCount(list.size());
        ui->USBdevice_twd->setColumnCount(1);
        if(list.size()>0)
        {
            for(int i = 0;i < list.size();i++)
            {
                QFileInfo file_info = list.at(i);
                QString name = file_info.fileName();
                QByteArray ba = name.toLatin1();
                ui->USBdevice_twd->setItem(i,0,new QTableWidgetItem(ba.data()));
            }
            ui->USBdevice_twd->setCurrentCell(0,0);
        }
    }
    else
    {
        tableChange(root);
    }
}

/*显示本地磁盘文件*/
void FileOperations::showinfo_DISK()
{
    QString name;
    QDir dir(Path_loacl);
    if(dir.exists())
    {
        dir.setFilter(QDir::Dirs|QDir::Files);
        dir.setNameFilters(filters);
        QFileInfoList list = dir.entryInfoList();
        ui->diskdevide_twd->clearContents();
        ui->diskdevide_twd->setRowCount(0);

        if(list.size()>0)
        {
            for(int i = 0;i < list.size();i++)
            {
                QFileInfo file_info = list.at(i);
                name = file_info.fileName();
                int row = ui->diskdevide_twd->rowCount();
                ui->diskdevide_twd->insertRow(row);
                QLabel *lab=new QLabel(this);
                //显示当前下载的花型文件
                if(file_info.fileName()==projectCOName)
                {
                    lab->setStyleSheet("image: url(:/img/settings.png);");
                }
                else
                {
                    lab->setStyleSheet("image: url(:/img/hard_disk.png);");
                }
                ui->diskdevide_twd->setCellWidget(i,0,lab);
                ui->diskdevide_twd->setItem(i,1,new QTableWidgetItem(name));
            }
            ui->diskdevide_twd->setCurrentCell(0,0);
        }
    }
}

/*删除文件*/
void FileOperations::on_filedelete_File_pb_clicked()
{
    int rowindex=ui->diskdevide_twd->currentRow();
    if(rowindex>-1){
        QString filename=ui->diskdevide_twd->item(rowindex,1)->text();
        if(filename==projectCOName)
        {
            MyMessageBox *box = new MyMessageBox("提示","无法删除当前编织花型！",0);
            box->exec();
            return;
        }
        MyMessageBox *box = new MyMessageBox("提示","是否删除该文件？",1);
        if(box->exec()==QDialog::Accepted)
        {
            deleteFileSlot(true);
        }
    }
}

/*文件拷贝到USB设备槽函数*/
void FileOperations::on_copytoUSB_File__pb_clicked()
{
    //U盘设备存在 可复制花型文件到U盘
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        int rowindex=ui->diskdevide_twd->currentIndex().row();
        //本地设备文件列表不为空
        if(rowindex>-1)
        {
            QString filenamefrom=QString("%1%2").arg(Path_loacl).arg(ui->diskdevide_twd->item(rowindex,1)->text());
            QString filenameto=QString("%1%2").arg(Path_myusb).arg(ui->diskdevide_twd->item(rowindex,1)->text());
            qDebug()<<filenamefrom<<filenameto;
            if(QFile::exists(filenameto))
            {
                QFile::remove(filenameto);
                QFile::copy(filenamefrom,filenameto);
                system("sync");
                MyMessageBox *box = new MyMessageBox("提示","覆盖成功！",0);
                box->exec();
            }
            else
            {
                QFile::copy(filenamefrom,filenameto);
                system("sync");
                MyMessageBox *box = new MyMessageBox("提示","拷贝成功！",0);
                box->exec();
            }
        }
    }
}

/*文件拷贝到本地磁盘槽函数*/
void FileOperations::on_copytodisk_File_pb_clicked()
{
    //U盘设备存在 可复制花型文件到U盘
    QDir dir(Path_myusb);
    if(dir.exists())
    {
        int rowindex=ui->USBdevice_twd->currentIndex().row();
        //本地设备文件列表不为空
        if(rowindex>-1)
        {
            QString filenamefrom=QString("%1%2").arg(Path_myusb).arg(ui->USBdevice_twd->item(rowindex,0)->text());
            QString filenameto=QString("%1%2").arg(Path_loacl).arg(ui->USBdevice_twd->item(rowindex,0)->text());
            if(QFile::exists(filenameto))
            {
                QFile::remove(filenameto);
                QFile::copy(filenamefrom,filenameto);
                system("sync");
                showinfo_DISK();//读取本地磁盘文件信息
                MyMessageBox *box = new MyMessageBox("提示","覆盖成功！",0);
                box->exec();
            }
            else
            {
                QFile::copy(filenamefrom,filenameto);
                system("sync");
                showinfo_DISK();//读取本地磁盘文件信息
                MyMessageBox *box = new MyMessageBox("提示","拷贝成功！",0);
                box->exec();
            }
        }
    }
}

/*刷新槽函数*/
void FileOperations::on_flush_file_pb_clicked()
{
    showinfo_DISK();
    showinfo_USB();
}

void FileOperations::transPat()
{
    if(tabletype==root)
    {
        filename_transmit_uco=table->item(table->currentRow(),1)->text();
        filename_transmit_uco.chop(3);
        filepath_DISK_transmit=tr(Path_loacl).append(filename_transmit);
//        filepath_DISK_transmit_uco=tr(Path_loacl).append(filename_transmit_uco).append(".uco");
        filepath_DISK_transmit_uco=Path_patternUCO;
        if(QFile::exists(filepath_DISK_transmit_uco))
                QFile::remove(filepath_DISK_transmit_uco);
        QString fileco="/xrfile/temp.CO";
        if(QFile::exists(fileco))
            QFile::remove(fileco);
        if(QFile::copy(filepath_DISK_transmit,fileco)==false)
        {
            this->setEnabled(true);
            ui->label_tips->setText(QString::fromUtf8("解析失败！"));
            ui->label_tips->show();
            return;
        }
        if(dealCO(fileco)==0)
        {
            this->setEnabled(true);
            ui->label_tips->setText(tr("解析失败！"));
            ui->label_tips->show();
            return;
        }
        QByteArray med = fileco.toAscii();
        char *filepath_DISK_transmit_char = med.data();
        QByteArray _med = filepath_DISK_transmit_uco.toAscii();
        char *filepath_DISK_transmit_uco_char = _med.data();
#ifdef Q_OS_LINUX
        int boo = co2uco(filepath_DISK_transmit_char,filepath_DISK_transmit_uco_char);
        if(boo == 0)
        {
            this->setEnabled(true);
            ui->label_tips->setText(tr("解析失败！"));
            ui->label_tips->show();
            return;
        }
        if(dealUCO(filepath_DISK_transmit_uco)==0)
        {
            this->setEnabled(true);
            ui->label_tips->setText(tr("解析失败！"));
            ui->label_tips->show();
            return;
        }
#endif
        //提取速度信息并判断花型总针数
        for(int i=0;i<70;i++)
        {

            speedinfo.step[i]=0;
            speedinfo.speed[i]=0;
            speedinfo.ramp[i]=0;
        }
        for(int i=0;i<50;i++)
        {
            ecoinfo.step_s[i]=0;
            ecoinfo.step_e[i]=0;
            ecoinfo.value[i]=0;
        }
        if(fileSpeed_extract(fileco))
        {
            //提取循环信息
            if(fileCirculation_extract(fileco)==false)
            {
                ui->label_tips->setText("");
                this->setEnabled(true);
                return;
            }
            //提取压针信息
            if(fileNeedle_extract(fileco)==false)
            {
                this->setEnabled(true);
                ui->label_tips->setText("");
                return;
            }
            //提取裸氨信息
            readluoan(filepath_DISK_transmit);

            para_operation.machine_par_open();
            QFile file(filepath_DISK_transmit_uco);
            if(file.open(QIODevice::ReadWrite))
            {
                totalBytes=file.size();
                //花型传递请求
                totalPacks=(totalBytes+2559)/2560;
                file.close();
                ui->label_tips->hide();
                ui->progressBar->show();
                ui->progressBar->setValue(0);
                ui->progressBar->setRange(0,totalPacks);

                Ram.Send(0xAA,0x300);
                Ram.SendData[0]=0x30;
                Ram.SendData[1]=0x50;
                Ram.SendData[2]=0x00;
                Ram.SendData[3]=totalPacks%256;//总包数L
                Ram.SendData[4]=totalPacks/256;//总包数H
                Ram.SendData[5]=0x00;
                Ram.SendData[6]=0x00;
                Ram.SendData[7]=0x00;
                Ram.SendData[8]=0x00;
                Ram.SendData[9]=0x00;
                Ram.UI_to_RTC();
                timecount=0;
                patterndowning=true;
            }
            else
            {
                ui->label_tips->hide();
                this->setEnabled(true);
            }
        }
        else
        {
            ui->label_tips->hide();
            this->setEnabled(true);
        }
    }
    else
    {
        ui->label_tips->hide();
        this->setEnabled(true);
    }
}

//17寸偶数针或者18寸奇数针固定出针动作针位+1
int FileOperations::dealCO(QString pathco)
{
    int rst=1;
    QFile file(pathco);
    if(file.open(QIODevice::ReadWrite))
    {
        QByteArray array;
        file.seek(0xcc);
        array=file.read(2);
        int pos=((int)array[0]&0xff)<<8|((int)array[1]&0xff)<<16;
        file.seek(pos);

        //总针数在rest文件下 起始地址存于0x50
        file.seek(0x50);
        QByteArray ba=file.read(2);
        int addr=0;
        addr=(((int)ba[1]&0xff)+((int)ba[0]&0xff))<<8;
        if((int)ba[1]!=0)
            addr=(((int)ba[1]&0xff)<<16)+(((int)ba[0]&0xff)<<8);
        file.seek(addr+0x02);
        ba=file.read(4);
        int diametro=((int)ba[0]&0xff)|((int)ba[1]&0xff)<<8|((int)ba[2]&0xff)<<16|((int)ba[3]&0xff)<<24;
        ba=file.read(2);
        //总针数
        quint16 tneedle = (((int)ba[1]&0xff) << 8 | ((int)ba[0]&0xff));
        if(tneedle!=machine_Pars.totalneedles)
        {
            rst=0;
            SumNeedleError NeedleError;
            NeedleError.exec();
            para_operation.saveSystemLog(tr("花型%1总针数不匹配！").arg(filename_transmit),QDateTime::currentDateTime());
        }
        else
        {
            int machine_Diametro=machineSize*2.54*10000000;
            if(machine_Diametro!=diametro)
            {
                rst=0;
                SumNeedleError NeedleError;
                NeedleError.settext(tr("CO机型尺寸不匹配！"));
                NeedleError.exec();
                para_operation.saveSystemLog(tr("花型%1机型尺寸不匹配！").arg(filename_transmit),QDateTime::currentDateTime());
            }
        }
        if(rst==1)
        {
            //总步骤数在supe文件下 起始地址存于0x129
            file.seek(0x129);
            array=file.read(2);
            pos=((int)array[0]&0xff)<<8|((int)array[1]&0xff)<<16;
            file.seek(pos+0x04);
            array=file.read(4);
            //总步骤数
            totalStep = (((int)array[3]&0xff) << 24 | ((int)array[2]&0xff) << 16 | ((int)array[1]&0xff) << 8 | ((int)array[0]&0xff));

            file.seek(0xcc);
            array=file.read(2);
            pos=((int)array[0]&0xff)<<8|((int)array[1]&0xff)<<16;
            file.seek(pos+0x40);
            int offset[totalStep+1];
            for(int i=0;i<=totalStep;i++)
            {
                array=file.read(4);
                offset[i]=(((int)array[3]&0xff) << 24 | ((int)array[2]&0xff) << 16 | ((int)array[1]&0xff) << 8 | ((int)array[0]&0xff));
            }

            for(int i=0;i<=totalStep;i++)
            {
                file.seek(pos+0x40+offset[i]);
                array=file.read(4);
                quint32 a=((int)array[3]&0xff)<<24|((int)array[2]&0xff)<<16|((int)array[1]&0xff)<<8|((int)array[0]&0xff);
                int count=0;
                while (a!=0x80000000) {
                    if(count>1000)
                    {
                        rst=0;
                        break;
                    }
                    addr=file.pos();
                    array=file.read(44);
                    if((quint8)array[0]==0x0b)
                    {
                        if((machineSize==17&&machine_Pars.fixed_needle==1))
                        {
                            int needle=((int)array[19]&0xff)<<24|((int)array[18]&0xff)<<16|((int)array[17]&0xff)<<8|((int)array[16]&0xff);
                            needle++;
                            array[16]=(quint8)needle;
                            array[17]=(quint8)(needle>>8);
                            array[18]=(quint8)(needle>>16);
                            array[19]=(quint8)(needle>>24);

                            needle=((int)array[39]&0xff)<<24|((int)array[38]&0xff)<<16|((int)array[37]&0xff)<<8|((int)array[36]&0xff);
                            needle++;
                            array[36]=(quint8)needle;
                            array[37]=(quint8)(needle>>8);
                            array[38]=(quint8)(needle>>16);
                            array[39]=(quint8)(needle>>24);
                            file.seek(addr);
                            file.write(array);
                            file.seek(addr+44);
                        }
                    }
                    array=file.read(4);
                    a=((int)array[3]&0xff)<<24|((int)array[2]&0xff)<<16|((int)array[1]&0xff)<<8|((int)array[0]&0xff);
                    count++;
                }
                if(rst==0)
                    break;
            }
        }
        file.close();
    }
    else
        rst=0;
    return rst;
}

//处理uco花型dis密度数据
int FileOperations::dealUCO(QString pathuco)
{
    int rst=1;
    QFile file(pathuco);
    if(file.open(QIODevice::ReadWrite))
    {
        QDataStream out(&file);
        //总步骤数在supe文件下 起始地址存于0x129
        out.device()->seek(0x129);
        QByteArray  array=file.read(2);
        int  supestart=((int)array[0]&0xff)<<8|((int)array[1]&0xff)<<16;
        file.seek(supestart+0x04);
        array=file.read(4);
        //总步骤数
        totalStep=(((int)array[3]&0xff)<<24|((int)array[2]&0xff)<<16|((int)array[1]&0xff)<<8|((int)array[0]&0xff));


        file.seek(0xcc);//Jacq头文件地址
        QByteArray ba=file.read(2);
        int addr=0;
        addr=(((int)ba[1]&0xff)+((int)ba[0]&0xff))<<8;
        if((int)ba[1]!=0)
            addr=(((int)ba[1]&0xff)<<16)+(((int)ba[0]&0xff)<<8);
        addr+=0x40;
        file.seek(addr);
        quint32 offset[totalStep+1];
        for(int i=0;i<totalStep+1;i++)
        {
            ba=file.read(4);
            offset[i]=(((int)ba[3]&0xff)<<24)+(((int)ba[2]&0xff)<<16)+(((int)ba[1]&0xff)<<8)+((int)ba[0]&0xff);
        }
        bool b=false;
        for(int i=0;i<totalStep+1;i++)
        {
            file.seek(addr+offset[i]);
            ba=file.read(4);
            while (!(((quint8)ba[0])==0x00&&((quint8)ba[1])==0x00&&((quint8)ba[2])==0x00&&((quint8)ba[3]&0xff)==0x80)) {
                ba=file.read(44);
                if((int)ba[0]==0x0A)//pattern指令
                {
                    if((int)ba[0x0c]==0x01)
                    {
                        b=true;
                        break;
                    }
                }
                ba=file.read(4);
            }
            if(b)
                break;
        }
        //花型进入步和结束步滞后一步
        if(b)
        {
            out.device()->seek(0x2d0);
            quint32 startAddr;
            out>>startAddr;startAddr=deal_4bytes(startAddr);
            out.device()->seek(startAddr);
            quint16 countDis;
            out>>countDis;
            countDis=deal_2bytes(countDis);
            quint32 startStep,endStep;
            for(int i=0;i<countDis;i++)
            {
                out.device()->seek(startAddr+2+i*16);
                out>>startStep;startStep=deal_4bytes(startStep);
                out>>endStep;endStep=deal_4bytes(endStep);
                startStep++;
                endStep++;
                out.device()->seek(startAddr+2+i*16);
                out<<quint8(startStep);
                out<<quint8(startStep>>8);
                out<<quint8(startStep>>16);
                out<<quint8(startStep>>24);
                out<<quint8(endStep);
                out<<quint8(endStep>>8);
                out<<quint8(endStep>>16);
                out<<quint8(endStep>>24);
            }
        }

        //处理dis密度数据
        {
            out.device()->seek(0x254);
            quint16 totalneedles;
            out>>totalneedles;
            totalneedles=deal_2bytes(totalneedles);
            out.device()->seek(0x260);
            quint32 startAddr,endAddr;
            out>>startAddr;startAddr=deal_4bytes(startAddr);
            out.device()->seek(0x268);
            out>>endAddr;endAddr=deal_4bytes(endAddr);

            out.device()->seek(startAddr);
            quint16 count;
            out>>count; count=deal_2bytes(count);
            if(count!=0)
            {
                QList<dismotor*> listMotor;
                for(int i=0;i<count;i++)
                {
                    listMotor.append(new dismotor);
                    out>>listMotor[i]->startStep;listMotor[i]->startStep=deal_4bytes(listMotor[i]->startStep);
                    out>>listMotor[i]->endStep;listMotor[i]->endStep=deal_4bytes(listMotor[i]->endStep);
                    out>>listMotor[i]->startAddr;listMotor[i]->startAddr=deal_4bytes(listMotor[i]->startAddr);
                    out>>listMotor[i]->endAddr;listMotor[i]->endAddr=deal_4bytes(listMotor[i]->endAddr);
                }
                quint16 tempquan,temprow,actcount;;
                for(int i=0;i<count;i++)
                {
                    out.device()->seek(listMotor[i]->startAddr);
                    out>>listMotor[i]->width;;
                    out>>listMotor[i]->height;;
                    out>>tempquan;tempquan=deal_2bytes(tempquan);
                    int count=0;
                    while (tempquan!=0xffff) {
                        if(count>9999)
                        {
                            rst=0;
                            break;
                        }
                        out>>temprow;temprow=deal_2bytes(temprow);
                        out>>actcount;actcount=deal_2bytes(actcount);
                        for(int j=0;j<actcount;j++)
                        {
                            action *act=new action();
                            act->quan=tempquan;
                            act->row=temprow;
                            out>>act->needle;act->needle=deal_2bytes(act->needle);
                            out>>act->value;
                            act->needle+=((45/360.0)*totalneedles+15);
                            if(act->needle>=totalneedles)
                            {
                                act->needle-=totalneedles;
                                act->quan++;
                                act->row+=8;
                            }
                            listMotor[i]->actions.append(act);
                        }
                        out>>tempquan;tempquan=deal_2bytes(tempquan);
                        count++;
                    }
                    if(rst==1)
                    {
                        listMotor[i]->lisact.clear();
                        for(int k=0;k<listMotor[i]->actions.length();k++)
                        {
                            int index=listMotor[i]->lisact.length();
                            for(int l=0;l<index;l++)
                            {
                                //判断圈、行是否存在，存在就在后面添加针位和动作数据
                                if(listMotor[i]->lisact[l]->quan==listMotor[i]->actions[k]->quan&&
                                        listMotor[i]->lisact[l]->row==listMotor[i]->actions[k]->row)
                                {
                                    listMotor[i]->lisact[l]->values.append(listMotor[i]->actions[k]->needle);
                                    listMotor[i]->lisact[l]->values.append(listMotor[i]->actions[k]->value);
                                    break;
                                }
                                //当l=最大值时，添加新的圈行数据
                                if(l==listMotor[i]->lisact.length()-1)
                                {
                                    bool b=true;
                                    for(int m=0;m<listMotor[i]->lisact.length();m++)
                                    {
                                        if(listMotor[i]->actions[k]->quan==listMotor[i]->lisact[m]->quan)
                                        {
                                            if(listMotor[i]->actions[k]->row<listMotor[i]->lisact[m]->row)
                                            {
                                                action *ac=new action;
                                                ac->quan=listMotor[i]->actions[k]->quan;
                                                ac->row=listMotor[i]->actions[k]->row;
                                                ac->values.append(listMotor[i]->actions[k]->needle);
                                                ac->values.append(listMotor[i]->actions[k]->value);
                                                listMotor[i]->lisact.insert(m,ac);
                                                b=false;
                                                break;
                                            }
                                        }
                                        else if(listMotor[i]->actions[k]->quan<listMotor[i]->lisact[m]->quan)
                                        {
                                            action *ac=new action;
                                            ac->quan=listMotor[i]->actions[k]->quan;
                                            ac->row=listMotor[i]->actions[k]->row;
                                            ac->values.append(listMotor[i]->actions[k]->needle);
                                            ac->values.append(listMotor[i]->actions[k]->value);
                                            listMotor[i]->lisact.insert(m,ac);
                                            b=false;
                                            break;
                                        }
                                    }
                                    if(b)
                                    {
                                        action *ac=new action;
                                        ac->quan=listMotor[i]->actions[k]->quan;
                                        ac->row=listMotor[i]->actions[k]->row;
                                        ac->values.append(listMotor[i]->actions[k]->needle);
                                        ac->values.append(listMotor[i]->actions[k]->value);
                                        listMotor[i]->lisact.append(ac);
                                    }
                                }
                            }
                            if(listMotor[i]->lisact.count()==0)
                            {
                                action *ac=new action;
                                ac->quan=listMotor[i]->actions[k]->quan;
                                ac->row=listMotor[i]->actions[k]->row;
                                ac->values.append(listMotor[i]->actions[k]->needle);
                                ac->values.append(listMotor[i]->actions[k]->value);
                                listMotor[i]->lisact.append(ac);
                                continue;
                            }
                        }
                    }
                    else
                        break;
                }
                if(rst==1)
                {
                    out.device()->seek(startAddr+2+16*count);
                    for(int i=0;i<count;i++)
                    {
                        listMotor[i]->startAddr=out.device()->pos();
                        out<<listMotor[i]->width;
                        out<<listMotor[i]->height;
                        qDebug()<<listMotor[i]->lisact.length();
                        for(int j=0;j<listMotor[i]->lisact.length();j++)
                        {
                            out<<(quint8)(listMotor[i]->lisact[j]->quan);
                            out<<(quint8)(listMotor[i]->lisact[j]->quan>>8);
                            out<<(quint8)(listMotor[i]->lisact[j]->row);
                            out<<(quint8)(listMotor[i]->lisact[j]->row>>8);
                            out<<(quint8)(listMotor[i]->lisact[j]->values.length()/2);
                            out<<(quint8)((listMotor[i]->lisact[j]->values.length()/2)>>8);
                            for(int k=0;k<listMotor[i]->lisact[j]->values.length();k++)
                            {
                                if(k%2==0)//写入针位
                                {
                                    out<<(quint8)(listMotor[i]->lisact[j]->values[k]);
                                    out<<(quint8)(listMotor[i]->lisact[j]->values[k]>>8);
                                }
                                else
                                    out<<listMotor[i]->lisact[j]->values[k];
                            }
                        }
                        quint16 temp=0xffff;
                        out<<temp;
                        listMotor[i]->endAddr=out.device()->pos()-1;
                    }
                    endAddr=out.device()->pos()-1;
                    out.device()->seek(startAddr+2);
                    for(int i=0;i<count;i++)
                    {
                        out<<quint8(listMotor[i]->startStep);
                        out<<quint8(listMotor[i]->startStep>>8);
                        out<<quint8(listMotor[i]->startStep>>16);
                        out<<quint8(listMotor[i]->startStep>>24);
                        out<<quint8(listMotor[i]->endStep);
                        out<<quint8(listMotor[i]->endStep>>8);
                        out<<quint8(listMotor[i]->endStep>>16);
                        out<<quint8(listMotor[i]->endStep>>24);
                        out<<quint8(listMotor[i]->startAddr);
                        out<<quint8(listMotor[i]->startAddr>>8);
                        out<<quint8(listMotor[i]->startAddr>>16);
                        out<<quint8(listMotor[i]->startAddr>>24);
                        out<<quint8(listMotor[i]->endAddr);
                        out<<quint8(listMotor[i]->endAddr>>8);
                        out<<quint8(listMotor[i]->endAddr>>16);
                        out<<quint8(listMotor[i]->endAddr>>24);
                    }
                    out.device()->seek(0x260);
                    out<<quint8(startAddr);
                    out<<quint8(startAddr>>8);
                    out<<quint8(startAddr>>16);
                    out<<quint8(startAddr>>24);
                    out.device()->seek(0x268);
                    out<<quint8(endAddr);
                    out<<quint8(endAddr>>8);
                    out<<quint8(endAddr>>16);
                    out<<quint8(endAddr>>24);
                }
            }
        }

        //处理dis色控纱嘴数据 更改色纱数据格式
        {
            out.device()->seek(0xeb);
            array=file.read(2);
            //获得guid起始地址
            int guidstart=((int)array[0]&0xff)<<8|((int)array[1]&0xff)<<16;
            guidstart+=0x40;
            //guid偏移地址
            quint32 guidOffset[totalStep+1];
            file.seek(guidstart);
            for(int i=0;i<=totalStep;i++)
            {
                array=file.read(4);
                guidOffset[i]=(((int)array[3]&0xff) << 24 | ((int)array[2]&0xff) << 16 | ((int)array[1]&0xff) << 8 | ((int)array[0]&0xff));
            }
            for(int i=0;i<=totalStep;i++)
            {
                file.seek(guidstart+guidOffset[i]);
                //读取四字节 判断结束标志（0x80000000）
                array=file.read(4);
                quint32 a=((int)array[3]&0xff)<<24|((int)array[2]&0xff)<<16|((int)array[1]&0xff)<<8|((int)array[0]&0xff);
                int count=0;
                while (a!=0x80000000) {
                    if(count>1000)
                    {
                        rst=0;
                        break;
                    }
                    //读取余下的44个字节
                    array=file.read(60);
                    //纱嘴路数 0-7路
                    int guidfeeder=a;
                    //纱嘴起始针位即机械位置
                    int needle=((int)array[23]&0xff)<<24|((int)array[22]&0xff)<<16|((int)array[21]&0xff)<<8|((int)array[20]&0xff);
                    if(guidfeeder<8)
                        guidPosition[guidfeeder]=needle-machine_Pars.actuator_position[guidfeeder*2];
                    array=file.read(4);
                    a=((int)array[3]&0xff)<<24|((int)array[2]&0xff)<<16|((int)array[1]&0xff)<<8|((int)array[0]&0xff);
                    count++;
                }
                if(rst==0)
                    break;
            }
            if(rst==1)
            {
                //读取数据起始地址和结束地址
                out.device()->seek(0x2e0);
                quint32 startAddr,endAddr;
                out>>startAddr;startAddr=deal_4bytes(startAddr);
                out.device()->seek(0x2e8);
                out>>endAddr;endAddr=deal_4bytes(endAddr);
                out.device()->seek(startAddr);
                //读取dis个数
                quint16 count;
                out>>count; count=deal_2bytes(count);
                if(count!=0)
                {
                    //读取每个dis的地址信息
                    QList<disYarn*> listYarn;
                    for(int i=0;i<count;i++)
                    {
                        listYarn.append(new disYarn);
                        out>>listYarn[i]->startStep;listYarn[i]->startStep=deal_4bytes(listYarn[i]->startStep);
                        out>>listYarn[i]->endStep;listYarn[i]->endStep=deal_4bytes(listYarn[i]->endStep);
                        out>>listYarn[i]->startAddr;listYarn[i]->startAddr=deal_4bytes(listYarn[i]->startAddr);
                        out>>listYarn[i]->endAddr;listYarn[i]->endAddr=deal_4bytes(listYarn[i]->endAddr);
                    }

//                    int interval=machine_Pars.actuator_position[1]-machine_Pars.actuator_position[0];
                    //提取每个纱嘴动作数据
                    quint16 tempquan,temprow,tempneedle;
                    quint8 actcount;
                    for(int i=0;i<count;i++)
                    {
                        //读取圈数 判断圈数是否为0xffff
                        out.device()->seek(listYarn[i]->startAddr);
                        out>>tempquan;tempquan=deal_2bytes(tempquan);

                        while (tempquan!=0xffff) {
                            //读取行
                            out>>temprow;temprow=deal_2bytes(temprow);
                            //读取针位
                            out>>tempneedle;tempneedle=deal_2bytes(tempneedle);
                            //读取动作个数
                            out>>actcount;
                            for(int j=0;j<actcount;j++)
                            {
                                yarnAction *act=new yarnAction;
                                act->quan=tempquan;
                                act->row=temprow;
                                act->needle=tempneedle;
                                out>>act->value[0]>>act->value[1]>>act->value[2];
                                if(act->value[0]<8)
                                {
                                    //针位+纱嘴机型位置
                                    act->needle+=(guidPosition[act->value[0]]+15);
//                                    if(act->value[1]<6)
//                                        act->needle+=(interval+yarnFingerAngle[act->value[1]]/360000.0*machine_Pars.totalneedles);
//                                    else
//                                        act->needle+=(interval+yarnFingerAngle[act->value[1]-1]/360000.0*machine_Pars.totalneedles);
                                    if(act->needle>=machine_Pars.totalneedles)
                                    {
                                        act->needle-=machine_Pars.totalneedles;
                                        act->quan++;
                                    }
                                }
                                listYarn[i]->actions.append(act);
                            }
                            out>>tempquan;tempquan=deal_2bytes(tempquan);
                        }
                        qSort(listYarn[i]->actions.begin(),listYarn[i]->actions.end(),compareBarData);
                        int addr=0;
                        if(listYarn[i]->actions.length()>0)
                        {
                            yarnNewAct *newact=new yarnNewAct;
                            quint16 quan=listYarn[i]->actions[0]->quan;
                            newact->quan=quan;
                            newact->count=0;
                            for(int j=0;j<listYarn[i]->actions.length();j++)
                            {
                                if(listYarn[i]->actions[j]->quan==quan)
                                {
                                    newact->count++;
                                    newact->values.append((quint8)listYarn[i]->actions[j]->needle);
                                    newact->values.append((quint8)(listYarn[i]->actions[j]->needle>>8));
                                    newact->values.append(listYarn[i]->actions[j]->value[0]);
                                    newact->values.append(listYarn[i]->actions[j]->value[1]);
                                    newact->values.append(listYarn[i]->actions[j]->value[2]);
                                    addr+=5;
                                    if(j==listYarn[i]->actions.length()-1)
                                        listYarn[i]->newActs.append(newact);
                                }
                                else
                                {
                                    listYarn[i]->newActs.append(newact);
                                    newact=new yarnNewAct;
                                    quan=listYarn[i]->actions[j]->quan;
                                    newact->quan=quan;
                                    newact->count=1;
                                    newact->values.append((quint8)listYarn[i]->actions[j]->needle);
                                    newact->values.append((quint8)(listYarn[i]->actions[j]->needle>>8));
                                    newact->values.append(listYarn[i]->actions[j]->value[0]);
                                    newact->values.append(listYarn[i]->actions[j]->value[1]);
                                    newact->values.append(listYarn[i]->actions[j]->value[2]);
                                    addr+=5;
                                    if(j==listYarn[i]->actions.length()-1)
                                        listYarn[i]->newActs.append(newact);
                                }
                            }
                        }
                        //起始地址先定为0
                        listYarn[i]->startAddr=0;
                        //结束地址为数据总长度
                        listYarn[i]->endAddr=addr+listYarn[i]->newActs.length()*3;
                    }
                    //重新计算每个dis数据的起始地址和结束地址
                    int yarnlength=2+count*16;
                    out.device()->seek(startAddr);
                    out<<(quint8)count;
                    out<<(quint8)(count>>8);

                    for(int i=0;i<count;i++)
                    {
                        yarnlength+=listYarn[i]->endAddr;
                        if(i==0)
                        {
                            listYarn[i]->startAddr=startAddr+2+count*16;
                            listYarn[i]->endAddr+=(listYarn[i]->startAddr-1+2);
                        }
                        else
                        {
                            listYarn[i]->startAddr=listYarn[i-1]->endAddr+1;
                            listYarn[i]->endAddr+=(listYarn[i]->startAddr-1+2);
                        }
                        out<<quint8(listYarn[i]->startStep);
                        out<<quint8(listYarn[i]->startStep>>8);
                        out<<quint8(listYarn[i]->startStep>>16);
                        out<<quint8(listYarn[i]->startStep>>24);
                        out<<quint8(listYarn[i]->endStep);
                        out<<quint8(listYarn[i]->endStep>>8);
                        out<<quint8(listYarn[i]->endStep>>16);
                        out<<quint8(listYarn[i]->endStep>>24);
                        out<<quint8(listYarn[i]->startAddr);
                        out<<quint8(listYarn[i]->startAddr>>8);
                        out<<quint8(listYarn[i]->startAddr>>16);
                        out<<quint8(listYarn[i]->startAddr>>24);
                        out<<quint8(listYarn[i]->endAddr);
                        out<<quint8(listYarn[i]->endAddr>>8);
                        out<<quint8(listYarn[i]->endAddr>>16);
                        out<<quint8(listYarn[i]->endAddr>>24);
                    }

                    quint16 end=0xffff;
                    for(int i=0;i<count;i++)
                    {
                        for(int j=0;j<listYarn[i]->newActs.length();j++)
                        {
                            out<<(quint8)(listYarn[i]->newActs[j]->quan);
                            out<<(quint8)(listYarn[i]->newActs[j]->quan>>8);
                            out<<listYarn[i]->newActs[j]->count;
                            for(int k=0;k<listYarn[i]->newActs[j]->count;k++)
                            {
                                out<<listYarn[i]->newActs[j]->values[k*5];
                                out<<listYarn[i]->newActs[j]->values[k*5+1];
                                out<<listYarn[i]->newActs[j]->values[k*5+2];
                                out<<listYarn[i]->newActs[j]->values[k*5+3];
                                out<<listYarn[i]->newActs[j]->values[k*5+4];
                            }
                        }
                        out<<end;
                    }
                    endAddr=out.device()->pos()-1;
                    out.device()->seek(0x2e0);
                    out<<quint8(startAddr);
                    out<<quint8(startAddr>>8);
                    out<<quint8(startAddr>>16);
                    out<<quint8(startAddr>>24);
                    out.device()->seek(0x2e8);
                    out<<quint8(endAddr);
                    out<<quint8(endAddr>>8);
                    out<<quint8(endAddr>>16);
                    out<<quint8(endAddr>>24);
                }
            }
        }
        file.close();
    }
    else
        rst=0;
    return rst;
}

bool FileOperations::compareBarData(const yarnAction *act1,const yarnAction *act2)
{
    if(act1->quan<act2->quan)
    {
        return true;
    }
    else if(act1->quan==act2->quan)
    {
        if(act1->needle<act2->needle)
            return true;
    }
    return false;
}

//读取裸氨参数
int FileOperations::readluoan(QString name)
{
    int rst=1;
    QFile file(name);
    int max_row=0;
    quint16 data[100][17];
    listmot.clear();
    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j < 17; j++)
        {
            data[i][j]=0xffff;
        }
    }
    if(file.open(QIODevice::ReadWrite))
    {
        QDataStream out(&file);
        out.device()->seek(0x6f);
        quint16 pos;
        out>>pos;
        quint32 position=pos;
        if(pos&0x00ff!=0)
            position=(pos&0xff00)+(pos&0x00ff)<<16;
        quint32 offset[8];
        //        pos+=0x20;
        out.device()->seek(position+0x20);
        for(int i=0;i<8;i++)
        {
            out>>offset[i];
            offset[i]=deal_4bytes(offset[i]);
        }
        quint8 bytes[100];
        quint16 se,ee;
        for(int i=0;i<8;i++)
        {
            out.device()->seek(position+offset[i]);
            for(int j=0;j<4;j++)
                out>>bytes[j];
            int count=0;
            while(bytes[0]==0x64)
            {
                if(count>1000)
                {
                    rst=0;
                    break;
                }
                for(int j=4;j<100;j++)
                    out>>bytes[j];
                se=bytes[32]+(bytes[33]<<8);
                ee=bytes[34]+(bytes[35]<<8);
                if(listmot.count()==0)
                {
                    mot m;
                    m.startstep=se;
                    m.endstep=ee;
                    m.data[i*2]=bytes[36]+(bytes[37]<<8);
                    m.data[i*2+1]=bytes[40]+(bytes[41]<<8);
                    listmot.append(m);
                }
                else
                {
                    bool b=false;
                    for(int j=0;j<listmot.count();j++)
                    {
                        if(listmot[j].startstep==se)
                        {
                            listmot[j].startstep=se;
                            listmot[j].endstep=ee;
                            listmot[j].data[i*2]=bytes[36]+(bytes[37]<<8);
                            listmot[j].data[i*2+1]=bytes[40]+(bytes[41]<<8);
                            b=true;
                            break;
                        }
                    }
                    if(b==false)
                    {
                        mot m;
                        m.startstep=se;
                        m.endstep=ee;
                        m.data[i*2]=bytes[36]+(bytes[37]<<8);
                        m.data[i*2+1]=bytes[40]+(bytes[41]<<8);
                        listmot.append(m);
                    }
                }
                for(int j=0;j<4;j++)
                    out>>bytes[j];
                count++;
            }
        }
        if(rst==1)
        {
            //读取总步骤数
            quint16 totalStep=0;
            qint32 start_addr;
            out.device()->seek(0x129);
            quint8 headerPos[2];
            out>>headerPos[0];
            out>>headerPos[1];
            if(headerPos[1]==0x00)
            {
                start_addr=headerPos[0]*256+headerPos[1];
            }
            else
            {
                start_addr=headerPos[0]*256+headerPos[1]*256*256;
            }
            out.device()->seek(start_addr+4);
            out>>totalStep;
            totalStep=deal_2bytes(totalStep);
            out.device()->seek(0x8e);
            out>>headerPos[0];
            out>>headerPos[1];
            if(headerPos[1]==0x00)
            {
                start_addr=headerPos[0]*256+headerPos[1];
            }
            else
            {
                start_addr=headerPos[0]*256+headerPos[1]*256*256;
            }
            quint16 delt_addr = (quint16)(start_addr+0x28);
            out.device()->seek(delt_addr);//从delt_addr开始存放偏移量
            out>>position;
            position=deal_4bytes(position);
            position+=start_addr;
            out.device()->seek(position);
            quint32 offset_1[totalStep];
            //获取8个电机的偏移量，每个偏移量占4个字节，低字节在前，高字节在后
            for(int i=0;i<totalStep;i++)
            {
                out>>offset_1[i];
                offset_1[i]=deal_4bytes(offset_1[i]);
            }
            quint16 degree;
            quint8 par[4];
            bool addrow=false;
            int index=-1;
            quint32 act;
            for(int i = 0; i < totalStep; i++)
            {
                out.device()->seek(position+offset_1[i]);
                out>>degree;
                degree=deal_2bytes(degree);
                addrow=false;
                int count=0;
                while(degree!=0x8000) //非结束标志
                {
                    if(count>1000)
                    {
                        rst=0;
                        break;
                    }
                    for(int j=0;j<4;j++)
                    {
                        out>>par[j];
                    }
                    if(par[0]>=0x0a&&par[0]<=0x19&&par[1]==0x01&&par[2]==0x02&&par[3]==0x00)
                    {
                        if(addrow==false)
                        {
                            max_row++;
                            addrow=true;
                            index++;
                        }
                        data[index][0]=i;
                        quint8 p=(par[0]-0x0a)/2;
                        data[index][p*2+1]=degree;
                        data[index][p*2+2]=(par[0]-1)%2;
                    }
                    if(par[2]==0x1f)////1f指令为10字节
                        out>>act;
                    out>>degree;
                    degree=deal_2bytes(degree);
                    count++;
                }
                if(rst==0)
                    break;
            }
        }
        file.close();
    }
    if(rst==1)
    {
        if(listmot.length()>0)
        {
            QSettings *set=new QSettings("/xrfile/luo_an.log",QSettings::IniFormat);
            set->setValue(QString("Luoan/row"),listmot.length());
            delete set;
            QFile fileluoan("/xrfile/Parameter_Luoan.er");
            if(fileluoan.exists())
                fileluoan.remove();
            if(fileluoan.open(QIODevice::ReadWrite))
            {
                QDataStream in(&fileluoan);
                for(int i=0;i<listmot.length();i++)
                {
                    in<<listmot[i].startstep;
                    in<<listmot[i].endstep;
                    for(int j=0;j<16;j++)
                        in<<listmot[i].data[j];
                }
                fileluoan.close();
                system("sync");
            }
        }
        else
        {
            if(QFile::exists("/xrfile/Parameter_Luoan.er"))
                QFile::remove("/xrfile/Parameter_Luoan.er");
        }
        if(max_row>0)
        {
            QFile fileluoan("/xrfile/Parameter_LuoanD.er");
            if(fileluoan.exists())
                fileluoan.remove();
            if(fileluoan.open(QIODevice::ReadWrite))
            {
                QDataStream in(&fileluoan);
                for(int i = 0; i < 100; i++)
                {
                    for(int j = 0; j < 17; j++)
                    {
                        in<<data[i][j];
                    }
                }
                fileluoan.close();
                system("sync");
            }
        }
        else{
            if(QFile::exists("/xrfile/Parameter_LuoanD.er"))
                QFile::remove("/xrfile/Parameter_LuoanD.er");
        }
    }
    return rst;
}

quint32 FileOperations::deal_4bytes(quint32 value)
{
    quint32 a=0;
    for(int i=0;i<4;i++)
    {
        a<<=8;
        a+=((value>>((i)*8))&0xff);
    }
    return a;
}

quint16 FileOperations::deal_2bytes(quint16 value)
{
    quint16 a=0;
    for(int i=0;i<2;i++)
    {
        a<<=8;
        a+=((value>>((i)*8))&0xff);
    }
    return a;
}

//界面初始化
void FileOperations::initialForm()
{
    for(int i=0;i<360;i++)
    {
        int needle=((i+25.5)/360)*machine_Pars.totalneedles;
        if(needle>machine_Pars.totalneedles)
            needle-=machine_Pars.totalneedles;
        listneedle.append(needle);
    }

    yarnFingerAngle[0]=15868;
    yarnFingerAngle[1]=14738;
    yarnFingerAngle[2]=13601;
    yarnFingerAngle[3]=12475;
    yarnFingerAngle[4]=11387;
    yarnFingerAngle[5]=9253;
    yarnFingerAngle[6]=3781;
    yarnFingerAngle[7]=2575;
    for(int i=0;i<8;i++)
        guidPosition[i]=0;
    ui->diskdevide_twd->setColumnCount(2);
    ui->diskdevide_twd->horizontalHeader()->resizeSection(0,40);
    ui->diskdevide_twd->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->diskdevide_twd->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->diskdevide_twd->verticalHeader()->setDefaultSectionSize(35); //设置行高
    ui->diskdevide_twd->setShowGrid(false); //设置不显示格子线
    ui->diskdevide_twd->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->diskdevide_twd->horizontalHeader()->setVisible(false);
    ui->diskdevide_twd->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->diskdevide_twd->setSelectionMode(QAbstractItemView::SingleSelection);//设置单行选择
    ui->diskdevide_twd->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->diskdevide_twd->setAlternatingRowColors(true);//设置奇偶行颜色不同
    ui->diskdevide_twd->horizontalHeader()->setStyleSheet("QHeaderView::section{color:white;background-color:#34495e;"
                                                       "font-size:22px;border:0px solid white;"
                                                       "border-width:0px 0px 0px 0px;}"); //设置表头背景色

    ui->diskdevide_twd->setStyleSheet("QScrollBar{width: 28px;margin: 28px 0 28px 0;}"
                                   "QScrollBar::handle{background:#a0a0a0; border:2px solid transparent; border-radius:0px;}"
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
                                   "font-size:24px;"
                                   "background-color:#ffffff;"
                                   "alternate-background-color:#a8ddfd;}"
                                   "QLabel{background-color:transparent;}");

    ui->USBdevice_twd->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->USBdevice_twd->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->USBdevice_twd->verticalHeader()->setDefaultSectionSize(35); //设置行高
    ui->USBdevice_twd->setShowGrid(false); //设置不显示格子线
    ui->USBdevice_twd->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->USBdevice_twd->horizontalHeader()->setVisible(false);
    ui->USBdevice_twd->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->USBdevice_twd->setSelectionMode(QAbstractItemView::SingleSelection);//设置单行选择
    ui->USBdevice_twd->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->USBdevice_twd->setAlternatingRowColors(true);//设置奇偶行颜色不同
    ui->USBdevice_twd->horizontalHeader()->setStyleSheet("QHeaderView::section{color:white;background-color:#34495e;"
                                                       "font-size:22px;border:0px solid white;"
                                                       "border-width:0px 0px 0px 0px;}"); //设置表头背景色

    ui->USBdevice_twd->setStyleSheet("QScrollBar{width: 28px;margin: 28px 0 28px 0;}"
                                   "QScrollBar::handle{background:#a0a0a0; border:2px solid transparent; border-radius:0px;}"
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
                                   "font-size:24px;"
                                   "background-color:#ffffff;"
                                   "alternate-background-color:#a8ddfd;}");
    connect(ui->diskdevide_twd,SIGNAL(cellClicked(int,int)),this,SLOT(diskCellClick(int,int)));
    connect(ui->USBdevice_twd,SIGNAL(cellClicked(int,int)),this,SLOT(usbCellClick(int,int)));
    ui->diskdevice_pb->setEnabled(false);
    ui->USBdevice_pb->setEnabled(false);
    tableChange(root);
}

//表格切换
void FileOperations::tableChange(TableType type)
{
    if(type==root)
    {
        ui->diskdevice_pb->setChecked(true);
        ui->USBdevice_pb->setChecked(false);
        table=ui->diskdevide_twd;
        tabletype=root;
    }
    else
    {
        QDir dir(Path_myusb);
        if(dir.exists())
        {
            ui->diskdevice_pb->setChecked(false);
            ui->USBdevice_pb->setChecked(true);
            table=ui->USBdevice_twd;
            tabletype=myusb;
        }
    }
}

/*花型下载*/
void FileOperations::on_patterndownload_pb_clicked()
{
    int rowindex=ui->diskdevide_twd->currentRow();
    if(rowindex>-1&&tabletype==root)
    {
        //编织状态和平布调试状态不能下载花型
        if(knitState==knitting||knitState==knitFlat)
        {
            ResetOrNot *reset = new ResetOrNot;
            reset->exec();
            return;
        }
        else
        {
            filename_transmit=ui->diskdevide_twd->item(rowindex,1)->text();
            QString name=filename_transmit;
            name.chop(3);
            filename_transmit_uco=QString("%1.uco").arg(name);
            if(filename_transmit.right(3)==".co"||filename_transmit.right(3)==".CO")
            {
                ui->progressBar->hide();
                ui->label_tips->show();
                ui->label_tips->setText(QString::fromUtf8("正在解析，请稍后..."));
                this->setEnabled(false);
                downLoadFlag = true;
                patterndowning=false;
            }
        }
    }
}

/*刷新槽*/
void FileOperations::update_slot()
{
    //第一次进入界面加载U盘文件
    if(firstEnter)
    {
        firstEnter=false;
        showinfo_USB();//读取USB文件信息
        ui->label_tips->hide();
    }
    //花型下载 开始解析花型
    if(downLoadFlag)
    {
        downLoadFlag = false;
        transPat();
    }
    //花型开始下载
    if(patterndowning)
    {
        timecount++;
        if(timecount>300)
        {
            ui->label_tips->setText(tr("花型下载超时！"));
            para_operation.saveSystemLog(ui->label_tips->text(),QDateTime::currentDateTime());
            ui->label_tips->show();
            ui->progressBar->hide();
            this->setEnabled(true);
            patterndowning=false;
            return;
        }
        if(Ram.Receive(0x300)==0x55)
        {
            if(Ram.Receive(0x200)==0x30)
            {
                if(Ram.Receive(0x201)==0x52)
                {
                    if(Ram.Receive(0x202)==0x00)
                    {
                        timecount=0;
                        Ram.Send(0xAA,0x300);
                        int baoshu=Ram.Receive(0x203)+Ram.Receive(0x204)*256;
                        int lastpack=2560;
                        if(baoshu==totalPacks-1)
                        {
                            lastpack= totalBytes-(totalPacks-1)*2560;
                        }
                        QFile file(filepath_DISK_transmit_uco);
                        if(file.open(QIODevice::ReadWrite))
                        {
                            file.seek(baoshu*2560);
                            QByteArray ba=file.read(lastpack);
                            for(int j=0;j<lastpack;j++)
                            {
                                Ram.Send((quint8)ba[j],0x500+j);
                            }
                            file.close();
                        }
                        ui->progressBar->setValue(baoshu);
                        Ram.SendData[0]=0x30;
                        Ram.SendData[1]=0x51;
                        Ram.SendData[2]=0x00;
                        Ram.SendData[3]=baoshu%256;//总包数L
                        Ram.SendData[4]=baoshu/256;//总包数H
                        Ram.SendData[5]=lastpack%256;
                        Ram.SendData[6]=lastpack/256;
                        Ram.SendData[7]=0x00;
                        Ram.SendData[8]=0x00;
                        Ram.SendData[9]=0x00;
                        Ram.UI_to_RTC();
                    }
                }
            }
        }
        else if(Ram.Receive(0x300)==0xcc)
        {
            Ram.Send(0xAA,0x300);
            if(projectCOName!=filename_transmit)
                machine_Pars.yield_current=0; //当前产量清零
            //写入当前已下载的花型名称
            projectCOName=filename_transmit;
            QFile file_project("/xrfile/PatternName.txt");
            if(file_project.open(QIODevice::ReadWrite))
            {
                file_project.resize(0);
                QTextStream out(&file_project);
                out<<filename_transmit;
                file_project.close();
                system("sync");
            }

            //保存参数
            para_operation.machine_par_save();
            //花型下载完成传递系统参数
            startToTransmitPara=0;
            b_sendPYF_Par=true;
            patterndowning=false;
            //下载完成后在主界面传递系统参数
            patterndownloaded=true;
            ui->progressBar->hide();
            ui->label_tips->show();
            ui->label_tips->setText(tr("花型 %1 下载完成！").arg(filename_transmit));
            para_operation.saveSystemLog(ui->label_tips->text(),QDateTime::currentDateTime());
            //下载完成删除uco文件
//            if(QFile::exists(filepath_DISK_transmit_uco))
//                QFile::remove(filepath_DISK_transmit_uco);
            system("sync");
            showinfo_DISK();
            downLoadPatn=1;
        }
        else if(Ram.Receive(0x300)==0xEE)
        {
            patterndowning=false;
            ui->progressBar->hide();
            ui->label_tips->show();
            ui->label_tips->setText(QString::fromUtf8("花型下载失败！"));
            para_operation.saveSystemLog(ui->label_tips->text(),QDateTime::currentDateTime());
            this->setEnabled(true);
        }
    }
    //花型下载完成 等待主界面参数传递完成
    if(patterndownloaded)
    {
        if(startToTransmitPara>=4)
        {
            patterndownloaded=false;
            this->setEnabled(true);
            Ram.to_Fileoperations_slot();
        }
    }
    //按键操作
    if(this->isActiveWindow()&&temp.length()>=1)
    {

        if(temp[0]==(char)2)
        {
            temp[0]=255;
            ui->copytoUSB_File__pb->click();
        }
        if(temp[0]==(char)3)
        {
            temp[0]=255;
            ui->copytodisk_File_pb->click();
        }
        if(temp[0]==(char)9)
        {
            temp[0]=255;
            ui->back_pb->click();
        }
        if(temp[0]==(char)10)
        {
            temp[0]=255;
            ui->pushButton_2->click();
        }
        if(temp[0]==(char)4)
        {
            temp[0]=255;
            ui->patterndownload_pb->click();
        }
        if(temp[0]==(char)34)//ok
        {
            temp[0]=255;
            if(tabletype==root)
            {
                ui->patterndownload_pb->click();
            }
            else if(tabletype==myusb)
            {
                ui->copytodisk_File_pb->click();
            }
        }
        if(temp[0]==(char)5||temp[0]==(char)27)
        {
            temp[0]=255;
            ui->filedelete_File_pb->click();
        }
        if(temp[0]==(char)7)
        {
            temp[0]=255;
            ui->flush_file_pb->click();
        }
        if(temp[0]==(char)28)
        {
            temp[0]=255;
            ui->back_pb->click();
        }
        if(temp[0]==(char)29)//左
        {
            temp[0]=255;
            tableChange(myusb);
        }
        if(temp[0]==(char)39)//右
        {
            temp[0]=255;
            tableChange(root);
        }
        if(temp[0]==(char)33)//上
        {
            temp[0]=255;
            int row=table->currentRow();
            if(row>0&&table->rowCount()>0)
            {
                row--;
                table->setCurrentCell(row,0);
            }
        }
        if(temp[0]==(char)35)//下
        {
            temp[0]=255;
            int row=table->currentRow();
            if(row>-1&&row<table->rowCount()-1)
            {
                row++;
                table->setCurrentCell(row,0);
            }
        }
    }
}

/*提取花型运行速度*/
bool FileOperations::fileSpeed_extract(QString filename)
{
    QFile file(filename);
    bool rst=false;
    if(file.open(QIODevice::ReadWrite))
    {
        int count=0;
        QByteArray ba;
        /**
             * --------------------提取速度数据-----------------------
       */
        {
            file.seek(0x8e);
            ba=file.read(4);
            int offset=0;
            int index=0;
            //起始地址
            offset=(((int)ba[1]&0xff)+((int)ba[0]&0xff))<<8;
            if((quint8)ba[1]!=0)
                offset=(((int)ba[1]&0xff)<<16)+(((int)ba[0]&0xff)<<8);
            offset+=0x40;
            file.seek(offset);
            ba=file.read(4);
            //步骤号为 FF FF FF FF时数据结束
            while(!(((int)ba[0]&0xff) == 0xff && ((int)ba[1]&0xff) == 0xff && ((int)ba[2]&0xff) == 0xff && ((int)ba[3]&0xff) == 0xff))
            {
                if(count>70)
                {
                    rst=false;
                    break;
                }
                speedinfo.step[index]=((quint8)ba[1])*256+(quint8)ba[0];
                ba=file.read(4);
                speedinfo.speed[index]=((quint8)ba[1])*256+(quint8)ba[0];
                ba=file.read(4);
                ba=file.read(4);
                speedinfo.ramp[index]=((quint8)ba[1])*256+(quint8)ba[0];
                index++;
                file.seek(offset+index*44);
                ba=file.read(4);
                count++;
            }
            file.close();
            if(count<=70)
                rst=true;
            else
            {
                MyMessageBox *mmb=new MyMessageBox(tr("提示"),tr("花型速度指令个数超限！"),0);
                mmb->exec();
            }
        }
    }
    return rst;
}

//提取循环参数
bool FileOperations::fileCirculation_extract(QString filename)
{
    QFile file(filename);
    bool rst=false;
    if(file.open(QIODevice::ReadWrite))
    {
        /**
     * --------------------提取循环数据-----------------------
*/
        {
            int count=0;
            file.seek(0x129);
            QByteArray ba=file.read(4);
            //起始地址
            int offset=(((int)ba[1]&0xff)+((int)ba[0]&0xff))<<8;
            if((quint8)ba[1]!=0)
                offset=(((int)ba[1]&0xff)<<16)+(((int)ba[0]&0xff)<<8);
            file.seek(offset+0x24);
            ba=file.read(4);
            offset+=(((quint8)ba[1])*256+(quint8)ba[0]);
//            offsetAddr[1]=offset;
            file.seek(offset);
            ba=file.read(4);
            int index=0;

            //步骤号为 00 00 00 00 时数据结束
            while(!(((int)ba[0]&0xff) == 0 && ((int)ba[1]&0xff) == 0 && ((int)ba[2]&0xff) == 0 && ((int)ba[3]&0xff) == 0))
            {
                if(count>50)
                {
                    rst=false;
                    break;
                }
                ecoinfo.step_s[index]=((quint8)ba[1])*256+(quint8)ba[0];
                ecoinfo.step_e[index]=((quint8)ba[3])*256+(quint8)ba[2];
                ba=file.read(4);
                ecoinfo.value[index]=((quint8)ba[1])*256+(quint8)ba[0];
                index++;
                file.seek(offset+index*36);
                ba=file.read(4);
                count++;
            }
            if(count<=50)
                rst=true;
           else {
                MyMessageBox *mmb=new MyMessageBox(tr("提示"),tr("花型循环指令个数超限！"),0);
                mmb->exec();
            }
        }
    }
    return rst;
}

/**提取花型压针位置**/
bool FileOperations::fileNeedle_extract(QString filename)
{
    QFile file(filename);
    bool rst=false;
    if(file.open(QIODevice::ReadWrite))
    {
        QByteArray data;
        int count=0;
        int offsetAddr[10];
        quint16 totalStep;
        int index=0;
        QList<StepperMotor> motorInfo;
        /**
         * 读取co总步骤数和总针数
         */
       {
            //总步骤数在supe文件下 起始地址存于0x129
            file.seek(0x129);
            QByteArray ba=file.read(2);
            int addr=0;
            addr=(((int)ba[1]&0xff)+((int)ba[0]&0xff))<<8;
            if((int)ba[1]!=0)
                addr=(((int)ba[1]&0xff)<<16)+(((int)ba[0]&0xff)<<8);
            file.seek(addr+0x04);

            data=file.read(4);
            //总步骤数
            totalStep = (((int)data[3]&0xff) << 24 | ((int)data[2]&0xff) << 16 | ((int)data[1]&0xff) << 8 | ((int)data[0]&0xff));
            for(int i=0;i<=totalStep;i++)
            {
                StepperMotor sm;
                sm.step=i;
                motorInfo.append(sm);
            }
            //写入总步骤数
            QFile file_sumStep(NeedleLocation);
            if(file_sumStep.open(QIODevice::ReadWrite))
            {
                QDataStream out(&file_sumStep);
                out.device()->seek(900);
                out<<totalStep;
                file_sumStep.close();
            }
        }

        /**
         * --------------------提取压针马达数据(MPP)-----------------------
   */
        {
            //读取MPP起始地址
            file.seek(0xad);
            data=file.read(4);
            //起始地址
            offsetAddr[2]=(((int)data[1]&0xff)+((int)data[0]&0xff))<<8;
            if((quint8)data[1]!=0)
                offsetAddr[2]=(((int)data[1]&0xff)<<16)+(((int)data[0]&0xff)<<8);
            file.seek(offsetAddr[2]+0x40+totalStep*4+4);
            int step=0;
            quint16 degree;
            data=file.read(2);
            degree=((quint8)data[0]&0xff)+(((quint8)data[1]&0xff)<<8);
            count=0;
            while (step<=totalStep) {
                if(count>1000)
                {
                    break;
                }
                if(degree==0x8000)
                {
                    step++;
                }
                else
                {
                    data=file.read(6);
                    MotorInfo mi;
                    mi.degree=degree;
                    mi.number=((quint8)data[0]&0xff)+(((quint8)data[1]&0xff)<<8);
                    mi.type=((quint8)data[2]&0xff)+(((quint8)data[3]&0xff)<<8);
                    mi.position=((quint8)data[4]&0xff)+(((quint8)data[5]&0xff)<<8);
                    if(mi.number<8)
                    {
                        motorInfo[step].listMotor.append(mi);
                        index++;
                    }
                }
                data=file.read(2);
                degree=((quint8)data[0]&0xff)+((((quint8)data[1]&0xff))<<8);
                count++;
            }
        }
        file.close();

        if(index<=200)
        {
            for(int i=0;i<200;i++)
            {
                machine_Pars.motor_degree[i]=0xffff;
                machine_Pars.motor_number[i]=0xffff;
                machine_Pars.motor_step[i]=0xffff;
                machine_Pars.motor_position[i]=0xffff;
            }
            int rowIndex=0;
            for(int i=0;i<motorInfo.length();i++)
            {
                for(int j=0;j<motorInfo[i].listMotor.length();j++)
                {
                    if(rowIndex<200)
                    {
                        machine_Pars.motor_step[rowIndex]=i;
                        machine_Pars.motor_degree[rowIndex]=motorInfo[i].listMotor[j].degree;
                        machine_Pars.motor_number[rowIndex]=motorInfo[i].listMotor[j].number;
                        qint16 pos=motorInfo[i].listMotor[j].position;
                        if(motorInfo[i].listMotor[j].type==8||motorInfo[i].listMotor[j].type==1)
                            pos*=-1;
                        machine_Pars.motor_position[rowIndex]=pos;
                        rowIndex++;
                    }
                }
            }

            //保存链条速度和循环到系统参数文件
            for(int i=0;i<70;i++)
            {
                machine_Pars.speed_step[i]=speedinfo.step[i];
                machine_Pars.speed_value[i]=speedinfo.speed[i];
                machine_Pars.speed_ramp[i]=speedinfo.ramp[i];
            }
            for(int i=0;i<50;i++)
            {
                machine_Pars.eco_stepstart[i]=ecoinfo.step_s[i];
                machine_Pars.eco_stepend[i]=ecoinfo.step_e[i];
                machine_Pars.eco_value[i]=ecoinfo.value[i];
            }
            para_operation.machine_par_save();
            rst=true;
        }
        else
        {
            para_operation.saveSystemLog("压针参数过多！",QDateTime::currentDateTime());
            MyMessageBox *box = new MyMessageBox("提示","花型压针参数过多！",0);
            box->exec();
        }

    }
    return rst;
}

/****高低位互换******/
int FileOperations::LH_deal(int valve)
{
    unsigned char L=(valve&0xff00)>>8; //07 00   00 07
    unsigned char H=valve&0x00ff;
    int  SPEED=(H<<8)|L;
    return SPEED;
}

int FileOperations::deal_Bit32(int valve)
{
    qint16 h16=valve>>16; //高16位
    qint8 h16_h8 = h16>>8;
    qint8 h16_L8 = h16 & 0xff;

    qint16 L16=valve&0xffff; //0001
    qint8 L16_h8 = L16>>8;
    qint8 L16_L8 = L16 & 0xff;

    int SPEED=(((h16_L8<<8)|h16_h8))|((L16_L8<<8 | L16_h8)<<16);
    return SPEED;
}

void  FileOperations::deleteFileSlot(bool flag)
{
    if(flag)
    {
        if(tabletype==root)
        {
            int rowindex=ui->diskdevide_twd->currentRow();
            if(rowindex>-1)
            {
                QString filename=QString("%1%2").arg(Path_loacl).arg(ui->diskdevide_twd->item(rowindex,1)->text());
                QFile::remove(filename);
                system("sync");
                showinfo_DISK();
            }
        }
        else if(tabletype==myusb)
        {
            int rowindex=ui->USBdevice_twd->currentRow();
            if(rowindex>-1)
            {
                QString filename=QString("%1%2").arg(Path_myusb).arg(ui->USBdevice_twd->item(rowindex,0)->text());
                QFile::remove(filename);
                system("sync");
            }
        }
    }
}

//退出U盘
void FileOperations::on_pushButton_2_clicked()
{
    system("cd /media");
    system("umount /media/myusb");
    system("rm -rf /media/myusb");
}

void FileOperations::usbCellClick(int row, int col)
{
    if(tabletype!=myusb)
    {
        tableChange(myusb);
    }
}

void FileOperations::diskCellClick(int row, int col)
{
    if(tabletype!=root)
    {
        tableChange(root);
    }
}
