#include "qluoanangle.h"
#include "ui_qluoanangle.h"

#define LUO_AN_PARA "/xrfile/Parameter_LuoanD.er"        //裸氨参数保存文件路径
#define MAX_STEP 100                     //最大步骤数也即表格最大行数100
#define MAX_COL 17                     //列数17：
extern QByteArray temp;
extern QString global_pattern_name;
extern bool global_btrans_luoanD;

QLuoAnAngle::QLuoAnAngle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QLuoAnAngle)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    InitTable();
    Ram.to_SettingMain_slot();
    ReadPara();
    keyboard_timer = new QTimer(this);
    connect(keyboard_timer,SIGNAL(timeout()),this,SLOT(InputEvent()));
    keyboard_timer->start(10);
}

void QLuoAnAngle::InitTable()
{
    ui->tableWidget->setColumnCount(MAX_COL);
    //设置表格行标题
    QStringList headerLabels;
    headerLabels<<"步段";
    for(int i=0;i<8;i++)
    {
            headerLabels<<QString("Elan_")+QString::number(i+1);
            headerLabels<<QString("State_")+QString::number(i+1);
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

    ui->tableWidget->setRowCount(MAX_STEP);
    for(int i=0;i<MAX_STEP;i++)
    {
        for(int j=0;j<MAX_COL;j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setText("");
            //item->setFlags(Qt::NoItemFlags);
            ui->tableWidget->setItem(i,j,item);
            data[i][j]=0xffff;
        }
    }
    focused_col=-1;
    focused_row=-1;
    downReceive=false;
    startDownload=false;
}

void QLuoAnAngle::display_table()
{
    ui->tableWidget->setRowCount(max_row);
    for(int i=0;i<max_row;i++)
    {
        ui->tableWidget->item(i,0)->setText(QString::number(data[i][0]));
        for(int j=1;j<MAX_COL;j++)
        {
            if(data[i][j]!=0xffff)
            {
                if((j-1)%2==0)
                {
                    ui->tableWidget->item(i,j)->setText(QString::number(data[i][j]));
                }
                else
                {
                    ui->tableWidget->item(i,j)->setText(data[i][j]==0?"Out":"In");
                }
            }
        }
    }
}

void QLuoAnAngle::ReadPara()
{
    QFile file(QString(LUO_AN_PARA));
    if(file.open(QIODevice::ReadOnly))
    {
        max_row=0;
        QDataStream in_stream(&file);
        for(int i = 0; i < MAX_STEP; i++)
        {
            for(int j = 0; j < MAX_COL; j++)
            {
                in_stream>>data[i][j];
            }
            if(data[i][0]!=0xffff)
                max_row++;
        }
        file.close();
        display_table();
    }
}

void QLuoAnAngle::read_luoan_file(const QString &name)
{
    QFile file(name);
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        //读取总步骤数
        quint16 totalStep=0;
        qint32 start_addr;
        in.device()->seek(0x129);
        quint8 headerPos[2];
        in>>headerPos[0];
        in>>headerPos[1];
        if(headerPos[1]==0x00)
        {
            start_addr=headerPos[0]*256+headerPos[1];
        }
        else
        {
            start_addr=headerPos[0]*256+headerPos[1]*256*256;
        }
        in.device()->seek(start_addr+4);
        in>>totalStep;
        totalStep=deal_Bit16(totalStep);
        in.device()->seek(0x8e);
        in>>headerPos[0];
        in>>headerPos[1];
        if(headerPos[1]==0x00)
        {
            start_addr=headerPos[0]*256+headerPos[1];
        }
        else
        {
            start_addr=headerPos[0]*256+headerPos[1]*256*256;
        }
        quint16 delt_addr = (quint16)(start_addr+0x28);
        in.device()->seek(delt_addr);//从delt_addr开始存放偏移量
        quint32 pos=(quint32)start_addr;
        quint8 byte;
        for(int i=0;i<4;i++)
        {
            in>>byte;
            pos+=byte*pow(256,i);
        }
        in.device()->seek(pos);
        //获取8个电机的偏移量，每个偏移量占4个字节，低字节在前，高字节在后
        quint8 mot[totalStep*4];
        for(int i = 0; i<totalStep*4; i++)
        {
            in>>mot[i];
        }
        quint32 delt_mot[totalStep];//每步偏移量
        memcpy(delt_mot,mot,totalStep*4);
        quint16 degree;
        quint8 par[4];
        bool addrow=false;
        max_row=0;
        int index=-1;
        quint32 act;
        for(int i = 0; i < totalStep; i++)
        {
            in.device()->seek(pos+delt_mot[i]);
            in>>degree;
            degree=deal_Bit16(degree);
            addrow=false;
            while(degree!=0x8000) //非结束标志
            {
                for(int j=0;j<4;j++)
                {
                    in>>par[j];
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
                    in>>act;
                in>>degree;
                degree=deal_Bit16(degree);
            }
        }
        file.close();
        SavePara();
        display_table();
    }
}

void QLuoAnAngle::SavePara()
{
    QFile::remove(QString(LUO_AN_PARA));
    QFile file(QString(LUO_AN_PARA));
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream in_stream(&file);
        for(int i = 0; i < MAX_STEP; i++)
        {
            for(int j = 0; j < MAX_COL; j++)
            {
                in_stream<<data[i][j];
            }
        }
        file.flush();
        file.close();
        system("sync");
    }
}

