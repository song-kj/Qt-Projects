#include "modifyco.h"
#include "ui_modifyco.h"
#include <QDebug>
#include <QListView>
#include "myheader.h"
#include "mymessagebox.h"

extern QByteArray temp;

ModifyCO::ModifyCO(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyCO)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    type=0;
    if(projectCOName!="")
    {
        filename=Path_loacl+projectCOName;
        if(!readcodata(filename))
        {
            speedinfo.clear();
            ecoinfo.clear();
            motorInfo.clear();
            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("打开CO文件出错，请检查文件！"),0);
            box->exec();
        }
        else
        {
            bool b=false;
            //写入速度信息
            for(int i=0;i<speedinfo.length();i++)
            {
                if(i<70)
                {
                    if(speedinfo[i].step!=machine_Pars.speed_step[i])
                        b=true;
                    if(speedinfo[i].value!=machine_Pars.speed_value[i])
                        b=true;
                    if(speedinfo[i].ramp!=machine_Pars.speed_ramp[i])
                        b=true;
                    if(b)
                        break;
                }
            }
            if(b)
            {
                MyMessageBox *box = new MyMessageBox(tr("提示"),tr("速度参数不一致！"),0);
                box->exec();
            }
            b=false;
            //写入速度信息
            for(int i=0;i<ecoinfo.length();i++)
            {
                if(i<50)
                {
                    if(ecoinfo[i].step!=machine_Pars.eco_stepstart[i])
                        b=true;
                    if(ecoinfo[i].step!=machine_Pars.eco_stepend[i])
                        b=true;
                    if(ecoinfo[i].value!=machine_Pars.eco_value[i])
                        b=true;
                    if(b)
                        break;
                }
            }
            if(b)
            {
                MyMessageBox *box = new MyMessageBox(tr("提示"),tr("循环参数不一致！"),0);
                box->exec();
            }
        }
    }
    else{
        MyMessageBox *box = new MyMessageBox(tr("提示"),tr("打开CO文件出错，请检查文件！"),0);
        box->exec();
    }
    initialForm();
    timer1=new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(timeout()));
    timer1->start(20);
}

ModifyCO::~ModifyCO()
{
    delete ui;
}

void ModifyCO::settype(int t)
{
    type=t;
    ui->comboBox->hide();
    refreshCOdata();
}

void ModifyCO::initialForm()
{
    ui->comboBox->setView(new QListView());
    ui->tableWidget->horizontalHeader()->setFixedHeight(35); //设置表头的高度
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40); //设置行高
    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget->setShowGrid(true); //设置不显示格子线
    ui->tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);//设置奇偶行颜色不同
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{color:white;background-color:#34495e;"
                                                       "font-size:24px;border:0px solid white;"
                                                       "border-width:0px 0px 0px 0px;}"); //设置表头背景色

    ui->tableWidget->setStyleSheet("QScrollBar{background:#ffffff; width: 28px;margin: 28px 0 28px 0;}"
                                   "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:0px;}"
                                   "QScrollBar::handle:hover{background:gray;}"
                                   "QScrollBar::handle:vertical{min-height:28px;}"
                                   "QScrollBar::sub-line{background:#ffffff;image: url(:/img/top-line.png);height:28px;subcontrol-position: top;subcontrol-origin: margin;}"
                                   "QScrollBar::sub-line:hover{image: url(:/img/top-line-hover.png);}"
                                   "QScrollBar::sub-line:pressed{image: url(:/img/top-line-pressed.png);}"
                                   "QScrollBar::add-line{background:#ffffff;image: url(:/img/bottom-line.png);height:28px;subcontrol-position: bottom;subcontrol-origin: margin;}"
                                   "QScrollBar::add-line:hover{image: url(:/img/bottom-line-hover.png);}"
                                   "QScrollBar::add-line:pressed{image: url(:/img/bottom-line-pressed.png);}"
                                   "QTableWidget{selection-background-color:#4d93fd;"
                                   "color:black;"
                                   "gridline-color: gray;"
                                   "selection-color:white;"
                                   "font-size:24px;"
                                   "background-color:#ffffff;"
                                   "alternate-background-color:#e8f5fd;}");

    refreshCOdata();
}

