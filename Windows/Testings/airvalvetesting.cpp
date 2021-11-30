#include "airvalvetesting.h"
#include "ui_airvalvetesting.h"

extern QByteArray temp;
AirValveTesting::AirValveTesting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AirValveTesting)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    autoType=noneAuto;
    readInit =true;
    times=0;
    for(int i =0;i<11;i++)
    {
        for(int j=0;j<32;j++)
        {
            initValve[i][j]=0;
        }
    }

    ui->shuttlegroup_Val_pb->hide();
    testSpeed=1;
    ui->speed_lb->setText(QString::number(testSpeed));

    /*辅助判断变量*/
    click=new QToolButton();
    buttun=new QPushButton();

    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutslot()));//timeoutslot()为自定义槽
    timer->start(10);
    initialValve();
}
/*析构函数*/
AirValveTesting::~AirValveTesting()
{
    delete ui;
}
/*8路气阀连接槽*/
void AirValveTesting::deal()
{
    buttun=(QPushButton *)sender();
    currentValveNum=buttun->toolTip().toInt()-1;
    chooseValveNum(currentValveNum);
}

/*刷新槽，暂时不用*/
void AirValveTesting::timeoutslot()
{
    //进入界面读取当前气阀状态
    if(readInit)
    {
        if(Ram.Receive(0x300) == 0x55)
        {
            if(Ram.Receive(0x200) == 0x11)
            {
                if(Ram.Receive(0x201) == 0xA0)
                {
                    if(Ram.Receive(0x202) == 0x02)
                    {
                        if(Ram.Receive(0x203) == 0xAA)
                        {
                            readInit = false;
                            Ram.Send(0xAA,0x300);
                            for(int i = 0;i<31;i++)
                            {
                                readValve[i] = Ram.Receive(0x500+i);
                            }

                            for(int j=0;j<16;j++)//0 1 2 3 4 5 6 7 ...
                            {
                                for(int i=0;i<8;i++)
                                {
                                    if(j%2 == 0)
                                    {
                                        initValve[j/2][i] = (readValve[j]>>i)&0x01;
                                    }
                                    else
                                    {
                                        initValve[j/2][i+8] = (readValve[j]>>i)&0x01;
                                    }

                                }
                            }

                            //8路梭子组后8个点
                            for(int j =0;j<8;j++)
                            {
                                for(int i=0;i<8;i++)
                                {

                                    initValve[j][i+15] = (readValve[j+16]>>i)&0x01;
                                }
                            }
                            //其他气阀1
                            for(int j=0;j<3;j++)
                            {
                                for(int i=0;i<8;i++)
                                {
                                    initValve[8][i+8*j] = (readValve[j+25]>>i)&0x01;
                                }
                            }
                            //其他气阀2
                            for(int j=0;j<3;j++)
                            {
                                for(int i=0;i<8;i++)
                                {
                                    initValve[9][i+8*j] = (readValve[j+28]>>i)&0x01;
                                }
                            }
                            //其他气阀3
                            for(int i=0;i<8;i++)
                            {
                                initValve[10][i] = (readValve[24]>>i)&0x01;
                            }

                            for(int i=0;i<11;i++)
                            {
                                for(int j=0;j<24;j++)
                                {
                                    valveStatus[i][j]=initValve[i][j];
                                }
                            }
                            refreshValve();
                        }
                    }
                }
            }
        }
    }
    //单个自动
    if(autoType==singleAuto)
    {
        times++;
        if(times%((11-testSpeed)*10) == 0)
        {
            quint8 lastIndex=currentValveIndex[currentValveNum];
            valveSwitchClicked(lastIndex, lastIndex);
            times=0;
        }
    }
    //成组自动
    else if(autoType==groupAuto)
    {
        times++;
        if(times%((11-testSpeed)*10) == 0)
        {
            on_singlemanual_Val_pb_clicked();
            times=0;
        }
    }
    //整体自动
    else if(autoType==wholeAuto)
    {
        times++;
        if(times%((11-testSpeed)*10) == 0)
        {
            if(currentValveNum==10)
               currentValveNum=10 ;
            if(manualCount==2&&currentValveIndex[currentValveNum]==ValveNum[currentValveNum]-1)
            {
                manualCount=0;
                currentValveIndex[currentValveNum]=0;
                if(currentValveNum<10)
                    currentValveNum++;
                else
                    currentValveNum=0;
                chooseValveNum(currentValveNum);
            }
            on_singlemanual_Val_pb_clicked();
            times=0;
        }
    }

    //键盘
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        quint8 key=temp[0];
        temp.clear();
        if(key == 31)
        {

        }
        else if(key==10)
        {
            on_back_Val_pb_clicked();
        }
        else if(key==1)
        {
            ui->singlemanual_Val_pb->click();
        }
        else if(key==2)
        {
            ui->singleauto_Val_pb->click();
        }
        else if(key==3)
        {
            ui->groupauto_Val_pb->click();
        }

        else if(key==4)
        {
            ui->wholeauto_Val_pb->click();
        }

        else if(key==5)
        {
            ui->shuttlegroup_Val_pb->click();
        }

        else if(key==6)
        {
            ui->speedup_Val_pb->click();
        }

        else if(key==7)
        {
            ui->slowdown_Val_pb->click();
        }

        else if(key==28)
        {
            ui->back_Val_pb->click();
        }
        else if(key==38)
        {
            if(currentValveNum>0)
            {
                currentValveNum--;
                chooseValveNum(currentValveNum);
            }
        }
        else if(key==40)
        {
            if(currentValveNum<8)
            {
                currentValveNum++;
                chooseValveNum(currentValveNum);
            }
        }
        else if(key==34)//ok
        {
            valveSwitch[click->toolTip().toInt()]->click();
        }
        else if(key==29)//left
        {
            if(currentValveIndex[currentValveNum]>11)
            {
                lastValveIndex=currentValveIndex[currentValveNum];
                currentValveIndex[currentValveNum]-=12;
                refreshValve();
            }
        }
        else if(key==39)//right
        {
            if(currentValveIndex[currentValveNum]+12<ValveNum[currentValveNum])
            {
                lastValveIndex=currentValveIndex[currentValveNum];
                currentValveIndex[currentValveNum]+=12;
                refreshValve();
            }
        }
        else  if(key==33)//up
        {
            if(currentValveIndex[currentValveNum]>0)
            {
                lastValveIndex=currentValveIndex[currentValveNum];
                currentValveIndex[currentValveNum]--;
                refreshValve();
            }
        }
        else if(key==35)//down
        {
            if(currentValveIndex[currentValveNum]<ValveNum[currentValveNum]-1)
            {
                lastValveIndex=currentValveIndex[currentValveNum];
                currentValveIndex[currentValveNum]++;
                refreshValve();
            }
            else
            {
                lastValveIndex=currentValveIndex[currentValveNum];
                currentValveIndex[currentValveNum]=0;
                refreshValve();
            }
        }
    }
}