void QLuoAnAngle::SetPara(const QString &str_para)
{
    if(focused_row>-1&&focused_col%2==1)
    {
        if(data[focused_row][focused_col]==0xffff)
            return;
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
        if(str.toInt()<=360)
        {
            data[focused_row][focused_col]=str.toInt();
            ui->tableWidget->item(focused_row,focused_col)->setText(str);
        }
    }
}

quint16 QLuoAnAngle::deal_Bit16(quint16 value)
{
    quint8 h16_h8 = value>>8;
    quint8 h16_L8 = value & 0xff;

    quint16 SPEED=(((h16_L8<<8)|h16_h8));
    return SPEED;
}

void QLuoAnAngle::cellclick(int i, int j)
{
    focused_row=i;
    focused_col=j;
    if(data[i][j]!=0xffff)
    {
        if(j>0&&j%2==0)
        {
            if(data[i][j]==0)
            {
                data[i][j]=1;
                ui->tableWidget->item(i,j)->setText("In");
            }
            else
            {
                data[i][j]=0;
                ui->tableWidget->item(i,j)->setText("Out");
            }
        }
    }
}

void QLuoAnAngle::InputEvent()
{
    if(startDownload)
    {
        count++;
        if(count>=100)
        {
            downReceive=false;
            ui->download_btn->setEnabled(true);
            ui->save_btn->setEnabled(true);
            ui->return_btn->setEnabled(true);
            ui->label_tips->setText("下载超时！");
            startDownload=false;
            return;
        }
        if(Ram.Receive(0x300)==0x55)
        {
            if(Ram.Receive(0x200)==0x60)
            {
                if(Ram.Receive(0x201)==0x52)
                {
                    if(Ram.Receive(0x202)==0x00)
                    {
                       Ram.Send(0xAA,0x300);
                       for(int i=0;i<2000;i++)
                       {

                       }
                       QFile file_transmit(LUO_AN_PARA);
                       int length=file_transmit.size();
                       file_transmit.open(QIODevice::ReadWrite);
                       QDataStream in(&file_transmit);
                       int pack=length/2560;
                       if((length%2560)!=0)
                          pack=length/2560+1;
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
                       Ram.SendData[0]=0x60;
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
            ui->download_btn->setEnabled(true);
            ui->save_btn->setEnabled(true);
            ui->return_btn->setEnabled(true);
            ui->label_tips->setText("下载完成！");
            startDownload=true;
            count=0;
        }
        else if((unsigned char)Ram.Receive(0x300)==0xEE)
        {
            ui->download_btn->setEnabled(true);
            ui->save_btn->setEnabled(true);
            ui->return_btn->setEnabled(true);
            ui->label_tips->setText("下载失败！");
            startDownload=true;
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

QLuoAnAngle::~QLuoAnAngle()
{
    delete ui;
}

void QLuoAnAngle::on_download_btn_clicked()
{
    ui->label_tips->setText("参数下载中...");
    ui->save_btn->setEnabled(false);
    ui->return_btn->setEnabled(false);
    ui->download_btn->setEnabled(false);
    QFile file_transmit(LUO_AN_PARA);
    int length=file_transmit.size();
    file_transmit.open(QIODevice::ReadWrite);
    int pack=length/2560;
    if((length%2560)!=0)
       pack=length/2560+1;
    Ram.Send(0xAA,0x300);
    for(int i=0;i<2000;i++)
    {

    }
    Ram.SendData[0]=0x60;
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

void QLuoAnAngle::on_save_btn_clicked()
{
    SavePara();
}

void QLuoAnAngle::on_return_btn_clicked()
{
    keyboard_timer->stop();
    Ram.to_PatternWoven_slot();
    close();
}

void QLuoAnAngle::transParameter()
{
    QFile file_transmit(LUO_AN_PARA);
    int length=file_transmit.size();
    file_transmit.open(QIODevice::ReadWrite);
    QDataStream in(&file_transmit);
    int pack=length/2560;
    if((length%2560)!=0)
       pack=length/2560+1;
    Ram.Send(0xAA,0x300);
    for(int i=0;i<2000;i++)
    {

    }
    Ram.SendData[0]=0x60;
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

    int ii=0;
    for(ii=0;ii<999999999;ii++)
    {
        if(Ram.Receive(0x300)==0x55)
        {
            if(Ram.Receive(0x200)==0x60)
            {
                if(Ram.Receive(0x201)==0x52)
                {
                    if(Ram.Receive(0x202)==0x00)
                    {
                       Ram.Send(0xAA,0x300);
                       for(int i=0;i<2000;i++)
                       {

                       }
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
                           ii=0;
                       }
                       Ram.SendData[0]=0x60;
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
        else if((unsigned char)Ram.Receive(0x300)==0xcc
                &&(unsigned char)Ram.Receive(0x200)==0x60
                &&(unsigned char)Ram.Receive(0x201)==0x52
                &&(unsigned char)Ram.Receive(0x202)==0x00
                &&(pack-1)==((unsigned char)Ram.Receive(0x203)+(unsigned char)Ram.Receive(0x204)*256))
        {
            ui->download_btn->setText("下\n载\n完\n成");
            ui->download_btn->setStyleSheet("border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 0, 0);");
            downReceive=true;
            count=0;
           break;
        }
        else if((unsigned char)Ram.Receive(0x300)==0xEE)
        {
            ui->download_btn->setStyleSheet("border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 0, 0);");
            ui->download_btn->setText("下\n载\n失\n败");
            downReceive=true;
            count=0;
            break;
        }
    }
    if(ii==999999999)
    {
        ui->download_btn->setStyleSheet("border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 0, 0);");
        ui->download_btn->setText("传\n输\n超\n时");
        downReceive=true;
        count=0;
    }
    file_transmit.close();
}