void ModifyCO::refreshCOdata()
{
    ui->tableWidget->clear();
    //设置表格行标题
    QStringList headerLabels;
    if(type==0)
    {
        headerLabels<<tr("序号")<<tr("步段")<<tr("循环数");
        ui->tableWidget->setColumnCount(headerLabels.length());
        ui->tableWidget->horizontalHeader()->resizeSection(0,110);
        ui->tableWidget->horizontalHeader()->resizeSection(1,300);
    }
    else if(type==1)
    {
        headerLabels<<tr("步段")<<tr("速度值")<<tr("渐变圈");
        ui->tableWidget->setColumnCount(headerLabels.length());
        ui->tableWidget->horizontalHeader()->resizeSection(0,110);
        ui->tableWidget->horizontalHeader()->resizeSection(1,300);
    }
    else if(type==2)
    {
        headerLabels<<tr("步段")<<tr("角度")<<tr("马达号")<<tr("数值 [模式]");
        ui->tableWidget->setColumnCount(headerLabels.length());
        ui->tableWidget->horizontalHeader()->resizeSection(0,120);
        ui->tableWidget->horizontalHeader()->resizeSection(1,120);
        ui->tableWidget->horizontalHeader()->resizeSection(2,120);
    }
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

    if(type==0)
    {
        ui->tableWidget->setRowCount(ecoinfo.length());
        for(int i=0;i<ecoinfo.length();i++)
        {
            for(int j=0;j<headerLabels.length();j++)
            {
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item->setText("");
                if(j==0)
                {
                    item->setFlags(Qt::NoItemFlags);
                    item->setText(QString::number(i+1));
                }
                else if(j==1)
                {
                    item->setFlags(Qt::NoItemFlags);
                    item->setText(QString("%1").arg(ecoinfo[i].step));
                }
                else {
                    item->setText(QString::number(ecoinfo[i].value));
                }
                ui->tableWidget->setItem(i,j,item);
            }
        }

    }
    else if(type==1)
    {
        ui->tableWidget->setRowCount(speedinfo.length());
        for(int i=0;i<speedinfo.length();i++)
        {
            for(int j=0;j<headerLabels.length();j++)
            {
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item->setText("");
                if(j==0)
                {
                    item->setFlags(Qt::NoItemFlags);
//                    item->setText(QString::number(i+1));
                    item->setText(QString("%1").arg(speedinfo[i].step));
                }
                else if(j==1)
                {
//                    item->setFlags(Qt::NoItemFlags);
                    item->setText(QString("%1").arg(speedinfo[i].value));
                }
                else {
                    item->setText(QString::number(speedinfo[i].ramp));
                }
                ui->tableWidget->setItem(i,j,item);
            }
        }
    }
    else if(type==2)
    {
        ui->tableWidget->setRowCount(0);
        for(int i=0;i<motorInfo.length();i++)
        {
            for(int j=0;j<motorInfo[i].listMotor.length();j++)
            {
                if(motorInfo[i].listMotor[j].number<8)
                {
                    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
                    QTableWidgetItem *item = new QTableWidgetItem();
                    item->setTextAlignment(Qt::AlignCenter);
                    item->setText(QString::number(motorInfo[i].step));
                    item->setFlags(Qt::NoItemFlags);

                    QTableWidgetItem *item4=new QTableWidgetItem;
                    item4->setTextAlignment(Qt::AlignCenter);
                    item4->setText(QString::number(motorInfo[i].listMotor[j].degree));
                    item4->setFlags(Qt::NoItemFlags);

                    QTableWidgetItem *item1 = new QTableWidgetItem();
                    item1->setTextAlignment(Qt::AlignCenter);
                    item1->setFlags(Qt::NoItemFlags);
                    QString str=getTypeString(motorInfo[i].listMotor[j].type,motorInfo[i].listMotor[j].position);
//                    item1->setText(QString("%1 [%2]").arg(motorInfo[i].listMotor[j].number+1).arg(str));
                    item1->setText(QString::number((motorInfo[i].listMotor[j].number+1)));
                    QTableWidgetItem *item2 = new QTableWidgetItem();
                    item2->setTextAlignment(Qt::AlignCenter);
                    item2->setText(QString("%1 [%2]").arg(motorInfo[i].listMotor[j].position).arg(str));
                    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,item);
                    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,item4);
                    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,item1);
                    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,item2);
                }
            }
        }
    }
}