/*单个自动*/
void AirValveTesting::on_singleauto_Val_pb_clicked()
{
    if(autoType!=singleAuto)
    {
        autoType=singleAuto;
        ui->singlemanual_Val_pb->setEnabled(false);
        ui->groupauto_Val_pb->setEnabled(false);
        ui->wholeauto_Val_pb->setEnabled(false);
        ui->shuttlegroup_Val_pb->setEnabled(false);
        ui->info_pb->setText(tr("单个自动..."));

    }
    else
    {
        autoType=noneAuto;
        ui->singlemanual_Val_pb->setEnabled(true);
        ui->groupauto_Val_pb->setEnabled(true);
        ui->wholeauto_Val_pb->setEnabled(true);
        ui->shuttlegroup_Val_pb->setEnabled(true);
        ui->info_pb->setText(tr(""));
    }
}

//成组自动
void AirValveTesting::on_groupauto_Val_pb_clicked()
{
    if(autoType!=groupAuto)
    {
        times=0;
        manualCount=0;
        autoType=groupAuto;
        ui->singleauto_Val_pb->setEnabled(false);
        ui->singlemanual_Val_pb->setEnabled(false);
        ui->wholeauto_Val_pb->setEnabled(false);
        ui->shuttlegroup_Val_pb->setEnabled(false);
        ui->info_pb->setText(tr("成组自动..."));
    }
    else
    {
        autoType=noneAuto;
        ui->singleauto_Val_pb->setEnabled(true);
        ui->singlemanual_Val_pb->setEnabled(true);
        ui->wholeauto_Val_pb->setEnabled(true);
        ui->shuttlegroup_Val_pb->setEnabled(true);
        ui->info_pb->setText(tr(""));
    }
}

