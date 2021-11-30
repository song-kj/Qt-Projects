#include "qluoansetting.h"
#include "ui_qluoansetting.h"
#include <QDebug>
#include <QMessageBox>

extern QByteArray temp;
extern bool global_btrans_luoan;
extern QString global_pattern_name;

QLuoAnSetting::QLuoAnSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QLuoAnSetting)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);//当关闭本窗口时此函数会自动释放内存
    this->setWindowFlags(Qt::FramelessWindowHint);
    Ram.to_SettingMain_slot();
    setall=false;
    max_row = 0;
    max_index = 0;
    focused_col=-1;
    focused_row=-1;
    for(int i = 0; i < 8; i++)
        MotorInfo[i].clear();
    for(int i=0;i<100;i++)
    {
        for(int j=0;j<18;j++)
        {
            motorData[i][j]=0xffff;
        }
    }

    ReadPara();
    keyboard_timer = new QTimer(this);
    connect(keyboard_timer,SIGNAL(timeout()),this,SLOT(InputEvent()));

    startDownload=false;
}

QLuoAnSetting::~QLuoAnSetting()
{
    delete ui;
}

int QLuoAnSetting::ReadIni()
{
    QString log_name = QString(LUO_AN_LOG);
    QSettings luoan_log(log_name,QSettings::IniFormat);
    int table_row = luoan_log.value("Luoan/row",MAX_STEP).toInt();
    return table_row;
}

void QLuoAnSetting::WriteIni(const int &row)
{
    QString log_name = QString(LUO_AN_LOG);
    QSettings luoan_log(log_name,QSettings::IniFormat);
    luoan_log.beginGroup("Luoan");
        luoan_log.setValue("row",row);
    luoan_log.endGroup();
}

void QLuoAnSetting::InitTable()
{
    ui->tableWidget->setColumnCount(MAX_COL);
    ui->tableWidget->setRowCount(max_row);
    //设置表格行标题
    QStringList headerLabels;
    headerLabels<<"起始步"<<"结束步";
    for(int i=0;i<16;i++)
    {
        if(i%2 == 0)
        {
            headerLabels<<QString("Elan")+QString::number(i/2+1)+QString("_B");
        }
        else
        {
            headerLabels<<QString("Elan")+QString::number(i/2+1)+QString("_E");
        }
    }
    for(int i=0;i<headerLabels.length();i++)
    {
        ui->tableWidget->horizontalHeader()->resizeSection(i,80); //设置列的宽度为60
        ui->tableWidget->setRowHeight(i,40);//设置列高
    }
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40); //设置行距
    //ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget->setShowGrid(true); //设置不显示格子线
    ui->tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
    //ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection); //可多选（Ctrl、Shift、 Ctrl+A都可以）
    ui->tableWidget->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->tableWidget->setStyleSheet("QScrollBar:vertical{width: 28px;}"
                                   "QScrollBar:horizontal{height:28px;}"
                                   "QTableWidget{"
                                   "selection-background-color:lightblue;"
                                   "color:black;"
                                   "gridline-color: black;"
                                   "selection-color:red;"
                                   "font-size:20px;}");
//    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;font-size:18px;border:1px solid white;}"); //设置表头背景色

    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(cellclick(int,int)));

    for(int i=0;i<100;i++)
    {
        for(int j=0;j<18;j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setText("");
            //item->setFlags(Qt::NoItemFlags);
            ui->tableWidget->setItem(i,j,item);
        }
    }
}

void QLuoAnSetting::ShowDialog()
{
    this->show();
    keyboard_timer->start(10);
}

void QLuoAnSetting::SetPara(const QString &str_para)
{
    if(focused_row>-1&&focused_col>1)
    {
        QString str=ui->tableWidget->item(focused_row,focused_col)->text();
        if(str_para=="")
        {
            str="0";
        }
        else
        {
            if(str=="0")
                str=str_para;
            else
                str+=str_para;
        }
        if(str.toInt()<4096)
        {
            motorData[focused_row][focused_col]=str.toInt();
            ui->tableWidget->item(focused_row,focused_col)->setText(str);
        }
    }
}