bool ModifyCO::readcodata(QString filename)
{
    QFile file(filename);
    if(file.open(QIODevice::ReadWrite))
    {
        QByteArray data;
        int count=0;
        bool rst=true;
        /**
         * --------------------提取速度数据-----------------------
   */
        {
            file.seek(0x8e);
            data=file.read(4);
            int offset=0;
            int index=0;
            //起始地址
            offset=(((int)data[1]&0xff)+((int)data[0]&0xff))<<8;
            if(data[1]!=0)
                offset=(((int)data[1]&0xff)<<16)+(((int)data[0]&0xff)<<8);
            offset+=0x40;
            file.seek(offset);
            offsetAddr[0]=offset;
            data=file.read(4);
            SpeedANDEco se;
            //步骤号为 FF FF FF FF时数据结束
            while(!(((int)data[0]&0xff) == 0xff && ((int)data[1]&0xff) == 0xff && ((int)data[2]&0xff) == 0xff && ((int)data[3]&0xff) == 0xff))
            {
                if(count>200)
                {
                    rst=false;
                    break;
                }
                se.step=((quint8)data[1])*256+(quint8)data[0];
                data=file.read(4);
                se.value=((quint8)data[1])*256+(quint8)data[0];
                data=file.read(4);
                data=file.read(4);
                se.ramp=((quint8)data[1])*256+(quint8)data[0];
                speedinfo.append(se);
                index++;
                file.seek(offset+index*44);
                data=file.read(4);
                count++;
            }
        }
        if(!rst)
        {
            file.close();
            return rst;
        }
        /**
         * --------------------提取循环数据-----------------------
   */
        {
            count=0;
            file.seek(0x129);
            QByteArray data=file.read(4);
            //起始地址
            int offset=(((int)data[1]&0xff)+((int)data[0]&0xff))<<8;
            if(data[1]!=0)
                offset=(((int)data[1]&0xff)<<16)+(((int)data[0]&0xff)<<8);
            file.seek(offset+0x24);
            data=file.read(4);
            offset+=(((quint8)data[1])*256+(quint8)data[0]);
            offsetAddr[1]=offset;
            file.seek(offset);
            data=file.read(4);
            int index=0;
            SpeedANDEco se;
            //步骤号为 00 00 00 00 时数据结束
            while(!(((int)data[0]&0xff) == 0 && ((int)data[1]&0xff) == 0 && ((int)data[2]&0xff) == 0 && ((int)data[3]&0xff) == 0))
            {
                if(count>200)
                {
                    rst=false;
                    break;
                }
                se.step=((quint8)data[1])*256+(quint8)data[0];
                data=file.read(4);
                se.value=((quint8)data[1])*256+(quint8)data[0];
                ecoinfo.append(se);
                index++;
                file.seek(offset+index*36);
                data=file.read(4);
                count++;
            }
        }

        if(!rst)
        {
            file.close();
            return rst;
        }
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
                    rst=false;
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
                    motorInfo[step].listMotor.append(mi);
                }
                data=file.read(2);
                degree=((quint8)data[0]&0xff)+((((quint8)data[1]&0xff))<<8);
                count++;
            }
        }
        file.close();
        return rst;
    }
}