//整体自动
void AirValveTesting::on_wholeauto_Val_pb_clicked()
{
    if(autoType!=wholeAuto)
    {
        times=0;
        manualCount=0;
        for(int i=0;i<9;i++)
        {
            currentValveIndex[i]=0;
        }
        refreshValve();
        autoType=wholeAuto;
        ui->singleauto_Val_pb->setEnabled(false);
        ui->singlemanual_Val_pb->setEnabled(false);
        ui->shuttlegroup_Val_pb->setEnabled(false);
        ui->groupauto_Val_pb->setEnabled(false);
        ui->info_pb->setText(tr("整体自动..."));
    }
    else
    {
        autoType=noneAuto;
        ui->singleauto_Val_pb->setEnabled(true);
        ui->singlemanual_Val_pb->setEnabled(true);
        ui->shuttlegroup_Val_pb->setEnabled(true);
        ui->groupauto_Val_pb->setEnabled(true);
        ui->info_pb->setText(tr(""));
    }
}

/*单路手动*/
void AirValveTesting::on_singlemanual_Val_pb_clicked()
{
    quint8 lastIndex=currentValveIndex[currentValveNum];
    if(manualCount==2)
    {
        manualCount=0;
        currentValveIndex[currentValveNum]++;
        if(currentValveIndex[currentValveNum]==ValveNum[currentValveNum])
            currentValveIndex[currentValveNum]=0;
    }
    valveSwitchClicked(lastIndex, currentValveIndex[currentValveNum]);
    manualCount++;
}

/*加速*/
void AirValveTesting::on_speedup_Val_pb_clicked()
{
    if(testSpeed<10)
    {
        testSpeed++;
        ui->speed_lb->setText(QString::number(testSpeed));
    }
}

/*减速*/
void AirValveTesting::on_slowdown_Val_pb_clicked()
{
    if(testSpeed>1)
    {
        testSpeed--;
        ui->speed_lb->setText(QString::number(testSpeed));
    }
}

/*返回回到编织状态*/
void AirValveTesting::on_back_Val_pb_clicked()
{
    timer->stop();
    Ram.SendData[0]=0x11;
    Ram.SendData[1]=0xAA;
    Ram.SendData[2]=0x00;
    Ram.SendData[3]=0x00;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    this->close();
}


void AirValveTesting::valveSwitchClicked()
{
    click=(QToolButton *)sender();
    quint8 curIndex=click->toolTip().toInt();
    valveSwitchClicked(currentValveIndex[currentValveNum],curIndex);
}

void AirValveTesting::valveNameClicked()
{
    QPushButton *btn=(QPushButton*)sender();
    quint8 curIndex=btn->toolTip().toInt();
    click=valveSwitch[curIndex];
    valveSwitchClicked(currentValveIndex[currentValveNum],curIndex);
}

void AirValveTesting::valveSwitchClicked(int lastIndex, int index)
{
    valveName[lastIndex]->setChecked(false);
    currentValveIndex[currentValveNum]=index;
    valveName[index]->setChecked(true);
    valveStatus[currentValveNum][index]^=0x01;
    if(valveStatus[currentValveNum][index]==0)
    {
        valveSwitch[index]->setText(tr("关"));
        valveSwitch[index]->setChecked(false);
    }
    else
    {
        valveSwitch[index]->setText(tr("开"));
        valveSwitch[index]->setChecked(true);
    }
    quint8 banhao=0;
    quint8 qifahao=0;
    if(currentValveNum<8)//一般气阀
    {
        if(index<15)
        {
            banhao=currentValveNum+1;
            qifahao=index+1;
        }
        else
        {
            banhao=currentValveNum/3+9;
            qifahao=(currentValveNum%3)*8+index-14;
        }
    }
    else if(currentValveNum==10)//其他气阀
    {
        banhao=11;
        qifahao=index+17;
    }
    else
    {
        banhao=currentValveNum+4;
        qifahao=index+1;
    }
    Ram.SendData[0]=0x11;
    Ram.SendData[1]=0xA1;
    Ram.SendData[2]=0x01;
    Ram.SendData[3]=banhao;
    Ram.SendData[4]=qifahao;
    Ram.SendData[5]=valveStatus[currentValveNum][index];
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    qDebug()<<"板号"<<banhao<<"气阀号"<<qifahao<<"状态"<<valveStatus[currentValveNum][index];
}

