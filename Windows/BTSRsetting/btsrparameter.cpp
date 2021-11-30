#include "btsrparameter.h"
#include "ui_btsrparameter.h"
#include "Tool/KeyBoard/dialogcalculator.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>

extern QByteArray temp;
BTSRParameter::BTSRParameter(int func, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BTSRParameter)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    function=func;
    sendflag=false;
    path="/xrfile/btsrParameter.txt";
    keyboard_timer = new QTimer(this);
    connect(keyboard_timer,SIGNAL(timeout()),this,SLOT(InputEvent()));
    initialForm();
    keyboard_timer->start(10);
}

BTSRParameter::~BTSRParameter()
{
    delete ui;
}

void BTSRParameter::cellclick(int row, int col)
{
    if(col!=0)
    {
        if(function==2)
        {
            if(ui->tableWidget->item(row,col)->text()==tr("启用"))
            {
                btsr_state[row]=0;
                ui->tableWidget->item(row,col)->setText(tr("禁用"));
            }
            else
            {
                btsr_state[row]=1;
                ui->tableWidget->item(row,col)->setText(tr("启用"));
            }
        }
        else
        {
            DialogCalculator *sr=new DialogCalculator;
            sr->maxValue=100;
            sr->minValue=0;
            if(sr->exec()==QDialog::Accepted)
            {
                btsr_data[row][col-1]=sr->str.toInt();
                ui->tableWidget->item(row,col)->setText(sr->str);
            }
            delete sr;
        }
    }
}

void BTSRParameter::initialForm()
{
    QStringList header;
    header<<tr("编号");
    switch (function) {
    case 0:
        header<<tr("断纱L")<<tr("断纱H")<<tr("走纱L")<<tr("走纱H");
        position=2;
        ui->btn_disable->setVisible(false);
        break;
    case 1:
        header<<tr("延时")<<tr("灵敏度")<<tr("开始频率")<<tr("使用频率");
        position=802;
        ui->btn_disable->setVisible(false);
        break;
    case 2:
        header<<tr("状态");
        position=1602;
        ui->btn_disable->setVisible(true);
        break;
    default:
        break;
    }
    columns=header.length();
    ui->tableWidget->setColumnCount(columns);

    if(function==2)
    {
        ui->tableWidget->horizontalHeader()->resizeSection(0,550); //设置列的宽度为60
    }
    else
    {
        for(int i=0;i<header.length();i++)
        {
//            ui->tableWidget->horizontalHeader()->resizeSection(i,143); //设置列的宽度为60
//            ui->tableWidget->setRowHeight(i,40);//设置列高
        }
        ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//设置表头列等宽
    }
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(35); //设置行高
    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget->setShowGrid(true); //设置不显示格子线
    ui->tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;font-size:18px;border:1px solid white;}"); //设置表头背景色

    ui->tableWidget->setStyleSheet("QScrollBar{background:white; width: 18px;}"
                                   "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:0px;}"
                                   "QScrollBar::handle:hover{background:gray;}"
                                   "QScrollBar::sub-line{background:transparent;}"
                                   "QScrollBar::add-line{background:transparent;}"
                                   "QTableWidget{font-size:18px;}"
                                   "QTableWidget{selection-background-color:#4d93fd;"
                                   "color:black;"
                                   "background-color:white;"
                                   "gridline-color: lightgray;"
                                   "selection-color:white;"
                                   "font-size:18px;}");
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(cellclick(int,int)));
    readFromFile();
}

//从文件读取参数
void BTSRParameter::readFromFile()
{
    QFile file(path);
    if(file.open(QIODevice::ReadWrite))
    {
        QDataStream out(&file);
        out>>btsr_num;//读取电子眼个数
        out.device()->seek(position);//设置文件读取位置
        if(function==2)//感应器禁用界面数据读取到btsr_state中
        {
            for(int i=0;i<btsr_num;i++)
            {
                out>>btsr_state[i];
            }
        }
        else
        {
            for(int i=0;i<btsr_num;i++)
            {
                for(int j=0;j<4;j++)
                    out>>btsr_data[i][j];
            }
        }
        file.close();
    }
    ui->tableWidget->setRowCount(btsr_num);//根据电子眼个数设置表格行数
    for(int i=0;i<btsr_num;i++)
    {
        for(int j=0;j<columns;j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            if(j==0)
            {
                item->setText(QString("%1").arg(i+1));
                item->setFlags(Qt::NoItemFlags);
            }
            else
            {
                if(function==2)//感应器禁用界面 参数显示为启用(1)和禁用(0)
                {
                    if(btsr_state[i]==1)
                        item->setText(QString("%1").arg(tr("启用")));
                    else
                        item->setText(QString("%1").arg(tr("禁用")));
                }
                else
                    item->setText(QString("%1").arg(btsr_data[i][j-1]));
            }
            ui->tableWidget->setItem(i,j,item);
        }
    }
}