void ModifyCO::savecodata(QString filename)
{
    quint16 speedstep[70];
    quint16 speedvalue[70];
    quint16 speedramp[70];

    quint16 ecosteps[50];
    quint16 ecostepe[50];
    quint16 ecovalue[50];
    for(int i=0;i<70;i++)
    {
        speedstep[i]=0;
        speedvalue[i]=0;
        speedramp[i]=0;
    }
    for(int i=0;i<50;i++)
    {
        ecostepe[i]=0;
        ecosteps[i]=0;
        ecovalue[i]=0;
    }
    QFile file(filename);
    if(file.open(QIODevice::ReadWrite))
    {
        QDataStream in(&file);
        //写入速度信息
        for(int i=0;i<speedinfo.length();i++)
        {
            speedstep[i]=speedinfo[i].step;
            speedvalue[i]=speedinfo[i].value;
            speedramp[i]=speedinfo[i].ramp;

            in.device()->seek(offsetAddr[0]+44*i);
            in<<(quint8)(speedinfo[i].step);
            in<<(quint8)(speedinfo[i].step>>8);
            in.device()->seek(offsetAddr[0]+44*i+4);
            in<<(quint8)(speedinfo[i].value);
            in<<(quint8)(speedinfo[i].value>>8);
            in.device()->seek(offsetAddr[0]+44*i+12);
            in<<(quint8)(speedinfo[i].ramp);
            in<<(quint8)(speedinfo[i].ramp>>8);
        }
        //写入循环信息
        for(int i=0;i<ecoinfo.length();i++)
        {
            ecosteps[i]=ecoinfo[i].step;
            ecostepe[i]=ecoinfo[i].step;
            ecovalue[i]=ecoinfo[i].value;
            in.device()->seek(offsetAddr[1]+36*i);
            in<<(quint8)(ecoinfo[i].step);
            in<<(quint8)(ecoinfo[i].step>>8);
            in.device()->seek(offsetAddr[1]+36*i+4);
            in<<(quint8)(ecoinfo[i].value);
            in<<(quint8)(ecoinfo[i].value>>8);
        }

        //写入链条压针数据
        file.seek(offsetAddr[2]+0x40+totalStep*4+4);
        quint16 end=0x0080;

        for(int i=0;i<motorInfo.length();i++)
        {
            for(int j=0;j<motorInfo[i].listMotor.length();j++)
            {
                in<<(quint8)motorInfo[i].listMotor[j].degree;
                in<<(quint8)(motorInfo[i].listMotor[j].degree>>8);
                in<<(quint8)motorInfo[i].listMotor[j].number;
                in<<(quint8)(motorInfo[i].listMotor[j].number>>8);
                in<<(quint8)motorInfo[i].listMotor[j].type;
                in<<(quint8)(motorInfo[i].listMotor[j].type>>8);
                in<<(quint8)motorInfo[i].listMotor[j].position;
                in<<(quint8)(motorInfo[i].listMotor[j].position>>8);
            }
            in<<end;
        }
        file.close();
        system(("sync"));
    }

    //保存链条速度和循环到系统参数文件
    for(int i=0;i<70;i++)
    {
        machine_Pars.speed_step[i]=speedstep[i];
        machine_Pars.speed_value[i]=speedvalue[i];
        machine_Pars.speed_ramp[i]=speedramp[i];
    }
    for(int i=0;i<50;i++)
    {
        machine_Pars.eco_stepstart[i]=ecostepe[i];
        machine_Pars.eco_stepend[i]=ecosteps[i];
        machine_Pars.eco_value[i]=ecovalue[i];
    }
    //初始化链条压针参数
    for(int i=0;i<200;i++)
    {
        machine_Pars.motor_degree[i]=0xffff;
        machine_Pars.motor_number[i]=0xffff;
        machine_Pars.motor_step[i]=0xffff;
        machine_Pars.motor_position[i]=0xffff;
    }
    int rowIndex=0;
    //保存链条压针到系统参数文件
    for(int i=0;i<motorInfo.length();i++)
    {
        for(int j=0;j<motorInfo[i].listMotor.length();j++)
        {
            if(motorInfo[i].listMotor[j].number<8)
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
    para_operation.machine_par_save();
}

quint16 ModifyCO::change2bytesdata(quint16 value)
{

}

quint32 ModifyCO::change4bytesdata(quint32 value)
{

}

QString ModifyCO::getTypeString(int type,quint16 pos)
{
    QString str="";
    switch (type) {
    case 1:
        str="-";
        break;
    case 2:
        str="+";
        break;
    case 3:
        if(pos==0)
            str="0";
        else
            str="P";
        break;
    case 7:
        str="T";
        break;
    case 8:
        str="N";
        break;
    default:
        break;
    }
    return str;
}

void ModifyCO::on_btn_copy2usb_clicked()
{
    savecodata(filename);
    if(type==0)
    {
        QFile file(Path_parameterfile);
        qint16 data_;qint8 DATA[300];
        long unsigned int addr=0x500;
        if(file.open(QIODevice::ReadWrite))
        {
            QDataStream in(&file);
            in.device()->seek(2128);
            for(int pos=0;pos<150;pos++)
            {
                in>>data_;
                DATA[2*pos]=data_&0x00ff;       //14
                DATA[2*pos+1]=(data_&0xff00)>>8;  //15
            }
            file.close();
        }
        for(int m=0;m<300;m++)
        {
            Ram.Send(DATA[m],addr);
            addr++;
        }

        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x09;
        Ram.SendData[3]=0x55;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else if(type==1)
    {
        QFile file(Path_parameterfile);
        qint16 data_;qint8 DATA[420];
        long unsigned int addr=0x500;
        if(file.open(QIODevice::ReadWrite))
        {
            QDataStream in(&file);
            in.device()->seek(1708);
            for(int pos=0;pos<210;pos++)
            {
                in>>data_;
                DATA[2*pos]=data_&0x00ff;       //14
                DATA[2*pos+1]=(data_&0xff00)>>8;  //15
            }
            file.close();
        }
        for(int m=0;m<420;m++)
        {
            Ram.Send(DATA[m],addr);
            addr++;
        }

        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x05;
        Ram.SendData[3]=0x55;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    else if(type==2)
    {
        QFile file(Path_parameterfile);
        qint16 data_;qint8 DATA[1600];
        long unsigned int addr=0x500;
        if(file.open(QIODevice::ReadWrite))
        {
            QDataStream in(&file);
            in.device()->seek(108);
            for(int pos=0;pos<800;pos++)
            {
                in>>data_;
                DATA[2*pos]=data_&0x00ff;       //14
                DATA[2*pos+1]=(data_&0xff00)>>8;  //15
            }
            file.close();
        }
        for(int m=0;m<1600;m++)
        {
            Ram.Send(DATA[m],addr);
            addr++;
        }

        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x04;
        Ram.SendData[3]=0x02;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    ui->btn_copy2usb->setCheckable(true);
    ui->btn_copy2usb->setChecked(true);
    ui->btn_copy2usb->setText(tr("保\n存\n成\n功"));
    QTimer::singleShot(500,this,SLOT(singleslot()));
}

void ModifyCO::on_toolButton_5_clicked()
{
    this->close();
}

void ModifyCO::on_comboBox_currentIndexChanged(int index)
{
    type=index;
    refreshCOdata();
}

//+
void ModifyCO::on_btn_copy2usb_2_clicked()
{
    int rowindex=ui->tableWidget->currentIndex().row();
    int colindex=ui->tableWidget->currentColumn();
    if(rowindex>-1)
    {
        if(type==1)
        {
            if(colindex==1)
            {
                if(speedinfo[rowindex].value<220)
                    speedinfo[rowindex].value++;
                ui->tableWidget->item(rowindex,colindex)->setText(QString::number(speedinfo[rowindex].value));
            }
            else if(colindex==2)
            {
                if(speedinfo[rowindex].ramp<250)
                    speedinfo[rowindex].ramp++;
                ui->tableWidget->item(rowindex,colindex)->setText(QString::number(speedinfo[rowindex].ramp));
            }
        }
        else if(type==0)
        {
            if(ecoinfo[rowindex].value<250)
                ecoinfo[rowindex].value++;
            ui->tableWidget->item(rowindex,2)->setText(QString::number(ecoinfo[rowindex].value));
        }
        else if(type==2)
        {
            //压针值
            quint16 pos=ui->tableWidget->item(rowindex,3)->text().split(' ')[0].toUInt();
            //角度
            quint16 degree=ui->tableWidget->item(rowindex,1)->text().toUInt();
            //步骤号
            quint16 step=ui->tableWidget->item(rowindex,0)->text().toUInt();
            //电机号
            quint16 num=ui->tableWidget->item(rowindex,2)->text().toUInt();

            int index=-1;
            for(int i=0;i<motorInfo[step].listMotor.length();i++)
            {
                if(motorInfo[step].listMotor[i].number==num-1&&motorInfo[step].listMotor[i].degree==degree)
                {
                    index=i;
                    break;
                }
            }
            if(index>-1)
            {
                //类型
                quint16 type=motorInfo[step].listMotor[index].type;
                if(pos<250)
                    pos++;
                motorInfo[step].listMotor[index].position=pos;
                ui->tableWidget->item(rowindex,3)->setText(QString("%1 [%2]").arg(pos).arg(getTypeString(type,pos)));
            }
        }
    }
}

//-
void ModifyCO::on_toolButton_6_clicked()
{
    int rowindex=ui->tableWidget->currentIndex().row();
    int colindex=ui->tableWidget->currentColumn();
    if(rowindex>-1)
    {
        if(type==1)
        {
            if(colindex==1)
            {
                if(speedinfo[rowindex].value>10)
                    speedinfo[rowindex].value--;
                ui->tableWidget->item(rowindex,colindex)->setText(QString::number(speedinfo[rowindex].value));
            }
            else if(colindex==2)
            {
                if(speedinfo[rowindex].ramp>1)
                    speedinfo[rowindex].ramp--;
                ui->tableWidget->item(rowindex,colindex)->setText(QString::number(speedinfo[rowindex].ramp));
            }
        }
        else if(type==0)
        {
            if(ecoinfo[rowindex].value>0)
                ecoinfo[rowindex].value--;
            ui->tableWidget->item(rowindex,2)->setText(QString::number(ecoinfo[rowindex].value));
        }
        else if(type==2)
        {
            //压针值
            int pos=ui->tableWidget->item(rowindex,3)->text().split(' ')[0].toUInt();
            //角度
            quint16 degree=ui->tableWidget->item(rowindex,1)->text().toUInt();
            //步骤号
            quint16 step=ui->tableWidget->item(rowindex,0)->text().toUInt();
            //电机号
            quint16 num=ui->tableWidget->item(rowindex,2)->text().toUInt();

            int index=-1;
            for(int i=0;i<motorInfo[step].listMotor.length();i++)
            {
                if(motorInfo[step].listMotor[i].number==num-1&&motorInfo[step].listMotor[i].degree==degree)
                {
                    index=i;
                    break;
                }
            }
            if(index>-1)
            {
                //类型
                quint16 type=motorInfo[step].listMotor[index].type;
                if(pos<250)
                    pos--;
                if(pos==0)
                {
                    type=3;
                }
                else if(pos<0)
                {
                    if(type==7)
                        pos=0;
                    else
                    {
                        quint16 t=type;
                        if(type==1)
                            t=2;
                        else if(type==2)
                            t=1;
                        else if(type==3)
                            t=8;
                        else if(type==8)
                            t=3;
                        type=t;
                        pos*=-1;
                    }
                }
                motorInfo[step].listMotor[index].type=type;
                motorInfo[step].listMotor[index].position=pos;
                ui->tableWidget->item(rowindex,3)->setText(QString("%1 [%2]").arg(pos).arg(getTypeString(type,pos)));
            }
        }
    }
}

void ModifyCO::timeout()
{
    //按键操作
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)7)
        {
            temp[0]=255;
            ui->btn_copy2usb_2->click();
        }
        else if(temp[0]==(char)8)
        {
            temp[0]=255;
            ui->toolButton_6->click();
        }
        else if(temp[0]==(char)9)
        {
            temp[0]=255;
            ui->btn_copy2usb->click();
        }
        else if(temp[0]==(char)10)
        {
            temp[0]=255;
            ui->toolButton_5->click();
        }
    }
}

void ModifyCO::singleslot()
{
    ui->btn_copy2usb->setCheckable(false);
    ui->btn_copy2usb->setText(tr("保\n存"));
}