void AirValveTesting::initialValve()
{
    currentValveNum=0;
    manualCount=0;
    for(int i=0;i<11;i++)
    {
        btn_valve[i]=new QPushButton(ui->widget_2);
        btn_valve[i]->setGeometry(9,9+42*i,100,36);
        btn_valve[i]->setText(QString(tr("第%1路气阀")).arg(i+1));
        if(i>=8)
            btn_valve[i]->setText(tr("其他气阀%1").arg(i-7));
        btn_valve[i]->setToolTip(tr("%1").arg(i+1));
        btn_valve[i]->setCheckable(true);
        connect(btn_valve[i],SIGNAL(clicked(bool)),this,SLOT(deal()));
        currentValveIndex[i]=0;
        //各路气阀状态默认为0（关）
        for(int j=0;j<24;j++)
            valveStatus[i][j]=0;
        //显示的气阀点位数
        if(i<8)
            ValveNum[i]=23;
        else if(i==10)
            ValveNum[i]=6;
        else
            ValveNum[i]=24;
    }
    buttun=btn_valve[0];
    btn_valve[0]->setChecked(true);
    for(int i=0;i<24;i++)
    {
        int d=i/12;
        valveName[i]=new QPushButton(ui->widget);
        valveName[i]->setCheckable(true);
        valveName[i]->setToolTip(tr("%1").arg(i));
        valveName[i]->setGeometry(10+d*285,10+35*(i%12),240,35);
        connect(valveName[i],SIGNAL(clicked(bool)),this,SLOT(valveNameClicked()));

        valveSwitch[i]=new QToolButton(ui->widget);
        valveSwitch[i]->setText(tr("关"));
        valveSwitch[i]->setCheckable(true);
        valveSwitch[i]->setToolTip(tr("%1").arg(i));  // 0 1 2 ... 22
        connect(valveSwitch[i],SIGNAL(clicked()),this,SLOT(valveSwitchClicked()));
        valveSwitch[i]->setGeometry(250+d*285,10+35*(i%12),30,30);
    }
    refreshValve();
}