void QLuoAnSetting::InputEvent()
{
    if(startDownload)
    {
        count++;
        if(count>=500)
        {
            startDownload=false;
            ui->download_btn->setEnabled(true);
            ui->save_btn->setEnabled(true);
            ui->return_btn->setEnabled(true);
            ui->setall_btn->setEnabled(true);
            ui->jia10_btn->setEnabled(true);
            ui->jia1_btn->setEnabled(true);
            ui->jian10_btn->setEnabled(true);
            ui->jian1_btn->setEnabled(true);
            ui->label_tips->setText("下载超时！");
            return;
        }
        if(Ram.Receive(0x300)==0x55)
        {
            if(Ram.Receive(0x200)==0x50)
            {
                if(Ram.Receive(0x201)==0x52)
                {
                    if(Ram.Receive(0x202)==0x00)
                    {
                       Ram.Send(0xAA,0x300);
                       count=0;
                       for(int i=0;i<2000;i++)
                       {

                       }
                       QFile file_transmit(LUO_AN_PARA);
                       int length=file_transmit.size();
                       int pack=length/2560;
                       file_transmit.open(QIODevice::ReadWrite);
                       QDataStream in(&file_transmit);
                       int baoshu=Ram.Receive(0x203)+Ram.Receive(0x204)*256;
                       int lastpack=2560;
                       if(baoshu==pack-1)
                       {
                          lastpack= length-(pack-1)*2560;
                       }

                       in.device()->seek(baoshu*2560);
                       for(int j=0;j<lastpack;j++)
                       {
                           quint8 s;
                           in>>s;
                           Ram.Send(s,0x500+j);
                       }
                       Ram.SendData[0]=0x50;
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
        else if((unsigned char)Ram.Receive(0x300)==0xcc)
        {
            startDownload=false;
            ui->label_tips->setText("下载完成！");
            ui->download_btn->setEnabled(true);
            ui->save_btn->setEnabled(true);
            ui->return_btn->setEnabled(true);
            ui->setall_btn->setEnabled(true);
            ui->jia10_btn->setEnabled(true);
            ui->jia1_btn->setEnabled(true);
            ui->jian10_btn->setEnabled(true);
            ui->jian1_btn->setEnabled(true);
            count=0;
        }
        else if((unsigned char)Ram.Receive(0x300)==0xEE)
        {
            startDownload=false;
            ui->label_tips->setText("下载失败！");
            ui->download_btn->setEnabled(true);
            ui->save_btn->setEnabled(true);
            ui->return_btn->setEnabled(true);
            ui->setall_btn->setEnabled(true);
            ui->jia10_btn->setEnabled(true);
            ui->jia1_btn->setEnabled(true);
            ui->jian10_btn->setEnabled(true);
            ui->jian1_btn->setEnabled(true);
            count=0;
        }
    }
    if(temp.length()>=1&&this->isActiveWindow())
    {
        if(temp[0]==(char)12)
        {
            temp[0]=255;
            SetPara("1");
        }
        else if(temp[0]==(char)13)
        {
            temp[0]=255;
            SetPara("4");
        }
        else if(temp[0]==(char)14)
        {
            temp[0]=255;
            SetPara("7");
        }
        else if(temp[0]==(char)17)
        {
            temp[0]=255;
            SetPara("2");
        }
        else if(temp[0]==(char)18)
        {
            temp[0]=255;
            SetPara("5");
        }
        else if(temp[0]==(char)19)
        {
            temp[0]=255;
            SetPara("8");
        }
        else if(temp[0]==(char)20)
        {
            temp[0]=255;
            SetPara("0");
        }
        else if(temp[0]==(char)22)
        {
            temp[0]=255;
            SetPara("3");
        }
        else if(temp[0]==(char)23)
        {
            temp[0]=255;
            SetPara("6");
        }
        else if(temp[0]==(char)24)
        {
            temp[0]=255;
            SetPara("9");
        }
        else if(temp[0]==(char)27)
        {
            temp[0]=255;
            SetPara("");
        }
        else if (temp[0]==(char)1) {
            temp[0]=255;
            ui->setall_btn->click();
        }
        else if (temp[0]==(char)2) {
            temp[0]=255;
            on_jia1_btn_clicked();
        }
        else if (temp[0]==(char)3) {
            temp[0]=255;
            on_jian1_btn_clicked();
        }
        else if (temp[0]==(char)4) {
            temp[0]=255;
            on_jia10_btn_clicked();
        }
        else if (temp[0]==(char)5) {
            temp[0]=255;
            on_jian10_btn_clicked();
        }
        else if (temp[0]==(char)7) {
            temp[0]=255;
            ui->setrow_btn->click();
        }
        else if (temp[0]==(char)8) {
            temp[0]=255;
            on_download_btn_clicked();
        }
        else if (temp[0]==(char)9) {
            temp[0]=255;
            on_save_btn_clicked();
        }
        else if (temp[0]==(char)10) {
            temp[0]=255;
            on_return_btn_clicked();
        }
        else
        {
            temp[0]=255;
        }
    }
}

void QLuoAnSetting::on_return_btn_clicked()
{
    keyboard_timer->stop();
    Ram.to_PatternWoven_slot();
    this->close();
}

void QLuoAnSetting::SavePara()
{
    ui->label_tips->setText(tr(""));
    QFile::remove(QString(LUO_AN_PARA));
    QFile file(QString(LUO_AN_PARA));
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream in_stream(&file);
        for(int i = 0; i < MAX_STEP; i++)
        {
            for(int j = 0; j < MAX_COL; j++)
            {
                in_stream<<motorData[i][j];
            }
        }
        file.flush();
        file.close();
        system("sync");
        ui->label_tips->setText(tr("保存成功！"));
    }
}

void QLuoAnSetting::on_save_btn_clicked()
{
    SavePara();
}

void QLuoAnSetting::ReadPara()
{
    int row = ReadIni();
    max_row=row;
    QFile file(QString(LUO_AN_PARA));

    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in_stream(&file); 
        for(int i = 0; i < MAX_STEP; i++)
        {
            for(int j = 0; j < MAX_COL; j++)
            {
                quint16 data;
                in_stream>>data;
                motorData[i][j]=data;
//                if(data != 0xFFFF)
//                    table_luoan->setData(table_luoan->index(i,j),data);
//                else
//                    table_luoan->setData(table_luoan->index(i,j),"");

//                table_luoan->item(i,j)->setTextAlignment(Qt::AlignCenter);
            }
        }
        file.close();
        InitTable();
        for(int i = 0; i < MAX_STEP; i++)
        {
            if(i>=max_row)
                break;
            for(int j = 0; j < MAX_COL; j++)
            {
                if(i<max_row)
                {
                    if(motorData[i][j]!=0xffff)
                    {
                        ui->tableWidget->item(i,j)->setText(QString::number(motorData[i][j]));
                    }
                    else
                    {
                        ui->tableWidget->item(i,j)->setText("");
                    }
                }
                else
                {
                   ui->tableWidget->item(i,j)->setText("");
                }
            }
        }
    }
}

void QLuoAnSetting::read_luoan_file(const QString &name)
{
    QFile file(name);
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in.device()->seek(0x6f);
        quint16 start_addr;
        in>>start_addr; //起始地址占2个字节，高字节在前，低字节在后
        quint16 delt_addr = start_addr+0x20;
        in.device()->seek(delt_addr);//从delt_addr开始存放偏移量

        //获取8个电机的偏移量，每个偏移量占4个字节，低字节在前，高字节在后
        quint8 mot[32];
        for(int i = 0; i<32; i++)
        {
            in>>mot[i];
        }

        quint32 delt_mot[8];
        memcpy(delt_mot,mot,32);//每个电机的偏移量

        for(int i = 0; i < 8; i++)
        {
            in.device()->seek(start_addr+delt_mot[i]);
            quint32 mot;
            in>>mot;
            in.device()->seek(start_addr+delt_mot[i]);
            if(mot>0) //如果mot为0，该电机没有数据
            {
                int block =1;
                if(i!=7)
                {
                    block = (delt_mot[i+1]-delt_mot[i]-4)/100;//block的数量，每个block信息占100个字节
                    if(max_row<block)
                    {
                        max_row = block;
                        max_index = i;
                    }
                }
                else //最后一个电机block数量需要单独计算
                {
                    int mot = 1;
                    int pos = in.device()->pos();
                    while(mot)
                    {
                        in.device()->seek(pos+100*block);
                        quint32 mot;
                        in>>mot;
                        if(mot==0)
                            break;
                        block++;
                    }
                    if(max_row<block)
                    {
                        max_row = block;
                        max_index = i;
                    }
                    in.device()->seek(start_addr+delt_mot[7]);
                }

                for(int j = 0; j < block; j++)
                {
                    int pos = in.device()->pos()+32;
                    in.device()->seek(pos);
                    quint8 start_step_l,start_step_h,end_step_l,end_step_h;
                    in>>start_step_l>>start_step_h>>end_step_l>>end_step_h;
                    quint16 start_step = start_step_h<<8 | start_step_l; //起始步
                    quint16 end_step = end_step_h<<8 | end_step_l;//结束步

                    quint8 mot_start_l,mot_start_h,mot_end_l,mot_end_h;
                    in>>mot_start_l>>mot_start_h>>mot_start_l>>mot_start_h
                            >>mot_end_l>>mot_end_h>>mot_end_l>>mot_end_h;
                    quint16 mot_start = mot_start_h<<8|mot_start_l; //电机起始值
                    quint16 mot_end = mot_end_h<<8 | mot_end_l;//电机终止值
                    int pos1 = in.device()->pos()+56;//电机起始和终止值总共占64个字节，只用到了前8个字节。
                    in.device()->seek(pos1);
                    Mot mot_info;
                    mot_info.startStep = start_step;
                    mot_info.endStep = end_step;
                    mot_info.motStart = mot_start;
                    mot_info.motEnd = mot_end;
                    MotorInfo[i].insert(std::pair<int,Mot>(j,mot_info));
                }
            }
        }
        file.close();
        WriteIni(max_row);
    }

    display_table();
    SavePara();
}

void QLuoAnSetting::display_table()
{
    InitTable();
    for(int i = 0; i<max_row; i++)
    {
        ui->tableWidget->item(i,0)->setText(QString::number(MotorInfo[max_index].at(i).startStep));

        ui->tableWidget->item(i,1)->setText(QString::number(MotorInfo[max_index].at(i).endStep));

        ui->tableWidget->item(i,2*(max_index+1))->setText(QString::number(MotorInfo[max_index].at(i).motStart));

        ui->tableWidget->item(i,2*(max_index+1)+1)->setText(QString::number(MotorInfo[max_index].at(i).motEnd));
    }
    for(int j = 0; j<8; j++)
    {
        if(j != max_index)
        {
            for(int i = 0; i<max_row; i++)
            {
                for(int k = 0; k<MotorInfo[j].size(); k++)
                {
                    if(MotorInfo[j].at(k).endStep==ui->tableWidget->item(i,1)->text().toInt())
                    {
                        ui->tableWidget->item(i,2*(j+1))->setText(QString::number(MotorInfo[j].at(k).motStart));
                        ui->tableWidget->item(i,2*(j+1)+1)->setText(QString::number(MotorInfo[j].at(k).motEnd));
                        break;
                    }
                    else
                    {
                        ui->tableWidget->item(i,2*(j+1))->setText("");
                        ui->tableWidget->item(i,2*(j+1)+1)->setText("");
                    }
                }
            }
        }
    }
    for(int i = 0; i < MAX_STEP; i++)
    {
        if(i>=max_row)
            break;
        for(int j = 0; j < MAX_COL; j++)
        {
            if(i<max_row)
            {
                if(ui->tableWidget->item(i,j)->text() != "")
                {
                    motorData[i][j]=ui->tableWidget->item(i,j)->text().toInt();
                }
                else
                {
                    motorData[i][j]=0xFFFF;
                }
            }
            else
            {
                motorData[i][j]=0xFFFF;
            }
        }
    }
}

void QLuoAnSetting::on_setall_btn_clicked()
{
    setrow=false;
    ui->setrow_btn->setChecked(false);
    setall=ui->setall_btn->isChecked();
}

void QLuoAnSetting::on_jia1_btn_clicked()
{
    if(setall)
    {
        for(int i = 0; i < MAX_STEP; i++)
        {
            if(i>=max_row)
                break;
            for(int j = 2; j < MAX_COL; j++)
            {
                //qDebug()<<i<<j;
                if(i<max_row)
                {
                    if(motorData[i][j]!=0xffff)
                    {
                        motorData[i][j]++;
                        if(motorData[i][j]>4095)
                            motorData[i][j]=4095;
                        ui->tableWidget->item(i,j)->setText(QString::number(motorData[i][j]));
                    }
                }
            }
        }
    }
    else if(setrow)
    {
        if(focused_row>-1)
        {
            for(int j = 2; j < MAX_COL; j++)
            {
                if(motorData[focused_row][j]!=0xffff)
                {
                    motorData[focused_row][j]++;
                    if(motorData[focused_row][j]>4095)
                        motorData[focused_row][j]=4095;
                    ui->tableWidget->item(focused_row,j)->setText(QString::number(motorData[focused_row][j]));
                }
            }
        }
    }
    else if(setcol)
    {
        if(focused_col>1)
        {
            for(int i=0;i<max_row;i++)
            {
                motorData[i][focused_col]++;
                if(motorData[i][focused_col]>4095)
                    motorData[i][focused_col]=0;
                ui->tableWidget->item(i,focused_col)->setText(QString::number(motorData[i][focused_col]));
            }
        }
    }
    else
    {
        if(focused_col>1&&focused_row!=-1)
        {
            if(motorData[focused_row][focused_col]!=0xffff)
            {
                motorData[focused_row][focused_col]++;
                if(motorData[focused_row][focused_col]>4095)
                    motorData[focused_row][focused_col]=4095;
                ui->tableWidget->item(focused_row,focused_col)->setText(QString::number(motorData[focused_row][focused_col]));
            }
        }
    }
}

void QLuoAnSetting::on_jian1_btn_clicked()
{
    if(setall)
    {
        for(int i = 0; i < MAX_STEP; i++)
        {
            if(i>=max_row)
                break;
            for(int j = 2; j < MAX_COL; j++)
            {
                //qDebug()<<i<<j;
                if(i<max_row)
                {
                    if(motorData[i][j]!=0xffff)
                    {
                        motorData[i][j]--;
                        if(motorData[i][j]>4095)
                            motorData[i][j]=0;
                        ui->tableWidget->item(i,j)->setText(QString::number(motorData[i][j]));
                    }
                }
            }
        }
    }
    else if(setrow)
    {
        if(focused_row>-1)
        {
            for(int j = 2; j < MAX_COL; j++)
            {
                if(focused_row<max_row)
                {
                    if(motorData[focused_row][j]!=0xffff)
                    {
                        motorData[focused_row][j]--;
                        if(motorData[focused_row][j]>4095)
                            motorData[focused_row][j]=0;
                        ui->tableWidget->item(focused_row,j)->setText(QString::number(motorData[focused_row][j]));
                    }
                }
            }
        }
    }
    else if(setcol)
    {
        if(focused_col>1)
        {
            for(int i=0;i<max_row;i++)
            {
                motorData[i][focused_col]--;
                if(motorData[i][focused_col]>4095)
                    motorData[i][focused_col]=0;
                ui->tableWidget->item(i,focused_col)->setText(QString::number(motorData[i][focused_col]));
            }
        }
    }
    else
    {
        if(focused_col>1&&focused_row!=-1)
        {
            if(motorData[focused_row][focused_col]!=0xffff)
            {
                motorData[focused_row][focused_col]--;
                if(motorData[focused_row][focused_col]>4095)
                    motorData[focused_row][focused_col]=0;
                ui->tableWidget->item(focused_row,focused_col)->setText(QString::number(motorData[focused_row][focused_col]));
            }
        }
    }
}

void QLuoAnSetting::on_jia10_btn_clicked()
{
    if(setall)
    {
        for(int i = 0; i < MAX_STEP; i++)
        {
            if(i>=max_row)
                break;
            for(int j = 2; j < MAX_COL; j++)
            {
                //qDebug()<<i<<j;
                if(i<max_row)
                {
                    if(motorData[i][j]!=0xffff)
                    {
                        motorData[i][j]+=10;
                        if(motorData[i][j]>4095)
                            motorData[i][j]=4095;
                        ui->tableWidget->item(i,j)->setText(QString::number(motorData[i][j]));
                    }
                }
            }
        }
    }
    else if(setrow)
    {
        if(focused_row>-1)
        {
            for(int j = 2; j < MAX_COL; j++)
            {
                if(motorData[focused_row][j]!=0xffff)
                {
                    motorData[focused_row][j]+=10;
                    if(motorData[focused_row][j]>4095)
                        motorData[focused_row][j]=4095;
                    ui->tableWidget->item(focused_row,j)->setText(QString::number(motorData[focused_row][j]));
                }
            }
        }
    }
    else if(setcol)
        {
            if(focused_col>1)
            {
                for(int i=0;i<max_row;i++)
                {
                    motorData[i][focused_col]+=10;
                    if(motorData[i][focused_col]>4095)
                        motorData[i][focused_col]=0;
                    ui->tableWidget->item(i,focused_col)->setText(QString::number(motorData[i][focused_col]));
                }
            }
        }
    else
    {
        if(focused_col>1&&focused_row!=-1)
        {
            if(motorData[focused_row][focused_col]!=0xffff)
            {
                motorData[focused_row][focused_col]+=10;
                if(motorData[focused_row][focused_col]>4095)
                    motorData[focused_row][focused_col]=4095;
                ui->tableWidget->item(focused_row,focused_col)->setText(QString::number(motorData[focused_row][focused_col]));
            }
        }
    }
}

void QLuoAnSetting::on_jian10_btn_clicked()
{
    if(setall)
    {
        for(int i = 0; i < MAX_STEP; i++)
        {
            if(i>=max_row)
                break;
            for(int j = 2; j < MAX_COL; j++)
            {
                //qDebug()<<i<<j;
                if(i<max_row)
                {
                    if(motorData[i][j]!=0xffff)
                    {
                        motorData[i][j]-=10;
                        if(motorData[i][j]>4095)
                            motorData[i][j]=0;
                        ui->tableWidget->item(i,j)->setText(QString::number(motorData[i][j]));
                    }
                }
            }
        }
    }
    else if(setrow)
    {
        if(focused_row>-1)
        {
            for(int j = 2; j < MAX_COL; j++)
            {
                if(focused_row<max_row)
                {
                    if(motorData[focused_row][j]!=0xffff)
                    {
                        motorData[focused_row][j]-=10;
                        if(motorData[focused_row][j]>4095)
                            motorData[focused_row][j]=0;
                        ui->tableWidget->item(focused_row,j)->setText(QString::number(motorData[focused_row][j]));
                    }
                }
            }
        }
    }
    else if(setcol)
        {
            if(focused_col>1)
            {
                for(int i=0;i<max_row;i++)
                {
                    motorData[i][focused_col]-=10;
                    if(motorData[i][focused_col]>4095)
                        motorData[i][focused_col]=0;
                    ui->tableWidget->item(i,focused_col)->setText(QString::number(motorData[i][focused_col]));
                }
            }
        }
    else
    {
        if(focused_col>1&&focused_row!=-1)
        {
            if(motorData[focused_row][focused_col]!=0xffff)
            {
                motorData[focused_row][focused_col]-=10;
                if(motorData[focused_row][focused_col]>4095)
                    motorData[focused_row][focused_col]=0;
                ui->tableWidget->item(focused_row,focused_col)->setText(QString::number(motorData[focused_row][focused_col]));
            }
        }
    }
}

void QLuoAnSetting::on_download_btn_clicked()
{
    ui->label_tips->setText("参数下载中...");
    ui->save_btn->setEnabled(false);
    ui->return_btn->setEnabled(false);
    ui->download_btn->setEnabled(false);
    ui->setall_btn->setEnabled(false);
    ui->jia10_btn->setEnabled(false);
    ui->jia1_btn->setEnabled(false);
    ui->jian10_btn->setEnabled(false);
    ui->jian1_btn->setEnabled(false);
    QFile file_transmit(LUO_AN_PARA);
    int length=file_transmit.size();
    int pack=length/2560;
    if((length%2560)!=0)
       pack=length/2560+1;
    Ram.Send(0xAA,0x300);
    for(int i=0;i<2000;i++)
    {

    }
    Ram.SendData[0]=0x50;
    Ram.SendData[1]=0x50;
    Ram.SendData[2]=0x00;
    Ram.SendData[3]=pack%256;//总包数L
    Ram.SendData[4]=pack/256;//总包数H
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    count=0;
    startDownload=true;
}

void QLuoAnSetting::cellclick(int i, int j)
{
    focused_row=i;
    focused_col=j;
}

void QLuoAnSetting::on_setrow_btn_clicked()
{
    setall=false;
    setcol=false;
    ui->setcol_btn->setChecked(false);
    ui->setall_btn->setChecked(false);
    setrow=ui->setrow_btn->isChecked();
}

void QLuoAnSetting::on_setcol_btn_clicked()
{
    setall=false;
    setrow=false;
    ui->setall_btn->setChecked(false);
    ui->setrow_btn->setChecked(false);
    setcol=ui->setcol_btn->isChecked();
}