void BTSRParameter::sendReceive(QString str)
{
    sendflag=false;
    ui->btn_ok->setStyleSheet("border-radius:10;background-color: rgb(100, 100, 100);font-size:18px;color:rgb(255, 0, 0);");
    ui->btn_ok->setText(str);
    QTimer::singleShot(1000,this,SLOT(ButtonVisible()));
}

//确定 先保存数据到文件再传递参数
void BTSRParameter::on_btn_ok_clicked()
{
    QFile file(path);
    if(file.open(QIODevice::ReadWrite))
    {
        QDataStream out(&file);
        out.device()->seek(position);
        for(int i=0;i<btsr_num;i++)
        {
            if(function==2)
                out<<btsr_state[i];
            else
            {
                for(int j=0;j<4;j++)
                    out<<btsr_data[i][j];
            }
        }
        file.close();
        system("sync");
        Ram.SendData[0]=0x70;
        Ram.SendData[1]=0x02;
        Ram.SendData[2]=0x50;
        Ram.SendData[3]=quint8(function);
        Ram.SendData[4]=0x01;//总包数L
        Ram.SendData[5]=0x00;//总包数H
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
        count=0;
        sendflag=true;
    }
}

//返回
void BTSRParameter::on_btn_back_clicked()
{
    this->close();
}

//禁用所有传感器
void BTSRParameter::on_btn_disable_clicked()
{
    if(function==2)
    {
        for(int i=0;i<btsr_num;i++)
        {
            btsr_state[i]=0;
            ui->tableWidget->item(i,1)->setText(tr("禁用"));
        }
    }
}

//确定按钮状态回归
void BTSRParameter::ButtonVisible()
{
    ui->btn_ok->setStyleSheet("border-radius:10;background-color: rgb(100, 100, 100);font-size:22px;color:rgb(255, 255, 0);");
    ui->btn_ok->setText(tr("确\n定"));
}

void BTSRParameter::InputEvent()
{
    if(temp.length()>=1&&this->isActiveWindow())
    {
        if(temp[0]==(char)10)
        {
            ui->btn_back->click();
        }
        else if(temp[0]==char(9))
            ui->btn_ok->click();
        else if(temp[0]==char(8))
            ui->btn_disable->click();

        temp[0]=255;
    }
    if(sendflag)
    {
        count++;
        if(count>=200)
        {
            sendReceive("传\n输\n超\n时");
        }
        if(Ram.Receive(0x300)==0x55)
        {
            if(Ram.Receive(0x200)==0x70)
            {
                if(Ram.Receive(0x201)==0x02)
                {
                    if(Ram.Receive(0x202)==0x52)
                    {
                        if(Ram.Receive(0x203)==char(function))
                        {
                            Ram.Send(0xAA,0x300);
                            for(int i=0;i<2000;i++)
                            {

                            }
                            int baoshu=Ram.Receive(0x204)+Ram.Receive(0x205)*256;
                            int lastpack=800;
                            if(baoshu==1-1)
                            {
                                lastpack= 800-(1-1)*2560;
                            }
                            QFile file_transmit(path);
                            if(file_transmit.open(QIODevice::ReadWrite))
                            {
                                QDataStream in(&file_transmit);

                                in.device()->seek(position+baoshu*2560);
                                for(int j=0;j<lastpack;j++)
                                {
                                    quint8 s;
                                    in>>s;
                                    Ram.Send(s,0x500+j);
                                }
                                file_transmit.close();
                            }
                            Ram.SendData[0]=0x70;
                            Ram.SendData[1]=0x02;
                            Ram.SendData[2]=0x51;
                            Ram.SendData[3]=quint8(function);
                            Ram.SendData[4]=baoshu%256;//总包数L
                            Ram.SendData[5]=baoshu/256;//总包数H
                            Ram.SendData[6]=lastpack%256;
                            Ram.SendData[7]=lastpack/256;
                            Ram.SendData[8]=0x00;
                            Ram.SendData[9]=0x00;
                            Ram.UI_to_RTC();
                        }
                    }
                }
            }
        }
        else if((unsigned char)Ram.Receive(0x300)==0xcc)
        {
            sendReceive("下\n载\n完\n成");
        }
        else if((unsigned char)Ram.Receive(0x300)==0xEE)
        {
            sendReceive("下\n载\n失\n败");
        }
    }
}