void AirValveTesting::refreshValve()
{
    QStringList strlist;
    if(currentValveNum<8)
    {
        strlist<<tr("%1路1号纱嘴B位").arg(currentValveNum+1)<<tr("%1路1号纱嘴C位").arg(currentValveNum+1)
              <<tr("%1路2号纱嘴B位").arg(currentValveNum+1)<<tr("%1路2号纱嘴C位").arg(currentValveNum+1)
             <<tr("%1路3号纱嘴C位").arg(currentValveNum+1)<<tr("%1路4号纱嘴B-D位").arg(currentValveNum+1)
            <<tr("%1路4号纱嘴C位").arg(currentValveNum+1)<<tr("%1路5号纱嘴B-D位").arg(currentValveNum+1)
           <<tr("%1路5号纱嘴C位").arg(currentValveNum+1)<<tr("%1路6号纱嘴B-D-F位").arg(currentValveNum+1)
          <<tr("%1路6号纱嘴C位").arg(currentValveNum+1)<<tr("%1路6号纱嘴E位").arg(currentValveNum+1)
         <<tr("%1路7号纱嘴C位").arg(currentValveNum+1)<<tr("%1路8号纱嘴C位").arg(currentValveNum+1)
        <<tr("%1路开针钩").arg(currentValveNum+1);
        if(currentValveNum==0)
        {
            strlist<<tr("%1路退圈全位(D7 05)").arg(currentValveNum+1)
                  <<tr("%1路退圈半位(D7 06)").arg(currentValveNum+1)
                 <<tr("%1路集圈全位(D7 07)").arg(currentValveNum+1)
                 <<tr("%1路集圈半位(D7 08)").arg(currentValveNum+1)
                <<tr("%1路降针全位(D6 07)").arg(currentValveNum+1)
                <<tr("%1路降针半位(D6 08)").arg(currentValveNum+1)
               <<tr("%1路中间针全位(D5 07)").arg(currentValveNum+1)
               <<tr("%1路中间针半位(D5 08)").arg(currentValveNum+1);
        }
        else if(currentValveNum==1)
        {
            strlist<<tr("%1路退圈全位(D7 01)").arg(currentValveNum+1)
                  <<tr("%1路退圈半位(D7 02)").arg(currentValveNum+1)
                 <<tr("%1路集圈全位(D7 03)").arg(currentValveNum+1)
                 <<tr("%1路集圈半位(D7 04)").arg(currentValveNum+1)
                <<tr("%1路降针全位(D6 05)").arg(currentValveNum+1)
                <<tr("%1路降针半位(D6 06)").arg(currentValveNum+1)
               <<tr("%1路中间针全位(D5 05)").arg(currentValveNum+1)
               <<tr("%1路中间针半位(D5 06)").arg(currentValveNum+1);
        }
        else if(currentValveNum==2)
        {
            strlist<<tr("%1路退圈全位(D8 05)").arg(currentValveNum+1)
                  <<tr("%1路退圈半位(D8 06)").arg(currentValveNum+1)
                 <<tr("%1路集圈全位(D8 07)").arg(currentValveNum+1)
                 <<tr("%1路集圈半位(D8 08)").arg(currentValveNum+1)
                <<tr("%1路降针全位(D6 03)").arg(currentValveNum+1)
                <<tr("%1路降针半位(D6 04)").arg(currentValveNum+1)
               <<tr("%1路中间针全位(D5 03)").arg(currentValveNum+1)
               <<tr("%1路中间针半位(D5 04)").arg(currentValveNum+1);
        }
        else if(currentValveNum==3)
        {
            strlist<<tr("%1路退圈全位(D8 01)").arg(currentValveNum+1)
                  <<tr("%1路退圈半位(D8 02)").arg(currentValveNum+1)
                 <<tr("%1路集圈全位(D8 03)").arg(currentValveNum+1)
                 <<tr("%1路集圈半位(D8 04)").arg(currentValveNum+1)
                <<tr("%1路降针全位(D6 01)").arg(currentValveNum+1)
                <<tr("%1路降针半位(D6 02)").arg(currentValveNum+1)
               <<tr("%1路中间针全位(D5 01)").arg(currentValveNum+1)
               <<tr("%1路中间针半位(D5 02)").arg(currentValveNum+1);
        }
        else if(currentValveNum==4)
        {
            strlist<<tr("%1路退圈全位(S7 08)").arg(currentValveNum+1)
                  <<tr("%1路退圈半位(S7 07)").arg(currentValveNum+1)
                 <<tr("%1路集圈全位(S7 06)").arg(currentValveNum+1)
                 <<tr("%1路集圈半位(S7 05)").arg(currentValveNum+1)
                <<tr("%1路降针全位(S6 08)").arg(currentValveNum+1)
                <<tr("%1路降针半位(S6 07)").arg(currentValveNum+1)
               <<tr("%1路中间针全位(S5 08)").arg(currentValveNum+1)
               <<tr("%1路中间针半位(S5 07)").arg(currentValveNum+1);
        }
        else if(currentValveNum==5)
        {
            strlist<<tr("%1路退圈全位(S7 04)").arg(currentValveNum+1)
                  <<tr("%1路退圈半位(S7 03)").arg(currentValveNum+1)
                 <<tr("%1路集圈全位(S7 02)").arg(currentValveNum+1)
                 <<tr("%1路集圈半位(S7 01)").arg(currentValveNum+1)
                <<tr("%1路降针全位(S6 06)").arg(currentValveNum+1)
                <<tr("%1路降针半位(S6 05)").arg(currentValveNum+1)
               <<tr("%1路中间针全位(S5 06)").arg(currentValveNum+1)
               <<tr("%1路中间针半位(S5 05)").arg(currentValveNum+1);
        }
        else if(currentValveNum==6)
        {
            strlist<<tr("%1路退圈全位(S8 08)").arg(currentValveNum+1)
                  <<tr("%1路退圈半位(S8 07)").arg(currentValveNum+1)
                 <<tr("%1路集圈全位(S8 06)").arg(currentValveNum+1)
                 <<tr("%1路集圈半位(S8 05)").arg(currentValveNum+1)
                <<tr("%1路降针全位(S6 04)").arg(currentValveNum+1)
                <<tr("%1路降针半位(S6 03)").arg(currentValveNum+1)
               <<tr("%1路中间针全位(S5 04)").arg(currentValveNum+1)
               <<tr("%1路中间针半位(S5 03)").arg(currentValveNum+1);
        }
        else if(currentValveNum==7)
        {
            strlist<<tr("%1路退圈全位(S8 04)").arg(currentValveNum+1)
                  <<tr("%1路退圈半位(S8 03)").arg(currentValveNum+1)
                 <<tr("%1路集圈全位(S8 02)").arg(currentValveNum+1)
                 <<tr("%1路集圈半位(S8 01)").arg(currentValveNum+1)
                <<tr("%1路降针全位(S6 02)").arg(currentValveNum+1)
                <<tr("%1路降针半位(S6 01)").arg(currentValveNum+1)
               <<tr("%1路中间针全位(S5 02)").arg(currentValveNum+1)
               <<tr("%1路中间针半位(S5 01)").arg(currentValveNum+1);
        }
    }
    else if(currentValveNum==8)
    {
        strlist<<tr("1路毛圈(D4 08)")<<tr("2路毛圈(D4 07)")<<tr("3路毛圈(D4 06)")<<tr("4路毛圈(D4 05)")
              <<tr("5路毛圈(D4 04)") <<tr("6路毛圈(D4 03)")<<tr("7路毛圈(D4 02)")<<tr("8路毛圈(D4 01)")
             <<tr("1路探针舌(D9 05)")<<tr("2路探针舌(D9 06)") <<tr("3路探针舌(D9 07)")<<tr("4路探针舌(D9 08)")
            <<tr("5路探针舌(S9 08)")<<tr("6路探针舌(S9 07)")<<tr("7路探针舌(S9 06)")<<tr("8路探针舌(S9 05)")
           <<tr("哈夫针吹气(D11 07)")<<tr("3路生克罩调节(D9 01)")<<tr("------------")<<tr("吹废纱1(S9 03)")
          <<tr("吹废纱2(S9 04)")<<tr("针筒吹气(S9 02)")<<tr("出衣口(S9 01)")<<tr("降哈夫盘(D10 08)");
    }
    else if(currentValveNum==9)
    {
        strlist<<tr("出哈夫针半位(S11 05)")<<tr("出哈夫针全位(S11 06)")<<tr("收哈夫针半位(S11 07)")<<tr("收哈夫针全位(S11 08)")
              <<tr("哈夫针整理三角(D10 01)") <<tr("1路线夹(S10 08)")<<tr("2路线夹(S10 07)")<<tr("2路橡筋夹(D10 02)")
             <<tr("3路线夹(S10 06)")<<tr("4路线夹(S10 05)") <<tr("5路线夹(S10 04)")<<tr("6路线夹(S10 03)")
            <<tr("6路橡筋夹(D10 03)")<<tr("7路线夹(S10 02)")<<tr("8路线夹(S10 01)")<<tr("上加油(D9 03)")
           <<tr("2路剪刀(S11 01)")<<tr("4路剪刀(S11 02)")<<tr("6路剪刀(S11 03)")<<tr("8路剪刀(S11 04)")
          <<tr("升机头(D10 05)")<<tr("降机头(D10 06)")<<tr("2路断针器(D9 04)")<<tr("下加油(D9 02)");
    }
    else if(currentValveNum==10)
    {
        strlist<<tr("1路橡筋夹(D11 08)")<<tr("3路橡筋夹(D11 07)")<<tr("4路橡筋夹(D11 06)")<<tr("5路橡筋夹(D11 05)")
                 <<tr("7路橡筋夹(D11 04)")<<tr("8路橡筋夹(D11 03)");
    }
    for(int i=0;i<24;i++)
    {
        if(i<strlist.length())
        {
            valveName[i]->setText(strlist[i]);
            valveName[i]->setVisible(true);
            if(i==currentValveIndex[currentValveNum])
            {
                valveName[i]->setChecked(true);
                click=valveSwitch[i];
            }
            else
                valveName[i]->setChecked(false);

            valveSwitch[i]->setVisible(true);
            if(valveStatus[currentValveNum][i]==0)
            {
                valveSwitch[i]->setText(tr("关"));
                valveSwitch[i]->setChecked(false);
            }
            else
            {
                valveSwitch[i]->setText(tr("开"));
                valveSwitch[i]->setChecked(true);
            }
        }
        else
        {
            valveName[i]->setVisible(false);
            valveSwitch[i]->setVisible(false);
        }
    }
}

void AirValveTesting::chooseValveNum(int num)
{
    for(int i=0;i<11;i++)
    {
        if(i==num)
            btn_valve[i]->setChecked(true);
        else
            btn_valve[i]->setChecked(false);
    }
    refreshValve();
}


void AirValveTesting::on_shuttlegroup_Val_pb_clicked()
{

}
